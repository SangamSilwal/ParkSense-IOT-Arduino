#include <U8g2lib.h>

// SPI OLED: SH1106 128x64, hardware SPI
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 8, /* dc=*/ 9, /* reset=*/ 10);

// Ultrasonic sensor pins
const int trigPin1 = 2, echoPin1 = 3;
const int trigPin2 = 4, echoPin2 = 5;
const int trigPin3 = 6, echoPin3 = 7;

// LED pins
const int led1 = 12, led2 = A0, led3 = A1;

// Distance threshold (in cm)
const int threshold = 10;

void setup() {
  Serial.begin(9600);
  if (!u8g2.begin()) {
    Serial.println("OLED initialization failed");
    for (;;);
  }

  pinMode(trigPin1, OUTPUT); pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT); pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT); pinMode(echoPin3, INPUT);
  pinMode(led1, OUTPUT); pinMode(led2, OUTPUT); pinMode(led3, OUTPUT);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, "Smart Parking");
  u8g2.sendBuffer();
  delay(1000);
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  long distance1 = getDistance(trigPin1, echoPin1);
  long distance2 = getDistance(trigPin2, echoPin2);
  long distance3 = getDistance(trigPin3, echoPin3);

  digitalWrite(led1, distance1 < threshold ? HIGH : LOW);
  digitalWrite(led2, distance2 < threshold ? HIGH : LOW);
  digitalWrite(led3, distance3 < threshold ? HIGH : LOW);

  int freeSlots = 0;
  if (distance1 >= threshold) freeSlots++;
  if (distance2 >= threshold) freeSlots++;
  if (distance3 >= threshold) freeSlots++;

  // Serial debug
  Serial.print("Distance 1: "); Serial.print(distance1); Serial.print(" cm, ");
  Serial.print("Distance 2: "); Serial.print(distance2); Serial.print(" cm, ");
  Serial.print("Distance 3: "); Serial.print(distance3); Serial.println(" cm");
  Serial.print("Free Slots: "); Serial.println(freeSlots);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 10);
  u8g2.print("Free Slots: ");
  u8g2.print(freeSlots);
  u8g2.sendBuffer();

  delay(100);
}