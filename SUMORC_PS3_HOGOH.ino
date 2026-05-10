#include <esp_mac.h>
#include <Ps3Controller.h>

// ================= EDIT HERE =================
const char* ps3MacAddress = "01:02:03:04:05:06";
// =============================================

#define rightMotorA 18
#define rightMotorB 17

#define leftMotorA 23
#define leftMotorB 19

// Default speed level = 3
int speedLevel = 3;

// 4 speed level
const int speedPWM[4] = {51, 119, 187, 255};

int deadzone = 15;

bool lastStart = false;
bool lastSelect = false;
bool ledUpdated = false;

int getCurrentSpeed() {
  return speedPWM[speedLevel - 1];
}

void stopMotor() {
  analogWrite(rightMotorA, 0);
  analogWrite(rightMotorB, 0);
  analogWrite(leftMotorA, 0);
  analogWrite(leftMotorB, 0);
}

void updatePs3Led() {
  Ps3.setPlayer(speedLevel);   // LED 1, 2, 3, 4 ikut speed level
}

void checkSpeedButton() {
  bool currentStart = Ps3.data.button.start;
  bool currentSelect = Ps3.data.button.select;

  // START = speed naik
  if (currentStart && !lastStart) {
    speedLevel++;

    if (speedLevel > 4) {
      speedLevel = 4;
    }

    updatePs3Led();

    Serial.print("Speed Level: ");
    Serial.print(speedLevel);
    Serial.print(" | PWM = ");
    Serial.println(getCurrentSpeed());
  }

  // SELECT = speed turun
  if (currentSelect && !lastSelect) {
    speedLevel--;

    if (speedLevel < 1) {
      speedLevel = 1;
    }

    updatePs3Led();

    Serial.print("Speed Level: ");
    Serial.print(speedLevel);
    Serial.print(" | PWM = ");
    Serial.println(getCurrentSpeed());
  }

  lastStart = currentStart;
  lastSelect = currentSelect;
}

void driveMotor(int leftPWM, int rightPWM) {
  int currentSpeed = getCurrentSpeed();

  leftPWM = constrain(leftPWM, -currentSpeed, currentSpeed);
  rightPWM = constrain(rightPWM, -currentSpeed, currentSpeed);

  // LEFT MOTOR
  if (leftPWM > 0) {
    analogWrite(leftMotorA, leftPWM);
    analogWrite(leftMotorB, 0);
  } 
  else if (leftPWM < 0) {
    analogWrite(leftMotorA, 0);
    analogWrite(leftMotorB, abs(leftPWM));
  } 
  else {
    analogWrite(leftMotorA, 0);
    analogWrite(leftMotorB, 0);
  }

  // RIGHT MOTOR
  if (rightPWM > 0) {
    analogWrite(rightMotorA, rightPWM);
    analogWrite(rightMotorB, 0);
  } 
  else if (rightPWM < 0) {
    analogWrite(rightMotorA, 0);
    analogWrite(rightMotorB, abs(rightPWM));
  } 
  else {
    analogWrite(rightMotorA, 0);
    analogWrite(rightMotorB, 0);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(rightMotorA, OUTPUT);
  pinMode(rightMotorB, OUTPUT);
  pinMode(leftMotorA, OUTPUT);
  pinMode(leftMotorB, OUTPUT);

  stopMotor();

  Ps3.begin(ps3MacAddress);

  Serial.println("PS3 Robot Ready");
  Serial.print("PS3 MAC Address: ");
  Serial.println(ps3MacAddress);
  Serial.println("Default Speed Level = 3");
  Serial.print("Default PWM = ");
  Serial.println(getCurrentSpeed());
}

void loop() {
  if (!Ps3.isConnected()) {
    stopMotor();
    ledUpdated = false;

    Serial.println("PS3 NOT CONNECTED");
    delay(500);
    return;
  }

  if (!ledUpdated) {
    updatePs3Led();
    ledUpdated = true;
  }

  checkSpeedButton();

  int currentSpeed = getCurrentSpeed();

  // Left stick Y = forward/backward
  int y = Ps3.data.analog.stick.ly;

  // Right stick X = turn left/right
  int x = Ps3.data.analog.stick.rx;

  // Invert Y supaya push atas = forward
  y = -y;

  // Deadzone
  if (abs(y) < deadzone) y = 0;
  if (abs(x) < deadzone) x = 0;

  int forwardPWM = map(y, -128, 127, -currentSpeed, currentSpeed);
  int turnPWM    = map(x, -128, 127, -currentSpeed, currentSpeed);

  int leftPWM  = forwardPWM + turnPWM;
  int rightPWM = forwardPWM - turnPWM;

  leftPWM = constrain(leftPWM, -currentSpeed, currentSpeed);
  rightPWM = constrain(rightPWM, -currentSpeed, currentSpeed);

  driveMotor(leftPWM, rightPWM);

  Serial.print("Level: ");
  Serial.print(speedLevel);

  Serial.print(" | PWM: ");
  Serial.print(currentSpeed);

  Serial.print(" | Y: ");
  Serial.print(y);

  Serial.print(" | X: ");
  Serial.print(x);

  Serial.print(" | LEFT: ");
  Serial.print(leftPWM);

  Serial.print(" | RIGHT: ");
  Serial.println(rightPWM);

  delay(20);
}