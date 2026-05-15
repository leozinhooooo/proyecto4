#include "kernel/types.h"
#include "user/user.h"

#define PAGES 10
#define PGSIZE 4096

int
main()
{
  char *seq;
  char *disp;
  int i;

  printf("reservando memoria \n");

  seq = sbrk(PAGES * PGSIZE);

  printf("acceso secuencial\n");

  for(i = 0; i < PAGES; i++){
    seq[i * PGSIZE] = 1;
  }

  printf("fin acceso secuencial\n");

  //coño

  printf("reservando memoria dispersa...\n");

  disp = sbrk(PAGES * PGSIZE);

  printf("acceso dispers\n");

  disp[9 * PGSIZE] = 1;
  disp[2 * PGSIZE] = 1;
  disp[7 * PGSIZE] = 1;
  disp[1 * PGSIZE] = 1;
  disp[5 * PGSIZE] = 1;

  printf("fin del acceso disperso\n");

  printf("fin del lazy allocation\n");

  exit(0);
}
