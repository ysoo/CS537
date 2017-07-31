#include "server_impl.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>

int *arg, i, numfull, produce, consume, numbuf,numtrd;
int *buffer;

pthread_cond_t empty;// condition signal
pthread_cond_t fill; // condition signal
pthread_mutex_t mutex;// mutex
void* consumer();
void server_init(int argc, char* argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s server [portnum] [threads] [buffers]\n", argv[0]);
    exit(1);
  }
  if(argv[2] <= 0) {
    perror("number of threads not positive");
    exit(1);
  }
  if(argv[3] <= 0) {
    perror("number of buffers not positive");
    exit(1);
  }
// create pool of threads here
  numbuf = *(int*)argv[3];
  buffer=malloc(sizeof(uint)*numbuf);//may need to create a struct in ulib.h
  for(i = 0; i < numbuf; i++) buffer[i] = -1;
// -1 means free
  numfull = 0;
  produce = 0;
  consume = 0;


  numtrd = *(int*)argv[2];
  pthread_t t[numtrd];
  if(pthread_cond_init(&empty,NULL) != 0) exit(1);
  if(pthread_cond_init(&fill,NULL) != 0) exit(1);
  if(pthread_mutex_init(&mutex,NULL) != 0) exit(1);


  for(i = 0; i < numtrd; i++) {
    if(pthread_create(&t[i],NULL,consumer,NULL) == 0)
      exit(1);
    pthread_join(t[i],NULL);
  }

}
void *consumer() {
// a consumer, when locking fill a buffer, when unlock, free a buffer

  pthread_mutex_lock(&mutex);

  while(numfull == 0){
    pthread_cond_wait(&fill,&mutex);
  }

  int connfd = *arg;
  free(arg);
  buffer[consume]=-1;
  consume=(consume+1)%numbuf;
  numfull-=1;

  pthread_cond_signal(&empty);
  pthread_mutex_unlock(&mutex);
  requestHandle(connfd);

  return NULL;
}

void server_dispatch(int connfd) { // handle a request
// Handle the request in the main thread.  
  
  arg = malloc(sizeof(int));
  *arg = connfd;
  consumer();
  pthread_mutex_lock(&mutex);
  while(numfull == numbuf){
    pthread_cond_wait(&empty,&mutex);
  }

  while(buffer[produce]!= -1){
  produce=(produce+1)%numbuf;
  }

  buffer[produce]=connfd;
  produce=(produce+1)%numbuf;
  numfull+=1;
  pthread_cond_signal(&fill);
  pthread_mutex_unlock(&mutex);


}
