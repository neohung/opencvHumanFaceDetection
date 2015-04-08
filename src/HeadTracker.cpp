#include"HeadTracker.h"

using namespace cv;
	
HeadTracker::HeadTracker(std::string face_cascade_name,std::string eyes_cascade_name)
{
	face_cascade = new CascadeClassifier();
	eyes_cascade = new CascadeClassifier();
	//printf("%s\n",xmlFileName.c_str());
	
	//CascadeClassifier face_cascade;
	
	if( !this->face_cascade->load( face_cascade_name ) )
	{ 
	   printf("--(!)Error loading %s\n",face_cascade_name.c_str()); 
	   return; 
	}
	if( !this->eyes_cascade->load( eyes_cascade_name ) )
	{ 
	   printf("--(!)Error loading %s\n",eyes_cascade_name.c_str()); 
	   return; 
	}
	
}

HeadTracker::~HeadTracker()
{

}

cv::Mat HeadTracker::DetectROI(cv::Mat src_img)
{

	cv::Mat gray_img, eq_img, result_img;
	cv::cvtColor(src_img, gray_img, CV_RGB2GRAY);
	cv::equalizeHist(gray_img, eq_img);
	result_img = src_img.clone();
    std::vector<cv::Rect> faces;
	face_cascade->detectMultiScale(
		eq_img,
		faces,
		1.1,          // scale factor
		3,            // minimum neighbors
		0,            // flags
		cv::Size(20, 20) // minimum size
		);
	std::vector<cv::Rect>::const_iterator i;
	for (i = faces.begin(); i != faces.end(); ++i) 
	{
		cv::rectangle(result_img, cv::Point(i->x, i->y), cv::Point(i->x + i->width, i->y + i->height), CV_RGB(255, 0, 0), 2);
	}
	return result_img;
}
