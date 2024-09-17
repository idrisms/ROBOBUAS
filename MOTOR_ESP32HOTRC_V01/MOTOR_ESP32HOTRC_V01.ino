const int chA = 13;  //on ESP channel 1 dari controller anda
// cara sambungan, V dari ESP (5V)- Barismerah ke V dari Reciever
// cara sambungan, G dari ESP ke G dari Reciever
// cara sambungan, Pin 13 dari ESP sambung ke S receiver (yang mula-mula sekali)
const int chB = 12;  ///on ESP
// #define RPWM1_PIN 23  //ESP32 WROOM //ROBOBUAS BUYO V0
// #define LPWM1_PIN 22
// #define RPWM2_PIN 19
// #define LPWM2_PIN 21

#define RPWM1_PIN 19  //ESP32 WROOM //ROBOBUAS BUYO V1.0
#define LPWM1_PIN 18
#define RPWM2_PIN 17
#define LPWM2_PIN 16

int ch1;  //Variables to store and display the values of each channel
int ch2;  //Variables to store and display the values of each channel

int ch1Min = 1000;  // read from serial monitor
int ch1Max = 2000;  // read from serial monitor
int ch2Min = 1000;  // read from serial monitor
int ch2Max = 2000;  // read from serial monitor

int offset = 20;  // Offset value for STOP function, get value from Serial Monitor
int val;

void setup() {

  Serial.begin(9600);
  pinMode(chA, INPUT);
  pinMode(chB, INPUT);
  pinMode(LPWM1_PIN, OUTPUT);
  pinMode(RPWM1_PIN, OUTPUT);
  pinMode(LPWM2_PIN, OUTPUT);
  pinMode(RPWM2_PIN, OUTPUT);
  analogWrite(LPWM1_PIN, 0);
  analogWrite(RPWM1_PIN, 0);
  analogWrite(LPWM2_PIN, 0);
  analogWrite(RPWM2_PIN, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read the input channels
  ch1 = pulseIn(chA, HIGH, 40000);  //Read and store channel 1
  //ch1 = constrain(ch1, 1000, 2000);
  Serial.print("Ch1:");  //Display text string on Serial Monitor to distinguish variables
  Serial.print(ch1);     //Print in the value of channel 1
  Serial.print("|");

  ch2 = pulseIn(chB, HIGH, 40000);
  //ch2 = constrain(ch2, 1000, 2000);
  Serial.print("Ch2:");
  Serial.print(ch2);
  Serial.print("|");

  //LEFT & RIGHT
  int SpeedCh1 = map(ch1, ch1Max, ch1Min, -255, 255);
  Serial.print(SpeedCh1);
  Serial.print("|");

  //FORWARD & BACKWARD
  int SpeedCh2 = map(ch2, ch2Min, ch2Max, -255, 255);
  Serial.print(SpeedCh2);
  Serial.println("|");

  if (SpeedCh2 > offset && SpeedCh2 <= 255) {
    Serial.print("FORWARD");
    Serial.print("|");
    analogWrite(LPWM1_PIN, abs(SpeedCh2));
    analogWrite(RPWM1_PIN, 0);
    analogWrite(LPWM2_PIN, abs(SpeedCh2));
    analogWrite(RPWM2_PIN, 0);
  }

  else if (SpeedCh2 < -offset && SpeedCh2 >= -255) {
    Serial.print("BACKWARD");
    Serial.print("|");
    analogWrite(LPWM1_PIN, 0);
    analogWrite(RPWM1_PIN, abs(SpeedCh2));
    analogWrite(LPWM2_PIN, 0);
    analogWrite(RPWM2_PIN, abs(SpeedCh2));
  }

  else if (SpeedCh1 > offset && SpeedCh1 <= 255) {
    Serial.print("Turn RIGHT");
    Serial.print("|");
    analogWrite(LPWM1_PIN, 0);
    analogWrite(RPWM1_PIN, abs(SpeedCh1));
    analogWrite(LPWM2_PIN, abs(SpeedCh1));
    analogWrite(RPWM2_PIN, 0);
  }

  else if (SpeedCh1 < -offset && SpeedCh1 >= -255) {
    Serial.print("Turn LEFT");
    Serial.print("|");
    analogWrite(LPWM1_PIN, abs(SpeedCh1));
    analogWrite(RPWM1_PIN, 0);
    analogWrite(LPWM2_PIN, 0);
    analogWrite(RPWM2_PIN, abs(SpeedCh1));

  }


  else if (SpeedCh1 <= offset && SpeedCh1 >= -offset) {
    Serial.print("STOP");
    Serial.print("|");
    // Move in reverse

    analogWrite(LPWM1_PIN, 0);
    analogWrite(RPWM1_PIN, 0);
    analogWrite(LPWM2_PIN, 0);
    analogWrite(RPWM2_PIN, 0);
  }

  delay(10);
}
