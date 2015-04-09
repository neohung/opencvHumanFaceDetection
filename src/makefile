ifeq ($(shell uname), Linux)
	RM = rm -f
	FixPath = $1
else
	ifdef SystemRoot
		RM = del /Q
		FixPath = $(subst /,\,$1)
	endif
endif


INCS=-I..\opencv\include -I..\gtk\include\gtk-3.0 -I..\gtk\include\glib-2.0 -I..\gtk\lib\glib-2.0\include -I..\gtk\include\gdk-pixbuf-2.0 -I..\gtk\include\pango-1.0 -I..\gtk\include\cairo -I..\gtk\include\atk-1.0
LIBS=-L..\opencv\lib -L..\gtk\lib -lopencv_highgui2410 -lopencv_core2410 -lopencv_imgproc2410 -lopencv_objdetect2410 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lpango-1.0 -lgtk-3 -lglib-2.0 -lgdk-3 -lgobject-2.0
#CFLAGS=-lopencv_highgui2410 -lopencv_core2410 -lopencv_imgproc2410 -lopencv_objdetect2410 -lopencv_video2410 -lopencv_calib3d2410 -lopencv_features2d2410 -lopencv_flann2410 -lopencv_legacy2410 -lcv2 -lopencv_haartraining_engine
CFLAGS=
CC = g++

all:
	$(CC) -c HeadTracker.cpp $(INCS) $(CFLAGS) -o HeadTracker.o
	$(CC) -c main.cpp $(INCS) $(CFLAGS) -o main.o
	$(CC) HeadTracker.o main.o $(INCS) $(LIBS) $(CFLAGS) -o main
run:
	main.exe
clean:
	$(RM) HeadTracker.o
	$(RM) main.o
	$(RM) main.exe