#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"

void solveMaze(Maze maze) {
	Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
	Pqueue fringe;
	Position pos, goal;
	State state, successor;
	int i, priority;

	int visitCount=0, pushCount=0;

	getPosition(maze, &pos);
	getGoal(maze, &goal);

	getMazeState(&state, maze);
	priority = (int)random();
	setStatePriority(&state, priority);

	initPqueue(&fringe);
	pushPqueue(&fringe, state);

	while (!isEmptyPqueue(fringe) && !isSolved(maze)) {
		visitCount++;
		topPqueue(fringe, &state);
		popPqueue(&fringe);  
		setMazeState(&maze, state);    
		printState(state);
		for (i=0; i<6; i++) {
			if (canMoveTo(maze, dir[i])) {
        		/* make move in direction dir[i] */
				makeMove(&maze, dir[i]);

        		/* set priority of successor an put it in the fringe */
				priority = (int)random();
				setStatePriority(&successor, priority);
				pushPqueue(&fringe, successor);
				pushCount++;
        		/* undo move */
				setMazeState(&maze, state);
			}
		}
	}
	if (isSolved(maze)) {
		printf ("MAZE SOLVED: Path=");
		printPathWithNewline(maze.path);
		printf("\n\nvisitCount = %d \t pushCount = %d.\n", visitCount, pushCount);
	}
	else {
		printf ("NO PATH FOUND\n");
	}
}

void solveMazeDFS(Maze maze) {
	Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
	Pqueue fringe, visited;
	Position pos, goal;
	State state, successor;
	int i, priority;

	int visitCount=0, pushCount=0;

	getPosition(maze, &pos);
	getGoal(maze, &goal);

	getMazeState(&state, maze);
	priority = getPqueueSize(fringe);
	setStatePriority(&state, priority);

	initPqueue(&fringe);
	pushPqueue(&fringe, state);
    
    initPqueue(&visited);

	while (!isEmptyPqueue(fringe) && !isSolved(maze))
	{
		visitCount++;
		topPqueue(fringe, &state);
		popPqueue(&fringe);  
		setMazeState(&maze, state); 
		// Add current state to the visited list
		pushPqueue(&visited, state);   
		printState(state);
		for (i=0; i<6; i++)
		{
			if (canMoveTo(maze, dir[i]))
			{
        		/* make move in direction dir[i] */
				makeMove(&maze, dir[i]);
        		/* get new state (i.e. is a successor) */
				getMazeState(&successor, maze);		
        		// If successor has not yet been visited, add it to the fringe. Otherwise, just ignore
				if (existsInPqueue(visited, successor) == 0)
				{
        			/* set priority of successor an put it in the fringe */
					priority = getPqueueSize(fringe);
					setStatePriority(&successor, priority);
					pushPqueue(&fringe, successor);
					pushCount++;
				}
        		/* undo move */
				setMazeState(&maze, state);
			}
		}
	}
	if (isSolved(maze)) {
		printf ("MAZE SOLVED: Path=");
		printPathWithNewline(maze.path);
		printf("\n\nvisitCount = %d \t pushCount = %d.\n", visitCount, pushCount);
	}
	else {
		printf ("NO PATH FOUND\n");
	}
}

void solveMazeBFS(Maze maze) {
	Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
	Pqueue fringe, visited;
	Position pos, goal;
	State state, successor;
	int i, priority, fifo_counter=0;

	int visitCount=0, pushCount=0;

	getPosition(maze, &pos);
	getGoal(maze, &goal);

	getMazeState(&state, maze);
	priority = --fifo_counter;
	setStatePriority(&state, priority);

	initPqueue(&fringe);
	pushPqueue(&fringe, state);
    
    initPqueue(&visited);

	while (!isEmptyPqueue(fringe) && !isSolved(maze))
	{
		visitCount++;
		topPqueue(fringe, &state);
		popPqueue(&fringe);
		fifo_counter++;
		setMazeState(&maze, state); 
		// Add current state to the visited list
		pushPqueue(&visited, state);   
		printState(state);
		for (i=0; i<6; i++)
		{
			if (canMoveTo(maze, dir[i]))
			{
        		/* make move in direction dir[i] */
				makeMove(&maze, dir[i]);
        		/* get new state (i.e. is a successor) */
				getMazeState(&successor, maze);		
        		// If successor has not yet been visited, add it to the fringe. Otherwise, just ignore
				if (existsInPqueue(visited, successor) == 0)
				{
        			/* set priority of successor an put it in the fringe */
					priority = --fifo_counter;
					setStatePriority(&successor, priority);
					pushPqueue(&fringe, successor);
					pushCount++;
				}
        		/* undo move */
				setMazeState(&maze, state);
			}
		}
	}
	if (isSolved(maze)) {
		printf ("MAZE SOLVED: Path=");
		printPathWithNewline(maze.path);
		printf("\n\nvisitCount = %d \t pushCount = %d.\n", visitCount, pushCount);
	}
	else {
		printf ("NO PATH FOUND\n");
	}
}

