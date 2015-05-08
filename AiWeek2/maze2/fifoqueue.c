void initFifoQueue(FifoQueue *queue) {
	initPqueue(queue->queue);
}

int getFifoQueueSize(FifoQueue queue) {
	return getPqueueSize(queue.queue);
}

int isEmptyFifoQueue(FifoQueue queue) {
	return isEmptyPqueue(queue.queue);
}

void pushFifoQueue(FifoQueue *queue, State s) {
	queue->priority_counter++;
	setStatePriority(&s, queue->priority_counter);
	pushPqueue(queue->queue, s);
}

void topFifoQueue(FifoQueue queue, State *s) {
	topPqueue(queue.queue, &s);
}

void popFifoQueue(FifoQueue *queue) {
	queue->priority_counter--;
	popPqueue(queue->queue);
}