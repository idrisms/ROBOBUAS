//untuk robot yang terconget, ubah delayloophit lagi besar 
void AccelForward(int valspeed) {
  if (motorOn == 1) {
    if (toaccel == 1) {  //prevent flipping when hitting
      int xled = 0;
      for (int aspeed = 0; aspeed <= (valspeed); aspeed = aspeed + 5) {
        xled++;
        if ((xled % 2) == 1) {
          digitalWrite(LED_BUILTIN, LOW);
        } else {
          digitalWrite(LED_BUILTIN, HIGH);
        }
        analogWrite(MOTOR_A_LPWM, 0);       // delay(1);
        analogWrite(MOTOR_A_RPWM, aspeed);  //delay(1);
        analogWrite(MOTOR_B_LPWM, 0);       //delay(1);
        analogWrite(MOTOR_B_RPWM, aspeed);  //delay(1);
        bacaSensor();
        delay(delayloophit);
        if ((middleIR < thold_obs) || ((rightEdge < thold_edge) || (leftEdge < thold_edge))) {
          Henti();
          break;
        }
        // jika middle ir <thold = tak detect opponent, rightedge<thold = detect line putin
        //Serial.println("Accelerate");
      }

    } else {  //
      digitalWrite(LED_BUILTIN, HIGH);
      analogWrite(MOTOR_A_LPWM, 0);         // delay(1);
      analogWrite(MOTOR_A_RPWM, valspeed);  //delay(1);
      analogWrite(MOTOR_B_LPWM, 0);         //delay(1);
      analogWrite(MOTOR_B_RPWM, valspeed);  //delay(1);
    }
  }
  toaccel = 0;  // flush value to prevent reaccelarate for continous hitting//
  //Serial.println("AccelForward");
  // may flip grip if attempt to max speed from 0
}
