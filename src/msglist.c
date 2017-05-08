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

#include "msglist.h"
#include "joblist.h"

struct Msg *new_Msg(int from, int to, int code, struct Job *job){
    struct Msg *new;
    new = (struct Msg *)malloc(sizeof(struct Msg));
    new->from = from;
    new->to = to;
    new->code = code;
    new->job = job;
    new->p = NULL;
    return new;
}

struct Msg *add_Msg(struct Msg *list ,struct Msg *ele)
{
    ele->p=list;
    return ele;
}


struct Msg extract_Msg(struct Msg **list){
    struct Msg tmp;
    struct Msg *old;
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

struct Msg extract_Head_Msg(struct Msg **list){
    struct Msg tmp;
    struct Msg *old;
    tmp = **list;
    old = *(list);
    *list=old->p;
    free(old);
    tmp.p=NULL;
    return tmp;
}

int check_Msg_List(struct Msg *list){
    if(!(list==NULL)){return 1;};
    return 0;
}

void print_Msgs(struct Msg *list){
    for(;list != NULL;list = list->p){
        printf(" ADDR=%x\n",list);
        printf(" FROM=%d\n",list->from);
        printf(" TO=%d\n",list->to);
        printf(" CODE=%d\n",list->code);
        printf(" JOB=%x\n",list->job);
        printf(" ADDRPUNT=%x\n",list->p);
        printf("\n");
    }
}