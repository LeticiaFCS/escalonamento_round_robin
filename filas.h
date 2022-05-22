#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define disc 0
#define magneticTape 1
#define printer 2

int io_time[] = {4, 12, 20};

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
	int used_cpu_time;
	
	
	io *ios;
	int io_qtd;
	
} process;


// printing data of process
void print_process(process *p){
	if(p == NULL)
		printf("\tEmpty process\n\n");
	else {
		printf("\tProcess with PID %d\n\n", p->pid);	
	}

}

// creating new process
process * new_process(int pid, int arrival_time, int cpu_time, int number_of_io){
	//allocating memory
	process * p = (process *) malloc (sizeof (process));
	
	if(p == NULL)
		return NULL;
	
	p -> pid = pid;
	p -> status = 0;
	p -> priority = 0;
	p -> arrival_time = arrival_time;
	p -> cpu_time = cpu_time;
	p -> used_cpu_time = 0;
	
	p->io_qtd = number_of_io;
	if(number_of_io > 0)
		p->ios = (io *) malloc(number_of_io * sizeof(io));
	
		
	return p;
}

typedef struct queue_node{
	process *p;
	struct queue_node *next;
} queue_node;


typedef struct queue{
	queue_node *head;
	queue_node *back;
} queue;


queue *init_queue(){
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

void push(queue * q, process *p){
	if( empty(q) ){
		q -> head = (queue_node *) malloc (sizeof(queue_node));
		q -> head -> next = NULL;
		q -> head -> p = p;
		q -> back = q -> head;	
	} else {
		q -> back -> next = (queue_node *) malloc (sizeof(queue_node));
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
		queue_node *new_head = q -> head -> next;
		free(q -> head);
		q -> head = new_head;
	}
}



