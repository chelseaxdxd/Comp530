
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <iostream>
#include <string>
#include <unistd.h>	   // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);
#include <fcntl.h>	   // for open, write, read
#include <sys/stat.h>  // creat function

using namespace std;

MyDB_PageHandle MyDB_BufferManager ::getPage(MyDB_TablePtr whichTable, long i)
{
	// get tablename and loc from my MyDB_TablePtr
	string loc = whichTable->getStorageLoc();
	string tablename = whichTable->getName();

	/* only do at first time, create repository and table if not exist*/
	set<string>::iterator iterLoc = tableLocations.find(loc);
	set<string>::iterator iterName = tableNames.find(tablename);

	if ((iterLoc == tableLocations.end()))
	{
		mkdir(("./" + loc).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		tableLocations.insert(loc);
	}

	if ((iterLoc == tableNames.end()))
	{
		string tablePath = ("./" + loc + "/" + tablename).c_str();
		int fd_tablePath = creat(tablePath.c_str(), O_RDWR);
		if (fd_tablePath < 0)
		{
			cout << "Unable to create table with table Path:" << tablePath << endl;
			exit(EXIT_FAILURE);
		}
		tableNames.insert(tablename);
	}

	/*
	check if {table, page} is in DiskPageMap
	1. not in map: load for first time
	2. in map but null: has other handles, load back to disk
	3. in map: no need to change map
	*/
	map<pair<string, long>, Page>::iterator iterMap;
	iterMap = diskPageMap.find(make_pair(whichTable, i));

	// 1. page not in map
	if (iterMap == diskPageMap.end())
	{
		// get data from disk to buffer
		Page_Buffer_Item *tempBufferItemPtr = diskToBuffer(loc, tablename, i);

		// create a page pointing to buffer item and insert to map
		diskPageMap[make_pair(whichTable, i)] = Page(i, tempBufferItemPtr, 0);

		// get page obj
		iterMap = diskPageMap.find(make_pair(whichTable, i)); // {whichTable, i}
	}
	// 2. key in map, but page points to null
	else if (iterMap->second.bufferItemPtr == nullptr)
	{
		// get data from disk to buffer
		Page_Buffer_Item *tempBufferItemPtr = diskToBuffer(loc, tablename, i);
		iterMap->second.bufferItemPtr = tempBufferItemPtr;
	}

	// refCnt increase by one
	iterMap->second.refCnt++;

	// make handle
	Page *tempPagePtr = &(iterMap->second);
	MyDB_PageHandle tempHandle;
	tempHandle->position = tempPagePtr;

	return tempHandle;
}

MyDB_PageHandle MyDB_BufferManager ::getPage()
{
	/*
		get a space from tempfile
		store that page to anonyPageMap
	*/
	// assign a slot number store in page for later store back to disk use
	int slotNum = anonySeq;
	anonySeq++;

	clockarmGetSpace(); // we don't call diskToBuffer cuz we don't need to, but we still need a place to store clockArm

	anonyPageMap[anonySeq] = Page(anonySeq, clockArm, 1);

	// make handle
	Page *tempPagePtr = &(anonyPageMap[anonySeq]);
	MyDB_PageHandle tempHandle;
	tempHandle->position = tempPagePtr;

	return tempHandle;
}

MyDB_PageHandle MyDB_BufferManager ::getPinnedPage(MyDB_TablePtr whichTable, long i)
{
	// get tablename and loc from my MyDB_TablePtr
	string loc = whichTable->getStorageLoc();
	string tablename = whichTable->getName();

	/* only do at first time, create repository and table if not exist*/
	set<string>::iterator iterLoc = tableLocations.find(loc);
	set<string>::iterator iterName = tableNames.find(tablename);

	if ((iterLoc == tableLocations.end()))
	{
		mkdir(("./" + loc).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		tableLocations.insert(loc);
	}

	if ((iterLoc == tableNames.end()))
	{
		string tablePath = ("./" + loc + "/" + tablename).c_str();
		int fd_tablePath = creat(tablePath.c_str(), O_RDWR);
		if (fd_tablePath < 0)
		{
			cout << "Unable to create table with table Path:" << tablePath << endl;
			exit(EXIT_FAILURE);
		}
		tableNames.insert(tablename);
	}

	/*
	check if {table, page} is in DiskPageMap
	1. not in map: load for first time
	2. in map but null: has other handles, load back to disk
	3. in map: no need to change map
	*/
	map<pair<string, long>, Page>::iterator iterMap;
	iterMap = diskPageMap.find(make_pair(whichTable, i));

	// 1. page not in map
	if (iterMap == diskPageMap.end())
	{
		// get data from disk to buffer
		Page_Buffer_Item *tempBufferItemPtr = diskToBuffer(loc, tablename, i);

		// create a page pointing to buffer item and insert to map
		diskPageMap[make_pair(whichTable, i)] = Page(i, tempBufferItemPtr, 0);

		// get page obj
		iterMap = diskPageMap.find(make_pair(whichTable, i)); // {whichTable, i}
	}
	// 2. key in map, but page points to null
	else if (iterMap->second.bufferItemPtr == nullptr)
	{
		// get data from disk to buffer
		Page_Buffer_Item *tempBufferItemPtr = diskToBuffer(loc, tablename, i);
		iterMap->second.bufferItemPtr = tempBufferItemPtr;
	}

	// access buffer, if unpin then pin it
	//@@@如果可pin值還夠的話
	iterMap->second.bufferItemPtr->isPinned = true;

	// refCnt increase by one
	iterMap->second.refCnt++;

	// make handle
	Page *tempPagePtr = &(iterMap->second);
	MyDB_PageHandle tempHandle;
	tempHandle->position = tempPagePtr;

	return tempHandle;
}

MyDB_PageHandle MyDB_BufferManager ::getPinnedPage()
{
	/*
	get a space from tempfile
	store that page to anonyPageMap
	*/
	// assign a slot number store in page for later store back to disk use
	int slotNum = anonySeq;
	anonySeq++;

	clockarmGetSpace(); // we don't call diskToBuffer cuz we don't need to, but we still need a place to store clockArm

	anonyPageMap[anonySeq] = Page(anonySeq, clockArm, 1);

	// access buffer, if unpin then pin it
	//@@@如果可pin值還夠的話
	anonyPageMap[anonySeq].bufferItemPtr->isPinned = true;

	// make handle
	Page *tempPagePtr = &(anonyPageMap[anonySeq]);
	MyDB_PageHandle tempHandle;
	tempHandle->position = tempPagePtr;

	return tempHandle;
}

void MyDB_BufferManager ::unpin(MyDB_PageHandle ph)
{
	ph->position->bufferItemPtr->isPinned = false;
}

MyDB_BufferManager ::MyDB_BufferManager(size_t pageSize, size_t numPages, string tempFile)
{
	// declare clockBuffer
	this->numPages = numPages;
	clockBuffer.assign(numPages, Page_Buffer_Item(pageSize));
	clockArm = clockBuffer.begin();

	// open a tempfile with name: tempFile for anonymous page
	string tempFilePath = "./" + tempFile;
	fd_tempFile = creat(tempFilePath.c_str(), O_RDWR);
	if (fd_tempFile < 0)
	{
		cout << "Unable to create anonymous file" << tempFile << endl;
		exit(EXIT_FAILURE);
	}
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

/*
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
*/

// when Clock_LRU needs to evict page, store the dirty data to disk
void MyDB_BufferManager ::bufferToDisk(MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony)
{
	// set acedbit to True

	// clockArm++

	// update map, set map[pageNum].bufferItemPtr = nullptr
}

// non anonymous: load data from table in disk to buffer
Page_Buffer_Item *MyDB_BufferManager ::diskToBuffer(string loc, string tablename, long pageNum)
{
	string diskFilePath = "./" + loc + "/" + tablename;
	// fd_disk = open(diskFilePath.c_str(), O_CREAT | O_RDWR, 0666);
	fd_disk = open(diskFilePath.c_str(), O_RDWR | O_FSYNC, 0666);
	if (fd_disk < 0)
	{
		cout << "diskToBuffer Unable to open " << diskFilePath << endl;
		exit(1);
	}

	clockarmGetSpace();
	char *readByte[pageSize];
	lseek(fd_disk, pageSize * (pageNum - 1), SEEK_SET);
	int size = read(fd_disk, readByte, pageSize);

	clockArm->pageData.assign(readByte, readByte + pageSize);
	close(fd_disk);

	/* for debug */
	cout << "size:" << size << " pageSize" << pageSize << endl;

	char *buff = readByte;
	buff[size] = '\0';
	cout << buff << endl;
	for (int i = 0; i < pageSize; i++)
	{
		cout << clockArm->pageData[i];
	}

	return clockArm;
}

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
Page_Buffer_Item *reloadFromDisk(MyDB_TablePtr whichTable, long pageNum)
{
	// load data from disk to buffer
	return bufferToDisk(whichTable->getStorageLoc(), whichTable->getName(), pageNum);
}
// anonymous: load data from tempfile in disk to buffer
Page_Buffer_Item *reloadTempFile(long slot)
{
	return nullptr;
}

void destructBufferItem(Page p)
{
	// @@@non anony
	//  @@@buffer to disk(friend function?)
	// @@@delete buffer

	// @@@anony
	//  @@@delte that buffer
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
