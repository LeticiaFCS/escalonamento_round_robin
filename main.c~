#include<stdio.h>
#include"filas.h"

#define quantum 4

queue *qarrival, *qhigh_priority, *qlow_priority, *qdisc_io, *qmagnetic_io, *qprinter;
int number_of_process;

void init(){
	qarrival = init_queue(); // read from file
	number_of_process = 5;
	push(qarrival, new_process(1,  0, 13, 0));
	push(qarrival, new_process(2,  4, 11, 0));
	push(qarrival, new_process(3,  5, 7, 0));
	push(qarrival, new_process(4,  7, 8, 0));
	push(qarrival, new_process(5, 10, 16, 0));
	qhigh_priority = init_queue();
	qlow_priority = init_queue();
	qdisc_io = init_queue();
	qmagnetic_io = init_queue();
	qprinter = init_queue();
}

int min(int x, int y){
	if( x < y ) return x;
	return y;
}

void round_robin(){
	int finished_process = 0;
	int cpu_time = 0;
	while(finished_process < number_of_process){
		printf("CPU TIME: %d\n", cpu_time);
		// add new arrival process
		while( !empty(qarrival) && front(qarrival)-> arrival_time  <= cpu_time ){
			printf("Arrived:\n");
			process *arrivalProcess = front(qarrival);
			print_process(arrivalProcess);
			pop(qarrival);
			push(qhigh_priority, arrivalProcess);
		}
		
		if( !empty(qhigh_priority) ){
			process *next_process = front(qhigh_priority);
			pop(qhigh_priority);
			int delta_time = min(quantum, next_process -> cpu_time - next_process -> used_cpu_time);
			next_process -> used_cpu_time += delta_time;
			
			printf("Executing (high priority):\n");
			print_process(next_process);
			//still needs time to finish
			if(next_process -> used_cpu_time < next_process -> cpu_time){
				push(qlow_priority, next_process);				
			} else {
				printf("Finished!\n");
				finished_process++;
			}		
			cpu_time += delta_time;
		} else if( !empty(qlow_priority) ){
			process *next_process = front(qlow_priority);
			pop(qlow_priority);
			int delta_time = min(quantum, next_process -> cpu_time - next_process -> used_cpu_time);
			next_process -> used_cpu_time += delta_time;
			
			printf("Executing (low priority):\n");
			print_process(next_process);
			//still needs time to finish
			if(next_process -> used_cpu_time < next_process -> cpu_time){
				push(qlow_priority, next_process);				
			} else {
				printf("Finished!\n");
				finished_process++;
			}	
			cpu_time += delta_time;		
		
		}
	}
	
	printf("Simulacao finalizada no tempo %d\n", cpu_time);
	
}

int main(){	
	init();
	round_robin();
	
}

