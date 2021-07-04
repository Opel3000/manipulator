//---------------FUNCTION LOGIC---------------
byte finalMas[17][7];

#define CUBE_HIGHT_B 41
#define CUBE_HIGHT_S 31

byte outputMas[3][3] = {{5, 4, 3}, {3, 2, 4}, {0, 0, 0}};
byte inputMas[5][2] = {{5, 7}, {1, 6}, {2, 10}, {4, 8}, {3, 9}};
byte spiersMas[4] = {0, 0, 0, 0};
byte countStep = 0;
byte count = 0;

void setup() {
  Serial.begin(9600);
  lord_of_the_builders_two_arrays();
  for (int i = 0; i < 17; i++) {
    for (int j = 0; j < 7; j++) {
      Serial.print(finalMas[i][j]);
      Serial.print("  ");
    }
    Serial.println();
  }
  Serial.println("/////////////////");

}

void loop() {
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
        outputMas[i][j] = outputMas[i][j] + 5;
      }
      else if (i == 1 and outputMas[i + 1][j] != 0) {
        //Serial.println(outputMas[i + 1][j]);
        outputMas[i + 1][j] = outputMas[i + 1][j] + 5;
      }
    }
  }
  for (byte i = 0; i <= 2; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (outputMas[i][j] > 5) {
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


void lord_of_the_builders_brotherhood_array() { //153221534
  Serial.print("G");
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
        int typeE = longdata % 10;
        byte typeW = byte(longdata / 10);
        count++;
        switch (count) {
          case 1:
            inputMas[0][0] = byte(typeE);
            inputMas[0][1] = typeW + 5;
            break;
          case 2:
            inputMas[1][0] = byte(typeE);
            inputMas[1][1] = typeW + 5;
            //            Serial.println(inputMas[1][0]);
            //            Serial.println(byte(typeE));
            //            Serial.println(typeE);
            break;
          case 3:
            inputMas[2][0] = byte(typeE);
            inputMas[2][1] = typeW + 5;
            break;
          case 4:
            inputMas[3][0] = byte(typeE);
            inputMas[3][1] = typeW + 5;
            break;
          case 5:
            inputMas[4][0] = byte(typeE);
            inputMas[4][1] = typeW + 5;
            break;
        }
        break;
      }
    }
  }
}


