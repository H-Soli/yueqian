#include "pid.h"
#include "board.h"
   
float err = 0;               
float last_err = 0;          
float err_sum=0;         
float err_difference = 0;    

float angle_Kp=2.0f,angle_Ki=0,angle_Kd=0.6f;  
float angle_err;               
float angle_last_err;          
float angle_err_sum=0;         
float angle_err_difference;    
float angle_PID_value(float measure,float calcu)
{

	angle_err = measure - calcu;             
	angle_err_sum+=angle_err;
	if (angle_err_sum >  100) angle_err_sum =  100;  
  if (angle_err_sum < -100) angle_err_sum = -100;	


	angle_err_difference = angle_err - angle_last_err;   
	angle_last_err = angle_err;                    
	
	return angle_Kp*angle_err + angle_Ki*angle_err_sum + angle_Kd*angle_err_difference;
}


float place_Kp=6,place_Ki=0.4,place_Kd=6;  
float place_err;               
float place_last_err;          
float place_err_sum=0;         
float place_err_difference;    
float place_PID_value(float measure,float calcu)
{
	place_err = measure - calcu;             
	place_err_sum+=place_err;                      
   place_err_sum = I_xianfu(50,place_err_sum);


	place_err_difference = place_err - place_last_err;   
	place_last_err = place_err;                    
	return place_Kp*place_err + place_Ki*place_err_sum + place_Kd*place_err_difference;

}

float L_moter_Kp=2.0f,L_moter_Ki=0,L_moter_Kd=0.6f;
float L_moter_err;
float L_moter_last_err;
float L_moter_err_sum=0;
float L_moter_err_difference;
float L_moter_pid_value(float measure,float calcu)
{
	L_moter_err = measure - calcu;             
	L_moter_err_sum+=L_moter_err;                      
  L_moter_err_sum = I_xianfu(2000,L_moter_err_sum);
	L_moter_err_difference = L_moter_err - L_moter_last_err;   
	L_moter_last_err = L_moter_err;                    
	return L_moter_Kp*L_moter_err + L_moter_Ki*L_moter_err_sum + L_moter_Kd*L_moter_err_difference;

}
float R_moter_Kp=2.0f,R_moter_Ki=0,R_moter_Kd=0.6f;
float R_moter_err;
float R_moter_last_err; 
float R_moter_err_sum=0;
float R_moter_err_difference;
float R_moter_pid_value(float measure,float calcu)
{
	R_moter_err = measure - calcu;             
	R_moter_err_sum+=R_moter_err;                      
  R_moter_err_sum = I_xianfu(2000,R_moter_err_sum);
	R_moter_err_difference = R_moter_err - R_moter_last_err;   
	R_moter_last_err = R_moter_err;                    
	return R_moter_Kp*R_moter_err + R_moter_Ki*R_moter_err_sum + R_moter_Kd*R_moter_err_difference;

}


float I_xianfu(float max,float now)
{
      if(now > max)  now =  max;
      if(now < -max) now = -max;
   
  return now;
}