
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_PageHandle.h"

void *MyDB_PageHandleBase :: getBytes () {
	return nullptr;
}

void MyDB_PageHandleBase :: wroteBytes () {

	// flag dirty in buffer 
	//(that when we want to pass the data back to disk we know that it is dirty)
}

MyDB_PageHandleBase :: ~MyDB_PageHandleBase () {
}

#endif

