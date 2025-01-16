const int chA = 13;  //on ESP channel 1 dari controller anda
// cara sambungan, V dari ESP (5V)- Barismerah ke V dari Reciever
// cara sambungan, G dari ESP ke G dari Reciever
// cara sambungan, Pin 13 dari ESP sambung ke S receiver (yang mula-mula sekali)
const int chB = 14;  ///on ESP


#define M1A_PIN 17
#define M1B_PIN 18
#define M2A_PIN 19
#define M2B_PIN 23
#define M3A_PIN 16
#define M3B_PIN 4
#define M4A_PIN 22
#define M4B_PIN 21

int ch1;  //Variables to store and display the values of each channel
int ch2;  //Variables to store and display the values of each channel

int ch1Min = 1000;  // read from serial monitor
int ch1Max = 2000;  // read from serial monitor
int ch2Min = 1000;  // read from serial monitorQ
int ch2Max = 2000;  // read from serial monitor

int offset = 40;  // Offset value for STOP function, get value from Serial Monitor
int val;

void setup() {

  //
  Serial.begin(9600);
  pinMode(chA, INPUT);
  pinMode(chB, INPUT);
  pinMode(M1A_PIN, OUTPUT);
  pinMode(M1B_PIN, OUTPUT);
  pinMode(M2A_PIN, OUTPUT);
  pinMode(M2B_PIN, OUTPUT);
  pinMode(M3A_PIN, OUTPUT);
  pinMode(M3B_PIN, OUTPUT);
  pinMode(M4A_PIN, OUTPUT);
  pinMode(M4B_PIN, OUTPUT);
  analogWrite(M1A_PIN, 0);
  analogWrite(M1B_PIN, 0);
  analogWrite(M2A_PIN, 0);
  analogWrite(M2B_PIN, 0);
  analogWrite(M3A_PIN, 0);
  analogWrite(M3B_PIN, 0);
  analogWrite(M4A_PIN, 0);
  analogWrite(M4B_PIN, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read the input channels
  ch1 = pulseIn(chA, HIGH);  //Read and store channel 1
                             // ch1=2000;
                             // ch1 = constrain(ch1, 1000, 2000);
  Serial.print("Ch1:");      //Display text string on Serial Monitor to distinguish variables
  Serial.print(ch1);         //Print in the value of channel 1
  Serial.print("|");

  ch2 = pulseIn(chB, HIGH);
  // ch2 = constrain(ch2, 1000, 2000);
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

  if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 > offset) {
    Serial.print("FORWARD");
    Serial.print("|");
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, abs(SpeedCh2));
    analogWrite(M3A_PIN, 0);
    analogWrite(M3B_PIN, abs(SpeedCh2));
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(M4A_PIN, 0);
    analogWrite(M4B_PIN, abs(SpeedCh2) - abs(SpeedCh1));

  }

  else if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 <= offset) {
    Serial.print("FORWARD");
    Serial.print("|");
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(M3A_PIN, 0);
    analogWrite(M3B_PIN, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh2));
    analogWrite(M4A_PIN, 0);
    analogWrite(M4B_PIN, abs(SpeedCh2));
  }


  else if (SpeedCh2 < -offset && SpeedCh2 >= -255) {
    Serial.print("BACKWARD");
    Serial.print("|");
    analogWrite(M1A_PIN, abs(SpeedCh2));
    analogWrite(M1B_PIN, 0);
    analogWrite(M3A_PIN, abs(SpeedCh2));
    analogWrite(M3B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh2));
    analogWrite(M2B_PIN, 0);
    analogWrite(M4A_PIN, abs(SpeedCh2));
    analogWrite(M4B_PIN, 0);
  }

  else if (SpeedCh1 > offset && SpeedCh1 <= 255) {
    Serial.print("Turn RIGHT");
    Serial.print("|");
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, abs(SpeedCh1));
    analogWrite(M3A_PIN, 0);
    analogWrite(M3B_PIN, abs(SpeedCh1));
    analogWrite(M2A_PIN, abs(SpeedCh1));
    analogWrite(M2B_PIN, 0);
    analogWrite(M4A_PIN, abs(SpeedCh1));
    analogWrite(M4B_PIN, 0);
  }

  else if (SpeedCh1 < -offset && SpeedCh1 >= -255) {
    Serial.print("Turn LEFT");
    Serial.print("|");
    analogWrite(M1A_PIN, abs(SpeedCh1));
    analogWrite(M1B_PIN, 0);
    analogWrite(M3A_PIN, abs(SpeedCh1));
    analogWrite(M3B_PIN, 0);

    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh1));
    analogWrite(M4A_PIN, 0);
    analogWrite(M4B_PIN, abs(SpeedCh1));
  }


  else if (SpeedCh1 <= offset && SpeedCh1 >= -offset) {
    Serial.print("STOP");
    Serial.print("|");
    // Move in reverse

    analogWrite(M1B_PIN, 0);
    analogWrite(M1A_PIN, 0);
    analogWrite(M2B_PIN, 0);
    analogWrite(M2A_PIN, 0);
    analogWrite(M4B_PIN, 0);
    analogWrite(M4A_PIN, 0);
    analogWrite(M3B_PIN, 0);
    analogWrite(M3A_PIN, 0);
  }

  delay(1);
}
