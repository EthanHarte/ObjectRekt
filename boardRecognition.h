#ifndef BOARD_RECOGNITION_H
#define BOARD_RECOGNITION_H

#include <iostream>
#include <string>

using namespace cv;

string intToString(int number);
void createTrackbars();
void drawObject(int x, int y, Mat &frame);
void morphOps(Mat &thresh);
void calculateCoordinates(int &x, int &y, double &x_angle, double &y_angle);
void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed, double &coord1, double &coord2);
void detectObject(double &coord1, double &coord2);

#endif