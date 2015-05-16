/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXQ 100
#define MAXSECONDS 16000

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */
int nsuccessors;
int successors[MAXQ][MAXQ];

void initializeRandomGenerator() {
    /* this routine initializes the random generator. You are not
     * supposed to understand this code. You can simply use it.
     */
    time_t t;
    srand((unsigned) time(&t));
}

/* Generate an initial position.
 * If flag == 0, then for each row, a queen is placed in the first column.
 * If flag == 1, then for each row, a queen is placed in a random column.
 */
void initiateQueens(int flag) {
    int q;
    for (q = 0; q < nqueens; q++) {
        queens[q] = (flag == 0? 0 : random()%nqueens);
    }
}

/* returns TRUE if position (row0,column0) is in 
 * conflict with (row1,column1), otherwise FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
    if (row0 == row1) return TRUE; /* on same row, */
    if (column0 == column1) return TRUE; /* column, */
    if (ABS(row0-row1) == ABS(column0-column1)) return TRUE;/* diagonal */
    return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in 
 * conflict with any other queen on the board, otherwise FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
    int queen;
    for (queen=0; queen < nqueens; queen++) {
        if (inConflict(row, col, queen, queens[queen])) {
            if ((row != queen) || (col != queens[queen])) return TRUE;
        }
    }
    return FALSE;
}

/* print configuration on screen */
void printState() {
    int row, column;
    printf("\n");
    for(row = 0; row < nqueens; row++) {
        for(column = 0; column < nqueens; column++) {
            if (queens[row] != column) {
                printf (".");
            } else {
                if (inConflictWithAnotherQueen(row, column)) {
                    printf("Q");
                } else {
                    printf("q");
                }
            }
        }
        printf("\n");
    }
}

/* print configuration on screen */
void printCustomState(int *state) {
    int row, column;
    printf("\n");
    for(row = 0; row < nqueens; row++) {
        for(column = 0; column < nqueens; column++) {
            if (state[row] != column) {
                printf (".");
            } else {
                printf("q");
            }
        }
        printf("\n");
    }
}

