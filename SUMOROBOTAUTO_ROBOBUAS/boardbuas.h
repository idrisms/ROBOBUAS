#ifndef BOARDBUAS_H
#define BOARDBUAS_H

//// tentukan papan litar yang nak digunakan (buang // untuk guna)
#define BUYOV1
//#define BOLOHV1
//#define LAIN

#ifdef BOLOHV1
#define SENSOR_LANTAI_KANAN 25  //EDGE SENSOR
#define SENSOP_KANAN 33
#define SENSOP_KANANTENGAH 32
#define SENSOP_TENGAH 35
#define SENSOP_KIRITENGAH 34  // SUMO AUTO PAKAI 5 sensor je, Analog
#define SENSOP_KIRI 39        //SENSOP = SENSOR opponent
#define SENSOR_LANTAI_KIRI 36
#define PIN_MULA1 13
//#define PIN_MULA2 17
//#define PIN_MULA3 16
//#define PIN_MULA4 4
#define MOTOR_A_RPWM 19  //motor kanan
#define MOTOR_A_LPWM 21
#define MOTOR_B_RPWM 18  // motor kiri
#define MOTOR_B_LPWM 17
#define LED_BUILTIN 2  //for ersp32

#elif defined(BUYOV1)
#define SENSOP_KANAN 27
#define SENSOP_KANANTENGAH 26
#define SENSOP_TENGAH 14
#define SENSOP_KIRITENGAH 12  // SUMO AUTO PAKAI 5 sensor je, Analog
#define SENSOP_KIRI 13        //SENSOP = SENSOR opponent
#define SENSOR_LANTAI_KANAN 33  //EDGE SENSOR
#define SENSOR_LANTAI_KIRI 32
#define PIN_MULA1 34
//#define PIN_MULA2 17
//#define PIN_MULA3 16
//#define PIN_MULA4 4
#define MOTOR_A_RPWM 19  //motor kanan
#define MOTOR_A_LPWM 18
#define MOTOR_B_RPWM 17  // motor kiri
#define MOTOR_B_LPWM 16
#define LED_BUILTIN 2  //for ersp32
#else
//#error "board error"
#endif

#endif