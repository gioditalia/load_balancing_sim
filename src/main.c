#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include "joblist.h"
#include "utils.h"
#include "logger.h"

#define N 100
#define AJL 5

int TIME=0;

struct Job *Jobs=NULL;
struct Job *CPU[N];
struct Job Current_jobs[N];

int CPU_load[N];

void Generator();
void Execution();


int main(){
    srand(time(NULL));
    set_Log_File("ciao!");
    Generator();
    write_Log("Processi generati:%d",(N+N/3));
    for(int i=0;i<N;i++){
        CPU[i]=NULL;
        CPU_load[i]=0;
        Current_jobs[i].exeCPU=0;
    }
    print_Jobs(Jobs);
    write_Log("Avvio di %d processi su %d CPU:",(N+N/3),N);
    Execution(AJL);
    CPU_status(N,Current_jobs,CPU);

    close_Log();
    return 1;
}

void Generator(){
    int AJLcurrent1,AJLcurrent2,AJLtmp;
    TIME++;
    AJLcurrent1 = N*AJL;
    AJLcurrent2 = (N/3)*(AJL/2);
    for(int i = 1; i<N; i++){
        if(AJLcurrent1 > 1){
            AJLtmp = Random(1,((AJLcurrent1)/2));
            AJLcurrent1 = AJLcurrent1 - AJLtmp ;
        }
        Jobs = add_Job(Jobs,new_Job(Random(1,N),TIME*AJL,AJLtmp));
        if(!((i+1)%3)){
            if(AJLcurrent2 > 1){
                AJLtmp = Random(1,((AJLcurrent2)/2));
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
    while(check_List(Jobs)){
        tmp = extract_Job(&Jobs);
        CPU[tmp.nCpu-1]=add_Job(CPU[tmp.nCpu-1],new_Job(tmp.nCpu,tmp.startTime,tmp.jobTime));
        CPU_load[tmp.nCpu-1]++;
        }
    //esegue per T secondi
    for(int i = 0;i<T;i++){
        //le cpu acquisiscono il lavoro
        for(int j=0;j<N;j++){
            if(Current_jobs[j].exeCPU==0 && check_List(CPU[j])){
                tmp = extract_Job(&CPU[j]);
                CPU_load[j]--;
                Current_jobs[j].nCpu = tmp.nCpu;
                Current_jobs[j].startTime = tmp.startTime;
                Current_jobs[j].jobTime = tmp.jobTime;
                Current_jobs[j].exeCPU = j+1;
            }
        }

        /*
        riduci jobTime del lavoro in corso su ogni CPU occupata
        il master rispondera alle richieste
        le CPU libere utilizzeranno il tempo di esecuzione per inviare la richiesta al master
        */
        for(int j=0;j<N;j++){
            if(Current_jobs[j].exeCPU!=0){
                Current_jobs[j].jobTime--;
            }
            //il master manda lavoro agli slave che lo hanno chiesto

            //le cpu chiedono lavoro al master

        }
        //togli dall'esecuzione i processi terminati
        for(int j=0;j<N;j++){
            if(Current_jobs[j].jobTime==0){
                Current_jobs[j].nCpu = 0;
                Current_jobs[j].startTime = 0;
                Current_jobs[j].jobTime = 0;
                Current_jobs[j].exeCPU = 0;
            }
        }
    }
}