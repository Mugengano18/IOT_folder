#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pins for ultrasonic sensor
const int trigPin = 2;
const int echoPin = 3;

// Define maximum distance measurable by the sensor in centimeters
const int maxDistance = 300;

// Variables for duration and distance calculation
long duration;
int distance;
int reversedDistance;

// Initialize the LCD object with the I2C address 0x27, and 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the LCD
  lcd.begin();

  // Turn on the backlight
  lcd.backlight();

  // Clear the LCD screen
  lcd.clear();

  // Set trigPin as an OUTPUT
  pinMode(trigPin, OUTPUT);
  // Set echoPin as an INPUT
  pinMode(echoPin, INPUT);
}

void loop() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distance = duration * 0.034 / 2;

  // Reverse the distance
  reversedDistance = maxDistance - distance;

  // Print the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(reversedDistance);
  Serial.println(" cm");

  // Display the distance on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(reversedDistance);
  lcd.print(" cm   ");

  // Delay before next reading
  delay(2000);
}
