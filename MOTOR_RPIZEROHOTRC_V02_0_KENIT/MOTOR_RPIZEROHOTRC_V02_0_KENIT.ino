const int chA = 14;  //on RPI channel 1 dari controller anda
// cara sambungan, V dari RPIZERO (5V)- Barismerah ke V dari Reciever
// cara sambungan, G dari RPIZERO ke G dari Reciever
const int chB = 15;  //on RPIZERO


#define M1A_PIN 2  //RPIZERO//ROBOBUAS KENIT V2.0
#define M1B_PIN 3
#define M2A_PIN 4
#define M2B_PIN 5
#define M3A_PIN 29
#define M3B_PIN 28
#define M4A_PIN 27
#define M4B_PIN 26

int ch1;  //Variables to store and display the values of each channel
int ch2;  //Variables to store and display the values of each channel
int ch1prev = 0;
int ch2prev = 0;
int ch1Min = 1000;  // read from serial monitor
int ch1Max = 2010;  // read from serial monitor
int ch2Min = 1000;  // read from serial monitorQ
int ch2Max = 2010;  // read from serial monitor

int offset = 40;  // Offset value for STOP function, get value from Serial Monitor
int val;


void setup() {

  //
  Serial.begin(9600);
  pinMode(chA, INPUT);
  pinMode(chB, INPUT);
  pinMode(M1B_PIN, OUTPUT);
  pinMode(M1A_PIN, OUTPUT);
  pinMode(M2B_PIN, OUTPUT);
  pinMode(M2A_PIN, OUTPUT);
  pinMode(M3B_PIN, OUTPUT);
  pinMode(M3A_PIN, OUTPUT);
  pinMode(M4B_PIN, OUTPUT);
  pinMode(M4A_PIN, OUTPUT);
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
  ch1 = pulseIn(chA, HIGH,25000);  //Read and store channel 1
  if (ch1 < 800) {
    ch1 = ch1prev;
  }
  ch1prev = ch1;
  Serial.print("Ch1:");  //Display text string on Serial Monitor to distinguish variables
  Serial.print(ch1);     //Print in the value of channel 1
  Serial.print("|");


  ch2 = pulseIn(chB, HIGH, 25000);
  if (ch2 < 800) {
    ch2 = ch2prev;
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
    Serial.print("FORWARD&RIGHT");
    Serial.print("|");
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(M3A_PIN, 0);
    analogWrite(M3B_PIN, abs(SpeedCh2));
    analogWrite(M4A_PIN, 0);
    analogWrite(M4B_PIN, abs(SpeedCh2));

  }

  else if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 <= 0) {
    Serial.print("FORWARD&LEFT");
    Serial.print("|");
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, abs(SpeedCh2));
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh2));
    analogWrite(M3A_PIN, 0);
    analogWrite(M3B_PIN, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(M4A_PIN, 0);
    analogWrite(M4B_PIN, abs(SpeedCh2) - abs(SpeedCh1));
  }

  else if (SpeedCh2 < -offset && SpeedCh2 >= -255) {
    Serial.print("BACKWARD");
    Serial.print("|");
    analogWrite(M1A_PIN, abs(SpeedCh2));
    analogWrite(M1B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh2));
    analogWrite(M2B_PIN, 0);
    analogWrite(M3A_PIN, abs(SpeedCh2));
    analogWrite(M3B_PIN, 0);
    analogWrite(M4A_PIN, abs(SpeedCh2));
    analogWrite(M4B_PIN, 0);
  }

  else if (SpeedCh1 > offset && SpeedCh1 <= 255) {
    Serial.print("Turn LEFT");
    Serial.print("|");
    analogWrite(M1A_PIN, abs(SpeedCh1) / 2);
    analogWrite(M1B_PIN, 0);
    analogWrite(M2A_PIN, abs(SpeedCh1) / 2);
    analogWrite(M2B_PIN, 0);
    analogWrite(M3A_PIN, 0);
    analogWrite(M3B_PIN, abs(SpeedCh1) / 2);
    analogWrite(M4A_PIN, 0);
    analogWrite(M4B_PIN, abs(SpeedCh1) / 2);

  }

  else if (SpeedCh1 < -offset && SpeedCh1 >= -255) {
    Serial.print("Turn RIGHT");
    Serial.print("|");
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, abs(SpeedCh1) / 2);
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, abs(SpeedCh1) / 2);
    analogWrite(M3A_PIN, abs(SpeedCh1) / 2);
    analogWrite(M3B_PIN, 0);
    analogWrite(M4A_PIN, abs(SpeedCh1) / 2);
    analogWrite(M4B_PIN, 0);
  }


  else if (SpeedCh1 <= offset && SpeedCh1 >= -offset) {
    Serial.print("STOP");
    Serial.print("|");
    // Move in reverse

    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, 0);
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, 0);
    analogWrite(M3A_PIN, 0);
    analogWrite(M3B_PIN, 0);
    analogWrite(M4A_PIN, 0);
    analogWrite(M4B_PIN, 0);
  }

  delay(10);
}
