#include "kernel/types.h"
#include "user/user.h"

int main() {
    int *p;

    printf("rrobando acceso a memoria invalida...\n");


    p = (int*)0xFFFFFFFF;
    printf("leyendo de direccion invalida...\n");
    printf("%d\n", *p);

    
    p = (int*)0x40000000;
    printf("recribiendo en direccion invalida...\n");
    *p = 10;

    exit(0);
}
