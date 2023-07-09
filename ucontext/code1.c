#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

int main() {
    // int done = 0;
    ucontext_t one, two;
    getcontext(&one);
    printf("Running...\n");
    sleep(1);
    // if (!done) {
    //  done = 1;
    swapcontext(&two, &one);
    //}
    return 0;
}