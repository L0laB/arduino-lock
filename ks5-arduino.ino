/*
  Controlling a servo position using a potentiometer (variable resistor)
  by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

  modified on 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Knob
*/

/*#include <Servo.h>

  Servo myservo;  // create servo object to control a servo

  int potpin = 0;  // analog pin used to connect the potentiometer
  int val;    // variable to read the value from the analog pin

  void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  }

  void loop() {
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
  }
*/

#include <Keypad.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 11;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int positionClose = 0;
const int positionOpen = 90;


char* password = "427";  // change the password here, just pick any 3 numbers
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // open connection to serial monitor
  Serial.begin(9600);
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  // shows pressed keys to debug
  char keypressed = keypad.getKey();
  if (keypressed != NO_KEY) {
    Serial.println(keypressed);
  }


  char key = keypad.getKey();
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed: If it is pressed, the buttonState is HIGH
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    myservo.write(positionOpen);
    delay(1500);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    myservo.write(positionClose);
  }
}
