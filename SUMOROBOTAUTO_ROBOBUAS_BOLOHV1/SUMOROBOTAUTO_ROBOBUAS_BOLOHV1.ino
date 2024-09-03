/*1/9/2023 https://sites.google.com/view/robobuasbuyov1 untuk BUYO
   guna ESP32 Wroom dan shield ROBOBUAS BOLOH v1.0
  remember esp 32 Analog is 12 bit = 1024*2*2=4096 but max is 4095 because start with 0

          Pandangan atas robot seperti dalam gambar

  Suis
                  (kedudukan sensor dan motor ROBOBUAS BOLOH))
  motorkiri edgekiri kiri kiri-tengah  tengah  kanan-tengah kanan  Edgkanan motorkanan
  19  21     36      39      34        35         32        33       25       18 17

*/

//-------------------- PIN DEFINITION TENGOK ROBOT DARI TAYAR
//---------------------untuk tahu kiri dan kanan untuk ROBOBUAS BUYO
#define SENSOR_TEPI_KANAN 36 //EDGE SENSOR
#define SENSOP_KANAN 34
#define SENSOP_KANANTENGAH 39
#define SENSOP_TENGAH 35
#define SENSOP_KIRITENGAH 33  // SUMO AUTO PAKAI 5 sensor je, Analog
#define SENSOP_KIRI 32       //SENSOP = SENSOR opponent
#define SENSOR_TEPI_KIRI 25
#define PIN_START1 13
//#define PIN_START2 17
//#define PIN_START3 16
//#define PIN_START4 4
#define MOTOR_A_RPWM 23 //motor kanan
#define MOTOR_A_LPWM 22
#define MOTOR_B_RPWM 16  // motor kiri
#define MOTOR_B_LPWM 4
#define LED_BUILTIN 2  //for ersp32

//-------------------- END OFF PIN DEFINITION

#define thold_edge 800 // nilai genting untuk pusing robot
#define thold_turn 300 //
#define thold_obs 300 //untuk analog

//----------------UBAH NILAI BERGANTUNG KEPADA KEPERLUAN-----------------
//delay, asal ade perkataan ini, lagi besar nilainya lagi lama proses sebelumnya
//pwm, lagi besar nilainya lagi laju robot. 0 berhenti, 255 paling laju.

const int delayloophit = 5; //untuk delayloophit, ubah nilai besar sikit supaya tak tercongek
//cuba dengan tayar paling bersih
const int delayturn = 10, delayBackward = 300;
const int pwmVal = 90;//
const int pwmSearch = 80; //kelajuan mencari opponent, jangan terlampau laju takut terlebih edge
const int pwmRev = 100; // kelajuan untuk. tak terlalu laju takut gelincir
const int pwmTurn = 70;
const int pwmTurnFast = 150;  // turning to opponent
const int pwmHit = 255;

int midRight, rightIR, middleIR, midLeft, leftIR, rightEdge, leftEdge, toHit;
String cond;
int toaccel = 1;
int detectOppo = 0;

const int Strategy = 1; // tukar strategy dalam lawan
const int motorOn = 1 ; //--- sini kalau nak off motor, 1 on,  0 off
const int tracerOnly = 0; // tukar jadi 1 untuk lawan tanpa langgar, hanya follow
//0 = lawan
const int serialOn = 1;  //
int state = 0;

void setup() {
  pinMode(PIN_START1, INPUT_PULLUP);
  // pinMode(PIN_START2, INPUT_PULLUP);
  // pinMode(PIN_START3, INPUT_PULLUP);
  // pinMode(PIN_START4, INPUT_PULLUP);
  pinMode(SENSOP_KANAN, INPUT);
  pinMode(SENSOP_TENGAH, INPUT);
  pinMode(SENSOP_KIRI, INPUT);
  pinMode(SENSOR_TEPI_KANAN, INPUT);
  pinMode(SENSOR_TEPI_KIRI, INPUT);
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
  suismula();
}

void loop() {

  /* untuk bahagian ini, untuk test robot sila pilih antara 1,2,3 atau 4.
    Hanya perlu guna satu pilihan sahaja untuk satu-satu masa. Perlu mula dari
    pilihan 1 sehingga 4 untuk cek samaada robot anda perlukan pembetulan ataupun tak.
    buang "//" yanga paling depan sekali untuk guna dan tambah "//" jika tidak guna.
  */

  //cubagerakan();    //1. cek pergerakan robot, perkara pertama perlu buat

  //sensorsahaja();   //2. bacaan sensor sahaja untuk tengok sensor ok ke tak

  //suismula(); Henti(); // 3. untuk test perlbagai suis mula, bergantung kepada jumlah permulaan

  lawan();  // 4. perlawanan sebenar
}
