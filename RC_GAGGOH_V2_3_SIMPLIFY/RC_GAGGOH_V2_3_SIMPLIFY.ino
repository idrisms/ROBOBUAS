#include <Adafruit_NeoPixel.h>

#define PINSA 16
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PINSA, NEO_GRB + NEO_KHZ800);

const int chA = 14;
const int chB = 15;

#define M1B_PIN 5
#define M1A_PIN 6
#define EN_PIN 7
#define M2A_PIN 28
#define M2B_PIN 29

#define VBAT_PIN 27
#define TRIMMER_PIN 26

int ch1, ch2;
int ch1prev = 1500;
int ch2prev = 1500;

int ch1Min = 1000;
int ch1Max = 2000;
int ch2Min = 1100;
int ch2Max = 1900;

int offset = 40;

float Vbat = 0;
const float pekali = 0.02992;

// Vmin = bilangan sel x 3.3V
// 2S LiPo = 6.6V
// 3S LiPo = 9.9V
// 4S LiPo = 13.2V
const float Vmin = 9.9;

// Supaya lock tidak on/off terlalu cepat
const float recoveryMargin = 0.3;  // unlock bila Vbat > Vmin + 0.3V

bool lowBatteryLock = false;

int compensateRightFW = 0;
int compensateLeftFW = 0;

const int compensateRightBW = 0;
const int compensateLeftBW = 0;

bool lowBatteryBlink = false;
unsigned long lastLowBatteryBlink = 0;
const unsigned long lowBatteryBlinkInterval = 200;

void stopMotor() {
  analogWrite(M1A_PIN, 0);
  analogWrite(M1B_PIN, 0);
  analogWrite(M2A_PIN, 0);
  analogWrite(M2B_PIN, 0);
}

void driveMotor(int leftPWM, int rightPWM) {
  if (lowBatteryLock) {
    stopMotor();
    return;
  }

  leftPWM = constrain(leftPWM, -255, 255);
  rightPWM = constrain(rightPWM, -255, 255);

  if (leftPWM > 0) {
    analogWrite(M1A_PIN, leftPWM);
    analogWrite(M1B_PIN, 0);
  } else if (leftPWM < 0) {
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, abs(leftPWM));
  } else {
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, 0);
  }

  if (rightPWM > 0) {
    analogWrite(M2A_PIN, rightPWM);
    analogWrite(M2B_PIN, 0);
  } else if (rightPWM < 0) {
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, abs(rightPWM));
  } else {
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, 0);
  }
}

void lowBatteryStop() {
  stopMotor();

  unsigned long now = millis();

  if (now - lastLowBatteryBlink >= lowBatteryBlinkInterval) {
    lowBatteryBlink = !lowBatteryBlink;
    lastLowBatteryBlink = now;
  }

  if (lowBatteryBlink) {
    pixels.setPixelColor(0, pixels.Color(80, 20, 0));
  } else {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  }

  pixels.show();
}

void setup() {
  Serial.begin(115200);

  pinMode(chA, INPUT);
  pinMode(chB, INPUT);

  pinMode(M1A_PIN, OUTPUT);
  pinMode(M1B_PIN, OUTPUT);
  pinMode(M2A_PIN, OUTPUT);
  pinMode(M2B_PIN, OUTPUT);

  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH);

  stopMotor();

  pixels.begin();

  pixels.setPixelColor(0, pixels.Color(20, 0, 0));
  pixels.show();
  delay(300);

  pixels.setPixelColor(0, pixels.Color(0, 20, 0));
  pixels.show();
  delay(300);

  pixels.setPixelColor(0, pixels.Color(0, 0, 20));
  pixels.show();
  delay(300);

  pixels.clear();
  pixels.show();
}

