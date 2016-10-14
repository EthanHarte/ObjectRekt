
#include <Servo.h>  // servo library


int xCur = 90;
int yCur = 90;
Servo xServo;  // servo object that controls x rotation
Servo yServo;  // servo object that controls y rotation

void setup()
{ 
  //set up the serial port
  Serial.begin(9600);
  
  //attach servos to pins
  xServo.attach(9);
  yServo.attach(10);
  
  // Init servos to 90
  xServo.write(90); 
  delay(1000);
  yServo.write(90);
  delay(1000);
   
}


void loop()
{ 
  // vars
  int xPos;
  int yPos;
  int xInput[3];
  int yInput[3];
  
  while (Serial.available()) {
     // read input from serial
     xInput[0] = Serial.read();
     delay(10);
     xInput[1] = Serial.read();
     delay(10);
     xInput[2] = Serial.read();
     delay(10);
     yInput[0] = Serial.read();
     delay(10);
     yInput[1] = Serial.read();
     delay(10);
     yInput[2] = Serial.read();
     
     //*****TESTING******
     // Prints the input ascii values
     Serial.print(xInput[0]);
     Serial.print(xInput[1]);
     Serial.println(xInput[2]);
     Serial.print(yInput[0]);
     Serial.print(yInput[1]);
     Serial.println(yInput[2]);
     //*******************/
     
     // base ten conversion from ascii to int
     xPos = 100*(xInput[0]-48) + 10*(xInput[1]-48) + (xInput[2]-48);
     yPos = 100*(yInput[0]-48) + 10*(yInput[1]-48) + (yInput[2]-48);

     if (xPos > 180 || xPos < 0) continue;
     if (yPos > 180 || yPos < 0) continue;     
     
     //******TSESTING******
     // Prints the input in integer form
     Serial.print("Recieved: ");
     Serial.print(xPos, DEC);
     Serial.print(" ");
     Serial.println(yPos, DEC);
     //*********************/

     xServo.write(xPos);
     delay(130);
     yServo.write(yPos);
     delay(130);
  /*
  if (xCur > xPos) {
    for (; xCur!=xPos; xCur++) {
      xServo.write(xCur);
      delay(100);
    }
  }
  if (yCur < yPos) {
    for (; yCur!=yPos; yCur--) {
      yServo.write(yCur);
      delay(100);
    }
  }
  if (yCur > yPos) {
    for (; yCur!=yPos; yCur++) {
      yServo.write(yCur);
      delay(100);
    }
  } */
  
  /*
    while(xCur!=xPos && yCur!=yPos && xCur>0 && xCur<180 && yCur>0 && yCur<180)
    {
      xServo.write(xCur);
      delay(100);
      yServo.write(yCur);
      delay(100);
      xCur = xPos;
      yCur = yPos;
      if(xPos < xCur) xCur--;
      if(yPos < yCur) yCur--;
      if(xPos > xCur) xCur++;
      if(yPos > yCur) yCur++;
    }
 */
  }
}

