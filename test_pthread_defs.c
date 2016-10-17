#include <pthread.h>
typedef pthread_t THREAD_T;
struct event_t {
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  int exited;
};

typedef struct event_t EVENT_T;
#define CREATE_EVENT(event) \
  ({ pthread_cond_init(&((event).cond), nullptr); \
   pthread_mutex_init(&(event).mutex, nullptr); \
   (event).exited = false; \
   })

#define SIGNAL_EVENT(event) \
  ({ pthread_mutex_lock(&(event).mutex); \
   (event).exited = true; \
   pthread_cond_signal(&(event).cond); \
   pthread_mutex_unlock(&(event).mutex); \
   })

#define WAIT_FOR_SINGLE_OBJECT(event) \
  ({ pthread_mutex_lock(&(event).mutex); \
   while (!(event).exited) { \
   pthread_cond_wait(&(event).cond, &(event).mutex); \
   } \
   pthread_mutex_unlock(&(event).mutex); \
   })

int main() {
  EVENT_T ev;
  CREATE_EVENT(ev);

  WAIT_FOR_SINGLE_OBJECT(ev);
}
