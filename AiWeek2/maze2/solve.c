#include <stdio.h>
#include <stdlib.h>
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
	priority = random();
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
				priority = random();
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
	Maze solved_maze = maze;
	int* visit_count_ptr = &visitCount;
	int* push_count_ptr = &pushCount;

	while(!isSolved(solved_maze)) {
		printf("1\n");
		solved_maze = solveMazeDLS(maze, depth_count, visit_count_ptr, push_count_ptr);
		depth_count++;
	}
	if (isSolved(solved_maze)) {
		printf ("MAZE SOLVED: Path=");
		printPathWithNewline(solved_maze.path);
		printf("\n\nvisitCount = %d \t pushCount = %d.\n", visitCount, pushCount);
	}
	else {
		printf ("NO PATH FOUND\n");
	}
}

// Returns a solved maze if it was possible to solve
Maze solveMazeDLS(Maze maze, int depth_limit, int *partial_visit_count, int *partial_push_count) {
	Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
	Pqueue fringe, visited;
	Position pos, goal;
	State state, successor;
	int i, priority;
	int visitCount=0, pushCount=0, depth_count = 0;

	getPosition(maze, &pos);
	getGoal(maze, &goal);

	getMazeState(&state, maze);
	priority = getPqueueSize(fringe);
	setStatePriority(&state, priority);

	initPqueue(&fringe);
	pushPqueue(&fringe, state);

	while (!isEmptyPqueue(fringe) && !isSolved(maze) && depth_count<depth_limit)
	{
		depth_count++;
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

	*partial_visit_count = visitCount;
	*partial_push_count = pushCount;
	return maze;
}