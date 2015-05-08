void initStack(Stack *stack) {
	initPqueue(stack->queue);
}

int getStackSize(Stack stack) {
	return getPqueueSize(stack.queue);
}

int isEmptyStack(Stack stack) {
	return isEmptyPqueue(stack.queue);
}

void pushStack(Stack *stack, State s) {
	setStatePriority(&s, getStackSize(stack));
	pushPqueue(stack->queue, s);
}

void topStack(Stack stack, State *s) {
	topPqueue(stack.queue, &s);
}

void popStack(Stack *stack) {
	popPqueue(stack->queue);
}