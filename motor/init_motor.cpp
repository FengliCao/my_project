#include<stdio.h>
#include<unistd.h>
#include"ctl_motor.h"
#include<time.h>
#include "../m_inc.h"
#include<math.h>
#include <pthread.h>
#define limit_speed 0.008
#define field 100
static int set_sm(int, int);
const float Kp = 0.00002;
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
if(motorReset(m)!=1)
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
	printf("\nMotor start !\n");
/*
 * while(1)
{
	set_sm(1, 150);	
	set_sm(2, 220);	
	set_sm(1, 100);	
	set_sm(2, 100);	


	p[0]=-field;
	p[1]=-field;
	p[2]=400;
	goThePoint(p,point,m);
	p[0]=field;
	p[1]=-field;
	p[2]=400;
	goThePoint(p,point,m);
	p[0]=field;
	p[1]=field;
	p[2]=400;
	goThePoint(p,point,m);
	p[0]=-field;
	p[1]=field;
	p[2]=400;
	goThePoint(p,point,m);
	}
*/
	t_function();
	p[1]=point[1]-30;
	p[2]=450;
	goThePoint(p,point,m);
	close(fd);
  	return 0;
}
void t_function(void)
{
while(1)
{
delay(5);
x_speed=Kp*(p_xy->x-160);
y_speed=Kp*(p_xy->y-120);
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
if(acc_x_speed==0&&acc_y_speed==0)
	return ;
	point[0]+=acc_x_speed;
	point[1]+=acc_y_speed;

	search(m,point);
}
}
}


static int set_sm(int n, int aim_pos)
{
	if (n != 1  &&  n!= 2)
		return -1;
	static int _1_now_pos(100);
	static int _2_now_pos(100);
	switch(n)
	{
		case 1:
			while(1)
			{
				
			if( aim_pos > _1_now_pos)
			{
				ioctl(fd, 7, _1_now_pos+2);
				_1_now_pos += 2;
			delay(18000);
			}
			else if( aim_pos < _1_now_pos)
			{
				ioctl(fd, 7, _1_now_pos-2);
				_1_now_pos -= 2;
			delay(18000);
			}
			else break;

			}
			break;
		case 2:
			while(1)
			{
			if( aim_pos > _2_now_pos)
			{
				ioctl(fd, 8, _2_now_pos+2);
				_2_now_pos += 2;
			delay(18000);
			}
			else if( aim_pos < _2_now_pos)
			{
				ioctl(fd, 8, _2_now_pos-2);
				_2_now_pos -= 2;
			delay(18000);
			}
			else break;

			}
			break;
		default :
			break;
	}


}






