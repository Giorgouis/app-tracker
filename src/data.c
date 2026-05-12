#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include<string.h>
#include "process.h"
#include "session.h"

typedef struct {
    char **process_names;
    unsigned int *timers;
    unsigned int length;
} process_timer;

bool construct_next_session(FILE *f, session *s){
    if (!f) return 0;
    if (fread(&s->timestamp_count, sizeof(unsigned int), 1, f) != 1) return false;

    s->timestamps = malloc(sizeof(time_t) * s->timestamp_count);
    s->process_list_length = malloc(sizeof(unsigned int) * s->timestamp_count);
    s->process_list = malloc(sizeof(process*) * s->timestamp_count);

    fread(s->timestamps, sizeof(time_t), s->timestamp_count, f);
    fread(s->process_list_length, sizeof(unsigned int), s->timestamp_count, f);
    for (unsigned int i = 0; i < s->timestamp_count; i++) {
        s->process_list[i] = malloc(sizeof(process) * s->process_list_length[i]);
        fread(s->process_list[i], sizeof(process), s->process_list_length[i], f);
    }
    return 1;
}

bool write_session(const char *filename, session *s){
    FILE *f = fopen(filename, "ab");
    if (!f) return 0;

    fwrite(&s->timestamp_count, sizeof(unsigned int), 1, f);
    fwrite(s->timestamps, sizeof(time_t), s->timestamp_count, f);
    fwrite(s->process_list_length, sizeof(unsigned int), s->timestamp_count, f);

    for (unsigned int i = 0; i < s->timestamp_count; i++) {
        fwrite(s->process_list[i], sizeof(process), s->process_list_length[i], f);
    }

    fclose(f);
    return 1;

}

bool write_session_const_bytelen(const char *filename, session *s){

    FILE *f = fopen(filename,"wb");
    if (!f) return 0;
    uint32_t timestamp_count = s->timestamp_count;
    fwrite(&timestamp_count, sizeof(uint32_t), 1, f);

    for (uint32_t i = 0; i < s->timestamp_count; i++) {
        uint64_t ts = (uint64_t)s->timestamps[i];  // always 8 bytes
        fwrite(&ts, sizeof(uint64_t), 1, f);
    }

    for (uint32_t i = 0; i < s->timestamp_count; i++) {
        uint32_t length = s->process_list_length[i];
        fwrite(&length, sizeof(uint32_t), 1, f);
    }

    for (uint32_t i = 0; i < s->timestamp_count; i++) {
        for (uint32_t j = 0; j < s->process_list_length[i]; j++) {
            process *p = &s->process_list[i][j];
            uint32_t pid = p->pid;
            uint8_t is_program = p->is_program ? 1 : 0;
            uint16_t name_len = (uint16_t)NAME_LEN;

            fwrite(&pid, sizeof(uint32_t), 1, f);
            fwrite(&is_program, sizeof(uint8_t), 1, f);
            fwrite(&name_len, sizeof(uint16_t), 1, f);
            fwrite(p->name, 1, name_len, f);
        }
    }
    fclose(f);
    return 1;
}

// unsigned char* session_to_bytes(session *s, size_t *out_size)
// {
//     if (!s) return NULL;

//     // First, calculate required size
//     size_t size = 0;
//     size += sizeof(uint32_t); // timestamp_count
//     size += s->timestamp_count * sizeof(uint64_t); // timestamps
//     size += s->timestamp_count * sizeof(uint32_t); // process_list_length

//     for (uint32_t i = 0; i < s->timestamp_count; i++) {
//         for (uint32_t j = 0; j < s->process_list_length[i]; j++) {
//             size += sizeof(uint32_t); // pid
//             size += sizeof(uint8_t);  // is_program
//             size += sizeof(uint16_t); // name_len
//             size += NAME_LEN;         // name
//         }
//     }

//     // Allocate buffer
//     unsigned char *buf = malloc(size);
//     if (!buf) return NULL;

