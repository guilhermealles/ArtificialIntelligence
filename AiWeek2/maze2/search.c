void solveMazeDFS(Maze maze) {
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
		printState(state);
		for (i=0; i<6; i++)
		{
			if (canMoveTo(maze, dir[i]))
			{
        		/* make move in direction dir[i] */
				makeMove(&maze, dir[i]);
        /* show result */
#if 0
        printf("makeMove %s\n", convertDirectionToString(dir[i])); 
        printMaze(maze);
#endif
        		/* get new state (i.e. is a successor) */
        		getMazeState(&successor, maze);
        /* show new state */
#if 0
        printState(successor);
#endif
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

void solveMazeBFS(Maze maze) {
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

	while (!isEmptyPqueue(fringe) && !isSolved(maze))
	{
		visitCount++;
		topPqueue(fringe, &state);
		popPqueue(&fringe);  
		setMazeState(&maze, state);    
		printState(state);
		for (i=0; i<6; i++)
		{
			if (canMoveTo(maze, dir[i]))
			{
        		/* make move in direction dir[i] */
				makeMove(&maze, dir[i]);
        /* show result */
#if 0
        printf("makeMove %s\n", convertDirectionToString(dir[i])); 
        printMaze(maze);
#endif
        		/* get new state (i.e. is a successor) */
        		getMazeState(&successor, maze);
        /* show new state */
#if 0
        printState(successor);
#endif
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