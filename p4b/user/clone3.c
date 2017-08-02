/* clone copies file descriptors, but doesn't share */
#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "x86.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
volatile uint newfd = 0;

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

   int fd = open("tmp", O_WRONLY|O_CREATE);
   assert(fd == 3);
   int clone_pid = clone(worker, 0);
   assert(clone_pid > 0);
 //worker(0);
   while(!newfd); //printf(1, "NEWFD: %d\n", newfd);
   printf(1, "PROBLEM IN WHILE LOOP\n");
   int wr = write(newfd, "goodbye\n", 8);
   printf(1,"WR: %d\n",wr);
   assert(wr == -1);
//   assert(write(newfd, "goodbye\n", 8) == -1);
   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
int pointer = 0;
  printf(1, " %d", pointer);
   assert(write(3, "hello\n", 6) == 6);
   int temp = open("tmp2", O_WRONLY|O_CREATE);
 //  printf(1,"TEMP: %d\n", temp);
   xchg(&newfd, temp);
}

