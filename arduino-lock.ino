
/*
  Controlling a lock with servo and a keypad
  by L0laB <https://github.com/L0laB/arduino-lock>

  modified on 1st Jan 2020
*/

//load mandatory libraries
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

//configuration for the servo - creating a servo object
Servo myservo;

//configuration for the keypad
const byte numRows = 4;                 //number of rows on the keypad
const byte numCols = 4;                 //number of columns on the keypad

char keymap[numRows][numCols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {9, 8, 7, 6};   //Rows 0 to 3 //if you modify your pins you should modify this too
byte colPins[numCols] = {5, 4, 3, 2};   //Columns 0 to 3

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//definition of variables
char keypressed;                        //variable to store the key strokes
char password[] = {'5', '8', '1', '2'}; //the default password
short a = 0, i = 0, j = 0;              //variables used later for iterating through password input sequence

//definition of constants
const int ledGreen = 13;                //connect green LED to pin number 13
const int ledRed = 12;                  //connect red LED to pin number 12

//initialise once various components
void setup() {
  Serial.begin(9600);               //open connection to serial monitor

  pinMode(LED_BUILTIN, OUTPUT);     //initialize the board LED as an output
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, HIGH);

  myservo.attach(10);               //attaches the servo on pin 10 to the servo object
  closeLock();
}

//main program - runs always in a loop
void loop() {
  keypressed = myKeypad.getKey();   //constantly wait and read for pressed key

  //shows pressed keys in Serial-Monitor to debug
  if (keypressed != NO_KEY) {
    Serial.println(keypressed);
  }

  //check pressed keys to open/close thelock
  if (keypressed == '*') {          //press '*' to enter password sequence to open the lock
    checkPassword();                //call function to read input
    if (a == sizeof(password)) {    //checkPassword function assigns a value to variable 'a' and it's correct when it has the size of the password array
      openLock();                   //open lock if password is correct
    }
  }

  if (keypressed == '#') {          //press '#' to close the lock
    delay(300);
    closeLock();
  }
}

//function to verify password characters
void checkPassword() {
  i = 0;
  a = 0;
  j = 0;

  while (keypressed != 'D') {                                   //to confirm the password press 'D' to 'open Door'
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'D' ) {           //If the char typed isn't D and neither "nothing"
      Serial.println(keypressed);                               //ATTENTION security risk, password will be shown in Serial Monitor!
      j++;
      if (keypressed == password[i] && i < sizeof(password)) {  //if the char is correct a-/ i-variable increments to verify the next caracter
        a++;                                                    //maybe only one iterator needed: a or i ?
        i++;
      }
      else
        a--;                                                    //if the character typed is wrong a decrements and cannot equal the size of password array
    }
  }
  keypressed = NO_KEY;
}

//function opening the lock and setting statuses
void closeLock() {
  myservo.write(90);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, LOW);
  Serial.println("closing the lock");
}

//function closing the lock and setting statuses
void openLock() {
  myservo.write(0);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, LOW);
  Serial.println("opening the lock");
}
