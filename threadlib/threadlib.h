#include <ucontext.h>

typedef struct thread_t {
    ucontext_t *context;
    void *(*fun)(void*);
    void *arg;
    struct thread_t *next;
    struct thread_t *join;
    void *retval;
    int zombie;
} thread_t;

int create(thread_t *thread, void *(*fun)(void*), void *arg);
int yield();
int join(thread_t *thread, void **val);