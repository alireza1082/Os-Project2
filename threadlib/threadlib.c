#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include "threadlib.h"

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096

static ucontext_t main_cntx = {0};
static thread_t main_thread = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};

static thread_t *running = &main_thread;

static void init() __attribute__ ((constructor));
void init() 
{
    getcontext(&main_cntx);
}

void thread()
{
    thread_t *this = running;

    void *result = (*this->fun)(this->arg);
    // place waiting (joining) thread in ready queue
    ///
    ///
    ///
    // save result of execution
    ///
    ///
    ///
    // we're a zombie
    ///
    ///
    ///
    // find the next thread to run
    thread_t *next = NULL;
    ///
    ///
    ///
    running = next;
    setcontext(next->context);
}

int create(thread_t *new, void *(*fun) (void*), void *arg)
{
    ucontext_t *cntx = (ucontext_t *) malloc(sizeof(ucontext_t));
    getcontext(cntx);

    void *stack = malloc(STACK_SIZE);

    cntx->uc_stack.ss_sp = stack;
    cntx->uc_stack.ss_size = STACK_SIZE;
    makecontext(cntx, thread, 0);

    new->context = cntx;
    new->fun = fun;
    new->arg = arg;
    new->next = NULL;
    new->join = NULL;
    new->retval = NULL;
    new->zombie = FALSE;
    // add new to the ready queue
    ///
    ///
    ///
    return 0 ;
}


int yield()
{
    thread_t *susp = running;
    // add susp to ready queue
    ///
    ///
    ///
    // select the next thread for execution
    thread_t *next = NULL;
    ///
    ///
    ///
    running = next;
    swapcontext(susp->context, next->context);
    return 0;
}

int join(thread_t *thread, void **res) 
{
    if(!thread->zombie) 
    {
        thread_t *susp = running;
        // add as joining thread
        ///
        ///
        ///
        // select the next thread for execution
        thread_t *next = NULL;
        ///
        ///
        ///
        running = next;
        swapcontext(susp->context, next->context);
    }
    // collect result
    ///
    ///
    ///
    // free context
    ///
    ///
    ///
    return 0;
}

