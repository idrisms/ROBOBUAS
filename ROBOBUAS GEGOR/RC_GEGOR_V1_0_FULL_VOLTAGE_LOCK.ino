/*
  ROBOBUAS GEGOR V1.0 ESP32
  Captive Portal AP Mode
  MAC address saved permanently using Preferences
  MAC format validation before save
  PS3 connection status displayed on OLED
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <BluetoothSerial.h>
#include <Ps3Controller.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

// =========== Battery adjustment ========
//#define VBAT_FACTOR 0.00852

// ================= PIN =================
#define WS_PIN 4
#define BTN1_PIN 34
#define BTN2_PIN 35

#define POT_FW_PIN 39
#define POT_BW_PIN 36

#define CH1_PIN 13
#define CH2_PIN 14
#define CH3_PIN 27
#define CH4_PIN 26

#define M1A_PIN 19
#define M1B_PIN 18
#define M2A_PIN 16
#define M2B_PIN 17
#define EN_PIN 23

#define VBAT_PIN 25

#define OLED_SDA 21
#define OLED_SCL 22

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_V_SPACING 8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= OBJECT =================
Adafruit_NeoPixel pixel(1, WS_PIN, NEO_GRB + NEO_KHZ800);
Preferences prefs;
WebServer server(80);
DNSServer dnsServer;
BluetoothSerial BT;

// ================= CAPTIVE PORTAL =================
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);



// ================= MODE =================
#define MODE_RC 1
#define MODE_PS3 2
#define MODE_BT 3

int currentMode = MODE_RC;

// ================= RC =================
const int rcMid = 1500;
const int rcOffset = 40;

// ================= SPEED =================
int speedLevel = 4;
int maxSpeedNow = 255;
const int speedTable[4] = { 100, 152, 203, 255 };

// ================= BIAS =================
const int maxBias = 100;

int compensateLeftFW = 0;
int compensateRightFW = 0;
int compensateLeftBW = 0;
int compensateRightBW = 0;

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

// ================= MOTOR VALUE =================
int leftSpeed = 0;
int rightSpeed = 0;
int fwBwSpeed = 0;

// ================= BATTERY LOCK =================
// Adjust VMIN here. For LiPo 3S, 9.9V = 3.3V/cell.
const float VMIN = 9.9;
bool lowBatteryLock = false;

// ================= PS3 / BT =================
String ps3Mac = "00:00:00:00:00:00";
String btName = "GEGOR";
String incoming = "";
int btBaseSpeed = 150;

bool ps3WasConnected = false;

// ================= BUTTON =================
bool btn1Old = HIGH;
bool btn2Old = HIGH;

unsigned long btn1PressTime = 0;
unsigned long btn2PressTime = 0;

bool btn1LongDone = false;
bool btn2LongDone = false;

const unsigned long longPressTime = 1200;

// ================= TIMING =================
unsigned long lastOled = 0;

// ================= DECLARATION =================
void checkButtonsOnlyNoAP();
float readBattery();
void updateBatteryLockIfNoInput(bool noInput);

// ================= MAC VALIDATION =================
bool isHexChar(char c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

bool isValidMac(String mac) {
  mac.trim();

  if (mac.length() != 17) return false;

  for (int i = 0; i < 17; i++) {
    if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14) {
      if (mac.charAt(i) != ':') return false;
    } else {
      if (!isHexChar(mac.charAt(i))) return false;
    }
  }

  return true;
}

// ================= WS2812 =================
void setColor(int r, int g, int b) {
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}

void blinkColor(int r, int g, int b, int times) {
  for (int i = 0; i < times; i++) {
    setColor(r, g, b);
    delay(250);
    setColor(0, 0, 0);
    delay(250);
  }
}

// ================= MOTOR =================
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

void drive(int left, int right) {
  left = constrain(left, -255, 255);
  right = constrain(right, -255, 255);

  // Voltage is checked only when there is no motor input.
  // If battery is below VMIN during stop, robot will be locked.
  // Lock will be released only when robot is stopped and voltage is above VMIN.
  bool noInput = (left == 0 && right == 0);
  updateBatteryLockIfNoInput(noInput);

  if (lowBatteryLock) {
    leftSpeed = 0;
    rightSpeed = 0;
    fwBwSpeed = 0;

    digitalWrite(EN_PIN, HIGH);
    driveMotor(M1A_PIN, M1B_PIN, 0);
    driveMotor(M2A_PIN, M2B_PIN, 0);
    return;
  }

  leftSpeed = left;
  rightSpeed = right;

  digitalWrite(EN_PIN, HIGH);

  driveMotor(M1A_PIN, M1B_PIN, left);
  driveMotor(M2A_PIN, M2B_PIN, right);
}

void stopAll() {
  leftSpeed = 0;
  rightSpeed = 0;
  fwBwSpeed = 0;

  driveMotor(M1A_PIN, M1B_PIN, 0);
  driveMotor(M2A_PIN, M2B_PIN, 0);
}

// ================= TRIMMER =================
void readTrimmerFW() {
  int trimValueFW = analogRead(POT_FW_PIN);

  if (trimValueFW < 2048) {
    compensateLeftFW = map(2048 - trimValueFW, 0, 2048, 0, maxBias);
    compensateRightFW = 0;
  } else {
    compensateRightFW = map(trimValueFW - 2048, 0, 2047, 0, maxBias);
    compensateLeftFW = 0;
  }
}

void readTrimmerBW() {
  int trimValueBW = analogRead(POT_BW_PIN);

  if (trimValueBW < 2048) {
    compensateLeftBW = map(2048 - trimValueBW, 0, 2048, 0, maxBias);
    compensateRightBW = 0;
  } else {
    compensateRightBW = map(trimValueBW - 2048, 0, 2047, 0, maxBias);
    compensateLeftBW = 0;
  }
}

void applyBiasAndDrive(int moveSpeed, int turnSpeed) {
  int left = moveSpeed + turnSpeed;
  int right = moveSpeed - turnSpeed;

  if (moveSpeed > 0) {
    left -= compensateLeftFW;
    right -= compensateRightFW;
  } else if (moveSpeed < 0) {
    left += compensateLeftBW;
    right += compensateRightBW;
  }

  drive(left, right);
}

// ================= BATTERY =================
float readBattery() {
  return analogRead(VBAT_PIN) * 0.007553 + 0.8597;
  ;
}

void updateBatteryLockIfNoInput(bool noInput) {
  if (!noInput) return;

  float vbat = readBattery();

  if (vbat < VMIN) {
    lowBatteryLock = true;
  } else {
    lowBatteryLock = false;
  }
}

// ================= MODE =================
void saveMode() {
  prefs.putInt("mode", currentMode);
}

void restartToMode(int mode) {
  stopAll();
  currentMode = mode;
  saveMode();
  delay(300);
  ESP.restart();
}

void setMode(int mode) {
  stopAll();

  currentMode = mode;
  saveMode();

  ps3WasConnected = false;

  if (currentMode == MODE_RC) {
    blinkColor(0, 0, 255, 1);
  }

  else if (currentMode == MODE_PS3) {
    blinkColor(0, 255, 0, 2);

    // PS3 guna Bluetooth Classic.
    // Jangan start BluetoothSerial serentak.
    Ps3.begin(ps3Mac.c_str());
    Ps3.setPlayer(speedLevel);
  }

  else if (currentMode == MODE_BT) {
    blinkColor(255, 0, 0, 3);

    // BluetoothSerial hanya hidup dalam mode BT.
    BT.begin(btName);
  }
}

// ================= RC =================
int readRC(int pin) {
  int val = pulseIn(pin, HIGH, 25000);

  if (val < 900 || val > 2100) {
    return rcMid;
  }

  return val;
}

void handleRC() {
  int ch1 = readRC(CH1_PIN);
  int ch2 = readRC(CH2_PIN);

  int throttle = 0;
  int steering = 0;

  if (ch2 > rcMid + rcOffset) {
    throttle = map(ch2, rcMid + rcOffset, 2000, 0, 255);
  } else if (ch2 < rcMid - rcOffset) {
    throttle = map(ch2, rcMid - rcOffset, 1000, 0, -255);
  }

  if (ch1 > rcMid + rcOffset) {
    steering = map(ch1, rcMid + rcOffset, 2000, 0, 255);
  } else if (ch1 < rcMid - rcOffset) {
    steering = map(ch1, rcMid - rcOffset, 1000, 0, -255);
  }

  fwBwSpeed = throttle;
  applyBiasAndDrive(throttle, steering);
}

// ================= PS3 =================
void updatePs3Led() {
  Ps3.setPlayer(speedLevel);
}

void handlePS3() {
  static unsigned long lastPs3Check = 0;
  static unsigned long lastReconnectTry = 0;

  bool ps3Connected = Ps3.isConnected();

  // Check PS3 status every 500ms
  if (millis() - lastPs3Check > 500) {
    lastPs3Check = millis();

    if (ps3Connected) {
      Serial.println("PS3: CONNECTED");
    } else {
      Serial.println("PS3: DISCONNECTED");
    }
  }

  if (!ps3Connected) {
    stopAll();

    if (ps3WasConnected) {
      ps3WasConnected = false;
      setColor(255, 0, 0);  // Red = disconnected
    }

    // Try restart PS3 Bluetooth every 5 seconds
    if (millis() - lastReconnectTry > 5000) {
      lastReconnectTry = millis();

      Serial.println("PS3: Reconnect try...");
      btStop();
      delay(200);
      Ps3.begin(ps3Mac.c_str());
    }

    return;
  }

  if (!ps3WasConnected) {
    ps3WasConnected = true;
    setColor(0, 255, 0);  // Green = connected
    Serial.println("PS3: Connected OK");
    updatePs3Led();
  }

  static bool startOld = false;
  static bool selectOld = false;

  bool startNow = Ps3.data.button.start;
  bool selectNow = Ps3.data.button.select;

  if (startNow && !startOld) {
    speedLevel++;
    if (speedLevel > 4) speedLevel = 4;

    maxSpeedNow = speedTable[speedLevel - 1];
    updatePs3Led();
  }

  if (selectNow && !selectOld) {
    speedLevel--;
    if (speedLevel < 1) speedLevel = 1;

    maxSpeedNow = speedTable[speedLevel - 1];
    updatePs3Led();
  }

  startOld = startNow;
  selectOld = selectNow;

  int ly = Ps3.data.analog.stick.ly;
  int rx = Ps3.data.analog.stick.rx;

  int throttle = map(-ly, -128, 127, -maxSpeedNow, maxSpeedNow);
  int steering = map(rx, 127, -128, -maxSpeedNow, maxSpeedNow);

  if (abs(throttle) < 25) throttle = 0;
  if (abs(steering) < 25) steering = 0;

  fwBwSpeed = throttle;
  applyBiasAndDrive(throttle, steering);
}

// ================= BLUETOOTH =================
void handleBT() {

  if (BT.available()) {
    char c = BT.read();
    char cmd;

    if (c == '\r') return;

    if (c == '\n') {
      incoming.trim();
      incoming.toUpperCase();

      if (incoming.length() > 0) {
        if (incoming.charAt(0) == 'V') {
          int v = incoming.substring(1).toInt();
          btBaseSpeed = constrain(v, 0, 255);
          BT.println(String("SPEED=") + btBaseSpeed);
        } else {

          cmd = incoming.charAt(0);

          if (cmd == 'F') {
            fwBwSpeed = btBaseSpeed;
            applyBiasAndDrive(btBaseSpeed, 0);
            BT.println("MOVE=FWD");
          } else if (cmd == 'B') {
            fwBwSpeed = -btBaseSpeed;
            applyBiasAndDrive(-btBaseSpeed, 0);
            BT.println("MOVE=BACK");
          } else if (cmd == 'L') {
            fwBwSpeed = 0;
            drive(-btBaseSpeed, btBaseSpeed);
            BT.println("MOVE=LEFT");
          } else if (cmd == 'R') {
            fwBwSpeed = 0;
            drive(btBaseSpeed, -btBaseSpeed);
            BT.println("MOVE=RIGHT");
          } else {
            fwBwSpeed = 0;
            drive(0, 0);
            BT.println("MOVE=STOP");
          }
        }
      }

      incoming = "";
    } else {
      incoming += c;
      cmd = 'e';
    }
  }
}

// ================= WEB PAGE =================
String htmlPage() {
  String page = "";

  page += "<!DOCTYPE html><html><head>";
  page += "<title>GEGOR PS3 MAC</title>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<style>";
  page += "body{font-family:Arial;text-align:center;background:#111;color:white;margin:0;padding:20px;}";
  page += ".box{border:2px solid orange;padding:18px;border-radius:12px;max-width:420px;margin:auto;}";
  page += "input{font-size:22px;width:90%;padding:10px;text-align:center;border-radius:8px;}";
  page += "button{font-size:20px;padding:12px;margin:10px;width:40%;border-radius:8px;}";
  page += ".save{background:#8a2be2;color:white;}";
  page += ".cancel{background:#555;color:white;}";
  page += ".note{font-size:13px;color:#aaa;}";
  page += "</style></head><body>";

  page += "<div class='box'>";
  page += "<h2>ROBOBUAS GEGOR</h2>";
  page += "<p>Saved PS3 MAC Address</p>";
  page += "<h3>" + ps3Mac + "</h3>";

  page += "<form action='/save' method='POST'>";
  page += "<p>New PS3 MAC Address</p>";
  page += "<input name='mac' value='" + ps3Mac + "' placeholder='01:23:45:67:89:AB'>";
  page += "<br><br>";
  page += "<button class='save' type='submit'>Save</button>";
  page += "<button class='cancel' type='button' onclick=\"location.href='/cancel'\">Cancel</button>";
  page += "</form>";

  page += "<p class='note'>Format mesti: 01:23:45:67:89:AB</p>";
  page += "<p class='note'>Jika format salah, MAC tidak akan disimpan.</p>";
  page += "<p class='note'>Jika berjaya save, ESP32 akan restart.</p>";

  page += "</div></body></html>";

  return page;
}

void handlePortal() {
  server.send(200, "text/html", htmlPage());
}

// ================= AP MODE =================
void startAP() {
  stopAll();

  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("GEGOR", "12345678");

  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", HTTP_GET, handlePortal);

  server.on("/generate_204", HTTP_GET, []() {
    server.sendHeader("Location", "http://192.168.4.1/", true);
    server.send(302, "text/plain", "");
  });

  server.on("/fwlink", HTTP_GET, []() {
    server.sendHeader("Location", "http://192.168.4.1/", true);
    server.send(302, "text/plain", "");
  });

  server.on("/connecttest.txt", HTTP_GET, []() {
    server.sendHeader("Location", "http://192.168.4.1/", true);
    server.send(302, "text/plain", "");
  });

  server.on("/ncsi.txt", HTTP_GET, []() {
    server.sendHeader("Location", "http://192.168.4.1/", true);
    server.send(302, "text/plain", "");
  });

  server.on("/hotspot-detect.html", HTTP_GET, handlePortal);

  server.on("/save", HTTP_POST, []() {
    if (server.hasArg("mac")) {
      String newMac = server.arg("mac");
      newMac.trim();
      newMac.toUpperCase();

      if (!isValidMac(newMac)) {
        server.send(200, "text/html",
                    "<html><body style='font-family:Arial;text-align:center;background:#111;color:white;padding:20px;'>"
                    "<h2 style='color:red;'>Invalid MAC Address</h2>"
                    "<p>MAC tidak disimpan.</p>"
                    "<p>Format mesti seperti ini:</p>"
                    "<h3 style='color:orange;'>01:23:45:67:89:AB</h3>"
                    "<br>"
                    "<a href='/' style='font-size:22px;color:orange;'>Back</a>"
                    "</body></html>");

        blinkColor(255, 0, 0, 2);
        return;
      }

      ps3Mac = newMac;

      // Simpan kekal dalam memory ESP32.
      // Kekal walaupun power OFF.
      // Hanya berubah bila save MAC baharu.
      prefs.putString("ps3mac", ps3Mac);

      server.send(200, "text/html",
                  "<html><body style='font-family:Arial;text-align:center;background:#111;color:white;padding:20px;'>"
                  "<h2 style='color:lime;'>PS3 MAC Saved</h2>"
                  "<p>MAC address has been saved permanently.</p>"
                  "<p>ESP32 will restart now.</p>"
                  "</body></html>");

      blinkColor(150, 0, 255, 2);

      delay(1200);
      ESP.restart();
    } else {
      server.send(200, "text/html",
                  "<html><body style='font-family:Arial;text-align:center;background:#111;color:white;padding:20px;'>"
                  "<h2 style='color:red;'>No MAC Received</h2>"
                  "<a href='/' style='font-size:22px;color:orange;'>Back</a>"
                  "</body></html>");
    }
  });

  server.on("/cancel", HTTP_GET, []() {
    server.send(200, "text/html",
                "<html><body style='font-family:Arial;text-align:center;background:#111;color:white;padding:20px;'>"
                "<h2>Cancelled</h2>"
                "<p>WiFi AP closed.</p>"
                "</body></html>");

    delay(500);
    ESP.restart();
  });

  server.onNotFound([]() {
    server.sendHeader("Location", "http://192.168.4.1/", true);
    server.send(302, "text/plain", "");
  });

  server.begin();

  while (WiFi.getMode() == WIFI_AP) {
    setColor(255, 80, 0);

    dnsServer.processNextRequest();
    server.handleClient();
    checkButtonsOnlyNoAP();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0 * SCREEN_V_SPACING);
    display.print("AP ");
    display.print("WiFi: GEGOR");

    display.setCursor(0, 1 * SCREEN_V_SPACING);
    display.print("Pass: 12345678");

    display.setCursor(0, 2 * SCREEN_V_SPACING);
    display.print("http://192.168.4.1");

    display.setCursor(0, 3 * SCREEN_V_SPACING);
    display.print("Insert PS3 address");

    display.display();

    delay(5);
  }

  setColor(0, 0, 0);
}

// ================= BUTTON =================
void checkButtonsOnlyNoAP() {
  bool btn2 = digitalRead(BTN2_PIN);

  if (btn2 == LOW && btn2Old == HIGH && currentMode == MODE_PS3) {
    btn2PressTime = millis();
    btn2LongDone = false;
  }

  if (btn2 == LOW && !btn2LongDone && millis() - btn2PressTime > longPressTime) {
    btn2LongDone = true;

    if (currentMode == MODE_RC) restartToMode(MODE_PS3);
    else if (currentMode == MODE_PS3) restartToMode(MODE_BT);
    else restartToMode(MODE_RC);
  }

  btn2Old = btn2;
}

void checkButtons() {
  bool btn1 = digitalRead(BTN1_PIN);
  bool btn2 = digitalRead(BTN2_PIN);

  if (btn1 == LOW && btn1Old == HIGH) {
    btn1PressTime = millis();
    btn1LongDone = false;
  }

  if (btn1 == LOW && !btn1LongDone && millis() - btn1PressTime > longPressTime) {
    btn1LongDone = true;
    startAP();
  }

  if (btn2 == LOW && btn2Old == HIGH) {
    btn2PressTime = millis();
    btn2LongDone = false;
  }

  if (btn2 == LOW && !btn2LongDone && millis() - btn2PressTime > longPressTime) {
    btn2LongDone = true;

    if (currentMode == MODE_RC) restartToMode(MODE_PS3);
    else if (currentMode == MODE_PS3) restartToMode(MODE_BT);
    else restartToMode(MODE_RC);
  }

  btn1Old = btn1;
  btn2Old = btn2;
}

// ================= OLED =================
char cmd;
void updateOLED() {
  if (millis() - lastOled < 300) return;
  lastOled = millis();

  float vbat = readBattery();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (currentMode == MODE_RC) {
    int ch1 = readRC(CH1_PIN);
    int ch2 = readRC(CH2_PIN);

    display.setCursor(0, 0);
    display.print("MODE: RC  BAT:");
    display.print(vbat, 2);
    display.print("V");


    display.setCursor(0, 1 * SCREEN_V_SPACING);
    display.print("CH1:");
    display.print(ch1);
    display.print(" CH2:");
    display.print(ch2);

    display.setCursor(0, 2 * SCREEN_V_SPACING);
    display.print("FW ");
    display.print(getCompFWDisplay());
    display.print(", BW ");
    display.print(getCompBWDisplay());

    if (vbat > VMIN) {
      display.setCursor(0, 3 * SCREEN_V_SPACING);
      display.print("FB:");
      display.print(fwBwSpeed);
      display.print(" L:");
      display.print(leftSpeed);
      display.print(" R:");
      display.print(rightSpeed);
    } else {
      display.setCursor(0, 3 * SCREEN_V_SPACING);
      display.print("LOW BATTERY STOP");
    }

  }

  else if (currentMode == MODE_PS3) {
    display.setCursor(0, 0);
    display.print("MODE: PS3 BAT:");
    display.print(vbat, 2);
    display.print("V");
    if (lowBatteryLock) display.print(" LOCK");

    display.setCursor(0, 1 * SCREEN_V_SPACING);
    if (Ps3.isConnected()) {
      display.print("PS3 CONNECTED");
    } else {
      display.print("PS3 NOT CONNECT");
    }

    if (vbat > VMIN) {
      display.setCursor(0, 2 * SCREEN_V_SPACING);
      display.print("FW ");
      display.print(getCompFWDisplay());
      display.print(", BW ");
      display.print(getCompBWDisplay());
    } else {
      display.setCursor(0, 2 * SCREEN_V_SPACING);
      display.print("LOW BATTERY STOP");
    }

    display.setCursor(0, 3 * SCREEN_V_SPACING);
    display.print("MAC:");
    display.print(ps3Mac);

  }

  else {
    display.setCursor(0, 0);
    display.print("MODE: BT  BAT:");
    display.print(vbat, 2);
    display.print("V");
    if (lowBatteryLock) display.print(" LOCK");

    display.setCursor(0, 1 * SCREEN_V_SPACING);
    display.print("BT:");
    display.print(btName);

    if (vbat > VMIN) {
      display.setCursor(0, 2 * SCREEN_V_SPACING);
      display.print("BTOOTH input ");
      display.print(cmd);
    } else {
      display.setCursor(0, 2 * SCREEN_V_SPACING);
      display.print("LOW BATTERY STOP");
    }

    display.setCursor(0, 3 * SCREEN_V_SPACING);
    display.print("FB:");
    display.print(fwBwSpeed);
    display.print(" L:");
    display.print(leftSpeed);
    display.print(" R:");
    display.print(rightSpeed);
  }

  display.display();
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);

  pinMode(CH1_PIN, INPUT);
  pinMode(CH2_PIN, INPUT);
  pinMode(CH3_PIN, INPUT);
  pinMode(CH4_PIN, INPUT);

  pinMode(M1A_PIN, OUTPUT);
  pinMode(M1B_PIN, OUTPUT);
  pinMode(M2A_PIN, OUTPUT);
  pinMode(M2B_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  digitalWrite(EN_PIN, HIGH);

  //analogReadResolution(12);

  pixel.begin();
  pixel.setBrightness(80);
  setColor(0, 0, 0);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("ROBOBUAS GEGOR");
  display.setCursor(0, 1 * SCREEN_V_SPACING);
  display.print("Starting...");
  display.display();

  prefs.begin("gegor", false);

  currentMode = prefs.getInt("mode", MODE_RC);
  ps3Mac = prefs.getString("ps3mac", "00:00:00:00:00:00");

  // Safety:
  // Kalau MAC dalam memory rosak/salah format,
  // jangan masuk PS3 mode. Reset ke RC mode.
  if (!isValidMac(ps3Mac)) {
    ps3Mac = "00:00:00:00:00:00";
    prefs.putString("ps3mac", ps3Mac);
    currentMode = MODE_RC;
    prefs.putInt("mode", MODE_RC);
  }

  speedLevel = 4;
  maxSpeedNow = speedTable[speedLevel-1];

  stopAll();

  Serial.println("Saved PS3 MAC:");
  Serial.println(ps3Mac);

  setMode(currentMode);
}

// ================= LOOP =================
void loop() {
  checkButtons();

  readTrimmerFW();
  readTrimmerBW();

  if (currentMode == MODE_RC) {
    handleRC();
  }

  else if (currentMode == MODE_PS3) {
    handlePS3();
  }

  else if (currentMode == MODE_BT) {
    handleBT();
  }

  updateOLED();
}
