
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_PageHandle.h"

void *MyDB_PageHandleBase ::getBytes()
{
	// check if data still in buffer
	if (position->bufferItemPtr == nullptr)
	{
		// @@@reload from disk
	}
	char *pageDataPtr = &((*(position->bufferItemPtr)).pageData[0]);
	return pageDataPtr;
}

void MyDB_PageHandleBase ::wroteBytes()
{

	// flag dirty in buffer
	(*(position->bufferItemPtr)).isDirty = true;
	//(that when we want to pass the data back to disk we know that it is dirty)
}

MyDB_PageHandleBase ::~MyDB_PageHandleBase()
{
	// remember to decrease reference cnt
	(position)->refCnt--;

	// if no handle to this buffer than evict
	if ((position)->refCnt == 0)
	{
		destructBufferItem(*position);
		// @@@anony 要刪掉在map裡的該page，但不做也沒關係？可以做成map背call到的時候檢查有沒有空的就刪？但效率好像很差
	}
}

#endif