/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
    if ((queen < 0) || (queen >= nqueens)) {
        fprintf(stderr, "Error in moveQueen: queen=%d "
                "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
        exit(-1);
    }
    if ((column < 0) || (column >= nqueens)) {
        fprintf(stderr, "Error in moveQueen: column=%d "
                "(should be 0<=column<%d)...Abort.\n", column, nqueens);
        exit(-1);
    }
    queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
 * (queen,column). Note that this routine checks only that 
 * the values of queen and column are valid! It does not test
 * conflicts!
 */
int canMoveTo(int queen, int column) {
    if ((queen < 0) || (queen >= nqueens)) {
        fprintf(stderr, "Error in canMoveTo: queen=%d "
                "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
        exit(-1);
    }
    if(column < 0 || column >= nqueens) return FALSE;
    if (queens[queen] == column) return FALSE; /* queen already there */
    return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
    if ((queen < 0) || (queen >= nqueens)) {
        fprintf(stderr, "Error in columnOfQueen: queen=%d "
                "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
        exit(-1);
    }
    return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
    int cnt = 0;
    int queen, other;
    for (queen=0; queen < nqueens; queen++) {
        for (other=queen+1; other < nqueens; other++) {
            if (inConflict(queen, queens[queen], other, queens[other])) {
                cnt++;
            }
        }
    }
    return cnt;
}

/* evaluation function. The maximal number of queens in conflict
 * can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * Since we want to do ascending local searches, the evaluation
 * function returns (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
    return (nqueens-1)*nqueens/2 - countConflicts();
}

int evaluateState2(){
    return - countConflicts();
}

void setState(int *state) {
    int i;
    for (i=0; i<nqueens; i++) {
        queens[i]=state[i];
    }
}

/*************************************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 2000000
void randomSearch() {
    int queen, iter = 0;
    int optimum = (nqueens-1)*nqueens/2;

    while (evaluateState() != optimum) {
        printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
        if (iter == MAXITER) break;  /* give up */
        /* generate a (new) random state: for each queen do ...*/
        for (queen=0; queen < nqueens; queen++) {
            int pos, newpos;
            /* position (=column) of queen */
            pos = columnOfQueen(queen);
            /* change in random new location */
            newpos = pos;
            while (newpos == pos) {
                newpos = random() % nqueens;
            }
            moveQueen(queen, newpos);
        }
    }
    if (iter < MAXITER) {
        printf ("Solved puzzle. ");
    }
    printf ("Final state is");
    printState();
}

/*************************************************************/
void saveToSuccessors(int successor_index) {
    int i;
    for (i=0; i<nqueens; i++) {
        successors[successor_index][i] = queens[i];
    }
}

void generateSuccessors() {
    nsuccessors = 0;
    
    int i=0;
    for (i=0; i<nqueens; i++) {
        int column=0;
        for (column=0; column<nqueens; column++) {
            if (queens[i] != column && canMoveTo(queens[i], column)) {
                int queen_position = queens[i]; //Save current position to undo movement later
                moveQueen(i, column);
                saveToSuccessors(nsuccessors);
                nsuccessors++;
                // Undo move
                moveQueen(i, queen_position);
            }
        }
    }
}

void hillClimbing() {
    int optimum = (nqueens-1)*nqueens/2;
    int iterations = 0;
    
    
    while (iterations < MAXITER && evaluateState() != optimum) {
        int best_successors_indices[MAXQ];
        int best_successors_count = 0;
        int best_successor_value = -999;
        nsuccessors = 0;
        
        // Save current state
        int current_state[10], i;
        for (i=0; i<nqueens; i++) {
            current_state[i] = queens[i];
        }
        
        generateSuccessors();
        for (i=0; i<nsuccessors; i++) {
            setState(successors[i]);
            if (evaluateState() > best_successor_value) {
                best_successor_value = evaluateState();
                best_successors_indices[0] = i;
                best_successors_count = 1;
            }
            else if (evaluateState() == best_successor_value) {
                best_successors_indices[best_successors_count] = i;
                best_successors_count++;
            }
            setState(current_state);
        }
        
        // Choose randomly between the best successors
        int chosen_index = 0 + random() % (best_successors_count - 0);
        setState(successors[chosen_index]);
        
        iterations++;
    }
    if (iterations < MAXITER) {
        printf ("Solved puzzle.\n");
        printf ("Final state is:\n\n");
        printState();
    }
    else {
        printf("Maximum number of iterations reached. Puzzle not solved.\n");
    }
}

/*************************************************************/
double timeToTemperature(double time)
{
    if (time == 0)
    {
        return 1000;
    }
    return (double)1/time;
}


void simulatedAnnealing() {
    time_t start, end;
    int optimum = (nqueens-1)*nqueens/2;
    double seconds = 0;
    double temperature = 0;
    int iterations = 0;
    int delta = 0;
    int current_evaluation;
    int successor_evaluation;
    double probability;
    double rand;
    
    // Starts the counter
    time(&start);
    
    // Save current state
    int current_state[10], i;
    for (i=0; i<nqueens; i++) {
        current_state[i] = queens[i];
    }
    generateSuccessors();
    
    while(iterations < MAXITER && evaluateState() != optimum)
    {
        setState(current_state);
        current_evaluation = evaluateState();
        
        time(&end);
        seconds = difftime(end, start);
        temperature = timeToTemperature(seconds);
        
        if (temperature < 0.0001)
        {
            printf("Final state is:\n\n");
            printState();
            break;
        }
        
        // Choose randomly between the best successors
        int chosen_index = 0 + random() % (nsuccessors - 0);
        setState(successors[chosen_index]);
        successor_evaluation = evaluateState();
        
        delta = successor_evaluation - current_evaluation;
        probability =  exp(delta/temperature);
        rand = random() % 100;
        
        if (delta > 0)
        {
            // Update current state to next state
            for (i = 0; i < nqueens; i++)
            {
                current_state[i] = queens[i];
            }
        }
        else
        {
            if (rand < probability)
            {
                // Update current state to next state
                for (i = 0; i < nqueens; i++)
                {
                    current_state[i] = queens[i];
                }
            }
          
        }
        
        iterations++;
    }
    if (iterations < MAXITER) {
        printf ("Solved puzzle.\n");
        printf ("Final state is:\n\n");
        printState();
        getchar();
    }
    else {
        printf("Maximum number of iterations reached. Puzzle not solved.\n");
    }

}


int main(int argc, char *argv[]) {
    int algorithm;

    do {
        printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
        scanf ("%d", &nqueens);
    } while ((nqueens < 1) || (nqueens > MAXQ));

    do {
        printf ("Algorithm: (1) Random search  (2) Hill climbing  ");
        printf ("(3) Simulated Annealing: ");
        scanf ("%d", &algorithm);
    } while ((algorithm < 1) || (algorithm > 3));
  
    initializeRandomGenerator();

    initiateQueens(1);
  
    printf("\nInitial state:");
    printState();
    int i=0;

    switch (algorithm) {
        case 1: randomSearch();       break;
        case 2: hillClimbing();       break;
        case 3:
            for(i = 0; i < 10000; i++)
            {
                initiateQueens(1);
                simulatedAnnealing();
            }
            
            break;
    }

    return 0;  
}