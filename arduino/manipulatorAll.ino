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

byte bly;
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

byte inputMas[5][2] = {{5, 9}, {4, 11}, {2, 10}, {1, 8}, {3, 7}};
byte outputMas[3][3] = {{4, 1, 3}, {5, 2, 1}, {4, 0, 0}};
byte outputMasMy[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

//---------------CONST MANIPULATOR---------------

#define STEPPER_EN_PIN 8
// The X Stepper pins
#define STEPPER1_DIR_PIN 5
#define STEPPER1_STEP_PIN 2
// The Y stepper pins
#define STEPPER2_DIR_PIN 6
#define STEPPER2_STEP_PIN 3

#define A_AXIS_LIMIT_SWITCH_PIN A0
#define Z_AXIS_LIMIT_SWITCH_PIN 9

#define SERVO_X   12
#define SERVO_Y   7
#define SERVO_Z   4
#define SERVO_ZZ  13

#define MUSIC_PIN 11

#define CUBE_HEIGHT_B 41
#define CUBE_HEIGHT_S 31

GStepper<STEPPER2WIRE> Astepper(200, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
GStepper<STEPPER2WIRE> Zstepper(200, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN, STEPPER_EN_PIN);

Servo ServoX;
Servo ServoY;
Servo ServoZ;
Servo ServoZZ;

int del = 20;
int ServX[4] = {150, 93, 90, 45};
int ServY[3] = {130, 95, 22};
int ServZ[3] = {120, 70, 40};
int ServZZ[2] = {70, 50};

const int gap = 5;
const int turnPerBigTurn = 8;
const int stepInmm = 6;
const int halfTurn = 200 * 4;

const int stepPerTurn = 200;

int Stepper[4] = {0, 0, 0 , 0};

int MasOfActionSteps[40][5];                            ///////     (Astepper, Zstepper, S1, S2, grab)

int IndexOfActionSteps = 0;
int MainIndex = 0;
int arrStepsHeight[40];
bool F =  1;
const bool bigFlag = 1;

//---------------END CONST---------------


void setup() {
  Serial.begin(115200);
//  Serial.println("============================================");

  pinMode(A_AXIS_LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(Z_AXIS_LIMIT_SWITCH_PIN, INPUT_PULLUP);

  pinMode(MUSIC_PIN, OUTPUT);

  ServoX.attach(SERVO_X);
  ServoY.attach(SERVO_Y);
  ServoZ.attach(SERVO_Z);
  ServoZZ.attach(SERVO_ZZ);
  GoHome();

  ServoZ.write(ServZ[0]);

  Astepper.setMaxSpeed(1000);
  Astepper.setAcceleration(4000);

  Zstepper.setMaxSpeed(800);
  Zstepper.setAcceleration(6000);

  Zstepper.setTarget(1230 - 13 * stepInmm);
  while (Zstepper.getState())
    Zstepper.tick();


  Astepper.setTarget(-200 * 3 - 25 + rotate(90));
  while (Astepper.getState())
    Astepper.tick();


  //  Serial.print("g");
  WaitPl();

   Zstepper.setTarget(MoveMM(72));
  while (Zstepper.getState())
    Zstepper.tick();
    

  Astepper.setTarget(rotate(90));
  while (Astepper.getState())
    Astepper.tick();

    
  ZeroFunktion();
  lord_of_the_builders_two_arrays();
  finalMas[countStep][0] = 99;
  Fix();
  PointG();
//  if(!bigFlag) {
//    Serial.println("/////////////////");
//    for (int indexSer = 0; indexSer < 40; indexSer++) {
//      Serial.print(indexSer);
//      Serial.print("  ");
//      Serial.print(int(arrStepsHeight[indexSer]));
//      Serial.println();
//    }
//  
//    Serial.println("/////////////////");
//    for (int i = 0; i < 19; i++) {
//      for (int j = 0; j < 7; j++) {
//        Serial.print(int(finalMas[i][j]));
//        Serial.print("  ");
//      }
//      Serial.println();
//    }
//    Serial.println("/////////////////");
//    PrintArr(MasOfActionSteps);
//  }
  
  Zstepper.setTarget(120 * stepInmm);
  Astepper.setTarget(0);
  while (Zstepper.getState() || Astepper.getState()) {
    Zstepper.tick();
    Astepper.tick();
  }
  //  PrintArr(MasOfActionSteps);
  /*
    Serial.println("/////////////////");
    for (int i = 0; i < 19; i++) {
      for (int j = 0; j < 7; j++) {
        Serial.print(int(finalMas[i][j]));
        Serial.print("  ");
      }
      Serial.println();
    }
    Serial.println("/////////////////");
    PrintArr(MasOfActionSteps);
  */
  //Serial.println();
  // delay(1000);

}

void loop() {
  if (MasOfActionSteps[MainIndex][0] != -1) {
    static uint32_t tmr = millis();
    if (MasOfActionSteps[MainIndex][3] == ServY[1] and MasOfActionSteps[MainIndex][4] != ServZ[0])
      ServoX.write(ServX[0]);
    else
      ServoX.write(MasOfActionSteps[MainIndex][2]);
    if(MasOfActionSteps[MainIndex][3] == ServY[0]) {
      ServoX.write(190);
      ServoZ.write(20);
      Astepper.setTarget(MasOfActionSteps[MainIndex][0] - 30);
    }
    ServoY.write(MasOfActionSteps[MainIndex][3]);
    if(MasOfActionSteps[MainIndex][3] != ServY[0])
      Astepper.setTarget(MasOfActionSteps[MainIndex][0]);
    while (Astepper.getState()) {
      Astepper.tick();
    }
    Zstepper.setTarget(MasOfActionSteps[MainIndex][1] - CUBE_HEIGHT_S * stepInmm);
    while (Zstepper.getState()) {
      Zstepper.tick();
    }
    //    Zstepper.setTarget(MasOfActionSteps[MainIndex][1]);
    //    while (Zstepper.getState() || millis() - tmr < 100)
    //      Zstepper.tick();
    //        delay(500);

    if (MasOfActionSteps[MainIndex][3] == ServY[1] and MasOfActionSteps[MainIndex][4] != ServZ[0])
      ServoX.write(MasOfActionSteps[MainIndex][2]);
    if(MasOfActionSteps[MainIndex][3] == ServY[0])
      ServoX.write(MasOfActionSteps[MainIndex][2]);
    delay(del);
//    for (int i = MasOfActionSteps[MainIndex - 1][4]; i > ServZ[0]; i++) {
//      ServoZ.write(i);
//      delay(10);
//    }
    ServoZ.write(ServZ[0]);
    for (int i = ServZ[0]; i > MasOfActionSteps[MainIndex][4]; i--) {
      ServoZ.write(i);
      delay(10);
    }
    
    if(MasOfActionSteps[MainIndex][3] == ServY[0])
      ServoZ.write(ServZ[0]);
      
    //    int pos = 0;
    //    for (pos = ServZ[0]; pos <= MasOfActionSteps[MainIndex][4]; pos -= 1) {
    //      ServoZ.write(pos);
    //      delay(10);
    //    }
    /*
      static uint32_t Servtmr = millis();
      ServoZ.write(MasOfActionSteps[MainIndex][4]);
      while (millis() - Servtmr < 100)
      delay(1);*/
    delay(del);
    Zstepper.setTarget(MoveMM(arrStepsHeight[MainIndex]) + (CUBE_HEIGHT_S + 10) * stepInmm);
    while (Zstepper.getState())
      Zstepper.tick();
    delay(del);
    MainIndex++;
    //    delay(500);
  }
  else if (F) {
    Zstepper.setTarget(1230 - 9 * stepInmm);
    while (Zstepper.getState()) {
      Zstepper.tick();
    }
    delay(del);
    // Astepper.setTarget(200 * 5 - 20);

    Astepper.setTarget(relativelyPos(Astepper.getCurrent(), 200 * 7 - 20));
    while (Astepper.getState()) {
      Astepper.tick();
    }
    ServoX.write(ServX[0]);
    ServoY.write(ServY[0]);
    ServoZ.write(ServZ[2]);
    ServoZZ.write(ServZZ[0]);
    delay(del);
    Pisk(500);
    F = 0;
  }
}

//---------------FUNCTION LOGIC---------------


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

//  Serial.print("input ");
//  Serial.println(inputMas[0][0]);
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
//  Serial.print("i1 ");
//  Serial.println(inputMas[numInPlaseB][floorTwo]);
//  Serial.print("numInPlaseB ");
//  Serial.println(numInPlaseB);
//  Serial.print("floorTwo ");
//  Serial.println(floorTwo);
//  Serial.print("i2 ");
//  Serial.println(inputMas[numInPlaseA][floorOne]);
//  Serial.print("numInPlaseA ");
//  Serial.println(numInPlaseA);
//  Serial.print("floorOne ");
//  Serial.println(floorOne);
//
//  Serial.print("input ");
//  Serial.println(inputMas[0][0]);
//  Serial.println();

}

byte MMInFloor(byte hight) {
  if (hight == 0)
    return 0;
  else if (hight < 60)
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
            inputMas[0][0] = typeE;
            inputMas[0][1] = typeW + 6;
            break;
          case 2:
            inputMas[1][0] = typeE;
            inputMas[1][1] = typeW + 6;
            break;
          case 3:
            inputMas[2][0] = typeE;
            inputMas[2][1] = typeW + 6;
            break;
          case 4:
            inputMas[3][0] = typeE;
            inputMas[3][1] = typeW + 6;
            break;
          case 5:
            inputMas[4][0] = typeE;
            inputMas[4][1] = typeW + 6;
            break;
        }
        break;
      }
    }
  }
}

