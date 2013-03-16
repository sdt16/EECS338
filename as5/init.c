#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#include "sem_common.h"

int main(int argc, char* argv[]) {

  int semid = sem_init();

  int shmid = shmem_create(sizeof(sh_data));
  sh_data* data = (sh_data*) shmem_get_ptr(shmid);

  data->h_count = 0;
  data->o_count = 0;
  data->b_count = 0;

  pid_t pid1, pid2, pid3, pid4;
    
  if((pid1=fork()) == 0) {
    fprintf(stderr, "hydrogen 1 pid: %d\n", getpid());
    execl("./hydrogen", "", (char*) NULL);
  } else if((pid2=fork()) == 0) {
    fprintf(stderr, "oxygen 1 pid: %d\n", getpid());
    execl("./oxygen", "", (char*) NULL);
  } else if((pid3=fork()) == 0) {
    fprintf(stderr, "hydrogen 2 pid: %d\n", getpid());
    execl("./hydrogen", "", (char*) NULL);
  } else if((pid4=fork()) == 0) {
    fprintf(stderr, "oxygen 2 pid: %d\n", getpid());
    execl("./oxygen", "", (char*) NULL);
  }

  waitpid(-1, 0, 0);
  sleep(2);

  sem_del(semid);
  shmem_del(shmid);

  return 0;
}
