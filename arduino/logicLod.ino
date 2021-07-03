byte whileMas[2] = {0, 0};
  byte entaw = 0;


  for (byte i = 5; i <= 6; i++) {
    if (inputMas[i][0] > 0) {
      entaw =  i;
    }
  }


  byte countNoneSpiers = 0;
  byte indexKub =  0;
  byte spiersInMainZone[2] = {0, 0};
  byte typeMap = 0;
  byte countSpiersInMainZoneTwo = 0;

  typeMap = search_type_house();

  byte indexWhite = 0;
  byte indexWhiteKub = 0;
  byte indexWhiteWpil = 0;

  for (byte i = 0; i <= 4; i++) {
    if (inputMas[i][0] == 6) {
      indexWhiteKub = i;
      for (byte j = 0; j <= 4; j++) {
        if (inputMas[j][1] == 11) {
          indexWhiteWpil = j;
          if (i == 4 and j != 3) {
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, 3, CUBE_HIGHT_S + CUBE_HIGHT_B);
            indexWhite = 3;
          }
          else if (i == 1 and j != 2) {
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, 2, CUBE_HIGHT_S + CUBE_HIGHT_B);
            indexWhite = 2;
          }
          else if (i == 4 and j == 3) {
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, 2, CUBE_HIGHT_S + CUBE_HIGHT_B);
            indexWhite = 3;
          }
          else if (i == 1 and j == 2) {
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, 3, CUBE_HIGHT_S + CUBE_HIGHT_B);
            indexWhite = 2;
          }
          else if (i != 1 and i != 4 and (i + 1 != j)) {
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, i + 1, CUBE_HIGHT_S + CUBE_HIGHT_B);
            indexWhite = i + 1;
          }
          else if (i != 1 and i != 4 and (i - 1 != j)) {
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, i - 1, CUBE_HIGHT_S + CUBE_HIGHT_B);

            indexWhite = i - 1;
          }
        }
      }
    }
  }


  array_generator(1, 0, indexWhiteWpil, CUBE_HIGHT_B, 0, indexWhiteKub, CUBE_HIGHT_B);

  array_generator(1, 0, indexWhite, CUBE_HIGHT_S + CUBE_HIGHT_B, 0, indexWhiteWpil, CUBE_HIGHT_B);

  byte inputMasIndexWhiteWpil = inputMas[indexWhiteWpil][1];
  inputMas[indexWhiteWpil][1] = inputMas[indexWhite][1];
  inputMas[indexWhite][1] = inputMasIndexWhiteWpil;

  bool flagWhile = false;
  byte indexDa = 0;

  //перетаскиваем один этаж и 3 шпиля в зону постройки, после чего убираем 2 бесполезных шпиля
  for (byte i = 0; i <= 4; i++) {
    if (inputMas[6][0] == outputMas[0][spiersMas[3]] or inputMas[5][0] == outputMas[0][spiersMas[3]]) {
      if (inputMas[6][0] == outputMas[0][spiersMas[3]])
        indexDa = 0;
      else
        indexDa = 1;
      array_generator(0, 2, indexDa, 0, 1, spiersMas[3], 0);

      byte countStepeers = 0;

      for (byte k = 0; k <= 2; k++) {
        for (byte j = 0; j <= 6; j++) {
          if (inputMas[j][1] == spiersMas[k]) {
            array_generator(1, 0, j, 0, 1, spiersMas[3], CUBE_HIGHT_B + (CUBE_HIGHT_S * countStepeers));
            countStepeers++;
          }
        }
      }
    }



    else if (inputMas[i][0] == outputMas[0][spiersMas[3]]) {

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

        for (byte j = 0; j <= 6; j++) {
          for (byte L = 0; L <= 3; L++) {
            if (inputMas[j][1] == spiersMas[L] and spiersMas[L] != inputMas[i][1]) {
              if (countNumSpiersInSpiersMas == 0) {
                if (j != 5 and j != 6) {
                  if (j == 5) {
                    array_generator(1, 2, 1, 0, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S);
                  }
                  else {
                    array_generator(1, 2, 0, 0, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S);
                  }
                }
                else {
                  array_generator(1, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B + CUBE_HIGHT_S);
                }
                spiersInMainZone[1] = inputMas[j][1];
              }
              else {
                if (j != 5 and j != 6) {
                  if (j == 5) {
                    array_generator(1, 2, 1, 0, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S);
                  }
                  else {
                    array_generator(1, 2, 0, 0, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S);
                  }
                }
                else {
                  array_generator(1, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S);
                }
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

  //перетаскиваем два этажа на первый этаж
  for (byte i = 0; i <= 6; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (inputMas[i][0] == outputMas[0][j] and j != spiersMas[3])
        array_generator(0, 0, i, 0, 1, j, 0);
    }
  }

  //перетаскиваем два этажа на второй этаж
  for (byte i = 0; i <= 6; i++) {
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
  }

  if (entaw == 6) {
    array_generator(1, 0, indexWhiteKub, CUBE_HIGHT_B, 2, 1, 0);
    array_generator(0, 0, indexWhiteKub, 0, 2, 0, 0);
  }

