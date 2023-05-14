#include "director.h"
#include "bsp_ledbuz.h"
#include "bsp_fdetect.h"
#include "bsp_linefind.h"
#include "bsp_spi.h"

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
