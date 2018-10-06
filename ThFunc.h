#ifndef __THFUNC_H__
#define __THFUNC_H__

#include "Thread.h"
#include <signal.h>

Thread* curThread;

Thread* make_Thread();
void __thread_wait_handler(int signo);
void __thread_wakeup(Thread* wake);
Thread* __getThread(thread_t tid);
Thread* __getThreadW(thread_t tid);

/* Double linked list functions */
void push_Thread(Thread **head, Thread **tail, Thread *new);
Thread *pop_Thread(Thread **head, Thread **tail);
void remove_Thread(Thread **head, Thread **tail, Thread* target);
BOOL empty_Queue(Thread *head);

#endif /* __THFUNC_H__ */
