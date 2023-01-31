
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_PageHandle.h"

void *MyDB_PageHandleBase ::getBytes()
{
	char *pageDataPtr = &((*(MyDB_PageHandleBase::position->bufferItemPtr)).pageData[0]);
	return pageDataPtr;
}

void MyDB_PageHandleBase ::wroteBytes()
{

	// flag dirty in buffer
	(*(MyDB_PageHandlecBase::position->bufferItemPtr)).isDirty = true;
	//(that when we want to pass the data back to disk we know that it is dirty)
}

MyDB_PageHandleBase ::~MyDB_PageHandleBase()
{
	// remember to decrease reference cnt
	(MyDB_PageHandleBase::position)->refCnt--;
}

#endif
