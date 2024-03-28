#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5); // Adjust pin numbers as needed

#define Password_size 6
char Data[Password_size];
char pass[Password_size] = "12345";
const int DISTANCE_THRESHOLD = 10; // centimeters\

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

const int TRIG_PIN = 11; // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN = 12; // Arduino pin connected to Ultrasonic Sensor's ECHO pin

byte rowPin[ROWS] = {9,8,7,6};
byte colPin[COLS] = {5,4,3,2};
Keypad customKeypad = Keypad(makeKeymap(keys), rowPin, colPin, ROWS, COLS);

int lampPin = 10;
int servoPin = 13;
int position = 0;

void setup() {
  pinMode(lampPin, OUTPUT);
  myservo.attach(servoPin);
  
  pinMode(TRIG_PIN, OUTPUT); // set arduino pin to output mode
  pinMode(ECHO_PIN, INPUT);  // set arduino pin to input mode

  lcd.begin(16, 2);
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  float duration_us = pulseIn(ECHO_PIN, HIGH);
  float distance_cm = 0.017 * duration_us;
  if (distance_cm < DISTANCE_THRESHOLD) {
    
  digitalWrite(lampPin, HIGH);
  CheckAccess();

  } else {

    lcd.clear();
    lcd.setCursor(0,0);
    digitalWrite(lampPin, LOW);
    myservo.write(0);  
    lcd.print("Server Room");

  }  
}

void clearData() {
  for (int i = 0; i < Password_size; i++) {
    Data[i] = 0;
  }
}

void ServoOpen() {
  for (position = 0; position <= 180; position++) {
    myservo.write(position);
    delay(15);
  }
}

void ServoClose() {
  for (position = 180; position >= 0; position--) {
    myservo.write(position);
    delay(15);
  }
}

void CheckAccess() {
  lcd.setCursor(0, 0);
  lcd.print("Enter Password: ");
  char customKey = customKeypad.getKey();
  if (customKey) {
    Data[strlen(Data)] = customKey;
    lcd.setCursor(strlen(Data) - 1, 1);
    lcd.print(customKey);
  }
  if (strlen(Data) >= Password_size - 1)  {
    if (strcmp(Data, pass) == 0) {
      lcd.clear();
      lcd.print("Access Granted");
      ServoOpen();
      delay(5000);
      ServoClose();
    } else {
      lcd.clear();
      lcd.print("Wrong Password");
      delay(5000);
    }
    clearData();
  }
}