#define CUBE_HIGHT_B 41
#define CUBE_HIGHT_S 31

byte count = 0;
byte finalMas[20][7];/* = {
  //                          {1, 0, 2, 41, 1, 0, 41},
                          {0, 0, 0, 72, 1, 2, 31},
                          {0, 0, 2, 72, 1, 2, 62},
                          {0, 0, 3, 72, 1, 2, 93},
                          {1, 0, 2, 41, 1, 0, 41},
                          {0, 0, 1, 72, 0, 2, 31},
                          {0, 0, 4, 72, 0, 2, 62},
                          {1, 0, 3, 41, 1, 0, 82},
                          {1, 0, 1, 41, 1, 1, 41},
                          {1, 0, 0, 41, 1, 1, 82},
                          {0, 1, 2, 93, 1, 0, 113},
                          {0, 1, 2, 62, 1, 1, 113},
                          {0, 1, 2, 31, 1, 1, 144},
                          {1, 0, 4, 41, 1, 2, 41},
                          {0, 1, 1, 144, 1, 2, 72},
                          {99, 0, 0, 0, 0, 0, 0}
  };*/
byte countStep = 0;
byte spiersMas[4];

byte kubStrong = 0;

byte inputMas[7][2] = {{1, 9}, {3, 11}, {2, 8}, {4, 7}, {5, 10}, {6, 0}, {12, 0}};
byte outputMas[3][3] = {{5, 4, 3}, {2, 1, 3}, {4, 2, 0}};
byte outputMasMy[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};



void setup() {
  Serial.begin(9600);
  lord_of_the_builders_two_arrays();
  Serial.println("/////////////////");
  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < 7; j++) {
      Serial.print(int(finalMas[i][j]));
      Serial.print("  ");
    }
    Serial.println();
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}




void search_spiers() {
  byte countSpiers = 100;
  byte countSpiersNice = 0;
  byte indexDom = 0;
  for (byte i = 0; i <= 1; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (outputMas[i + 1][j] == 0 and outputMas[i][j] != 0) {
        if (countSpiers > i) {
          countSpiers = i;
        }
        outputMas[i][j] = outputMas[i][j] + 6;
      }
      else if (i == 1 and outputMas[i + 1][j] != 0) {
        //Serial.println(outputMas[i + 1][j]);
        outputMas[i + 1][j] = outputMas[i + 1][j] + 6;
      }
    }
  }
  for (byte i = 0; i <= 2; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (outputMas[i][j] > 6) {
        if (countSpiersNice == 0) spiersMas[3] = j;
        spiersMas[countSpiersNice] = outputMas[i][j];
        countSpiersNice++;
      }
    }
  }
}


void array_generator(byte type, byte plaseA, byte numInPlaseA, byte HightInPlaseA, byte plaseB, byte numInPlaseB, byte HightInPlaseB) {

  finalMas[countStep][0] = type;
  finalMas[countStep][1] = plaseA;
  finalMas[countStep][2] = numInPlaseA;
  finalMas[countStep][3] = HightInPlaseA;
  finalMas[countStep][4] = plaseB;
  finalMas[countStep][5] = numInPlaseB;
  finalMas[countStep][6] = HightInPlaseB;
  countStep++;

  byte floorOne = MMInFloor(HightInPlaseA);
  byte floorTwo = MMInFloor(HightInPlaseB);

  if (plaseB == 0) {
    inputMas[numInPlaseB][floorTwo] = inputMas[numInPlaseA][floorOne];
    inputMas[numInPlaseA][floorOne] = 0;
  }
  else {
    outputMasMy[floorTwo][numInPlaseB] = inputMas[numInPlaseA][floorOne];
    inputMas[numInPlaseA][floorOne] = 0;
  }


}


byte MMInFloor(byte hight) {
  if (hight < 31)
    return 0;
  else if (hight < 62)
    return 1;
  else
    return 2;
}


void build_matrix(int oneF, int twoF, int threeF) {
  outputMas[0][0] = byte(oneF / 100);
  outputMas[0][1] = byte(oneF / 10 % 10);
  outputMas[0][2] = byte(oneF % 10);
  outputMas[1][0] = byte(twoF / 100);
  outputMas[1][1] = byte(twoF / 10 % 10);
  outputMas[1][2] = byte(twoF % 10);
  outputMas[2][0] = byte(threeF / 100);
  outputMas[2][1] = byte(threeF / 10 % 10);
  outputMas[2][2] = byte(threeF % 10);
  //PlatfornIO IDE
}


