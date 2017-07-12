 //Important Functions
 /*drive.forward(speed,time_delay);  drive.full_stop(time_delay);  drive.turn_right(speed,time_delay);  drive.full_stop(time_delay);
  drive.turn_left(speed,time_delay);   drive.full_stop(time_delay);   drive.backward(speed,time_delay);

  servoNose.write(val);                  // sets the servo position according to the scaled value

  sonar.ping_cm();  // Returns distance in cm of obstacle in front
  */
  
//Libraries
#include <L298N.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

const int ENA = 6;
const int IN1 = 8;
const int IN2 = 7;
const int IN3 = 2;
const int IN4 = 4;
const int ENB = 3;


L298N drive(ENA,IN1,IN2,IN3,IN4,ENB); 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo servoNose;


int time_delay = 500;
int speed = 150;

bool way_clear = 0;

void setup()
{
   servoNose.attach(9);  // attaches the servo on pin 9 to the servo object
  //Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop()
{
  check_obstacle();
  
  while (way_clear == 1)
  {
	drive.backward(speed,time_delay);
  }
  drive.full_stop(2);
  find_way();
  
  
}

void check_obstacle()
{
	int  dist = sonar.ping_cm;
	delay(50); 
	if (dist < 10) {
		way_clear = 0;
	}
	else {
		way_clear = 1;
	}
}
 void find_way()
 {
	 int start_angle = 90; // servo origin position
	 servoNose.write(start_angle);
	 int angle;
	 int dir=1;  //right
	 int obstacle;
	 
	 for (int i=0;i<10;i++)
	 {
		  angle = start_angle + (i*9);		//angle of rotation
		  servoNose.write(angle);
		  delay(2);  // Wait for servo to rotate
		  check_obstacle();
		  if (way_clear == 1)
		  {
			  servoNose.write(start_angle);
			  i=11; // end loop
			  
		  }
		  else {
			  i=0;
			  start_angle = 0;
			  dir = 2; // left
		  }
	 }
	 turn(dir,45); // 
 }
 
 void turn(int dir,int angle)
 {
	 switch (dir)
	 {
		 case 1: 
			drive.turn_left(100,angle);
		 break;
		 case 2:
		  drive.turn_right(100,angle);
		 break;
		 default:
		 break;
	 }
 }