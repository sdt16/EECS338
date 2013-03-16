#include <stdio.h>
#include <unistd.h>
#include "sem_common.h"

int main(int argc, char* argv[]) {

  int semid = get_semid();
  int shmid = shmem_get_id(sizeof(sh_data));

  //fprintf(stderr, "semid: %d, shmid: %d\n", semid, shmid);

  sh_data* data = (sh_data*)shmem_get_ptr(shmid);

  sem_wait(semid, MUTEX);
  if (data->h_count >= 2) {
    //fprintf(stderr, "h count > 2, pid: %d\n", getpid());
    data->b_count = 2;
    sem_signal(semid, H_SEM);
    sem_signal(semid, H_SEM);
  } else { 
    // fprintf(stderr, "h count < 2, pid: %d\n", getpid());
    data->o_count++;
    sem_signal(semid, MUTEX);
    //fprintf(stderr, "about to wait for O_SEM, pid: %d\n", getpid());
    sem_wait(semid, O_SEM);
    //fprintf(stderr, "passed O_SEM, pid: %d\n", getpid());
    sem_wait(semid, BARRIER);
    data->o_count--;
    data->b_count--;
    if (data->b_count != 0) {
      sem_signal(semid, BARRIER);
    } else {
      sem_signal(semid, BARRIER);
      sem_signal(semid, MUTEX);
    }
  }

  printf("Oxygen is bonding, pid: %d\n", getpid());

  return 0;
}
