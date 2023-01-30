
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <iostream>
#include <string>
#include <unistd.h>	   // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);
#include <fcntl.h>	   // for open, write, read

using namespace std;

MyDB_PageHandle MyDB_BufferManager ::getPage(MyDB_TablePtr whichTable, long i)
{
	map<pair<string, long>, Page_Map_Item>::iterator it;
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

MyDB_PageHandle MyDB_BufferManager ::getPinnedPage(MyDB_TablePtr whichTable, long i)
{
	// .getName()
	string tablename = whichTable.getName();
	string loc = whichTable.getStorageLoc();
	// create repoitory if not exists
	mkdir(("./" + loc).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	// check tablename file existance,
	// if it is the first time being called, create the table

	// check DiskPageMap see if that table / page is in buffer
	map<pair<string, long>, Page>::iterator it;
	it = MyDB_BufferManager::diskPageMap.find({whichTable, i}); // make_pair(whichTable, i)

	// page not exist, store into buffer

	// key not in map
	if (it == MyDB_BufferManager::diskPageMap.end())
	{
		// 還沒搬過來 loadbuffer function in clock_LRU
		reloadBufferItem(whichTable, i, true, false);
		Page_Buffer_Item *tempBufferItemPtr; //

		// 記得load完buffer要把更新的page存到map裡
		MyDB_BufferManager::diskPageMap[make_pair(whichTable, i)] = Page(i, tempBufferItemPtr, refCnt++);
		// 結束後得到正確page object
		it = MyDB_BufferManager::diskPageMap.find({whichTable, i}); // make_pair(whichTable, i)
	}
	// key in map, but page points to null
	else if (it->second.bufferItemPtr == nullptr)
	{
		// 還沒搬過來 loadbuffer function in clock_LRU
		reloadBufferItem(whichTable, i, true, false);
		Page_Buffer_Item *tempBufferItemPtr; //
		MyDB_BufferManager::diskPageMap[make_pair(whichTable, i)].bufferItemPtr = tempBufferItemPtr;
	}

	// access buffer, if unpin then pin it, then make a handle
	Page *tempPagePtr = &(it->second);
	tempPagePtr->bufferItemPtr->isPinned = true;

	// 有點不確定
	MyDB_PageHandle tempHandle;
	tempHandle->position = tempPagePtr;

	return tempHandle;
}

MyDB_PageHandle MyDB_BufferManager ::getPinnedPage()
{
	// find a PageBufferItemSpace, the clockArm points to the available bufferItem Space
	clockarmGetSpace()
	
	// create page to anonyPageMap
	// the disk(tempfile) is fd_tempfile, anonySeq keep the track of current anonymous page
	// anonySeq 不一定會用到，除非evict 且有handle 時才會存進去
	// 可以想想一下是不是evict 時才放這個 anonySeq
	// 要怎麼回收anonySeq? 大部分可能都用不到？
	map<pair<string, long>, Page>::iterator it;
	MyDB_BufferManager::anonyPageMap[make_pair(fd_tempFile, anonySeq)] = Page(anonySeq, clockArm, refCnt++);
	it = MyDB_BufferManager::anonyPageMap.find({fd_tempFile, anonySeq});
	anonySeq++;

	// access buffer, if unpin then pin it, then make a handle
	Page *tempPagePtr = &(it->second);
	tempPagePtr->bufferItemPtr->isPinned = true;
	
	
	// 有點不確定
	MyDB_PageHandle tempHandle;
	tempHandle->position = tempPagePtr;

	return tempHandle;
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

	// open a tempfile with name: tempFile for anonymous page
	string tempFilePath = "./" + tempFile;
	fd_tempFile = creat(tempFilePath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd_tempFile < 0)
	{
		cout << "Unable to create " << tempFile << endl;
		exit(EXIT_FAILURE);
	}

	// anonymous save page sequence number
	anonyseq = 0;
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

	//
	close(fd_tempFile);
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
void MyDB_BufferManager ::bufferToDisk(MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony)
{
	// set acedbit to True

	// clockArm++

	// update map, set map[pageNum].bufferItemPtr = nullptr
}

// load data from disk to buffer
bool MyDB_BufferManager ::diskToBuffer(long ItemSlotIdx, MyDB_TablePtr whichTable, long pageNum){
	ofstream myFile.open(
		"./" + whichTable.getStorageLoc() + "/" + whichTable.getName())}

vector<Page_Buffer_Item>::iterator MyDB_BufferManager ::clockarmGetSpace()
{
	// clock arm movement
	while (true)
	{
		// currentPage exceed the max_len of bufferPage
		// reset pointer to 0
		if (clockArm > colckBuffer.end())
		{
			clockArm = clockBuffer.begin();
		}
		// if item slot(page) is pinned, just move arm to next item slot
		if ((*clockArm).isPinned)
		{
			clockArm++;
			continue;
		}

		// found the page to evict (acedBit == false)
		// store dirty data to disk/tempFile/delete & set & move arm
		if (!(*clockArm).acedBit)
		{
			// clean the data
			if ((*clockArm).isDirty)
			{
				// Anonymous & no handle
				// delete

				// Anonymous & with handle
				// save to a temp file

				// disk: update disk page
			}
			//(*clockArm).acedBit = true;

			// return index of available item slot
			// then increment the index;
			return clockArm++;
		}
		// when arm point to a setted item, unset & move arm to next item slot
		(*clockArm).acedBit = false;
		clockArm++;
	}
}

// (1) buffMgr wants page i, and finds it's not in buffer (points to NULL)
// (2) first time load data to pageItem & map
void reloadBufferItem(MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony)
{
	// return idx of available slot to laod data
	clockarmGetSpace();
	// load data from disk to buffer
	MyDB_BufferManager::bufferToDisk(whichTable, pageNum, isPinned, isAnony);

	// return which slotIdx the page is loaded
	// return newItemSlotIdx;
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
