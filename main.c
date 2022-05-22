#include<stdio.h>
#include"filas.h"



void testQueues(){
	queue *q = initQueue();
	process *frente;
	
	process *p1 = newProcess(1, 0);
	process *p2 = newProcess(2, 0);
	process *p3 = newProcess(3, 0);
	
	enqueue(q, p1);
	enqueue(q, p2);
	
	frente = front(q);
	printProcess(frente); // p1
	
	pop(q);
	frente = front(q);
	printProcess(frente); // p2
	
	enqueue(q, p3);
	frente = front(q);
	printProcess(frente); // p2
	
	pop(q);
	frente = front(q);
	printProcess(frente); // p3
	
	pop(q);
	frente = front(q);
	printProcess(frente); // empty
}

int main(){	
	testQueues();
	
}

