#ifndef JOBLIST_H
#define JOBLIST_H

// struct
struct Job{
    int nCpu;
    int startTime;
    int jobTime;
    int exeCPU;
    struct Job *p;
};

// prototypes
//manage job lists
struct Job *new_Job(int nCpu, int startTime, int jobTime);
struct Job *add_Job(struct Job *list ,struct Job *ele);
struct Job *assign_Job(struct Job *list, int CPU);
struct Job extract_Job(struct Job **list);
struct Job extract_Head_Job(struct Job **list);
int check_List(struct Job *list);

//print job lists
void print_Jobs(struct Job *list);

#endif /* JOBLIST */
