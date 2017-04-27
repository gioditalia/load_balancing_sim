#include<stdio.h>
#include<stdlib.h>
#include <stdarg.h>

#include "logger.h"

void set_Log_File(){
    log_file = fopen("file.txt", "w");
    if (log_file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(log_file, "###################INIT###################\n\n");
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

