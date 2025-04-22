// Motor control functions
void moveForward(int valspeed) {
  if (motorOn == 1) {
    toaccel = 1;

    analogWrite(MOTOR_A_LPWM, 0);         // delay(1);
    analogWrite(MOTOR_A_RPWM, valspeed);  //delay(1);
    analogWrite(MOTOR_B_LPWM, 0);         //delay(1);
    analogWrite(MOTOR_B_RPWM, valspeed);  //delay(1);
  }
  if (serialOn == 1) {
    Serial.println("moveForward");
  }
}

void moveBackward(int valpwm) {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_RPWM, 0);
    analogWrite(MOTOR_A_LPWM, valpwm);
    analogWrite(MOTOR_B_RPWM, 0);
    analogWrite(MOTOR_B_LPWM, valpwm);
  }
  if (serialOn == 1) {
    Serial.println("moveBackward");
  }
}

void turnRight(int valpwm) {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_RPWM, 0);
    analogWrite(MOTOR_A_LPWM, valpwm);
    analogWrite(MOTOR_B_LPWM, 0);
    analogWrite(MOTOR_B_RPWM, valpwm);
  }
  if (serialOn == 1) {
    Serial.println("turnRight");
  }
}

void turnLeft(int valpwm) {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_LPWM, 0);
    analogWrite(MOTOR_A_RPWM, valpwm);
    analogWrite(MOTOR_B_RPWM, 0);
    analogWrite(MOTOR_B_LPWM, valpwm);
  }
  if (serialOn == 1) {
    Serial.println("turnLeft");
  }
}

void Henti() {
  toaccel = 1;
  analogWrite(MOTOR_A_RPWM, 0);
  analogWrite(MOTOR_A_LPWM, 0);
  analogWrite(MOTOR_B_RPWM, 0);
  analogWrite(MOTOR_B_LPWM, 0);
  if (serialOn == 1) {
    Serial.println("Henti");
  }
}

void moveForwardLeft(int valspeed) {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_LPWM, 0);             // delay(1);
    analogWrite(MOTOR_A_RPWM, valspeed);      //delay(1);
    analogWrite(MOTOR_B_LPWM, 0);             //delay(1);
    analogWrite(MOTOR_B_RPWM, valspeed / 2);  //delay(1);
    //Serial.println("moveForward");
  }
}

void curveRight(int valpwm, int valpwm2) {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_RPWM, valpwm2);
    analogWrite(MOTOR_A_LPWM, 0);
    analogWrite(MOTOR_B_LPWM, 0);
    analogWrite(MOTOR_B_RPWM, valpwm);
  }
  //Serial.print("turnRight");
}

void curveLeft(int valpwm, int valpwm2) {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_LPWM, 0);
    analogWrite(MOTOR_A_RPWM, valpwm);
    analogWrite(MOTOR_B_RPWM, valpwm2);
    analogWrite(MOTOR_B_LPWM, 0);
  }
}

void reverseLeft() {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_RPWM, 0);
    analogWrite(MOTOR_A_LPWM, pwmRev);
    analogWrite(MOTOR_B_LPWM, pwmRev - (pwmRev / 3));
    analogWrite(MOTOR_B_RPWM, 0);
  }
  //Serial.print("reverseLeft");
}

void reverse(int valpwm) {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_RPWM, 0);
    analogWrite(MOTOR_A_LPWM, valpwm);
    analogWrite(MOTOR_B_LPWM, valpwm);
    analogWrite(MOTOR_B_RPWM, 0);
  }
  //Serial.print("reverse");
}
void reverseRight(int valpwm) {
  if (motorOn == 1) {
    toaccel = 1;
    analogWrite(MOTOR_A_LPWM, 0);
    analogWrite(MOTOR_A_RPWM, valpwm);
    analogWrite(MOTOR_B_RPWM, valpwm - (valpwm / 3));
    analogWrite(MOTOR_B_LPWM, 0);
  }
  //Serial.print("void reverseRight(int valpwm) {
}