void solveMazeIDA(Maze maze) {
	int depth_count = 1, visitCount=0, pushCount=0;
	int* visit_count_ptr = &visitCount;
	int* push_count_ptr = &pushCount;
    
    Maze *maze_copy = malloc(sizeof(Maze));
    *maze_copy = maze;

	while(!isSolved(*maze_copy)) {
        *maze_copy = maze;
        solveMazeDLS(maze_copy, depth_count, visit_count_ptr, push_count_ptr);
		depth_count++;
	}
	if (isSolved(*maze_copy)) {
		printf ("MAZE SOLVED: Path=");
		printPathWithNewline(maze_copy->path);
		printf("\n\nvisitCount = %d \t pushCount = %d\t solution found at depth %d.\n", visitCount, pushCount, depth_count);
	}
	else {
		printf ("NO PATH FOUND\n");
	}
}

// Returns a solved maze if it was possible to solve
void solveMazeDLS(Maze *maze, int depth_limit, int *visit_count_ptr, int *push_count_ptr) {
	Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
	Pqueue fringe, visited;
	Position pos, goal;
	State state, successor;
	int i, priority;
	int visitCount=0, pushCount=0, depth_count = 0;
    
	getPosition(*maze, &pos);
	getGoal(*maze, &goal);

	getMazeState(&state, *maze);
	priority = getPqueueSize(fringe);
	setStatePriority(&state, priority);

	initPqueue(&fringe);
	pushPqueue(&fringe, state);
    
    initPqueue(&visited);
    
	while (!isEmptyPqueue(fringe) && !isSolved(*maze))
	{
		depth_count++;
		visitCount++;
		topPqueue(fringe, &state);
		popPqueue(&fringe);  
		setMazeState(maze, state);
		// Add current state to the visited list
		pushPqueue(&visited, state);   
		printState(state);
		for (i=0; i<6; i++)
		{
			if (canMoveTo(*maze, dir[i]))
			{
        		/* make move in direction dir[i] */
				makeMove(maze, dir[i]);
        		/* get new state (i.e. is a successor) */
				getMazeState(&successor, *maze);
        		// If successor has not yet been visited, add it to the fringe. Otherwise, just ignore
				if (existsInPqueue(visited, successor) == 0 && successor.cost<depth_limit)
				{
        			/* set priority of successor an put it in the fringe */
					priority = getPqueueSize(fringe);
					setStatePriority(&successor, priority);
					pushPqueue(&fringe, successor);
					pushCount++;
				}
        		/* undo move */
				setMazeState(maze, state);
			}
		}
	}
	*visit_count_ptr += visitCount;
	*push_count_ptr += pushCount;
}

