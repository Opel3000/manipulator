void PointG() {

  int (*a)[4];
  //Serial.print("sizeof(MasOfAction) = ");
  //Serial.println(sizeof(MasOfAction));
  //Serial.print("sizeof(MasOfAction[0]) = ");
  //Serial.println(sizeof(MasOfAction[0]));
  arrHeight[0][0] = 72;
  arrHeight[0][1] = 72;
  arrHeight[0][2] = 72;
  arrHeight[0][3] = 72;
  arrHeight[0][4] = 72;
  int i = 0;
  while (finalMas[i][0] != 99) {

    if (finalMas[i][1] == 0)
      MoveToStart(finalMas[i][2], finalMas[i][3], finalMas[i][0]);
    else
      MoveToFinish(finalMas[i][2], finalMas[i][3], finalMas[i][0]);

    MasOfActionSteps[IndexOfActionSteps][4] = (finalMas[i][0]) ? ServZ[1] : ServZ[2];

    int indexArrHeight = finalMas[i][2];
    if (finalMas[i][1])
      indexArrHeight += 5;
    arrHeight[IndexOfActionSteps][indexArrHeight] -= (finalMas[i][0]) ? CUBE_HEIGHT_B : CUBE_HEIGHT_S;
    for (int a = 0; a < 8; a++)
      arrHeight[IndexOfActionSteps + 1][a] = arrHeight[IndexOfActionSteps][a];

    IndexOfActionSteps++;

    if (finalMas[i][4] == 0)
      MoveToStart(finalMas[i][5], finalMas[i][6], finalMas[i][0]);
    else
      MoveToFinish(finalMas[i][5], finalMas[i][6], finalMas[i][0]);

    MasOfActionSteps[IndexOfActionSteps][4] = ServZ[0];


    indexArrHeight = finalMas[i][5];
    if (finalMas[i][4])
      indexArrHeight += 5;
    arrHeight[IndexOfActionSteps][indexArrHeight] += (finalMas[i][0]) ? CUBE_HEIGHT_B : CUBE_HEIGHT_S;
    for (int a = 0; a < 8; a++)
      arrHeight[IndexOfActionSteps + 1][a] = arrHeight[IndexOfActionSteps][a];

    IndexOfActionSteps++;
    i++;
  }
  MasOfActionSteps[IndexOfActionSteps][0] = -1;
  ///////               (type, PlaceIn, xIn, yIn, PlaceOut, xOut, yOut)

  IndexOfActionSteps = 0;
  i = 0;
  while (finalMas[i][0] != 99) {
    Serial.print("Index = ");
    Serial.println(IndexOfActionSteps);
    Serial.println(finalMas[i][2] + ((finalMas[i][1]) ? 5 : 0));
    Serial.println(finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0));
    Serial.println();
    int maxH = 0;
    if (finalMas[i][2] + ((finalMas[i][1]) ? 5 : 0) < finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0)) {
      for (int j = finalMas[i][2] + ((finalMas[i][1]) ? 5 : 0); j < finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0); j ++) {
        Serial.print(j);
        if (arrHeight[i][j] > maxH) {
          maxH = arrHeight[i][j];
          Serial.print(" ");
          Serial.println(maxH);
        }
      }
    }
    else {
      for (int j = finalMas[i][2] + ((finalMas[i][1]) ? 5 : 0); j > finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0); j--) {
        Serial.print(j);
        if (arrHeight[i][j] > maxH) {
          maxH = arrHeight[i][j];
          Serial.print(" ");
          Serial.println(maxH);
        }
      }
    }
    arrStepsHeight[IndexOfActionSteps] = maxH;
    IndexOfActionSteps++;

    maxH = 0;
    if (finalMas[i + 1][0] != 99) {
      Serial.print("Index = ");
      Serial.println(IndexOfActionSteps);
      Serial.println(finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0));
      Serial.println(finalMas[i + 1][2] + ((finalMas[i + 1][1]) ? 5 : 0));
      Serial.println();
      if (finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0) < finalMas[i + 1][2] + ((finalMas[i + 1][1]) ? 5 : 0)) {
        for (int j = finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0); j < finalMas[i + 1][2] + ((finalMas[i + 1][1]) ? 5 : 0); j ++) {
          Serial.print(j);
          if (arrHeight[i][j] > maxH) {
            maxH = arrHeight[i][j];
            Serial.print(" ");
            Serial.println(maxH);
          }
        }
      }
      else {
        for (int j = finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0); j > finalMas[i + 1][2] + ((finalMas[i + 1][1]) ? 5 : 0); j--) {
          Serial.print(j);
          if (arrHeight[i][j] > maxH) {
            maxH = arrHeight[i][j];
            Serial.print(" ");
            Serial.println(maxH);
          }
        }
      }
    }
    arrStepsHeight[IndexOfActionSteps] = maxH;
    IndexOfActionSteps++;
    i++;
  }
}
