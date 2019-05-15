/** The Sleeping Teaching Assistant A university computer science department has a teaching 
 *  assistant (TA) who helps undergraduate students with their programming assignments during 
 *  regular office hours. The TA’s office is rather small and has room for only one desk with
 *  a chair and computer. There are three chairs in the hallway outside the office where students
 *  can sit and wait if the TA is currently helping another student. When there are no students
 *  who need help during office hours, the TA sits at the desk and takes a nap. If a student
 *  arrives during office hours and finds the TA sleeping, the student must awaken the TA to ask
 *  for help. If a student arrives and finds the TA currently helping another student, the student
 *  sits on one of the chairs in the hallway and waits. If no chairs are available, the student 
 *  will come back at a later time. 
 * 
 *  Implement a solution that coordinates the activities of the TA and the students. Details for 
 *  this assignment are provided below. 
 * 
 *  The Students and the TA Using threads, begin by creating n students. Each will run as a separate
 *  thread. The TA will run as a separate thread as well. Student threads will alternate between
 *  programming for a period of time and seeking help from the TA. If the TA is available, they will
 *  obtain help. Otherwise, they will either sit in a chair in the hallway or, if no chairs are available,
 *  will resume programming and will seek help at a later time. If a student arrives and notices that the
 *  TA is sleeping, the student must notify the TA using a semaphore. When the TA finishes helping a
 *  student, the TA must check to see if there are students waiting for help in the hallway. If so, the
 *  TA must help each of these students in turn. If no students are present, the TA may return to napping. 
 * 
 *  Perhaps the best option for simulating students programming—as well as the TA providing help to a
 *  student—is to have the appropriate threads sleep for a random period of time. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <pthread.h> // pthread_t, pthread_create, pthread_join
#include <semaphore.h> // sem_init, sem_wait, sem_post
#include <time.h>
#include <unistd.h>


#define NUM_SEAT 3

#define SLEEP_MAX 5

// sematphores
sem_t sem_stu;
sem_t sem_ta;

// sem_t mutex;
pthread_mutex_t mutex;

int chair[3];
int count = 0; // number of waiting students
int next_seat = 0;
int next_teach = 0;

void rand_sleep(void);
void* stu_programming(void* stu_id);
void* ta_teaching();

// written by Muhammad
int main(int argc, char **argv){

	// thread(s)
	pthread_t *students;
	pthread_t ta;
	
	int* student_ids;
	int student_num;
	
	// index
	int i;

	// get number of students from user
	printf("How many students? ");
	scanf("%d", &student_num);

	// initialize
	students = (pthread_t*)malloc(sizeof(pthread_t) * student_num);
	student_ids = (int*)malloc(sizeof(int) * student_num);

	memset(student_ids, 0, student_num);

	sem_init(&sem_stu, 0, 0);
	sem_init(&sem_ta, 0, 1);
	
	// set random
	srand(time(NULL));

	// sem_init(&mutex,0,1);
	pthread_mutex_init(&mutex, NULL);

	// create thread
	pthread_create(&ta, NULL, ta_teaching, NULL);

	// create threads
	for(i=0; i<student_num; i++) {
		student_ids[i] = i + 1;
		pthread_create(&students[i], NULL, stu_programming, (void*) &student_ids[i]);
	} 
	
	pthread_join(ta, NULL);
	
	for(i=0; i<student_num;i++) {
		pthread_join(students[i], NULL);
	}	
	
	return 0;
}

// written by Wadri
void* stu_programming(void* stu_id) {
	int id = *(int*)stu_id;

	printf("[stu] student %d is programming\n",id);		
	
	while(1) {
		rand_sleep();

		// sem_wait(&mutex);
		pthread_mutex_lock(&mutex);

		if(count < NUM_SEAT) {
			chair[next_seat] = id;
			count++;
			
			printf("    [stu] student %d is waiting\n", id);
			printf("waiting students : [1] %d [2] %d [3] %d\n", chair[0], chair[1], chair[2]);
			next_seat = (next_seat + 1) % NUM_SEAT;
			
			// sem_post(&mutex);
			pthread_mutex_unlock(&mutex);
	
			// wakeup ta
			sem_post(&sem_stu);
			sem_wait(&sem_ta);
		}
		else { // no more chairs
			// sem_post(&mutex);
			pthread_mutex_unlock(&mutex);
			
			printf("[stu] no more chairs. student %d is programming\n", id);		
		}
	}				
}

// written by James
void* ta_teaching() {
	while(1) {
		sem_wait(&sem_stu);	
		
		// sem_wait(&mutex);
		pthread_mutex_lock(&mutex);
		
		printf("		[ta] TA is teaching student %d\n",chair[next_teach]);	
		chair[next_teach]=0;
		count--;
		
		printf("waiting students : [1] %d [2] %d [3] %d\n",chair[0],chair[1],chair[2]);
		next_teach = (next_teach + 1) % NUM_SEAT;
		
		rand_sleep();

		printf("		[ta] teaching finish.\n");	

		// sem_post(&mutex);
		pthread_mutex_unlock(&mutex);

		sem_post(&sem_ta);
	}	
}

// written by Muhammad
void rand_sleep(void) {
	int time = rand() % SLEEP_MAX + 1;
	sleep(time);
}