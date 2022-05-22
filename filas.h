#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define disc 0
#define magnetic_tape 1
#define printer 2

int io_time[] = {5, 15, 20};

typedef struct io{
	int type;
	int time;
} io;

typedef struct process{
	int pid;
	int status;
	int priority;
	
	int arrival_time;
	int cpu_time;
	
	io *ios;
	int ioQtd;
	
} process;


// printing data of process
void printProcess(process *p){
	if(p == NULL)
		printf("Empty process\n");
	else {
		printf("Process with PID %d\n", p->pid);	
	}

}

// creating new process
process * newProcess(int pid, int numberOfIo){
	//allocating memory
	process * p = (process *) malloc (sizeof (process));
	
	if(p == NULL)
		return NULL;
	
	p -> pid = pid;
	p -> status = 0;
	p -> priority = 0;
	p -> arrival_time = 0;
	p -> cpu_time = 0;
	
	p->ioQtd = numberOfIo;
	if(numberOfIo > 0)
		p->ios = (io *) malloc(numberOfIo * sizeof(io));
	
		
	return p;
}

typedef struct queueNode{
	process *p;
	struct queueNode *next;
} queueNode;


typedef struct queue{
	queueNode *head;
	queueNode *back;
} queue;


queue *initQueue(){
	queue *q = (queue *) malloc (sizeof(queue));
	q -> head = NULL;
	q -> back = NULL;
	return q;
}

int empty(queue *q){
	if(q->head == NULL)
		return 1;
	return 0;
}

void enqueue(queue * q, process *p){
	if( empty(q) ){
		q -> head = (queueNode *) malloc (sizeof(queueNode));
		q -> head -> next = NULL;
		q -> head -> p = p;
		q -> back = q -> head;	
	} else {
		q -> back -> next = (queueNode *) malloc (sizeof(queueNode));
		q -> back -> next -> next = NULL;
		q -> back -> next -> p = p;
		q -> back = q -> back -> next;
	}
}



process *front(queue *q){
	if( empty(q) )
		return NULL;
	return q -> head -> p;
}


void pop(queue *q){
	if( empty(q) ){
		return;
	}
	// only one element
	if(q -> head == q -> back){
		free(q -> head);
		q -> head = q -> back = NULL;
	}
	else{
		queueNode *newHead = q -> head -> next;
		free(q -> head);
		q -> head = newHead;
	}
}



