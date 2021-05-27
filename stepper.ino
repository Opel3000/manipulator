#include "GyverStepper.h"
#include <Servo.h>


#define STEPPER_EN_PIN 8
// The X Stepper pins
#define STEPPER1_DIR_PIN 5  ////////change pins!!!!!
#define STEPPER1_STEP_PIN 2
// The Y stepper pins
#define STEPPER2_DIR_PIN 6
#define STEPPER2_STEP_PIN 3

#define A_AXIS_LIMIT_SWITCH_PIN 9
#define Z_AXIS_LIMIT_SWITCH_PIN 10

#define SERVO_X 11
#define SERVO_Y 12
#define SERVO_Z 13

#define CUBE_HEIGHT_B 41
#define CUBE_HEIGHT_S 31


GStepper<STEPPER2WIRE> Astepper(200, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
GStepper<STEPPER2WIRE> Zstepper(200, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN, STEPPER_EN_PIN);

Servo ServoX;
Servo ServoY;
Servo ServoZ;

int ServX[4] = { 135, 45, 50, 90};
int ServY[3] = { 30, 90, 22};
const int gap = 5;
const int turnPerBigTurn = 8;
const int stepInmm = 60;
int IndexOfActionSteps = 0;

const int stepPerTurn = 200;
const int AAxisTurn = stepPerTurn * 8;
const int ZAxisTurn = stepPerTurn * 8;

int Stepper[4] = {0, 0, 0 ,0};

int InputArrPlan[3][4] = { { 1, 2, 0, 0 }, { 2, 4, 3, 0 }, { 5, 4, 0, 0 } };  ///////     X  Z

int InputArrFack[5][2] = { { 1, 2 }, { 2, 3 }, { 4, 3 }, { 5, 0 }, { 2, 4 } };  ///////     X  Z

int MasOfAction[4][7] = { {1, 1, 0, 40, 1, 2, 40},
                          {0, 0, 1, 70, 1, 0, 40},
                          {1, 1, 0, 40, 0, 1, 70},
                          {0, 0, 1, 70, 1, 0, 40}};         ///////     (type, PlaceIn, xIn, yIn, PlaceOut, xOut, yOut)

int MasOfActionSteps[8][5] = {  {0, 0, 0, 0, 0},            ///////     (Astepper, Zstepper, S1, S2, grab)
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0}};

int MainIndex = 0;

void setup() {
  Serial.begin(9600);

  pinMode(A_AXIS_LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(Z_AXIS_LIMIT_SWITCH_PIN, INPUT_PULLUP);

  ServoX.attach(SERVO_X);
  ServoY.attach(SERVO_Y);
  ServoZ.attach(SERVO_Z);
  
  Serial.println("/////////////////");
  PointG();
  Serial.println();
  PrintArr(MasOfActionSteps);
  PrintArr1(MasOfAction);
  Serial.println("/////////////////");
  GoHome();
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
    Zstepper.setTarget(MasOfActionSteps[MainIndex][1]);
    ServoX.write(MasOfActionSteps[MainIndex][2]);
    ServoY.write(MasOfActionSteps[MainIndex][3]);
    while (Astepper.getState() || Zstepper.getState() || millis() - tmr < 100) {
      Astepper.tick();
      Zstepper.tick();
    }

    Zstepper.setTarget(MasOfActionSteps[MainIndex][1] - CUBE_HEIGHT_B * stepInmm);
    while (Zstepper.tick())
      Zstepper.tick();

    static uint32_t Servtmr = millis();
      ServoZ.write(MasOfActionSteps[MainIndex][4]);
    while (millis() - Servtmr < 100)
      delay(1);

    Zstepper.setTarget(MasOfActionSteps[MainIndex][1]);
    while (Zstepper.tick())
      Zstepper.tick();

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
    Serial.println(Astepper.getCurrent());
  }*/
}

void GoHome() {/*
  Astepper.setRunMode(KEEP_SPEED);
  Astepper.setSpeed(-200);

  while (digitalRead(A_AXIS_LIMIT_SWITCH_PIN)) {
    Astepper.tick();
  }*/
  Astepper.reset();
  Astepper.setRunMode(FOLLOW_POS);
  Zstepper.reset();
  Zstepper.setRunMode(FOLLOW_POS);

  // delay(1000);
}

void PointG() {
  int ServZ[3] = { 0, 30, 50 };

  int (*a)[4];
  Serial.print("sizeof(MasOfAction) = ");
  Serial.println(sizeof(MasOfAction));
  Serial.print("sizeof(MasOfAction[0]) = ");
  Serial.println(sizeof(MasOfAction[0]));

  for (int i = 0; i < sizeof(MasOfAction) / sizeof(MasOfAction[0]); i++) {

    if (MasOfAction[i][1] == 0) 
      MoveToStart(MasOfAction[i][2], MasOfAction[i][3], MasOfAction[i][0]);
    else
      MoveToFinish(MasOfAction[i][2], MasOfAction[i][3], MasOfAction[i][0]);

    MasOfActionSteps[IndexOfActionSteps][4] = (MasOfAction[i][0]) ? ServZ[1] : ServZ[2];
    IndexOfActionSteps++;
    
    if (MasOfAction[i][4] == 0)
      MoveToStart(MasOfAction[i][5], MasOfAction[i][6], MasOfAction[i][0]);
    else
      MoveToFinish(MasOfAction[i][5], MasOfAction[i][6], MasOfAction[i][0]);

    MasOfActionSteps[IndexOfActionSteps][4] = ServZ[0];
    IndexOfActionSteps++;
  }
}

///////               (type, PlaceIn, xIn, yIn, PlaceOut, xOut, yOut)

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

void PrintArr(int (&SomeArr)[8][5]) {
  Serial.println("11111111111111111111111111111");

  for (int i = 0; i < sizeof(SomeArr) / sizeof(SomeArr[0]); i++) {
    for (int j = 0; j < sizeof(SomeArr[0]) / sizeof(SomeArr[0][0]); j++) {
      Serial.print(int(SomeArr[i][j]));
      Serial.print("  ");
    }
    Serial.println();
  }
}

void PrintArr1(int (&SomeArr)[4][7]) {
  Serial.println("22222222222222222222222222222");

  for (int i = 0; i < sizeof(SomeArr) / sizeof(SomeArr[0]); i++) {
    for (int j = 0; j < sizeof(SomeArr[0]) / sizeof(SomeArr[0][0]); j++) {
      Serial.print(int(SomeArr[i][j]));
      Serial.print("  ");
    }
    Serial.println();
  }
}
