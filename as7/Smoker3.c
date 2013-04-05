#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Smoking.h"


int* get_rpc_value(int* (*func)(void*, CLIENT*), CLIENT* clnt)
{
  int *result = (*func)((void*)NULL, clnt);
  if (result == NULL) {
    clnt_perror(clnt, "call failed: ");
  }
  return result;
}


void smoker_1(char* host, int argc, char* argv[])
{
  sleep(10);
  CLIENT *clnt;
  int *paper, *tobacco, *matches;

  clnt = clnt_create(host, SMOKING, AS7, "udp");
  if (clnt == NULL) {
    clnt_pcreateerror(host);
    exit(1);
  }

  paper = get_rpc_value(get_paper_1, clnt);
  tobacco = get_rpc_value(get_tobacco_1, clnt);
  matches = get_rpc_value(get_matches_1, clnt);

  if (*paper < 1) {
    printf("NO PAPER!\n");
    exit(1);
  } else {
    printf("PAPER IS AVAILABLE! P: %d\n", *paper);
  }

  if (*tobacco < 1) {
    printf("NO TOBACCO!\n");
    exit(1);
  } else {
    printf("TOBACCO IS AVAILABLE! T: %d\n", *tobacco);
  }

  if (*matches < 1) {
    printf("NO MATCHES\n");
    exit(1);
  } else {
    printf("MATCHES IS AVAILABLE! M: %d\n", *matches);
  }

  printf("SMOKING! SMOKER 3\n");
}

int main(int argc, char* argv[])
{
  char *host;

  if(argc < 2) {
    printf("usage: %s server_host ...\n", argv[0]);
    exit(1);
  }
  host = argv[1];
  smoker_1(host, argc, argv);
  return 0;
}
