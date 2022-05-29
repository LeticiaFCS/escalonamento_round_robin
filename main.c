#include<stdio.h>
#include"filas.h"

#define quantum 4

queue *qarrival, *qhigh_priority, *qlow_priority, *qdisc_io, *qmagnetic_io, *qprinter_io;
int number_of_process;

//reads process from file nd inits queues
void init(char *file_name){

	printf("file name %s\n\n", file_name);
	qarrival = init_queue(); // read from file
	
	
	number_of_process = 0; 
	
	
	FILE * file_ptr;
	char * line = NULL;
    size_t len = 0;
    ssize_t read;
    file_ptr = fopen(file_name, "r");
    if (file_ptr == NULL){
    	printf("Didn't read anything\n");
        exit(0);
    }

    while ((read = getline(&line, &len, file_ptr)) != -1) {
    	int *time = (int *) malloc (sizeof(time));
    	process *next_process = new_process( line, time );
        push( qarrival, next_process, * time);
        number_of_process++;
    }

    fclose(file_ptr);
    if (line != NULL){
    	free(line);
	}
	
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



int finished_process = 0;
int clock_time = 0;
int disc_time = 0;
int printer_time = 0;
int magnetic_time = 0;


//tests if process returned from ios
void returning_of_ios(){
	while(!empty(qdisc_io)){
			disc_time = max(disc_time, front(qdisc_io) -> push_time);
			if(disc_time + io_time[DISC] <= clock_time){
				printf("\tProcess %d used DISC from time %d to time %d\n", front(qdisc_io) -> pid, disc_time, disc_time + io_time[DISC]);
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
			printf("\tProcess %d used MAGNETIC TAPE from time %d to time %d\n", front(qmagnetic_io) -> pid, magnetic_time, magnetic_time + io_time[MAGNETICTAPE]);
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
			printf("\tProcess %d used PRINTER from time %d to time %d\n", front(qprinter_io) -> pid, printer_time, printer_time+io_time[PRINTER]);
			print_process(front(qprinter_io));
			printer_time += io_time[PRINTER];
			push(qhigh_priority, front(qprinter_io) , printer_time);	
			pop(qprinter_io);
				
		} else {
			break;
		}			
	}
}


// add new arrival process
void arriving(){
	while( !empty(qarrival) && front(qarrival)-> push_time  <= clock_time ){
		printf("\tArrived:\n");
		process *arrivalProcess = front(qarrival);
		print_process(arrivalProcess);
		pop(qarrival);
		push(qhigh_priority, arrivalProcess, arrivalProcess -> push_time);
	}
}


// executes the process in front of 
void exec_next_process(int current_queue){
	process *next_process;
	if(current_queue == 0){ // low priority
		next_process = front(qlow_priority);
		pop(qlow_priority);	
		printf("\tExecuting (low priority):\n");
	} else { // high priority
		next_process = front(qhigh_priority);
		pop(qhigh_priority);	
		printf("\tExecuting (high priority):\n");
	}
	int delta_time = min(quantum, next_process -> cpu_time - next_process -> used_cpu_time);
	
	int time_until_next_io = -1;
	if(next_process -> next_io < next_process -> io_qtd) // still have ios)
		time_until_next_io = next_process -> ios[next_process -> next_io] . time - next_process -> used_cpu_time;
	
	if( time_until_next_io != -1 && delta_time >= time_until_next_io ){
		
		delta_time = time_until_next_io;
		next_process -> next_io++;
		next_process -> used_cpu_time += delta_time;
		print_process(next_process);
		//still needs time to finish		
		int next_clock_time = clock_time + delta_time;
		while(clock_time < next_clock_time){				
			clock_time++;			
			if(clock_time < next_clock_time)
				printf("CLOCK TIME: %d\n\n", clock_time);
			arriving(); 
			returning_of_ios();
		}
		if(next_process -> ios[next_process -> next_io - 1] . type == DISC ){
			push(qdisc_io, next_process, clock_time);
		} else if (next_process -> ios[next_process -> next_io - 1] . type == MAGNETICTAPE){
			push(qmagnetic_io, next_process, clock_time);				
		} else { // printer
			push(qprinter_io, next_process, clock_time);			
		}
		
		
					
	} else {
		
		next_process -> used_cpu_time += delta_time;
		
		print_process(next_process);
		//still needs time to finish		
		int next_clock_time = clock_time + delta_time;
		while(clock_time < next_clock_time){				
			clock_time++;
			if(clock_time < next_clock_time)
				printf("CLOCK TIME: %d\n\n", clock_time);
			arriving();
			returning_of_ios();	
		}
		if(next_process -> used_cpu_time < next_process -> cpu_time){
			push(qlow_priority, next_process, clock_time);				
		} else {
			free_process(next_process);
			printf("\tFinishing!\n");
			finished_process++;
		}
	}


}

// Round Robin simulation
void round_robin(){
	while(finished_process < number_of_process){
		printf("CLOCK TIME: %d\n\n", clock_time);
		
		arriving(); 
		returning_of_ios();	
		if( !empty(qhigh_priority) ){ //executing process from high priority queue
			exec_next_process(1);
		} else if( !empty(qlow_priority) ){ //executing process from low priority queue
			exec_next_process(0);
		} else {
			// all process are in IO or didn't arrived
			clock_time ++;		
		}
	}
	
	printf("\n\nFinished simulation on time %d\n", clock_time);
	
	free_queue(qarrival);
	free_queue(qhigh_priority);
	free_queue(qlow_priority);
	free_queue(qdisc_io);
	free_queue(qmagnetic_io);
	free_queue(qprinter_io);	
}



int main(int argc, char *argv[]){	
	init(argv[1]);
	round_robin();
}


