  #include <BitVoicer11.h>
  #include <Servo.h>
  
  Servo servoLeft;                             
  Servo servoRight;
  
  BitVoicerSerial bvSerial = BitVoicerSerial(); 
  
  void setup() {
   Serial.begin(9600);
   servoLeft.attach(13);
   servoRight.attach(12);
  }

  void loop() {
  
    bvSerial.getData();
    
    if (bvSerial.strData == "")
    {
     return; 
    }
    if (bvSerial.strData == "forward")
    {
    
       servoLeft.writeMicroseconds(1700);         
       servoRight.writeMicroseconds(1300);    
       bvSerial.strData = "";
    }
      else if (bvSerial.strData == "backward")
    {
        servoLeft.writeMicroseconds(1300);
        servoRight.writeMicroseconds(1700);
        bvSerial.strData = "";
    }
      else if (bvSerial.strData == "left")
    {
        servoLeft.writeMicroseconds(1300);
        servoRight.writeMicroseconds(1300);
        bvSerial.strData = "";
    }
      else if (bvSerial.strData == "right")
    {
        servoLeft.writeMicroseconds(1700);
        servoRight.writeMicroseconds(1700);
        bvSerial.strData = "";
    }
      else if (bvSerial.strData == "stop")
     {
        servoLeft.detach();                        
        servoRight.detach();
        bvSerial.strData = "";
     } 
       else 
       {
        Serial.println("Error:" + bvSerial.strData);
       bvSerial.strData = ""; 
       }
}
