#include"HeadTracker.h"

using namespace cv;
	
HeadTracker::HeadTracker(std::string face_cascade_name,std::string eyes_cascade_name)
{
	this->face_cascade = new CascadeClassifier();
	this->eyes_cascade = new CascadeClassifier();	
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

std::vector<cv::Rect> HeadTracker::DetectROI(cv::Mat src_img, float scale)
{
	cv::Mat gray_img, scale_img,eq_img, result_img;
	Size dsize = Size(src_img.cols*scale,src_img.rows*scale);
	scale_img = Mat(dsize,CV_32S);
	resize(src_img, scale_img,dsize);
	cv::cvtColor(scale_img, gray_img, CV_RGB2GRAY);
	cv::equalizeHist(gray_img, eq_img);
	//result_img = src_img.clone();
    std::vector<cv::Rect> faces;
	std::vector<cv::Rect> result;
	face_cascade->detectMultiScale(
		eq_img,
		faces,
		1.2,          // scale factor
		2,            // minimum neighbors
		0|CV_HAAR_SCALE_IMAGE,            // flags
		cv::Size(30, 30) // minimum size
		);
	for( size_t i = 0; i < faces.size(); i++ )
	{
		result.clear();
		Point face_center( (faces[i].x + faces[i].width*0.5)/scale, (faces[i].y + faces[i].height*0.5)/scale );
		Size face_center_size = Size( faces[i].width*0.5/scale, faces[i].height*0.5/scale);
		//ellipse( src_img, face_center, face_center_size, 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
		result.push_back(cv::Rect(face_center.x,face_center.y,face_center_size.width,face_center_size.height));
		Mat faceROI = eq_img(faces[i]);
		std::vector<cv::Rect> eyes;
		this->eyes_cascade->detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30*scale, 30*scale) );
		//char temp[16];
		//sprintf(temp,"%d",eyes.size());
		//cv::putText(faceROI,temp,cv::Point(0,10) ,cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(255,255,255,255),1,8);
		//cv::imshow("ROI", faceROI);
		for( size_t j = 0; j < eyes.size(); j++ )
		{
			Point eye_center( (faces[i].x + eyes[j].x + eyes[j].width*0.5)/scale, (faces[i].y + eyes[j].y + eyes[j].height*0.5)/scale );
			int eye_radius = cvRound( ((eyes[j].width + eyes[j].height)*0.25)/scale );
			//circle( src_img, eye_center, eye_radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
			result.push_back(cv::Rect(eye_center.x,eye_center.y,eye_radius,eye_radius));	
		}
	}
	return result;
}

cv::Rect HeadTracker::DetectRectROI(cv::Mat src_img, float scale)
{
	//float scale = 0.5;
	cv::Mat gray_img, scale_img,eq_img, result_img;
	Size dsize = Size(src_img.cols*scale,src_img.rows*scale);
	scale_img = Mat(dsize,CV_32S);
	resize(src_img, scale_img,dsize);
	cv::cvtColor(scale_img, gray_img, CV_RGB2GRAY);
	cv::equalizeHist(gray_img, eq_img);
	//result_img = src_img.clone();
    std::vector<cv::Rect> faces;
	face_cascade->detectMultiScale(
		eq_img,
		faces,
		1.2,          // scale factor
		2,            // minimum neighbors
		0|CV_HAAR_SCALE_IMAGE,            // flags
		cv::Size(30, 30) // minimum size
		);
	std::vector<cv::Rect>::const_iterator i;
	cv::Rect biggest_rect = cv::Rect(0,0,0,0);
	
	for (i = faces.begin(); i != faces.end(); ++i) 
	{
		if ((biggest_rect.width < i->width) || (biggest_rect.height < i->height))
		{
			biggest_rect = cv::Rect(i->x /scale,i->y / scale,i->width / scale,i->height / scale);
		}
	}
	//cv::rectangle(result_img, cv::Point(biggest_rect.x, biggest_rect.y), cv::Point(biggest_rect.x + biggest_rect.width, biggest_rect.y + biggest_rect.height), CV_RGB(255, 0, 0), 2);
	return biggest_rect;
}
