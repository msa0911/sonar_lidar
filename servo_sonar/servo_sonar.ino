

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <std_msgs/Float64.h>
#include <Servo.h> 
#include <std_msgs/UInt16.h>


//Set up the ros node and publisher
std_msgs::Float64 Distance;
ros::Publisher pub_sonar("sonar",&Distance);
ros::NodeHandle nh;

int trigPin = 4;    // Trigger
int echoPin = 5;    // Echo

// defines variables
long duration;
float distance;
Servo servo;

void servo_cb( const std_msgs::UInt16& cmd_msg){
  servo.write(cmd_msg.data); //set servo angle, should be from 0-180  
}

ros::Subscriber<std_msgs::UInt16> sub("servo", servo_cb);

void setup() {
 
  Serial.begin(57600); // Starts the serial communication
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  nh.initNode();
  nh.advertise(pub_sonar);
  nh.subscribe(sub); 
  servo.attach(9); //attach it to pin 9
}




void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
//publishing data

Distance.data=distance;
pub_sonar.publish(&Distance);
nh.spinOnce();

delay(100);
}
