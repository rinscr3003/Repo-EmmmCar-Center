#include "director.h"
#include "bsp_ledbuz.h"
#include "bsp_fdetect.h"
#include "bsp_linefind.h"
#include "bsp_spi.h"
#include "systick.h"
#include "stdint.h"

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
    POS_LOOP_BOTTOM,
    POS_BRANCH_1_3,
    POS_BRANCH_1_2,
    POS_BROKENROAD,

    POS_TUNNEL_2, // 15
    POS_BRANCH_3_3,
    POS_BRANCH_4_2,
    POS_BRANCH_4_3,
    POS_LOOP_TOP,

    POS_LOOP_INSIDE, // 20
    POS_LOOP_BOTTOM,
    POS_BRANCH_1_3,
    POS_BRANCH_1_2,
    POS_BROKENROAD,

    POS_TUNNEL_2, // 25
    POS_BRANCH_3_3,
    POS_BRANCH_4_2,
    POS_BARRIERS_LEFT,
    POS_BARRIERS_INSIDE,

    POS_BARRIERS_RIGHT, // 30
    POS_BRANCH_4_1,
    POS_BRANCH_3_1,
    POS_BRANCH_3_2,
    POS_BRANCH_2_1,

    POS_VISITSITE, // 35
    POS_BRANCH_2_1,
    POS_BRANCH_2_2,
    POS_PARKING_1,
    POS_BRANCH_2_3,

    POS_PARKING_2, // 40
    POS_BRANCH_2_4,
    POS_PARKING_3,
    POS_BRANCH_2_5,
    POS_TUNNEL_3,

    POS_BRANCH_1_1, // 45
    POS_SPECIALZONE,
    POS_BRANCH_1_1,
    POS_REDUCER,
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

