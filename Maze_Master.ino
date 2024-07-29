// L298 (H-Driver) Pins 
#define enA 5  // Enable1
#define in1 6  // Motor1
#define in2 7  // Motor1
#define in3 8  // Motor2
#define in4 9  // Motor2
#define enB 10 // Enable2 

// IR Sensors Pins
#define L_S A0 // Left
#define R_S A1 // Right

// Ultrasonic Pins
#define echo A2    // Echo
#define trigger A3 // Trigger

// Servo Motor Pins
#define servo A5 // (Ultrasonic Holder)

int Set=20; // Ultra Sonic Stopping Distance
int distance_L, distance_F, distance_R;  // Variables for measured distances Left, Forward, Right 

void setup(){ // Setup code, which runs once

Serial.begin(9600); // Start serial communication at 9600bps

pinMode(R_S, INPUT); // Declare Right IR sensor as input  
pinMode(L_S, INPUT); // Declare Left  IR sensor as input

pinMode(echo, INPUT );    // Declare Ultrasonic sensor Echo    pin as input
pinMode(trigger, OUTPUT); // Declare Ultrasonic sensor Trigger pin as Output  

pinMode(enA, OUTPUT); // Declare as output for L298 Pin enA 
pinMode(in1, OUTPUT); // Declare as output for L298 Pin in1
pinMode(in2, OUTPUT); // Declare as output for L298 Pin in2 
pinMode(in3, OUTPUT); // Declare as output for L298 Pin in3   
pinMode(in4, OUTPUT); // Declare as output for L298 Pin in4 
pinMode(enB, OUTPUT); // Declare as output for L298 Pin enB 

analogWrite(enA, 215); // Write The Duty Cycle 0 to 255 enAble Pin A for Motor1 Speed 
analogWrite(enB, 215); // Write The Duty Cycle 0 to 255 enAble Pin B for Motor2 Speed 

pinMode(servo, OUTPUT); // Declare Servo Motor pin as output

for (int angle = 70; angle <= 140; angle += 5) { // Sweep the servo from 70 to 140 degrees in 5-degree increments
  servoPulse(servo, angle);}   // Call the function to set the servo to the current angle

for (int angle = 140; angle >= 0; angle -= 5) { // Sweep the servo back from 140 to 0 degrees in 5-degree decrements
  servoPulse(servo, angle);}   // Call the function to set the servo to the current angle

for (int angle = 0; angle <= 70; angle += 5) { // Sweep the servo from 0 to 70 degrees in 5-degree increments
  servoPulse(servo, angle);}   // Call the function to set the servo to the current angle

distance_F = Ultrasonic_read(); // Read the distance from the ultrasonic sensor
delay(500); // Introduce a delay of 500 milliseconds
} // End of Setup code

void loop(){  // Main Loop Function
distance_F = Ultrasonic_read();   // Read distance from Ultrasonic sensor
Serial.print("D F=");Serial.println(distance_F);   // Print distance value to Serial monitor
// If both right and left sensors are on white color then it will call forward function
 if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)){ // Check if both right and left sensors detect an obstacle
    // Check if the front distance is greater than a certain threshold ('Set')
  if(distance_F > Set){forward();}     // If front distance is sufficient, move forward
                  else{Check_side();}  // If front distance is not sufficient, check the side for further action
 }  
 
// Else, If Right Sensor is Black and Left Sensor is White then it will call turn Right function
else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)){turnRight();}  

// Else, If Right Sensor is White and Left Sensor is Black then it will call turn Left function
else if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)){turnLeft();} 
    
delay(10);  // Introduce a delay of 10 milliseconds
}

void servoPulse (int pin, int angle){ // Function to generate a servo pulse with specified angle
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);   // Activate the servo
 delayMicroseconds(pwm);    // Introduce a microsecond delay based on the 'pwm' variable
 digitalWrite(pin, LOW);    // Deactivate the servo
 delay(50);   // Introduce a delay for the servo refresh cycle

}

long Ultrasonic_read(){ // Function to read distance from Ultrasonic sensor
  // Trigger a pulse to initiate ultrasonic sensor measurement
  digitalWrite(trigger, LOW); // Set trigger pin to LOW
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(trigger, HIGH); // Set trigger pin to HIGH to start the ultrasonic pulse
  delayMicroseconds(10);  // Keep the trigger pin HIGH for 10 microseconds
  // Measure the pulse duration and convert it to distance in centimeters
  long time = pulseIn (echo, HIGH); // Measure the duration of the pulse from the echo pin
  return time / 29 / 2; // Convert pulse duration to distance in centimeters
}

