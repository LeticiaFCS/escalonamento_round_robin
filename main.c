#include<stdio.h>
#include"filas.h"

#define quantum 4

queue *qarrival, *qhigh_priority, *qlow_priority, *qdisc_io, *qmagnetic_io, *qprinter_io, *qauxiliar;
int number_of_process;

void init(){
	qarrival = init_queue(); // read from file
	//number_of_process = 5;
	debug("init\n");
	//push(qarrival, new_process(1,  0, 13, 1), 0);
	//push(qarrival, new_process(2,  4, 11, 0), 4);
	//push(qarrival, new_process(3,  5, 7, 0), 5);
	//push(qarrival, new_process(4,  7, 8, 0), 7);
	//push(qarrival, new_process(5, 10, 16, 0), 10);
	
	number_of_process = 3; 
	push(qarrival, new_process(1,  0, 31, 2), 0); //2 //22
	push(qarrival, new_process(2,  2, 7, 0), 2);
	push(qarrival, new_process(3,  3, 23, -1), 3); //2
	debug("criou processos\n");
	qhigh_priority = init_queue();
	qlow_priority = init_queue();
	qdisc_io = init_queue();
	qmagnetic_io = init_queue();
	qprinter_io = init_queue();
}

int min(int x, int y){
	if( x < y ) return x;
	return y;
}
int max(int x, int y){
	if( x > y ) return x;
	return y;
}


void round_robin(){
	int finished_process = 0;
	int clock_time = 0;
	int disc_time = 0;
	int printer_time = 0;
	int magnetic_time = 0;
	while(finished_process < number_of_process){
		printf("CLOCK TIME: %d\n", clock_time);
		 
		while(!empty(qdisc_io)){
			disc_time = max(disc_time, front(qdisc_io) -> push_time);
			if(disc_time + io_time[DISC] <= clock_time){
				printf("Process %d using DISC at time %d\n", front(qdisc_io) -> pid, disc_time);
				disc_time += io_time[DISC];
				push(qlow_priority, front(qdisc_io) , disc_time);	
				pop(qdisc_io);		
			} else {
				break;
			}			
		}
		
		while(!empty(qmagnetic_io)){
			magnetic_time = max(magnetic_time, front(qmagnetic_io) -> push_time);
			if(magnetic_time + io_time[MAGNETICTAPE] <= clock_time){
				printf("Process %d using MAGNETIC TAPE at time %d\n", front(qmagnetic_io) -> pid, magnetic_time);
				magnetic_time += io_time[MAGNETICTAPE];
				push(qhigh_priority, front(qmagnetic_io) , magnetic_time);	
				pop(qmagnetic_io);		
			} else {
				break;
			}			
		}
		
		while(!empty(qprinter_io)){
			printer_time = max(printer_time, front(qprinter_io) -> push_time);
			if(printer_time + io_time[PRINTER] <= clock_time){
				printf("Process %d using PRINTER at time %d\n", front(qprinter_io) -> pid, printer_time);
				print_process(front(qprinter_io));
				printer_time += io_time[PRINTER];
				push(qhigh_priority, front(qprinter_io) , printer_time);	
				pop(qprinter_io);
					
			} else {
				break;
			}			
		}
		

		
			
		// add new arrival process
		while( !empty(qarrival) && front(qarrival)-> push_time  <= clock_time ){
			printf("Arrived:\n");
			process *arrivalProcess = front(qarrival);
			print_process(arrivalProcess);
			pop(qarrival);
			push(qhigh_priority, arrivalProcess, arrivalProcess -> push_time);
		}
		
		
		
		if( !empty(qhigh_priority) ){ //executing process from high priority queue
			process *next_process = front(qhigh_priority);
			pop(qhigh_priority);
			int delta_time = min(quantum, next_process -> cpu_time - next_process -> used_cpu_time);
			
			int time_until_next_io = -1;
			if(next_process -> next_io < next_process -> io_qtd) // still have ios)
				time_until_next_io = next_process -> ios[next_process -> next_io] . time - next_process -> used_cpu_time;
			
			if( time_until_next_io != -1 && delta_time >= time_until_next_io ){
				
				delta_time = time_until_next_io;
				next_process -> next_io++;
				next_process -> used_cpu_time += delta_time;			
				printf("Executing (high priority):\n");
				print_process(next_process);
				//still needs time to finish		
				clock_time += delta_time;
				if(next_process -> ios[next_process -> next_io - 1] . type == DISC ){
					debug("push disc\n");
					push(qdisc_io, next_process, clock_time);
				} else if (next_process -> ios[next_process -> next_io - 1] . type == MAGNETICTAPE){
					debug("push tape\n");
					push(qmagnetic_io, next_process, clock_time);				
				} else { // printer
					debug("H push printer %d\n", clock_time);
					push(qprinter_io, next_process, clock_time);				
				}
				
				
							
			} else {
				
				next_process -> used_cpu_time += delta_time;
				
				printf("Executing (high priority):\n");
				print_process(next_process);
				//still needs time to finish		
				clock_time += delta_time;
				if(next_process -> used_cpu_time < next_process -> cpu_time){
					push(qlow_priority, next_process, clock_time);				
				} else {
					printf("Finished!\n");
					finished_process++;
				}
			}
		} else if( !empty(qlow_priority) ){ //executing process from low priority queue
			process *next_process = front(qlow_priority);
			pop(qlow_priority);	
			int delta_time = min(quantum, next_process -> cpu_time - next_process -> used_cpu_time);
			
			int time_until_next_io = -1;
			if(next_process -> next_io < next_process -> io_qtd) // still have ios)
				time_until_next_io = next_process -> ios[next_process -> next_io] . time - next_process -> used_cpu_time;
			
			if( time_until_next_io != -1 && delta_time >= time_until_next_io ){
				
				delta_time = time_until_next_io;
				
				
				next_process -> next_io++;
				next_process -> used_cpu_time += delta_time;			
				printf("Executing (low priority):\n");
				print_process(next_process);
				//still needs time to finish		
				clock_time += delta_time;
				if(next_process -> ios[next_process -> next_io - 1] . type == DISC ){
					push(qdisc_io, next_process, clock_time);
				} else if (next_process -> ios[next_process -> next_io - 1] . type == MAGNETICTAPE){
					push(qmagnetic_io, next_process, clock_time);				
				} else { // printer
					debug("L push printer %d\n", clock_time);
					push(qprinter_io, next_process, clock_time);				
				}
				
				
							
			} else {
				
				
				next_process -> used_cpu_time += delta_time;
				
				printf("Executing (low priority):\n");
				print_process(next_process);
				//still needs time to finish		
				
				
				if(next_process -> used_cpu_time < next_process -> cpu_time){
					push(qlow_priority, next_process, clock_time);				
				} else {
					printf("Finished!\n");
					finished_process++; 
				}
			}		
		
		} else {
			// all process are in IO or didn't arrived
			clock_time ++;		
		}
	}
	
	printf("Simulacao finalizada no tempo %d\n", clock_time);
	
}

int main(){	
	init();
	round_robin();
	
}


/*
	push_time - momento que um processo entra na fila

*/

