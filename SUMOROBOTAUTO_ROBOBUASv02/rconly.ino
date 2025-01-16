void rcOnly() {

  //cetakBacaan();
  ch1 = pulseIn(chA, HIGH);  //Read and store channel 1
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


  if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 > 0) {
    Serial.print("FORWARD");
    Serial.print("|");
    analogWrite(MOTOR_A_RPWM, 0);
    analogWrite(MOTOR_A_LPWM, abs(SpeedCh2));
    analogWrite(MOTOR_B_RPWM, 0);
    analogWrite(MOTOR_B_LPWM, abs(SpeedCh2) - abs(SpeedCh1));
  }

  else if (SpeedCh2 > offset && SpeedCh2 <= 255 && SpeedCh1 <= 0) {
    Serial.print("FORWARD");
    Serial.print("|");
    analogWrite(MOTOR_A_RPWM, 0);
    analogWrite(MOTOR_A_LPWM, abs(SpeedCh2) - abs(SpeedCh1));
    analogWrite(MOTOR_B_RPWM, 0);
    analogWrite(MOTOR_B_LPWM, abs(SpeedCh2));
  }

  else if (SpeedCh2 < -offset && SpeedCh2 >= -255) {
    Serial.print("BACKWARD");
    Serial.print("|");
    analogWrite(MOTOR_A_RPWM, abs(SpeedCh2));
    analogWrite(MOTOR_A_LPWM, 0);
    analogWrite(MOTOR_B_RPWM, abs(SpeedCh2));
    analogWrite(MOTOR_B_LPWM, 0);
  }

  else if (SpeedCh1 > offset && SpeedCh1 <= 255) {
    Serial.print("Turn RIGHT");
    Serial.print("|");
    analogWrite(MOTOR_A_RPWM, 0);
    analogWrite(MOTOR_A_LPWM, abs(SpeedCh1));
    analogWrite(MOTOR_B_RPWM, abs(SpeedCh1));
    analogWrite(MOTOR_B_LPWM, 0);
  }

  else if (SpeedCh1 < -offset && SpeedCh1 >= -255) {
    Serial.print("Turn LEFT");
    Serial.print("|");
    analogWrite(MOTOR_A_RPWM, abs(SpeedCh1));
    analogWrite(MOTOR_A_LPWM, 0);
    analogWrite(MOTOR_B_RPWM, 0);
    analogWrite(MOTOR_B_LPWM, abs(SpeedCh1));
  }


  else if (SpeedCh1 <= offset && SpeedCh1 >= -offset) {
    Serial.print("STOP");
    Serial.print("|");
    // Move in reverse

    analogWrite(MOTOR_A_LPWM, 0);
    analogWrite(MOTOR_A_RPWM, 0);
    analogWrite(MOTOR_B_LPWM, 0);
    analogWrite(MOTOR_B_RPWM, 0);
  }
}