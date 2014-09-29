#include <Servo.h>

Servo servoLeft;
Servo servoRight;

int left_IR_Get_Pin = 9;
int left_IR_Send_Pin = 10; 
int right_IR_Get_Pin = 2;
int right_IR_Send_Pin = 3;
  
int servoLeft_Pin = 12;
int servoRight_Pin = 13;

void setup()
{
  servoLeft.attach(servoLeft_Pin); 
  servoRight.attach(servoRight_Pin); 
  
  turnRight(600);
  forward(700);
  backward(700);
  turnLeft(600);
  forward(1200);
  turnLeft(1900);
}

void loop()
{
  //See if there are any objects close to the robot
  int irLeft = irDetect(left_IR_Send_Pin, left_IR_Get_Pin, 38000);
  int irRight = irDetect(right_IR_Send_Pin, right_IR_Get_Pin, 38000);

  //If there is an object on both sides of the robot
  if(irLeft == 0 && irRight == 0)
  {
    //  Go backwards and a left
     backward(20);
     turnLeft(100);
  }
  //If there is an object to the left only
  else if(irLeft == 0)
  {
    //  Go a little right
    turnRight(50);
  }
  //If there is an object to the right only
  else if(irRight == 0)
  {
    //  Go a little left
    turnLeft(50);
  }
  else
  {
    //  Go a little forward
    forward(20);
  }
}

//See if there are any objects close to the robot
int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  //Send InfraRed
  tone(irLedPin, frequency, 8);
  delay(1);
  //Get InfraRed
  int ir = digitalRead(irReceiverPin);
  delay(1);
  return ir;
}

void forward(int time)                       // Forward function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}

void turnLeft(int time)                      // Left turn function
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}

void turnRight(int time)                     // Right turn function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}

void backward(int time)                      // Backward function
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}

