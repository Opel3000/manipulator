void skLadno(int oneStepA, int oneSpetZ, int twoStepA, int twoStepZ) {

  //maxSpeedA
  //maxSpeedZ

  //speedA
  //speedZ

  int raznostA = twoStepA - oneStepA;
  int raznostZ = twoStepZ - oneStepZ;
  int maxSpeedAZ = 0;

  if (raznostA < 0)
    raznostA *= -1;
  if (raznostZ < 0)
    raznostZ *= -1;

  if (raznostA >= raznostZ) {
    float timeAZ = raznostA/maxSpeedA;
    speedZ = raznostZ/timeAZ;
    speedA = maxSpeedA;
  }
  else {
    float timeAZ = raznostZ/ = maxSpeedZ;
    speedA = raznostA/timeAZ;
    speedZ = maxSpeedZ;
  }
}
