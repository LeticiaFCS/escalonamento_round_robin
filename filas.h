#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define debug 

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
	
	//int total_time;
	int used_cpu_time;
	
	io *ios;
	int io_qtd;
	int next_io;
	
} process;

// freeing process
void free_process(process *p){
	free(p -> ios);
	free(p);
}

// printing data of process
void print_process(process *p){
	if(p == NULL)
		printf("\tEmpty process\n\n");
	else {
		printf("\tProcess with PID %d used: %d total: %d\n\n", p->pid, p->used_cpu_time, p->cpu_time);	
	}

}

//gets next integer from the line spliting in the next ";"
int next_int(char * line, int *begin){
	int x = 0;
	int i = *begin;
	while(line[i] != ';'){
		x = 10 * x + line[i] - '0';
		i++;
	}
	*begin = i+1;
	return x;
}

//creates new process from the line of file
process * new_process(char * line, int *time){
	printf("new process %s", line);
	int *i = (int *) malloc(sizeof(int));
	*i = 0;
	int pid = next_int(line, i);
	printf("pid %d\n", pid);
	int arrival_time = next_int(line, i);
	*time = arrival_time;
	printf("arrival_time %d\n", arrival_time);
	int cpu_time = next_int(line, i);
	printf("cpu_time %d\n", cpu_time);
	int number_of_io = next_int(line, i);
	printf("number_of_io %d\n", number_of_io);
	
	
	
	//allocating memory
	process * p = (process *) malloc (sizeof (process));
	if(p == NULL)
		return NULL;
	
	p -> pid = pid;
	p -> status = 0;
	p -> priority = 0;
	p -> push_time = arrival_time;
	p -> cpu_time = cpu_time;
	
	//p -> total_time = cpu_time;
	p -> used_cpu_time = 0;
	
	p->io_qtd = number_of_io;
		
	if(number_of_io > 0){
		p->ios = (io *) malloc(number_of_io * sizeof(io));

		for(int j=0; j< number_of_io; j++){
			int type_of_io = next_int(line, i);
			int time_of_io = next_int(line, i);
			printf("\t type_of_io %d time_of_io %d\n",type_of_io,time_of_io);			
			p->ios[j].time = time_of_io;
			p->ios[j].type = type_of_io;
		}
	}
	p->next_io = 0;
	
	
	puts(" ");
	
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


//inits queue
queue *init_queue(){
	queue *q = (queue *) malloc (sizeof(queue));
	q -> head = NULL;
	q -> back = NULL;
	return q;
}

//tests if queue is empty
int empty(queue *q){
	if(q->head == NULL)
		return 1;
	return 0;
}

//freeing a queue
void free_queue(queue *q){
	if(q != NULL){
		if(q -> head != NULL){
			free(q -> head);
			q -> head = NULL;
		}if(q -> back != NULL){
			free(q -> back);
			q -> back = NULL;
		}
		free(q);
		q = NULL;
	}
}

//push a new process to the queue
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


//returns the process in front of the queue or NULL if queue is empty
process *front(queue *q){
	if( empty(q) )
		return NULL;
	return q -> head -> p;
}

//pops element of queue if it is not empty
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



