//objectTrackingTutorial.cpp

//Written by  Kyle Hounslow 2013

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <raspicam/raspicam.h>

using namespace cv;
using namespace std;

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 137;
int H_MAX = 147;
int S_MIN = 39;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

// set true if obejct detected
bool OBJECT_DETECTED = false;

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//default center width and height
const int X_CENTER = 320;
const int Y_CENTER = 240;

//arbitrary distances for demo purposes
const double ARB_WIDTH = 7;
const double ARB_HEIGHT = 9;
const int ARB_DISTANCE = 20;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;

//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;

//names that will appear at the top of each window
const string windowName = "Original Image";
//const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";

//const string trackbarWindowName = "Trackbars";
/*void on_trackbar(int, void*)
{//This function gets called whenever a
// trackbar position is changed
}*/

/*
string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}
*/

/*void createTrackbars(){
//create window for trackbars


namedWindow(trackbarWindowName, 0);
//create memory to store trackbar name on window
char TrackbarName[50];
sprintf(TrackbarName, "H_MIN", H_MIN);
sprintf(TrackbarName, "H_MAX", H_MAX);
sprintf(TrackbarName, "S_MIN", S_MIN);
sprintf(TrackbarName, "S_MAX", S_MAX);
sprintf(TrackbarName, "V_MIN", V_MIN);
sprintf(TrackbarName, "V_MAX", V_MAX);
//create trackbars and insert them into window
//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
//the max value the trackbar can move (eg. H_HIGH),
//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
//                                  ---->    ---->     ---->
createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);


}*/

/*
void drawObject(int x, int y, Mat &frame)
{

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

	//UPDATE:JUNE 18TH, 2013
	//added 'if' and 'else' statements to prevent
	//memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

	circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
	if (y - 25>0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);

	if (y + 25<FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);

	if (x - 25>0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);

	if (x + 25<FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);

	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);

}
*/ 

void morphOps(Mat &thresh)
{

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	// erode gets rid of white noise
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));

	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);

	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);

}

void calculateCoordinates(int &x, int &y, double &x_angle, double &y_angle)
{
	int x_offset = x - X_CENTER;
	int y_offset = y - Y_CENTER;
	double cameraDistance = sqrt(ARB_DISTANCE * ARB_DISTANCE + x_offset * x_offset);

	//convert this to actual physical distance 

	//find angles
	x_angle = atan(ARB_WIDTH / ARB_DISTANCE) * 180.0 / 3.14159265;
	y_angle = atan(ARB_HEIGHT / cameraDistance) * 180.0 / 3.14159265;
	x_angle += 90;
	y_angle += 90;
}

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed, double &coord1, double &coord2)
{

	Mat temp;
	threshold.copyTo(temp);

	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();

		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;


			}

			//let user know you found an object
			if (objectFound == true && OBJECT_DETECTED != true){
				putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);

				//draw object location on screen
				// 320, 240 is the center
				// drawObject(x, y, cameraFeed);

				calculateCoordinates(x, y, coord1, coord2);
				OBJECT_DETECTED = true;

			}

		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}

void detectObject(double &coord1, double &coord2)
{
	//some boolean variables for different functionality within this
	//program
	bool trackObjects = true;	// set to true to turn on tracking object
	bool useMorphOps = true;	// set to true to turn on erode/dilate

	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;

	//matrix storage for HSV image
	Mat HSV;

	//matrix storage for binary threshold image
	Mat threshold;

	//x and y values for the location of the object
	int x = 0, y = 0;

	//create slider bars for HSV filtering
	//createTrackbars();
	//video capture object to acquire webcam feed
	VideoCapture capture;

	//open capture object at location zero (default location for webcam)
//	capture.open(0);

	//set height and width of capture frame
//	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
//	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	raspicam::RaspiCam camera;
	if (!camera.open()) 
		{
			cerr << "Error opening camera" << endl;
			return;
		}
	sleep(3);

	
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while (1){ //only detect once?
		//store image to matrix
//		capture.read(cameraFeed);
		camera.grab();
		unsigned char *data = new unsigned char[camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
		camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB);
		std::ofstream outFile("boardPicture.ppm",std::ios::binary);
		outFile << "P6\n" << camera.getWidth() << " " << camera.getHeight() << " 255\n";
		outFile.write((char*)data, camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));


		cameraFeed = imread("boardPicture.ppm", 1);		

		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		

		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);

		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if (useMorphOps)
			morphOps(threshold);

		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if (trackObjects)
			trackFilteredObject(x, y, threshold, cameraFeed, coord1, coord2);

		//show frames 
		imshow(windowName2, threshold);
		imshow(windowName, cameraFeed);
		//imshow(windowName1, HSV);


		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);

		if (OBJECT_DETECTED)
		{
			ofstream file;
			int result1, result2;
			result1 = (int) coord1;
			result2 = (int) coord2;
			file.open("coordinates.txt");
			file << setfill('0') << setw(3) << result1 << setfill('0') << setw(3) << result2;
			break;
		}

	}

}


int main()
{
	double a, b;
	detectObject(a, b);
}
