#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h> 
#define MDir_A  0
#define MDir_B  1
#define MDir_C  2
#define MPul_ABC 3

#define l1 195.03
#define l2 200
#define l3 518.4
#define l4 66.29
#define l5 400.83
#define l6 301.02
#define pi 3.14159265358979323846
#define step_cnt  64000  //stepCountForRightAngle
#define acclerate 0.000001
#define maxSpeed  0.03      //22650
void delay(int n);
void search(int m[3],double p[3]);
void Coordinate_calculation(int id,int m,double p[3],double *e);
void Coordinate_calculation(int id,int m,double p[3],double *e);
void goThePoint(double targetPonit[3],double point[3],int m[3]);
int motorReset(void);
