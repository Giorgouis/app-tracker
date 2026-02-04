#ifndef SESSION_H
#define SESSION_H

#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include "processes.h" // Required for the 'process' struct definition

/* 
 * The Session struct tracks a history of snapshots.
 * process_list: An array of pointers, where each pointer points to an array of processes.
 * process_list_length: Parallel array storing the number of processes in each snapshot.
 * timestamps: Parallel array storing when each snapshot was taken.
 */
typedef struct {
    time_t *timestamps;
    struct process **process_list;
    unsigned int *process_list_length;
    unsigned int timestamp_count;
} session;


void init_session(session *s);


void update_session(session *s, process *new_proc, unsigned int count);

#endif // SESSION_H