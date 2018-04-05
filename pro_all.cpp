#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sched.h>
#include "./photo/photo.h"
#include "./motor/init_motor.h"
#include "m_inc.h"

int main ()
{
	pid_t pid;
	int shmid ;
	shmid = shmget(0x91, 3*sizeof(float), IPC_CREAT|0666);
	if(shmid == -1)
	{
		perror("shmget error");
		return errno;
	}
	printf("shmid :%d\n",shmid);
	PointXY *p = NULL;
	p =(PointXY *)shmat(shmid, NULL, 0);
	if(p == (void *)-1)
	{
		perror("shmat error");
		return errno;
	}



	pid = fork();
	if(-1 == pid){
		printf("\nProcess creat error! \n");
	}//if
	else if(0 == pid){
	/*	cpu_set_t mask,get;
		CPU_ZERO(&mask);
		CPU_SET(7,&mask);
		if(sched_setaffinity(0, sizeof(mask), &mask) == -1)
			perror("can not set cpu 4");
		else*/
		printf("\nChild process !\n");
			p->flag = 0.0;
			ret_pos( p );
		printf("\nChild process over\n");
		}//if
		else {
		printf("\nThis is parent process \n Child PID is : %d \n",pid);
		
		/*cpu_set_t mask,get;
		CPU_ZERO(&mask);
		CPU_SET(6,&mask);
		if(sched_setaffinity(0, sizeof(mask), &mask) == -1)
			perror("can not set cpu 5");
		else*/
			init_motor( p );
			printf("\nParent process over\n");
		}//else
return 0;

}//main