void _Director_LoopLineFollow()
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
    BSP_SPI_SetMotorPWM(0, 650, 0);
    BSP_SPI_SetMotorPWM(1, 200, 1);
    BSP_SPI_SetMotorPWM(2, 650, 0);
    BSP_SPI_SetMotorPWM(3, 200, 1);
  }
  else if (lineRes.L3 == 1 && lineRes.L2 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 200, 1);
    BSP_SPI_SetMotorPWM(1, 650, 0);
    BSP_SPI_SetMotorPWM(2, 200, 1);
    BSP_SPI_SetMotorPWM(3, 650, 0);
  }
  else if (lineRes.R2 == 1 && lineRes.R1 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 650, 0);
    BSP_SPI_SetMotorPWM(1, 150, 0);
    BSP_SPI_SetMotorPWM(2, 650, 0);
    BSP_SPI_SetMotorPWM(3, 150, 0);
  }
  else if (lineRes.L1 == 1 && lineRes.L2 == 1)
  {
    BSP_SPI_SetMotorPWM(0, 150, 0);
    BSP_SPI_SetMotorPWM(1, 650, 0);
    BSP_SPI_SetMotorPWM(2, 150, 0);
    BSP_SPI_SetMotorPWM(3, 650, 0);
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
  BSP_LineFind_Result lineRes;
  static uint32_t _c0_timestamp = 0;
  switch (thisState->steps)
  {

    /*
    POS_START, // 0
    POS_BRANCH_3_1,
    POS_BRANCH_4_1,
    POS_TUNNEL_1,
    POS_ANGLES_RIGHT,
    */
  case 0:
  {
    _Director_AggressiveLineFollow();
    if (BSP_FDetect_Read())
    {
      if (_c0_timestamp == 0)
        _c0_timestamp = getSysPeriod();
    }
    if (_c0_timestamp != 0 && (getSysPeriod() - _c0_timestamp) >= 550)
    {
      BSP_SPI_AllBrake();
      BSP_LEDBUZ_Flash(BSP_LEDBUZ_LED, 500, 2);
      BSP_LEDBUZ_Flash(BSP_LEDBUZ_BUZ, 1000, 1);
      delay_1ms(100);
      _Director_PosShift(thisState, _route[thisState->steps + 1]);
      thisState->steps++;
    }
  }
  break;

  case 1:
  case 2:
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 3:
    BSP_SPI_SetMotorPWM(0, 400, 0);
    BSP_SPI_SetMotorPWM(1, 400, 0);
    BSP_SPI_SetMotorPWM(2, 400, 0);
    BSP_SPI_SetMotorPWM(3, 400, 0);
    while (1)
    {
      lineRes = BSP_Linefind_Read();
      if (lineRes.L4 == 1 && lineRes.L3 == 1)
      {
        break;
      }
    }
    BSP_FDetect_SetAngle(55);
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 550, 1);
      BSP_SPI_SetMotorPWM(1, 550, 0);
      BSP_SPI_SetMotorPWM(2, 550, 1);
      BSP_SPI_SetMotorPWM(3, 550, 0);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 500 && lineRes.L2 == 1 && lineRes.L1 == 1 && lineRes.R1 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 4)
      {
        break;
      }
    }
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 4:
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

    /*
    POS_ANGLES_INSIDE, // 5
    POS_ANGLES_LEFT,
    POS_BRANCH_5_1,
    POS_BRANCH_4_3,
    POS_LOOP_TOP,
    */

  case 5:
    BSP_SPI_AllBrake();
    uint8_t _barrier3 = 0;
    for (int8_t i = 55; i >= 20; i--)
    {
      BSP_FDetect_SetAngle(i);
      if (BSP_FDetect_Read())
      {
        _barrier3 = 1;
        break;
      }
      delay_1ms(10);
    }
    BSP_FDetect_SetAngle(0);
    if (_barrier3)
    {
      // Barrier 3 exists
      BSP_FDetect_SetAngle(-45);
      _c0_timestamp = getSysPeriod();
      while (1)
      {
        BSP_SPI_SetMotorPWM(0, 550, 1);
        BSP_SPI_SetMotorPWM(1, 550, 0);
        BSP_SPI_SetMotorPWM(2, 550, 1);
        BSP_SPI_SetMotorPWM(3, 550, 0);
        if (getSysPeriod() - _c0_timestamp >= 500)
        {
          break;
        }
      }
      while (1)
      {
        _Director_AggressiveLineFollow();
        lineRes = BSP_Linefind_Read();
        if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 4)
        {
          break;
        }
      }
      BSP_SPI_AllBrake();
      BSP_FDetect_SetAngle(-15);
      while (1)
      {
        BSP_SPI_SetMotorPWM(0, 550, 0);
        BSP_SPI_SetMotorPWM(1, 550, 1);
        BSP_SPI_SetMotorPWM(2, 550, 0);
        BSP_SPI_SetMotorPWM(3, 550, 1);
        lineRes = BSP_Linefind_Read();
        if (lineRes.L1 == 1 && lineRes.L2 == 1 && lineRes.R2 == 0)
        {
          break;
        }
      }
      BSP_SPI_AllBrake();
      uint8_t _barrier1 = 0;
      for (int8_t i = -15; i <= 40; i++)
      {
        BSP_FDetect_SetAngle(i);
        if (BSP_FDetect_Read())
        {
          _barrier1 = 1;
          break;
        }
        delay_1ms(10);
      }
      BSP_FDetect_SetAngle(0);
      if (_barrier1)
      {
        // use middle path
        _c0_timestamp = getSysPeriod();
        while (1)
        {
          BSP_SPI_SetMotorPWM(0, 650, 0);
          BSP_SPI_SetMotorPWM(1, 650, 1);
          BSP_SPI_SetMotorPWM(2, 650, 0);
          BSP_SPI_SetMotorPWM(3, 650, 1);
          lineRes = BSP_Linefind_Read();
          if (getSysPeriod() - _c0_timestamp >= 200 && lineRes.L1 == 1 && lineRes.R1 == 1 && lineRes.R3 == 0)
          {
            break;
          }
        }
        BSP_SPI_AllBrake();
        while (1)
        {
          _Director_AggressiveLineFollow();
          lineRes = BSP_Linefind_Read();
          if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 == 0)
          {
            break;
          }
        }
        BSP_SPI_AllBrake();
        while (1)
        {
          BSP_SPI_SetMotorPWM(0, 600, 1);
          BSP_SPI_SetMotorPWM(1, 600, 0);
          BSP_SPI_SetMotorPWM(2, 600, 1);
          BSP_SPI_SetMotorPWM(3, 600, 0);
          lineRes = BSP_Linefind_Read();
          if (lineRes.L1 == 1 && lineRes.R1 == 1 && lineRes.R2 == 0)
          {
            break;
          }
        }
        _c0_timestamp = getSysPeriod();
        while (1)
        {
          _Director_AggressiveLineFollow();
          lineRes = BSP_Linefind_Read();
          if (getSysPeriod() - _c0_timestamp >= 500 && lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 4)
          {
            break;
          }
        }
        delay_1ms(150);
        _c0_timestamp = getSysPeriod();
        while (1)
        {
          BSP_SPI_SetMotorPWM(0, 550, 0);
          BSP_SPI_SetMotorPWM(1, 550, 1);
          BSP_SPI_SetMotorPWM(2, 550, 0);
          BSP_SPI_SetMotorPWM(3, 550, 1);
          if (getSysPeriod() - _c0_timestamp >= 350)
          {
            break;
          }
        }
      }
      else
      {
        // No more barriers.
        _c0_timestamp = getSysPeriod();
        while (1)
        {
          _Director_AggressiveLineFollow();
          lineRes = BSP_Linefind_Read();
          if (getSysPeriod() - _c0_timestamp >= 500 && lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 4)
          {
            break;
          }
        }
        delay_1ms(150);
        _c0_timestamp = getSysPeriod();
        while (1)
        {
          BSP_SPI_SetMotorPWM(0, 550, 1);
          BSP_SPI_SetMotorPWM(1, 550, 0);
          BSP_SPI_SetMotorPWM(2, 550, 1);
          BSP_SPI_SetMotorPWM(3, 550, 0);
          if (getSysPeriod() - _c0_timestamp >= 350)
          {
            break;
          }
        }
      }
    }
    else
    {
      // Barrier 3 not exists
      BSP_SPI_AllBrake();
      delay_1ms(100);
      _c0_timestamp = getSysPeriod();
      while (1)
      {
        BSP_SPI_SetMotorPWM(0, 550, 0);
        BSP_SPI_SetMotorPWM(1, 550, 1);
        BSP_SPI_SetMotorPWM(2, 550, 0);
        BSP_SPI_SetMotorPWM(3, 550, 1);
        if (getSysPeriod() - _c0_timestamp >= 500)
        {
          break;
        }
      }
      while (1)
      {
        _Director_NormalLineFollow();
        lineRes = BSP_Linefind_Read();
        if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 == 0)
        {
          break;
        }
      }
      BSP_SPI_AllBrake();
      // No more barriers.
      _c0_timestamp = getSysPeriod();
      while (1)
      {
        BSP_SPI_SetMotorPWM(0, 550, 1);
        BSP_SPI_SetMotorPWM(1, 550, 0);
        BSP_SPI_SetMotorPWM(2, 550, 1);
        BSP_SPI_SetMotorPWM(3, 550, 0);
        lineRes = BSP_Linefind_Read();
        if (getSysPeriod() - _c0_timestamp >= 250 && lineRes.L1 == 1 && lineRes.L2 == 1 && lineRes.L3 == 0)
        {
          break;
        }
      }
      _c0_timestamp = getSysPeriod();
      while (1)
      { // LEFT ANGLE
        _Director_AggressiveLineFollow();
        lineRes = BSP_Linefind_Read();
        if (getSysPeriod() - _c0_timestamp >= 150 && lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 4)
        {
          break;
        }
      }
      BSP_SPI_AllBrake();
      _c0_timestamp = getSysPeriod();
      while (1)
      {
        BSP_SPI_SetMotorPWM(0, 550, 0);
        BSP_SPI_SetMotorPWM(1, 550, 1);
        BSP_SPI_SetMotorPWM(2, 550, 0);
        BSP_SPI_SetMotorPWM(3, 550, 1);
        lineRes = BSP_Linefind_Read();
        if (getSysPeriod() - _c0_timestamp >= 150 && lineRes.L1 == 1 && lineRes.L2 == 1 && lineRes.R2 == 0)
        {
          break;
        }
      }
    }

    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 6:
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 7:
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L4 == 1 && lineRes.L3 == 1 && lineRes.L2 == 1)
      {
        break;
      }
    }
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 550, 1);
      BSP_SPI_SetMotorPWM(1, 550, 0);
      BSP_SPI_SetMotorPWM(2, 550, 1);
      BSP_SPI_SetMotorPWM(3, 550, 0);
      lineRes = BSP_Linefind_Read();
      if (lineRes.L2 == 1 && lineRes.L1 == 1 && lineRes.R1 == 0)
      {
        break;
      }
    }

    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 8:
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L3 == 1 && lineRes.L4 == 1 && lineRes.L2 == 1)
      {
        break;
      }
    }
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 9:
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 == 0)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

    /*
    POS_LOOP_INSIDE, // 10
    POS_LOOP_BOTTOM,
    POS_BRANCH_1_3,
    POS_BRANCH_1_2,
    POS_BROKENROAD,
    */

  case 10:
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 650, 1);
      BSP_SPI_SetMotorPWM(1, 650, 0);
      BSP_SPI_SetMotorPWM(2, 650, 1);
      BSP_SPI_SetMotorPWM(3, 650, 0);
      lineRes = BSP_Linefind_Read();
      if (lineRes.L2 == 1 && lineRes.L1 == 1 && lineRes.R1 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_LoopLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 6)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 11:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 650, 1);
      BSP_SPI_SetMotorPWM(1, 650, 0);
      BSP_SPI_SetMotorPWM(2, 650, 1);
      BSP_SPI_SetMotorPWM(3, 650, 0);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 150 && lineRes.L2 == 1 && lineRes.L1 == 1 && lineRes.R1 == 0)
      {
        break;
      }
    }
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 50 && lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 == 0)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 12:
  case 22:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 600, 1);
      BSP_SPI_SetMotorPWM(1, 600, 0);
      BSP_SPI_SetMotorPWM(2, 600, 1);
      BSP_SPI_SetMotorPWM(3, 600, 0);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 350 && lineRes.R1 == 1 && lineRes.R2 == 1 && lineRes.L1 == 0)
      {
        break;
      }
    }
    delay_1ms(110);
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      _Director_LoopLineFollow();
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp > 300 && lineRes.L4 == 1 && lineRes.L3 == 1 && lineRes.L2 == 1)
      {
        break;
      }
    }
    delay_1ms(50);
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 13:
  case 23:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 600, 1);
      BSP_SPI_SetMotorPWM(1, 600, 0);
      BSP_SPI_SetMotorPWM(2, 600, 1);
      BSP_SPI_SetMotorPWM(3, 600, 0);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 100 && lineRes.L2 == 1 && lineRes.L1 == 1 && lineRes.R2 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 7)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 14:
  case 24:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 500, 0);
      BSP_SPI_SetMotorPWM(1, 500, 0);
      BSP_SPI_SetMotorPWM(2, 500, 0);
      BSP_SPI_SetMotorPWM(3, 500, 0);
      if (getSysPeriod() - _c0_timestamp >= 200 && lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 7)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

    /*
    POS_TUNNEL_2, // 15
    POS_BRANCH_3_3,
    POS_BRANCH_4_2,
    POS_BRANCH_4_3,
    POS_LOOP_TOP,
    */

  case 15:
  case 25:
    BSP_LEDBUZ_Flash(BSP_LEDBUZ_LED, 500, 2);
    BSP_LEDBUZ_Flash(BSP_LEDBUZ_BUZ, 1000, 1);
    delay_1ms(100);
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 300 && lineRes.R4 == 1 && lineRes.R3 == 1)
      {
        break;
      }
    }
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 550, 0);
      BSP_SPI_SetMotorPWM(1, 550, 1);
      BSP_SPI_SetMotorPWM(2, 550, 0);
      BSP_SPI_SetMotorPWM(3, 550, 1);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 500 && lineRes.R2 == 1 && lineRes.R1 == 1 && lineRes.L1 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L4 == 1 && lineRes.L3 == 1)
      {
        break;
      }
    }
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 16:
  case 17:
  case 26:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 600, 1);
      BSP_SPI_SetMotorPWM(1, 600, 0);
      BSP_SPI_SetMotorPWM(2, 600, 1);
      BSP_SPI_SetMotorPWM(3, 600, 0);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 100 && lineRes.L2 == 1 && lineRes.L1 == 1 && lineRes.R2 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 7)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 18:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 600, 1);
      BSP_SPI_SetMotorPWM(1, 600, 0);
      BSP_SPI_SetMotorPWM(2, 600, 1);
      BSP_SPI_SetMotorPWM(3, 600, 0);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 100 && lineRes.L2 == 1 && lineRes.L1 == 1 && lineRes.R2 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 == 0)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 19:
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

    /*
    POS_LOOP_INSIDE, // 20
    POS_LOOP_BOTTOM,
    POS_BRANCH_1_3,
    POS_BRANCH_1_2,
    POS_BROKENROAD,
    */

  case 20:
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 650, 0);
      BSP_SPI_SetMotorPWM(1, 650, 1);
      BSP_SPI_SetMotorPWM(2, 650, 0);
      BSP_SPI_SetMotorPWM(3, 650, 1);
      lineRes = BSP_Linefind_Read();
      if (lineRes.R2 == 1 && lineRes.R1 == 1 && lineRes.L1 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_LoopLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 >= 6)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 21:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 650, 0);
      BSP_SPI_SetMotorPWM(1, 650, 1);
      BSP_SPI_SetMotorPWM(2, 650, 0);
      BSP_SPI_SetMotorPWM(3, 650, 1);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 250)
      {
        break;
      }
    }
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 50 && lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 == 0)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

    /*
    POS_TUNNEL_2, // 25
    POS_BRANCH_3_3,
    POS_BRANCH_4_2,
    POS_BARRIERS_LEFT,
    POS_BARRIERS_INSIDE,
    */

  case 27:
  case 28:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 600, 0);
      BSP_SPI_SetMotorPWM(1, 600, 1);
      BSP_SPI_SetMotorPWM(2, 600, 0);
      BSP_SPI_SetMotorPWM(3, 600, 1);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 100 && lineRes.R2 == 1 && lineRes.R1 == 1 && lineRes.L2 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 == 0)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 29:
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

    /*
    POS_BARRIERS_RIGHT, // 30
    POS_BRANCH_2_1,
    POS_VISITSITE,
    POS_BRANCH_2_1,
    POS_BRANCH_2_2,
    */

  case 30:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 600, 1);
      BSP_SPI_SetMotorPWM(1, 600, 0);
      BSP_SPI_SetMotorPWM(2, 600, 1);
      BSP_SPI_SetMotorPWM(3, 600, 0);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 100 && lineRes.L2 == 1 && lineRes.L1 == 1 && lineRes.R2 == 0)
      {
        break;
      }
    }
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L1 + lineRes.L2 + lineRes.L3 + lineRes.L4 + lineRes.R1 + lineRes.R2 + lineRes.R3 + lineRes.R4 == 0)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  case 31:
    _c0_timestamp = getSysPeriod();
    while (1)
    {
      BSP_SPI_SetMotorPWM(0, 600, 0);
      BSP_SPI_SetMotorPWM(1, 600, 1);
      BSP_SPI_SetMotorPWM(2, 600, 0);
      BSP_SPI_SetMotorPWM(3, 600, 1);
      lineRes = BSP_Linefind_Read();
      if (getSysPeriod() - _c0_timestamp >= 100 && lineRes.R1 == 1 && lineRes.R2 == 1 && lineRes.R3 == 0)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    while (1)
    {
      _Director_AggressiveLineFollow();
      lineRes = BSP_Linefind_Read();
      if (lineRes.L2 == 1 && lineRes.L3 == 1 && lineRes.R1 == 1)
      {
        break;
      }
    }
    BSP_SPI_AllBrake();
    _Director_PosShift(thisState, _route[thisState->steps + 1]);
    thisState->steps++;
    break;

  default:
    while (1)
    {
      BSP_SPI_AllBrake();
      BSP_LEDBUZ_Flash(BSP_LEDBUZ_LED, 250, 100);
    }
  }
}
