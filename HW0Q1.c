/** Questions 1: The Collatz conjecture concerns what happens when we take any positive integer n and 
 *  apply the following algorithm: 
 *  The conjecture states that when this algorithm is continually applied, all positive integers will 
 *  eventually reach 1. For example, if n = 35, the sequence is 
 *  35, 106, 53, 160, 80, 40, 20, 10, 5, 16, 8, 4, 2, 1 
 *  Write a C program using the fork() system call that generates this sequence in the child process. 
 *  The starting number will be provided from the command line. For example, if 8 is passed as a parameter 
 *  on the command line, the child process will output 8, 4, 2, 1. Because the parent and child processes have 
 *  their own copies of the data, it will be necessary for the child to output the sequence. Have the parent 
 *  invoke the wait() call to wait for the child process to complete before exiting the program. Perform 
 *  necessary error checking to ensure that a positive integer is passed on the command line.
 */ 

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> 

// All numbers reach 1
void collatz_conjecture(int starting_int) {
    int shouldRun = 1;
    int n = starting_int;
    printf("%d, ", n);
    while (shouldRun == 1) {
        // this modulo operation divides 'n' by 2, then takes the remainder and checks if it is
        // equal to 0. this will return 'true' for even numbers as any even number divided by
        // 2 will have a 0 remainder (no remainder) and satisfy 0 == 0
        // we could also take a non-mathematical approach and use a binary bitwise AND operation
        // to detect evenness, like (n & 1) because the lowest bit will ALWAYS be equal to zero
        // consider: number 1 dec = 0b001, number 2 dec = 0b010, number 3 dec = 0b011, so forth.
        if ((n % 2) == 0) {
            n = n / 2;
            printf("%d", n);
        } else {
            n = 3 * n + 1;
            printf("%d", n);
        }
        if (n == 1) {
            shouldRun = 0;
        } else {
            printf(", ");
        }
    }
    printf("\n");
}

void main(int argc, char **argv) {
    // printf("hello world!\n");

    if (argc != 2) {
        puts("Invalid Argument. Please use:\n./program1 <positive integer>");
        return;
    }

    char *arg1 = argv[1];
    // puts(arg1);
    int starting_int = atoi(arg1); // ascii-to-integer C standard function

    if (starting_int < 0) {
        printf("%d: must be in domain [0-2147483647]\n", starting_int);
        return;
    }

    // On Unix-based systems, fork() is used to instanciate a concurrent CPU operation
    // on a newly created thread. On Windows, the Win32 CreateThread() API would instead
    // be used.
    // fork() will continue execution with the current scope in mind by copying everything
    // in local memory to its own memory space.

    // System call fork() is used to create processes. It takes no arguments and returns a process ID. The purpose of fork() is to create a new process, which becomes the child process of the caller. After a new child process is created, both processes will execute the next instruction following the fork() system call. Therefore, we have to distinguish the parent from the child. This can be done by testing the returned value of fork():

    // 1. If fork() returns a negative value, the creation of a child process was unsuccessful.
    // 2. fork() returns a zero to the newly created child process.
    // 3. fork() returns a positive value, the process ID of the child process, to the parent. The returned process ID is of type pid_t defined in sys/types.h. Normally, the process ID is an integer. Moreover, a process can use function getpid() to retrieve the process ID assigned to this process.
    // http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html

    // We can use property #2 of fork() to check if fork is greater than 0 and assume this means we got a valid
    // process ID (this would only happen in the main thread).
    
    int ret = fork();
    if (ret > 0)
        collatz_conjecture(starting_int);
    else
        wait(0);
    // Above wait() function pauses continued execution of program before moving on below
    // This is only done by the parent: the child thread running collatz_conjecture will
    // fall through and exit the program. We do not need a return statement here (implicit void).
}