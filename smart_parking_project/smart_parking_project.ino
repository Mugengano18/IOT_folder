#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define ULTRASONIC_SERVO_PIN 9
#define INFRARED_PIN 8
#define INFRARED_SERVO_PIN 10
#define BUZZER_PIN 11
#define MAX_DISTANCE 100
#define MIN_DISTANCE 10

Servo ultrasonicServo;
Servo infraredServo;

const int flameSensorMin = 0;     // sensor minimum
const int flameSensorMax = 1024;  // sensor maximum

int parkingSpaceCount = 5;  // Initialize parking space count

// Initialize the LCD object with the I2C address 0x27, and 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define pins for traffic lights
const int greenPin = 12;
const int redPin = 13;

void setup() {
  Serial.begin(115200);

  // Initialize the LCD
  lcd.begin();

  // Turn on the backlight
  lcd.backlight();

  // Clear the LCD screen
  // lcd.clear();

  // Ultrasonic sensor setup
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  ultrasonicServo.attach(ULTRASONIC_SERVO_PIN);  // Attach the ultrasonic servo to its pin
  ultrasonicServo.write(150);                    // Set initial position to 150 degrees

  // Infrared sensor setup
  pinMode(INFRARED_PIN, INPUT);
  infraredServo.attach(INFRARED_SERVO_PIN);  // Attach the infrared servo to its pin
  infraredServo.write(150);                  // Set initial position to 150 degrees

  // Traffic lights setup
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Ultrasonic sensor
  ultrasonicSensorLoop();

  // Infrared sensor
  infraredSensorLoop();

  // read the sensor on analog A0:
  int flameSensorReading = analogRead(A0);
  // map the sensor range (four options):
  int range = map(flameSensorReading, flameSensorMin, flameSensorMax, 0, 3);
  Serial.println(flameSensorReading);
  // range value:
  switch (range) {
    case 0:  // A fire closer than 1.5 feet away.
      Serial.println("** Close  Fire **");
      tone(BUZZER_PIN, 1000);  // Set tone with frequency 1000 Hz
      break;
    case 1:  // A fire between 1-3 feet away.
      Serial.println("**  Distant Fire **");
      tone(BUZZER_PIN, 1000);  // Set tone with frequency 1000 Hz
      delay(1000);             // Play the tone for 1 second
      noTone(BUZZER_PIN);      // Turn off the tone
      break;
    case 2:  // No fire detected.
      Serial.println("No  Fire");
      noTone(BUZZER_PIN); 
      break;
  }

  // Display parking space count
  Serial.println("Parking Spaces Available: ");
  Serial.println(parkingSpaceCount);

  lcd.setCursor(0, 0);
  lcd.print("Parking : ");
  lcd.print(parkingSpaceCount);

  // Update traffic lights based on parking space count
  updateTrafficLights();

  // Delay between reads
  delay(2000);
}

void ultrasonicSensorLoop() {
  long duration, distance;

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Ultrasonic Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If an object is detected within 10cm, tilt the ultrasonic servo by 40 degrees
  if (distance <= MIN_DISTANCE && distance > 0 && parkingSpaceCount > 0) {
    tiltUltrasonicServo();
    delay(1000);
    // Decrement parking space count
    parkingSpaceCount--;
  }
}

void tiltUltrasonicServo() {
  // Tilt the ultrasonic servo motor 40 degrees
  ultrasonicServo.write(40);
  delay(2000);
  ultrasonicServo.write(150);
  delay(1000);
}

void infraredSensorLoop() {
  // Read the state of the infrared sensor
  int state = digitalRead(INFRARED_PIN);

  if (state == LOW && parkingSpaceCount < 30) {
    Serial.println("Infrared: The obstacle is present");
    tiltInfraredServo();  // Tilt the infrared servo by 40 degrees
    // Decrement parking space count
    parkingSpaceCount++;
  } else {
    Serial.println("Infrared: The obstacle is NOT present");
    infraredServo.write(150);  // Reset servo position to 150 degrees
  }

  // Check if parking spaces are full
  if (parkingSpaceCount <= 0) {
    Serial.println("No parking space left!");
    lcd.setCursor(0, 1);
    lcd.print("No space left  ");
    // Turn on red light
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    // Stay the ultrasonic servo at position 150
    ultrasonicServo.write(150);
  } else {
    digitalWrite(redPin, LOW);
  }
}

void tiltInfraredServo() {
  infraredServo.write(40);  // Tilt the infrared servo by 40 degrees
  delay(5000);
}

void updateTrafficLights() {
  if (parkingSpaceCount > 0 && parkingSpaceCount <= 30) {
    // Turn on green light
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
  } else if (parkingSpaceCount >= 30) {
    // Turn on red light
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
  } else {
    // Turn off all lights
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, LOW);
  }
}
