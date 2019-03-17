/** Write a multithreaded program that outputs prime numbers. This program should work as follows: 
 *  The user will run the program and will enter a number on the command line. The program will then 
 *  create a separate thread that outputs all the prime numbers less than or equal to the number 
 *  entered by the user. (You can use Java or C)
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void find_primes_below(int starting_number) {
    int n = starting_number;
    while (n > 2) {
        int foundDivisor = 0;
        for (int i = n - 1; i > 1; i--) {
            if ((n % i) == 0) {
                foundDivisor = 1;
            }
        }
        if (foundDivisor != 1) {
            printf("Prime: %d\n", n);
        }
        n--;
    }
}

void main(int argc, char **argv) {
    if (argc != 2) {
        puts("Invalid Argument. Please use:\n./program2 <positive integer>");
        return;
    }

    char *arg1 = argv[1];
    // puts(arg1);
    int starting_int = atoi(arg1); // ascii-to-integer C standard function

    if (starting_int < 0) {
        printf("%d: must be in domain [0-2147483647]\n", starting_int);
        return;
    }

    int ret = fork();
    if (ret > 0)
        find_primes_below(starting_int);
    else
        wait(0);
}