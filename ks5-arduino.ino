
/*
  Controlling a lock with servo and a keypad
  by L0laB <https://github.com/L0laB/arduino-lock>

  modified on 1st Jan 2020
*/

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo


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


char keypressed;                    //variable to store the key strokes
char code[] = {'5', '8', '1', '2'}; //the default password
short a = 0, i = 0, j = 0;          //variables used later for iterating through password input sequence

const int ledGreen = 13;            // the number of the LED pin
const int ledRed = 12;              // the number of the LED pin

void setup() {
  Serial.begin(9600);               //open connection to serial monitor

  pinMode(LED_BUILTIN, OUTPUT);          //initialize the LED pin as an output
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, HIGH);

  myservo.attach(10);               //attaches the servo on pin 10 to the servo object
  closeLock();
}

void loop() {
  keypressed = myKeypad.getKey();   //constantly wait and read for pressed key

  //shows pressed keys in Serial Monitor to debug
  if (keypressed != NO_KEY) {
    Serial.println(keypressed);
  }

  //check pressed keys to open/close thelock
  if (keypressed == '*') {          //press '*' to enter password sequence to open the lock
    getCode();                      //call function to read input
    if (a == sizeof(code)) {        //getCode function assigns a value to variable 'a' (it's correct when it has the size of the code array)
      openLock();                   //open lock if password is correct
    }
    delay(2000);
  }

  if (keypressed == '#') {          //press '#' to close the lock
    closeLock();
  }
}

void getCode() {                    //getting character sequence
  i = 0;
  a = 0;
  j = 0;

  while (keypressed != 'D') {                             //The user press D to confirm the code otherwise he can keep typing
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'D' ) {     //If the char typed isn't D and neither "nothing"
      Serial.println(keypressed);                         //ATTENTION security risk, password will be shown in Serial Monitor!
      j++;
      if (keypressed == code[i] && i < sizeof(code)) {    //if the char typed is correct a and i increments to verify the next caracter
        a++;                                              //Now I think maybe I should have use only a or i ... too lazy to test it -_-'
        i++;
      }
      else
        a--;                                               //if the character typed is wrong a decrements and cannot equal the size of code []
    }
  }
  keypressed = NO_KEY;

}

void closeLock() {
  digitalWrite(LED_BUILTIN, HIGH);
  myservo.write(90);
  Serial.println("closing the lock");
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, LOW);
}

void openLock() {
  digitalWrite(LED_BUILTIN, LOW);
  myservo.write(0);
  Serial.println("opening the lock");
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, LOW);
}
