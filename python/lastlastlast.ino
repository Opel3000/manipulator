bool generalСollection() {

  for (byte i = 0; i <= 2; i++) {
    for (byte j = 0; j <= 6; j++) {
      if (outputMas[0][i] != outputMasMy[0][i]) {
        kubStrong = j;
        if (outputMas[0][i] == inputMas[j][0] and check(j)) {
          if (outputMas[0][i] > 6)
            array_generator(1, 0, j, 0, 1, i, 0);
          else
            array_generator(0, 0, j, 0, 1, i, 0);
          return true;
        }
        else if (outputMas[0][i] == inputMas[j][1]) {
          array_generator(1, 0, j, 0, 1, i, 0);
          return true;
        }
      }
      else if (outputMas[1][i] != outputMasMy[1][i]) {
        kubStrong = j;
        if (outputMas[1][i] == inputMas[j][0] and check(j)) {
          if (outputMas[1][i] > 6)
            array_generator(1, 0, j, 0, 1, i, CUBE_HIGHT_B);
          else
            array_generator(0, 0, j, 0, 1, i, CUBE_HIGHT_B);
          return true;
        }
        else if (outputMas[0][i] == inputMas[j][1]) {
          array_generator(1, 0, j, 0, 1, i, CUBE_HIGHT_B);
          return true;
        }
      }
      else if (outputMas[2][i] == inputMas[j][0]) {
        kubStrong = j; //noooooooooooooooooooooooooooooooooooooope dont check
         if(check(j))
          array_generator(1, 0, j, 0, 1, i, CUBE_HIGHT_B * 2);
        return true;
      }
      else if (outputMas[2][i] == inputMas[j][1]) {
        kubStrong = j;
        array_generator(1, 0, j, 0, 1, i, CUBE_HIGHT_B * 2);
        return true;
      }

    }
  }
  return false;
}




void nextStep() {
  for (byte i = 0; i <= 6; i++) {
    if (inputMas[i][1] == 0)
      array_generator(1, 0, kubStrong, 0, 0, i, CUBE_HIGHT_B);
  }
}
