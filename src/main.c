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
#include<math.h>
#include<time.h>

#include "joblist.h"
#include "utils.h"
#include "logger.h"
#include "msglist.h"

int TIME=0;
int AJL=5;
int THRESHOLD=5;
int GEN=5;
int AL_TYPE=0;
int N=10;
int MAXTIME=10000;

FILE *fconf;

#define X 10

//LAVORI
struct Job *Jobs=NULL;
struct Job **CPU; // array di liste dei lavori sui NODI
struct Job *Master_cpu; //lista dei lavori sul MASTER
struct Job *Current_jobs; //array del lavoro in esecuzione sul NODI

//MESSAGGI
struct Msg **Msgs; //array di liste dei messaggi sul NODO
struct Msg *Master_msgs; //lista dei messaggi sul MASTER

//CONTATORI
int *Msgs_counter; //array contatore dei messaggi inviati dai NODI
int Msgs_master_counter=0; //contatore dei messaggi inviati dal MASTER
int Master_job_counter=0; //contatore dei lavori che il MASTER ha da offrire(algoritmo iniziato dal mittente)
int *CPU_load; //array del carico di lavoro sul NODO

int Gen_job_tot=0;
int Waiting_job=0;
//FLAG
int FreeCPU=0; //flag NODI liberi(algoritmo iniziato dal ricevente)

void Generator();
void Execution(int T,int type);

void Alg_ricevente();
void Alg_mittente();


int main(){
    //inizializzo il generatore di numeri casuali per il random
    srand(time(NULL));
    //inizializzo il file di log
    set_Log_File();
    //leggo file di configurazione
    fconf=fopen("conf","r");
    if(fconf){
        fscanf(fconf,"%d\n%d\n%d\n%d\n%d\n%d",&AJL,&THRESHOLD,&GEN,&N,&AL_TYPE,&MAXTIME);
    }
    //scrivo configurazione sul file di log e sulla console
    write_Log("!!! FILE DI CONFIGURAZIONE !!!");
    write_Log("- AJL = %d",AJL);
    write_Log("- THRESHOLD = %d",THRESHOLD);
    write_Log("- GENERAZIONI = %d",GEN);
    write_Log("- NODI = %d",N);
    write_Log("- TYPE = %d",AL_TYPE);
    printf("\n!!! FILE DI CONFIGURAZIONE !!!\n");
    printf("- AJL = %d\n",AJL);
    printf("- THRESHOLD = %d\n",THRESHOLD);
    printf("- GENERAZIONI = %d\n",GEN);
    printf("- NODI = %d\n",N);
    printf("- TYPE = %d\n",AL_TYPE);
    if(AL_TYPE==0){
        write_Log("|- ALGORITMO = iniziato dal mittente");
        printf("|- ALGORITMO = iniziato dal mittente\n");

    }
    else{
        write_Log("|- ALGORITMO = iniziato dal ricevente");
        printf("|- ALGORITMO = iniziato dal mittente\n");

    }
    write_Log("- MAXTIME = %d\n",MAXTIME);
    printf("- MAXTIME = %d\n\n",MAXTIME);
    //alloco la memoria per gli array
    CPU=malloc(N*sizeof(struct Job));
    CPU_load=malloc(N*sizeof(int));
    Current_jobs=malloc(N*sizeof(struct Job));
    Msgs=malloc(N*sizeof(struct Msg));
    Msgs_counter=malloc(N*sizeof(int));
    //inizializzo le variabili globali
    for(int i=0;i<N;i++){
        CPU[i]=NULL;
        CPU_load[i]=0;
        Current_jobs[i].exeCPU=0;

        Msgs[i]=NULL;
        Msgs_counter[i]=0;
    }
    //inizio simulazione
    for(int i=0;i<GEN;i++){
        Generator();
        Execution(AJL,AL_TYPE);
    }
    write_Log("\n\n!!! COMPLETAMENTO DEI LAVORI RESTANTI !!!\n\n");
    Execution(0,AL_TYPE);

    write_Log("!!! RESOCONTO FINALE !!!");
    write_Log("!!! MESSAGGI INVIATI !!!");
    write_Log("- MASTER = %d",Msgs_master_counter);
    printf("\n!!! MESSAGGI INVIATI !!!\n");
    printf("- MASTER = %d\n",Msgs_master_counter);
    for(int j=0;j<N;j++){
        write_Log("- CPU %d = %d",j+1,Msgs_counter[j]);
        printf("- CPU %d = %d\n",j+1,Msgs_counter[j]);
    }
    write_Log("\n- LAVORO/(UNITA DI TEMPO) = %d/%d = %lf",Gen_job_tot,TIME,(float)Gen_job_tot/(float)TIME);
    printf("\n- LAVORO/(UNITA DI TEMPO) = %d/%d = %lf\n\n",Gen_job_tot,TIME,(float)Gen_job_tot/(float)TIME);
    close_Log();
    return 1;
}

