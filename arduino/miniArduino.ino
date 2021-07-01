byte finalMas[12];
byte countStep = 0;



void setup() {
  Serial.begin(9600);
  delay(1000);
  raspberry();
  raspberry();

  /*for (byte i = 0; i < 12; i++) {
    Serial.print(finalMas[i]);
    Serial.print(" ");*/
  }

}

void loop() {

}


void raspberry() {
  Serial.println("Go");
  while (true) {
    if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');
      Serial.println(data);
      long longdata = data.toInt();
      if (countStep == 0) {
        finalMas[0]  = byte(longdata / 100000);
        finalMas[1]  = byte(longdata / 10000 % 10);
        finalMas[2]  = byte(longdata / 1000 % 10);
        finalMas[3]  = byte(longdata / 100 % 10);
        finalMas[4]  = byte(longdata / 10 % 10);
        finalMas[5]  = byte(longdata % 10);
        countStep++;
        break;
      }
      else if (countStep == 1) {
        finalMas[6]  = byte(longdata / 100000);
        finalMas[7]  = byte(longdata / 10000 % 10);
        finalMas[8]  = byte(longdata / 1000 % 10);
        finalMas[9]  = byte(longdata / 100 % 10);
        finalMas[10]  = byte(longdata / 10 % 10);
        finalMas[11]  = byte(longdata % 10);
        countStep++;
        break;
      }
    }
  }
}
