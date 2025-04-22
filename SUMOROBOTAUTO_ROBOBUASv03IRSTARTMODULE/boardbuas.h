#ifndef BOARDBUAS_H
#define BOARDBUAS_H

// Define the board version to be used (uncomment one line to select)
//#define BUYOV1
//#define HOGOHV1
#define BOLOHV2RC


#ifdef BOLOHV2RC
#define SENSOP_KANAN 39
#define SENSOP_KANANTENGAH 34
#define SENSOP_TENGAH 35
#define SENSOP_KIRITENGAH 32   // SUMO AUTO uses 5 sensors, Analog
#define SENSOP_KIRI 33          // SENSOP = SENSOR opponent
#define SENSOR_LANTAI_KANAN 36  // EDGE SENSOR
#define SENSOR_LANTAI_KIRI 25
#define PIN_MULA1 26
#define chA 13
#define chB 14
#define suisinverse 0
#define IRSTART_MODULE 27
//#define PIN_MULA2 17
//#define PIN_MULA3 16
//#define PIN_MULA4 4
#define MOTOR_A_RPWM 21  // motor kanan
#define MOTOR_A_LPWM 22
#define MOTOR_B_RPWM 16  // motor kiri
#define MOTOR_B_LPWM 4
#define LED_BUILTIN 2  // for ersp32

#elif defined(BUYOV1)
#define SENSOP_KANAN 27
#define SENSOP_KANANTENGAH 26
#define SENSOP_TENGAH 14
#define SENSOP_KIRITENGAH 12    // SUMO AUTO uses 5 sensors, Analog
#define SENSOP_KIRI 13          // SENSOP = SENSOR opponent
#define SENSOR_LANTAI_KANAN 33  // EDGE SENSOR
#define SENSOR_LANTAI_KIRI 32
#define PIN_MULA1 34
#define chA 13
#define chB 14
#define suisinverse 1
//#define PIN_MULA2 17
//#define PIN_MULA3 16
//#define PIN_MULA4 4
#define MOTOR_A_RPWM 19  // motor kanan
#define MOTOR_A_LPWM 18
#define MOTOR_B_RPWM 17  // motor kiri
#define MOTOR_B_LPWM 16
#define LED_BUILTIN 2  // for ersp32

#elif defined(HOGOHV1)
#define SENSOP_KANAN 39
#define SENSOP_KANANTENGAH 34
#define SENSOP_TENGAH 35
#define SENSOP_KIRITENGAH 32    // SUMO AUTO uses 5 sensors, Analog
#define SENSOP_KIRI 33          // SENSOP = SENSOR opponent
#define SENSOR_LANTAI_KANAN 36  // EDGE SENSOR
#define SENSOR_LANTAI_KIRI 25
#define PIN_MULA1 26
#define chA 13
#define chB 14
#define IRSTART_MODULE 27
#define suisinverse 0
//#define PIN_MULA2 17
//#define PIN_MULA3 16
//#define PIN_MULA4 4
#define MOTOR_A_RPWM 19  // motor kanan
#define MOTOR_A_LPWM 23

#define MOTOR_B_RPWM 17  // motor kiri
#define MOTOR_B_LPWM 18
#define LED_BUILTIN 2  // for ersp32

#else
#error "No valid board selected. Please uncomment one board definition."
#endif

#endif  // BOARDBUAS_H
