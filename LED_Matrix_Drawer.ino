#include "Arduino_LED_Matrix.h"
#include <vector>
#include <algorithm>
ArduinoLEDMatrix matrix;

// At rest
// X position: 520
// Y position:508
// Button status2

// up
// X position: 528
// Y position:1023
// Button status2

// down
// X position: 522
// Y position:0
// Button status2

// right
// X position: 0
// Y position:502
// Button status2

// left
// X position: 1023
// Y position:514
// Button status2


const int pinX = A1;
const int pinY = A2;
const int Button = 2;
const int CenterJoyPos = 520;
const int Threshold = 100;
const int GreenLED = 4;
int dotX = 5;
int dotY = 3;
// struct is a custom data type (a dictionary like in python)

// Has an x pos and a y pos refering the to led matrix
struct Point { 
  int x;
  int y;
  // custom find logic
  bool operator==(const Point& other) const {
    return (x == other.x && y == other.y);
  }
};
// vector is a dynamic list which holds a specific type of data
std::vector<Point> path;

byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void printPOS(int X_position,int Y_position,int Button_Status) {
  delay(2000);
  Serial.print("X position: ");
  Serial.print(X_position);
  Serial.println("");
  Serial.print("Y position:" );
  Serial.print(Y_position);
  Serial.println("");
  Serial.print("Button status" );
  Serial.print(Button_Status);
  Serial.println("");
  delay(2000);
}

void coolintro() {
  for (auto& row : frame) {
    delay(100);
    for (byte& x : row) {
      x = 1;
      delay(10);
      matrix.renderBitmap(frame, 8, 12);
      x = 0;
      delay(20);
      matrix.renderBitmap(frame, 8, 12);
    }
  }
}

void blink(int time, int pin) {
  delay(time);
  if (digitalRead(pin) == HIGH) {
    delay(time);
    digitalWrite(pin, LOW);
    delay(time);
    digitalWrite(pin, HIGH);
    delay(20);
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, HIGH);
    delay(time);
    digitalWrite(pin, LOW);
    delay(10);
    digitalWrite(pin, HIGH);
    delay(10);
    digitalWrite(pin, LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  matrix.begin();
  coolintro();
  delay(1000);
  frame[dotY][dotX] = 1;
  matrix.renderBitmap(frame, 8, 12);
  pinMode(Button, INPUT_PULLUP);
  pinMode(GreenLED, OUTPUT);
  // for (auto& row : frame) {
  //  for (byte x : row) {
  //  Serial.print(x);
  //    Serial.print("");
  //  }
  //  Serial.println();
  // }
}

void loop() {
  // put your main code here, to run repeatedly:
  // variables
  int X_position = analogRead(pinX);
  int Y_position = analogRead(pinY);
  int Button_Status = digitalRead(Button);
  bool Changed = false; 
  // printPOS(X_position, Y_position, Button_Status);

  
  // X movement logic
  int deltaX = CenterJoyPos - X_position;
  if (abs(deltaX) > Threshold) {
    Changed = true;
    if (deltaX > 0) {
      dotX -= 1;
    } else {
      dotX += 1;
    }
  }

  // Y movement logic
  int deltaY = CenterJoyPos - Y_position;
  if (abs(deltaY) > Threshold) {
    Changed = true;
    if (deltaY > 0) {
      dotY -= 1;
    } else {
      dotY += 1;
    }
  }
  // constraining both x and y to the allowed ranges
  dotX = constrain(dotX, 0, 11);
  dotY = constrain(dotY, 0, 7);

  // making a temp variable with the same data type as the items in the vector to find
  Point currentPos = {dotX, dotY};
  auto adress = std::find(path.begin(), path.end(), currentPos);
  bool Registered = false;
  // Green led logic
  if (adress != path.end()) { 
    Registered = true;
    // it found it if adress didnt reach path.end()
    // adress is a pointer data type thats why we can compare to path.end()
    // path.end() points past the last data in a list so like outside of the data
    digitalWrite(GreenLED, HIGH);
    } else {
        Registered = false;
        digitalWrite(GreenLED, LOW);
      }
  // button logic
  if (Button_Status == LOW) {
    Changed = true;
    blink(20, GreenLED);
    if (Registered == true) {
      path.erase(adress);
      delay(50);
    } else {
      // c++ version of append
      path.push_back(currentPos);
    }
  }

  // If something changed
  if (Changed == true){
    // clearing the board
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 12; j++) {
        frame[i][j] = 0;
      }
    }
    // cursor
    frame[dotY][dotX] = 1;

    // drawing whtas on already
    for (Point& p: path) {
      frame[p.y][p.x] = 1;
    }
    // render
    matrix.renderBitmap(frame, 8, 12);
  }
  
  // delay for visibility 
  delay(100);
}
