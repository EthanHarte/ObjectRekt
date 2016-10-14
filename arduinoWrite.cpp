//#include <unistd.h>
#include <stdio.h>
#include "arduinoWrite.h"



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
	int total = 0;
	int j = 0;

	for (j; j < ARRAY_LENGTH; j++)
	{
		total += arr[j];
	}

	return (total / ARRAY_LENGTH);
}

bool pointing_to_board()
{
	return false;
}



// Actual function


// Implementation that treats arduino connection as a file
// may work, may need to be more complex

int writeToArduino(int x, int y)
{
	FILE *file;
	file = fopen("USB\\VID_234&PID_0043\55330333930351718130", "w");      // open the usb port as writeable TODO: change to USB port

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