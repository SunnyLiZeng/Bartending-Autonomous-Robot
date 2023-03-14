#include <Servo.h>

Servo hand;
Servo elbow; //Will be referred to as B
Servo shoulder; //Will be referred to as A
Servo base;

//These variables are 0-180 but are 0-270 since the servos are 270 degrees max
int handPos = 90; //Hand position starts at 90
int elbowPos = 51; //Elbow position starts at 180
int shoulderPos = 120; //Shoulder position starts at 0
int basePos = 0;

int handAngle = 90; //Preferred hand angle
int elbowAngle = 51; //Preferred elbow angle
int shoulderAngle = 120;  //Preferred shoulder angle
int baseAngle = 0;

//Servo angle variables in actual degrees
const int HAND = 135;
const int ELBOW = 270;
const int SHOULDER = 0;

bool flagtest = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Begin UART communication
  hand.attach(9); //Digital Pin  9
  elbow.attach(10); //Digital Pin 10
  shoulder.attach(11);  //Digital Pin 11
  base.attach(6);

  //Write starting positions to servos
  hand.write(handPos);
  elbow.write(elbowPos);
  shoulder.write(shoulderPos);
  base.write(basePos);

  pinMode(2, INPUT); //Reset Position Pin
  while(!Serial){}
  Serial.println("69");
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(2, HIGH);  //Enable MOSFET
  
  if(Serial.available() >0){
    flagtest = true;
    
    double Ax = 0;  //x-axis base origin offset
    double Ay = 0;  //y-axis base origin offset
    
    String input = Serial.readString(); //Read UART input in the form of "x,y,theta" coordiantes for end effector

    int split = input.indexOf(",");
    
    String strX = input.substring(0, split);

    input = input.substring(split + 1, input.length());
    split = input.indexOf(",");
    String strY = input.substring(0, split);
    String theta = input.substring(split + 1, input.length() - 1);

    Serial.println(strX);
    Serial.println(strY);
    Serial.println(theta);
    
    baseAngle = theta.toInt();

    Serial.println(baseAngle);
    
    baseAngle = map(baseAngle, 0, 270, 0, 180);

    Serial.println(baseAngle);
    
    double Cx = strX.toDouble();  //End Effector Coordinates
    double Cy = strY.toDouble();  //End Effector Coordinates

    double shoulderLength = 20; //Length C in Centimeters
    double elbowLength = 20;    //Length A in Centimeters

    double lengthB = sqrt(sq(Cx) + sq(Cy)); //Distance from origin to end effector coordinate
    
    double angleA = acos((sq(lengthB)+sq(shoulderLength)-sq(elbowLength)) / (2*lengthB*shoulderLength)) + atan((Cy - Ay) / (Cx - Ax)); //Shoulder Angle in radians
    double angleB = PI - acos((sq(elbowLength)+sq(shoulderLength)-sq(lengthB)) / (2*elbowLength*shoulderLength)); //Elbow Angle in radians

    angleA = angleA * (180/PI); //Convert shoulder angle to degrees
    angleB = angleB * (180/PI); //Convert elbow angle to degrees

    //Check boundary condition for origin
    
    if(Cx < 0.001 && Cx > -0.001 && Cy < 0.001 && Cy > -0.001){
      angleA = 180;
      angleB = 180;
    }

    //Check if End Effector is physically out of reach of the robotic arm
    if(shoulderLength + elbowLength < lengthB){
      angleA = atan((Cy - Ay) / (Cx - Ax)) * (180/PI);
      angleB = 0;
      
      shoulderAngle = angleA;
      elbowAngle = angleB;
      elbowAngle = ELBOW - elbowAngle;

      //handAngle = HAND - int(angleB) + int(angleA);

      //Map angles to 0-180 range for the servo library
      shoulderAngle = map(shoulderAngle, 0, 270, 0, 180);
      elbowAngle = map(elbowAngle, 0, 270, 0, 180);
      //handAngle = map(handAngle, 0, 270, 0, 180);
    }
    else{
      //Serial.println(angleA);
      //Serial.println(angleB);
      shoulderAngle = angleA;
      elbowAngle = angleB;
      elbowAngle = ELBOW - elbowAngle;

      //handAngle = HAND - int(angleB) + int(angleA);

      //Map angles to 0-180 range for the servo library
      shoulderAngle = map(shoulderAngle, 0, 270, 0, 180);
      elbowAngle = map(elbowAngle, 0, 270, 0, 180);
      //handAngle = map(handAngle, 0, 270, 0, 180);

      //Serial.println(shoulderAngle);
      //Serial.println(elbowAngle);
    }
  }

  //Check servo position vs preferred position
  while((elbowAngle != elbowPos) || (shoulderAngle != shoulderPos) || (baseAngle != basePos)){
    //Move elbow segment
    if(elbowPos < elbowAngle){
      elbowPos++;
    }
    else if(elbowPos > elbowAngle){
      elbowPos--;
    }
    else{
    }

    //Move shoulder segment
    if(shoulderPos < shoulderAngle){
      shoulderPos++;
    }
    else if(shoulderPos > shoulderAngle){
      shoulderPos--;
    }
    else{
    }

    handPos = 90 - (180 - elbowPos) + shoulderPos;  //Level hand segment

    if(basePos < baseAngle){
      basePos++;
    }
    else if(basePos > baseAngle){
      basePos--;
    }
    else{
    }
    
    //Move servos
    hand.write(handPos);
    elbow.write(elbowPos);
    shoulder.write(shoulderPos);
    base.write(basePos);
    
    delay(40); //Slow down servo movement
    if((elbowAngle == elbowPos) && (shoulderAngle == shoulderPos) && (baseAngle == basePos)){
      Serial.println("69");
      flagtest = false;
    }
  }
  if(flagtest){
    Serial.println("69");
    flagtest = false;
  }
  //Serial.println(digitalRead(2));
  
  if(digitalRead(2) == 0){
    handAngle = 90; //Preferred hand angle
    elbowAngle = 51; //Preferred elbow angle
    shoulderAngle = 120;  //Preferred shoulder angle
    baseAngle = 0;
    while((elbowAngle != elbowPos) || (shoulderAngle != shoulderPos) || (baseAngle != basePos) || (handAngle != handPos)){
      if(handPos < handAngle){
        handPos++;
      }
      else if(handPos > handAngle){
        handPos--;
      }
      else{
      }
      
      //Move elbow segment
      if(elbowPos < elbowAngle){
        elbowPos++;
      }
      else if(elbowPos > elbowAngle){
        elbowPos--;
      }
      else{
      }
  
      //Move shoulder segment
      if(shoulderPos < shoulderAngle){
        shoulderPos++;
      }
      else if(shoulderPos > shoulderAngle){
        shoulderPos--;
      }
      else{
      }
  
      if(basePos < baseAngle){
        basePos++;
      }
      else if(basePos > baseAngle){
        basePos--;
      }
      else{
      }
      
      //Move servos
      hand.write(handPos);
      elbow.write(elbowPos);
      shoulder.write(shoulderPos);
      base.write(basePos);
      
      delay(40); //Slow down servo movement
    }
  }
  
  //Serial.println(String(handAngle) + "," + String(elbowAngle) + "," + String(shoulderAngle));
}
