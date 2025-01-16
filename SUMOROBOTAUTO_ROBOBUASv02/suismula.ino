void delaytime(int cycle) {
  for (int xstart = 0; xstart < cycle; xstart++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);  //competition rules 3 second//competition rules 3 second IIUMRC
  }
}

void suismula(int masa) {
  if (suisinverse == 1) {
    while (state == 0) {

      if (digitalRead(PIN_MULA1) == 1) {
        if (serialOn == 0) {
          Serial.println("No1");
        }
        delaytime(masa);  // 5 = 5 saat delay, 3 = 3 saat delay
        state = 0;
        break;
      }
    }
  }
  else{
    while (state == 0) {

      if (digitalRead(PIN_MULA1) == 0) {
        if (serialOn == 0) {
          Serial.println("No1");
        }
        delaytime(masa);  // 5 = 5 saat delay, 3 = 3 saat delay
        state = 0;
        break;
      }
    }
  }
}
