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
#ifndef MSGLIST_H
#define MSGLIST_H

// struct
struct Msg{
    int from;
    int to;
    int code;   // 0->ok    1->invia job    2->chiedi job
    struct Job *job;
    struct Msg *p;
};

// prototypes
//manage msg lists
struct Msg *new_Msg(int from, int to, int code, struct Job *job);
struct Msg *add_Msg(struct Msg *list ,struct Msg *ele);
struct Msg extract_Msg(struct Msg **list);
struct Msg extract_Head_Msg(struct Msg **list);
int check_Msg_List(struct Msg *list);

//print msg lists
void print_Msgs(struct Msg *list);

#endif /* MSGLIST */