void compareDistance(){ // Function to compare distances and adjust the robot's path
    if(distance_L > distance_R){   // If left distance is more than right distance, perform a left turn; otherwise, perform a right turn
                     // Execute left turn sequence
        turnLeft();     // Turn the robot left
        delay(350);     // Wait for 350 milliseconds
        forward();      // Move the robot forward
        delay(750);     // Wait for 750 milliseconds
        turnRight();    // Turn the robot right
        delay(350);     // Wait for 350 milliseconds
        forward();      // Move the robot forward
        delay(750);     // Wait for 750 milliseconds
        turnRight();    // Turn the robot right
        delay(350);     // Wait for 350 milliseconds
    } else {         // Execute right turn sequence
        turnRight();    // Turn the robot right
        delay(350);     // Wait for 350 milliseconds
        forward();      // Move the robot forward
        delay(750);     // Wait for 750 milliseconds
        turnLeft();     // Turn the robot left
        delay(350);     // Wait for 350 milliseconds
        forward();      // Move the robot forward
        delay(750);     // Wait for 750 milliseconds
        turnLeft();     // Turn the robot left
        delay(350);     // Wait for 350 milliseconds
    }
}

void Check_side(){ // Function to check distances on the robot's sides
    Stop();   // Stop the robot
    delay(100); // Introduce a delay
 for (int angle = 70; angle <= 140; angle += 5)  {   // Sweep the servo from 70 to 140 degrees to check the left side
   servoPulse(servo, angle);  } // Call the function to set the servo to the current angle
    delay(300);   // Introduce a delay 
    distance_L = Ultrasonic_read(); // Read the distance on the left side
    Serial.print("D L=");Serial.println(distance_L); // Print the distance value for the Left sensor
    delay(100);   // Introduce a delay
  for (int angle = 140; angle >= 0; angle -= 5)  {   // Sweep the servo from 140 to 0 degrees to check the right side
   servoPulse(servo, angle);  } // Call the function to set the servo to the current angle
    delay(500);  // Introduce a delay
    distance_R = Ultrasonic_read(); // Read the distance on the right side
    Serial.print("D R=");Serial.println(distance_R); // Print the distance value for the Right sensor
    delay(100);   // Introduce a delay
 for (int angle = 0; angle <= 70; angle += 5)  {   // Sweep the servo from 0 to 70 degrees to reset its position
   servoPulse(servo, angle);  } // Call the function to set the servo to the current angle
    delay(300);   // Introduce a delay
    compareDistance();   // Compare distances and adjust the robot's path
}

void forward(){  // Forward Function
digitalWrite(in1, LOW);  // Left Motor backward Pin 
digitalWrite(in2, HIGH); // Left Motor forward Pin 
digitalWrite(in3, HIGH); // Right Motor forward Pin 
digitalWrite(in4, LOW);  // Right Motor backward Pin 
}

void backward(){ // Backward Function
digitalWrite(in1, HIGH); // Left Motor backward Pin 
digitalWrite(in2, LOW);  // Left Motor forward Pin 
digitalWrite(in3, LOW);  // Right Motor forward Pin 
digitalWrite(in4, HIGH); // Right Motor backward Pin 
}

void turnRight(){ // Turn Right Function
digitalWrite(in1, LOW);  // Left Motor backward Pin 
digitalWrite(in2, HIGH); // Left Motor forward Pin 
digitalWrite(in3, LOW);  // Right Motor forward Pin 
digitalWrite(in4, HIGH); // Right Motor backward Pin 
}

void turnLeft(){ // Turn Left Function
digitalWrite(in1, HIGH); // Left Motor backward Pin 
digitalWrite(in2, LOW);  // Left Motor forward Pin 
digitalWrite(in3, HIGH); // Right Motor forward Pin 
digitalWrite(in4, LOW);  // Right Motor backward Pin 
}

void Stop(){ // Stop Function
digitalWrite(in1, LOW); // Left Motor backward Pin 
digitalWrite(in2, LOW); // Left Motor forward Pin 
digitalWrite(in3, LOW); // Right Motor forward Pin 
digitalWrite(in4, LOW); // Right Motor backward Pin 
}


