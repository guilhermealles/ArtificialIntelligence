/*
    Solution of the N queens problem using genetic algorithm. The idea is having a population of board configurations (a two dimensional array) and, on every generation, creating multiple children (crossing over different pairs of configurations) and selecting the best out of the childrens set. If the population count is 5, we should select the 5 best children, and so on.
 
    Regarding mutation, still don't know what to do.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXQ 100
#define POPULATION_COUNT 5
#define NEXT_GEN_COUNT 4*3*2*1 //(POPULATION_COUNT-1)!

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens */
int queens_buffer[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */

int current_population[POPULATION_COUNT][MAXQ];
int next_generation[NEXT_GEN_COUNT][MAXQ]; //Next generation has size (POPULATION_COUNT-1)!;

void initializeRandomGenerator() {
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
        queens_buffer[q] = (flag == 0? 0 : rand()%nqueens);
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

void haveSex(int father_index, int mother_index, int children_index) {
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

void everybodyHavingSex() {
    int i, j, children_index=0;
    for (i=0; i<POPULATION_COUNT; i++) {
        for (j=i; j<POPULATION_COUNT; j++) {
			haveSex(i, j, children_index);
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

void printCurrentPopulation() {
	int i;
	for (i=0; i<POPULATION_COUNT; i++) {
		int j;
		for (j=0; j<nqueens; j++) {
			printf("%d ", current_population[i][j]);
		}
		printf("\n");
	}
}

void geneticAlgorithm() {
	int generation_count = 0;
	
	// Initializes the population
	int i;
	for (i=0; i<POPULATION_COUNT; i++) {
		initiateQueens(0);
		addBufferToPopulation(i);
	}
	
	while (TRUE) {
		generation_count++;
		everybodyHavingSex();
		selectNewPopulation();
		
		if (populationHasSolution()) {
			printf("Found a solution in generation %d:\n", generation_count);
			printSolutionFromPopulation();
			return;
		}
		else {
			//printf("Current generation #: %d\n", generation_count);
		}
	}
}


int main (int argc, char **argv) {
    do {
        printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
        scanf ("%d", &nqueens);
    } while ((nqueens < 1) || (nqueens > MAXQ));
    
    initializeRandomGenerator();
	
	for (int i=0; i<10; i++) {
		geneticAlgorithm();
	}
}