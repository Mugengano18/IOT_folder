#include <Servo.h>

const int INFRARED_PIN = 8;
const int IFRARED_SERVO_PIN = 10; // Assuming servo is connected to pin 9

Servo InfraredServoMotor; // Create a servo object to control a servo motor

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bits per second
  pinMode(INFRARED_PIN, INPUT); // initialize the Arduino's pin as an input
  InfraredServoMotor.attach(IFRARED_SERVO_PIN); // attaches the servo on pin 9 to the servo object
  InfraredServoMotor.write(150); // set initial position to 150 degrees
}

void loop() {
  // read the state of the input pin
  int state = digitalRead(INFRARED_PIN);

  if (state == LOW) {
    Serial.println("The obstacle is present");
    tiltInfraredServo(); // Tilt the servo by 40 degrees
  } else {
    Serial.println("The obstacle is NOT present");
    InfraredServoMotor.write(150); // Reset servo position to 150 degrees
  }

  delay(2000);
}

void tiltInfraredServo() {
  InfraredServoMotor.write(40); // Tilt the servo by adding 40 degrees to the initial position (150)
  delay(5000); // Delay to let the servo move
  
}
