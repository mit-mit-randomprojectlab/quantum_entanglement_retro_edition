/*
// solver.h: functions for finding a feasible solution to a given puzzle
*/

#include <stdio.h>
#include <stdlib.h>

#include "consts.h"

typedef struct {
    int n;
    int current;
    int *moves;
} Solution;

typedef struct {
    int n;
    int *camefrom;
    int *camefrom_move;
    int *score;
    int *visited;
} SolveData;

Solution* solve_level(int ldata_left[PLAYSPACE_NY][PLAYSPACE_NX],
    int ldata_right[PLAYSPACE_NY][PLAYSPACE_NX]);
void PrintSolution(Solution *solution);
void FreeSolution(Solution *solution);

