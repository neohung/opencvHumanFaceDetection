#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <gtk/gtk.h>
#include <glib.h>

#include "HeadTracker.h"

#define WINNAME "camera"
//using namespace cv;
//using namespace std;

HeadTracker tracker = HeadTracker("haarcascade_frontalface_alt.xml","haarcascade_eye_tree_eyeglasses.xml");

cv::Mat frame;
cv::VideoCapture cap(0);
static cv::Point pre_mouse_pt = cv::Point(-1,-1); 
static cv::Point cur_mouse_pt = cv::Point(-1,-1); 
static cv::Point tmp_mouse_pt = cv::Point(-1,-1); 

GtkWidget *mainWindow;
GtkWidget *image;

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

GdkPixbuf *pixBuff;

int refresh(cv::Mat *f){
	int sz = f->dataend - f->datastart;
    memcpy((uchar*)gdk_pixbuf_get_pixels(pixBuff),f->datastart,sz);
    gtk_widget_queue_draw(mainWindow);
    return 0;
}


gboolean drawing_area_draw_callback(GtkWidget *widget, GdkEventExpose *event) 
{
/*
	//pix = gdk_pixbuf_new_from_data((guchar*) opencvImage->imageData,
    //                                GDK_COLORSPACE_RGB, FALSE, opencvImage->depth, opencvImage->width,
     //opencvImage->height, (opencvImage->widthStep), NULL, NULL);
	printf("test\n");
	cairo_t *cr;
 	cr = gdk_cairo_create (gtk_widget_get_window (widget));
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	cairo_set_source_rgb (cr, 0.42, 0.65, 0.80);
	cairo_set_line_width (cr,6);
	cairo_rectangle (cr, 3, 3, 100, 100);
	cairo_stroke (cr); 
	cairo_destroy(cr);
	return FALSE;
*/
	cap >> frame;
	std::vector<cv::Rect> roi_rect = tracker.DetectROI(frame,0.5);
	for( size_t i = 0; i < roi_rect.size(); i++ )
	{
		ellipse( frame, Point(roi_rect[i].x,roi_rect[i].y), Size(roi_rect[i].width,roi_rect[i].height), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
	}
	cv::Mat rgb_frame;
	cv::cvtColor(frame, rgb_frame, CV_BGR2RGB);
	cv::Mat *im = &rgb_frame;
	/*
	uint8_t* pixelPtr = (uint8_t*)im->data;
	Scalar_<uint8_t> bgrPixel;
	int channels = frame.channels();
	for(int i = 0; i < im->rows; i++)
	{
		for(int j = 0; j < im->cols; j++)
		{
			bgrPixel.val[0] = pixelPtr[i*im->cols*channels + j*channels + 0]; // B
			bgrPixel.val[1] = pixelPtr[i*im->cols*channels + j*channels + 1]; // G
			bgrPixel.val[2] = pixelPtr[i*im->cols*channels + j*channels + 2]; // R
			pixelPtr[i*im->cols*channels + j*channels + 0] = 255 - bgrPixel.val[0];
			pixelPtr[i*im->cols*channels + j*channels + 1] = 255 - bgrPixel.val[1];
			pixelPtr[i*im->cols*channels + j*channels + 2] = 255 - bgrPixel.val[2];
		}
	}
	*/
	int sz = im->dataend - im->datastart;
    memcpy((uchar*)gdk_pixbuf_get_pixels(pixBuff),im->datastart,sz);
    image = gtk_image_new_from_pixbuf(pixBuff);
	gtk_widget_queue_draw(mainWindow);
	return false;
}

int main(int argc,char *argv[]){
	//
	//tracker = HeadTracker("haarcascade_frontalface_alt.xml","haarcascade_eye_tree_eyeglasses.xml");
    //嘗試開啟攝影機
    if(!cap.isOpened()) return -1;
	cap >> frame;
	cv::Mat rgb_frame;
	cv::cvtColor(frame, rgb_frame, CV_BGR2RGB);
	//Do gtk
	//GtkWidget *window;
	GtkWidget *drawing_area;
	gtk_init(&argc, &argv);
	mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainWindow), "嗨! GTK+");
	gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 1);
	gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
	//gtk_widget_set_size_request(window, frame.cols, frame.rows);
	g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    cv::Mat *im = &rgb_frame;
	pixBuff=gdk_pixbuf_new_from_data((guchar*)im->datastart,GDK_COLORSPACE_RGB,FALSE,8,im->size().width,im->size().height,(im->channels()*im->cols), NULL,NULL);
    image = gtk_image_new_from_pixbuf(pixBuff);
	gtk_widget_set_size_request(image, frame.cols, frame.rows);
    gtk_container_add(GTK_CONTAINER(mainWindow), image);
	gtk_widget_show(image);
	g_signal_connect (G_OBJECT (image), "draw", G_CALLBACK (drawing_area_draw_callback), NULL);
	gtk_widget_show(mainWindow);    
	gtk_main();
	
	//Finish Gtk
	/*
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
		//ROI
		std::vector<cv::Rect> roi_rect = tracker.DetectROI(frame,0.5);
		for( size_t i = 0; i < roi_rect.size(); i++ )
		{
			ellipse( frame, Point(roi_rect[i].x,roi_rect[i].y), Size(roi_rect[i].width,roi_rect[i].height), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
		}
		//cv::rectangle(frame, cv::Point(roi_rect.x, roi_rect.y), cv::Point(roi_rect.x + roi_rect.width, roi_rect.y + roi_rect.height), CV_RGB(255, 0, 0), 2);
		//顯示字串
		char temp[32];
		sprintf(temp,"(%d,%d)-(%d,%d)", pre_mouse_pt.x,pre_mouse_pt.y,cur_mouse_pt.x,cur_mouse_pt.y );
		cv::putText(frame,temp,cv::Point(pre_mouse_pt.x,pre_mouse_pt.y-10) ,cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0,255),1,8);
		//畫方框
		cv::rectangle(frame,pre_mouse_pt,cur_mouse_pt,cv::Scalar(0,255,0,0),1,8,0);
        //顯示frame到camera名稱的視窗
        cv::imshow(WINNAME, frame);
	    if(cv::waitKey(30) >= 0) break;
    }
    //std::system("PAUSE");
    */
	return 0;
}