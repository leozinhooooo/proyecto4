#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  char *p;

  p = sbrk(4096);

  p[0] = 'A';

  printf("lazy allocation funcionando\n");

  exit(0);
}

