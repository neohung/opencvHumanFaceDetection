INCS_PATH=-I..\opencv\include
LIBS_PATH=-L..\opencv\lib
#CFLAGS=-lopencv_highgui2410 -lopencv_core2410 -lopencv_imgproc2410 -lopencv_objdetect2410 -lopencv_video2410 -lopencv_calib3d2410 -lopencv_features2d2410 -lopencv_flann2410 -lopencv_legacy2410 -lcv2 -lopencv_haartraining_engine
CFLAGS=-lopencv_highgui2410 -lopencv_core2410 -lopencv_imgproc2410 -lopencv_objdetect2410

all:
	g++ -c HeadTracker.cpp $(INCS_PATH) -o HeadTracker.o
	g++ -c main.cpp $(INCS_PATH) -o main.o
	g++ HeadTracker.o main.o $(LIBS_PATH) $(CFLAGS) -o main
run:
	main.exe
clean:
	rm HeadTracker.o
	rm main.o
	rm main.exe