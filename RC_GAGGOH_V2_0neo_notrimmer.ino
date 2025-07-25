
#include <Adafruit_NeoPixel.h>
#define PINSA 16
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PINSA, NEO_GRB + NEO_KHZ800);
const int chA = 14;  // Channel 1 dari receiver
const int chB = 15;  // Channel 2 dari receiver
#define M1B_PIN 5
#define M1A_PIN 6
#define M2B_PIN 28
#define M2A_PIN 29
const int TRIMMER_PIN = 26;  // Pin analog trimmer (GPIO 26)

int ch1, ch2;
int ch1prev = 0, ch2prev = 0;
int ch1Min = 1000, ch1Max = 2000;
int ch2Min = 1000, ch2Max = 1950;
int offset = 40;  // Dead zone threshold

const int compensateRight = 0;
const int compensateLeft = 0;

void setup() {
  Serial.begin(115200);

  pinMode(chA, INPUT);
  pinMode(chB, INPUT);
  pinMode(16, OUTPUT);
  pinMode(M1A_PIN, OUTPUT);
  pinMode(M1B_PIN, OUTPUT);
  pinMode(M2A_PIN, OUTPUT);
  pinMode(M2B_PIN, OUTPUT);
  analogWrite(M1A_PIN, 0);
  analogWrite(M1B_PIN, 0);
  analogWrite(M2A_PIN, 0);
  analogWrite(M2B_PIN, 0);
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  delay(300);
  pixels.setPixelColor(0, pixels.Color(20, 0, 0));
  pixels.show();
  delay(300);
  pixels.setPixelColor(0, pixels.Color(0, 20, 0));
  pixels.show();
  delay(300);
  pixels.setPixelColor(0, pixels.Color(0, 0, 20));
  pixels.show();
  delay(300);
}

void loop() {
  // === Trimmer Input ===
  // int trimValue = analogRead(TRIMMER_PIN);
  // if (trimValue < 512) {
  //   compensateLeft = map(512 - trimValue, 0, 512, 0, 60);
  //   compensateRight = 0;
  // } else {
  //   compensateRight = map(trimValue - 512, 0, 511, 0, 60);
  //   compensateLeft = 0;
  // }

  // === Channel Readings ===
  ch1 = pulseIn(chA, HIGH, 30000);
  delay(1);
  ch2 = pulseIn(chB, HIGH, 30000);
  delay(1);
  Serial.print("Ch1:");
  Serial.print(ch1);
  if (ch1 < 700) {
    ch1 = ch1prev;
  } else if (ch1 > 900 && ch1 < ch1Min) {
    ch1 = ch1Min;
  } else if (ch1 > ch1Max) {
    ch1 = ch1Max;
  }
  ch1prev = ch1;

  if (ch2 < 700) {
    ch2 = ch2prev;
  } else if (ch2 > 900 && ch2 < ch2Min) {
    ch2 = ch2Min;
  } else if (ch2 > ch2Max) {
    ch2 = ch2Max;
  }
  ch2prev = ch2;

  // === Mapping ===
  int SpeedCh1 = map(ch1, ch1Min, ch1Max, -255, 255);

  int SpeedCh2 = map(ch2, ch2Min, ch2Max, -255, 255);
  if (SpeedCh2 < -300) {
    // Red
    pixels.setPixelColor(0, pixels.Color(30, 0, 0));
    pixels.show();
  } else if (abs(SpeedCh2) < offset) {
    // Green
    pixels.setPixelColor(0, pixels.Color(0, 30, 0));
    pixels.show();
  } else if (abs(SpeedCh2) >= offset && abs(SpeedCh2) <= 255) {
    // Blue
    pixels.setPixelColor(0, pixels.Color(0, 0, abs(SpeedCh2)));
    pixels.show();
  }


  // === Serial Output ===
  Serial.print(" Ch1:");
  Serial.print(ch1);
  Serial.print(" | Ch2:");
  Serial.print(ch2);
  Serial.print(" | Spd1:");
  Serial.print(SpeedCh1);
  Serial.print(" | Spd2:");
  Serial.print(SpeedCh2);
  Serial.print(" | CompL:");
  Serial.print(compensateLeft);
  Serial.print(" | CompR:");
  Serial.println(compensateRight);

  // === Movement Logic ===
  if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 > 0) {
    Serial.println(" | FORWARD RIGHT");
    analogWrite(M1B_PIN, 0);
    analogWrite(M1A_PIN, abs(SpeedCh2) - compensateRight);
    analogWrite(M2B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh2) - abs(SpeedCh1) - compensateLeft);
  } else if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 <= 0) {
    Serial.println(" | FORWARD LEFT");
    analogWrite(M1B_PIN, 0);
    analogWrite(M1A_PIN, abs(SpeedCh2) - abs(SpeedCh1) - compensateRight);
    analogWrite(M2B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh2) - compensateLeft);
  } else if (SpeedCh2 < -offset && SpeedCh2 >= -255 && SpeedCh1 > 0) {
    Serial.println(" | BACKWARD RIGHT");
    analogWrite(M1B_PIN, abs(SpeedCh2) - abs(SpeedCh1) - compensateLeft);
    analogWrite(M1A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh2) - compensateRight);
    analogWrite(M2A_PIN, 0);
  } else if (SpeedCh2 < -offset && SpeedCh2 >= -255 && SpeedCh1 <= 0) {
    Serial.println(" | BACKWARD LEFT");
    analogWrite(M1B_PIN, abs(SpeedCh2) - compensateLeft);
    analogWrite(M1A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh2) - abs(SpeedCh1) - compensateRight);
    analogWrite(M2A_PIN, 0);
  } else if (SpeedCh1 > offset && SpeedCh1 <= 255) {
    Serial.println(" | TURN RIGHT");
    analogWrite(M1B_PIN, 0);
    analogWrite(M1A_PIN, abs(SpeedCh1 / 1));
    analogWrite(M2B_PIN, abs(SpeedCh1 / 1));
    analogWrite(M2A_PIN, 0);

  } else if (SpeedCh1 < -offset && SpeedCh1 >= -255) {
    Serial.println(" | TURN LEFT");
    analogWrite(M1B_PIN, abs(SpeedCh1 / 1));
    analogWrite(M1A_PIN, 0);
    analogWrite(M2B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh1 / 1));
  } else if (SpeedCh1 <= offset && SpeedCh1 >= -offset && SpeedCh2 <= offset && SpeedCh2 >= -offset) {
    Serial.println(" | STOP");
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, 0);
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, 0);
  }

  delay(15);
}
