#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h> 
#include "ctl_motor.h"
#include <math.h>
#include "tab.h"
#include <time.h>
#include <sys/time.h>
#include <pthread.h> // pthread header
static const int high = 1,low = 0;
static struct timeval ta,tb;
int tm[3];
int fd;

void delay(int n)
{
	gettimeofday(&ta,NULL);
	while(1)
	{
		gettimeofday(&tb,NULL);
		if((tb.tv_sec-ta.tv_sec)*1000000.0+(tb.tv_usec-ta.tv_usec) >= n) break;
	}
	//printf("\n%f\n",(ts.tv_sec-te.tv_sec)+(ts.tv_usec-te.tv_usec)/1000000.0);
}

void Coordinate_calculation(int id,int m,double p[3],double *e)
	{
	int i;
	double A[3];
	double B[3];
switch (id)
	{
	case 0:
		{
		A[0]=M0[m][0];
		A[1]=M0[m][1];
		A[2]=M0[m][2];
		B[0]=p[0];
		B[1]=p[1]+l4;
		B[2]=p[2];
		break;
		}
	case 1:
		{
		A[0]=M1[m][0];
		A[1]=M1[m][1];
		A[2]=M1[m][2];
		B[0]=p[0]-sqrt(3)/2*l4;
		B[1]=p[1]-1*l4/2;
		B[2]=p[2];
		break;
		}
	case 2:
		{
		A[0]=M2[m][0];
		A[1]=M2[m][1];
		A[2]=M2[m][2];
		B[0]=p[0]+sqrt(3)/2*l4;
		B[1]=p[1]-1*l4/2;
		B[2]=p[2];
		break;
		}
	default :break;
	}
for(i=0;i<3;i++)
{
	A[i]=A[i]-B[i];
	A[i]=A[i]*A[i]; 
}
*e=A[0]+A[1]+A[2]-l3*l3;
}


void search(int m[3],double p[3])
{
int i;
double e[3];
for(i=0;i<3;i++)
{

Coordinate_calculation(i,m[i],p,&e[i]);
  if(e[i]>0)
{
	m[i]++;////
}
else if(e[i]<0)
{
	m[i]--;////

}

if(tm[i]-m[i]<-1)
{
	if(i==2)
 	ioctl(fd,4,1);
	else
	ioctl(fd,i,1);
	tm[i]++;
ioctl(fd,5,1+i);
}
else if (tm[i]-m[i]>1) 
{

	if(i==2)
 	ioctl(fd,4,0);
	else
	ioctl(fd,i,0);
	tm[i]--;
ioctl(fd,5,1+i);
}
}
}
void goThePoint(double targetPonit[3],double point[3],int m[3])
	{
	double currentPoint[3],temp[3];
	double distance=0,runningDistance=0,speed=0,acclerateDistance=0;
	int i,runningState=0;
	for(i=0;i<3;i++)
{
	currentPoint[i]=point[i];
	temp[i]=targetPonit[i]-point[i];
	temp[i]=temp[i]*temp[i]; 
}
distance=sqrt(temp[0]+temp[1]+temp[2]);
for(i=0;i<3;i++)
{
	temp[i]=(targetPonit[i]-point[i])/distance;
}

while(1)
	{
if(runningState!=2&&acclerateDistance<(distance-runningDistance))
	{
	if(runningState==0&&speed+acclerate<=maxSpeed)
		{
		speed+=acclerate;
		acclerateDistance+=speed;
		runningDistance=acclerateDistance;
		}
	else
		{
		runningState=1;
		runningDistance+=speed;	
		}
	}
else
	{
	runningState=2;
	speed-=acclerate;
	runningDistance+=speed;
	if(speed<=0)
	{
	for(i=0;i<3;i++)
	{
	point[i]=targetPonit[i];
	}
//printf("m[i]=%d,tm[i]=%d\n ",m[i-1],tm[i-1]);
	return ;
	}
	}
for(i=0;i<3;i++)
{
	currentPoint[i]=temp[i]*runningDistance+point[i];
}


search(m,currentPoint);

//printf("m[0]=%d \n",m[0]);
	}

	}

/*
 *thread func
 */
static int n; //save the input var
void* read_signal(void* arg)
{
	int *tmp = (int *)arg;
	if(1 == *tmp) printf("\nbegin read:");

	while(1){
	read(fd,&n,1);
	printf("\n	ABC singal is:  %1d_%1d_%1d",n&0x01,(n>>1)&0x01,(n>>2)&0x01);
	}
}
int motorReset(int m[3]) //reset motor 
{
	int flag[3] = {0};
	int i;
	    fd = open("/dev/motor", O_RDWR); //open dev file
	    if (fd < 0) {
		perror("open");
		exit(-2);
   		return 0;
	    }
	    i = read(fd,&n,1); // read the input signal (A B C /4 2 1)
	    if(i < 0) {
	    	printf("read err");
	    	return 0;
		}
		else 
			printf("\nsignal is =: %d\n",n);
	
	/* creat a thread to read the input singal*/
	
	pthread_t tid; 
	int t_arg = 1;
	printf("Waiting the motor reset");
	if(pthread_create(&tid, NULL, read_signal, &t_arg))
		perror("Fail to create a thread !\n");	
	sleep(1);
	while(n)
	{
	if((n&0x01))
		ioctl(fd,5,1);
	if((n&0x02))
		ioctl(fd,5,2);
	if((n&0x04))
		ioctl(fd,5,3);
	delay(100);
	}
	pthread_cancel(tid); // cancel the thread
i=3000;
ioctl(fd,4,0);
ioctl(fd,1,0);
ioctl(fd,0,0);

while(i--)
{
    ioctl(fd,MPul_ABC);	
	delay(100);
}
for(i=0;i<3;i++)
{
tm[i]=m[i];
}
return 1;
}



