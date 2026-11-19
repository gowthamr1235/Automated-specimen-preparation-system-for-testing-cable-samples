/*
  Automated Specimen Preparation System for Testing Cable Samples
  Arduino UNO - demonstration prototype

  Functions:
  - Starts cable feed motor when START button is pressed.
  - Stops the motor when the IR sensor detects the cable.
  - Moves a small servo as a demonstration actuator.
  - Shows the system status on a 16x2 I2C LCD.

  IMPORTANT:
  This is demonstration code only. The IR sensor detects presence/position;
  it does NOT measure an accurate specimen length.
  Do not connect this servo to a real cutting blade or use this sketch
  to operate an unguarded cutting machine. Real equipment needs suitable
  industrial controls, guarding, emergency stop, and safety interlocks.

  Libraries:
  - Servo
  - LiquidCrystal_I2C
*/

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo cutter;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// L298N motor driver
const int IN1 = 5;
const int IN2 = 6;
const int ENA = 9;

// Inputs
const int IR_SENSOR = 2;
const int START_BUTTON = 3;

// Demonstration servo
const int SERVO_PIN = 10;

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

void feedMotor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150); // PWM speed: 0 to 255
}

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  pinMode(START_BUTTON, INPUT_PULLUP);

  stopMotor();

  cutter.attach(SERVO_PIN);
  cutter.write(0);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Cable Specimen");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Button uses INPUT_PULLUP, so LOW means pressed.
  if (digitalRead(START_BUTTON) == LOW) {
    delay(40); // basic button debounce

    if (digitalRead(START_BUTTON) == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Feeding cable");

      feedMotor();

      // Common IR obstacle sensors output LOW when an object is detected.
      // Change LOW to HIGH if your sensor module has opposite logic.
      while (digitalRead(IR_SENSOR) == HIGH) {
        // Wait for cable detection.
        // Add a properly designed timeout and emergency-stop circuit
        // before using any real motorized equipment.
      }

      stopMotor();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cable detected");
      delay(1000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Servo demo");

      // Demonstration movement only; not a real cutting mechanism.
      cutter.write(90);
      delay(700);
      cutter.write(0);
      delay(700);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sample ready");
      delay(2000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("System Ready");

      // Wait for button release so one press triggers one cycle.
      while (digitalRead(START_BUTTON) == LOW) {
        delay(10);
      }
    }
  }
}
