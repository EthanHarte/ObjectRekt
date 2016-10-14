#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include "boardRecognition.h"
#include "arduinoWrite.h"
using namespace cv;
using namespace std;




int main()
{
	double x_degree_board, y_degree_board;
	detectObject(x_degree_board, y_degree_board);

	int movement_mode = 0;
	/*
	0: No movement
	1: Follow person
	2: Move to screen
	*/

	while (1)
	{

		// arduino can only deal with values between 0 and 180

		int xCenter = 90;       // default camera angle is at the complete center
		int yCenter = 90;

		int xData[ARRAY_LENGTH], yData[ARRAY_LENGTH];
		int i = 0;

		while (i < ARRAY_LENGTH)
		{
			int xAngle = xCenter;       // sets base case of original center if we get nothing from function
			int yAngle = yCenter;

			xAngle = getXAngle();
			yAngle = getYAngle();     // find out how much we need to turn the servo and set angles to this

			xData[i] = xAngle;      // store data in our array that will be averaged
			yData[i] = yAngle;

			i++;
			Sleep(1);
		}

		int xAverage = findAverage(xData);      // finds averages of the data
		int yAverage = findAverage(yData);

		if (pointing_to_board())
		{
			writeToArduino(x_degree_board, y_degree_board);
			xCenter = x_degree_board;
			yCenter = y_degree_board;
		}
		else if ((xAverage != xCenter || yAverage != yCenter) && xAverage <= MAX_ANGLE && xAverage >= MIN_ANGLE && yAverage <= MAX_ANGLE && yAverage >= MIN_ANGLE)
		{
			writeToArduino(xAverage, yAverage);

			x_degree_board = x_degree_board - xAverage + 90;
			y_degree_board = y_degree_board - yAverage + 90;
			xCenter = xAverage;
			yCenter = yAverage;
		}
	}
}