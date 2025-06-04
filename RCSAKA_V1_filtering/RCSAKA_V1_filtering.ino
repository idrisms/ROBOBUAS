const int chA = 10; // Channel 1 RCcontroller
const int chB = 11; // Channel 2 RC Controller
#define M1A_PIN 9
#define M1B_PIN 6
#define M2A_PIN 5
#define M2B_PIN 3


int ch1, ch2;
int ch1prev = 1500;
int ch2prev = 1500;

int ch1Min = 1000;
int ch1Max = 2000;
int ch2Min = 1000;
int ch2Max = 1950;
int offset = 40;

void setup() {
  Serial.begin(115200);
  pinMode(chA, INPUT);
  pinMode(chB, INPUT);
  pinMode(M1A_PIN, OUTPUT);
  pinMode(M1B_PIN, OUTPUT);
  pinMode(M2A_PIN, OUTPUT);
  pinMode(M2B_PIN, OUTPUT);

  analogWrite(M1A_PIN, 0);
  analogWrite(M1B_PIN, 0);
  analogWrite(M2A_PIN, 0);
  analogWrite(M2B_PIN, 0);
  delay(1000);
}

void loop() {
  int raw_ch1 = pulseIn(chA, HIGH, 30000);
  int raw_ch2 = pulseIn(chB, HIGH, 30000);

  // Channel 1
  if (raw_ch1 >= 700) {
    ch1 = raw_ch1;
    ch1prev = ch1;
  } else {
    ch1 = ch1prev;
  }

  // Channel 2
  static int ch2DropCount = 0;
  const int suspiciousThreshold = 1700;
  const int disconnectThreshold = 900;

  if (raw_ch2 < disconnectThreshold) {
    // Signal lost
    Serial.println("❌ Ch2 signal lost — holding previous value");
    ch2 = ch2prev;
    ch2DropCount = 0;
  } else if (raw_ch2 <= suspiciousThreshold) {
    // Suspect value (e.g., glitch)
    ch2DropCount++;
    if (ch2DropCount >= 6) {
      ch2 = raw_ch2;
    } else {
      ch2 = ch2prev;
      Serial.print("⚠️ Ch2 suspicious drop ignored (count ");
      Serial.print(ch2DropCount);
      Serial.println(")");
    }
  } else {
    // Normal value
    ch2 = raw_ch2;
    ch2DropCount = 0;
  }

  ch2prev = ch2;

  // Clamp and map
  ch1 = constrain(ch1, ch1Min, ch1Max);
  ch2 = constrain(ch2, ch2Min, ch2Max);

  Serial.print("Ch1:"); Serial.print(ch1); Serial.print("|");
  Serial.print("Ch2:"); Serial.print(ch2); Serial.print("|");

  int SpeedCh1 = map(ch1, ch1Min, ch1Max, -255, 255);
  int SpeedCh2 = map(ch2, ch2Min, ch2Max, -255, 255);

  Serial.print(SpeedCh1); Serial.print("|");
  Serial.print(SpeedCh2); Serial.println("|");

  // Motor logic
  if (raw_ch2 < disconnectThreshold) {
    // Optional: stop motors when signal is lost
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, 0);
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, 0);
    Serial.println("🛑 Motors stopped due to signal loss");
  }

  else if (SpeedCh2 > offset && SpeedCh1 > 0) {
    Serial.print("FORWARD|");
    analogWrite(M1B_PIN, 0);
    analogWrite(M1A_PIN, abs(SpeedCh2));
    analogWrite(M2B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh2) - abs(SpeedCh1));
  }

  else if (SpeedCh2 > offset && SpeedCh1 <= 0) {
    Serial.print("FORWARD|");
    analogWrite(M1B_PIN, 0);
    analogWrite(M1A_PIN, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(M2B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh2));
  }

  else if (SpeedCh2 < -offset) {
    Serial.print("BACKWARD|");
    analogWrite(M1B_PIN, abs(SpeedCh2));
    analogWrite(M1A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh2));
    analogWrite(M2A_PIN, 0);
  }

  else if (SpeedCh1 > offset) {
    Serial.print("Turn RIGHT|");
    analogWrite(M1B_PIN, 0);
    analogWrite(M1A_PIN, abs(SpeedCh1));
    analogWrite(M2B_PIN, abs(SpeedCh1));
    analogWrite(M2A_PIN, 0);
  }

  else if (SpeedCh1 < -offset) {
    Serial.print("Turn LEFT|");
    analogWrite(M1B_PIN, abs(SpeedCh1));
    analogWrite(M1A_PIN, 0);
    analogWrite(M2B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh1));
  }

  else {
    Serial.print("STOP|");
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, 0);
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, 0);
  }

  delay(15);
}
