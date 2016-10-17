#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
typedef pthread_t THREAD_T;
struct event_t {
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  int exited;
};

typedef struct event_t EVENT_T;
#define CREATE_EVENT(event) \
  ({ pthread_cond_init(&((event).cond), NULL); \
   pthread_mutex_init(&(event).mutex, NULL); \
   (event).exited = 0; \
   })

#define SIGNAL_EVENT(event) \
  ({ pthread_mutex_lock(&(event).mutex); \
   (event).exited = 1; \
   pthread_cond_signal(&(event).cond); \
   pthread_mutex_unlock(&(event).mutex); \
   })

#define WAIT_FOR_SINGLE_OBJECT(event) \
  ({ pthread_mutex_lock(&((event).mutex)); \
   while (!(event).exited) { \
   pthread_cond_wait(&((event).cond), &((event).mutex)); \
   } \
   pthread_mutex_unlock(&((event).mutex)); \
   })



/* parameter structure for every thread */
struct parameter {
	char character; /* printed character */
	int number;     /* how many times */
};

EVENT_T ev;
/* the function performed by every thread */
void* print_character(void *params)
{
	struct parameter* p = (struct parameter*) params;
	int i;

	for (i=0;i<p->number;i++)
		printf("%c", p->character);
	printf("\n");
  fflush(stdout);
  printf("Waiting to release the cond\n");
  sleep(1);
  SIGNAL_EVENT(ev);
  printf("I released the cond\n");

	return NULL;
}

int main()
{
	pthread_t fir1, fir2;
	struct parameter fir1_args, fir2_args;

  CREATE_EVENT(ev);

	/* create one thread that will print 'x' 11 times */
	fir1_args.character = 'x';
	fir1_args.number = 11;
	if (pthread_create(&fir1, NULL, &print_character, &fir1_args)) {
		perror("pthread_create");
		exit(1);
	}


  WAIT_FOR_SINGLE_OBJECT(ev);
  //pthread_mutex_lock(&lock);
  //while (!exit_cond) {
  //  printf("Waiting for the thread to unlock the cond\n");
  //  pthread_cond_wait(&cond, &lock);
  //}
  //pthread_mutex_unlock(&lock);
  //printf("the thread unlocked the cond\n");

	/* wait for completion */
	if (pthread_join(fir1, NULL))
		perror("pthread_join");

	return 0;
}
