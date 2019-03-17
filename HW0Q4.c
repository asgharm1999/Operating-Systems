/** The Fibonacci sequence is the series of numbers 0, 1, 1, 2, 3, 5, 8, .... 
 *  Formally, it can be expressed as: 
 *  fib0 = 0 fib1 = 1 fibn = fibn−1 + fibn−2 
 *  Write a multithreaded program that generates the Fibonacci sequence. This program 
 *  should work as follows: On the command line, the user will enter the number of 
 *  Fibonacci numbers that the program is to generate. The program will then create a 
 *  separate thread that will generate the Fibonacci numbers, placing the sequence in 
 *  data that can be shared by the threads (an array is probably the most convenient data structure). 
 *  When the thread finishes execution, the parent thread will output the sequence generated by the 
 *  hild thread. Because the parent thread cannot begin outputting the Fibonacci sequence until the 
 *  child thread finishes, the parent thread will have to wait for the child thread to finish. Use the 
 *  techniques described in Section 4.4 to meet this requirement. (You can use Java or C) 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>

int fibonacci(int number);

int sum; // this data is shared by the threads

void *runner(void * param); // the thread 

int main(int argc, char *argv[]) {

    int count, i;

    pthread_attr_t attr;

    if (argc != 2) {
        fprintf(stderr, "usage: pthreads <integer value>\n");
        return -1;
    }

    count = atoi(argv[1]);

    if (count < 1) {
        fprintf(stderr, "%d must be>= 1\n", count);
        return -1;
    }

    pthread_attr_init(&attr);

    for (i = 1; i <= count; i++) {
        pthread_t thread;
        pthread_create(&thread, &attr, runner, (void *)(intptr_t)i);
        pthread_join(thread, NULL);
        printf("fib of %d is %d\n", i, sum);
    }
}

void *runner(void *param) {
    sum = fibonacci((uintptr_t)param);
    
    pthread_exit(0);
}

int fibonacci(int x) {

    if (x <= 1) {
        return 1;
    }

    return fibonacci(x - 1) + fibonacci(x - 2);
}
