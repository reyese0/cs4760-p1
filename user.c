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

int main(int argc, char *argv[]) {
    const char optstr[] = "hn:s:t:";
    char opt;
    pid_t pid;
    int totalChildren = 0;
    int maxSimultaneous = 0;
    int iterations = 0;

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
}