void Generator(){
    int AJLcurrent1,AJLcurrent2,tmpAJL;
    int tmpCPU;
    int Gen_job=0;
    AJLcurrent1 = N*AJL;
    AJLcurrent2 = (N/3)*(AJL/2);
    write_Log("##################################");
    write_Log("#        GENERATORE T=%d         #",(TIME));
    write_Log("##################################\n");
    write_Log("@@@ INIZIO GENERAZIONE @@@");
    //Per ogni nodo creo un processo, per mantenere AJL costante la somma dei jobTime di ogni lavoro diviso il numero di lavori è AJL
    for(int i = 1; i<N; i++){
        if(AJLcurrent1 > 1){
            tmpAJL = Random(1,((AJLcurrent1)/2));
            if(tmpAJL>2*AJL)tmpAJL= (2*AJL)+Random(-5,5);
            AJLcurrent1 = AJLcurrent1 - tmpAJL ;
        
            tmpCPU = Random(1,N);
            Jobs = add_Job(Jobs,new_Job(tmpCPU,TIME,tmpAJL));
            Gen_job++;
            Gen_job_tot++;
            Waiting_job++;
            write_Log("Il NODO %d ha generato un processo[jobTime = %d]",tmpCPU,tmpAJL);
        }
        //ogni 3 lavori generati ne genero uno in più
        if(!((i+1)%3)){
            if(AJLcurrent2 > 1){
                tmpAJL = Random(1,((AJLcurrent2)/2));
                if(tmpAJL>2*AJL)tmpAJL=2*AJL+Random(5,5);
                AJLcurrent2 = AJLcurrent2 - tmpAJL ;
                tmpCPU = Random(1,N);
                Jobs = add_Job(Jobs,new_Job(tmpCPU,TIME,tmpAJL));
                Gen_job++;
                Gen_job_tot++;
                Waiting_job++;
                write_Log("Il NODO %d ha generato un processo[jobTime = %d]",tmpCPU,tmpAJL);
            }
        }
    }
    if(AJLcurrent1>1){
        tmpCPU = Random(1,N);
        Jobs = add_Job(Jobs,new_Job(tmpCPU,TIME,AJLcurrent1));
        Gen_job++;
        Gen_job_tot++;
        Waiting_job++;
        write_Log("Il NODO %d ha generato un processo[jobTime = %d]",tmpCPU,AJLcurrent1);
    }
    write_Log("Generati %d processi [TOTALE: %d]",Gen_job,Gen_job_tot);
    write_Log("@@@ FINE GENERAZIONE @@@\n");
}

void Execution(int T,int type){
    struct Job tmpJob;
    int i=0, no_more_Job;
    //riempie le liste dei processi dei NODI
    while(check_Job_List(Jobs)){
        tmpJob = extract_Job(&Jobs);
        CPU[tmpJob.nCpu-1]=add_Job(CPU[tmpJob.nCpu-1],new_Job(tmpJob.nCpu,tmpJob.startTime,tmpJob.jobTime));
        CPU_load[tmpJob.nCpu-1]+=tmpJob.jobTime;
        }
    //se T!= 0 esegue per T secondi altrimenti esegue fino a quando non ci sono più i lavori
    // in lista per ogni nodo sono terminati e nessun NODO ha in esecuzione un lavoro
    if(T>0){
        for(i= 0;i<T;i++){
            if(type==1){
                Alg_ricevente();
            }
            else{
                Alg_mittente();
            }
        }
    }
    else{
        while(1){
            no_more_Job=1;
            for(int j=0;j<N;j++){
                if(Current_jobs[j].exeCPU!=0 || check_Job_List(CPU[j])){
                    no_more_Job=0;
                }
            }
            if(no_more_Job==1 || TIME>=MAXTIME){
                if(TIME>=MAXTIME){
                    write_Log("+++ TEMPO MASSIMO DI ESECUZIONE RAGGIUNTO+++");
                    printf("\n+++ TEMPO MASSIMO DI ESECUZIONE RAGGIUNTO +++\n");
                }
                else{
                write_Log("+++ NON CI SONO PIU LAVORI +++");
                }
                write_Log("+++  TEMPO IMPIEGATO T=%d  +++\n",TIME);
                break;
            }
            else if(type==1){
                Alg_ricevente();
            }
            else{
                Alg_mittente();
            }
        }
    }
}

