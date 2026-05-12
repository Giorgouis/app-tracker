#include"session.h"
#include<stdlib.h>
#include<string.h>

#define MAX_TRACKED_PROGRAMS 256
typedef struct Program{
    char name[NAME_LEN];
    time_t time_run;
    time_t current_session_run;
}Program;

process *tracked_programs[MAX_TRACKED_PROGRAMS] = {0};
Program *tracked[MAX_TRACKED_PROGRAMS] = {0};
DWORD len = 0;



inline bool match_process(process *p1, process *p2){
    return (strcmp(p1->name, p2->name) == 0);
}

void start_tracking(process *p){

    for (DWORD i =0; i<len; i++){
        if (match_process(p, tracked_programs[i])) return;
    }

    if (len>=MAX_TRACKED_PROGRAMS) return;
    tracked_programs[len++] = p;
}

void stop_tracking(process *p){
    if (!tracked_programs) return;

    for (DWORD i = 0; i<len;i++){
        if (match_process(tracked_programs[i], p)){

            for (int j = i+1; j<len;j++){
                tracked_programs[j-1] = tracked_programs[j];
            }
            tracked_programs[len-1] = NULL;
            len--;
            return;
        }
    }
}

void init_counter(){}