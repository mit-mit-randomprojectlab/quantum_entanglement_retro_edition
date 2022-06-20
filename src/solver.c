/*
// solver.c: functions for finding a feasible solution to a given puzzle
*/

#include "solver.h"

int xy2c(int ix_l, int iy_l, int ix_r, int iy_r) {
    return PLAYSPACE_NX*PLAYSPACE_NX*PLAYSPACE_NY*iy_r + 
        PLAYSPACE_NX*PLAYSPACE_NY*ix_r + PLAYSPACE_NX*iy_l + ix_l;
}

void c2xy(int c, int *ix_l, int *iy_l, int *ix_r, int *iy_r) {
    *ix_l = c % PLAYSPACE_NX;
    *iy_l = (c/PLAYSPACE_NX) % PLAYSPACE_NX;
    *ix_r = (c/(PLAYSPACE_NX*PLAYSPACE_NY)) % PLAYSPACE_NX;
    *iy_r = (c/(PLAYSPACE_NX*PLAYSPACE_NX*PLAYSPACE_NY)) % PLAYSPACE_NX;
}

int check_space(int ldata[PLAYSPACE_NY][PLAYSPACE_NX], int ix, int iy) {
    if ( (ix < 0) || (ix >= PLAYSPACE_NX) || (iy < 0) || (iy >= PLAYSPACE_NY) ) {
        return 0; // wall, no move
    }
    else if (ldata[iy][ix] == 1) {
        return 0; // wall, no move
    }
    else if (ldata[iy][ix] == 2) {
        return -1; // infeasible, hits lava
    }
    else {
        return 1; // free space
    }
}

int check_move(int c, int ldata_left[PLAYSPACE_NY][PLAYSPACE_NX],
    int ldata_right[PLAYSPACE_NY][PLAYSPACE_NX], int move) {
    
    int ix_l, iy_l, ix_r, iy_r;
    c2xy(c, &ix_l, &iy_l, &ix_r, &iy_r);
    
    int ix_l2 = ix_l;
    int iy_l2 = iy_l;
    int ix_r2 = ix_r;
    int iy_r2 = iy_r;
    switch (move) {
        case 0: // L
            ix_l2--;
            ix_r2--;
            break;
        case 1: // R
            ix_l2++;
            ix_r2++;
            break;
        case 2: // U
            iy_l2--;
            iy_r2--;
            break;
        case 3: // D
            iy_l2++;
            iy_r2++;
            break;
    }
    
    int move_l = check_space(ldata_left, ix_l2, iy_l2);
    int move_r = check_space(ldata_right, ix_r2, iy_r2);
    if ( ( (move_l == -1) || (move_r == -1) ) || ( (move_l == 0) && (move_r == 0) ) ) {
        return -1; // infeasible move
    }
    if (move_l == 0) {
        ix_l2 = ix_l;
        iy_l2 = iy_l;
    }
    if (move_r == 0) {
        ix_r2 = ix_r;
        iy_r2 = iy_r;
    }
    
    int c2 = xy2c(ix_l2, iy_l2, ix_r2, iy_r2);
    return c2;
    
}

void check_node(int c, int current_score, int ldata_left[PLAYSPACE_NY][PLAYSPACE_NX],
    int ldata_right[PLAYSPACE_NY][PLAYSPACE_NX], SolveData *solvedata) {
    
    for (int im = 0; im < 4; im++) {
        int cnew = check_move(c, ldata_left, ldata_right, im);
        if (cnew > 0) {
            if ( (solvedata->score[cnew] == -1) || (solvedata->score[cnew] > (current_score+1)) ) {
                solvedata->score[cnew] = current_score+1;
                solvedata->camefrom[cnew] = c;
                solvedata->camefrom_move[cnew] = im;
            }
        }
    }
    
}

int get_priority(SolveData *solvedata) {
    int min = solvedata->n;
    int ind = -1;
    for (int i = 0; i < solvedata->n; i++) {
        if ( (solvedata->visited[i] == 0) && (solvedata->score[i] >= 0) && (solvedata->score[i] < min) ) {
            min = solvedata->score[i];
            ind = i;
        }
    }
    if (ind >= 0) {
        solvedata->visited[ind] = 1;
        return ind;
    }
    else {
        return -1;
    }
}

