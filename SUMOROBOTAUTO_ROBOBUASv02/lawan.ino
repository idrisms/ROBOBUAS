void lawan() {
  bacaSensor();

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
    delay(250);

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
    delay(250);
  }

  if ((rightEdge < thold_edge) && (leftEdge < thold_edge)) {  //&& (middleIR == 0)
    // Move backward
    toaccel = 1;  //ready to accelerate
    cond = "White line 3!";
    printSerial();

    reverse(pwmRev);
    delay(350);
    turnLeft(pwmTurn);
    delay(250);
  }

  //   End of EDGE DETECTION----------------------------------------

  // enemy on Right, turn Right
  if (((rightIR > thold_turn) || (midRight > thold_turn)) && (middleIR == thold_obs) && (rightEdge > thold_edge) && (leftEdge > thold_edge)) {
    cond = "Turn Right";
    printSerial();
    turnRight(pwmTurnFast);
    delay(delayturn);
    toaccel = 1;
    //Henti();
  }
  // got enemy infront, move forward
  else if ((middleIR > thold_obs) && ((rightEdge > thold_edge) && (leftEdge > thold_edge))) {  // && (rightEdge > thold_edge) && (leftEdge > thold_edge)
    cond = "Attack!!";
    printSerial();
    if (tracerOnly == 0) {
      //Henti();//moveForward(pwmHit);

      //AccelForward(pwmHit);
      moveForward(pwmHit);
      //toHit=0;
    }
  }

  //enemy on left, turn left
  else if (((leftIR > thold_turn) || (midLeft > thold_turn)) && (middleIR == thold_obs) && (rightEdge > thold_turn) && (leftEdge > thold_edge)) {
    cond = "Turn left";
    printSerial();
    turnLeft(pwmTurnFast);
    delay(delayturn);
    toaccel = 1;
    //Henti();
  }

  else if ((tracerOnly != 1) && (rightIR < thold_turn) && (middleIR <= thold_obs) && (leftIR < thold_turn) && (rightEdge > thold_edge) && (leftEdge > thold_edge)) {
    //Strategy 1 moveforward while searching and destroy
    //Strategy 2 tornado
    //Strategy 3 movefor
    cond = "strategy ";
    printSerial();
    if (tracerOnly == 0) {
      switch (Strategy) {
        case 1:
          moveForward(pwmSearch);
          //delay(1);
          break;
        case 2:
          turnRight(pwmTurnFast);
          delay(20);
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
}  //end loop 1
