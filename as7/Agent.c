#include <rpc/rpc.h>
#include "Smoking.h"

static int paper_count = 3;
static int tobacco_count = 3;
static int matches_count = 3;

void process_num(int* num)
{
  if (*num > 0) {
    *num = *num - 1;
  }
}

int *get_paper_1(void* ptr, CLIENT* clnt) 
{
  process_num(&paper_count);
  return &paper_count;
}

int *get_tobacco_1(void* ptr, CLIENT* clnt) 
{
  process_num(&tobacco_count);
  return &tobacco_count;
}

int *get_matches_1(void* ptr, CLIENT* clnt) 
{
  process_num(&matches_count);
  return &matches_count;
}

int *get_paper_1_svc(void* ptr,  struct svc_req * svc_r)
{ 
  CLIENT* client;
  return get_paper_1(ptr, client);
}

int *get_tobacco_1_svc(void* ptr,  struct svc_req * svc_r)
{ 
  CLIENT* client;
  return get_tobacco_1(ptr, client);
}

int *get_matches_1_svc(void* ptr,  struct svc_req * svc_r)
{ 
  CLIENT* client;
  return get_matches_1(ptr, client);
}

