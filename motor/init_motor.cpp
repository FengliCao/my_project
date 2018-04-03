#include<stdio.h>
#include<unistd.h>
#include"ctl_motor.h"
#include<time.h>
#include "../m_inc.h"
#include<math.h>
#define limit_speed 0.01
struct timeval ts,te;
extern int fd;

int init_motor( PointXY * p_xy)
{
	int m[3]={16000,16000,16000};
	double p[3]={0,0,400};
	double point[3]={0,0,l6};
	double x_speed,y_speed,acc_x_speed,acc_y_speed,acc=0.0001;
if(motorReset()!=1)
return 1;

printf("\nMotor init successful !\n");
goThePoint(p,point,m);
printf("\nm=%d \n",m[0]);
while(1)
{
//printf("Pxy %f , %f\n",p_xy->x, p_xy->y);
//printf("m=%d,%d,%d \n",m[0],m[1],m[2]);
x_speed=0.0002*(p_xy->x-80);
y_speed=0.0002*(p_xy->y-60);
if(fabs(x_speed)>limit_speed)
		x_speed=x_speed/fabs(x_speed)*limit_speed;
if(fabs(y_speed)>limit_speed)
		y_speed=y_speed/fabs(y_speed)*limit_speed;
if(p_xy->x==0.0&&p_xy->y==0.0)
{
	p[0]=0;
	p[1]=0;
	p[2]=400;
	goThePoint(p,point,m);
	acc_x_speed=0;
	acc_y_speed=0;
}
else if(fabs(x_speed)>0.0001||fabs(y_speed)>0.0001)
{	
	if(x_speed>acc_x_speed)
		acc_x_speed+=acc/10;
	else if(x_speed<acc_x_speed)
		acc_x_speed-=acc;
	if(y_speed>acc_y_speed)
		acc_y_speed+=acc/10;
	else if(y_speed<acc_y_speed)
		acc_y_speed-=acc;

	point[0]-=acc_x_speed;
	point[1]-=acc_y_speed;
	search(m,point);
}
//delay(3);
/*p[0]=-100;
goThePoint(p,point,m);
printf("m=%d,%d,%d \n",m[0],m[1],m[2]);
p[1]=100;
goThePoint(p,point,m);
printf("m=%d,%d,%d \n",m[0],m[1],m[2]);
p[0]=100;
goThePoint(p,point,m);
printf("m=%d,%d,%d \n",m[0],m[1],m[2]);
p[1]=-100;
goThePoint(p,point,m);
printf("m=%d,%d,%d \n\n\n",m[0],m[1],m[2]);
*/}
  close(fd);
  return 0;
}