void loop() {
  int trimValue = analogRead(TRIMMER_PIN);

  if (trimValue < 512) {
    compensateLeftFW = map(512 - trimValue, 0, 512, 0, 60);
    compensateRightFW = 0;
  } else {
    compensateRightFW = map(trimValue - 512, 0, 511, 0, 60);
    compensateLeftFW = 0;
  }

  ch1 = pulseIn(chA, HIGH, 30000);
  delay(1);

  ch2 = pulseIn(chB, HIGH, 30000);
  delay(1);

  if (ch1 < 700) ch1 = ch1prev;
  else ch1 = constrain(ch1, ch1Min, ch1Max);

  if (ch2 < 700) ch2 = ch2prev;
  else ch2 = constrain(ch2, ch2Min, ch2Max);

  ch1prev = ch1;
  ch2prev = ch2;

  int SpeedCh1 = map(ch1, ch1Min, ch1Max, -255, 255);
  int SpeedCh2 = map(ch2, ch2Min, ch2Max, -255, 255);

  int forward = SpeedCh2;
  int turn = SpeedCh1;

  bool belowOffset = abs(forward) < offset && abs(turn) < offset;

  Serial.print("CH1:");
  Serial.print(ch1);
  Serial.print(" | CH2:");
  Serial.print(ch2);
  Serial.print(" | SPD1:");
  Serial.print(SpeedCh1);
  Serial.print(" | SPD2:");
  Serial.print(SpeedCh2);
  Serial.print(" | VBAT:");
  Serial.print(Vbat);
  Serial.print(" | LOCK:");
  Serial.print(lowBatteryLock);
  Serial.print(" | CompLFW:");
  Serial.print(compensateLeftFW);
  Serial.print(" | CompRFW:");
  Serial.print(compensateRightFW);
  Serial.print(" | CompLBW:");
  Serial.print(compensateLeftBW);
  Serial.print(" | CompRBW:");
  Serial.print(compensateRightBW);

  // Check voltage waktu STOP sahaja
  if (belowOffset) {
    stopMotor();

    Vbat = analogRead(VBAT_PIN) * pekali;

    if (Vbat <= Vmin) {
      lowBatteryLock = true;
    }

    if (lowBatteryLock && Vbat > (Vmin + recoveryMargin)) {
      lowBatteryLock = false;
    }

    if (lowBatteryLock) {
      lowBatteryStop();
      Serial.println(" | LOW BATTERY LOCK");
    } else {
      pixels.setPixelColor(0, pixels.Color(0, 30, 0));
      pixels.show();
      Serial.println(" | STOP / BATTERY OK");
    }

    delay(15);
    return;
  }

  // Jika lock dan joystick bergerak, motor tetap block
  if (lowBatteryLock) {
    lowBatteryStop();
    Serial.println(" | LOW BATTERY MOTOR BLOCKED");
    delay(15);
    return;
  }

  if (forward > 0) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 30));
  } else {
    pixels.setPixelColor(0, pixels.Color(30, 0, 0));
  }

  pixels.show();

  int leftPWM = forward + turn;
  int rightPWM = forward - turn;

  leftPWM = constrain(leftPWM, -255, 255);
  rightPWM = constrain(rightPWM, -255, 255);

  if (leftPWM > 0) {
    leftPWM -= compensateLeftFW * abs(leftPWM) / 255;
  }

  if (rightPWM > 0) {
    rightPWM -= compensateRightFW * abs(rightPWM) / 255;
  }

  if (leftPWM < 0) {
    leftPWM += compensateLeftBW * abs(leftPWM) / 255;
  }

  if (rightPWM < 0) {
    rightPWM += compensateRightBW * abs(rightPWM) / 255;
  }

  leftPWM = constrain(leftPWM, -255, 255);
  rightPWM = constrain(rightPWM, -255, 255);

  Serial.print(" | LEFT:");
  Serial.print(leftPWM);
  Serial.print(" | RIGHT:");
  Serial.println(rightPWM);

  driveMotor(leftPWM, rightPWM);

  delay(15);
}