#include "ThFunc.h"
#include "Thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

Thread* make_Thread() {
	// mutex, cond, signal
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t wait = PTHREAD_COND_INITIALIZER;
	signal(SIGUSR1, __thread_wait_handler);

	// Thread
	Thread* new_th = (Thread*)malloc(sizeof(Thread));
	new_th->status = THREAD_STATUS_READY;
	new_th->tid = thread_self();

	new_th->readyMutex = lock;
	new_th->readyCond = wait;

	new_th->bRunnable = 0; // false
	new_th->pPrev = NULL;
	new_th->pNext = NULL;

	return new_th;
}

void __thread_wait_handler(int signo) {
	Thread* pTh;

	pTh = __getThread(thread_self());
	if(pTh == NULL)
		pTh = curThread;

	pthread_mutex_lock(&(pTh->readyMutex));
	while (pTh->bRunnable == 0)
		pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
	pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_wakeup(Thread* wake){
	pthread_mutex_lock(&(wake->readyMutex));
	if(wake->status != THREAD_STATUS_ZOMBIE)
		wake->status = THREAD_STATUS_RUN;
	wake->bRunnable = 1;
	pthread_cond_signal(&(wake->readyCond));
	pthread_mutex_unlock(&(wake->readyMutex));
}

Thread* __getThread(thread_t tid){
	Thread *target;

	for(target=ReadyQTail; target!=NULL; target=target->pPrev)
		if(target->tid == tid)return target;
	return NULL;
}

Thread* __getThreadW(thread_t tid){
	Thread *target;

	for(target=WaitQTail; target!=NULL; target=target->pPrev)
		if(target->tid == tid)return target;
	return NULL;
}

void push_Thread(Thread **head, Thread **tail, Thread *new) {
	if (empty_Queue((*head)))
		(*head) = new;
	else {
		(*tail)->pNext = new;
		new->pPrev = (*tail);
	}
	(*tail) = new;
}

void remove_Thread(Thread **head, Thread **tail, Thread* target){
	for(Thread* i=(*head); i!=NULL; i=i->pNext){
		if(i==target){
			if(i==(*head))
				pop_Thread(head, tail);
			else if(i==(*tail)){
				Thread* temp = (*tail);
				(*tail)=(*tail)->pPrev;
				(*tail)->pNext=NULL;
				temp->pPrev=NULL;
			}
			else{
				i->pNext->pPrev = i->pPrev;
				i->pPrev->pNext = i->pNext;
				i->pPrev = NULL;
				i->pNext = NULL;
			}

			break;
		}
	}
}

Thread *pop_Thread(Thread **head, Thread **tail) {
	Thread *th;
	if (empty_Queue((*head)))
		return NULL;
	else if((*head)->pNext == NULL){
		th = (*head);
		(*head) = NULL;
		(*tail) = NULL;
	}
	else{
		th = (*head);
		(*head) = (*head)->pNext;
		(*head)->pPrev = NULL;
		th->pNext = NULL;
	}

	return th;
}

BOOL empty_Queue(Thread *head) {
	if (head == NULL)
		return 1;
	else
		return 0;
}
