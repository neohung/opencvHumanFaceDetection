#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "HeadTracker.h"

#define WINNAME "camera"
//using namespace cv;
//using namespace std;

cv::Mat frame;
static cv::Point pre_mouse_pt = cv::Point(-1,-1); 
static cv::Point cur_mouse_pt = cv::Point(-1,-1); 
static cv::Point tmp_mouse_pt = cv::Point(-1,-1); 

void on_mouse(int event, int x, int y, int flags, void* param)
{
/*
#define CV_EVENT_MOUSEMOVE 0             
#define CV_EVENT_LBUTTONDOWN 1           
#define CV_EVENT_RBUTTONDOWN 2           
#define CV_EVENT_MBUTTONDOWN 3           
#define CV_EVENT_LBUTTONUP 4             
#define CV_EVENT_RBUTTONUP 5             
#define CV_EVENT_MBUTTONUP 6             
#define CV_EVENT_LBUTTONDBLCLK 7         
#define CV_EVENT_RBUTTONDBLCLK 8         
#define CV_EVENT_MBUTTONDBLCLK 9         
*/
  if (event == CV_EVENT_LBUTTONDOWN)
  {
    cur_mouse_pt = cv::Point(-1,-1); 
	pre_mouse_pt = cv::Point(-1,-1); 
    tmp_mouse_pt = cv::Point(x,y); 
  }else if(event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON)){
	pre_mouse_pt = tmp_mouse_pt;
    cur_mouse_pt = cv::Point(x,y); 
  }  
}

int main(){
	HeadTracker tracker = HeadTracker("haarcascade_frontalface_alt.xml","haarcascade_eye_tree_eyeglasses.xml");
    //抓取攝影機
    cv::VideoCapture cap(0);
    //嘗試開啟攝影機
    if(!cap.isOpened()) return -1;

    //建立一個視窗,名稱為camera
    cv::namedWindow(WINNAME,1);
	//cv::waitKey(0);
	cv::setMouseCallback(WINNAME,on_mouse,0); 	
    for(;;)
    {
        //把取得的影像放置到矩陣中
        cap >> frame;
		//
		cv::Mat roi = tracker.DetectROI(frame);
		//顯示字串
		char temp[32];
		sprintf(temp,"(%d,%d)-(%d,%d)", pre_mouse_pt.x,pre_mouse_pt.y,cur_mouse_pt.x,cur_mouse_pt.y );
		cv::putText(frame,temp,cv::Point(pre_mouse_pt.x,pre_mouse_pt.y-10) ,cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0,255),1,8);
		//畫方框
		cv::rectangle(frame,pre_mouse_pt,cur_mouse_pt,cv::Scalar(0,255,0,0),1,8,0);
        //顯示frame到camera名稱的視窗
        cv::imshow(WINNAME, roi);
	    if(cv::waitKey(30) >= 0) break;
    }
    //std::system("PAUSE");
    return 0;
}