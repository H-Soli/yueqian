#ifndef MPU_PORT_H
#define MPU_PORT_H
#include <stdint.h>
int mpu_port_read(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *data);
int mpu_port_write(uint8_t addr,uint8_t reg,uint8_t len,const uint8_t *data);
void mpu_request_fifo_reset(void);
extern volatile uint32_t mpu_bus_errors;
/* B: 0=OK, 1=timeout, 2=NACK, 3=arbitration, 4=short transfer,
 *    5=bus idle timeout, 6=FIFO flush timeout. */
extern volatile unsigned int mpu_bus_fault,mpu_scl_level,mpu_sda_level;
extern volatile uint32_t mpu_fault_status,mpu_fault_irqs;
/* The archived driver uses 0x68 internally; the port redirects it if AD0=1. */
extern volatile uint8_t mpu_i2c_address;
/* Error snapshots: phase 1=idle, 2=flush, 3=register TX, 4=data RX, 5=data TX.
 * GPIO levels are measured with the peripheral disconnected, not via I2C mux. */
extern volatile unsigned int mpu_fault_phase;
extern volatile uint8_t mpu_idle_levels,mpu_pullup_levels;
void mpu_port_sample_lines(void);
#endif
