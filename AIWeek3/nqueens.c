/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXQ 100
#define MAXITER 10000

// Constants for sumulated annealing
#define TEMPERATURE_DECREASING_FACTOR 0.98
#define INITIAL_TEMPERATURE 100

// Constants for the genetic algorithm
#define POPULATION_COUNT 5
#define NEXT_GEN_COUNT 4*3*2*1 //(POPULATION_COUNT-1)!

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens_buffer[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */
int nsuccessors;
int successors[MAXQ*MAXQ][MAXQ];

int current_population[POPULATION_COUNT][MAXQ];
int next_generation[NEXT_GEN_COUNT][MAXQ]; //Next generation has size (POPULATION_COUNT-1)!;

int solutions_found; // Counts the number of solutions found in a certain number of executions

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
        queens_buffer[q] = (flag == 0? 0 : random()%nqueens);
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
        if (inConflict(row, col, queen, queens_buffer[queen])) {
            if ((row != queen) || (col != queens_buffer[queen])) return TRUE;
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
            if (queens_buffer[row] != column) {
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
    queens_buffer[queen] = column;
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
    if (queens_buffer[queen] == column) return FALSE; /* queen already there */
    return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
    if ((queen < 0) || (queen >= nqueens)) {
        fprintf(stderr, "Error in columnOfQueen: queen=%d "
                "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
        exit(-1);
    }
    return queens_buffer[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
    int cnt = 0;
    int queen, other;
    for (queen=0; queen < nqueens; queen++) {
        for (other=queen+1; other < nqueens; other++) {
            if (inConflict(queen, queens_buffer[queen], other, queens_buffer[other])) {
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
int evaluateBuffer() {
    return (nqueens-1)*nqueens/2 - countConflicts();
}

void setBuffer(int *state) {
    int i;
    for (i=0; i<nqueens; i++) {
        queens_buffer[i]=state[i];
    }
}

/************************************************************/
// Genetic Algorithm code

void addBufferToPopulation(int population_index) {
    if (population_index < POPULATION_COUNT) {
        int i;
        for (i=0; i<nqueens; i++) {
            current_population[population_index][i] = queens_buffer[i];
        }
    }
}

// Evaluates the next generation and chooses the best children to be the new population
void selectNewPopulation() {
    int best_children_indices[POPULATION_COUNT];
    int best_children_count = 0;
    int current_evaluation = -999;
    int last_children_evaluation = 999;
    
    // Select the POPULATION_COUNT-1 best children from next generation. The last children will be chosen randomly
    while (best_children_count < POPULATION_COUNT) {
        current_evaluation = -999;
        if (best_children_count == POPULATION_COUNT-1) {
            int random_index = 0 + rand() % (0+POPULATION_COUNT);
            best_children_indices[best_children_count] = random_index;
            best_children_count++;
        }
        else {
            int i, chosen_index=0;
            for (i=0; i<NEXT_GEN_COUNT; i++) {
                setBuffer(next_generation[i]);
                if ((evaluateBuffer() > current_evaluation) && (evaluateBuffer() < last_children_evaluation)) {
                    current_evaluation = evaluateBuffer();
                    chosen_index = i;
                }
            }
            last_children_evaluation = current_evaluation;
            best_children_indices[best_children_count] = chosen_index;
            best_children_count++;
        }
    }
    
    // Add the new selected children to the population
    int i;
    for (i=0; i<POPULATION_COUNT; i++) {
        setBuffer(next_generation[best_children_indices[i]]);
        addBufferToPopulation(i);
    }
}

void mutateWithProbability(int children_index) {
    int probability = 0 + rand() % (100 - 0);
    
    // Mutation happens with probability of 1%
    if (probability <= 1) {
        int random_index = 0 + rand() % (nqueens - 0);
        int random_value = 0 + rand() % (nqueens - 0);
        
        next_generation[children_index][random_index] = random_value;
    }
}

void crossOver(int father_index, int mother_index, int children_index) {
    // Choose a random value for the percentage of information from each parent. If the value is 3, the first 3 rows of the children will be the same as the father, while the remaining will be the same as the mother.
    int cross_over_point = 0 + random()%(nqueens-0);
    int i;
    
    for (i=0; i<cross_over_point; i++) {
        // Get the value from the father
        next_generation[children_index][i] = current_population[father_index][i];
    }
    for (; i<nqueens; i++) {
        // Get the value from the mother
        next_generation[children_index][i] = current_population[mother_index][i];
    }
    
    mutateWithProbability(children_index);
}

void multipleCrossOver() {
    int i, j, children_index=0;
    for (i=0; i<POPULATION_COUNT; i++) {
        for (j=i; j<POPULATION_COUNT; j++) {
            crossOver(i, j, children_index);
            children_index++;
        }
    }
}

int populationHasSolution() {
    int solution = (nqueens-1)*nqueens/2;
    int i;
    for (i=0; i<POPULATION_COUNT; i++) {
        setBuffer(current_population[i]);
        if (evaluateBuffer() == solution) {
            return TRUE;
        }
    }
    return FALSE;
}

void printSolutionFromPopulation() {
    int solution = (nqueens-1)*nqueens/2;
    int i;
    for (i=0; i<POPULATION_COUNT; i++) {
        setBuffer(current_population[i]);
        if (evaluateBuffer() == solution) {
            printState();
            return;
        }
    }
}

void geneticAlgorithm() {
    int generation_count = 0;
    
    // Initializes the population
    int i;
    for (i=0; i<POPULATION_COUNT; i++) {
        initiateQueens(1);
        addBufferToPopulation(i);
    }
    
    while (TRUE) {
        generation_count++;
        multipleCrossOver();
        selectNewPopulation();
        
        if (populationHasSolution()) {
            printf("Found a solution in generation %d.\n", generation_count);
            printf("Solution:");
            printSolutionFromPopulation();
            return;
        }
    }
}



/*************************************************************/
/* A very silly random search 'algorithm' */
void randomSearch() {
    int queen, iter = 0;
    int optimum = (nqueens-1)*nqueens/2;

    while (evaluateBuffer() != optimum) {
        printf("iteration %d: evaluation=%d\n", iter++, evaluateBuffer());
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
        successors[successor_index][i] = queens_buffer[i];
    }
}

void hillClimbing() {
    int iterations = 0;
    int optimum = (nqueens-1)*nqueens/2;
    
    int current_state[nqueens], i;
    for (i=0; i<nqueens; i++) {
        current_state[i] = queens_buffer[i];
    }
    
    while ((iterations < MAXITER) && (evaluateBuffer() != optimum)) {
        int best_move_evaluation = evaluateBuffer(); // Value to be maximized. Initialize with the current evaluation.
        nsuccessors = 0;
        
        // Loops through all possible moves
        int i, j;
        for (i=0; i<nqueens; i++) {
            for (j=0; j<nqueens; j++) {
                moveQueen(i,j);
                int successor_evaluation = evaluateBuffer();
                if (successor_evaluation > best_move_evaluation) {
                    saveToSuccessors(0);
                    nsuccessors = 1;
                    best_move_evaluation = successor_evaluation;
                }
                else if (successor_evaluation == best_move_evaluation) {
                    saveToSuccessors(nsuccessors);
                    nsuccessors++;
                }
                
                // Undo last move
                setBuffer(current_state);
            }
        }
        
        // Select randomly the best successor
        if (nsuccessors != 0) {
            int random_successor = rand() % nsuccessors;
            setBuffer(successors[random_successor]);
        }
        
        // Update current state
        for (i=0; i<nqueens; i++) {
            current_state[i] = queens_buffer[i];
        }
        
        iterations++;
    }
    if (evaluateBuffer() == optimum) {
        printf ("Solved puzzle.\n");
        printf ("Solution:");
        printState();
        solutions_found++;
    }
    else {
        printf("Puzzle not solved (maximum number of iterations). Final state:\n");
        printState();
    }
}

/*************************************************************/
double timeToTemperature(clock_t clock_start) {
    clock_t clock_end = clock();
    long int clock_diff = clock_end - clock_start;
    if (clock_diff == 0) {
        return 999999999;
    }
    
    return INITIAL_TEMPERATURE - ((double)clock_diff/((double)CLOCKS_PER_SEC));
}

void simulatedAnnealing() {
    clock_t clock_start;
    int optimum = (nqueens-1)*nqueens/2;
    int iterations = 0;
    double current_temperature = INITIAL_TEMPERATURE;
    
    // Starts the clock counter
    clock_start = clock();
    while (evaluateBuffer() != optimum && current_temperature > 0.005) {
        // Save current state
        int current_evaluation = evaluateBuffer();
        int current_state[nqueens], i;
        for (i=0; i<nqueens; i++) {
            current_state[i] = queens_buffer[i];
        }
        setBuffer(current_state);
        
        // Make a random move
        int random_queen = rand() % nqueens;
        int random_column = rand() % nqueens;
        moveQueen(random_queen, random_column);
        int successor_evaluation = evaluateBuffer();
        
        int delta = successor_evaluation - current_evaluation;
        // If the chosen successor is better than the current state
        if (delta > 0) {
            // do nothing
        }
        else {
            double probability_of_change = exp(delta/current_temperature);
            double random_number = (rand() % 100) / (double)100;
            if (random_number < probability_of_change) {
                // do nothing
            }
            else { // Else, revert state to the original one
                setBuffer(current_state);
            }
        }
        
        // Update temperature
        current_temperature = timeToTemperature(clock_start);
        iterations++;
    }
    if (evaluateBuffer() == optimum) {
        printf ("Solved puzzle.\n");
        printf ("Solution:");
        printState();
        solutions_found++;
    }
    else {
        printf("Not solved in this iteration. Final state:");
        printState();
    }
}


int main(int argc, char *argv[]) {
    int algorithm, times_to_execute=1;

    do {
        printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
        scanf ("%d", &nqueens);
    } while ((nqueens < 1) || (nqueens > MAXQ));

    do {
        printf ("Algorithm: (1) Random search  (2) Hill climbing  ");
        printf ("(3) Simulated Annealing  (4) Genetic Algorithm: ");
        scanf ("%d", &algorithm);
    } while ((algorithm < 1) || (algorithm > 4));
    
    if (algorithm != 4) {
        do {
            printf("How many times to execute? (1<n<10000):");
            scanf("%d", &times_to_execute);
        } while ((times_to_execute < 1 || times_to_execute > 10000));
    }
  
    initializeRandomGenerator();
    
    int i=0;
    switch (algorithm) {
        case 1: randomSearch();       break;
        case 2:
            solutions_found = 0;
            for (i=0; i<times_to_execute; i++) {
                initiateQueens(1);
                hillClimbing();
            }
            printf("Algorithm finished. Found a solution in %d out of %d executions.\n", solutions_found, i);
            break;
        case 3:
            solutions_found = 0;
            for(i = 0; i < times_to_execute; i++)
            {
                initiateQueens(1);
                simulatedAnnealing();
            }
            printf("Algorithm finished. Found a solution in %d out of %d executions.\n", solutions_found, i);
            break;
        case 4:
            initiateQueens(1);
            geneticAlgorithm();
    }

    return 0;  
}