void Alg_ricevente(){
        struct Job tmpJob;
        struct Msg tmpMsg;
        write_Log("##################################");
        write_Log("#       ESECUZIONE T=%d          #",TIME);
        write_Log("##################################\n");

        FreeCPU=0; //il master considera tutte le CPU a lavoro
        //il MASTER legge i messaggi
        write_Log("@@@ INIZIO OPERAZIONI MASTER @@@");
        while(check_Msg_List(Master_msgs)){
            tmpMsg = extract_Msg(&Master_msgs);
            //se code=1 aggiunge il lavoro ricevuto alla propria lista
            if(tmpMsg.code==1){
                Master_cpu=add_Job(Master_cpu,new_Job(tmpMsg.job->nCpu,tmpMsg.job->startTime,tmpMsg.job->jobTime));
                write_Log("Il MASTER riceve un lavoro dal NODO %d",tmpMsg.from);
            }
            //se code=2 invia il lavoro al NODO(se disponibile nella lista dei lavori del MASTER) oppure segnala NODI liberi(FreeCPU=1)
            else if(tmpMsg.code==2){
                if(check_Job_List(Master_cpu)){
                    tmpJob = extract_Job(&Master_cpu);
                    Msgs[tmpMsg.from-1]=add_Msg(Msgs[tmpMsg.from-1],new_Msg(0,tmpMsg.from,1,new_Job(tmpJob.nCpu,tmpJob.startTime,tmpJob.jobTime)));
                    Msgs_master_counter++;
                    write_Log("Il MASTER invia un lavoro al NODO %d",tmpMsg.from);
                }
                else{
                    FreeCPU=1;
                }
            }
        }
        //se NON ci sono CPU libere invia un messaggio con code=0 ad ognuna
        //se ci sono CPU libere invia una richiesta(code=2) ad 1/4 delle CPU in modo casuale 
        for(int j=0;j<N;j++){
            if(FreeCPU==0){
                Msgs[j]=add_Msg(Msgs[j],new_Msg(0,j+1,0,NULL));
                Msgs_master_counter++;
            }
            else if(!(Random(0,3))){
                Msgs_master_counter++;
                write_Log("Il MASTER chiede al NODO %d",j+1);
                Msgs[j]=add_Msg(Msgs[j],new_Msg(0,j+1,2,NULL));
            }else{
                Msgs_master_counter++;
                Msgs[j]=add_Msg(Msgs[j],new_Msg(0,j+1,0,NULL));
            }
        }
        write_Log("Il MASTER ha inviato %d messaggi",Msgs_master_counter);
        write_Log("@@@ FINE OPERAZIONI MASTER @@@\n");

        write_Log("@@@ INIZIO OPERAZIONI CPU(SLAVE) @@@");
        for(int j=0;j<N;j++){
            //estraggo il primo messaggio ricevuto ed elimino il resto
            tmpMsg = extract_Msg(&Msgs[j]);
            while(check_Msg_List(Msgs[j]))extract_Msg(&Msgs[j]);
            //se code!=2 non ci sono richieste di lavoro(esecuzione normale)
            if(tmpMsg.code!=2){
                //se code==1 ha ricevuto un lavoro, lo preleva e inserisce in coda
                if(tmpMsg.code==1){
                    CPU[j]=add_Job(CPU[j],new_Job(tmpMsg.job->nCpu,tmpMsg.job->startTime,tmpMsg.job->jobTime));
                    CPU_load[j]+=tmpMsg.job->jobTime;
                }
                //se non sta gia eseguendo un lavoro lo preleva dalla coda e lo mette in esecuzione
                if(Current_jobs[j].exeCPU==0 && check_Job_List(CPU[j])){
                    tmpJob = extract_Job(&CPU[j]);
                    Current_jobs[j].nCpu = tmpJob.nCpu;
                    Current_jobs[j].startTime = tmpJob.startTime;
                    Current_jobs[j].jobTime = tmpJob.jobTime;
                    Current_jobs[j].exeCPU = j+1;
                    write_Log("Il NODO %d acquisisce un lavoro",j+1,Current_jobs[j].jobTime);
                }
                //riduce jobTime del lavoro in corso
                if(Current_jobs[j].exeCPU!=0 && Current_jobs[j].jobTime>0){
                    write_Log("Il NODO %d esegue il lavoro per un unita di tempo[ %d -> %d ]",j+1,Current_jobs[j].jobTime,Current_jobs[j].jobTime-1);
                    Current_jobs[j].jobTime--;
                    CPU_load[j]--;
                }
            }
            //altrimenti(code==2) elabora la richiesta
            else{
                write_Log("Il NODO %d elabora la richiesta del MASTER",j+1);
                //invia un lavoro se il carico sul NODO corrente è maggiore di THRESHOLD
                if(check_Job_List(CPU[j]) && CPU_load[j]>THRESHOLD){
                    tmpJob = extract_Job(&CPU[j]);
                    CPU_load[j]-=tmpJob.jobTime;
                    Master_msgs=add_Msg(Master_msgs,new_Msg(j+1,0,1,new_Job(tmpJob.nCpu,tmpJob.startTime,tmpJob.jobTime)));
                    Msgs_counter[j]++;
                    write_Log("Il NODO %d invia un lavoro al MASTER",j+1);
                }
                //altrimenti non risponde
                else{
                    write_Log("Il NODO %d non ha abbastanza lavori per inviarne uno al MASTER",j+1);
                }
            }
        }
        write_Log("@@@ FINE OPERAZIONI CPU(SLAVE) @@@\n");

        write_Log("@@@ INIZIO CONTROLLO DEI LAVORI @@@");
        //toglie dall'esecuzione i processi terminati
        for(int j=0;j<N;j++){
            if(Current_jobs[j].jobTime<1 && Current_jobs[j].exeCPU != 0){
                write_Log("Il NODO %d termina un lavoro generato dal NODO %d",j+1,Current_jobs[j].nCpu);
                Current_jobs[j].nCpu = 0;
                Current_jobs[j].startTime = 0;
                Current_jobs[j].jobTime = 0;
                Current_jobs[j].exeCPU = 0;
                Waiting_job--;
            }
            //se Il NODO NON ha almeno un processo in coda chiede al MASTER
            if(Current_jobs[j].exeCPU == 0 && !(check_Job_List(CPU[j]))){
                Master_msgs=add_Msg(Master_msgs,new_Msg(j+1,0,2,NULL));
                Msgs_counter[j]++;
                write_Log("Il NODO %d non ha lavori in coda, chiede un lavoro al MASTER",j+1);
            }
        }
        write_Log("@@@ FINE CONTROLLO DEI LAVORI @@@\n");
        TIME++;
}

