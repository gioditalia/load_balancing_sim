#ifndef LOGGER_H
#define LOGGER_H

FILE *log_file;

void set_Log_File();
void close_Log();
void write_Log(const char *str, ... );


#endif /* UTILS */
