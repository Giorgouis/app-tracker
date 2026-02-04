#ifndef PROCESSES_H
#define PROCESSES_H

#include <windows.h>
#include <stdbool.h>

#define NAME_LEN 128
#define PIDS_LEN 2048
typedef struct process{
    DWORD pid;
    char name[128];
    bool is_program;
}process;



void get_processes(DWORD *all_processes, DWORD *process_count);

void get_process_info(process *curr_process, DWORD pid);

process *get_all_process_info_malloced(DWORD *processes, DWORD process_count);

bool check_process_in_process(process *ps, DWORD process_count, const char *p_name);

#endif