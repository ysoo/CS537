/* clone and join syscalls */
#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
int global = 1;

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
   int arg = 42;
   int clone_pid = clone(worker, &arg);
   assert(clone_pid > 0);
   printf(1,"clone pid =%d\n",clone_pid);
   int join_pid = join();
   assert(join_pid == clone_pid);
   printf(1,"global = %d\n",global);
   assert(global == 2);

   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   int arg = *(int*)arg_ptr;
   assert(arg == 42);
   printf(1,"arg =%d\n",arg);
   assert(global == 1);
   global++;
}

