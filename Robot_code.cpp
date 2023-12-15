#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> 

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  125 // Minimum pulse length count (out of 4096)
#define SERVOMAX  575 // Maximum pulse length count (out of 4096)

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  // Initialize servos to starting positions
  initializeServos();
}

void loop() {
  // Step 1: Lower the arm to the box
  moveServo(3, angleToPulse(120), angleToPulse(90)); // Example angles, adjust as needed
  moveServo(2, angleToPulse(100), angleToPulse(120)); 

  // Step 2: Close the gripper
  moveServo(0, angleToPulse(10), angleToPulse(30)); 

  // Step 3: Lift the arm
  moveServo(3, angleToPulse(90), angleToPulse(120));
  moveServo(2, angleToPulse(120), angleToPulse(100)); 

  // Step 4: Rotate the base
  moveServo(5, angleToPulse(30), angleToPulse(60)); 

  // Step 5: Lower the arm to place the box
  moveServo(3, angleToPulse(120), angleToPulse(90)); 
  moveServo(2, angleToPulse(100), angleToPulse(120)); 

  // Step 6: Open the gripper
  moveServo(0, angleToPulse(30), angleToPulse(10)); 

  // Step 7: Return to starting position
  initializeServos();

  delay(10000); // Wait before next cycle
}

void initializeServos() {
  pwm.setPWM(0, 0, angleToPulse(10));
  pwm.setPWM(1, 0, angleToPulse(65));
  pwm.setPWM(2, 0, angleToPulse(120));
  pwm.setPWM(3, 0, angleToPulse(100));
  pwm.setPWM(5, 0, angleToPulse(30));
}

void moveServo(int channel, int startPulse, int endPulse) {
  int step = startPulse < endPulse ? 1 : -1;
  for (int pulse = startPulse; pulse != endPulse; pulse += step) {
    pwm.setPWM(channel, 0, pulse);
    delay(20); // Slow movement, adjust delay as needed
  }
}

int angleToPulse(int ang){
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
  Serial.print("Angle: "); Serial.print(ang);
  Serial.print(" pulse: "); Serial.println(pulse);
  return pulse;
}