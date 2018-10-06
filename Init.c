#include "Init.h"
#include "Thread.h"
#include "ThFunc.h"
#include <stdio.h>

void Init(){
	ReadyQHead = NULL;
	ReadyQTail = NULL;

	WaitQHead = NULL;
	WaitQTail = NULL;

	curThread = NULL;
}
