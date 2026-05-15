#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  char *p;

  printf("reservando region del mmap simulada\n");

  p = (char*)mapzero(4096);

  printf("primer byte %c\n", p[0]);
  printf("segundo byte %c\n", p[1]);

  exit(0);
}
