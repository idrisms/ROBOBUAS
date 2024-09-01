void delaytime(int cycle) {
  for (int xstart = 0; xstart < cycle; xstart++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);  //competition rules 3 second//competition rules 3 second IIUMRC
  }
}

void suismula() {
  while (state == 0) {

    if (digitalRead(PIN_START1) == 0) {
      if (serialOn == 1) {
        Serial.println("No1");
      }
      delaytime(5);  // 5 = 5 saat delay, 3 = 3 saat delay
      state = 0;
      break;
    }
    /*else if (digitalRead(PIN_START2) == 0) {
      delaytime(5);  // how many seconds
      if (serialOn == 1) {
        Serial.println("No1");
      }
      turnLeft(255);
      delay(60);
      curveLeft(40, 60);
      delay(100);
      curveLeft(50, 120);
      delay(100);
      curveLeft(50, 180);
      delay(100);
      curveLeft(30, 255);
      delay(150);
      curveLeft(40, 255);
      delay(100);
      state = 0;
      break;
    }

    else if (digitalRead(PIN_START3) == 0) {
      if (serialOn == 1) {
        Serial.println("No3");
      }
      delaytime(5);
      state = 0;
      break;
    }

    else if (digitalRead(PIN_START4) == 0) {
      if (serialOn == 1) {
        Serial.println("No4");
      }
      delaytime(5);  // how many seconds
      turnRight(255);
      delay(60);
      curveRight(40, 60);
      delay(100);
      curveRight(50, 120);
      delay(100);
      curveRight(50, 180);
      delay(100);
      curveRight(30, 255);
      delay(150);
      curveRight(40, 255);
      delay(100);
      state = 0;
      break;
    }
    //delay(10);*/
  }
}