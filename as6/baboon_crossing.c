#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define TOTAL_A 20
#define TOTAL_B 20

int xing_count, xed_count, to_b_wait_count, to_a_wait_count;

typedef enum _xingdir {
  none,
  a_to_b,
  b_to_a
} xing_dir;

xing_dir curr_dir;

sem_t mutex, to_b, to_a, print_mutex;

int ids[TOTAL_A + TOTAL_B];

unsigned short xsubi[3];

unsigned int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 */

/* reference: http://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130 */

    long int divisor = RAND_MAX/(limit+1);
    unsigned int retval;

    do { 
      retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

void sleep_rand(int max_ms) {
  struct timespec rand_time;
  unsigned int time_to_sleep = rand_lim(max_ms);
  rand_time.tv_sec = 0;
  rand_time.tv_nsec = time_to_sleep * 100000 + 1;
  if (rand_time.tv_nsec > 999999999) {
    rand_time.tv_nsec = 999999999;
  }
  //fprintf(stderr, "nsec val: %ld\n", rand_time.tv_nsec);
  if (nanosleep(&rand_time, NULL) != 0) {
    perror("nanosleep");
    exit(1);
  }
}

void print_thread_info(char* status, char* type, int* id) {
  sem_wait(&print_mutex);
  time_t time_val;
  time(&time_val);

  printf("Thread ID: %d, Status: %s, Type: %s, Current Time: %s\n", *id, status, type, ctime(&time_val));

  sem_post(&print_mutex);
}

void* run_a_baboons(void* thread_id) {
  //unsigned short xsubi[3];
  // print_thread_info("About to start", "A to B", thread_id);
  sleep_rand(1000);
  sem_wait(&mutex);
  if ((curr_dir == a_to_b || curr_dir == none) && xing_count < 5 && (xed_count + xing_count) < 10) {
    curr_dir = a_to_b;
    xing_count++;
    sem_post(&mutex);
  } else {
    //print_thread_info("Waiting to cross", "B to A", thread_id);
    to_b_wait_count++;
    sem_post(&mutex);
    sem_wait(&to_b);
    to_b_wait_count--;
    xing_count++;
    curr_dir = a_to_b;
    sem_post(&mutex);
  }


  int valp;
  sem_getvalue(&mutex, &valp);
  //fprintf(stderr, "thread id: %d, mutex: %d\n", *((int*)thread_id), valp);

  print_thread_info("Crossing", "A to B", thread_id);
  sleep_rand(10000);
  print_thread_info("Done crossing", "A to B", thread_id);

  sem_getvalue(&mutex, &valp);
  //fprintf(stderr, "thread id: %d, mutex: %d\n", *((int*)thread_id), valp);

  sem_wait(&mutex);
  xed_count++;
  xing_count--;
  if (xing_count > 0 && xing_count <= 4 && (((xed_count + xing_count) < 10 && to_b_wait_count == 0) || 
					    ((xed_count + xing_count) >= 10 && to_a_wait_count != 0))) {
    sem_post(&mutex);
  } else if (xing_count >= 0 &&  xing_count <= 4 && (((xed_count + xing_count) < 10 && to_b_wait_count != 0) || 
						   ((xed_count+xing_count) >= 10 && to_a_wait_count == 0 && to_b_wait_count != 0))) {
    sem_post(&to_b);
  } else if ((xing_count == 0 && (((xed_count + xing_count) < 10 && to_b_wait_count == 0 && to_a_wait_count != 0) || 
				  ((xed_count+xing_count) >= 10 && to_a_wait_count != 0)))) {
    curr_dir = b_to_a;
    xed_count = 0;
    sem_post(&to_a);    
  } else if (xing_count == 0 && to_b_wait_count == 0 && to_a_wait_count == 0) {
    curr_dir = none; 
    xed_count = 0; 
    sem_post(&mutex);
  } else {
    print_thread_info("ERROR", "A to B", thread_id);
    fprintf(stderr, "xed_count: %d, xing_count: %d, to_a: %d, to_b: %d, xing_dir: %d\n", xed_count, xing_count, to_a_wait_count, to_b_wait_count, curr_dir);
    exit(1);
  }


  pthread_exit(0);
  return NULL;
}

void* run_b_baboons(void* thread_id) {
  //unsigned short xsubi[3];
  //print_thread_info("About to start", "B to A", thread_id);
  sleep_rand(1000);
  sem_wait(&mutex);
  if ((curr_dir == b_to_a || curr_dir == none) && xing_count < 5 && (xed_count + xing_count) < 10) {
    curr_dir = b_to_a;
    xing_count++;
    sem_post(&mutex);
  } else {
    // print_thread_info("Waiting to cross", "B to A", thread_id);
    to_a_wait_count++;
    sem_post(&mutex);
    sem_wait(&to_a);
    to_a_wait_count--;
    xing_count++;
    curr_dir = b_to_a;
    sem_post(&mutex);
  }

  int valp;
  sem_getvalue(&mutex, &valp);
  //fprintf(stderr, "thread id: %d, mutex: %d\n", *((int*)thread_id), valp);

  print_thread_info("Crossing", "B to A", thread_id);
  sleep_rand(10000);
  print_thread_info("Done crossing", "B to A", thread_id);

  sem_getvalue(&mutex, &valp);
  //fprintf(stderr, "thread id: %d, mutex: %d\n", *((int*)thread_id), valp);

  sem_wait(&mutex);
  xed_count++;
  xing_count--;
  if (xing_count > 0 && xing_count <= 4 && (((xed_count + xing_count) < 10 && to_a_wait_count == 0) || 
					    ((xed_count + xing_count) >= 10 && to_b_wait_count != 0))) {
    sem_post(&mutex);
  } else if (xing_count >= 0 &&  xing_count <= 4 && (((xed_count + xing_count) < 10 && to_a_wait_count != 0) || 
						   ((xed_count+xing_count) >= 10 && to_b_wait_count == 0 && to_a_wait_count != 0))) {
    sem_post(&to_a);
  } else if ((xing_count == 0 && (((xed_count + xing_count) < 10 && to_a_wait_count == 0 && to_b_wait_count != 0) || 
				  ((xed_count+xing_count) >= 10 && to_b_wait_count != 0)))) {
    curr_dir = a_to_b;
    xed_count = 0;
    sem_post(&to_b);
  } else if (xing_count == 0 && to_a_wait_count == 0 && to_b_wait_count == 0) {
    curr_dir = none; 
    xed_count = 0; 
    sem_post(&mutex);
  } else {
    print_thread_info("ERROR", "B to A", thread_id);
fprintf(stderr, "xed_count: %d, xing_count: %d, to_a: %d, to_b: %d, xing_dir: %d\n", xed_count, xing_count, to_a_wait_count, to_b_wait_count, curr_dir);
    exit(1);
  }

  pthread_exit(0);
  return NULL;
}


int main(int argc, char* argv[]) {

  xing_count = 0;
  xed_count = 0;
  to_b_wait_count = 0;
  to_a_wait_count = 0;
  curr_dir = none;

  sem_init(&mutex, 0, 1);
  sem_init(&to_b, 0, 0);
  sem_init(&to_a, 0, 0);
  sem_init(&print_mutex, 0, 1);

  srand(time(NULL));

  pthread_t a_threads[TOTAL_A];
  pthread_t b_threads[TOTAL_B];

  for (int i = 0; i < TOTAL_A; i++) {
    ids[i] = i;
    if (pthread_create(&a_threads[i], NULL, run_a_baboons, (void*)&ids[i])) {
      perror("pthread_create a");
      exit(1);
    }
  }
  for (int i = 0; i < TOTAL_B; i++) {
    ids[TOTAL_A + i] = i;
    if (pthread_create(&b_threads[i], NULL, run_b_baboons, (void*)&ids[i])) {
      perror("pthread_create b");
      exit(1);
    }
  }

  void* status;

   for (int i = 0; i < TOTAL_A; i++) {
    if (pthread_join(a_threads[i], &status) == -1) {
      perror("Joining a");
      exit(1);
    }
  }

   for (int i = 0; i < TOTAL_B; i++) {
    if (pthread_join(b_threads[i], &status) == -1) {
      perror("Joining b");
      exit(1);
    }
  }
    

  return 0;
}