byte lord_of_the_builders_brotherhood_array() {
  Serial.print("g");
  while (true) {
    if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');
      long longdata = data.toInt();
      //      Serial.println(longdata);
      if (longdata > 100) {
        int floorOne = int(longdata / 1000000);
        int floorTwo = int(longdata / 1000 % 1000);
        int floorThree = int(longdata % 1000);
        build_matrix(floorOne, floorTwo, floorThree);
        break;
      }
      else {
        byte typeE = byte(longdata % 10);
        byte typeW = byte((longdata - longdata % 10) / 10);
        count++;
        switch (count) {
          case 1:
            if (longdata <= 6 and longdata != 0)
              inputMas[5][0] = longdata;
            else
              inputMas[5][1] = longdata + 5;
            break;
          case 2:
            inputMas[0][0] = typeE;
            inputMas[0][1] = typeW + 5;
            break;
          case 3:
            inputMas[1][0] = typeE;
            inputMas[1][1] = typeW + 5;
            break;
          case 4:
            inputMas[2][0] = typeE;
            inputMas[2][1] = typeW + 5;
            break;
          case 5:
            inputMas[3][0] = typeE;
            inputMas[3][1] = typeW + 5;
            break;
          case 6:
            inputMas[4][0] = typeE;
            inputMas[4][1] = 0;
            break;
          case 7:
            if (longdata <= 6 and longdata != 0)
              inputMas[6][0] = longdata;
            else
              inputMas[6][1] = longdata + 5;
            break;
        }
        break;
      }
    }
  }
}


void lord_of_the_builders_two_arrays() {

  search_spiers();
  loxSpier();
  while (!workQuestion()) {
    if (!funcWork())
      nextStep;




    Serial.println("/////////////////");
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 2; j++) {
        Serial.print(int(inputMas[i][j]));
        Serial.print("  ");
      }
      Serial.println();
    }

    Serial.println("/////////////////");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        Serial.print(int(outputMasMy[i][j]));
        Serial.print("  ");
      }
      Serial.println();
    }
    Serial.println("/////////////////");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        Serial.print(int(outputMas[i][j]));
        Serial.print("  ");
      }
      Serial.println();
    }
  }


}



bool checkKub(byte indexMain) {
  if (inputMas[indexMain][1] != 0)
    return false;
  else
    return true;
}


void loxSpier() {
  for (byte i = 0; i <= 6; i++) {
    if (inputMas[i][1] != spiersMas[0] and inputMas[i][1] != spiersMas[1] and inputMas[i][1] != spiersMas[2] and inputMas[i][1] != 0) {
      for (byte j = 0; j <= 2; j++) {
        if (outputMas[0][j] == inputMas[i][0]) {
          array_generator(0, 0, i, 0, 1, j, 0);
          break;
        }
        else {
          array_generator(3, 0, i, CUBE_HIGHT_B, 0, 0, 0);
          inputMas[i][1] = 0;
          break;
        }
      }
    }
  }
}


bool workQuestion() {
  for (byte i = 0; i <= 2; i++)
    for (byte j = 0; j <= 2; j++)
      if (outputMas[i][j] != outputMasMy[i][j])
        return false;
  return true;
}


bool nalichie(byte numKub) {
  for (byte i = 0; i <= 2; i++)
    for (byte j = 0; j <= 2; j++)
      if (outputMas[i][j] == numKub)
        return true;
  return false;
}


bool funcWork() {

  for (byte i = 0; i <= 2; i++) {
    for (byte j = 0; j <= 6; j++) {
      if (outputMas[0][i] != outputMasMy[0][i]) {
        if (!checkKub(j))
          kubStrong = j;
        if (outputMas[0][i] == inputMas[j][0] and checkKub(j)) {
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
        if (!checkKub(j))
          kubStrong = j;
        if (outputMas[1][i] == inputMas[j][0] and checkKub(j)) {
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
        if (!checkKub(j))
          kubStrong = j;
        if (checkKub(j))
          array_generator(1, 0, j, 0, 1, i, CUBE_HIGHT_B * 2);
        return true;
      }
      else if (outputMas[2][i] == inputMas[j][1]) {
        if (!checkKub(j))
          kubStrong = j;
        array_generator(1, 0, j, 0, 1, i, CUBE_HIGHT_B * 2);
        return true;
      }
    }
  }
  Serial.println("false");
  Serial.println(kubStrong);
  return false;
}


void nextStep() {
  for (byte i = 0; i <= 6; i++) {
    if (inputMas[i][1] == 0) {
      array_generator(1, 0, kubStrong, CUBE_HIGHT_B, 0, i, CUBE_HIGHT_B);
      Serial.println(kubStrong);
    }
  }
  for (byte i = 0; i <= 6; i++) {
    if (inputMas[i][0] == 0) {
      array_generator(1, 0, kubStrong, CUBE_HIGHT_B, 0, i, 0);
      Serial.println(kubStrong);
    }
  }
}
