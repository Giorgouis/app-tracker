#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "processes.h"

int main(int argc, char **argv){
    printf("%d\n\n", argc);
    for (int i = 0; i<argc; i++){
        printf("%i | %s\n", i, argv[i]);
    }
    if (argc>=2 && strcmp(argv[1], "test") == 0){
        DWORD pids[2048];
        DWORD count = 0;

        printf("Gathering system processes...\n\n");

        // 1. Get raw PIDs using your stack-based array
        get_processes(pids, &count);

        if (count == 0) {
            printf("Failed to retrieve processes.\n");
            return 1;
        }

        // 2. Use your malloced function to create the descriptive list
        struct process *list = get_all_process_info_malloced(pids, count);

        if (list == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }

        // 3. Print a header for our table
        printf("%-8s | %-30s | %s\n", "PID", "NAME", "CATEGORY");
        printf("----------------------------------------------------------\n");

        // 4. Loop through and display the data
        for (DWORD i = 0; i < count; i++) {
            // Only print if the PID is valid (skipping ones we couldn't open)
            if (list[i].pid != 0) {
                char *type = list[i].is_program ? "User App (Chrome/Spotify)" : "System/Background";
                printf("%-8lu | %-30s | %s\n", list[i].pid, list[i].name, type);
            }
        }

        // 5. Cleanup: Critical in C!
        free(list);

        printf("\nFinished. Total processes scanned: %lu\n", count);}
    return 0;
}








