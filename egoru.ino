#include "GyverStepper.h"
#include <Servo.h>

//1 - красный
//2 - синий
//3 - жёлтый
//4 - зелёный
//5 - black

//шпиль 1
//этаж 0
//зона материалов 0
//зона постройки 1
//output массив типа (тип, зона откуда, местоположение в зоне, высота в мм, зона куда, местоположение в зоне, высота в мм)


//---------------CONST LOGIC---------------

#define CUBE_HIGHT_B 41
#define CUBE_HIGHT_S 31

byte count = 0;
byte finalMas[20][7];
byte countStep = 0;
byte spiersMas[4];

byte inputMas[5][2] = {{1, 8}, {3, 10}, {2, 7}, {4, 6}, {5, 9}};
byte outputMas[3][3] = {{8, 9, 7}, {0, 0, 0}, {0, 0, 0}};

//---------------CONST MANIPULATOR---------------

#define STEPPER_EN_PIN 8
// The X Stepper pins
#define STEPPER1_DIR_PIN 5  ////////change pins!!!!!
#define STEPPER1_STEP_PIN 2
// The Y stepper pins
#define STEPPER2_DIR_PIN 6
#define STEPPER2_STEP_PIN 3

#define A_AXIS_LIMIT_SWITCH_PIN 9
#define Z_AXIS_LIMIT_SWITCH_PIN 10

#define SERVO_X   12
#define SERVO_Y   7
#define SERVO_Z   4
#define SERVO_ZZ  13

#define CUBE_HEIGHT_B 41
#define CUBE_HEIGHT_S 31

GStepper<STEPPER2WIRE> Astepper(200, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
GStepper<STEPPER2WIRE> Zstepper(200, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN, STEPPER_EN_PIN);

Servo ServoX;
Servo ServoY;
Servo ServoZ;
Servo ServoZZ;

int ServX[4] = {135, 50, 60, 90};
int ServY[3] = {40, 90, 25};
int ServZ[3] = {150, 100, 80};
const int gap = 5;
const int turnPerBigTurn = 8;
const int stepInmm = 18;
int IndexOfActionSteps = 0;

const int stepPerTurn = 200;
const int AAxisTurn = stepPerTurn * 8;
const int ZAxisTurn = stepPerTurn * 8;

int Stepper[4] = {0, 0, 0 , 0};

int InputArrPlan[3][4] = { { 1, 2, 0, 0 }, { 2, 4, 3, 0 }, { 5, 4, 0, 0 } };  ///////     X  Z

int InputArrFack[5][2] = { { 1, 2 }, { 2, 3 }, { 4, 3 }, { 5, 0 }, { 2, 4 } };  ///////     X  Z
/*
  int MasOfAction[4][7] = { {1, 1, 0, 40, 1, 2, 40},
                          {0, 0, 1, 70, 1, 0, 40},
                          {1, 1, 0, 40, 0, 1, 70},
                          {0, 0, 1, 70, 1, 0, 40}
  };         ///////     (type, PlaceIn, xIn, yIn, PlaceOut, xOut, yOut)
*/
int MasOfActionSteps[40][5] = {  {0, 0, 0, 0, 0},            ///////     (Astepper, Zstepper, S1, S2, grab)
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0}
};

int MainIndex = 0;

//---------------END CONST---------------


void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(A_AXIS_LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(Z_AXIS_LIMIT_SWITCH_PIN, INPUT_PULLUP);

  ServoX.attach(SERVO_X);
  ServoY.attach(SERVO_Y);
  ServoZ.attach(SERVO_Z);
  ServoZZ.attach(SERVO_ZZ);

  GoHome();

  //ФУНКЦИЯ НОМЕР НОЛЬ

  //ФУНКЦИЯ НОМЕР НОЛЬ
  lord_of_the_builders_two_arrays(inputMas, outputMas);



  //Serial.println("/////////////////");
  PointG();
  PrintArr(MasOfActionSteps);

  //Serial.println();
  //    PrintArr(MasOfActionSteps);
  //    PrintArr1(MasOfAction);
  //Serial.println("/////////////////");
  // delay(1000);

  Astepper.setMaxSpeed(1000);
  Astepper.setAcceleration(4000);

  Zstepper.setMaxSpeed(800);
  Zstepper.setAcceleration(6000);

}