void lord_of_the_builders_two_arrays() {

  search_spiers();


  byte countNoneSpiers = 0;
  byte indexKub =  0;
  byte spiersInMainZone[2] = {0, 0};
  byte typeMap = 0;
  byte countSpiersInMainZoneTwo = 0;

  typeMap = search_type_house();

  if (typeMap == 4 or typeMap == 5) {

  byte countPerenosSpiers = 0

    for (byte i = 0; i <= 2; i++){
      for (byte j = 0; j <= 4; j++){
        if (spiersMas[i] == inputMas[j][1]){
          array_generator(1, 0, j, CUBE_HIGHT_B, 0, spiersMas, CUBE_HIGHT_S * countPerenosSpiers);
          if (countPerenosSpiers == 1)
            spiersInMainZone[1] = inputMas[j][1];
          else if (countPerenosSpiers == 2)
            spiersInMainZone[0] = inputMas[j][1];
          countPerenosSpiers++;
        }
      }
    }

  }
  else {
    //перетаскиваем один этаж и 3 шпиля в зону постройки, после чего убираем 2 бесполезных шпиля
    for (byte i = 0; i <= 4; i++) {
      if (inputMas[i][0] == outputMas[0][0]) {

        byte indexPutNoSpiers = i;

        if (inputMas[i][1] == spiersMas[0]) {//если на этаже шпиль, который изначально долежн быть на нём
          if (i == 4)
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, 3, CUBE_HIGHT_S + CUBE_HIGHT_B);
          else
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, 4, CUBE_HIGHT_S + CUBE_HIGHT_B);

          array_generator(0, 0, i, 0, 1, spiersMas[3], 0);

          if (i == 4)
            array_generator(1, 0, 3, CUBE_HIGHT_S + CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B);
          else
            array_generator(1, 0, 4, CUBE_HIGHT_S + CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B);

          byte countNumSpiersInSpiersMas = 0;

          for (byte j = 0; j <= 4; j++) {
            for (byte L = 0; L <= 3; L++) {
              if (inputMas[j][1] == spiersMas[L] and spiersMas[L] != inputMas[i][1]) {
                if (countNumSpiersInSpiersMas == 0) {
                  array_generator(1, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S);
                  spiersInMainZone[1] = inputMas[j][1];
                }
                else {
                  array_generator(1, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S);
                  spiersInMainZone[0] = inputMas[j][1];
                }
                countNumSpiersInSpiersMas++;
              }
            }
          }



          for (byte j = 0; j <= 4; j++) {
            if (inputMas[j][1] != spiersMas[0] and inputMas[j][1] != spiersMas[1] and inputMas[j][1] != spiersMas[2]) {
              if (countNoneSpiers == 0) {
                array_generator(1, 0, j, CUBE_HIGHT_B, 0, i, 0);
                countNoneSpiers++;
              }
              else {
                array_generator(1, 0, j, CUBE_HIGHT_B, 0, i, CUBE_HIGHT_S);
                break;
              }
            }
          }

          break;
        }

        else if (inputMas[i][1] == spiersMas[1] or inputMas[i][1] == spiersMas[2]) { //если на этаже шпили, которые используются в постройке
          byte spiersForSpiers[2] = {0, 0};


          for (byte j = 0; j <= 4; j++) {
            if (inputMas[j][1] == spiersMas[0]) {
              array_generator(1, 0, i, CUBE_HIGHT_B, 0, j, CUBE_HIGHT_B + CUBE_HIGHT_S);
              spiersInMainZone[1] = inputMas[i][1];
              inputMas[i][1] = 0;
            }
            else {
              continue;
            }


            array_generator(0, 0, i, 0, 1, spiersMas[3], 0);

            array_generator(2, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B);
            spiersInMainZone[0] = inputMas[j][1];
            inputMas[j][1] = 0;

            break;
          }

          for (byte j = 0; j <= 4; j++) {
            for (byte L = 0; L <= 2; L++) {
              if (inputMas[j][1] == spiersMas[L]) {
                array_generator(1, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S);
                spiersInMainZone[0] = inputMas[j][1];
                inputMas[j][1] = 0;
                break;
              }
            }
          }

          for (byte j = 0; j <= 4; j++) {
            if (inputMas[j][1] > 5) {
              if (countNoneSpiers == 0) {
                array_generator(1, 0, j, CUBE_HIGHT_B, 0, i, 0);
                countNoneSpiers++;
              }
              else {
                array_generator(1, 0, j, CUBE_HIGHT_B, 0, i, CUBE_HIGHT_S);
                break;
              }
            }
          }

          break;
        }

        else { //если на этаже бесполезный шпиль
          for (byte j = 0; j <= 4; j++) {
            if (j != i and inputMas[j][1] != spiersMas[0] and inputMas[j][1] != spiersMas[1] and inputMas[j][1] != spiersMas[2]) {
              array_generator(1, 0, i, CUBE_HIGHT_B, 0, j, CUBE_HIGHT_B + CUBE_HIGHT_S);
              indexKub = j;
            }
          }

          array_generator(0, 0, i, 0, 1, spiersMas[3], 0);

          byte countSpiersForInputMasTrash = 0;
          byte countSpiersInMainZone = 2;

          for (byte k = 0; k <= 2; k++) {
            for (byte j = 0; j <= 4; j++) {
              if (inputMas[j][1] == spiersMas[k]) {
                array_generator(1, 0, j, CUBE_HIGHT_B, 0, spiersMas[3], CUBE_HIGHT_B + (countSpiersForInputMasTrash * CUBE_HIGHT_S));
                if (countSpiersInMainZone != 2) {
                  spiersInMainZone[countSpiersInMainZone] = inputMas[j][1];
                }
                countSpiersInMainZone--;
                countSpiersForInputMasTrash++;
              }
            }
          }

          array_generator(2, 0, indexKub, CUBE_HIGHT_B, 0, i, 0);

          break;
        }
      }
    }
  }

  //перетаскиваем два этажа на первый этаж
  for (byte i = 0; i <= 4; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (inputMas[i][0] == outputMas[0][j] and j != spiersMas[3])
        array_generator(0, 0, i, 0, 1, j, 0);
    }
  }

  //перетаскиваем два этажа на второй этаж
  for (byte i = 0; i <= 4; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (inputMas[i][0] == outputMas[1][j] and j != spiersMas[3])
        array_generator(0, 0, i, 0, 1, j, CUBE_HIGHT_B);
    }
  }



  switch (typeMap) {

    case 1:

      for (byte j = 0; j <= 1; j++) {
        for (byte i = 0; i <= 2; i++) {
          if (spiersInMainZone[j] == outputMas[2][i] and i != spiersMas[3]) {
            if (countSpiersInMainZoneTwo == 0) {
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S, 1, i, 2 * CUBE_HIGHT_B);
              countSpiersInMainZoneTwo++;
            }
            else
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S, 1, i, 2 * CUBE_HIGHT_B);
          }
        }
      }
      break;


    case 2:

      for (byte j = 0; j <= 1; j++) {
        for (byte i = 0; i <= 2; i++) {
          for (byte k = 1; k <= 2; k++) {
            if (spiersInMainZone[j] == outputMas[k][i] and i != spiersMas[3]) {
              if (countSpiersInMainZoneTwo == 0) {
                if (k == 1)
                  array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S, 1, i, CUBE_HIGHT_B);
                else
                  array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S, 1, i, 2 * CUBE_HIGHT_B);
                countSpiersInMainZoneTwo++;
              }
              else {
                if (k == 1)
                  array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S, 1, i, CUBE_HIGHT_B);
                else
                  array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S, 1, i, 2 * CUBE_HIGHT_B);
              }
            }
          }
        }
      }
      break;

    case 3:

      for (byte j = 0; j <= 1; j++) {
        for (byte i = 0; i <= 2; i++) {
          if (spiersInMainZone[j] == outputMas[1][i] and i != spiersMas[3]) {
            if (countSpiersInMainZoneTwo == 0) {
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S, 1, i,  CUBE_HIGHT_B);
              countSpiersInMainZoneTwo++;
            }
            else
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S, 1, i,  CUBE_HIGHT_B);
          }
        }
      }
      break;

      case 4:
      for (byte j = 0; j <= 1; j++) {
        for (byte i = 0; i <= 2; i++) {
          if (spiersInMainZone[j] == outputMas[1][i] and i != spiersMas[3]) {
            if (countSpiersInMainZoneTwo == 0) {
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S, 1, i,  CUBE_HIGHT_B * 2);
              countSpiersInMainZoneTwo++;
            }
            else
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S, 1, i,  CUBE_HIGHT_B);
          }
        }
      }

      break;

      case 5:
      for (byte j = 0; j <= 1; j++) {
        for (byte i = 0; i <= 2; i++) {
          if (spiersInMainZone[j] == outputMas[1][i] and i != spiersMas[3]) {
            if (countSpiersInMainZoneTwo == 0) {
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S, 1, i,  CUBE_HIGHT_B * 2);
              countSpiersInMainZoneTwo++;
            }
            else
              array_generator(1, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S, 1, i,  CUBE_HIGHT_B * 2);
          }
        }
      }

      break;
  }

}


