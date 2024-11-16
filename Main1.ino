#include <Servo.h>

#define IR_PIN 7
#define PIR_PIN 4
#define LED_WHITE 11    // white
#define LED_WHITE2 12   // white
#define LED_BLUE 5      // blue
#define LED_BLUE2 6     // blue
#define LED_ORANGE 13   // orange

Servo motor;

//Hold the door
bool doorOpen = false;

//Previous State of the IR Sensor
int prevSensorState = 0;

// To store the last time motion was detected
unsigned long previousMillis = 0;

// Delay time in milliseconds
const long interval = 10000;

void setup() {
  motor.attach(10);
  pinMode(IR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_WHITE, OUTPUT);   // White
  pinMode(LED_WHITE2, OUTPUT);  // Whihte
  pinMode(LED_BLUE, OUTPUT);    // Blue
  pinMode(LED_BLUE2, OUTPUT);   // Blue
  pinMode(LED_ORANGE, OUTPUT);  //Orange
  
  // Bluetooth Commiunication
  Serial.begin(9600);   
}

void loop() {

  // IR Sensor Input
  int ir_Sensor = digitalRead(IR_PIN);

  // PIR Sensor Input
  int pir_Sensor = digitalRead(PIR_PIN);

  // Servo Delay Time
  int time = 3000;

  // Bluetooth Input initialization
  String bt_Command = "";  

  // Read the Bluetooth input
  if (Serial.available()) {
    bt_Command = Serial.readString();  // Read string from Bluetooth
    bt_Command.trim();  // Remove any trailing newline or carriage return characters
  }

  // If IR sensor value is HIGH (1) and previous state was LOW (0), rotate to 90 to 0 degrees
  if (ir_Sensor == HIGH && prevSensorState == LOW) {
    for (int pos = 90; pos >= 0; pos--) {  
      motor.write(pos);
      delay(15);  
    }
  }

  // If IR sensor value is LOW (0) and previous state was HIGH (1), rotate back to 0 to 90 degrees
  if (ir_Sensor == LOW && prevSensorState == HIGH) {
    for (int pos = 0; pos <= 90; pos++) {  
      motor.write(pos);
      delay(15);  
    }
    delay(time);
  }

  prevSensorState = ir_Sensor;

  // Light turn on function
  unsigned long currentMillis = millis();  // To do multitasking

  // If motion is detected, turn on the lights and reset the timer
  if (pir_Sensor == HIGH) {
    digitalWrite(LED_WHITE, HIGH);
    digitalWrite(LED_WHITE2, HIGH);
    previousMillis = currentMillis;  // Reset the timer
  }

  // If no motion is detected and 10 seconds have passed, turn off the lights
  if (currentMillis - previousMillis >= interval) {
    digitalWrite(LED_WHITE, LOW);
    digitalWrite(LED_WHITE2, LOW);
  }

  // Handle Bluetooth Commands
  if (bt_Command.length() > 0) {  // If there is a Bluetooth command
    Serial.print(bt_Command);     // Debugging - print the received command to Serial Monitor

    // Turn on White LEDs
    if (bt_Command == "A") {
      digitalWrite(LED_WHITE, HIGH);
      digitalWrite(LED_WHITE2, HIGH);
    } 
    // Turn off White LEDs
    else if (bt_Command == "B") {
      digitalWrite(LED_WHITE, LOW);
      digitalWrite(LED_WHITE2, LOW);
    } 
    // Turn on Blue LEDs
     else if (bt_Command == "C") {
      digitalWrite(LED_BLUE, HIGH);
      digitalWrite(LED_BLUE2, HIGH);
    } 
    // Turn off Blue LEDs
     else if (bt_Command == "D") {
      
      // Turn off yellow LEDs
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_BLUE2, LOW);
    } 
    // Turn on Orange LED
     else if (bt_Command == "E") {
      digitalWrite(LED_ORANGE, HIGH);
    } 
    // Turn on Orange LEDs
     else if (bt_Command == "F") {
      digitalWrite(LED_ORANGE, LOW);
    } 
    //Door Open 
     else if (bt_Command == "G") {
      if (!doorOpen) {  // Only open if the door is currently closed
        for (int pos = 0; pos <= 90; pos++) {  
          motor.write(pos);
          delay(15);  
        }
        doorOpen = true;  // Set the door state to open
      }
    } 
    //Door Close
     else if (bt_Command == "H") {
      if (doorOpen) {  // Only close if the door is currently open
        for (int pos = 90; pos >= 0; pos--) {  
          motor.write(pos);
          delay(15);  
        }
        doorOpen = false;  // Set the door state to closed
      }
    }
  }
}
