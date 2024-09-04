/*void JENISBOARD(String BOARD) {

  /*1/9/2023 https://sites.google.com/view/robobuasbuyov1 untuk BUYO
     guna ESP32 Wroom dan shield ROBOBUAS BUYO DAN BOLOH SAHAJA
    remember esp 32 Analog is 12 bit = 1024*2*2=4096 but max is 4095 because start with 0

            Pandangan atas robot seperti dalam gambar

    Suis
                    (kedudukan sensor dan motor ROBOBUAS BOLOH))
    motorkiri edgekiri kiri kiri-tengah  tengah  kanan-tengah kanan  Edgkanan motorkanan
    19  21     36      39      34        35         32        33       25       18 17
    masih bergantung kepada cara pemasangan sensor
  
  if (BOARD == "BOLOHV1") {
    //--------- UNTUK ROBOBUAS BOLOH-----------
   
  }
  else if (BOARD == "BUYOHV1") {
    //--------- UNTUK ROBOBUAS BUYO-----------
      const int SENSOR_LANTAI_KANAN 25 //EDGE SENSOR
      const int SENSOP_KANAN 33
      const int SENSOP_KANANTENGAH 32
      const int SENSOP_TENGAH 35
      const int SENSOP_KIRITENGAH 34  // SUMO AUTO PAKAI 5 sensor je, Analog
      const int SENSOP_KIRI 39       //SENSOP = SENSOR opponent
      const int SENSOR_LANTAI_KIRI 36
      const int PIN_MULA1 13
      //const int PIN_MULA2 17
      //const int PIN_MULA3 16
      //const int PIN_MULA4 4
      const int MOTOR_A_RPWM 19 //motor kanan
      const int MOTOR_A_LPWM 21
      const int MOTOR_B_RPWM 18  // motor kiri
      const int MOTOR_B_LPWM 17
      const int LED_BUILTIN 2  //for ersp32
    
    //-------------------- END OFF PIN DEFINITION
  }
    else if (BOARD == "LAIN") {
    //--------- UNTUK ROBOBUAS BUYO-----------
      const int SENSOR_LANTAI_KANAN 25 //EDGE SENSOR
      const int SENSOP_KANAN 33
      const int SENSOP_KANANTENGAH 32
      const int SENSOP_TENGAH 35
      const int SENSOP_KIRITENGAH 34  // SUMO AUTO PAKAI 5 sensor je, Analog
      const int SENSOP_KIRI 39       //SENSOP = SENSOR opponent
      const int SENSOR_LANTAI_KIRI 36
      const int PIN_MULA1 13
      //const int PIN_MULA2 17
      //const int PIN_MULA3 16
      //const int PIN_MULA4 4
      const int MOTOR_A_RPWM 19 //motor kanan
      const int MOTOR_A_LPWM 21
      const int MOTOR_B_RPWM 18  // motor kiri
      const int MOTOR_B_LPWM 17
      const int LED_BUILTIN 2  //for ersp32
    
    //-------------------- END OFF PIN DEFINITION
  }

}
*/