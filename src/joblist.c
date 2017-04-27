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
    printf("scorro ed estraggo:%x",(*list)->p);
    tmp = *((*list)->p);
    free((*list)->p);
    (*list)->p=NULL;
    *list = old;
    return tmp;
    }
    else{
        printf("estraggo:%x",(*list)->p);
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

int check_List(struct Job *list){
    printf("%x è vuoto?\n",&list);
    if(!(list==NULL)){printf("%x\n",list);return 1;};
    printf("%x\n",list);
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