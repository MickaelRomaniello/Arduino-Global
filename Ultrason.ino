#include<Servo.h>
#include<NewPing.h>

#define TRIGGER_PIN  11                                  //listener pin
#define ECHO_PIN     11                                  //active pin
#define MAX_PING_DISTANCE 200                            // power

//WHEELS
int pin_servoWheelLeft = 9;
int pin_servoWheelRight = 10;
Servo servoLeft;                                         // Left Wheel
Servo servoRight;                                        // Right Wheel
int variator = 50;                                      // Positive is turn under the direction of clockwise (100)
                                                         // Negative is turn on the oposite of the direction of clockwise
int little_variator = variator/5;                       // Positive is turn under the direction of clockwise (variator/10)
                                                         // Negative is turn on the oposite of the direction of clockwise
                                                         
//color sensor declarations
int color_red = 0;
int color_blue = 0;
int color_green = 0;
int color_black = 0;

int s0=3;                          //color sensor
int s1=4;                          //color sensor
int s2=5;                          //color sensor
int s3=6;                          //color sensor
int flag=0;
int counter=0;
int countR=0,countG=0,countB=0;

int blancR=45, blancG=45, blancB=40;     // Valeurs de détection pour le Blanc
int rougeR=30, rougeG=20, rougeB=20;     // Valeurs de détection pour le Rouge
int vertR=20,  vertG=25,  vertB=21;      // Valeurs de détection pour le Vert
int vert2R=23, vert2G=31, vert2B=24;     // Valeurs de détection n°2 pour le Vert
int bleuR=18,  bleuG=19,  bleuB=20;      // Valeurs de détection pour le Bleu
int noirR=10,  noirG=10,  noirB=10;      // Valeurs de détection pour le Noir
int pause=200;                           // Temps de pause entre 2 prises

//for ISR void method
int i=0;

//sonar
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_PING_DISTANCE);  //sonar declaration
int distance_for_reaction = 20;                           // in centimeters 
int pos = 150;    // variable to store the servo position 
int myservo_ping = 5;
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
//savec values for 1 scan
int scan_values[5];
int position_values[5]; 
int myserv_position[5] = {0, 50, 80, 130, 170};

void setup(){
  Serial.begin(115200);
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT); 
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  
  //wheels
  servoLeft.attach(pin_servoWheelLeft);                     // Attach left wheel signal to pin 13
  servoRight.attach(pin_servoWheelRight);                    // Attach right wheel signal to pin 12
  
  //sonar
  myservo.attach(myservo_ping);  // attaches the servo on pin 9 to the servo object                 // in steps of 1 degree 
  myservo.write(myserv_position[0]);
  
}


void loop(){
  Serial.println("MOVING WITH PING");
  move_with_ping();
}