void lord_of_the_builders_two_arrays() {
//  byte tmr = 0;

  search_spiers();
  bly = inputMas[0][0];
  loxSpier();
  inputMas[0][0] = bly;
  while (!workQuestion()) {
//    if(tmr > 40)
//      break;
//    tmr++;
    if (!funcWork())
      nextStep();




//    Serial.println("/////////////////");
//    for (int i = 0; i < 5; i++) {
//      for (int j = 0; j < 2; j++) {
//        Serial.print(int(inputMas[i][j]));
//        Serial.print("  ");
//      }
//      Serial.println();
//    }
//
//    Serial.println("/////////////////");
//    for (int i = 0; i < 3; i++) {
//      for (int j = 0; j < 3; j++) {
//        Serial.print(int(outputMasMy[i][j]));
//        Serial.print("  ");
//      }
//      Serial.println();
//    }
//    Serial.println("/////////////////");
//    for (int i = 0; i < 3; i++) {
//      for (int j = 0; j < 3; j++) {
//        Serial.print(int(outputMas[i][j]));
//        Serial.print("  ");
//      }
//      Serial.println();
//    }
  }
}

bool checkKub(byte indexMain) {
  if (inputMas[indexMain][1] != 0)
    return false;
  else
    return true;
}

void loxSpier() {
  for (byte i = 0; i <= 4; i++) {
    if (inputMas[i][1] != spiersMas[0] and inputMas[i][1] != spiersMas[1] and inputMas[i][1] != spiersMas[2]) {
      for (byte j = 0; j <= 2; j++) {
        array_generator(3, 0, i, CUBE_HIGHT_B, 0, 0, 0);
        inputMas[i][1] = 0;
        break;
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
    int visot = 0;
    for (byte k = 0; k <= 2; k++) {
      if(outputMas[k][i] == outputMasMy[k][i])
        visot++;
      else
        break;
    }
//    Serial.println(i);
//    Serial.println(visot);
    if(visot == 3)
      continue;
    for (byte j = 0; j <= 4; j++) {
      if (outputMas[visot][i] != outputMasMy[visot][i]) {
        if (outputMas[visot][i] == inputMas[j][0]) {
          if (!checkKub(j)) {
            kubStrong = j;
//            Serial.print("kubStrong ");
//            Serial.println(kubStrong);
//            continue;
          }
          else {
            if (outputMas[0][i] > 6)
              array_generator(1, 0, j, 0, 1, i, visot * CUBE_HIGHT_B);
            else
              array_generator(0, 0, j, 0, 1, i, visot * CUBE_HIGHT_B);
//            Serial.println("true");
            return true;
          }
        }
        else if (outputMas[visot][i] == inputMas[j][1]) {
            array_generator(1, 0, j, CUBE_HIGHT_B, 1, i, visot * CUBE_HIGHT_B);
//            Serial.println("true");
            return true;
        }
      }
    }
  }
//  Serial.println("false");
  return false;
}


byte nextStep() {
  for (byte i = 0; i <= 4; i++) {
    if (inputMas[i][1] == 0 and inputMas[i][0] != 0 and inputMas[i][0] < 7) {
      array_generator(1, 0, kubStrong, CUBE_HIGHT_B, 0, i, CUBE_HIGHT_B);
      return 0;
    }
  }
  for (byte i = 0; i <= 4; i++) {
    if (inputMas[i][0] == 0) {
      array_generator(1, 0, kubStrong, CUBE_HIGHT_B, 0, i, 0);
      return 0;
    }
  }
}


//---------------FUNCTION MANIPULATOR---------------

void Fix() {
  int i = 0;
  while (finalMas[i][0] != 99) {
    if (finalMas[i][0] == 0) {
      finalMas[i][3] += CUBE_HEIGHT_B;
      finalMas[i][6] += CUBE_HEIGHT_B;
    }
    else if (finalMas[i][0] == 1 || finalMas[i][0] == 2 || finalMas[i][0] == 3) {
      finalMas[i][3] += CUBE_HEIGHT_S;
      finalMas[i][6] += CUBE_HEIGHT_S;
    }
    i++;
  }
}

void GoHome() {
  int porog = 680;
  Astepper.setRunMode(KEEP_SPEED);
  Astepper.setSpeed(-100);

  Zstepper.setRunMode(KEEP_SPEED);
  Zstepper.setSpeed(400);
  while (digitalRead(Z_AXIS_LIMIT_SWITCH_PIN))
    Zstepper.tick();
  Zstepper.reset();
  Zstepper.setRunMode(FOLLOW_POS);
  Zstepper.setTarget(-80);
  while (Zstepper.getState())
    Zstepper.tick();
  Zstepper.setRunMode(KEEP_SPEED);
  Zstepper.setSpeed(100);
  while (digitalRead(Z_AXIS_LIMIT_SWITCH_PIN))
    Zstepper.tick();
  Zstepper.reset();

  Astepper.reset();
  Astepper.setRunMode(FOLLOW_POS);
  Astepper.setTarget(50);
  while (Astepper.getState())
    Astepper.tick();
  Astepper.setRunMode(KEEP_SPEED);
  Astepper.setSpeed(-400);
  while (analogRead(A_AXIS_LIMIT_SWITCH_PIN) > porog)
    Astepper.tick();
  Astepper.reset();
  Astepper.setRunMode(FOLLOW_POS);
  Astepper.setTarget(200);
  while (Astepper.getState())
    Astepper.tick();
  Astepper.setRunMode(KEEP_SPEED);
  Astepper.setSpeed(-150);
  while (analogRead(A_AXIS_LIMIT_SWITCH_PIN) > porog)
    Astepper.tick();
  Astepper.reset();

  Astepper.setRunMode(FOLLOW_POS);
  Zstepper.setRunMode(FOLLOW_POS);
  Astepper.setCurrent(-200 + 20);
  Zstepper.setCurrent(1230 - 9 * stepInmm);

  ServoX.write(ServX[0]);
  ServoY.write(ServY[0]);
  ServoZ.write(ServZ[0]);
  ServoZZ.write(ServZZ[0]);
}

void ZeroFunktion() {
  int angleBly = -1;

  Astepper.setTarget(-50 + rotate(90 + angleBly));
  Zstepper.setTarget(530);
  ServoZZ.write(ServZZ[0]);
  while (Astepper.getState() || Zstepper.getState()) {
    Astepper.tick();
    Zstepper.tick();
  }
  if(bigFlag)
    lord_of_the_builders_brotherhood_array();
  else
    WaitPl();

  ServoX.write(ServX[0]);
  ServoY.write(ServY[1]);
  ServoZ.write(ServZ[0]);

  ServoZZ.write(ServZZ[1]);
  Zstepper.setTarget(200);
  while (Zstepper.getState())
    Zstepper.tick();
  
  delay(100);
  for (int i = 0; i < 5; i++) {
    Astepper.setTarget(rotate(180 - 70 + 30 * i) + rotate(90 + angleBly));
    while (Astepper.getState())
      Astepper.tick();
    if(bigFlag)
      lord_of_the_builders_brotherhood_array();
    else
      WaitPl();
  }

  Zstepper.setTarget(600);
  while (Zstepper.getState())
    Zstepper.tick();

}

void PointG() {
  int arrHeight[40][8];
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
  arrHeight[0][5] = 0;
  arrHeight[0][6] = 0;
  arrHeight[0][7] = 0;
  int i = 0;
  while (finalMas[i][0] != 99) {
    if (finalMas[i][1] == 0)
      MoveToStart(finalMas[i][2], finalMas[i][3], finalMas[i][0]);
    else
      MoveToFinish(finalMas[i][2], finalMas[i][3], finalMas[i][0]);
    MasOfActionSteps[IndexOfActionSteps][4] = (finalMas[i][0] == 0) ? ServZ[1] : ServZ[2];
    if(finalMas[i][0] == 3) {
//      MasOfActionSteps[IndexOfActionSteps][1] -= MoveMM(CUBE_HEIGHT_B);
      MasOfActionSteps[IndexOfActionSteps][4] = ServZ[2];
      MasOfActionSteps[IndexOfActionSteps][3] = ServY[0];
    }
    
    int indexArrHeight = finalMas[i][2];
    if (finalMas[i][1])
      indexArrHeight += 5;
    indexArrHeight = Fix2(indexArrHeight);
    arrHeight[IndexOfActionSteps][indexArrHeight] -= (finalMas[i][0] == 0) ? CUBE_HEIGHT_B : CUBE_HEIGHT_S;
    arrHeight[IndexOfActionSteps][indexArrHeight] -= (finalMas[i][0] == 2) ? CUBE_HEIGHT_S : 0;
    for (int a = 0; a < 8; a++)
      arrHeight[IndexOfActionSteps + 1][a] = arrHeight[IndexOfActionSteps][a];
    IndexOfActionSteps++;
    if(finalMas[i][0] != 3) {
      if (finalMas[i][4] == 0)
        MoveToStart(finalMas[i][5], finalMas[i][6], finalMas[i][0]);
      else
        MoveToFinish(finalMas[i][5], finalMas[i][6], finalMas[i][0]);
      MasOfActionSteps[IndexOfActionSteps][4] = ServZ[0];
      indexArrHeight = finalMas[i][5];
      if (finalMas[i][4])
        indexArrHeight += 5;
      indexArrHeight = Fix2(indexArrHeight);
      arrHeight[IndexOfActionSteps][indexArrHeight] += (finalMas[i][0] == 0) ? CUBE_HEIGHT_B : CUBE_HEIGHT_S;
      arrHeight[IndexOfActionSteps][indexArrHeight] += (finalMas[i][0] == 2) ? CUBE_HEIGHT_S : 0;
      for (int a = 0; a < 8; a++)
        arrHeight[IndexOfActionSteps + 1][a] = arrHeight[IndexOfActionSteps][a];
      IndexOfActionSteps++;
    }
    i++;
  }
  MasOfActionSteps[IndexOfActionSteps][0] = -1;
  ///////               (type, PlaceIn, xIn, yIn, PlaceOut, xOut, yOut)
//  Fix1(arrHeight);
  IndexOfActionSteps = 0;
  i = 0;
  while (finalMas[i][0] != 99) {
    int fin = Fix2(finalMas[i][2] + ((finalMas[i][1]) ? 5 : 0));
    int fout = Fix2(finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0));
    if(MasOfActionSteps[IndexOfActionSteps][0] > MasOfActionSteps[IndexOfActionSteps + 1][0])
      arrStepsHeight[IndexOfActionSteps] = MaxHeight(arrHeight, fin, fout, IndexOfActionSteps, 0);
    else
      arrStepsHeight[IndexOfActionSteps] = MaxHeight(arrHeight, fin, fout, IndexOfActionSteps, 1);
    IndexOfActionSteps++;
    fin = Fix2(finalMas[i][5] + ((finalMas[i][4]) ? 5 : 0));
    fout = Fix2(finalMas[i + 1][2] + ((finalMas[i + 1][1]) ? 5 : 0));
    if(MasOfActionSteps[IndexOfActionSteps][0] > MasOfActionSteps[IndexOfActionSteps + 1][0])
      arrStepsHeight[IndexOfActionSteps] = MaxHeight(arrHeight, fin, fout, IndexOfActionSteps, 0);
    else
      arrStepsHeight[IndexOfActionSteps] = MaxHeight(arrHeight, fin, fout, IndexOfActionSteps, 1);
    IndexOfActionSteps++;
    i++;
  }
  
//  Serial.println("/////////////////");
//  for (int i = 0; i < 40; i++) {
//    Serial.print(i);
//    Serial.print("  ");
//    for (int j = 0; j < 8; j++) {
//      Serial.print(int(arrHeight[i][j]));
//      Serial.print("  ");
//    }
//    Serial.println();
//  }
}

int Fix2(int a) {
  if(a == 7)
    a = 5;
  else if(a == 5)
    a = 7;
  return a;
}

int MaxHeight(int (&SomeArr)[40][8], int a, int b, int indexS, bool F) {
  int i = a;
  int m = 0;
  
  m = SomeArr[indexS][i];
    
  do {
    if(F)
      i++;
    else
      i--;
      
    if(i == -1)
      i = 9;
    if(i == 10)
      i = 0;
      
    if(SomeArr[indexS][i] >= m)
      m = SomeArr[indexS][i];
  } while(i != b);
  return m;
}

/*
int MaxHeight(int (&SomeArr)[40][8], int a, int b, int indexS) {
  int i = a;
  int m = 0;
  if(a < b)
    b++;
  else
    b--;
  do {
    if(SomeArr[indexS][i] >= m)
      m = SomeArr[indexS][i];
    if(a < b)
      i++;
    else
      i--;
  } while(i != b);
  return m;
}
*/

void MoveToFinish(int Indx, int MMy, int T) {
  int deltaAngle = 24;
  int smeshenie = 273;
  int angle;
  switch (Indx) {
    case 0:
      angle = rotate(smeshenie + deltaAngle);
      MasOfActionSteps[IndexOfActionSteps][2] = ServX[3];
      MasOfActionSteps[IndexOfActionSteps][3] = ServY[1] + ServY[2];
      break;
    case 1:
      angle = rotate(smeshenie - 5);
      MasOfActionSteps[IndexOfActionSteps][2] = ServX[2] + 10;
      MasOfActionSteps[IndexOfActionSteps][3] = ServY[1];
      break;
    case 2:
      angle = rotate(smeshenie - deltaAngle - 8);
      MasOfActionSteps[IndexOfActionSteps][2] = ServX[3] + 15;
      MasOfActionSteps[IndexOfActionSteps][3] = ServY[1] - ServY[2] - 3;
      break;
  }

  if(IndexOfActionSteps > 0)
    MasOfActionSteps[IndexOfActionSteps][0] = relativelyPos(MasOfActionSteps[IndexOfActionSteps - 1][0], angle);
  else
    MasOfActionSteps[IndexOfActionSteps][0] = angle;

  MasOfActionSteps[IndexOfActionSteps][1] = MoveMM(MMy + gap);
  if (T == 0)
    MasOfActionSteps[IndexOfActionSteps][1] -= MoveMM(CUBE_HEIGHT_B - CUBE_HEIGHT_S);
}

void MoveToStart(int Indx, int MMy, int T) {
  int angle = rotate((30 * Indx + 30)) - 8;
  
  if(IndexOfActionSteps > 0)
    MasOfActionSteps[IndexOfActionSteps][0] = relativelyPos(MasOfActionSteps[IndexOfActionSteps - 1][0], angle);
  else
    MasOfActionSteps[IndexOfActionSteps][0] = angle;

  MasOfActionSteps[IndexOfActionSteps][1] = MoveMM(MMy + gap);
  if (T == 0)
    MasOfActionSteps[IndexOfActionSteps][1] -= MoveMM(CUBE_HEIGHT_B - CUBE_HEIGHT_S);
    
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

int relativelyPos(int start, int finish) {
  int deltaStart = relativelyAngle(start);
  if(finish > halfTurn)
    finish = revers(finish);
  finish -= deltaStart;
  if(abs(revers(finish)) < abs(finish))
    finish = revers(finish);
  return start + finish;
}

int revers(int stp) {
  int all = stepPerTurn * turnPerBigTurn;
  if(stp > 0)
    return -all + stp;
  else
    return all + stp;
}

int relativelyAngle(int stp) {
  return stp % (stepPerTurn * turnPerBigTurn);
}

void Pisk(int T) {
  tone(MUSIC_PIN, 1920);
  delay(T);
  noTone(MUSIC_PIN);
}

void WaitPl() {
  while (digitalRead(Z_AXIS_LIMIT_SWITCH_PIN))
    delay(1);
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
      Serial.print(int(SomeArr[i][j]));
      Serial.print("  ");
    }
    Serial.println();
  }
}

//---------------END PROGGRAM---------------
