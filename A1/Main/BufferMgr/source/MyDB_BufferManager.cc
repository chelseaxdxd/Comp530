
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <string>
#include <unistd.h>	   // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);

using namespace std;

MyDB_PageHandle MyDB_BufferManager ::getPage(MyDB_TablePtr whichTable, long i)
{
	it = MyDB_BufferManager::diskPageMap.find({whichTable, i});

	// page not exist, store into buffer and return handle
	if (it == MyDB_BufferManager::diskPageMap.end())
	{
		// 還沒搬過來 loadbuffer function in clock_LRU
	}

	// page exist in buffer, return the handle
	else
	{
		//
	}

	return nullptr;
}

MyDB_PageHandle MyDB_BufferManager ::getPage()
{
	// find an unpin or pinned anonymous page in buffer, return handle

	// an anonymous page not existin buffer, create an anonlymous(temp) page,return the handle
	return nullptr;
}

MyDB_PageHandle MyDB_BufferManager ::getPinnedPage(MyDB_TablePtr, long)
{
	// page not exist, store into buffer, if unpin then pin it and return handle

	// page exist in buffer, if unpin then pin it and return the handle
	return nullptr;
}

MyDB_PageHandle MyDB_BufferManager ::getPinnedPage()
{
	// find an anonymous pinned page in buffer, return handle

	// an anonymous pinned page not existin buffer, create an anonlymous(temp) page, pin it, and return the handle
	return nullptr;
}

void MyDB_BufferManager ::unpin(MyDB_PageHandle unpinMe)
{
}

MyDB_BufferManager ::MyDB_BufferManager(size_t pageSize, size_t numPages, string tempFile)
{
	// declare buffer pool

	this->numPages = numPages;
	clockBuffer.assign(numPages, Page_Buffer_Item(pageSize));
	clockArm = clockBuffer.begin();
}

// when the page buffer is destroyed
// - disk page:
// dirty pages need to be written back to disk,
// any necessary data needs to be written to the catalog
// - anonymous page:
// |-- still have handle on it: written to a temporary file, and return the address
// |-- no handle: delete
MyDB_BufferManager ::~MyDB_BufferManager()
{
	// delete [] headPagePtr;
}

// Update item
void MyDB_BufferManager ::updateBufferItem(Page_Buffer_Item *buffItemPtr, long pageNum, bool isPinned, bool isDirty, bool isAnony, bool acedBit)
{
	buffItemPtr->pageNum = pageNum;
	buffItemPtr->isPinned = isPinned;
	buffItemPtr->isDirty = isDirty;
	buffItemPtr->isAnony = isAnony;
	buffItemPtr->acedBit = acedBit;
}

// Update pageData
void MyDB_BufferManager ::updatePagedata(Page_Buffer_Item *buffItemPtr, vector<char> newPageData)
{
	buffItemPtr->pageData = newPageData // depend on how pageData is defined
}

// when Clock_LRU needs to evict page, store the dirty data to disk
void MyDB_BufferManager ::bufferToDisk(long ItemSlotIdx, MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony);

// load data from disk to buffer
bool MyDB_BufferManager ::diskToBuffer(long ItemSlotIdx, MyDB_TablePtr whichTable, long pageNum)
{
	ofstream myFile.open(
		"./" + whichTable.getStorageLoc() + "/" + whichTable.getName())
}

long MyDB_BufferManager ::findItemSlot()
{
	// currentPage exceed the max_len of bufferPage
	// reset pointer to 0
	if (clockArm > colckBuffer.end())
	{
		clockArm = clockBuffer.begin();
	}

	Page_Buffer_Item *currBuffPage;
	// clock arm movement
	while (true)
	{
		currBuffPage = buffPagePool[this->currBuffPageIdx];

		// if item slot(page) is pinned, just move arm to next item slot
		if (currBuffPage.isPinned)
		{
			this->currBuffPageIdx++;
			continue;
		}

		// found the page to evict (acedBit == false)
		// store dirty data to disk/tempFile/delete & set & move arm
		if (!currBuffPage.acedBit)
		{
			// clean the data
			if (currBuffPage.isDirty)
			{
				// Anonymous & no handle
				// delete

				// Anonymous & with handle
				// save to a temp file

				// disk: update disk page
			}
			currBuffPage.acedBit = true;

			// return index of available item slot
			// then increment the index;
			return this->currBuffPageIdx++;
		}
		// when arm point to a setted item, unset & move arm to next item slot
		currBuffPage.acedBit = false;
		this->currBuffPageIdx++;
	}
}

// (1) buffMgr wants page i, and finds it's not in buffer (points to NULL)
// (2) buffMgr go to idx i at buffPagePool, and finds the pageNum is inconsistent to its records
Page_Buffer_Item *reloadBufferItem(MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony)
{
	// return idx of available slot to laod data
	vector<Page_Buffer_Item>::iterator newItemSlotIdx = getBufferItemSpace();
	// load data from disk to buffer
	MyDB_BufferManager::bufferToDisk(newItemSlotIdx, whichTable, pageNum, isPinned, isAnony);
	// return which slotIdx the page is loaded
	return newItemSlotIdx;
}

/*
// buffMgr wants to update data in buffer i (points to page i)
	// update newPage, mark isDirty & acedBit to true, return True == succeed
	bool writeBuffer(long pageNum, vector<char> pageData);

	// buffMgr wants to access page i data (points to page i)
	// mark acedBit to true
	vector<char> readBuffer(long pageNum);

	// buffMgr marks page i as pinned (points to page i)
	bool pinThisPage(long pageNum);
*/

#endif
