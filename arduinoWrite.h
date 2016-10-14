//
//  arduinoWrite.h
//  
//
//  Created by Jeremy Rotman on 6/27/15.
//
//

#ifndef _arduinoWrite_h
#define _arduinoWrite_h


#define MAX_ANGLE 180
#define MIN_ANGLE 0
#define ARRAY_LENGTH 200

int writeToArduino(int x, int y);
int getXAngle();
int getYAngle();
int findAverage(int arr[]);
bool pointing_to_board();

#endif
