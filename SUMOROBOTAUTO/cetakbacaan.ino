void sensorsahaja() {
  bacaSensor();
  cetakBacaan();
  //delay(10);

}

void cetakBacaan() {
  Serial.print(", TL: ");
  print4digit(midLeft);  //Serial.print(midLeft);
  Serial.print(", ML: ");
  print4digit(leftIR);  //Serial.print(leftIR);
  Serial.print(", Mid: ");
  print4digit(middleIR);  //Serial.print(middleIR);
  Serial.print("Kanantengah:");
  print4digit(rightIR);  //Serial.print(rightIR);
  Serial.print("Kanan:");
  print4digit(midRight);  //Serial.print(midRight);
  Serial.print(", L-edge: ");
  print4digit(leftEdge);  //Serial.print(leftEdge);
  Serial.print(", R-edge: ");
  print4digit(rightEdge);  //Serial.print(rightEdge);
  Serial.print(", toaccel:");
  Serial.print(toaccel);
  Serial.print(", detectOppo:");
  Serial.print(detectOppo);
  Serial.println();
}


void printSerial() {
  if (serialOn == 1) {
    Serial.print(cond);
    Serial.print("  ");
  }
} 

void print4digit(int vale) {
  if (vale < 10) {
    Serial.print("   ");
    Serial.print(vale);
  } else if (vale < 100) {
    Serial.print("  ");
    Serial.print(vale);
  } else if (vale < 1000) {
    Serial.print(" ");
    Serial.print(vale);
  } else if (vale < 10000) {
    Serial.print(vale);
  }
}