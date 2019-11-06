#include <Arduino.h>

int joyPin1 = 0;
int joyPin2 = 1;
int joyX = 0;
int joyY = 0;

void setup() {
    Serial.begin(9600);
}

int toUsable(int data) {
    return (data * 9 / 1024);
}

void loop() {
    joyX = analogRead(joyPin1);
    delay(100); // don't read same analog value twice
    joyY = analogRead(joyPin2);

    Serial.print("X: ");
    Serial.print(toUsable(joyX));
    Serial.print("; Y: ");
    Serial.println(toUsable(joyY));
}