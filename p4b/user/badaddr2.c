/* thread should not access other thread's stack */
#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)
int ppid;
int global = 0;
char *ptr;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);
void worker2(void *arg_ptr);

int
main(int argc, char *argv[])
{
    int ppid = getpid();
    assert(clone(worker, 0) > 0);
    sleep(1);
    assert(clone(worker2, 0) > 0);
    assert(join() > 0);
    assert(join() > 0);
    printf(1, "TEST PASSED\n");
    exit();
}

void
worker(void *arg_ptr) {
    ptr = (char *)(0xa0000 - 3 * PGSIZE);
    strcpy(ptr, "README");
    assert(open(ptr, O_WRONLY|O_CREATE) > 0);
    ptr = (char *)(0xa0000- 5 * PGSIZE);
    strcpy(ptr, "README");
    assert(open(ptr, O_WRONLY|O_CREATE) < 0);
}

void
worker2(void *arg_ptr) {
    ptr = (char *)(0xa0000 - 5 * PGSIZE);
    strcpy(ptr, "README");
    assert(open(ptr, O_WRONLY|O_CREATE) > 0);
    ptr = (char *)(0xa0000 - 3 * PGSIZE);
    strcpy(ptr, "README");
    assert(open(ptr, O_WRONLY|O_CREATE) < 0);
}

