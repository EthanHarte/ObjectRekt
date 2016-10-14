#include <unistd.h>
#include <stdio.h>
#include "arduinoWrite.h"


// things to add to master function

// arduino can only deal with values between 0 and 180

#define MAX_ANGLE 180
#define MIN_ANGLE 0

// define the number of angles for the average (also determines delay time)

#define ARRAY_LENGTH 200

    // Function to figure out how much to turn by


int getXAngle()
{
    return 45;
}

int getYAngle()
{
    // here we will use the data from the Lepton to determine how much we need to turn


    return 45;
}


    // Find the average angle over data

int findAverage(int arr[])
{
    int total;
    int j = 0;
    
    for (j; j < ARRAY_LENGTH; j++)
    {
        total += arr[j];
    }
    
    return (total/ARRAY_LENGTH);
}

int xCenter = 90;       // default camera angle is at the complete center
int yCenter = 90;

int xData[ARRAY_LENGTH], yData[ARRAY_LENGTH];
int i = 0;

while (i < ARRAY_LENGTH)
{
    int xAngle = xCenter;       // sets base case of original center if we get nothing from function
    int yAngle = yCenter;
    
    yAngle = getYAngle();     // find out how much we need to turn the servo and set angles to this
    xAngle = getXAngle();
    
    xData[i] = xAngle;      // store data in our array that will be averaged
    yData[i] = yAngle;
    
    delay(1);
}

int xAverage = findAverage(xData);      // finds averages of the data
int yAverage = findAverage(yData);


if ((xAngle != xCenter || yAngle != yCenter) && xAverage <= MAX_ANGLE && xAverage >= MIN_ANGLE && yAverage <= MAX_ANGLE && yAverage >= MIN_ANGLE)
{
    writeToArduino(xAverage, yAverage);
}

xCenter = xAverage;     // set the new center
yCenter = yAverage;











// Actual function


    // Implementation that treats arduino connection as a file
    // may work, may need to be more complex

int writeToArduino(int x, int y)
{
    FILE *file;
    file = fopen("USB\VID_234&PID_0043\55330333930351718130", "w");      // open the usb port as writeable

    if (file == NULL)
    {
        fprintf(stderr, "Error finding connection");
        return -1;
    }
    
    fprintf(file, "%03d%03d", x, y);       // write the amount to turn to the "file" (actually arduino)
            
    fclose(file);       // close connection to arduino
    return 0;
}


    // Implementation that treats arduino as a serial port
    // really not sure how good this is tbh

/*
int main()
{
    while (1)
    {
        int angle = 0;
        
        angle = getTurnAngle();
    
        if (serialport_init("COM4", 9600))
            fprintf(stderr, "Error initializing the serial port");
        else
        {
            int serial_fd;
            if ((serial_fd = open("/dev/USB4", O_WRONLY)))
                fprintf("Error finding file descriptor");
            else
            {
                if (serialport_write(serial_fd, "%d", angle))
                    fprintf("Error writing to serial port");
            }
            close(serial_fd);
        }
    }
}
*/






















            




