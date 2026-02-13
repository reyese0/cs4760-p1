//Elisa Reyes
//02/10/2026

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//Prints help message
void print_help() {
    printf("How to use: oss [-h] [-n proc] [-s simul] [-t iter]\n");
    printf("  -h      Show help message\n");
    printf("  -n proc Total number of children to launch\n");
    printf("  -s simul Maximum number of children to run simultaneously\n");
    printf("  -t iter Number of iterations to pass to the user process\n");
}

int main(int argc, char* argv[]) {
    const char optstr[] = "hn:s:t:";
    char opt;
    pid_t pid;
    int totalChildren = 0;
    int maxSimultaneous = 0;
    int iterations = 0;
    int currentProcesses = 0;
    int totalProcesses = 0;

    //Parse command line arguments
    while ((opt = getopt(argc, argv, optstr)) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                return 0;
            case 'n':
                totalChildren = atoi(optarg);
                break;
            case 's':
                maxSimultaneous = atoi(optarg);
                break;
            case 't':
                iterations = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Invalid option\n");
                print_help();
                return 1;
        }
    }

    printf("OSS: Launching %d children, %d simultaneous, with %d iterations\n",
           totalChildren, maxSimultaneous, iterations);

    while(currentProcesses < maxSimultaneous && totalProcesses < totalChildren) {
        pid = fork(); //copy process from the parent
        if (pid < 0) {
            perror("Fork failed");
            return 1;
        } else if (pid == 0) { //child process
            char strArg[20]; 
            sprintf(strArg, "%d", iterations); //Convert iterations to string and pass it as an argument to the user process
            char* args[] = {"./user", strArg, NULL}; //Arguments to pass to the user process
            execvp(args[0], args); 
            fprintf(stderr, "execl failed\n");
            return 1;
        } 

        currentProcesses++;
        totalProcesses++;
        printf("OSS: Created child with PID %d\n", pid);
        
    }

    while (totalProcesses < totalChildren) {
        //Wait for a child process to finish before creating a new one
        if (wait(0) > 0) { //if a child process finishes, decrease the count of current processes
            currentProcesses--;
        }

        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            return 1;
        } else if (pid == 0) { 
            char strArg[20];
            sprintf(strArg, "%d", iterations);
            char* args[] = {"./user", strArg, NULL}; 
            execvp(args[0], args); 
            fprintf(stderr, "execl failed\n");
            return 1;
        }
        
        currentProcesses++;
        totalProcesses++;
    }

    //Wait for all child processes to finish before exiting
    while (currentProcesses > 0) { 
        if (wait(0) > 0) {
            currentProcesses--;
        }
    }
    
    printf("OSS: Summary - Total processes finished %d\n", totalProcesses);
    return 0;
}
