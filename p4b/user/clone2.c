/* clone and play with the argument */
#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
volatile int arg = 55;
volatile int global = 1;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();
   printf(1,"ppid =%d\n",ppid);
   int clone_pid = clone(worker, (void*)&arg);
   printf(1,"clone_pid =%d\n",clone_pid);
   assert(clone_pid > 0);
   while(global != 55);
   assert(arg == 1);
   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   int tmp = *(int*)arg_ptr;
   printf(1,"tmp =%d\n",tmp);
   *(int*)arg_ptr = 1;
   printf(1,"*arg_ptr =%d\n",*(int*)arg_ptr);
   assert(global == 1);
   global = tmp;
   printf(1,"global =%d\n",global);
}