void InitSolveData(SolveData *solvedata) {
    solvedata->n = PLAYSPACE_NX*PLAYSPACE_NY*PLAYSPACE_NX*PLAYSPACE_NY;
    solvedata->camefrom = (int*)malloc(solvedata->n*sizeof(int));
    solvedata->camefrom_move = (int*)malloc(solvedata->n*sizeof(int));
    solvedata->score = (int*)malloc(solvedata->n*sizeof(int));
    solvedata->visited = (int*)malloc(solvedata->n*sizeof(int));
    for (int i = 0; i < solvedata->n; i++) {
        solvedata->camefrom[i] = -1;
        solvedata->score[i] = -1;
        solvedata->visited[i] = 0;
    }
}

void FreeSolveData(SolveData *solvedata) {
    free(solvedata->camefrom);
    free(solvedata->camefrom_move);
    free(solvedata->score);
    free(solvedata->visited);
}

// solve_level: provide solution steps to solve a level
Solution* solve_level(int ldata_left[PLAYSPACE_NY][PLAYSPACE_NX],
    int ldata_right[PLAYSPACE_NY][PLAYSPACE_NX]) {
    
    Solution *solution = (Solution*)malloc(1*sizeof(Solution));
    solution->n = 0;
    solution->moves = NULL;
    
    SolveData solvedata;
    InitSolveData(&solvedata);
    
    // get start and goal states
    int sx_l = 0;
    int sy_l = 0;
    int sx_r = 0;
    int sy_r = 0;
    int gx_l = PLAYSPACE_NX-1;
    int gy_l = PLAYSPACE_NY-1;
    int gx_r = PLAYSPACE_NX-1;
    int gy_r = PLAYSPACE_NY-1;
    for (int iy = 0; iy < PLAYSPACE_NY; iy++) {
        for (int ix = 0; ix < PLAYSPACE_NX; ix++) {
            if (ldata_left[iy][ix] == 9) {
                sx_l = ix;
                sy_l = iy;
            }
            else if (ldata_left[iy][ix] == 3) {
                gx_l = ix;
                gy_l = iy;
            }
            if (ldata_right[iy][ix] == 9) {
                sx_r = ix;
                sy_r = iy;
            }
            else if (ldata_right[iy][ix] == 3) {
                gx_r = ix;
                gy_r = iy;
            }
        }
    }
    int c_start = xy2c(sx_l, sy_l, sx_r, sy_r);
    int c_goal = xy2c(gx_l, gy_l, gx_r, gy_r);
    
    // Dijkstras
    int cnext = c_start;
    int current_score = 0;
    while ( (cnext >= 0) && (cnext != c_goal) ) {
        check_node(cnext, current_score, ldata_left, ldata_right, &solvedata);
        cnext = get_priority(&solvedata);
        current_score = solvedata.score[cnext];
    }
    
    if (cnext == -1) {
        solution->n = 1;
        solution->current = 0;
        solution->moves = (int*)malloc(solution->n*sizeof(int));
        solution->moves[0] = -1;
    }
    else {
        solution->n = solvedata.score[cnext];
        solution->current = 0;
        solution->moves = (int*)malloc(solution->n*sizeof(int));
        for (int i = solvedata.score[cnext]-1; i >= 0; i--) {
            //printf("%d, %d, %d, %d\n",cnext,solvedata.score[cnext],solvedata.camefrom[cnext],solvedata.camefrom_move[cnext]);
            solution->moves[i] = solvedata.camefrom_move[cnext];
            cnext = solvedata.camefrom[cnext];
        }
    }
    
    // Clean-up
    FreeSolveData(&solvedata);
    
    return solution;
    
}

void PrintSolution(Solution *solution) {
    if (solution->moves[0] == -1) {
        printf("NOSOL\n");
        return;
    }
    for (int i = 0; i < solution->n; i++) {
        switch (solution->moves[i]) {
            case 0:
                printf("L, ");
                break;
            case 1:
                printf("R, ");
                break;
            case 2:
                printf("U, ");
                break;
            case 3:
                printf("D, ");
                break;
            default:
                printf("%d, ",solution->moves[i]);
                break;
        }
        if ( ((i+1) % 16) == 0 ) {
            printf("\n");
        }
    }
    printf("END\n");
}

void FreeSolution(Solution *solution) {
    free(solution->moves);
    free(solution);
}
