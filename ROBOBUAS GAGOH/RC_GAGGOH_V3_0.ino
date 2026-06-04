/*
  =====================================================
  ROBOBUAS RC GAGOH V3.0
  -----------------------------------------------------
  Function:
  - RC control using CH1 and CH2
  - NeoPixel status LED
  - Forward and backward compensation using trimmer
  - Battery voltage check ONLY when robot is STOP
  - Low battery lock if Vbat <= Vmin
  - After lock, voltage can be checked again ONLY when STOP
  =====================================================
*/

#include <Adafruit_NeoPixel.h>

// ================= NEOPIXEL SETUP =================
#define PINSA 16
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PINSA, NEO_GRB + NEO_KHZ800);

// ================= RC RECEIVER PIN =================
const int chA = 14;   // CH1 steering
const int chB = 15;   // CH2 throttle

// ================= MOTOR DRIVER PIN =================
#define M1B_PIN 4
#define M1A_PIN 7
#define EN_PIN 8
#define M2A_PIN 5
#define M2B_PIN 6

// ================= ANALOG INPUT PIN =================
#define VBAT_PIN 29
#define TRIMMER_PIN_FW 28
#define TRIMMER_PIN_BW 27

// ================= RC VARIABLE =================
int ch1, ch2;
int ch1prev = 1500;
int ch2prev = 1500;

int ch1Min = 1000;
int ch1Max = 2000;
int ch2Min = 1050;
int ch2Max = 1950;

int offset = 40;

// ================= BATTERY SETTING =================
float Vbat = 0;
const float pekali = 0.02992;

// Vmin = bilangan cell x 3.3V
// 2S LiPo = 6.6V
// 3S LiPo = 9.9V
// 4S LiPo = 13.2V
const float Vmin = 9.9;

// Unlock semula bila voltage naik lebih daripada Vmin + margin
const float recoveryMargin = 0.3;
const float maxBias =100;

bool lowBatteryLock = false;

// ================= COMPENSATION VARIABLE =================
int compensateRightFW = 0;
int compensateLeftFW = 0;
int compensateRightBW = 0;
int compensateLeftBW = 0;

// ================= LOW BATTERY BLINK =================
bool lowBatteryBlink = false;
unsigned long lastLowBatteryBlink = 0;
const unsigned long lowBatteryBlinkInterval = 200;

// ================= STOP MOTOR =================
void stopMotor() {
  analogWrite(M1A_PIN, 0);
  analogWrite(M1B_PIN, 0);
  analogWrite(M2A_PIN, 0);
  analogWrite(M2B_PIN, 0);
}

// ================= DRIVE MOTOR =================
// Semua arahan motor akan lalu function ini.
// Jika lowBatteryLock aktif, motor terus dipaksa STOP.
void driveMotor(int leftPWM, int rightPWM) {
  if (lowBatteryLock) {
    stopMotor();
    return;
  }

  leftPWM = constrain(leftPWM, -255, 255);
  rightPWM = constrain(rightPWM, -255, 255);

  // Left motor direction
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

  // Right motor direction
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

// ================= LOW BATTERY LED =================
void lowBatteryStop() {
  stopMotor();

  unsigned long now = millis();

  if (now - lastLowBatteryBlink >= lowBatteryBlinkInterval) {
    lowBatteryBlink = !lowBatteryBlink;
    lastLowBatteryBlink = now;
  }

  if (lowBatteryBlink) {
    pixels.setPixelColor(0, pixels.Color(80, 20, 0));  // orange blink
  } else {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  }

  pixels.show();
}

// ================= READ FORWARD TRIMMER =================
void readTrimmerFW() {
  int trimValueFW = analogRead(TRIMMER_PIN_FW);

  if (trimValueFW < 512) {
    compensateLeftFW = map(512 - trimValueFW, 0, 512, 0, maxBias);
    compensateRightFW = 0;
  } else {
    compensateRightFW = map(trimValueFW - 512, 0, 511, 0, maxBias);
    compensateLeftFW = 0;
  }
}

// ================= READ BACKWARD TRIMMER =================
void readTrimmerBW() {
  int trimValueBW = analogRead(TRIMMER_PIN_BW);

  if (trimValueBW < 512) {
    compensateLeftBW = map(512 - trimValueBW, 0, 512, 0, maxBias);
    compensateRightBW = 0;
  } else {
    compensateRightBW = map(trimValueBW - 512, 0, 511, 0, maxBias);
    compensateLeftBW = 0;
  }
}

// Negative value means compensation to left side.
// Positive value means compensation to right side.
int getCompFWDisplay() {
  if (compensateLeftFW > 0) return -compensateLeftFW;
  return compensateRightFW;
}

int getCompBWDisplay() {
  if (compensateLeftBW > 0) return -compensateLeftBW;
  return compensateRightBW;
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

  // Startup LED test
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

  delay(500);
}

void loop() {
  // Read compensation trimmer
  readTrimmerFW();
  readTrimmerBW();

  // Read RC signal
  ch1 = pulseIn(chA, HIGH, 30000);
  delay(1);

  ch2 = pulseIn(chB, HIGH, 30000);
  delay(1);

  // If signal lost, use previous value
  if (ch1 < 700) ch1 = ch1prev;
  else ch1 = constrain(ch1, ch1Min, ch1Max);

  if (ch2 < 700) ch2 = ch2prev;
  else ch2 = constrain(ch2, ch2Min, ch2Max);

  ch1prev = ch1;
  ch2prev = ch2;

  // Convert RC signal to PWM value
  int SpeedCh1 = map(ch1, ch1Min, ch1Max, -255, 255);
  int SpeedCh2 = map(ch2, ch2Min, ch2Max, -255, 255);

  int forward = SpeedCh2;
  int turn = SpeedCh1;

  bool belowOffset = abs(forward) < offset && abs(turn) < offset;

  // Serial monitor display
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
  Serial.print(" | CompFW:");
  Serial.print(getCompFWDisplay());
  Serial.print(" | CompBW:");
  Serial.print(getCompBWDisplay());

  if (belowOffset) {
    Serial.print(" | Motion:STOP");
  } else if (lowBatteryLock) {
    Serial.print(" | Motion:LOW BAT");
  } else if (abs(forward) >= abs(turn)) {
    if (forward > 0) Serial.print(" | Motion:FW");
    else Serial.print(" | Motion:BW");
  } else {
    if (turn > 0) Serial.print(" | Motion:RIGHT");
    else Serial.print(" | Motion:LEFT");
  }

  // Voltage check hanya waktu STOP
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

  // Kalau battery lock, motor tidak dibenarkan bergerak
  if (lowBatteryLock) {
    lowBatteryStop();
    Serial.println(" | LOW BATTERY MOTOR BLOCKED");
    delay(15);
    return;
  }

  // Normal movement LED
  if (forward > 0) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 30));
  } else {
    pixels.setPixelColor(0, pixels.Color(30, 0, 0));
  }

  pixels.show();

  // Mixing control
  int leftPWM = forward + turn;
  int rightPWM = forward - turn;

  leftPWM = constrain(leftPWM, -255, 255);
  rightPWM = constrain(rightPWM, -255, 255);

  // Forward compensation
  if (leftPWM > 0) {
    leftPWM -= compensateLeftFW * abs(leftPWM) / 255;
  }

  if (rightPWM > 0) {
    rightPWM -= compensateRightFW * abs(rightPWM) / 255;
  }

  // Backward compensation
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