//ubah pendawaian sekiranya robot tak bergerak seperti di dalam code
//nak ubah code pon boleh, terpulang kepada citarasa dan kemahiran koding
void cubagerakan() {
  moveForward(pwmVal);//kedepan
  delay(1000);
  Henti();
  delay(1000);
  moveBackward(pwmRev);//kebelakang
  delay(1000);
  Henti();
  delay(1000);
  turnRight(pwmTurn);//pusing kanan
  delay(1000);
  Henti();
  delay(1000);
  turnLeft(pwmTurn);// pusing kiri
  delay(1000);
  Henti();
  delay(3000);
}