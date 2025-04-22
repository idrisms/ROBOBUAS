void lawan_semiauto() {
  bacaSensor();
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
  /*
  // Edge detection ---- IF VALUE LESS then 100 (value is higher if detect white line (if near) OR no obstacle(far from floor))
  if ((rightEdge < thold_edge) && (leftEdge > thold_edge)) {  //&& (middleIR == 0)
    // Move backward
    toaccel = 1;  //ready to accelerate
    cond = "White line 1!";
    printSerial();
    Henti();
    delay(30);
    reverse(pwmRev);
    delay(350);
    turnLeft(pwmTurnFast);
    delay(150);

  }

  else if ((leftEdge < thold_edge) && (rightEdge > thold_edge)) {  //&& (middleIR == 0)
    // Move backward
    toaccel = 1;  //ready to accelerate
    cond = "White line 2!";
    printSerial();
    Henti();
    delay(30);
    reverse(pwmRev);
    delay(350);
    turnRight(pwmTurnFast);
    delay(150);
  }

  if ((rightEdge < thold_edge) && (leftEdge < thold_edge) && (motorOn == 1)) { 
    // Move backward
    toaccel = 1;  //ready to accelerate
    cond = "White line 3!";
    printSerial();

    reverse(pwmRev);
    delay(350);
    turnLeft(pwmTurn);
    delay(150);
  }*/

  //   End of EDGE DETECTION----------------------------------------

  // enemy on Right, turn Right
  if (rightIR > thold_turn) {
    cond = "Turn Right";
    Serial.print(cond);
    Serial.println("  ");
    if (motorOn == 1) {
      turnRight(pwmTurnFast);
      delay(delayturn);
      toaccel = 1;
    }
    //Henti();
  }

  // enemy on Right, turn Right
  /*else if ((midRight > thold_turn) && (SpeedCh2 < offset) && (SpeedCh1 < offset)) {
    cond = "mid Turn Right";
    Serial.print(cond);
    Serial.println("  ");
    if (motorOn == 1) {
      turnRight(pwmTurnFast/2);
      delay(delayturn / 2);
      toaccel = 1;
    }
    //Henti();
  }*/

  //enemy on left, turn left
  else if ((leftIR > thold_turn)) {
    cond = "Turn left";
    Serial.print(cond);
    Serial.print("  ");
    //printSerial(cond);
    if (motorOn == 1) {
      turnLeft(pwmTurnFast);
      delay(delayturn);
      toaccel = 1;
    }
    //Henti();
  }

 /* else if ((midLeft > thold_turn) && (SpeedCh2 < offset) && (SpeedCh1 < offset)) {
    cond = "mid Turn left";
    Serial.print(cond);
    Serial.print("  ");
    //printSerial(cond);
    if (motorOn == 1) {
      turnLeft(pwmTurnFast/2);
      delay(delayturn / 2);
      toaccel = 1;
    }
    //Henti();
  }*/

  else if ((tracerOnly != 1) && (rightIR < thold_turn) && (leftIR < thold_turn)) {
    //Strategy 1 moveforward while searching and destroy
    //Strategy 2 tornado
    //Strategy 3 movefor
    cond = "strategy ";
    printSerial();
    if ((tracerOnly == 0)) {
      switch (Strategy) {
        case 1:
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

          //delay(1);
          break;
        case 2:
          //turnRight(pwmTurnFast);
          //delay(20);
          break;
        case 3:
          // statements
          break;
        default:
          // statements
          break;
      }
      toaccel = 1;
    }

    //Henti(); delay(50);
  } else {
    if (tracerOnly == 1) {
      Henti();
      cond = "Henti";
      printSerial();
      toaccel = 1;
    }
  }
  delay(1);
}  //end loop 1
