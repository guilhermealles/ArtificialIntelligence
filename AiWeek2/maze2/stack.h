#ifndef STACK_H
#define STACK_H

#include "prioqueue.h"

typedef struct Stack {
	Pqueue queue;
} Stack;

void initStack(Stack *stack);
int getStackSize(Stack stack);
int isEmptyStack(Stack stack);
void pushStack(Stack *stack, State s);
void topStack(Stack stack, State *state);
void popStack(Stack *stack);

#endif