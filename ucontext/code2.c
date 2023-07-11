#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>

static int running_process_id;
static ucontext_t mContext[2];
static ucontext_t mContext_main;
static ucontext_t mContext_done;
static int p1_flag1;
static int p1_flag2;
static int p1_done;
static int p2_flag1;
static int p2_flag2;
static int p2_done;

void yield()
{
    printf("yield from pid %d to pid %d\n", running_process_id, 1 - running_process_id);
    if (running_process_id == 0)
    {
        p1_flag1 = 1;
        running_process_id = 1;
        swapcontext(&mContext[0], &mContext[1]);
    }
    else
    {
        p2_flag1 = 1;
        running_process_id = 0;
        swapcontext(&mContext[1], &mContext[0]);
    }
}

void go_to_done(int pid){
    if (pid == 0)
        p1_done = 1;
    if (pid == 1)
        p2_done = 1;
    printf("switch to done context");
    swapcontext(&mContext[pid], &mContext_done);
}

void some_job(int pid, int max_num)
{
    for(int i = -max_num; i < 0; i++)
        printf("pid %d -> %d\n", pid, i);
    yield();
    for(int i = 1; i <= max_num; i++)
        printf("pid %d -> %d\n", pid, i);
    if (pid == 0)
        p1_flag2 = 1;
    else if (pid == 1)
        p2_flag2 = 1;
    go_to_done(pid);
}

void switch_to_main(int num){
    printf("switch to main context from done context %d", num);
    swapcontext(&mContext_done, &mContext_main);
}

void make_done(){
    getcontext(&mContext_done);
    printf("make done context");
    makecontext(&mContext_done, (void(*) (void)) switch_to_main, 1, 1);
}

int main(int argc, char *argv[])
{
    char stack1[4*1024];
    char stack2[4*1024];

    getcontext(&mContext[0]);
    mContext[0].uc_stack.ss_sp = stack1;
    mContext[0].uc_stack.ss_size = sizeof(stack1);
    p1_flag1 = 0;
    p1_flag2 = 0;
    p1_done = 0;
    makecontext(&mContext[0], (void(*) (void)) some_job, 2, 1, 3);
//-----------------------------------------------------------------
    getcontext(&mContext[1]);
    mContext[1].uc_stack.ss_sp = stack2;
    mContext[1].uc_stack.ss_size = sizeof(stack2);
    p2_flag1 = 0;
    p2_flag2 = 0;
    p2_done = 0;
    makecontext(&mContext[1], (void(*) (void))some_job, 2, 2, 5);

    make_done();
    running_process_id = 0;
    swapcontext(&mContext_main, mContext);
    printf("Returned to main function");
    return 0;

}