byte search_type_house() {
  byte countOneFloorW = 0;
  byte countTwoFloorW = 0;

  byte countOneFloorE = 0;
  byte countTwoFloorE = 0;
  byte countThreeFloorE = 0;

  for (byte i = 0; i <= 2; i++) {
    if (outputMas[0][i] < 6 and outputMas[0][i] != 0)
      countOneFloorW++;
    if (outputMas[1][i] < 6 and outputMas[1][i] != 0)
      countTwoFloorW++;
    if (outputMas[0][i] > 5)
      countOneFloorE++;
    if (outputMas[1][i] > 5)
      countTwoFloorE++;
    if (outputMas[2][i] > 5)
      countThreeFloorE++;
  }

  if (countOneFloorW == 3 and countTwoFloorW == 1 and countOneFloorE == 0  and  countTwoFloorE == 1 and countThreeFloorE == 2)
    return 1;
  if (countOneFloorW == 3 and countTwoFloorW == 2 and countOneFloorE == 0  and  countTwoFloorE == 2 and countThreeFloorE == 1)
    return 2;
  if (countOneFloorW == 3 and countTwoFloorW == 0 and countOneFloorE == 0  and  countTwoFloorE == 3 and countThreeFloorE == 0)
    return 3;
  if (countOneFloorW == 2 and countTwoFloorW == 1 and countOneFloorE == 1  and  countTwoFloorE == 1 and countThreeFloorE == 1)
    return 4;
  if (countOneFloorW == 2 and countTwoFloorW == 2 and countOneFloorE == 1  and  countTwoFloorE == 1 and countThreeFloorE == 1)
    return 5;
}
