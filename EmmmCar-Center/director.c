#include "director.h"
#include "bsp_ledbuz.h"
#include "bsp_fdetect.h"
#include "bsp_linefind.h"
#include "bsp_spi.h"
#include "systick.h"

/*
 * Director
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = The leader.
 */

void Director_Init(Director_ThisState *thisState, Director_PosState posState)
{
  thisState->lastState = POS_UNDEFINED;
  thisState->nowState = posState;
  thisState->nextState = POS_UNDEFINED;
  thisState->lastLoopSide = LOOP_RIGHT;
  thisState->steps = 0;
}

const static Director_PosState _route[] = {
    POS_START, // 0
    POS_BRANCH_3_1,
    POS_BRANCH_4_1,
    POS_TUNNEL_1,
    POS_ANGLES_RIGHT,

    POS_ANGLES_INSIDE, // 5
    POS_ANGLES_LEFT,
    POS_BRANCH_5_1,
    POS_BRANCH_4_3,
    POS_LOOP_TOP,

    POS_LOOP_INSIDE, // 10
    POS_LOOP_TOP,
    POS_BRANCH_1_3,
    POS_BRANCH_1_2,
    POS_BROKENROAD,

    POS_TUNNEL_2, // 15
    POS_BRANCH_3_3,
    POS_BRANCH_4_2,
    POS_BRANCH_4_3,
    POS_LOOP_TOP,

    POS_LOOP_INSIDE, // 20
    POS_LOOP_TOP,
    POS_BRANCH_1_3,
    POS_BRANCH_1_2,
    POS_BROKENROAD,

    POS_TUNNEL_2, // 25
    POS_BRANCH_3_3,
    POS_BRANCH_4_2,
    POS_BARRIERS_LEFT,
    POS_BARRIERS_INSIDE,

    POS_BARRIERS_RIGHT, // 30
    POS_BRANCH_2_1,
    POS_VISITSITE,
    POS_BRANCH_2_1,
    POS_BRANCH_2_2,

    POS_PARKING_1, // 35
    POS_BRANCH_2_3,
    POS_PARKING_2,
    POS_BRANCH_2_4,
    POS_PARKING_3,

    POS_BRANCH_2_5, // 40
    POS_TUNNEL_3,
    POS_BRANCH_1_1,
    POS_SPECIALZONE,
    POS_BRANCH_1_1,

    POS_REDUCER, // 45
    POS_START};

void _Director_PosShift(Director_ThisState *thisState, Director_PosState targetState)
{
  thisState->lastState = thisState->nowState;
  thisState->nowState = thisState->nextState;
  thisState->nextState = targetState;
}

void _Director_NormalLineFollow()
{
  BSP_LineFind_Result lineRes = BSP_Linefind_Read();
  if (lineRes.R3 == 1 && lineRes.R2 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 600, 0);
    BSP_SPI_SetMotorPWM(1, 350, 0);
    BSP_SPI_SetMotorPWM(2, 600, 0);
    BSP_SPI_SetMotorPWM(3, 350, 0);
  }
  else if (lineRes.L3 == 1 && lineRes.L2 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 350, 0);
    BSP_SPI_SetMotorPWM(1, 600, 0);
    BSP_SPI_SetMotorPWM(2, 350, 0);
    BSP_SPI_SetMotorPWM(3, 600, 0);
  }
  else if (lineRes.R2 == 1 && lineRes.R1 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 550, 0);
    BSP_SPI_SetMotorPWM(1, 400, 0);
    BSP_SPI_SetMotorPWM(2, 550, 0);
    BSP_SPI_SetMotorPWM(3, 400, 0);
  }
  else if (lineRes.L1 == 1 && lineRes.L2 == 1)
  {

    BSP_SPI_SetMotorPWM(0, 400, 0);
    BSP_SPI_SetMotorPWM(1, 550, 0);
    BSP_SPI_SetMotorPWM(2, 400, 0);
    BSP_SPI_SetMotorPWM(3, 550, 0);
  }
  else if (lineRes.R4 == 1 && lineRes.R3 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 450, 0);
    BSP_SPI_SetMotorPWM(1, 450, 1);
    BSP_SPI_SetMotorPWM(2, 450, 0);
    BSP_SPI_SetMotorPWM(3, 450, 1);
  }
  else if (lineRes.L4 == 1 && lineRes.L3 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 450, 1);
    BSP_SPI_SetMotorPWM(1, 450, 0);
    BSP_SPI_SetMotorPWM(2, 450, 1);
    BSP_SPI_SetMotorPWM(3, 450, 0);
  }
  else
  {

    BSP_SPI_SetMotorPWM(0, 500, 0);
    BSP_SPI_SetMotorPWM(1, 500, 0);
    BSP_SPI_SetMotorPWM(2, 500, 0);
    BSP_SPI_SetMotorPWM(3, 500, 0);
  }
  delay_1ms(10);
}

void _Director_AggressiveLineFollow()
{
  BSP_LineFind_Result lineRes = BSP_Linefind_Read();
  if (lineRes.R4 == 1 && lineRes.R3 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 600, 0);
    BSP_SPI_SetMotorPWM(1, 400, 1);
    BSP_SPI_SetMotorPWM(2, 600, 0);
    BSP_SPI_SetMotorPWM(3, 400, 1);
  }
  else if (lineRes.L4 == 1 && lineRes.L3 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 400, 1);
    BSP_SPI_SetMotorPWM(1, 600, 0);
    BSP_SPI_SetMotorPWM(2, 400, 1);
    BSP_SPI_SetMotorPWM(3, 600, 0);
  }
  else if (lineRes.R3 == 1 && lineRes.R2 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 600, 0);
    BSP_SPI_SetMotorPWM(1, 250, 0);
    BSP_SPI_SetMotorPWM(2, 600, 0);
    BSP_SPI_SetMotorPWM(3, 250, 0);
  }
  else if (lineRes.L3 == 1 && lineRes.L2 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 250, 0);
    BSP_SPI_SetMotorPWM(1, 600, 0);
    BSP_SPI_SetMotorPWM(2, 250, 0);
    BSP_SPI_SetMotorPWM(3, 600, 0);
  }
  else if (lineRes.R2 == 1 && lineRes.R1 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 500, 0);
    BSP_SPI_SetMotorPWM(1, 300, 0);
    BSP_SPI_SetMotorPWM(2, 500, 0);
    BSP_SPI_SetMotorPWM(3, 300, 0);
  }
  else if (lineRes.L1 == 1 && lineRes.L2 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 300, 0);
    BSP_SPI_SetMotorPWM(1, 500, 0);
    BSP_SPI_SetMotorPWM(2, 300, 0);
    BSP_SPI_SetMotorPWM(3, 500, 0);
  }
  else
  {
    BSP_SPI_SetMotorPWM(0, 400, 0);
    BSP_SPI_SetMotorPWM(1, 400, 0);
    BSP_SPI_SetMotorPWM(2, 400, 0);
    BSP_SPI_SetMotorPWM(3, 400, 0);
  }
}

void Director_Loop(Director_ThisState *thisState)
{
  switch (thisState->steps)
  {
  case 0:

    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  default:
    while (1)
    {
      BSP_LEDBUZ_Flash(BSP_LEDBUZ_LED, 500, 100);
    }
  }
}
