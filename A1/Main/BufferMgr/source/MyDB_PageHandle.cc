
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_BufferManager.h"
#include "MyDB_PageHandle.h"
#include <iostream>
using namespace std;

void *MyDB_PageHandleBase ::getBytes()
{
	cout << "\ngetBytes" << endl;
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
	cout << "end of getBytes" << endl;
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
		position->bufferItemPtr->acedBit = false;
		position->bufferItemPtr->isDirty = false;
		// destructBufferItem(*position);
		//  @@@anony 要刪掉在map裡的該page，但不做也沒關係？可以做成map背call到的時候檢查有沒有空的就刪？但效率好像很差
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
