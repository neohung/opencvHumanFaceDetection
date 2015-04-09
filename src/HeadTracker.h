#ifndef __HEADTRACKER_H__
#define __HEADTRACKER_H__

#include <string>
#include <opencv2/objdetect/objdetect.hpp> 

#include <stdio.h>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 

using namespace cv;

class HeadTracker
{
public:
	HeadTracker(std::string face_cascade_name,std::string eyes_cascade_name);
	~HeadTracker();
	cv::Rect DetectRectROI(cv::Mat src_img, float scale);
	std::vector<cv::Rect> DetectROI(cv::Mat src_img, float scale);
	CascadeClassifier* face_cascade;
	CascadeClassifier* eyes_cascade;
private:
private:
};
#endif