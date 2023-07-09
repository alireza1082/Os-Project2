#include <stdio.h>
#include "threadlib.h"

void *test(void *arg)
{
    int i = *(int*)arg;
    int loop = 4;
    while(loop > 0)
    {
        printf("thread %d: %d\n", i, loop);
        loop--;
        yield();
    }
}

int main()
{
    thread_t g0, g1;
    int a0 = 0;
    int a1 = 1;
    create(&g0, test, &a0);
    create(&g1, test, &a1);

    join(&g0, NULL);
    join(&g1, NULL);
    printf("done\n");
    return 0;
}