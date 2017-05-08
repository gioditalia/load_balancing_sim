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
#include <stdarg.h>
#include<time.h>
#include "logger.h"

void set_Log_File(){
    log_file = fopen("log.txt", "w");
    if (log_file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    struct tm* newtime;
    time_t t;

    time(&t);
    newtime=localtime(&t);
    fprintf(log_file, "#############################################\n");
    fprintf(log_file, "############ %d/%d/%d - %d:%d:%d ############\n",
    newtime->tm_mday,newtime->tm_mon+1,newtime->tm_year+1900,newtime->tm_hour,newtime->tm_min,newtime->tm_sec);+
    fprintf(log_file, "#############################################\n\n");

}

void write_Log(const char *str, ...){
    va_list args;
    va_start(args, str);
    vfprintf(log_file, str, args);
    fprintf(log_file,"\n");
    va_end(args);
}

void close_Log(){
    fclose(log_file);
}

