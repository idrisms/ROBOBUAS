//ubah pendawaian sekiranya robot tak bergerak seperti di dalam code
//nak ubah code pon boleh, terpulang kepada citarasa dan kemahiran koding
void cubagerakan() {
  moveForward(100);//kedepan
  delay(1000);
  Henti();
  delay(1000);
  moveBackward(100);//kebelakang
  delay(1000);
  Henti();
  delay(1000);
  turnRight(100);//pusing kanan
  delay(1000);
  Henti();
  delay(1000);
  turnLeft(100);// pusing kiri
  delay(1000);
  Henti();
  delay(3000);
}
