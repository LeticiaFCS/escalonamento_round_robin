# Escalonamento Round Robin (circular) com Feedback
Simulador em C que implementa o algoritmo de escalonamento de processos Round Robin (circular) com Feedback. Trabalho apresentado na disciplina Sistemas Operacionais da UFRJ em 2022.1 pelos alunos **Ana Claudia Ribeiro dos Santos**, **Letícia Freire Carvalho de Sousa** e **Augusto Guimarães Rodrigues de Lima**.

# Relatório

https://docs.google.com/document/d/1rACQlqDR-uVVbuxi1s4W4dPCWXMmj0nUTMk_Vvxd2-Q/edit?usp=sharing

# Formato dos arquivos de teste
Os processos devem ser representados como um processo por linha em ordem **crescente** de tempo de chegada.

Formato da linha que representa um processo:

pid;arrival_time;cpu_time;number_of_io;type_of_io_1;time_of_io1; ... ;type_of_io_{number_of_io};time_of_io_{number_of_io}


# Para rodar

make main - compilar
./main teste.txt - rodar arquivo de teste


