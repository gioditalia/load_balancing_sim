#include<stdio.h>
#include<stdlib.h>
#include "joblist.h"

#include "utils.h"

void CPU_status(int N,struct Job Current_jobs[],struct Job *CPUlist[]){
    for(int i=0;i<N;i++){
        printf("##CPU %d:\n",i+1);
        printf("#IN ESECUZIONE:\n");
        printf(" CPU=%d\n",Current_jobs[i].nCpu);
        printf(" START=%d\n",Current_jobs[i].startTime);
        printf(" TIME=%d\n",Current_jobs[i].jobTime);
        printf(" ASS=%d\n",Current_jobs[i].exeCPU);
        printf("#IN CODA:\n");
        print_Jobs(CPUlist[i]);
    }
}


int Random(int min, int max){
    return rand() % (max+1 - min) + min;
}