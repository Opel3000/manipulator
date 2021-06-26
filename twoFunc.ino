byte lord_of_the_builders_two_arrays(byte inputMas[5][2], byte outputMas[3][3]) {


  search_spiers(outputMas);

  byte countNoneSpiers = 0;
  byte spiersMasNO = {0, 0};

  //ищем бесполезные шпили
  for (byte i = 0; i <= 4; i++) {
    for (byte j = 0; j <= 3; j++) {
      if (inputMas[i][1] != spiersMas[j]) {
        spiersMasNO[countNoneSpiers] = i;
        countNoneSpiers++;
      }
    }
  }

  //перетаскиваем один этаж и 3 шпиля в зону постройки, после чего убираем 2 бесполезных шпиля
  for (byte i = 0; i <= 4; i++) {
    if (inputMas[i][0] == outputMas[0][spiersMas[3]]) {

      byte indexPutNoSpiers = i;
      byte spiersInMainZone = {0, 0};

      if (inputMas[i][1] == spiersMas[0]) { //если на этаже шпиль, который изначально долежн быть на нём
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

        array_generator(1, 0, spiersMasNO[0], CUBE_HIGHT_B, 0, i, 0);
        array_generator(1, 0, spiersMasNO[1], CUBE_HIGHT_B, 0, i, CUBE_HIGHT_S);


        break;
      }

      else if (inputMas[i][1] == spiersMas[1] or inputMas[i][1] == spiersMas[2]) { //если на этаже шпили, которые используются в постройке
        byte spiersForSpiers = {0, 0};

        for (byte j = 0; j <= 4; j++) {
          if (inputMas[j][1] == spiersMas[0]) {
            array_generator(1, 0, i, CUBE_HIGHT_B, 0, j, CUBE_HIGHT_B + CUBE_HIGHT_S);
            spiersInMainZone[1] = inputMas[j][1];
          }
          else
            continue;

          spiersForSpiers[0] = inputMas[i][1];
          spiersForSpiers[1] = inputMas[j][1];

          array_generator(0, 0, i, 0, 1, spiersMas[3], 0);

          array_generator(2, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B);

          break;
        }

        for (byte j = 0; j <= 4; j++) {
          for (byte L = 0; L <= 1; L++) {
            if (inputMas[j][1] != spiersForSpiers[L])
              array_generator(1, 0, j, CUBE_HIGHT_B, 1, spiersMas[3], CUBE_HIGHT_B + 2 * CUBE_HIGHT_S);
            spiersInMainZone[0] = inputMas[j][1];
            break;
          }
        }

        array_generator(1, 0, spiersMasNO[0], CUBE_HIGHT_B, 0, i, 0);
        array_generator(1, 0, spiersMasNO[1], CUBE_HIGHT_B, 0, i, CUBE_HIGHT_S);

        break;
      }

      else if (i == spiersMasNO[0] or i == spiersMasNO[1]) { //если на этаже бесполезный шпиль
        if (i == spiersMasNO[0])
          array_generator(1, 0, spiersMasNO[0], CUBE_HIGHT_B, 0, spiersMasNO[1], CUBE_HIGHT_B + CUBE_HIGHT_S);
        else
          array_generator(1, 0, spiersMasNO[1], CUBE_HIGHT_B, 0, spiersMasNO[0], CUBE_HIGHT_B + CUBE_HIGHT_S);

        array_generator(0, 0, i, 0, 1, spiersMas[3], 0);

        byte countSpiersForInputMasTrash = 0;
        byte countSpiersInMainZone = 2;

        for (byte j = 0; j <= 4; j++) {
          for (byte L = 0; L <= 3; L++) {
            if (spiersMas[L] == inputMas[j][1]) {
              array_generator(1, 0, j, CUBE_HIGHT_B, 0, spiersMasNO[1], CUBE_HIGHT_B + countSpiersForInputMasTrash * CUBE_HIGHT_S);
              if (countSpiersInMainZone != 2) {
                spiersInMainZone[countSpiersInMainZone] = inputMas[j][1];
              }
              countSpiersInMainZone--;
              countSpiersForInputMasTrash++;
            }
          }
        }

        if (i == spiersMasNO[0])
          array_generator(2, 0, spiersMasNO[1], CUBE_HIGHT_B, 0, i, 0);
        else
          array_generator(2, 0, spiersMasNO[0], CUBE_HIGHT_B, 0, i, 0);


        break;
      }
    }
  }

  //перетаскиваем два этажа на первый этаж
  for (byte i = 0; i <= 4; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (inputMas[i][0] == outpuMas[0][j] and j != spiersMas[3])
        array_generator(0, 0, i, 0, 1, j, 0);
    }
  }

  //перетаскиваем два этажа на второй этаж
  for (byte i = 0; i <= 4; i++) {
    for (byte j = 0; j <= 2; j++) {
      if (inputMas[i][0] == outpuMas[1][j] and j != spiersMas[3])
        array_generator(0, 0, i, 1, j, CUBE_HIGHT_B);
    }
  }

  for (byte j = 0; j <= 1; j++) {
    for (byte i = 0; i <= 2; i++) {
      if (spiersInMainZone[j] == outputMas[2][i] and i != spiersMas[3])
          array_generator(1, 1, spiersMas[3], 1, i, 2*CUBE_HIGHT_B);
  }
}