//     unsigned char *ptr = buf;

//     // Write timestamp_count
//     uint32_t timestamp_count = s->timestamp_count;
//     memcpy(ptr, &timestamp_count, sizeof(uint32_t));
//     ptr += sizeof(uint32_t);

//     // Write timestamps
//     for (uint32_t i = 0; i < s->timestamp_count; i++) {
//         uint64_t ts = (uint64_t)s->timestamps[i];
//         memcpy(ptr, &ts, sizeof(uint64_t));
//         ptr += sizeof(uint64_t);
//     }

//     // Write process_list_length
//     for (uint32_t i = 0; i < s->timestamp_count; i++) {
//         uint32_t length = s->process_list_length[i];
//         memcpy(ptr, &length, sizeof(uint32_t));
//         ptr += sizeof(uint32_t);
//     }

//     // Write processes
//     for (uint32_t i = 0; i < s->timestamp_count; i++) {
//         for (uint32_t j = 0; j < s->process_list_length[i]; j++) {
//             process *p = &s->process_list[i][j];
//             uint32_t pid = p->pid;
//             uint8_t is_program = p->is_program ? 1 : 0;
//             uint16_t name_len = (uint16_t)NAME_LEN;

//             memcpy(ptr, &pid, sizeof(uint32_t)); ptr += sizeof(uint32_t);
//             memcpy(ptr, &is_program, sizeof(uint8_t)); ptr += sizeof(uint8_t);
//             memcpy(ptr, &name_len, sizeof(uint16_t)); ptr += sizeof(uint16_t);
//             memcpy(ptr, p->name, NAME_LEN); ptr += NAME_LEN;
//         }
//     }

//     *out_size = size;
//     return buf; // caller must free
// }


// char **get_common_names(session *s, unsigned int i, unsigned int *out_count){
//     process **a = s->process_list[i];
//     process **b = s->process_list[i+1];

//     unsigned int len_A = s->process_list_length[i];
//     unsigned int len_B = s->process_list_length[i+1];

//     unsigned int max_len = (len_A < len_B) ? len_A : len_B;

//     unsigned int x = 0, y = 0;
//     unsigned int count = 0;

//     char **result = malloc(max_len * sizeof(char *));
//     while (x < len_A && y < len_B) {
//         int cmp = strcmp(a[x]->name, b[y]->name);

//         if (cmp == 0){
//             result[count++] = strdup(a[x]->name);
//             char *current = a[x]->name;

//             while (x < len_A && strcmp(a[x]->name, current) == 0) x++;

//             while (y < len_B && strcmp(b[y]->name, current) == 0) y++;
//         }
//         else if (cmp<0) x++;
//         else if (cmp>0) y++;

//     }
//     *out_count = count;
//     return result;
// }

// unsigned int find_name_in_process_timer(process_timer *t, const char *name){
//     unsigned int left = 0;
//     unsigned int right = t->length - 1;

//     if (right == 0) reutrn NULL;

//     while (left<=right){
//         unsigned int mid = (left+right)/2;

//         int cmp = strcmp(t->process_names[mid], name);
//         if (cmp == 0) return mid;
//         else if (cmp<0) left = mid+1;
//         else right = mid-1;
//     }
//     return NULL;
// }

// bool get_time_run(FILE *f, session *s, process_timer *result){
//     if (s == NULL && f !=NULL)
//         while (construct_next_session(f, s));
//     else if (s == NULL && f == NULL) 
//         return 0;
//     else if (f == NULL && s != NULL){
//             sort_session(s);
//             for (unsigned int i = 0;i<s->process_list_length-1;i++){
//                 unsigned int index =  find_name_in_process_timer(result, s->process_list[i]);
//                 if (index == NULL){
//                     result->length++;
//                     result->process_names = realloc(result->process_names, (result->length+1)*sizeof(char*));

//                     result->process_names[result->length] =  malloc(sizeof(char)*NAME_LEN);
//                 }

//             }
//         }
//     return 0;
// }

