#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <unistd.h>
#include "photo.h"
#include "../m_inc.h"
using namespace std;
using namespace cv;
Point2f Centor[30];

//草莓颜色阀值
static int iLowH = 0;    
static int iHighH = 15;    
		  
static int iLowS = 150;     
static int iHighS = 255;    
				
static int iLowV = 0;    
static int iHighV = 255;

static int thread_flag = 1; //
Mat thread_edge;
void* thread_func(void *p){
	PointXY *p_xy = (PointXY *)p;
	//-------------查找轮廓并拟合圆-----------------------------
	//定义轮廓和层次结构
	vector< std::vector< cv::Point> > contours;
	vector< Vec4i > hierarchy;
	//查找轮廓
	int model=CV_RETR_CCOMP;
	int method=CV_CHAIN_APPROX_NONE;
  	findContours(thread_edge,contours,hierarchy,model,method);//遍历轮廓，绘拟合圆 	

	int cnt;	
	cnt = 0;
	for(unsigned int index=0;index < contours.size();index++){        
                float radius;
		Point2f center;
				
		minEnclosingCircle(contours[index],center,radius);  
		RotatedRect rect=cv::minAreaRect(contours[index]);

		double ratio=double(rect.size.height)/double (rect.size.width+0.01);
		
		if(radius>5&&ratio>0.5&&ratio<5.0){ //长短轴比进行一定的限定               
			Centor[cnt].x = center.x;
			Centor[cnt].y = center.y;
			cnt++;
			circle(Yimg,center,(int)radius+5,Scalar(0,255,0),1);  //radius+6.0将之前腐蚀时减小量补回来        
			}//if
		}//for
	if(cnt > 0){
		float aim = Centor[0].x * Centor[0].x + Centor[0].y * Centor[0].y;
		int _aim = 0;
		float temp;
		for(int i = 0; i < cnt; i++)
		{
			temp = Centor[i].x * Centor[i].x + Centor[i].y * Centor[i].y;		
			if (aim >= temp) {
				 aim = temp;
				_aim = i;
				}//if		
			}//for
		cnt = 0;
		p_xy->x = Centor[_aim].x;
		p_xy->y = Centor[_aim].y;

		//printf("\n aim is : %f ,%f",Centor[_aim].x , Centor[_aim].y);
		}//if
	else{
		p_xy->x = 0.0;
		p_xy->y = 0.0;
	}
thread_flag = 0;
}
int ret_pos( PointXY *p_xy){
	
	VideoCapture cap(0); // open the default camera
    	if(!cap.isOpened())  // check if we succeeded
        	return -1;
 

//  namedWindow("原始图像",CV_WINDOW_NORMAL);
//	namedWindow("edge",CV_WINDOW_NORMAL);
    	cap >> edges; // get a new frame from camera
	//cout<< "rows: " << edges.rows <<", cols: " << edges.cols <<endl;

for(;;){
	Mat srcImage,Image;
	cap >> Image; // get a new frame from camera
 
	//flip(Image, Image,0);//镜像
	resize(Image,srcImage,Size(Image.cols/4,Image.rows/4),0,0,INTER_AREA);
	Mat Yimg = srcImage.clone();
	cvtColor(srcImage, edges, CV_BGR2HSV); //rgb to hsv
	//hsv 二值化
    Mat edges;
	inRange(edges, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV),srcImage);

	Mat kern = getStructuringElement(MORPH_RECT, Size(5, 5)); 
//	morphologyEx(srcImage, srcImage, MORPH_OPEN, kern);// open 
	
//	imshow("open",srcImage);
	morphologyEx(srcImage, srcImage, MORPH_CLOSE, kern);// close
//	imshow("close",srcImage);
	//------------对图像进行腐蚀，使得各个目标不再连通------------
	Mat element = getStructuringElement(MORPH_ELLIPSE,Size(5,5));
	Mat erodeImg;
	erode(srcImage,erodeImg,element);
	
//	imshow("erode",erodeImg);
	//------------对腐蚀后的图像膨胀，以便获得轮廓---------------
	Mat element2 = getStructuringElement(MORPH_ELLIPSE,Size(3,3));    
	Mat dilateImg;
	dilate(erodeImg,dilateImg,element2);
//	imshow("dilate",Img2);

	Mat edge=dilateImg-erodeImg;

//	imshow("edge",edge);
while(1){
	if(thread_flag == 0){
		break;
	}else 
		usleep(10);
}

//create a thread to calculate the picuure
thread_edge = edge;
thread_flag = 1;
pthread_t tid;
if(pthraed_create(&tid, NULL, thread_func, p_xy))
	perror("thread create error");
usleep(100);


	//printf("\n %d \n",cnt);	
	//circle(srcImage, center,15, Scalar(0,255,0), 1);
	//imshow("处理结果", srcImage);
	//imshow("原始图像", Yimg);
        if(waitKey(1) == 'q') break;
    }
    return 0;
}
