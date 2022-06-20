/*
// solve_main.c: test solver
*/

#include <stdio.h>

#include "solver.h"

/*
// Level data
static int levelData_test_left[PLAYSPACE_NY][PLAYSPACE_NX] = {
    {9,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,3}
};
static int levelData_test_right[PLAYSPACE_NY][PLAYSPACE_NX] = {
    {9,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0},
    {0,0,0,0,0,1,0,0},
    {0,0,0,0,0,1,0,3}
};
*/

/*
// Level data
static int levelData_test_left[PLAYSPACE_NY][PLAYSPACE_NX] = {
    {9,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,3}
};
static int levelData_test_right[PLAYSPACE_NY][PLAYSPACE_NX] = {
    {9,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,3}
};
*/


static int levelData_test_left[PLAYSPACE_NY][PLAYSPACE_NX] = {
    {9,0,0,0,2,2,2,2},
    {0,0,0,0,2,0,0,0},
    {0,0,0,0,2,0,2,0},
    {0,0,0,0,2,0,2,0},
    {0,0,0,1,2,0,2,0},
    {0,0,0,0,0,0,2,0},
    {0,0,0,0,2,0,2,0},
    {0,0,0,0,2,0,2,3}
};
static int levelData_test_right[PLAYSPACE_NY][PLAYSPACE_NX] = {
    {9,0,2,0,0,0,0,0},
    {0,0,2,0,0,0,0,0},
    {0,0,2,2,2,2,2,0},
    {0,0,2,0,0,0,2,0},
    {0,0,2,0,2,0,2,0},
    {0,0,2,0,2,0,2,0},
    {0,1,2,0,2,0,2,2},
    {0,0,0,0,2,0,0,3}
};


// Test solution finder
int main() {
    
    /*
    int ix_l = 6;
    int iy_l = 3;
    int ix_r = 4;
    int iy_r = 5;
    int c = xy2c(ix_l,iy_l,ix_r,iy_r);
    c2xy(c,&ix_l,&iy_l,&ix_r,&iy_r);
    printf("%d %d %d %d\n",ix_l,iy_l,ix_r,iy_r);
    */
    
    Solution *solution = solve_level(levelData_test_left, levelData_test_right);
    
    PrintSolution(solution);
    FreeSolution(solution);
    
    return 0;
}

