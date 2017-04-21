#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<math.h>
#include<time.h>

#define N 3
#define AJL 5

struct Job{
    int nCpu;
    int startTime;
    int jobTime;
    int exeCPU;
    struct Job *p;
};

int TIME=0;
struct Job *Jobs=NULL;
struct Job *CPU[N];
struct Job Current_job[N];
int CPU_load[N];
void Generator();
void Execution();

struct Job *new_Job(int nCpu, int startTime, int jobTime);
struct Job *add_Job(struct Job *list ,struct Job *ele);
struct Job *assign_Job(struct Job *list, int CPU);
void print_Jobs(struct Job *list);
void CPU_status();
int check_list(struct Job *list);
struct Job extract_Job(struct Job **list);
struct Job extract_Head_Job(struct Job **list);

int Random(int min, int max);

int main(){
    srand(time(NULL));

    Generator();
    for(int i=0;i<N;i++){
        CPU[i]=NULL;
        CPU_load[i]=0;
        Current_job[i].exeCPU=0;
    }
    print_Jobs(Jobs);
    Execution(AJL);
    CPU_status();
    
    return 1;
}

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

int check_list(struct Job *list){
    printf("%x è vuoto?\n",&list);        
    if(!(list==NULL)){printf("%x\n",list);return 1;};
    printf("%x\n",list);
    return 0;
}

int Random(int min, int max){
    return rand() % (max+1 - min) + min;
}

void Generator(){
    int AJLcurrent1,AJLcurrent2,AJLtmp;
    TIME++;
    AJLcurrent1 = N*AJL;
    AJLcurrent2 = (N/3)*(AJL/2);
    for(int i = 1; i<N; i++){
        if(AJLcurrent1 > 1){
            AJLtmp = Random(1,(AJLcurrent1/2));
            AJLcurrent1 = AJLcurrent1 - AJLtmp ;
        }
        Jobs = add_Job(Jobs,new_Job(Random(1,N),TIME*AJL,AJLtmp));
        if(!((i+1)%3)){
            if(AJLcurrent2 > 1){
                AJLtmp = Random(1,(AJLcurrent2/2));
                AJLcurrent2 = AJLcurrent2 - AJLtmp ;
                Jobs = add_Job(Jobs,new_Job(Random(1,N),TIME*AJL,AJLtmp));
            }
        }
    }
    Jobs = add_Job(Jobs,new_Job(Random(1,N),TIME*AJL,AJLcurrent1));
}

void Execution(int T){
    struct Job tmp;
    //riempie le liste dei processi delle CPU
    while(check_list(Jobs)){
        tmp = extract_Job(&Jobs);
        CPU[tmp.nCpu-1]=add_Job(CPU[tmp.nCpu-1],new_Job(tmp.nCpu,tmp.startTime,tmp.jobTime));
        CPU_load[tmp.nCpu-1]++;
        }
    //esegue per T secondi
    for(int i = 0;i<T;i++){
        //le cpu acquisiscono il lavoro
        for(int j=0;j<N;j++){
            if(Current_job[j].exeCPU==0 && check_list(CPU[j])){
                tmp = extract_Job(&CPU[j]);
                CPU_load[j]--;
                Current_job[j].nCpu = tmp.nCpu;
                Current_job[j].startTime = tmp.startTime;
                Current_job[j].jobTime = tmp.jobTime;
                Current_job[j].exeCPU = j+1;
            }
        }

        /*
        riduci jobTime del lavoro in corso su ogni CPU occupata
        il master rispondera alle richieste
        le CPU libere utilizzeranno il tempo di esecuzione per inviare la richiesta al master
        */
        for(int j=0;j<N;j++){
            if(Current_job[j].exeCPU!=0){
                Current_job[j].jobTime--;
            }
            //il master manda lavoro agli slave che lo hanno chiesto

            //le cpu chiedono lavoro al master

        }
        //togli dall'esecuzione i processi terminati
        for(int j=0;j<N;j++){
            if(Current_job[j].jobTime==0){
                Current_job[j].nCpu = 0;
                Current_job[j].startTime = 0;
                Current_job[j].jobTime = 0;
                Current_job[j].exeCPU = 0;
            }
        }
    }
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

void CPU_status(){
    for(int i=0;i<N;i++){
        printf("##CPU %d:\n",i+1);
        printf("#IN ESECUZIONE:\n");
        printf(" CPU=%d\n",Current_job[i].nCpu);
        printf(" START=%d\n",Current_job[i].startTime);
        printf(" TIME=%d\n",Current_job[i].jobTime);
        printf(" ASS=%d\n",Current_job[i].exeCPU);
        printf("#IN CODA:\n");
        print_Jobs(CPU[i]);
    }
}
