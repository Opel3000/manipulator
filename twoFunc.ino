void lord_of_the_builders_two_arrays() {

  search_spiers();


  byte countNoneSpiers = 0;
  byte indexKub =  0;
  byte spiersInMainZone[2] = {0, 0};


  //ищем бесполезные шпили
  //  for (byte i = 0; i <= 4; i++) {
  //    for (byte j = 0; j <= 2; j++) {
  //      if (inputMas[i][1] != spiersMas[j]) {
  //        spiersMasNO[countNoneSpiers] = i;
  //        countNoneSpiers++;
  //      }
  //    }
  //  }

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

  byte countSpiersInMainZoneTwo = 0;

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
}
