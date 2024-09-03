
void bacaSensor() {
  rightIR = analogRead(SENSOP_KANAN) / 4;
  midRight = analogRead(SENSOP_KANANTENGAH) / 4;     //
  middleIR = analogRead(SENSOP_TENGAH) / 4;         //
  leftIR = analogRead(SENSOP_KIRI) / 4;             //
  midLeft = analogRead(SENSOP_KIRITENGAH) / 4;       //
  rightEdge = analogRead(SENSOR_TEPI_KANAN) / 4;  //
  leftEdge = analogRead(SENSOR_TEPI_KIRI) / 4;    //

  if ((rightIR > thold_turn) || (middleIR < thold_obs) || (leftIR > thold_turn)) {
    detectOppo = 0;  // to reset whether opponnent is detected using any obstacle IR sensor
  }
  if (middleIR > thold_obs) {
    toHit = 1;
  }

}
