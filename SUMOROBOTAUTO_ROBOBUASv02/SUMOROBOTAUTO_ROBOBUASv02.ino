/* Pengenalan SUMO AUTO KOD 
 *  Dibina Pada 4 SEPTEMBER 2024
 *  Dihasilkan oleh IDRIS BIN MAT SAHAT
 *  khas untuk BOARD ROBOBUAS
 Hai semua, selamat bermain kod, selamat pening sama-sama hahahahaha
 All the best ye. Sama-sama belajar koding untuk menjadi negara yang unggul
 MAJULAH SUMO UNTUK NEGARA

 - KOD MASIH DI PERINGKAT R&D TAPI BOLEH LAH DIGUNAKAN. 
 Nak pakai board lain pon boleh tapi pastikan guna pin yang betul
   ___  ____  ___  ____  ___  __  _____   ____
  / _ \/ __ \/ _ )/ __ \/ _ )/ / / / _ | / __/
 / , _/ /_/ / _  / /_/ / _  / /_/ / __ |_\ \  
/_/|_|\____/____/\____/____/\____/_/ |_/___/  
https://sites.google.com/view/robobuasbuyopage/robobuas
                                                                                             
 */
#include "boardbuas.h"

//#define JENISBOARD BOLOHV1// board yang ada buat masa sekarang adalah BUYOV1 dan BOLOHV1 dan LAIN
// masukkan dalam kurungan jenis board tu BUYOV1 atau BOLOHV1
// ubah di garis ke-4 dalam tab boardbuas.h

#define thold_edge 900   // nilai genting untuk pusing robot, standard 800
#define thold_turn 300   // untuk fs80nk = 0
#define thold_obs 0      //untuk analog 300, untuk fs80nk 0
#define sensorOppoLow 0  // sensor jenis FS80NK Jenis detect opponent nilai rendah
// kod asal berdasarkan on FS80NK, sharp pon boleh tapi kena ubah lain
// sharp sensor bila detect opponent,nilai tinggi, so kena set sensorOppoLow 0

//----------------UBAH NILAI BERGANTUNG KEPADA KEPERLUAN-----------------
//delay, asal ade perkataan ini, lagi besar nilainya lagi lama proses sebelumnya
//pwm, lagi besar nilainya lagi laju robot. 0 berhenti, 255 paling laju.

const int delayloophit = 10;  //untuk delayloophit, ubah nilai besar sikit supaya tak tercongek
//cuba dengan tayar paling bersih
const int delayturn = 2, delayBackward = 300;
const int pwmVal = 90;     //
const int pwmSearch = 90;  //kelajuan mencari opponent, jangan terlampau laju takut terlebih edge
const int pwmRev = 150;    // kelajuan untuk. tak terlalu laju takut gelincir
const int pwmTurn = 110;
const int pwmTurnFast = 100;  // turning to opponent
const int pwmHit = 255;

int midRight, rightIR, middleIR, midLeft, leftIR, rightEdge, leftEdge, toHit;
String cond;
int toaccel = 1;
int detectOppo = 0;

const int Strategy = 1;    // tukar strategy dalam lawan
const int motorOn = 1;     //--- sini kalau nak off motor, 1 on,  0 off
const int tracerOnly = 0;  // tukar jadi 1 untuk  tracking lawan tanpa langgar, hanya follow, 0 untuk  tracking dan langgar
const int serialOn = 1;    //
int state = 0;

//---------------- FOR RC ---------------------------//
int ch1;  //Variables to store and display the values of each channel
int ch2;  //Variables to store and display the values of each channel

int ch1Min = 1000;  // read from serial monitor
int ch1Max = 2000;  // read from serial monitor
int ch2Min = 1000;  // read from serial monitorQ
int ch2Max = 2000;  // read from serial monitor

int offset = 40;  // Offset value for STOP function, get value from Serial Monitor
int val;
//----------------EN RC---------------------??

void setup() {
  pinMode(PIN_MULA1, INPUT_PULLUP);
  pinMode(chA, INPUT);
  pinMode(chB, INPUT);
  // pinMode(PIN_MULA2, INPUT_PULLUP);
  // pinMode(PIN_MULA3, INPUT_PULLUP);
  // pinMode(PIN_MULA4, INPUT_PULLUP);
  pinMode(SENSOP_KANAN, INPUT);
  pinMode(SENSOP_TENGAH, INPUT);
  pinMode(SENSOP_KIRI, INPUT);
  pinMode(SENSOR_LANTAI_KANAN, INPUT);
  pinMode(SENSOR_LANTAI_KIRI, INPUT);
  pinMode(SENSOP_KANANTENGAH, INPUT);
  pinMode(SENSOP_KIRITENGAH, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(MOTOR_A_RPWM, OUTPUT);
  pinMode(MOTOR_A_LPWM, OUTPUT);
  pinMode(MOTOR_B_RPWM, OUTPUT);
  pinMode(MOTOR_B_LPWM, OUTPUT);

  for (int i = 0; i < 5; i++) {  //lampu controller berkelip laju menandakan robot sudah sedia beraksi
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
  bacaSensor();
  Serial.begin(9600);
  suismula(5) ;// 5 = 5 sat, 4 = 4 saat, 3=3 saat
  //
}
void loop() {

  /* untuk bahagian ini, untuk test robot sila pilih antara 1,2,3 atau 4.
    Hanya perlu guna satu pilihan sahaja untuk satu-satu masa. Perlu mula dari
    pilihan 1 sehingga 4 untuk cek samaada robot anda perlukan pembetulan ataupun tak.
    buang "//" yanga paling depan sekali untuk guna dan tambah "//" jika tidak guna.
  */
  //cubagerakan();  //1. cek pergerakan robot, perkara pertama perlu buat

  //  sensorsahaja();  //2. bacaan sensor sahaja untuk tengok sensor ok ke tak

  //suismula(); Henti(); // 3. untuk test perlbagai suis mula, bergantung kepada jumlah permulaan

  // lawan();  // 4. perlawanan sebenar

 
  //rcOnly();//rc sahaja masih dalam RND
}
