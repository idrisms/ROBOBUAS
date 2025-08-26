// ====== ESP32 Bluetooth Motor Control - BUYO1 ======
#include <BluetoothSerial.h>
BluetoothSerial BT;

#define M1A 19
#define M1B 18
#define M2A 17
#define M2B 16
#define LED 2   // ✅ LED pada pin 2

// Nilai kelajuan asas (0-255)
int baseSpeed = 200;

void driveMotor(int pinA, int pinB, int speedVal) {
  speedVal = constrain(speedVal, -255, 255);
  if (speedVal > 0) {
    analogWrite(pinA, speedVal);
    analogWrite(pinB, 0);
  } else if (speedVal < 0) {
    analogWrite(pinA, 0);
    analogWrite(pinB, -speedVal);
  } else {
    analogWrite(pinA, 0);
    analogWrite(pinB, 0);
  }
}

void drive(int leftSpeed, int rightSpeed) {
  driveMotor(M1A, M1B, leftSpeed);
  driveMotor(M2A, M2B, rightSpeed);
}

void stopAll() { drive(0, 0); }

void setup() {
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);   // LED off mula-mula

  stopAll();
  Serial.begin(115200);

  // Jika perlu pairing PIN, buka komen baris bawah:
  // BT.setPin("1234");

  // ✅ Nama Bluetooth ditukar ke BUYO1
  BT.begin("BUYO1");
  Serial.println("BT SPP ready. Cari 'BUYO1' dan pair.");
}

String incoming;

void loop() {
  // ✅ Check status sambungan Bluetooth
  if (BT.hasClient()) {
    digitalWrite(LED, HIGH);   // LED ON bila connect
  } else {
    digitalWrite(LED, LOW);    // LED OFF bila disconnect
  }

  if (BT.available()) {
    char c = BT.read();
    if (c == '\r') return;
    if (c == '\n') {
      incoming.trim();
      incoming.toUpperCase();

      if (incoming.length() > 0) {
        Serial.print("[CMD] "); Serial.println(incoming);

        if (incoming.charAt(0) == 'V') {
          int v = incoming.substring(1).toInt();
          baseSpeed = constrain(v, 0, 255);
          Serial.print("[SPEED] baseSpeed="); Serial.println(baseSpeed);
          BT.println(String("SPEED=") + baseSpeed);
        } else {
          char cmd = incoming.charAt(0);
          switch (cmd) {
            case 'F': drive(+baseSpeed, +baseSpeed); BT.println("MOVE=FWD"); break;
            case 'B': drive(-baseSpeed, -baseSpeed); BT.println("MOVE=BACK"); break;
            case 'L': drive(-baseSpeed, +baseSpeed); BT.println("MOVE=LEFT"); break;
            case 'R': drive(+baseSpeed, -baseSpeed); BT.println("MOVE=RIGHT"); break;
            case 'S': default: stopAll(); BT.println("MOVE=STOP"); break;
          }
        }
      }
      incoming = ""; 
    } else {
      incoming += c;
    }
  }
}