void loop() {

  if (MainIndex < 2) {
    static uint32_t tmr = millis();
    Astepper.setTarget(MasOfActionSteps[MainIndex][0]);
    //    Zstepper.setTarget(MasOfActionSteps[MainIndex][1]);
    ServoX.write(MasOfActionSteps[MainIndex][2]);
    ServoY.write(MasOfActionSteps[MainIndex][3]);
    while (Astepper.getState()/* || Zstepper.getState()*/ || millis() - tmr < 100) {
      Astepper.tick();
      //      Zstepper.tick();
    }

    //    Zstepper.setTarget(MasOfActionSteps[MainIndex][1] - CUBE_HEIGHT_B * stepInmm);
    //    while (Zstepper.tick())
    //      Zstepper.tick();

    static uint32_t Servtmr = millis();
    ServoZ.write(MasOfActionSteps[MainIndex][4]);
    while (millis() - Servtmr < 100)
      delay(1);

    //    Zstepper.setTarget(MasOfActionSteps[MainIndex][1]);
    //    while (Zstepper.tick())
    //      Zstepper.tick();

    MainIndex++;
  }
  /*
      static bool dir;
      if (!Astepper.tick()) {
      dir = !dir;
      Astepper.setTarget(dir ? 0 : 200 * 8);
      delay(500);
      }

      static uint32_t tmr2;
      if (millis() - tmr2 > 20) {
      tmr2 = millis();
      //Serial.println(Astepper.getCurrent());
      }*/
}

//---------------FUNCTION LOGIC---------------


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


byte lord_of_the_builders_brotherhood_array() { //153221534
  Serial.println("Go");
  while (true) {
    if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');
      long longdata = data.toInt();
      Serial.println(longdata);
      if (longdata > 100) {
        int floorOne = int(longdata / 1000000);
        int floorTwo = int(longdata / 1000 % 1000);
        int floorThree = int(longdata % 1000);
        build_matrix(floorOne, floorTwo, floorThree);
      }
      else {
        byte typeE = byte(longdata % 10);
        byte typeW = byte((longdata - longdata % 10) / 10);
        count++;
        switch (count) {
          case 1:
            inputMas[0][0] = typeE;
            inputMas[0][1] = typeW + 5;
            break;
          case 2:
            inputMas[1][0] = typeE;
            inputMas[1][1] = typeW + 5;
            break;
          case 3:
            inputMas[2][0] = typeE;
            inputMas[2][1] = typeW + 5;
            break;
          case 4:
            inputMas[3][0] = typeE;
            inputMas[3][1] = typeW + 5;
            break;
          case 5:
            inputMas[4][0] = typeE;
            inputMas[4][1] = typeW + 5;
            break;
        }
      }
    }
  }
}


