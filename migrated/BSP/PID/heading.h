#ifndef HEADING_H
#define HEADING_H
typedef struct { float target,correction;unsigned int locked; } HeadingHold;
/* Inputs are degrees / degrees per second, positive for a left turn. */
void heading_begin(HeadingHold *s,float yaw,unsigned int valid);
float heading_step(HeadingHold *s,float yaw,float rate,unsigned int valid);
float heading_error(float target,float current);
#endif
