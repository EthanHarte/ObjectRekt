CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

boardRecognition.o: boardRecognition.cpp ~/Desktop/raspicam-0.0.5/src/raspicam.h
	g++ -o test $(CFLAGS) boardRecognition.cpp $(LIBS) -L/opt/vc/lib -I//usr/local/include -lraspicam -lmmal -lmmal_core -lmmal_util
 