byte lord_of_the_builders_two_arrays(byte inputMas[5][2], byte outputMas[3][3]) {

  //byte spiersMas[4];
  byte nullSpiersMas[3];
  byte countInOneFloor = 0;

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

    for (byte i = 0; i <= 4; i++) {
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
        for (byte i = 0; i <= 4; i++) {
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


    for (byte i = 0; i <= 4; i++) {
      if (inputMas[i][1] != 0) {
        stupidSpiers[countSS] = i;
        countSS++;
      }
    }

    array_generator(1, 0, stupidSpiers[0], CUBE_HIGHT_B, 0, plaseTwoSS, 0);
    array_generator(1, 0, stupidSpiers[1], CUBE_HIGHT_B, 0, plaseTwoSS, CUBE_HIGHT_S);

    bool flag = false;

    for (byte i = 0; i <= 2; i++) {
      if (outputMas[2][i] > 0) flag = true;
    }

    if (flag) {

      byte stupidSpiersNum2[2][2] = {{11, 11}, {11, 11}};
      countSS = 0;
      byte flagSS = 0;

      for (byte i = 0; i <= 2; i++) {
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
        for (byte i = 0; i <= 4; i++) {
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
            if (outputMas[spiersMas[3]][0] == inputMas[i][0]) {
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
              array_generator(0, 0, i, CUBE_HIGHT_B, 1, stupidSpiersNum2[j][1], CUBE_HIGHT_B);

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


//---------------FUNCTION MANIPULATOR---------------


void GoHome() {
  //  Astepper.setRunMode(KEEP_SPEED);
  //  Astepper.setSpeed(-200);

  Zstepper.setRunMode(KEEP_SPEED);
  Zstepper.setSpeed(200);

  //  while (digitalRead(A_AXIS_LIMIT_SWITCH_PIN)) {
  //    Astepper.tick();
  //  }
  Astepper.reset();

  //  while (digitalRead(A_AXIS_LIMIT_SWITCH_PIN)) {
  //    Zstepper.tick();
  //  }
  Zstepper.reset();

  ServoX.write(50);
  ServoY.write(90);
  //  ServoZ.write(150);
  ServoZZ.write(90);

  delay(2000);

  //  ServoX.write(90);
  //  ServoY.write(25);
  ServoZ.write(80);
  ServoZZ.write(0);

  delay(2000);

  //  ServoX.write(135);
  //  ServoY.write(115);
  //  ServoZ.write(150);
  ServoZZ.write(90);

  delay(2000);

  //  Astepper.reset();
  //  Astepper.setRunMode(FOLLOW_POS);
  //  Zstepper.reset();
  //  Zstepper.setRunMode(FOLLOW_POS);

}


void PointG() {

  int (*a)[4];
  //Serial.print("sizeof(MasOfAction) = ");
  //Serial.println(sizeof(MasOfAction));
  //Serial.print("sizeof(MasOfAction[0]) = ");
  //Serial.println(sizeof(MasOfAction[0]));

  for (int i = 0; i < sizeof(finalMas) / sizeof(finalMas[0]); i++) {

    if (finalMas[i][1] == 0)
      MoveToStart(finalMas[i][2], finalMas[i][3], finalMas[i][0]);
    else
      MoveToFinish(finalMas[i][2], finalMas[i][3], finalMas[i][0]);

    MasOfActionSteps[IndexOfActionSteps][4] = (finalMas[i][0]) ? ServZ[1] : ServZ[2];
    IndexOfActionSteps++;

    if (finalMas[i][4] == 0)
      MoveToStart(finalMas[i][5], finalMas[i][6], finalMas[i][0]);
    else
      MoveToFinish(finalMas[i][5], finalMas[i][6], finalMas[i][0]);

    MasOfActionSteps[IndexOfActionSteps][4] = ServZ[0];
    IndexOfActionSteps++;
  }
  ///////               (type, PlaceIn, xIn, yIn, PlaceOut, xOut, yOut)
}


void MoveToFinish(int Indx, int MMy, int T) {
  int angle = 23;
  switch (Indx) {
    case 0:
      MasOfActionSteps[IndexOfActionSteps][0] = rotate(180 - angle);
      MasOfActionSteps[IndexOfActionSteps][2] = ServX[3];
      MasOfActionSteps[IndexOfActionSteps][3] = ServY[1] + ServY[2];
      break;
    case 1:
      MasOfActionSteps[IndexOfActionSteps][0] = rotate(180);
      MasOfActionSteps[IndexOfActionSteps][2] = ServX[2];
      MasOfActionSteps[IndexOfActionSteps][3] = ServY[1];
      break;
    case 2:
      MasOfActionSteps[IndexOfActionSteps][0] = rotate(180 + angle);
      MasOfActionSteps[IndexOfActionSteps][2] = ServX[3];
      MasOfActionSteps[IndexOfActionSteps][3] = ServY[1] - ServY[2];
      break;
  }
  MasOfActionSteps[IndexOfActionSteps][1] = MoveMM(MMy + gap);
  if (T)
    MasOfActionSteps[IndexOfActionSteps][1] += MoveMM(CUBE_HEIGHT_B - CUBE_HEIGHT_S);
}


void MoveToStart(int Indx, int MMy, int T) {

  MasOfActionSteps[IndexOfActionSteps][0] = rotate(30 * Indx - 60);

  MasOfActionSteps[IndexOfActionSteps][1] = MoveMM(MMy + gap);
  if (T)
    MasOfActionSteps[IndexOfActionSteps][1] += MoveMM(CUBE_HEIGHT_B - CUBE_HEIGHT_S);
  MasOfActionSteps[IndexOfActionSteps][2] = ServX[1];
  MasOfActionSteps[IndexOfActionSteps][3] = ServY[1];
}


int rotate(int Angl) {
  if (Angl == 0 || Angl == 360 || Angl == -360)
    Angl = 0;

  return map(Angl, -359, 359, -stepPerTurn * turnPerBigTurn, stepPerTurn * turnPerBigTurn);
}


int MoveMM(int MM) {
  return MM * stepInmm;
}


void PrintArr(int (&SomeArr)[40][5]) {
  //Serial.println("11111111111111111111111111111");

  for (int i = 0; i < sizeof(SomeArr) / sizeof(SomeArr[0]); i++) {
    for (int j = 0; j < sizeof(SomeArr[0]) / sizeof(SomeArr[0][0]); j++) {
      Serial.print(int(SomeArr[i][j]));
      Serial.print("  ");
    }
    Serial.println();
  }
}


void PrintArr1(int (&SomeArr)[4][7]) {
  //Serial.println("22222222222222222222222222222");

  for (int i = 0; i < sizeof(SomeArr) / sizeof(SomeArr[0]); i++) {
    for (int j = 0; j < sizeof(SomeArr[0]) / sizeof(SomeArr[0][0]); j++) {
      //Serial.print(int(SomeArr[i][j]));
      //Serial.print("  ");
    }
    //Serial.println();
  }
}


//---------------END PROGGRAM---------------
