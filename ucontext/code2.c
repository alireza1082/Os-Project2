#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>

static int running_process_id;
static ucontext_t mContext[2];
static ucontext_t mContext_main;
static ucontext_t mContext_done;
static int p1_flag;

void yield()
{
    printf("yield from pid %d to pid %d\n", running_process_id, 1 - running_process_id);
    if (running_process_id == 0)
    {
        running_process_id = 1;
        swapcontext(&mContext[0], &mContext[1]);
    }
    else
    {
        running_process_id = 0;
        swapcontext(&mContext[1], &mContext[0]);
    }
}

void some_job(int pid, int max_num)
{
    for(int i = -max_num; i < 0; i++)
        printf("pid %d -> %d\n", pid, i);
    yield();
    for(int i = 1; i <= max_num; i++)
        printf("pid %d -> %d\n", pid, i);
}

int main(int argc, char *argv[])
{
    char stack1[4*1024];
    char stack2[4*1024];

    getcontext(&mContext[0]);
    mContext[0].uc_stack.ss_sp = stack1;
    mContext[0].uc_stack.ss_size = sizeof(stack1);
    makecontext(&mContext[0], (void(*) (void)) some_job, 2, 1, 3);
//-----------------------------------------------------------------
    getcontext(&mContext[1]);
    mContext[1].uc_stack.ss_sp = stack2;
    mContext[1].uc_stack.ss_size = sizeof(stack2);
    makecontext(&mContext[1], (void(*) (void))some_job, 2, 2, 5);
    
    running_process_id = 0;
    swapcontext(&mContext_main, mContext);
    printf("Returned to main function");
    return 0;

}
