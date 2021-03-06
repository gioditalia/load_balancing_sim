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
#include<malloc.h>

#include "joblist.h"

struct Job *new_Job(int nCpu, int startTime, int jobTime){
    struct Job *new;
    new = (struct Job *)malloc(sizeof(struct Job));
    new->nCpu = nCpu;
    new->startTime = startTime;
    new->jobTime = jobTime;
    new->exeCPU = 0;
    new->p = NULL;
    return new;
}

struct Job *add_Job(struct Job *list ,struct Job *ele)
{
    ele->p=list;
    return ele;
}

struct Job *assign_Job(struct Job *list, int CPU){
    list->exeCPU = CPU;
    return list;
}

struct Job extract_Job(struct Job **list){
    struct Job tmp;
    struct Job *old;
    old = *(list);
    if((*list)->p!=NULL){
    for(;(*list)->p->p != NULL;*list = (*list)->p);
    tmp = *((*list)->p);
    free((*list)->p);
    (*list)->p=NULL;
    *list = old;
    return tmp;
    }
    else{
        tmp=**list;
        tmp.p=NULL;
        (*list)->p=NULL;
        free(*list);
        (*list)=NULL;
        return tmp;
    }
}

struct Job extract_Head_Job(struct Job **list){
    struct Job tmp;
    struct Job *old;
    tmp = **list;
    old = *(list);
    *list=old->p;
    free(old);
    tmp.p=NULL;
    return tmp;
}

int check_Job_List(struct Job *list){
    if(!(list==NULL)){return 1;};
    return 0;
}

void print_Jobs(struct Job *list){
    for(;list != NULL;list = list->p){
        printf(" ADDR=%x\n",list);
        printf(" CPU=%d\n",list->nCpu);
        printf(" START=%d\n",list->startTime);
        printf(" TIME=%d\n",list->jobTime);
        printf(" ASS=%d\n",list->exeCPU);
        printf(" ADDRPUNT=%x\n",list->p);
        printf("\n");
    }
}