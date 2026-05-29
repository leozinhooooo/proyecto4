#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define PROT_READ  0x1
#define PROT_WRITE 0x2

int
main(void)
{
    int fd = open("README", 0);

    if(fd < 0){
        printf("open failed\n");
        exit(1);
    }

    char *p = (char*)mmap((uint64)0x40000000,
                          4096,
                          PROT_READ,
                          0,
                          fd);

    if((uint64)p == -1){
        printf("mmap failed\n");
        exit(1);
    }

    printf("mapped addr = %p\n", p);

    
    printf("first char ascii = %d\n", p[0]);
    printf("%c%c%c%c\n", p[0], p[1], p[2], p[3]);

int pid = fork();

if(pid == 0){

    printf("child sees: %c\n", p[0]);

    exit(0);

} else {

    wait(0);

    printf("parent done\n");
}

if(munmap((uint64)p) < 0){
    printf("munmap failed\n");
} else {
    printf("munmap ok\n");
}

    close(fd);

    exit(0);
}
