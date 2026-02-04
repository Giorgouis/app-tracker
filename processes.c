#include <windows.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <psapi.h>


typedef struct {
    DWORD pid;
    char name[128];
    bool is_program;
}process;

void get_processes(DWORD all_processes[2048], DWORD *process_count){
    DWORD cb_needed;


    if (!EnumProcesses(all_processes, sizeof(DWORD)*2048, &cb_needed)) all_processes = NULL;

    *process_count = cb_needed/sizeof(DWORD);

}

void get_process_info(process *curr_process, DWORD pid){
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    DWORD sessionID;

    if (hProcess == NULL) return;


    char path[MAX_PATH];
    DWORD size = sizeof(path);
    if (QueryFullProcessImageNameA(hProcess, 0, path, &size)){
        curr_process->pid = pid;
        char *last_slash = strrchr(path, '\\');
        if (last_slash != NULL)
            strncpy(curr_process->name, last_slash + 1, 128);

        else 
            strncpy(curr_process->name, path, 128);
    
    curr_process->name[127] = '\0';
    if (ProcessIdToSessionId(pid, &sessionID)){
        if (sessionID != 0) curr_process->is_program = true;
        else curr_process->is_program = false;
    }
    else curr_process->is_program = false;
    }
    curr_process->name[127] = '\0';

    
}

process *get_all_process_info_malloced(DWORD *processes, DWORD process_count){
    process *res;
    res = calloc(process_count,sizeof(process));
    if (res == NULL) return NULL;
    for (unsigned int i = 0; i<process_count;i++){
        get_process_info(&res[i], processes[i]);
    }

    return res;
}

bool check_process_in_process(process *ps, DWORD process_count, const char *p_name){
    for (DWORD i = 0; i<process_count;i++)
        if (strcmp(ps[i].name, p_name) == 0) return 1;
    
    return 0;
}