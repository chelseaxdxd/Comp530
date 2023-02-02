
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_BufferManager.h"
#include "MyDB_PageHandle.h"
#include <iostream>
using namespace std;

void *MyDB_PageHandleBase ::getBytes()
{
	Page_Buffer_Item *tempItemPtr;
	// check if data still in buffer
	if (position->bufferItemPtr == nullptr)
	{
		if (position->isAnony)
		{
			tempItemPtr = reloadTempFile(position->pageNum);
		}
		else
		{
			tempItemPtr = reloadFromDisk(position->tablePath, position->pageNum);
		}
		position->bufferItemPtr = tempItemPtr;
	}

	char *pageDataPtr = &(position->bufferItemPtr->pageData[0]);
	return pageDataPtr;
}

void MyDB_PageHandleBase ::wroteBytes()
{

	// flag dirty in buffer
	position->bufferItemPtr->isDirty = true;
	//(that when we want to pass the data back to disk we know that it is dirty)
}

MyDB_PageHandleBase ::~MyDB_PageHandleBase()
{

	// remember to decrease reference cnt
	(position)->refCnt--;

	// if no handle to this buffer than evict
	if ((position)->refCnt == 0)
	{
		bool dirty = position->bufferItemPtr->isDirty;
		position->bufferItemPtr->acedBit = false;
		position->bufferItemPtr->isDirty = false;
		position->bufferItemPtr->isPinned = false;
		if (!(position->bufferItemPtr->isAnony) && (dirty))
		{
		bm->bufferToDisk(position->bufferItemPtr);
		}
	}
}

// (1) buffMgr wants page i, and finds it's not in buffer (points to NULL)
// (2) first time load data to pageItem & map
Page_Buffer_Item *MyDB_PageHandleBase ::reloadFromDisk(string tablePath, long pageNum)
{
	// load data from disk to buffer
	return bm->diskToBuffer(tablePath, pageNum);
}
// anonymous: load data from tempfile in disk to buffer
Page_Buffer_Item *MyDB_PageHandleBase ::reloadTempFile(long slot)
{
	return bm->tempFileToBuffer(slot);
}

#endif