void solveMazeAstar(Maze maze) {
    Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
    Pqueue fringe, visited;
    Position pos, goal;
    State state, successor;
    int i, priority;
    
    int visitCount=0, pushCount=0;
    
    getPosition(maze, &pos);
    getGoal(maze, &goal);
    
    getMazeState(&state, maze);
    priority = aStarEvaluate(state, goal);
    setStatePriority(&state, priority);
    
    initPqueue(&fringe);
    pushPqueue(&fringe, state);
    
    initPqueue(&visited);
    
    while (!isEmptyPqueue(fringe) && !isSolved(maze))
    {
        visitCount++;
        topPqueue(fringe, &state);
        popPqueue(&fringe);
        setMazeState(&maze, state);
        // Add current state to the visited list
        pushPqueue(&visited, state);
        printState(state);
        for (i=0; i<6; i++)
        {
            if (canMoveTo(maze, dir[i]))
            {
                /* make move in direction dir[i] */
                makeMove(&maze, dir[i]);
                /* get new state (i.e. is a successor) */
                getMazeState(&successor, maze);
                // If successor has not yet been visited, add it to the fringe. Otherwise, just ignore
                if (existsInPqueue(visited, successor) == 0)
                {
                    /* set priority of successor an put it in the fringe */
                    priority = aStarEvaluate(successor, goal);
                    setStatePriority(&successor, priority);
                    pushPqueue(&fringe, successor);
                    pushCount++;
                }
                /* undo move */
                setMazeState(&maze, state);
            }
        }
    }
    if (isSolved(maze)) {
        printf ("MAZE SOLVED: Path=");
        printPathWithNewline(maze.path);
        printf("\n\nvisitCount = %d \t pushCount = %d.\n", visitCount, pushCount);
    }
    else {
        printf ("NO PATH FOUND\n");
    }
}

int aStarEvaluate(State s, Position goal) {
    Position pos;
    getStatePosition(s, &pos);
    return -(s.cost + (int)sqrt(((goal.x-pos.x)*(goal.x-pos.x))+((goal.y-pos.y)*(goal.y-pos.y))+((goal.z-pos.z)*(goal.z-pos.z)))); // The return value is negated so that the lowest evaluation value becomes the highest priority.
}

/*
================================================= IMPLEMENTATION WITHOUT CLOSED LIST =====================================
*/

void solveMazeDFSNC(Maze maze) {
    Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
    Pqueue fringe;
    Position pos, goal;
    State state, successor;
    int i, priority;
    
    int visitCount=0, pushCount=0;
    
    getPosition(maze, &pos);
    getGoal(maze, &goal);
    
    getMazeState(&state, maze);
    priority = getPqueueSize(fringe);
    setStatePriority(&state, priority);
    
    initPqueue(&fringe);
    pushPqueue(&fringe, state);
    
    while (!isEmptyPqueue(fringe) && !isSolved(maze))
    {
        visitCount++;
        topPqueue(fringe, &state);
        popPqueue(&fringe);
        setMazeState(&maze, state);
        //printState(state);
        for (i=0; i<6; i++)
        {
            if (canMoveTo(maze, dir[i]))
            {
                /* make move in direction dir[i] */
                makeMove(&maze, dir[i]);
                /* get new state (i.e. is a successor) */
                getMazeState(&successor, maze);
                /* set priority of successor an put it in the fringe */
                priority = getPqueueSize(fringe);
                setStatePriority(&successor, priority);
                pushPqueue(&fringe, successor);
                pushCount++;
                /* undo move */
                setMazeState(&maze, state);
            }
        }
    }
    if (isSolved(maze)) {
        printf ("MAZE SOLVED: Path=");
        printPathWithNewline(maze.path);
        printf("\n\nvisitCount = %d \t pushCount = %d.\n", visitCount, pushCount);
    }
    else {
        printf ("NO PATH FOUND\n");
    }
}

void solveMazeBFSNC(Maze maze) {
    Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
    Pqueue fringe;
    Position pos, goal;
    State state, successor;
    int i, priority, fifo_counter=0;
    
    int visitCount=0, pushCount=0;
    
    getPosition(maze, &pos);
    getGoal(maze, &goal);
    
    getMazeState(&state, maze);
    priority = --fifo_counter;
    setStatePriority(&state, priority);
    
    initPqueue(&fringe);
    pushPqueue(&fringe, state);
    
    while (!isEmptyPqueue(fringe) && !isSolved(maze))
    {
        visitCount++;
        topPqueue(fringe, &state);
        popPqueue(&fringe);
        fifo_counter++;
        setMazeState(&maze, state);
        //printState(state);
        for (i=0; i<6; i++)
        {
            if (canMoveTo(maze, dir[i]))
            {
                /* make move in direction dir[i] */
                makeMove(&maze, dir[i]);
                /* get new state (i.e. is a successor) */
                getMazeState(&successor, maze);
                /* set priority of successor an put it in the fringe */
                priority = --fifo_counter;
                setStatePriority(&successor, priority);
                pushPqueue(&fringe, successor);
                pushCount++;
                /* undo move */
                setMazeState(&maze, state);
            }
        }
    }
    if (isSolved(maze)) {
        printf ("MAZE SOLVED: Path=");
        printPathWithNewline(maze.path);
        printf("\n\nvisitCount = %d \t pushCount = %d.\n", visitCount, pushCount);
    }
    else {
        printf ("NO PATH FOUND\n");
    }
}

