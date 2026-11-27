#ifndef __PID_H
#define __PID_H

//#ifndef PID_CONTROLLER_H
//#define PID_CONTROLLER_H
// PID锟结构锟藉（锟斤拷锟侥ｏ拷
typedef struct {
    float Kp, Ki, Kd;      // PID系锟斤拷
    float err_sun;        // 锟斤拷锟斤拷锟桔伙拷
    float last_error;      // 锟较达拷锟斤拷睿拷锟斤拷锟轿拷郑锟?
    float max_err_sun;    // 锟斤拷锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟酵ｏ拷
    float max_output;      // 锟斤拷锟斤拷薹锟?
} PID_Controller;
//#endif // PID_CONTROLLER_H

float I_xianfu(float max,float now);
float angle_PID_value(float measure,float calcu);
float place_PID_value(float measure,float calcu);//循锟斤拷锟矫碉拷位锟矫伙拷
float L_moter_pid_value(float measure,float calcu);
float R_moter_pid_value(float measure,float calcu);
	
extern float place_err_sum;
void place_PID_reset(void);
extern float place_err_difference;
extern float angle_err;
extern float angle_last_err;
extern float angle_err_sum;

#endif