#define CUBE_HIGHT_B 41
#define CUBE_HIGHT_S 31

#define I_TO_3 byte i = 0; i <= 2; i++
#define J_TO_3 byte j = 0; j <= 2; j++

#define FOR_I_5 byte i = 0; i <= 4; i++
#define FOR_J_5 byte j = 0; j <= 4; j++


byte count = 0;
byte massPlanKyd[5][2];
byte finalMas[20][7];
byte countStep = 0;
byte spiersMas[4];

//1 - красный
//2 - синий
//3 - жёлтый
//4 - зелёный
//5 - black

byte inputMas[5][2] = {{3, 6}, {4, 9}, {5, 8}, {1, 10}, {2, 7}};
byte outputMas[3][3] = {{2, 4, 3}, {6, 1, 5}, {0, 7, 8}};

void setup() {
  Serial.begin(9600);
  delay(1000);
  lord_of_the_builders_two_arrays(inputMas, outputMas);
  for (byte i = 0; i <= 19; i++) {
    for (byte j = 0; j <= 6; j++) {
      Serial.print(finalMas[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println("-----------------------------");
}

void loop() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void search_spiers(byte outputMas[3][3]) {
  byte countSpiers = 100;
  byte countSpiersNice = 0;
  byte indexDom = 0;
  for (byte i = 0; i <= 1; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (outputMas[i + 1][j] == 0 and outputMas[i][j] != 0) {
        if (countSpiers > i) {
          countSpiers = i;
          indexDom = j;
        }
        //Serial.println(outputMas[i][j]);
        //outputMas[i][j] = outputMas[i][j] + 5;
        spiersMas[countSpiersNice] = outputMas[i][j];
        countSpiersNice++;
      }
      else if (i == 1 and outputMas[i + 1][j] != 0) {
        //Serial.println(outputMas[i + 1][j]);
        //outputMas[i][j] = outputMas[i][j] + 5;
        spiersMas[countSpiersNice] = outputMas[i + 1][j];
        countSpiersNice++;
      }
    }
  }

  spiersMas[3] = indexDom;
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
}

byte lord_of_the_builders_brotherhood_array()
{
  Serial.println("Go");
  while (true) {
    if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');
      long longdata = data.toInt();
      if (longdata > 100) {
        int floorOne = int(longdata - (longdata % 1000) / 1000000);
        int floorTwo = int((longdata - (longdata % 1000) / 1000) - (floorOne * 1000));
        int floorThree = int(longdata % 1000000);
        byte massPlanPos = build_matrix(floorOne, floorTwo, floorThree);
      }
      else {
        byte typeE = byte(longdata % 10);
        byte typeW = byte((longdata - longdata % 10) / 10);
        count++;
        switch (count) {
          case 1:
            massPlanKyd[0][0] = typeE;
            massPlanKyd[0][1] = typeW + 5;
            break;
          case 2:
            massPlanKyd[1][0] = typeE;
            massPlanKyd[1][1] = typeW + 5;
            break;
          case 3:
            massPlanKyd[2][0] = typeE;
            massPlanKyd[2][1] = typeW + 5;
            break;
          case 4:
            massPlanKyd[3][0] = typeE;
            massPlanKyd[3][1] = typeW + 5;
            break;
          case 5:
            massPlanKyd[4][0] = typeE;
            massPlanKyd[4][1] = typeW + 5;
            break;
        }
      }
    }
  }
}

byte build_matrix(int oneF, int twoF, int threeF) {
  byte massPlanPos[3][3];
  massPlanPos[0][0] = byte(oneF - (oneF % 100) / 100);
  massPlanPos[0][1] = byte(oneF - (oneF % 10) / 100);
  massPlanPos[0][2] = byte(oneF - ((oneF - (oneF % 100)) - (oneF % 10)) / 10);
  massPlanPos[1][0] = byte(twoF - (twoF % 100) / 100);
  massPlanPos[1][1] = byte(twoF - ((twoF - (twoF % 100)) - (twoF % 10)) / 10);
  massPlanPos[1][2] = byte(twoF % 10);
  massPlanPos[2][0] = byte(threeF - (threeF % 100) / 100);
  massPlanPos[2][1] = byte(threeF - ((threeF - (threeF % 100)) - (threeF % 10)) / 10);
  massPlanPos[2][2] = byte(threeF % 10);
  return massPlanPos;
  //PlatfornIO IDE
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//шпиль 1
//этаж 0
//зона материалов 0
//зона постройки 1
//input массив откуда, массив куда
//output массив типа (тип, зона откуда, местоположение в зоне, высота в мм, зона куда, местоположение в зоне, высота в мм)

byte lord_of_the_builders_two_arrays(byte inputMas[5][2], byte outputMas[3][3]) {

  //byte spiersMas[4];
  byte nullSpiersMas[3];
  byte countInOneFloor = 0;

  byte failSimulation[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};



  for (I_TO_3) {
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
    for (I_TO_3) {
      /*Serial.print("outputMas[0][i] - ");
        Serial.println(outputMas[0][i]);
        Serial.print("spiersMas[3] - ");
        Serial.println(spiersMas[3]);
        Serial.print("i - ");
        Serial.println(i);
        Serial.print("countOneFloor - ");
        Serial.println(countOneFloor);*/
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

    for (FOR_I_5) {
      for (byte j = 0; j <= 1; j++) {
        if (inputMas[i][0] == oneFloorMas[j]) {
          /*Serial.print("i - ");
            Serial.println(i);*/
          stopMas[j] = i;
        }
      }
    }

    //проверка 1 слоя на этажи и перенос их

    byte plaseTwoSS;
    bool flagErrorVar2 = false;

    for (byte j = 0; j <= 2; j++) {
      if (outputMas[0][j] < 6 and outputMas[0][j] != 0 and j != spiersMas[3]) {
        for (FOR_I_5) {
          flagErrorVar2 = false;
          /*Serial.print("outputMas[0][j] - ");
          Serial.println(outputMas[0][j]);
          Serial.print("i - ");
          Serial.println(i);
          Serial.print("inputMas[i][0] - ");
          Serial.println(inputMas[i][0]);
          Serial.print("inputMas[i][1] - ");
          Serial.println(inputMas[i][1]);*/
          if (outputMas[0][j] == inputMas[i][0] and inputMas[i][1] == 0) {
            array_generator(0, 0, i, 0, 1, j, 0);
            plaseTwoSS = i;
            failSimulation[0][j] = inputMas[i][0];
            inputMas[i][0] = 0;
          }
          else if (outputMas[0][j] == inputMas[i][0] and inputMas[i][1] != 0) {
            //Serial.println("YEEEEEEEEEEEEEEEEEEP");
            for (byte b = 0; b <= 4; b++) { // перетаскивание шпилей
              for (byte k = 0; k <= 1; k++) {
                /*Serial.println("ERROR LINE");
                Serial.print("stopMas[k] - ");
                Serial.println(stopMas[k]);
                Serial.print("b - ");
                Serial.println(b);
                Serial.print("inputMas[b][0] - ");
                Serial.println(inputMas[b][0]);
                Serial.print("inputMas[b][1] - ");
                Serial.println(inputMas[b][1]);8*/
                if (stopMas[k] != b and inputMas[b][0] != 0 and inputMas[b][1] == 0) {
                  //Serial.println("NOOOOOOOOOOOOOOOOOOOPE");
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
            //finalMas[countStep] = {0, 0, i, 0, 1, j, 0};
            failSimulation[0][j] = inputMas[i][0];
            plaseTwoSS = i;
            inputMas[i][0] = 0;
          }
        }
      }
    }

    byte stupidSpiers[2];
    byte countSS = 0;


    for (FOR_I_5) {
      if (inputMas[i][1] != 0) {
        stupidSpiers[countSS] = i;
        countSS++;
      }
    }

    array_generator(1, 0, stupidSpiers[0], CUBE_HIGHT_B, 0, plaseTwoSS, 0);
    array_generator(1, 0, stupidSpiers[1], CUBE_HIGHT_B, 0, plaseTwoSS, CUBE_HIGHT_S);

    bool flag = false;

    for (I_TO_3) {
      if (outputMas[2][i] > 0) flag = true;
    }

    if (flag) {

      byte stupidSpiersNum2[2][2] = {{11, 11}, {11, 11}};
      countSS = 0;
      byte flagSS = 0;

      for (I_TO_3) {
        //Serial.print("outputMas[1][i] - ");
        //Serial.println(outputMas[1][i]);
        if (outputMas[1][i] < 6 and outputMas[1][i] > 0) {
          stupidSpiersNum2[countSS][0] = outputMas[1][i];
          stupidSpiersNum2[countSS][1] = i;
          countSS++;
          if (stupidSpiersNum2[countSS] != 11) flagSS++;
        }
      }



      if (flagSS == 2) {
        for (FOR_I_5) {
          for (byte j = 0; j <= 1; j++) {
            //Serial.print("stupidSpiersNum2[j][0] - ");
            //Serial.println(stupidSpiersNum2[j][0]);
            //Serial.print("inputMas[i][0] - ");
            //Serial.println(inputMas[i][0]);
            if (stupidSpiersNum2[j][0] == inputMas[i][0]) {
              array_generator(0, 0, i, CUBE_HIGHT_B, 1, stupidSpiersNum2[j][1], CUBE_HIGHT_B);
              //failSimulation[1][stupidSpiers[j][1]];
            }
          }
        }

        byte countSOS = 0;
        byte saveAss;

        for (int i = 2; i >= 0; i--) {
          for (byte j = 0; j <= 2; j++) {
            /*Serial.print("failSimulation[i][spiersMas[3]] - ");
              Serial.println(failSimulation[i][spiersMas[3]]);
              Serial.print("outputMas[2][j] - ");
              Serial.println(outputMas[2][j]);
              Serial.print("j - ");
              Serial.println(j);
              Serial.print("i - ");
              Serial.println(i);*/
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
        for (I_TO_3) {
          Serial.print("outputMas[i][spiersMas[3]] - ");
          Serial.println(outputMas[i][spiersMas[3]]);
          if (outputMas[i][spiersMas[3]] < 6 and outputMas[i][spiersMas[3]] != 0) {
            eerrorFlag = false;
          }
        }
        Seri

        if (!errorFlag) {
          return 1;
        }
        else {
          array_generator(1, 1, spiersMas[3], 0, 1, saveAss, 2 * CUBE_HIGHT_B + CUBE_HIGHT_S);

          for (FOR_I_5) {
            if (outputMas[spiersMas[3]][1] == inputMas[i][0]) {
              array_generator(0, 0, i, 0, 1, spiersMas[3], 0);
            }

          }
          array_generator(1, 1, saveAss, 2 * CUBE_HIGHT_B + CUBE_HIGHT_S, 1, spiersMas[3], CUBE_HIGHT_B);

          return 1;
        }
      }

      else if (flagSS == 1) {
        for (FOR_I_5) {
          for (byte j = 0; j <= 1; j++) {
            if (stupidSpiersNum2[j][0] == inputMas[i][0]) {
              array_generator(0, 0, i, CUBE_HIGHT_B, 1, stupidSpiersNum2[j][1], CUBE_HIGHT_B);

            }
          }
        }

        byte countSpirtsTwoFloor = 0;

        for (I_TO_3)
          if (outputMas[1][i] > 5 and outputMas[1][i] <= 10) countSpirtsTwoFloor++;

        if (countSpirtsTwoFloor == 2) {
          byte spirtsMas[2][2];
          for (I_TO_3) {
            if (outputMas[2][i] > 5 and outputMas[2][i] <= 10 and i != spiersMas[3]) {
              spirtsMas[0][0] = i;
              spirtsMas[0][1] = outputMas[2][i];
            }
          }
          for (I_TO_3) {
            if (outputMas[1][i] > 5 and outputMas[1][i] <= 10 and i != spiersMas[3]) {
              spirtsMas[1][0] = i;
              spirtsMas[1][1] = outputMas[1][i];
            }
          }

          byte countDa = 0;
          byte saveAssVar2;

          for (I_TO_3) {
            for (J_TO_3) {
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
            array_generator(1, 1, spiersMas[3], 0, 1, saveAssVar2, CUBE_HIGHT_B + CUBE_HIGHT_S);
            for (FOR_I_5) {
              if (inputMas[i][0] == outputMas[0][spiersMas[3]])
                array_generator(0, 0, i, 0, 1, spiersMas[3], 0);
            }
            array_generator(1, 1, saveAssVar2, CUBE_HIGHT_B + CUBE_HIGHT_S, 1, spiersMas[3], CUBE_HIGHT_B);
          }
          return 1;
        }

        else if (countSpirtsTwoFloor == 1) {
          byte spirtsMas[2][2];
          for (I_TO_3) {
            if (outputMas[2][i] > 5 and outputMas[2][i] <= 10 and i != spiersMas[3]) {
              spirtsMas[0][0] = i;
              spirtsMas[0][1] = outputMas[2][i];
            }
          }
          for (I_TO_3) {
            if (outputMas[1][i] > 5 and outputMas[1][i] <= 10 and i != spiersMas[3]) {
              spirtsMas[1][0] = i;
              spirtsMas[1][1] = outputMas[1][i];
            }
          }

          byte countDa = 0;
          byte saveAssVar2;

          for (I_TO_3) {
            for (J_TO_3) {
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
              else
                array_generator(1, 1, spiersMas[3], CUBE_HIGHT_S, 1, j, CUBE_HIGHT_B);
            }
          }
        }
      }
    }
    else {
      byte errorVar = 0;
      byte saveAssVar3;
      for (int i = 2; i >= 0; i--) {
        for (J_TO_3) {
          if (failSimulation[i][spiersMas[3]] == outputMas[1][j]) {
            if (errorVar == 0) {
              array_generator(1, 1, spiersMas[3], 2 * CUBE_HIGHT_S, 1, j, CUBE_HIGHT_B);
              errorVar++;
              saveAssVar3 = j;
            }
            else
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_S, 1, j, CUBE_HIGHT_B);
          }
        }
      }
      bool flagUh = false;

      for (I_TO_3) {
        if (outputMas[i][spiersMas[3]] > 0 and outputMas[i][spiersMas[3]] < 6) flagUh = true;
      }
      if (flagUh) {
        array_generator(1, 1, spiersMas[3], 0, 1, saveAssVar3, CUBE_HIGHT_S + CUBE_HIGHT_B);
        for (FOR_I_5) {
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
      for (I_TO_3) {
        for (FOR_J_5) {
          if (outputMas[0][i] == inputMas[j][1]) {
            array_generator(1, 0, j, CUBE_HIGHT_B, 1, i, 0);
          }
        }
      }
    }
    else {
      byte errorIndex;
      for (I_TO_3) {
        if (outputMas[0][i] < 6 and outputMas[0][i] > 0) errorIndex = i;
      }
      byte countFloorMeh = 0;
      byte oneKub[2] = {0 , 0};
      byte twoKub[2] = {0, 0};
      for (I_TO_3) {
        if (outputMas[i][errorIndex] < 6 and outputMas[i][errorIndex] > 0) {
          oneKub[countFloorMeh] = outputMas[i][errorIndex];
          countFloorMeh++;
        }
      }

      for (I_TO_3) {
        for (FOR_J_5) {
          if (outputMas[0][i] == inputMas[j][1]) {
            array_generator(1, 0, j, CUBE_HIGHT_B, 1, i, 0);
            inputMas[j][1] = 0;
          }
        }
      }

      for (FOR_I_5) {
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

      for (FOR_I_5) {
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
      for (I_TO_3) {
        for (FOR_J_5) {
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
