#include "board.h"
volatile int _encoder_l_count=0, _encoder_r_count=0;
void encoder_function2(void)
{
 
    if (DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, ENCODER_Left_A_PIN))
    {
         if(DL_GPIO_readPins(ENCODER_PORT,ENCODER_Left_B_PIN)==0)  
         {
               
              _encoder_l_count++; 
         }
         else                                                                       
         {
               _encoder_l_count--; 
         }
        
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_Left_A_PIN);
    }  


    if (DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, ENCODER_Left_B_PIN))
    {
         if(DL_GPIO_readPins(ENCODER_PORT,ENCODER_Left_A_PIN)==0)  
         {
               
              _encoder_l_count++; 
         }
         else                                                                     
         {
               _encoder_l_count--; 
         }
        
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_Left_B_PIN);
    }
    
    
   
     
    if (DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, ENCODER_Right_A_PIN))
    {
         if(DL_GPIO_readPins(ENCODER_PORT,ENCODER_Right_B_PIN)==0)  
         {
               
              _encoder_r_count--; 
         }
         else                                                                       
         {
               _encoder_r_count++; 
         }
        
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_Right_A_PIN);
    }  


    if (DL_GPIO_getEnabledInterruptStatus(ENCODER_PORT, ENCODER_Right_B_PIN))
    {
         if(DL_GPIO_readPins(ENCODER_PORT,ENCODER_Right_A_PIN)==0)  
         {
               
              _encoder_r_count--; 
         }
         else                                                                     
         {
               _encoder_r_count++; 
         }
        
        DL_GPIO_clearInterruptStatus(ENCODER_PORT, ENCODER_Right_B_PIN);
    }
}


void GROUP1_IRQHandler(void)
{
 if(DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)==DL_INTERRUPT_GROUP1_IIDX_GPIOB){
  /* PB1 shares GROUP1 with encoders. Acknowledge it without blocking I2C. */
  if(DL_GPIO_getEnabledInterruptStatus(MPU6050_PORT,MPU6050_INT_PIN_PIN))
   DL_GPIO_clearInterruptStatus(MPU6050_PORT,MPU6050_INT_PIN_PIN);
  encoder_function2();
 }
}