/** moving function */
void maneuver(int speedLeft, int speedRight, int msTime){
  // speedLeft, speedRight ranges: Backward  Linear  Stop  Linear   Forward
  //                               -200      -100......0......100       200
  servoLeft.writeMicroseconds(1500 + speedLeft);   // Set Left servo speed
  servoRight.writeMicroseconds(1500 - speedRight); // Set right servo speed
  /** //////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
  // do an action here
    while(color_red < 1){
      //Serial.print(" to detect : color_red ");
      //Serial.println(color_red);
      Serial.println("CALL TEST COLOR, WAITING RED");
      test_color();
    }
    while(color_blue < 1){
      Serial.println("CALL TEST COLOR, WAITING BLUE");
      //Serial.print(" to detect : color_blue ");
      //Serial.println(color_blue);
      test_color();
    }
    while(color_green < 1){
      Serial.println("CALL TEST COLOR, WAITING GREEN");
      //Serial.print(" to detect : color_green ");
      //Serial.println(color_green);
      test_color();
    }
    while(color_black < 1){
      Serial.println("CALL TEST COLOR, WAITING BLACK");
     // Serial.print(" to detect : color_black ");
      //Serial.println(color_black);
      test_color();
    }
  /*
  /*Serial.println("gogo maneuver");
  delay(100);
  int a = 0;
  while(a < 10){ 
    Serial.println("gogo while");
    if(a==5){
      servoLeft.writeMicroseconds(1500);   // Set Left servo speed
      servoRight.writeMicroseconds(1500); // Set right servo speed
      Serial.println("a=5 : break !");
     break;
    }
    delay(100);
    a++;
  }*/
  /** //////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
  //set speed to 0;
  servoLeft.writeMicroseconds(1500);   // Set Left servo speed
  servoRight.writeMicroseconds(1500); // Set right servo speed
  if(msTime==-1)                                   // if msTime = -1
  {                                  
    servoLeft.detach();                            // Stop servo signals
    servoRight.detach();   
  }
  //delay(msTime);                                   // Delay for msTime}
}

/**
* @Author : Mohorelien
*
*/
void test_color(){
     Serial.print("COLORS : ");
     Serial.print(" -R");
     Serial.print(countR);
     Serial.print(" -G");
     Serial.print(countG);
     Serial.print(" -B");
     Serial.print(countB);
     Serial.println();
   delay(50);
   TCS();
   if((countR >= blancR) && (countG >= blancG) && (countB >= blancB)){        // Beaucoup de chaque couleur = Blanc
    Serial.println("----------> BLANC");
    //go_little_stop();
  //delay(pause);
   }else if((countR >= rougeR) && (countG <= rougeG) && (countB <= rougeB)){  // Majorité de Rouge
     Serial.println("----------> ROUGE");      
     color_red++;
      //go_little_stop();  
     //delay(pause);
   }else if( ((countR <= vertR) && (countG >= vertG) && (countB <= vertB)) || ((countR <= vert2R) && (countG >= vert2G) && (countB <= vert2B)) ){   // Majorité de Vert
     Serial.println("----------> VERT");    
      color_green++; 
      //go_little_stop();    
     delay(pause);
   }else if((countR <= bleuR) && (countG <= bleuG) && (countB >= bleuB)){      // Majorité de Bleu
     Serial.println("----------> BLEU");     
     color_blue++;
      //go_little_stop();    
     delay(pause);
   }else if((countR <= noirR) && (countG <= noirG) && (countB <= noirB)){      // On capte peu de couleurs = Noir
     Serial.println("----------> NOIR");    
     color_black++;
     //go_little_stop();  
     //delay(pause);
   }else{   // Majorité de bordel à cul
      Serial.println("BORDEL A CUL !!");
      move_with_ping();
     // go_little_stop();
     //delay(pause);      
   }
}

void TCS(){
  digitalWrite(s1,HIGH);
  digitalWrite(s0,LOW);
  flag=0;
  attachInterrupt(0, ISR_INTO, CHANGE);
  timer2_init();
}

void ISR_INTO(){
  counter++;
}

void timer2_init(void){
  TCCR2A=0x00;
  TCCR2B=0x07;   //the clock frequency source 1024 points
  TCNT2= 100;    //10 ms overflow again
  TIMSK2 = 0x01; //allow interrupt
}
 
/*
* ISR
*/
ISR(TIMER2_OVF_vect){//the timer 2, 10ms interrupt overflow again. Internal overflow interrupt executive function
  TCNT2=100;
  flag++;
  
  if(flag==1){
     counter=0;
  }else if(flag==2){
     digitalWrite(s2,LOW);
     digitalWrite(s3,LOW); 
     countR=counter/1.051;
     //Serial.print("red=");
     //Serial.print(countR,DEC);
     digitalWrite(s2,HIGH);
     digitalWrite(s3,HIGH);   
  }else if(flag==3){
    countG=counter/1.0157;
    //Serial.print(" - green=");
    //Serial.print(countG,DEC);
    digitalWrite(s2,LOW);
    digitalWrite(s3,HIGH); 
  }else if(flag==4){
    countB=counter/1.114;
    //Serial.print(" - blue=");
    //Serial.print(countB,DEC);
    digitalWrite(s2,LOW);
    digitalWrite(s3,LOW);
  }else{
    flag=0; 
    TIMSK2 = 0x00;
  }
  counter=0;
  delay(2);
}

/**
* move_with_ping
*/
void move_with_ping(){
   
  Serial.println("MOVING WITH PING INSIDE");
  //
  //go_forward();
    //go_little_stop();
  //Serial.println("go riiight");
  //go_right();
  Serial.println("MEASUREMENTS");
  
  for(int i=0;i<5;i++){
    myservo.write(myserv_position[i]);      //move to another position 5 times)

    // if need to do a big turn to the captor, we are wainting the operation finished
    if(i==0){
      delay(1000);
    }else{ 
      delay(200);
    }
    int uS = sonar.ping_median(5);        //doing a ping
    
    delay(50);
    
    scan_values[i]=uS / US_ROUNDTRIP_CM;
    position_values[i]=pos;  
    Serial.print("pos : ");
    Serial.print(myserv_position[i]);
    Serial.print(" - DIST IN CENTIMERS : ");
    Serial.print(scan_values[i]);
    Serial.println(" ");
  }
  //var we need to chosse automatically the good one position
  int highest_value_index = 0;
  int highest_value = 0;
  int current_value = 0;
  
  for( int i=0; i < 5; i++){
    current_value = scan_values[i];
    if(current_value > highest_value){
      highest_value = current_value;
      highest_value_index =  i;
    }
  }
  Serial.print("Highest Value : ");
  Serial.print(highest_value);
  Serial.print(" - index : ");
  Serial.print(highest_value_index);
  Serial.print(" - Go to : ");
  Serial.print(myserv_position[highest_value_index]);
  Serial.println();
  
  if(scan_values[0] < 5){                              //protection to unblock robot if we have a wall just to the right or left;
      go_unblock_left();
  }else if(scan_values[4] < 5){
      go_unblock_right();
  }else{
    Serial.println("GO AWAY");
    go_away(myserv_position[highest_value_index],highest_value); //move to another locatio
  }
}

/**
* Go away
*/
void go_away(int pos,int dist){
  Serial.print("POS : ");
  Serial.print(pos);
  Serial.print("DIST : ");
  Serial.println(dist);
  //myserv_position[5] = {0, 50, 80, 130, 170};  
  if(dist > distance_for_reaction){                              //if the highest value we get is higher than a minimum distance
    switch (pos) {
      case 0:  go_right();         break;
      case 50: go_little_right();  break;
      case 80: go_forward();       break;
      case 130: go_little_left();  break;
      case 170: go_left();         break;
      default:                     break;
    }
  }else{
        maneuver(-variator, variator, 1000);                // return
  } 
}



void go_right(){
    Serial.println("---------------------------------------- GO RIGHT");      
    maneuver(-variator, -variator, 200);                // GO FORWARD 1 seconds
    maneuver(variator, little_variator, 1000);                         // FULL RIGHT 1 seconds
    maneuver(-variator, -variator, 2000);                // GO FORWARD 1 seconds
    test_color();
  }

void go_little_right(){
    Serial.println("---------------------------------------- GO LITTLE RIGHT");    
    maneuver(-variator, -variator, 100);                // GO FORWARD 1 seconds
    maneuver(little_variator, -little_variator, 1000);   // LITTLE Right 1 seconds
    maneuver(-variator, -variator, 2000);                // GO FORWARD 1 seconds
}

void go_infinite_forward(){
    Serial.println("---------------------------------------- GO FORWARD");    
    maneuver(-variator, -variator, 10);                // FULL CENTER 1 seconds
    test_color();
}

void go_forward(){
    Serial.println("---------------------------------------- GO FORWARD");    
    maneuver(-variator, -variator, 2000);                // FULL CENTER 1 seconds
    test_color();
}

void go_backward(){
    Serial.println("---------------------------------------- GO BACKWARD");    
    maneuver(variator, variator, 2000);                // FULL CENTER 1 seconds
    test_color();
}

void go_little_left(){
    Serial.println("---------------------------------------- GO LITTLE LEFT");    
    maneuver(-variator, -variator, 100);                // GO FORWARD 1 seconds
    maneuver(-little_variator, little_variator, 1000);   // LITTLE LEFT 1 seconds
    maneuver(-variator, -variator, 2000);                // GO FORWARD 1 seconds
}

void go_left(){
    Serial.println("---------------------------------------- GO LEFT");    
    maneuver(-variator, -variator, 200);                // GO FORWARD 1 seconds
    maneuver(little_variator, variator, 1000);          // FULL LEFT 1 seconds
    maneuver(-variator, -variator, 2000);                // GO FORWARD 1 seconds
    test_color();
}

void go_unblock_left(){
    Serial.println("---------------------------------------- UNBLOCK LEFT");    
  maneuver(variator,variator, 1000);                    // return
  maneuver(-little_variator, little_variator, 1000);   // LITTLE LEFT 1 seconds
  maneuver(-variator, -variator, 2000);                // GO FORWARD 1 seconds 
}

void go_unblock_right(){
    Serial.println("---------------------------------------- UNBLOCK LEFT");    
  maneuver(variator,variator, 1000);                    // return
  maneuver(little_variator, -little_variator, 1000);   // LITTLE Right 1 seconds
  maneuver(-variator, -variator, 2000);                // GO FORWARD 1 seconds          // GO FORWARD 1 seconds 
}

void go_return(){
    Serial.println("---------------------------------------- GO FORWARD");    
    maneuver(-variator, variator, 1000);                // return
}

void go_little_stop(){
    Serial.println("---------------------------------------- GO FORWARD");    
    maneuver(0, 0, 1000);                // return  
    //go_forward();
}
