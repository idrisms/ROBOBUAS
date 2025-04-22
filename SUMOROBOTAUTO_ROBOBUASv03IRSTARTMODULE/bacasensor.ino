
void bacaSensor() {
  if (sensorOppoLow == 1) {
    rightIR = analogRead(SENSOP_KANAN) / 4;
    midRight = analogRead(SENSOP_KANANTENGAH) / 4;    //
    middleIR = analogRead(SENSOP_TENGAH) / 4;         //
    leftIR = analogRead(SENSOP_KIRI) / 4;             //
    midLeft = analogRead(SENSOP_KIRITENGAH) / 4;      //
    rightEdge = analogRead(SENSOR_LANTAI_KANAN) / 4;  //
    leftEdge = analogRead(SENSOR_LANTAI_KIRI) / 4;    //
  } else {
    rightIR = 1023 - analogRead(SENSOP_KANAN) / 4;
    midRight = 1023 - analogRead(SENSOP_KANANTENGAH) / 4;  //
    middleIR = 1023 - analogRead(SENSOP_TENGAH) / 4;       //
    leftIR = 1023 - analogRead(SENSOP_KIRI) / 4;           //
    midLeft = 1023 - analogRead(SENSOP_KIRITENGAH) / 4;    //
    rightEdge = analogRead(SENSOR_LANTAI_KANAN) / 4;       //
    leftEdge = analogRead(SENSOR_LANTAI_KIRI) / 4;         //
  }

  if ((rightIR > thold_turn) || (middleIR < thold_obs) || (leftIR > thold_turn)) {
    detectOppo = 0;  // to reset whether opponnent is detected using any obstacle IR sensor
  }
  if (middleIR > thold_obs) {
    toHit = 1;
  }
  irStart = digitalRead(IRSTART_MODULE);
}
