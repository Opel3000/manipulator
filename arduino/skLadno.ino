int maxSpeedA = 1500;
int maxSpeedZ = 1200;

int speedA = 0;
int speedZ = 0;


void setup() {
  Serial.begin(9600);
  skLadno(-400, 0, 635, 800);
  Serial.println(speedA);
  Serial.println(speedZ);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void skLadno(int oneStepA, int oneStepZ, int twoStepA, int twoStepZ) {

  //int maxSpeedA = 1500;
  //int maxSpeedZ = 1200;

  //int speedA = 0;
  //int speedZ = 0;


  int raznostA = twoStepA - oneStepA;
  int raznostZ = twoStepZ - oneStepZ;
  int maxSpeedAZ = 0;


  if (raznostA < 0)
    raznostA *= -1;
  if (raznostZ < 0)
    raznostZ *= -1;

  if (raznostA > raznostZ) {
    speedZ = raznostZ / (float(raznostA) / maxSpeedA);
    speedA = maxSpeedA;
    if (speedZ > maxSpeedZ) {
      speedA = maxSpeedZ;
      speedZ = maxSpeedZ;
    }
  }
  else if (raznostA == raznostZ) {
    speedA = maxSpeedZ;
    speedZ = maxSpeedZ;
  }
  else {
    speedA = raznostA / (float(raznostZ) / maxSpeedZ);
    speedZ = maxSpeedZ;
  }
}
