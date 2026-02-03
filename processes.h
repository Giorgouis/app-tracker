#ifndef PROCESSES_H
#define PROCESSES_H

#include <windows.h>
#include <stdbool.h>


struct process {
    DWORD pid;
    char name[128];
    bool is_program;
};

struct process_list {
    struct process *processes;
    DWORD count;
};

void get_processes(DWORD *all_processes, DWORD *process_count);

void get_process_info(struct process *curr_process, DWORD pid);

struct process *get_all_process_info_malloced(DWORD *processes, DWORD process_count);

#endif