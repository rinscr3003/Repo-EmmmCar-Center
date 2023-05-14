#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include <stdint.h>

/*
 * Director
 * Version = v1.0.0.0
 * Author = 9223020209
 * Comment = The leader.
 */

typedef enum
{
    POS_UNDEFINED = 0,
    POS_START = 1,
    POS_REDUCER = 2,
    POS_BRANCH_1_1 = 3,
    POS_TUNNEL_3 = 4,
    POS_BRANCH_1_2 = 5,
    POS_BRANCH_1_3 = 6,
    POS_SPECIALZONE = 7,
    POS_VISITSITE = 8,
    POS_BROKENROAD = 9,
    POS_LOOP_BOTTOM = 10,
    POS_LOOP_INSIDE = 11,
    POS_LOOP_TOP = 12,
    POS_BRANCH_2_1 = 13,
    POS_BRANCH_2_2 = 14,
    POS_BRANCH_2_3 = 15,
    POS_BRANCH_2_4 = 16,
    POS_PARKING_1 = 17,
    POS_PARKING_2 = 18,
    POS_PARKING_3 = 19,
    POS_BRANCH_2_5 = 20,
    POS_BRANCH_3_1 = 21,
    POS_BRANCH_3_2 = 22,
    POS_BRANCH_3_3 = 23,
    POS_TUNNEL_2 = 24,
    POS_BRANCH_4_1 = 25,
    POS_BARRIERS_RIGHT = 26,
    POS_BARRIERS_INSIDE = 36,
    POS_BARRIERS_LEFT = 27,
    POS_BRANCH_4_2 = 28,
    POS_BRANCH_4_3 = 29,
    POS_TUNNEL_1 = 30,
    POS_ANGLES_RIGHT = 31,
    POS_ANGLES_INSIDE = 32,
    POS_ANGLES_LEFT = 34,
    POS_BRANCH_5_1 = 35,

    POS_OUTSIDE = 114514,
    POS_UNREACHABLE_INDEX = 1919810
} Director_PosState;

typedef enum
{
    LOOP_LEFT = 0,
    LOOP_RIGHT,
} Director_LoopSide;

typedef struct
{
    Director_PosState lastState;
    Director_PosState nowState;
    Director_PosState nextState;
    Director_LoopSide lastLoopSide;
    uint16_t steps;
} Director_ThisState;

void Director_Init(Director_ThisState *thisState, Director_PosState posState);
void Director_Loop(Director_ThisState *thisState);

#endif
