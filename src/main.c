#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "processes.h"
#include "session.h"
#include"data.h"


int main(int argc, char **argv){
    

    char output[128] = "output.txt";
    bool read = false;
    DWORD count = 6;
    DWORD delta_t = 6000;
    (void)read; (void)count; (void)delta_t;
    for (int i =0; i<argc;i++){
        if ((strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-output") == 0 ) && i+1<argc){
            strcpy(output, argv[i+1]);
        }

        else if ((strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "-count") == 0 ) && i+1<argc){
            count = (DWORD)strtoul(argv[i+1], NULL, 10);
        }
        
        else if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-times") == 0 || strcmp(argv[i], "-time") == 0 ) && i+1<argc){
            delta_t = (DWORD)strtoul(argv[i+1], NULL, 10);
        }

        else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-read") == 0) read = true;
    }

    session s;
    init_session(&s);
    if (delta_t<1000) delta_t = 1000;
    for (DWORD i =0; i<count;i++){
        update_session(&s, NULL, 0);
        Sleep(delta_t);
        
    }

    write_session_const_bytelen("sessions.bin", &s);
    free_session(&s);

    return 0;
}

