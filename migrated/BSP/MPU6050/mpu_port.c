#include "board.h"
#include "mpu_port.h"
#define BUS I2C_0_INST
#define ERROR_FLAGS (DL_I2C_INTERRUPT_CONTROLLER_NACK | DL_I2C_INTERRUPT_CONTROLLER_ARBITRATION_LOST)
#define TRANSFER_FLAGS (ERROR_FLAGS | DL_I2C_INTERRUPT_CONTROLLER_TX_DONE | DL_I2C_INTERRUPT_CONTROLLER_RX_DONE)
volatile uint32_t mpu_bus_errors=0;
volatile unsigned int mpu_bus_fault=0,mpu_scl_level=0,mpu_sda_level=0;
volatile uint32_t mpu_fault_status=0,mpu_fault_irqs=0;
volatile uint8_t mpu_i2c_address=0x68;
volatile unsigned int mpu_fault_phase=0;
volatile uint8_t mpu_idle_levels=0,mpu_pullup_levels=0;
static unsigned int transfer_phase=0;
static uint8_t gpio_levels(void)
{
 return (DL_GPIO_readPins(GPIO_I2C_0_SCL_PORT,GPIO_I2C_0_SCL_PIN)?2U:0U)|
        (DL_GPIO_readPins(GPIO_I2C_0_SDA_PORT,GPIO_I2C_0_SDA_PIN)?1U:0U);
}
void mpu_port_sample_lines(void)
{
 uint32_t scl=IOMUX->SECCFG.PINCM[GPIO_I2C_0_IOMUX_SCL];
 uint32_t sda=IOMUX->SECCFG.PINCM[GPIO_I2C_0_IOMUX_SDA];
 DL_I2C_disableController(BUS);
 /* True GPIO input sampling: never actively drive a high level onto I2C. */
 DL_GPIO_disableOutput(GPIO_I2C_0_SCL_PORT,GPIO_I2C_0_SCL_PIN);
 DL_GPIO_disableOutput(GPIO_I2C_0_SDA_PORT,GPIO_I2C_0_SDA_PIN);
 DL_GPIO_initDigitalInputFeatures(GPIO_I2C_0_IOMUX_SCL,DL_GPIO_INVERSION_DISABLE,
  DL_GPIO_RESISTOR_NONE,DL_GPIO_HYSTERESIS_DISABLE,DL_GPIO_WAKEUP_DISABLE);
 DL_GPIO_initDigitalInputFeatures(GPIO_I2C_0_IOMUX_SDA,DL_GPIO_INVERSION_DISABLE,
  DL_GPIO_RESISTOR_NONE,DL_GPIO_HYSTERESIS_DISABLE,DL_GPIO_WAKEUP_DISABLE);
 delay_us(100);mpu_idle_levels=gpio_levels();
 DL_GPIO_initDigitalInputFeatures(GPIO_I2C_0_IOMUX_SCL,DL_GPIO_INVERSION_DISABLE,
  DL_GPIO_RESISTOR_PULL_UP,DL_GPIO_HYSTERESIS_DISABLE,DL_GPIO_WAKEUP_DISABLE);
 DL_GPIO_initDigitalInputFeatures(GPIO_I2C_0_IOMUX_SDA,DL_GPIO_INVERSION_DISABLE,
  DL_GPIO_RESISTOR_PULL_UP,DL_GPIO_HYSTERESIS_DISABLE,DL_GPIO_WAKEUP_DISABLE);
 delay_us(100);mpu_pullup_levels=gpio_levels();
 IOMUX->SECCFG.PINCM[GPIO_I2C_0_IOMUX_SCL]=scl;
 IOMUX->SECCFG.PINCM[GPIO_I2C_0_IOMUX_SDA]=sda;
 DL_I2C_enableController(BUS);
 mpu_scl_level=(mpu_idle_levels>>1)&1U;mpu_sda_level=mpu_idle_levels&1U;
}
/* Bound both elapsed time and iterations, including when SysTick is faulty. */
static int expired(uint32_t start,uint32_t *budget)
{
 if(*budget==0)return 1;
 --*budget;
 return (uint32_t)(uwTick-start)>=6U;
}
static int failed(uint32_t start,uint32_t *budget)
{
 uint32_t flags=DL_I2C_getRawInterruptStatus(BUS,ERROR_FLAGS);
 if(flags&DL_I2C_INTERRUPT_CONTROLLER_NACK){mpu_bus_fault=2;return 1;}
 if(flags&DL_I2C_INTERRUPT_CONTROLLER_ARBITRATION_LOST){mpu_bus_fault=3;return 1;}
 if(expired(start,budget)){mpu_bus_fault=1;return 1;}
 return 0;
}
static int abort_transfer(void)
{
 mpu_bus_errors++;
 if(!mpu_bus_fault)mpu_bus_fault=4;
 mpu_fault_phase=transfer_phase;
 mpu_fault_status=DL_I2C_getControllerStatus(BUS);
 mpu_fault_irqs=DL_I2C_getRawInterruptStatus(BUS,TRANSFER_FLAGS);
 DL_I2C_reset(BUS);DL_I2C_enablePower(BUS);
 delay_cycles(POWER_STARTUP_DELAY);SYSCFG_DL_I2C_0_init();
 mpu_port_sample_lines();
 return -1;
}
static int prepare(void)
{
 uint32_t start=uwTick,budget=40000;
 mpu_bus_fault=0;
 transfer_phase=1;
 while(!(DL_I2C_getControllerStatus(BUS)&DL_I2C_CONTROLLER_STATUS_IDLE) ||
       (DL_I2C_getControllerStatus(BUS)&DL_I2C_CONTROLLER_STATUS_BUSY_BUS)){
  if(expired(start,&budget)){mpu_bus_fault=5;return abort_transfer();}
 }
 DL_I2C_resetControllerTransfer(BUS);
 transfer_phase=2;
 /* DriverLib's convenience flush routines wait forever. Bound them too. */
 DL_I2C_startFlushControllerTXFIFO(BUS);DL_I2C_startFlushControllerRXFIFO(BUS);
 while(!DL_I2C_isControllerTXFIFOEmpty(BUS) || !DL_I2C_isControllerRXFIFOEmpty(BUS)){
  if(expired(start,&budget)){mpu_bus_fault=6;return abort_transfer();}
 }
 DL_I2C_stopFlushControllerTXFIFO(BUS);DL_I2C_stopFlushControllerRXFIFO(BUS);
 DL_I2C_clearInterruptStatus(BUS,TRANSFER_FLAGS);return 0;
}
int mpu_port_write(uint8_t addr,uint8_t reg,uint8_t len,const uint8_t *data)
{
 uint32_t start,budget=40000;unsigned int sent=0;
 if(addr==0x68)addr=mpu_i2c_address;
 if(!len || !data || prepare())return -1;
 transfer_phase=5;
 start=uwTick;DL_I2C_transmitControllerData(BUS,reg);
 DL_I2C_startControllerTransfer(BUS,addr,DL_I2C_CONTROLLER_DIRECTION_TX,(uint16_t)len+1U);
 while(!DL_I2C_getRawInterruptStatus(BUS,DL_I2C_INTERRUPT_CONTROLLER_TX_DONE)){
  if(failed(start,&budget))return abort_transfer();
  if(sent<len)sent+=DL_I2C_fillControllerTXFIFO(BUS,(uint8_t *)(data+sent),len-sent);
 }
 if(failed(start,&budget) || sent!=len)return abort_transfer();
 transfer_phase=0;
 return 0;
}
int mpu_port_read(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *data)
{
 uint32_t start,budget=40000;unsigned int received=0;
 if(addr==0x68)addr=mpu_i2c_address;
 if(!len || !data || prepare())return -1;
 start=uwTick;
 /* Explicit register-pointer write, followed by a separate read. This
  * avoids relying on the combined RD_ON_TXEMPTY controller sequence.
  * MPU6050 retains its register pointer between these two transfers. */
 transfer_phase=3;
 DL_I2C_transmitControllerData(BUS,reg);
 DL_I2C_startControllerTransfer(BUS,addr,DL_I2C_CONTROLLER_DIRECTION_TX,1);
 while(!DL_I2C_getRawInterruptStatus(BUS,DL_I2C_INTERRUPT_CONTROLLER_TX_DONE))
  if(failed(start,&budget))return abort_transfer();
 if(failed(start,&budget))return abort_transfer();
 while(DL_I2C_getControllerStatus(BUS)&DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
  if(failed(start,&budget))return abort_transfer();
 DL_I2C_resetControllerTransfer(BUS);
 DL_I2C_clearInterruptStatus(BUS,TRANSFER_FLAGS);
 transfer_phase=4;
 DL_I2C_startControllerTransfer(BUS,addr,DL_I2C_CONTROLLER_DIRECTION_RX,len);
 for(;;){
  if(failed(start,&budget))return abort_transfer();
  while(!DL_I2C_isControllerRXFIFOEmpty(BUS)){
   uint8_t value=DL_I2C_receiveControllerData(BUS);
   if(received<len)data[received++]=value;
  }
  if(DL_I2C_getRawInterruptStatus(BUS,DL_I2C_INTERRUPT_CONTROLLER_RX_DONE))break;
 }
 while(!DL_I2C_isControllerRXFIFOEmpty(BUS)){
  uint8_t value=DL_I2C_receiveControllerData(BUS);
  if(received<len)data[received++]=value;
 }
 if(received!=len)return abort_transfer();
 transfer_phase=0;
 return 0;
}
