#include "heading.h"
#include <math.h>
float heading_error(float target,float current)
{
 float error=target-current;
 if(!isfinite(error))return 0;
 while(error>180.0f)error-=360.0f;
 while(error< -180.0f)error+=360.0f;
 return error;
}
void heading_begin(HeadingHold *s,float yaw,unsigned int valid)
{
 s->target=isfinite(yaw)?yaw:0;s->correction=0;
 s->locked=valid && isfinite(yaw);
}
float heading_step(HeadingHold *s,float yaw,float rate,unsigned int valid)
{
 float error,desired,step;
 /* Drop a stale correction immediately, but never recapture the target
  * midway through the SAME unmarked route segment. */
 if(!valid || !s->locked || !isfinite(yaw) || !isfinite(rate)){
  s->correction=0;return 0;
 }
 error=heading_error(s->target,yaw);
 if(error> -0.3f && error<0.3f)error=0;
 desired=1.5f*error-0.12f*rate;
 if(desired>8.0f)desired=8.0f;
 if(desired< -8.0f)desired= -8.0f;
 step=desired-s->correction;
 if(step>0.5f)step=0.5f;
 if(step< -0.5f)step= -0.5f;
 s->correction+=step;return s->correction;
}
