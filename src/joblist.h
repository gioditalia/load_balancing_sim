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
int check_Job_List(struct Job *list);

//print job lists
void print_Jobs(struct Job *list);

#endif /* JOBLIST */
