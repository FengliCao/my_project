#include<stdio.h>
#include<unistd.h>
#include"ctl_motor.h"
#include<time.h>
#include "../m_inc.h"
#include<math.h>
#include <pthread.h>
#define limit_speed 0.01
const float Kp = 0.0001;
void t_function(void);

struct timeval ts,te;
extern int fd;
	int m[3]={16000,16000,16000};
	double point[3]={0,0,l6};
	double p[3]={0,0,400};
	double x_speed,y_speed,acc_x_speed,acc_y_speed,acc=0.0001;
PointXY *p_xy = NULL;
int init_motor( PointXY * _p)
{	
	p_xy = _p;
	p_xy->flag = 0.0;
if(motorReset()!=1)
return 1;

printf("\nMotor init successful !\n");
goThePoint(p,point,m);
printf("\nm=%d \n",m[0]);
p_xy->flag = 1.0;
	pthread_t tpid;
	pthread_attr_t a;
	pthread_attr_init(&a);
	pthread_attr_setdetachstate(&a,PTHREAD_CREATE_DETACHED);
	int t_arg;
while(1)
{
	ioctl(fd, 7, 100);
	delay(20000);
	ioctl(fd, 7, 100);
	delay(20000);
	ioctl(fd, 8, 100);
	delay(20000);
	ioctl(fd, 8, 100);
	delay(1000000);
	ioctl(fd, 7, 220);
	delay(20000);
	ioctl(fd, 7, 220);
	delay(20000);
	ioctl(fd, 8, 220);
	delay(20000);
	ioctl(fd, 8, 220);
	delay(1000000);

}

while(1)
{//	usleep(10);
	delay(1);
	t_function();
}
	close(fd);
  	return 0;
}
void t_function(void)
{
x_speed=Kp*(p_xy->x-80);
y_speed=Kp*(p_xy->y-60);
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
else if(fabs(x_speed)>5*Kp||fabs(y_speed)>5*Kp)
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
}
