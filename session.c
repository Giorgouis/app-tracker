#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "processes.h"



typedef struct {
    time_t *timestamps;
    process **process_list;
    unsigned int *process_list_length;
    unsigned int timestamp_count;

}session;

void init_session(session *s){
    s->timestamps = NULL;
    s->process_list = NULL;
    s->process_list_length = NULL;
    s->timestamp_count = 0;

}

void update_session(session *s, process *new_proc, DWORD count){
    unsigned int new_timestamps;

    time_t *temp;
    unsigned int *temp_l;
    process **temp_plist;
    if (new_proc == NULL){

        DWORD pids[PIDS_LEN];

        get_processes(pids, &count);
        if (count == 0) return;
        new_proc = get_all_process_info_malloced(pids, count);
        if (new_proc == NULL) return;
    }
    new_timestamps = s->timestamp_count+1;
    temp = realloc(s->timestamps, new_timestamps*sizeof(size_t));
    temp_l = realloc(s->process_list_length, sizeof(unsigned int)*new_timestamps);
    temp_plist = realloc(s->process_list, sizeof(process*)*new_timestamps);
    if (temp == NULL || temp_plist == NULL || temp_l == NULL) return;

    
    s->process_list = temp_plist;
    s->timestamp_count = new_timestamps;
    s->timestamps = temp;
    s->process_list_length = temp_l;

    s->timestamps[new_timestamps-1] = time(NULL);
    s->process_list[new_timestamps-1] = new_proc;
    s->process_list_length[new_timestamps-1]= count;
    
}

