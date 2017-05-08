/*
lb_sim - load balancing simulator
Copyright (C) 2017  Giovanni D'Italia

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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