void solveMazeIDANC(Maze maze) {
    int depth_count = 1, visitCount=0, pushCount=0;
    int* visit_count_ptr = &visitCount;
    int* push_count_ptr = &pushCount;
    
    Maze *maze_copy = malloc(sizeof(Maze));
    *maze_copy = maze;
    
    while(!isSolved(*maze_copy)) {
        *maze_copy = maze;
        solveMazeDLSNC(maze_copy, depth_count, visit_count_ptr, push_count_ptr);
        depth_count++;
    }
    if (isSolved(*maze_copy)) {
        printf ("MAZE SOLVED: Path=");
        printPathWithNewline(maze_copy->path);
        printf("\n\nvisitCount = %d \t pushCount = %d\t solution found at depth %d.\n", visitCount, pushCount, depth_count);
    }
    else {
        printf ("NO PATH FOUND\n");
    }
}

// Returns a solved maze if it was possible to solve
void solveMazeDLSNC(Maze *maze, int depth_limit, int *visit_count_ptr, int *push_count_ptr) {
    Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
    Pqueue fringe;
    Position pos, goal;
    State state, successor;
    int i, priority;
    int visitCount=0, pushCount=0, depth_count = 0;
    
    getPosition(*maze, &pos);
    getGoal(*maze, &goal);
    
    getMazeState(&state, *maze);
    priority = getPqueueSize(fringe);
    setStatePriority(&state, priority);
    
    initPqueue(&fringe);
    pushPqueue(&fringe, state);
    
    while (!isEmptyPqueue(fringe) && !isSolved(*maze))
    {
        depth_count++;
        visitCount++;
        topPqueue(fringe, &state);
        popPqueue(&fringe);
        setMazeState(maze, state);
        //printState(state);
        for (i=0; i<6; i++)
        {
            if (canMoveTo(*maze, dir[i]))
            {
                /* make move in direction dir[i] */
                makeMove(maze, dir[i]);
                /* get new state (i.e. is a successor) */
                getMazeState(&successor, *maze);
                if (successor.cost<depth_limit)
                {
                    /* set priority of successor an put it in the fringe */
                    priority = getPqueueSize(fringe);
                    setStatePriority(&successor, priority);
                    pushPqueue(&fringe, successor);
                    pushCount++;
                }
                /* undo move */
                setMazeState(maze, state);
            }
        }
    }
    *visit_count_ptr += visitCount;
    *push_count_ptr += pushCount;
}

void solveMazeAstarNC(Maze maze) {
    Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
    Pqueue fringe;
    Position pos, goal;
    State state, successor;
    int i, priority;
    
    int visitCount=0, pushCount=0;
    
    getPosition(maze, &pos);
    getGoal(maze, &goal);
    
    getMazeState(&state, maze);
    priority = aStarEvaluate(state, goal);
    setStatePriority(&state, priority);
    
    initPqueue(&fringe);
    pushPqueue(&fringe, state);
    while (!isEmptyPqueue(fringe) && !isSolved(maze))
    {
        visitCount++;
        topPqueue(fringe, &state);
        popPqueue(&fringe);
        setMazeState(&maze, state);
        //printState(state);
        for (i=0; i<6; i++)
        {
            if (canMoveTo(maze, dir[i]))
            {
                /* make move in direction dir[i] */
                makeMove(&maze, dir[i]);
                /* get new state (i.e. is a successor) */
                getMazeState(&successor, maze);
                /* set priority of successor an put it in the fringe */
                priority = aStarEvaluate(successor, goal);
                setStatePriority(&successor, priority);
                pushPqueue(&fringe, successor);
                pushCount++;
                /* undo move */
                setMazeState(&maze, state);
            }
        }
    }
    if (isSolved(maze)) {
        printf ("MAZE SOLVED: Path=");
        printPathWithNewline(maze.path);
        printf("\n\nvisitCount = %d \t pushCount = %d.\n", visitCount, pushCount);
    }
    else {
        printf ("NO PATH FOUND\n");
    }
}
