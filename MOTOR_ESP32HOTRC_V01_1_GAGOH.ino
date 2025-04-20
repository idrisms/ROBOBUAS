const int chA = 20;  //on ESP channel 1 dari controller anda
// cara sambungan, V dari ESP (5V)- Barismerah ke V dari Reciever
// cara sambungan, G dari ESP ke G dari Reciever
// cara sambungan, Pin 20 dari ESP sambung ke S receiver (yang mula-mula sekali)
const int chB = 21;  ///on ESP channel 2


#define RPWM1_PIN 4  //ESP32 C3//ROBOBUAS GAGOH V1.1
#define LPWM1_PIN 3
#define RPWM2_PIN 7
#define LPWM2_PIN 10

int ch1;  //Variables to store and display the values of each channel
int ch2;  //Variables to store and display the values of each channel
int ch1prev = 0;
int ch2prev = 0;
long last_heartbeat;
int ch1Min = 1000;  // read from serial monitor
int ch1Max = 2000;  // read from serial monitor
int ch2Min = 1000;  // read from serial monitorQ
int ch2Max = 2000;  // read from serial monitor

int offset = 40;  // Offset value for STOP function, get value from Serial Monitor
int val;

void setup() {

  //
  Serial.begin(115200);
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
  delay(1000);
  // Create config structure
}

void loop() {
  // put your main code here, to run repeatedly:
  // read the input channels
  ch1 = pulseIn(chA, HIGH);  //Read and store channel 1
  if (ch1 < 500) {
    ch1 = ch1prev;
  } else if ((ch1 > 900) && (ch1 < 1000)) {
    ch1 = 1000;
  } else if (ch1 > 2000) {
    ch1 = 2000;
  }
  ch1prev = ch1;
  Serial.print("Ch1:");  //Display text string on Serial Monitor to distinguish variables
  Serial.print(ch1);     //Print in the value of channel 1
  Serial.print("|");


  ch2 = pulseIn(chB, HIGH);
  if (ch2 < 500) {
    ch2 = ch2prev;
  } else if ((ch2 > 900) && (ch2 < 1000)) {
    ch2 = 1000;
  } else if (ch2 > 2000) {
    ch2 = 2000;
  }
  ch2prev = ch2;
  Serial.print("Ch2:");
  Serial.print(ch2);
  Serial.print("|");


  //LEFT & RIGHT
  int SpeedCh1 = map(ch1, ch1Min, ch1Max, -255, 255);
  Serial.print(SpeedCh1);
  Serial.print("|");

  //FORWARD & BACKWARD
  int SpeedCh2 = map(ch2, ch2Min, ch2Max, -255, 255);
  Serial.print(SpeedCh2);
  Serial.println("|");

  if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 > 0) {
    Serial.print("FORWARD");
    Serial.print("|");
    analogWrite(RPWM1_PIN, 0);
    analogWrite(LPWM1_PIN, abs(SpeedCh2));
    analogWrite(RPWM2_PIN, 0);
    analogWrite(LPWM2_PIN, abs(SpeedCh2) - abs(SpeedCh1));
  }

  else if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 <= 0) {
    Serial.print("FORWARD");
    Serial.print("|");
    analogWrite(RPWM1_PIN, 0);
    analogWrite(LPWM1_PIN, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(RPWM2_PIN, 0);
    analogWrite(LPWM2_PIN, abs(SpeedCh2));
  }

  else if (SpeedCh2 < -offset && SpeedCh2 >= -255) {
    Serial.print("BACKWARD");
    Serial.print("|");
    analogWrite(RPWM1_PIN, abs(SpeedCh2));
    analogWrite(LPWM1_PIN, 0);
    analogWrite(RPWM2_PIN, abs(SpeedCh2));
    analogWrite(LPWM2_PIN, 0);
  }

  else if (SpeedCh1 > offset && SpeedCh1 <= 255) {
    Serial.print("Turn RIGHT");
    Serial.print("|");
    analogWrite(RPWM1_PIN, 0);
    analogWrite(LPWM1_PIN, abs(SpeedCh1));
    analogWrite(RPWM2_PIN, abs(SpeedCh1));
    analogWrite(LPWM2_PIN, 0);
  }

  else if (SpeedCh1 < -offset && SpeedCh1 >= -255) {
    Serial.print("Turn LEFT");
    Serial.print("|");
    analogWrite(RPWM1_PIN, abs(SpeedCh1));
    analogWrite(LPWM1_PIN, 0);
    analogWrite(RPWM2_PIN, 0);
    analogWrite(LPWM2_PIN, abs(SpeedCh1));
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

  delay(1);

}
