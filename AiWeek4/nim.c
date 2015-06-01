#include <stdio.h>
#include <stdlib.h>


#define MAX_V 0
#define MIN_V 1

#define INF 9999999


int minValue(int state); /* forward declaration: mutual recursion */

int maxValue(int state) {
    int move, max = -INF;
    /* terminal state ? */
    if (state == 1) {
        return -1; /* Min wins if max is in a terminal state */
    }
    /* non-terminal state */
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { /* legal move */
            int m = minValue(state - move);
            if (m > max) max = m;
        }
    }
    return max;
}

int minValue(int state) {
    int move, min = INF;
    /* terminal state ? */
    if (state == 1) {
        return 1; /* Max wins if min is in a terminal state */
    }
    /* non-terminal state */
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { /* legal move */
            int m = maxValue(state - move);
            if (m < min) min = m;
        }
    }
    return min;
}

int minimaxDecision(int state, int turn) {
    int move, bestmove, max, min;
    
    if (turn == MAX_V) {
        max = -INF;
        for (move = 1; move <= 3; move++) {
            if (state - move > 0) { /* legal move */
                int m = minValue(state - move);
                if (m > max) {
                    max = m;
                    bestmove = move;
                }
            }
        }
        return bestmove;
    }
    /* turn == MIN */
    min = INF;
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { /* legal move */
            int m = maxValue(state - move);
            if (m < min) {
                min = m;
                bestmove = move;
            }
        }
    }
    return bestmove;
}
int negamaxValue(int state, int player) {
    int move, max = -INF;
    /* terminal state ? */
    if (state == 1) {
        int return_val = player == MAX_V ? -1 : 1;
        return return_val;
    }
    /* non-terminal state */
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { /* legal move */
            int m = negamaxValue(state - move, 1-player);
            if (m > max)
                max = m;
        }
    }
    return max;

}

int negamaxDecision(int state) {
    int move, bestmove = 0, max = -INF;
    
    for (move=1; move<=3; move++) {
        if (state - move > 0) {
            int m = -1 * negamaxValue(state - move, MAX_V);
            if (m > max) {
                max = m;
                bestmove = move;
            }
        }
    }
    return bestmove;
}


void playNim(int state) {
    int turn = 0;
    while (state != 1) {
        int action = minimaxDecision(state, turn);
        printf("%d: %s takes %d\n", state,
               (turn==MAX_V ? "Max" : "Min"), action);
        state = state - action;
        turn = 1 - turn;
    }
    printf("1: %s looses\n", (turn==MAX_V ? "Max" : "Min"));
}

void playNimNegamax(int state)
{
    int turn = 0;
    while (state != 1)
    {
        int action = negamaxDecision(state);
        printf("%d: %s takes %d\n", state, (turn==MAX_V ? "Max" : "Min"), action);
        state = state - action;
        turn = 1 - turn;
    }
    printf("1: %s looses\n", (turn==MAX_V ? "Max" : "Min"));
}


int main(int argc, char *argv[]) {
    /*
     if ((argc != 2) || (atoi(argv[1]) < 3)) {
     fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
     fprintf(stderr, "<number of sticks> must be at least 3!\n");
     return -1;
     }
     */
    

    //playNim(20);
    playNimNegamax(20);
    return 0;
}
