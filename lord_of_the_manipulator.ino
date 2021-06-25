byte lord_of_the_builders_two_arrays(byte inputMas[5][2], byte outputMas[3][3]) {

  //byte spiersMas[4];
  byte nullSpiersMas[3];
  byte countInOneFloor = 0;
  bool flagNothing = false;

  byte failSimulation[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};



  for (byte i = 0; i <= 2; i++) {
    if (outputMas[0][i] > 5) countInOneFloor++;
    }

  if (countInOneFloor != 2 and countInOneFloor != 3) {


    //первые 3 шпиля в столб с наименьшей итоговой высотой

    search_spiers(outputMas);

    for (byte i = 0; i <= 3; i++) {
      for (byte j = 0; j <= 4; j++) {
        if (spiersMas[i] == inputMas[j][1] and inputMas[j][1] != 0) {
          failSimulation[countStep][spiersMas[3]] = spiersMas[i];
          array_generator(1, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], countStep * CUBE_HIGHT_S);
          //finalMas[countStep] = {1, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], countStep * CUBE_HIGHT_S};
          inputMas[j][1] = 0;
          nullSpiersMas[countStep] = j;
        }
      }
    }

    //находим этажи на первом слое и ищем их индексы в зоне материалов

    byte oneFloorMas[2] = {11, 11};
    byte countOneFloor = 0;
    for (byte i = 0; i <= 2; i++) {
      if (outputMas[0][i] < 6 and outputMas[0][i] != 0 and i != spiersMas[3]) {
        oneFloorMas[countOneFloor] = outputMas[0][i];
        //Serial.print("oneFloorMas[countOneFloor] - ");
        //Serial.println(oneFloorMas[countOneFloor]);
        countOneFloor++;
      }
    }

    /*Serial.print(oneFloorMas[0]);
      Serial.println(oneFloorMas[1]);*/

    byte stopMas[2] = {0, 0};

    for (byte i = 0; i <= 4; i++) {
      for (byte j = 0; j <= 1; j++) {
        if (inputMas[i][0] == oneFloorMas[j]) {
          stopMas[j] = i;
        }
      }
    }


    byte stupidSpiers[2];
    byte countSS = 0;


    for (byte i = 0; i <= 4; i++) {
      if (inputMas[i][1] != 0) {
        stupidSpiers[countSS] = i;
        countSS++;
      }
    }

    //проверка 1 слоя на этажи и перенос их

    byte plaseTwoSS;
    bool flagErrorVar2 = false;
    bool flagFix = false

    for (byte j = 0; j <= 2; j++) {
      if (outputMas[0][j] < 6 and outputMas[0][j] != 0 and j != spiersMas[3]) {
        for (byte i = 0; i <= 4; i++) {
          flagErrorVar2 = false;
          if (outputMas[0][j] == inputMas[i][0] and inputMas[i][1] == 0) {
            array_generator(0, 0, i, 0, 1, j, 0);
            if (flagFix == false){
                array_generator(1, 0, stupidSpiers[0], CUBE_HIGHT_B, 0, i, 0);
                array_generator(1, 0, stupidSpiers[1], CUBE_HIGHT_B, 0, i, CUBE_HIGHT_S);
                flagFix = true
            }
            failSimulation[0][j] = inputMas[i][0];
            inputMas[i][0] = 0;
          }
          else if (outputMas[0][j] == inputMas[i][0] and inputMas[i][1] != 0) {
            for (byte b = 0; b <= 4; b++) { // перетаскивание шпилей
              for (byte k = 0; k <= 1; k++) {
                if (stopMas[k] != b and inputMas[b][0] != 0 and inputMas[b][1] == 0) {
                  array_generator(1, 0, i, CUBE_HIGHT_B, 0, b, CUBE_HIGHT_B);
                  inputMas[b][1] = inputMas[i][1];
                  inputMas[i][1] = 0;
                  flagErrorVar2 = true;
                  break;
                }
                if (flagErrorVar2) break;
              }
              if (flagErrorVar2) break;
            }

            array_generator(0, 0, i, 0, 1, j, 0); //перенос этажей
            if (flagFix == false){
                array_generator(1, 0, stupidSpiers[0], CUBE_HIGHT_B, 0, i, 0);
                array_generator(1, 0, stupidSpiers[1], CUBE_HIGHT_B, 0, i, CUBE_HIGHT_S);
                flagFix = true
            }
            failSimulation[0][j] = inputMas[i][0];
            plaseTwoSS = i;
            inputMas[i][0] = 0;
          }
        }
      }
    }

    if (flagNothing) {

      byte oneFloorMas[2] = {11, 11};
      byte countOneFloor = 0;

      for (byte i = 0; i <= 2; i++) {
        if (outputMas[1][i] < 6 and outputMas[1][i] != 0 and i != spiersMas[3]) {
          oneFloorMas[countOneFloor] = outputMas[1][i];
          countOneFloor++;
        }
      }

      byte stopMas[2] = {0, 0};

      for (byte i = 0; i <= 4; i++) {
        for (byte j = 0; j <= 1; j++) {
          if (inputMas[i][1] == oneFloorMas[j]) {
            stopMas[j] = i;
          }
        }
      }


      bool flagErrorVar2 = false;

      for (byte j = 0; j <= 2; j++) {
        if (outputMas[1][j] < 6 and outputMas[1][j] != 0 and j != spiersMas[3]) {
          for (byte i = 0; i <= 4; i++) {
            flagErrorVar2 = false;
            if (outputMas[1][j] == inputMas[i][0] and inputMas[i][1] == 0) {
              array_generator(0, 0, i, 0, 1, j, CUBE_HIGHT_B);
              plaseTwoSS = i;
              failSimulation[0][j] = inputMas[i][0];
              inputMas[i][0] = 0;
            }
            else if (outputMas[0][j] == inputMas[i][0] and inputMas[i][1] != 0) {
              for (byte b = 0; b <= 4; b++) { // перетаскивание шпилей
                for (byte k = 0; k <= 1; k++) {
                  if (stopMas[k] != b and inputMas[b][0] != 0 and inputMas[b][1] == 0) {
                    array_generator(1, 0, i, CUBE_HIGHT_B, 0, b, CUBE_HIGHT_B);
                    inputMas[b][1] = inputMas[i][1];
                    inputMas[i][1] = 0;
                    flagErrorVar2 = true;
                    break;
                  }
                  if (flagErrorVar2) break;
                }
                if (flagErrorVar2) break;
              }

              array_generator(0, 0, i, 0, 1, j, CUBE_HIGHT_B); //перенос этажей
              failSimulation[1][j] = inputMas[i][0];
              plaseTwoSS = i;
              inputMas[i][0] = 0;
            }
          }
        }
      }
    }

    bool flag = false;

    for (byte i = 0; i <= 2; i++) {
      if (outputMas[2][i] > 0) flag = true;
    }

    if (flag) {

      byte stupidSpiersNum2[2][2] = {{11, 11}, {11, 11}};
      countSS = 0;
      byte flagSS = 0;

      for (byte i = 0; i <= 2; i++) {
        if (outputMas[1][i] < 6 and outputMas[1][i] > 0) {
          stupidSpiersNum2[countSS][0] = outputMas[1][i];
          stupidSpiersNum2[countSS][1] = i;
          countSS++;
          if (stupidSpiersNum2[countSS] != 11) flagSS++;
        }
      }

      if (flagSS == 2) {
        for (byte i = 0; i <= 4; i++) {
          for (byte j = 0; j <= 1; j++) {
            if (stupidSpiersNum2[j][0] == inputMas[i][0]) {
              array_generator(0, 0, i, 0, 1, stupidSpiersNum2[j][1], CUBE_HIGHT_B);
              inputMas[i][0] = 0;
            }
          }
        }

        byte countSOS = 0;
        byte saveAss;

        for (int i = 2; i >= 0; i--) {
          for (byte j = 0; j <= 2; j++) {
            if (failSimulation[i][spiersMas[3]] == outputMas[2][j] and j != spiersMas[3]) {
              if (countSOS == 0) {
                array_generator(1, 1, spiersMas[3], 2 * CUBE_HIGHT_S, 1, j, 2 * CUBE_HIGHT_B);
                countSOS++;
                break;
              }
              else if (countSOS == 1) {
                array_generator(1, 1, spiersMas[3], CUBE_HIGHT_S, 1, j, 2 * CUBE_HIGHT_B);
                saveAss = j;
                countSOS++;
              }
            }
          }
        }


        bool errorFlag = true;

        for (byte i = 0; i <= 2; i++) {
          if (outputMas[i][spiersMas[3]] < 6 and outputMas[i][spiersMas[3]] != 0) {
            errorFlag = false;
          }
        }

        if (errorFlag) {
          return 1;
        }
        else {
          array_generator(1, 1, spiersMas[3], 0, 1, saveAss, 2 * CUBE_HIGHT_B + CUBE_HIGHT_S);

          for (byte i = 0; i <= 4; i++) {
            if (inputMas[i][0] > 0 and inputMas[i][0] < 6) {
              array_generator(0, 0, i, 0, 1, spiersMas[3], 0);
            }

          }
          array_generator(1, 1, saveAss, 2 * CUBE_HIGHT_B + CUBE_HIGHT_S, 1, spiersMas[3], CUBE_HIGHT_B);

          return 1;
        }
      }

      else if (flagSS == 1) {
        for (byte i = 0; i <= 4; i++) {
          for (byte j = 0; j <= 1; j++) {
            if (stupidSpiersNum2[j][0] == inputMas[i][0]) {
              array_generator(0, 0, i, 0, 1, stupidSpiersNum2[j][1], CUBE_HIGHT_B);

            }
          }
        }

        byte countSpirtsTwoFloor = 0;

        for (byte i = 0; i <= 2; i++)
          if (outputMas[1][i] > 5 and outputMas[1][i] <= 10) countSpirtsTwoFloor++;

        if (countSpirtsTwoFloor == 2) {
          byte spirtsMas[2][2];
          for (byte i = 0; i <= 2; i++) {
            if (outputMas[2][i] > 5 and outputMas[2][i] <= 10 and i != spiersMas[3]) {
              spirtsMas[0][0] = i;
              spirtsMas[0][1] = outputMas[2][i];
            }
          }
          for (byte i = 0; i <= 2; i++) {
            if (outputMas[1][i] > 5 and outputMas[1][i] <= 10 and i != spiersMas[3]) {
              spirtsMas[1][0] = i;
              spirtsMas[1][1] = outputMas[1][i];
            }
          }

          byte countDa = 0;
          byte saveAssVar2;

          for (byte i = 0; i <= 2; i++) {
            for (byte j = 0; j <= 2; j++) {
              for (int k = 0; k <= 1; k++) {
                if (outputMas[i][j] == spirtsMas[k][1]) {
                  if (countDa == 0) {
                    array_generator(1, 1, j, 2 * CUBE_HIGHT_S, 1, spirtsMas[k][0], CUBE_HIGHT_B);
                    countDa++;
                    saveAssVar2 = spirtsMas[k][0];
                  }
                  else if (countDa == 1) {
                    array_generator(1, 1, j, CUBE_HIGHT_S, 1, spirtsMas[k][0], 2 * CUBE_HIGHT_B);
                    countDa++;
                  }
                }
              }
            }
          }
          array_generator(1, 1, spiersMas[3], 0, 1, saveAssVar2, CUBE_HIGHT_B + CUBE_HIGHT_S);
          for (byte i = 0; i <= 4; i++) {
            if (inputMas[i][0] == outputMas[0][spiersMas[3]])
              array_generator(0, 0, i, 0, 1, spiersMas[3], 0);
          }
          array_generator(1, 1, saveAssVar2, CUBE_HIGHT_B + CUBE_HIGHT_S, 1, spiersMas[3], CUBE_HIGHT_B);
          return 1;
        }

        else if (countSpirtsTwoFloor == 1) {
          byte spirtsMas[2][2];
          for (byte i = 0; i <= 2; i++) {
            if (outputMas[2][i] > 5 and outputMas[2][i] <= 10 and i != spiersMas[3]) {
              spirtsMas[0][0] = i;
              spirtsMas[0][1] = outputMas[2][i];
            }
          }
          for (byte i = 0; i <= 2; i++) {
            if (outputMas[1][i] > 5 and outputMas[1][i] <= 10 and i != spiersMas[3]) {
              spirtsMas[1][0] = i;
              spirtsMas[1][1] = outputMas[1][i];
            }
          }

          byte countDa = 0;
          byte saveAssVar2;

          for (byte i = 0; i <= 2; i++) {
            for (byte j = 0; j <= 2; j++) {
              for (byte k = 0; k <= 1; k++) {
                if (outputMas[i][j] == spirtsMas[k][1]) {
                  if (countDa == 0) {
                    array_generator(1, 1, j, 2 * CUBE_HIGHT_S, 1, spirtsMas[k][0], CUBE_HIGHT_B);
                    countDa++;
                    saveAssVar2 = spirtsMas[k][0];
                  }
                  else
                    array_generator(1, 1, j, CUBE_HIGHT_S, 1, spirtsMas[k][0], 2 * CUBE_HIGHT_B);
                }
              }
            }
          }
        }
      }
      else {
        /////////////////////////////////////////////////////////////////////////////////////
        byte errorVar = 0;
        for (int i = 2; i >= 0; i--) {
          for (byte j = 0; j <= 2; j++) {
            if (failSimulation[i][spiersMas[3]] == outputMas[1][j] and j != spiersMas[3]) {
              if (errorVar == 0) {
                array_generator(1, 1, spiersMas[3], 2 * CUBE_HIGHT_S, 1, j, CUBE_HIGHT_B);
                errorVar++;
              }
              else if (errorVar == 1) {
                array_generator(1, 1, spiersMas[3], CUBE_HIGHT_S, 1, j, CUBE_HIGHT_B);
                errorVar++;;
              }
            }
          }
        }
      }
    }
    else {
      byte errorVar = 0;
      byte saveAssVar3;
      for (int i = 2; i >= 0; i--) {
        for (byte j = 0; j <= 2; j++) {
          if (failSimulation[i][spiersMas[3]] == outputMas[1][j]) {
            if (errorVar == 0) {
              array_generator(1, 1, spiersMas[3], 2 * CUBE_HIGHT_S, 1, j, CUBE_HIGHT_B);
              failSimulation[i][spiersMas[3]] = 0;
              errorVar++;
              saveAssVar3 = j;
            }
            else if (errorVar == 1) {
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_S, 1, j, CUBE_HIGHT_B);
              failSimulation[i][spiersMas[3]] = 0;
              errorVar++;
            }
          }
        }
      }

      bool flagUh = false;

      for (byte i = 0; i <= 2; i++) {
        if (outputMas[i][spiersMas[3]] > 0 and outputMas[i][spiersMas[3]] < 6) flagUh = true;
      }
      if (flagUh) {
        array_generator(1, 1, spiersMas[3], 0, 1, saveAssVar3, CUBE_HIGHT_S + CUBE_HIGHT_B);
        for (byte i = 0; i <= 4; i++) {
          if (outputMas[0][spiersMas[3]] == inputMas[i][0])
            array_generator(0, 0, i, 0, 1, spiersMas[3], 0);
        }
        array_generator(1, 1, saveAssVar3, CUBE_HIGHT_S + CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B);
      }
      else {
        return 1;
      }
    }
  }
  else {
    if (countInOneFloor == 3) {
      for (byte i = 0; i <= 2; i++) {
        for (byte j = 0; j <= 4; j++) {
          if (outputMas[0][i] == inputMas[j][1]) {
            array_generator(1, 0, j, CUBE_HIGHT_B, 1, i, 0);
          }
        }
      }
    }
    else {
      byte errorIndex;
      for (byte i = 0; i <= 2; i++) {
        if (outputMas[0][i] < 6 and outputMas[0][i] > 0) errorIndex = i;
      }
      byte countFloorMeh = 0;
      byte oneKub[2] = {0 , 0};
      byte twoKub[2] = {0, 0};
      for (byte i = 0; i <= 2; i++) {
        if (outputMas[i][errorIndex] < 6 and outputMas[i][errorIndex] > 0) {
          oneKub[countFloorMeh] = outputMas[i][errorIndex];
          countFloorMeh++;
        }
      }

      for (byte i = 0; i <= 2; i++) {
        for (byte j = 0; j <= 4; j++) {
          if (outputMas[0][i] == inputMas[j][1]) {
            array_generator(1, 0, j, CUBE_HIGHT_B, 1, i, 0);
            inputMas[j][1] = 0;
          }
        }
      }

      for (byte i = 0; i <= 4; i++) {
        for (byte j = 0; j <= 1; j++) {
          if (inputMas[i][0] == oneKub[j] and inputMas[i][1] != 0 ) {
            for (byte k = 0; k <= 4 ; k++) {
              if (inputMas[k][1] == 0 and k != i and inputMas[k][0] != oneKub[0] and inputMas[k][0] != oneKub[1]) {
                array_generator(1, 0, i, CUBE_HIGHT_B, 0, k, CUBE_HIGHT_B);
                inputMas[k][1] = inputMas[i][1];
                inputMas[i][1] = 0;
                break;
              }
            }
          }
        }
      }

      byte countMinusVara = 0;

      for (byte i = 0; i <= 4; i++) {
        for (byte j = 0; j <= 1; j++) {
          if (inputMas[i][0] == oneKub[j]) {
            if (countMinusVara == 0) {
              array_generator(0, 0, i, 0, 1, errorIndex, 0);
              countMinusVara++;
            }
            else if (countMinusVara == 1 and oneKub[1] != 0) {
              array_generator(0, 0, i, 0, 1, errorIndex, CUBE_HIGHT_B);
              countMinusVara++;
            }
          }
        }
      }
      for (byte i = 0; i <= 2; i++) {
        for (byte j = 0; j <= 4; j++) {
          if (countMinusVara == 1) {
            if (outputMas[1][i] == inputMas[j][1] and outputMas[1][i] != 0) {
              array_generator(1, 0, j, CUBE_HIGHT_B, 1, i, CUBE_HIGHT_B);
            }
          }
          else {
            if (outputMas[2][i] == inputMas[j][1]  and outputMas[1][i] != 0) {
              array_generator(1, 0, j, CUBE_HIGHT_B, 1, i, 2 * CUBE_HIGHT_B);
            }
          }
        }
      }
    }
  }
  return 1;
}
