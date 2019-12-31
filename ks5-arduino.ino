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

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 11;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int positionClose = 0;
const int positionOpen = 90;

const byte numRows = 4;         //number of rows on the keypad
const byte numCols = 4;         //number of columns on the keypad

char keymap[numRows][numCols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {9, 8, 7, 6}; //Rows 0 to 3 //if you modify your pins you should modify this too
byte colPins[numCols] = {5, 4, 3, 2}; //Columns 0 to 3

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);


char keypressed;                 //Where the keys are stored it changes very often
char code[] = {'5', '8', '1', '2'}; //The default code, you can change it or make it a 'n' digits one
short a = 0, i = 0, j = 0;  //Variables used later


// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // open connection to serial monitor
  Serial.begin(9600);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  myservo.attach(10);  // attaches the servo on pin 10 to the servo object
  closeLock();
}

void loop() {
  keypressed = myKeypad.getKey();               //Constantly waiting for a key to be pressed

  // shows pressed keys to debug
  if (keypressed != NO_KEY) {
    Serial.println(keypressed);
  }


  // check on keys to open/close lock
  if (keypressed == '*') {                    // * to open the lock
    getCode();                          //Getting code function
    if (a == sizeof(code)) {       //The GetCode function assign a value to a (it's correct when it has the size of the code array)
      openLock();                   //Open lock function if code is correct
    }
    delay(2000);
  }

  if (keypressed == '#') {                // # to close the lock
    closeLock();
  }

  /*  // read the state of the pushbutton value:
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
    }*/
}

void getCode() {                 //Getting code sequence
  i = 0;                    //All variables set to 0
  a = 0;
  j = 0;

  while (keypressed != 'A') {                                   //The user press A to confirm the code otherwise he can keep typing
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {     //If the char typed isn't A and neither "nothing"
      Serial.println(keypressed);                         //ATTENTION security risk, password will be shown in Serial Monitor!
      j++;
      if (keypressed == code[i] && i < sizeof(code)) {       //if the char typed is correct a and i increments to verify the next caracter
        a++;                                              //Now I think maybe I should have use only a or i ... too lazy to test it -_-'
        i++;
      }
      else
        a--;                                               //if the character typed is wrong a decrements and cannot equal the size of code []
    }
  }
  keypressed = NO_KEY;

}

void openLock() {            // Lock opening function
  digitalWrite(ledPin, LOW);
  myservo.write(90);
  Serial.println("opening the lock");
}

void closeLock() {            // Lock closing function
  digitalWrite(ledPin, HIGH);
  myservo.write(0);
  Serial.println("closing the lock");
}