void Alg_mittente(){
        struct Job tmpJob;
        struct Msg tmpMsg;
        int job_req=0;
        write_Log("##################################");
        write_Log("#       ESECUZIONE T=%d          #",TIME);
        write_Log("##################################\n");

        //il MASTER legge i messaggi
        write_Log("@@@ INIZIO OPERAZIONI MASTER @@@");
        while(check_Msg_List(Master_msgs)){
            tmpMsg = extract_Msg(&Master_msgs);
            //se code=1 aggiunge il lavoro ricevuto alla propria lista
            if(tmpMsg.code==1){
                Master_cpu=add_Job(Master_cpu,new_Job(tmpMsg.job->nCpu,tmpMsg.job->startTime,tmpMsg.job->jobTime));
                Master_job_counter++;
                write_Log("Il MASTER riceve un lavoro dal NODO %d",tmpMsg.from,Master_job_counter);
            }
        }
        //se il master ha almeno un lavoro disponibile manda una richiesta(code=2) ad 1/4 dei nodi
        for(int j=0;j<N;j++){
            if(check_Job_List(Master_cpu) && !(Random(0,3)) && job_req<Master_job_counter){
                Msgs_master_counter++;
                write_Log("Il MASTER chiede al NODO %d di accettare un lavoro",j+1, job_req,Master_job_counter);
                Msgs[j]=add_Msg(Msgs[j],new_Msg(0,j+1,2,NULL));
                job_req++;
            }else{
                Msgs_master_counter++;
                Msgs[j]=add_Msg(Msgs[j],new_Msg(0,j+1,0,NULL));
            }
        }
        write_Log("Il MASTER ha inviato %d messaggi",Msgs_master_counter);
        write_Log("@@@ FINE OPERAZIONI MASTER @@@\n");

        write_Log("@@@ INIZIO OPERAZIONI NODI SLAVE @@@");
        for(int j=0;j<N;j++){
            //estraggo il primo messaggio ricevuto
            tmpMsg = extract_Msg(&Msgs[j]);
            while(check_Msg_List(Msgs[j]))extract_Msg(&Msgs[j]); //CHECK
            //se code==0 non ci sono richieste di lavoro(esecuzione normale)
            if(tmpMsg.code==0){
                //se il carico di lavoro corrente supera THRESHOLD*(3+(num_lavori_in_attesa/numero_nodi)) ed un lavoro è nella coda dei lavori invia un lavoro al MASTER
                if(CPU_load[j]>(THRESHOLD*(3+(Waiting_job/N))) && check_Job_List(CPU[j])){
                    tmpJob = extract_Job(&CPU[j]);
                    CPU_load[j]-=tmpJob.jobTime;
                    Master_msgs=add_Msg(Master_msgs,new_Msg(j+1,0,1,new_Job(tmpJob.nCpu,tmpJob.startTime,tmpJob.jobTime)));
                    Msgs_counter[j]++;
                    write_Log("Il NODO %d invia un lavoro al MASTER",j+1);
                }
                else{
                //se non sta gia eseguendo un lavoro lo preleva dalla coda e lo mette in esecuzione
                if(Current_jobs[j].exeCPU==0 && check_Job_List(CPU[j])){
                    tmpJob = extract_Job(&CPU[j]);
                    Current_jobs[j].nCpu = tmpJob.nCpu;
                    Current_jobs[j].startTime = tmpJob.startTime;
                    Current_jobs[j].jobTime = tmpJob.jobTime;
                    Current_jobs[j].exeCPU = j+1;
                    write_Log("Il NODO %d acquisisce un lavoro",j+1,Current_jobs[j].jobTime);
                }
                //riduce jobTime del lavoro in corso
                if(Current_jobs[j].exeCPU!=0 && Current_jobs[j].jobTime>0){
                    write_Log("Il NODO %d esegue il lavoro per un unita di tempo[ %d -> %d ]",j+1,Current_jobs[j].jobTime,Current_jobs[j].jobTime-1);
                    Current_jobs[j].jobTime--;
                    CPU_load[j]--;
                }
                }
                
            }
            //altrimenti(code==2) elabora la richiesta
            else{
                write_Log("Il NODO %d elabora la richiesta del MASTER",j+1);
                //Il NODO corrente accetta il lavoro se il carico corrente è minore di THRESHOLD*1.5
                if(CPU_load[j]<(THRESHOLD*(2.5+(Waiting_job/N)))){
                    Master_msgs=add_Msg(Master_msgs,new_Msg(j+1,0,0,NULL));
                    Msgs_counter[j]++;
                    //il master riceve il messaggio, preleva il lavoro e lo invia
                    tmpMsg = extract_Head_Msg(&Master_msgs);
                    if(check_Job_List(Master_cpu)){
                        tmpJob = extract_Job(&Master_cpu);
                        Msgs[tmpMsg.from-1]=add_Msg(Msgs[tmpMsg.from-1],new_Msg(0,tmpMsg.from,1,new_Job(tmpJob.nCpu,tmpJob.startTime,tmpJob.jobTime)));
                        Msgs_master_counter++;
                        Master_job_counter--;
                        write_Log("Il MASTER invia un lavoro al NODO %d",tmpMsg.from);
                        //Il NODO preleva il messaggio e aggiunge il lavoro alla lista dei lavori
                        tmpMsg = extract_Msg(&Msgs[j]);
                        CPU[j]=add_Job(CPU[j],new_Job(tmpMsg.job->nCpu,tmpMsg.job->startTime,tmpMsg.job->jobTime));
                        CPU_load[j]+=tmpMsg.job->jobTime;
                        
                    }
                }                
                //altrimenti non risponde
                else{
                    write_Log("Il NODO %d non puo' accettare altri lavori",j+1);
                }
            }
        }
        write_Log("@@@ FINE OPERAZIONI CPU(SLAVE) @@@\n");

        write_Log("@@@ INIZIO CONTROLLO DEI LAVORI @@@");
        //toglie dall'esecuzione i processi terminati
        for(int j=0;j<N;j++){
            if(Current_jobs[j].jobTime<1 && Current_jobs[j].exeCPU != 0){
                write_Log("Il NODO %d termina un lavoro generato dalNODO %d",j+1,Current_jobs[j].nCpu);
                Current_jobs[j].nCpu = 0;
                Current_jobs[j].startTime = 0;
                Current_jobs[j].jobTime = 0;
                Current_jobs[j].exeCPU = 0;
                Waiting_job--;
            }
        }
        write_Log("@@@ FINE CONTROLLO DEI LAVORI @@@\n");
        TIME++;
}