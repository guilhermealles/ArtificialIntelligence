#include <stdio.h>
#include <stdlib.h>


#define MAX_V 0
#define MIN_V 1

#define INF 9999999


int minValue(int state); /* forward declaration: mutual recursion */

int maxValue(int state) {
    int move, max = INF;
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
    int move, bestmove = 0, max, min;
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
    else {
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
}
/*
int evaluate(int state)
{
    int move, best = INF, best_move;
    
    if (state == 1) // Terminal state
    {
        return 1;
    }
    
    for (move = 1; move <=3; move++)
    {
        if (state - move > 0)
        {
            int m = evaluate(state - move);
            
            if (m > best)
            {
                best = m;
                best_move = move;
            }
        }
    }
    return best;
}

// Returns an array with the best move (index 0) and its evaluation (index 1).
int* negamax(int state)
{
<<<<<<< Updated upstream
    int best_choice[2]; // TODO need malloc
    int move, value, best_value = -INF, best_move;
=======
    int* best_choice = malloc(2 * sizeof(int));
    int move = 0, value, best_value = -INF, best_move;
>>>>>>> Stashed changes
    
    if (state - move == 0) // Is terminal state
    {
        best_choice[0] = move;
        best_choice[1] = evaluate(state);
        return best_choice;
    }
    
    for (move = 1; move <=3; move++)
    {
        
        if (state - move > 0) // Is a valid move
        {
            int new_choice[2];
            new_choice[0] = move;
            new_choice[1] = evaluate(state - move);
            
            int evaluated_choice[2]'
            evaluated_choice = negamax(state - move);
            evaluated_choice[1] = -1 * evaluated_choice[1];
            
            if (evaluated_choice[1] > best_value)
            {
                best_value = evaluated_choice[1];
                best_move = move;
            }
        }
    }
    
    best_choice[0] = best_move;
    best_choice[1] = best_value;
    return best_choice;
}
 */

int negamaxValue(int state) {
    int move, max = -INF;
    /* terminal state ? */
    if (state == 1) {
        return 1;
    }
    /* non-terminal state */
    for (move = 1; move <= 3; move++) {
        if (state - move > 0) { /* legal move */
            int m = negamaxValue(state - move);
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
            int m = -1 * negamaxValue(state - move);
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
        printf("%d: %s takes %d\n", state, (turn==MAX_V ? "Max" : "Min"), action);
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
<<<<<<< Updated upstream
        int action = negamaxDecision(state);
=======
        int result[2];
        result = negamax(state);
>>>>>>> Stashed changes
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
    
    //playNim(atoi(argv[1]));
<<<<<<< Updated upstream
    
    playNimNegamax(3);
=======
    //playNim(3);
    playNimNegamax(3);

>>>>>>> Stashed changes
    return 0;
}
