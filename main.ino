#include <AFMotor.h>                             
#include <Servo.h>                                

AF_DCMotor rightBack(1);                          
AF_DCMotor rightFront(2);
AF_DCMotor leftFront(3);
AF_DCMotor leftBack(4);
Servo servoLook;
Servo myservo;                                  

byte trig = 2;                                   
byte echo = 13;
const int left = 24;
const int front = 26;
const int right = 28;
byte maxDist = 150;                               
byte stopDist = 50;                              
float timeOut = 2*(maxDist+10)/100/340*1000000; 

int motorSpeed = 140;                             
int motorOffset = 30;                             
int turnSpeed = 80;  

void setup() 
{
  rightBack.setSpeed(motorSpeed);                 
  rightFront.setSpeed(motorSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset);
  leftBack.setSpeed(motorSpeed+motorOffset);
  rightBack.run(RELEASE);                        
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
  servoLook.attach(10);
  myservo.attach(9);                         
  pinMode(trig,OUTPUT);                           
  pinMode(echo,INPUT);
  pinMode(left,INPUT);
  pinMode(front,INPUT);
  pinMode(right,INPUT);
}

int getDistance()                                   
{
  unsigned long pulseTime;                          
  int distance;                                    
  digitalWrite(trig, HIGH);                         
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pulseTime = pulseIn(echo, HIGH, timeOut);         
  distance = (float)pulseTime * 340 / 2 / 10000;    
  return distance;
}
void accelerate()                                 
{
  for (int i=0; i<motorSpeed; i++)                
  {
    rightBack.setSpeed(i);                        
    rightFront.setSpeed(i);
    leftFront.setSpeed(i+motorOffset);
    leftBack.setSpeed(i+motorOffset);
    delay(10);
  }
}

void decelerate()                                 
{
  for (int i=motorSpeed; i!=0; i--)               
  {
    rightBack.setSpeed(i);                        
    rightFront.setSpeed(i);
    leftFront.setSpeed(i+motorOffset);
    leftBack.setSpeed(i+motorOffset); 
    delay(10);
  }
}

void moveForward()                                
{
  rightBack.run(FORWARD);
  rightFront.run(FORWARD);
  leftFront.run(FORWARD);
  leftBack.run(FORWARD);
}

void stopMove()                                   
{
  rightBack.run(RELEASE);
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
}

void turnLeft(int duration)                                 
{
  rightBack.setSpeed(motorSpeed+turnSpeed);                 
  rightFront.setSpeed(motorSpeed+turnSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset+turnSpeed);
  leftBack.setSpeed(motorSpeed+motorOffset+turnSpeed);
  rightBack.run(FORWARD);
  rightFront.run(FORWARD);
  leftFront.run(BACKWARD);
  leftBack.run(BACKWARD);
  delay(duration);
  rightBack.setSpeed(motorSpeed);                           
  rightFront.setSpeed(motorSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset);
  leftBack.setSpeed(motorSpeed+motorOffset);
  rightBack.run(RELEASE);
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
  
}

void turnRight(int duration)                                
{
  rightBack.setSpeed(motorSpeed+turnSpeed);                 
  rightFront.setSpeed(motorSpeed+turnSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset+turnSpeed);
  leftBack.setSpeed(motorSpeed+motorOffset+turnSpeed);
  rightBack.run(BACKWARD);
  rightFront.run(BACKWARD);
  leftFront.run(FORWARD);
  leftBack.run(FORWARD);
  delay(duration);
  rightBack.setSpeed(motorSpeed);                           
  rightFront.setSpeed(motorSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset);
  leftBack.setSpeed(motorSpeed+motorOffset);
  rightBack.run(RELEASE);
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
}


int checkDirection()                                            
{
  int distances [2] = {0,0};                                    
  int turnDir = 1;                                              
  servoLook.write(180);                                         
  delay(500);
  distances [0] = getDistance();                                
  servoLook.write(0);                                           
  delay(1000);
  distances [1] = getDistance();                                
  if (distances[0]>=200 && distances[1]>=200)                   
    turnDir = 0;
  else if (distances[0]<=stopDist && distances[1]<=stopDist)    
    turnDir = 1;
  else if (distances[0]>=distances[1])                          
    turnDir = 0;
  else if (distances[0]<distances[1])                           
    turnDir = 2;
  return turnDir;
}

void loop() 
{
  int pos;
  myservo.write(100);
  delay(750);
  servoLook.write(90);                            
  delay(750);
  int distance = getDistance();
  int fireValue_1 = left;
  int fireValue_2 = front;
  int fireValue_3 = right;                   
  if(distance >= stopDist && digitalRead(left) ==1 && digitalRead(front)==1 && digitalRead(right) ==1)                        
  {
    moveForward();
  }
  while(distance >= stopDist)                     
  {
    distance = getDistance();
    delay(250);
  }
  stopMove();                                     
  int turnDir = checkDirection();                 
  Serial.print(turnDir);
  switch (turnDir)                                
  {
    case 0:                                       
      turnLeft (1800);
      break;
    case 1:                                       
      turnLeft (2000);
      break;
    case 2:                                       
      turnRight (1800);
      break;
  }
  if (digitalRead(left) ==0 && digitalRead(front) ==1  && digitalRead(right) ==1)
  {
    stopMove();
    turnLeft (1100);
    for (pos = 100; pos <= 20; pos += 1) {
    myservo.write(pos);             
    delay(15); 
    }
  }
  else if (digitalRead(left) ==0 && digitalRead(front) ==1  && digitalRead(right) ==1)
  {
    stopMove();
    for (pos = 100; pos <= 20; pos += 1) {
    myservo.write(pos);             
    delay(15);
  }
  } else if (digitalRead(left) ==1 && digitalRead(front) ==1  && digitalRead(right) ==0)
  {
    turnRight (1100);
    stopMove();
    for (pos = 100; pos <= 20; pos += 1) {
    myservo.write(pos);             
    delay(15);
  }
  }}

