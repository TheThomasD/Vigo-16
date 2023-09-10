#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

    Serial.println("Hello World 3!");
    delay(2000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}