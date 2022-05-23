#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define debug printf

#define DISC 0
#define MAGNETICTAPE 1
#define PRINTER 2

int io_time[] = {4, 12, 20};

typedef struct io{
	int type;
	int time;
} io;

typedef struct process{
	int pid;
	int status;
	int priority;
	
	int push_time;
	int cpu_time;
	
	int total_time;
	int used_time;
	
	io *ios;
	int io_qtd;
	int next_io;
	
} process;


// printing data of process
void print_process(process *p){
	if(p == NULL)
		printf("\tEmpty process\n\n");
	else {
		printf("\tProcess with PID %d used: %d total: %d\n\n", p->pid, p->used_time, p->total_time);	
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
	p -> push_time = arrival_time;
	p -> cpu_time = cpu_time;
	
	p -> total_time = cpu_time;
	p -> used_time = 0;
	
	p->io_qtd = number_of_io;
		
	if(number_of_io > 0){
		p->ios = (io *) malloc(number_of_io * sizeof(io));

		for(int i=0; i< number_of_io; i++){
			if(i == 0)
				p->ios[i].time = 1;
			else if(i == 1)
				p->ios[i].time = 30;
			p->ios[i].type = PRINTER;
			p -> total_time += io_time[ p->ios[i].type ];
		}
	}
	p->next_io = 0;
	
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

void push(queue * q, process *p, int time){
	if( empty(q) ){
		q -> head = (queue_node *) malloc (sizeof(queue_node));
		q -> head -> p = p;
		q -> head -> p -> push_time = time;
		q -> head -> next = NULL;
		q -> back = q -> head;	
	} else {
		q -> back -> next = (queue_node *) malloc (sizeof(queue_node));
		q -> back -> next -> p = p;
		q -> back -> next -> p -> push_time = time;
		q -> back -> next -> next = NULL;
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



