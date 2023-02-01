
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <iostream>
#include <string>
#include <unistd.h>	   // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);
#include <fcntl.h>	   // for open, write, read
#include <sys/stat.h>

using namespace std;

MyDB_PageHandle MyDB_BufferManager ::getPage(MyDB_TablePtr whichTable, long i)
{
	// get tablename and loc from my MyDB_TablePtr
	string loc = whichTable->getStorageLoc();
	string tablename = whichTable->getName();
	string tablePath = ("./" + loc + "/" + tablename).c_str();

	/* only do at first time, create repository and table if not exist*/
	set<string>::iterator iterLoc = tableLocations.find(loc);
	set<string>::iterator iterName = tableNames.find(tablename);

	if ((iterLoc == tableLocations.end()))
	{
		mkdir(("./" + loc).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		tableLocations.insert(loc);
	}

	if ((iterName == tableNames.end()))
	{
		int fd_tablePath = open(tablePath.c_str(), O_CREAT | O_RDWR, 0666);
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
	iterMap = diskPageMap.find(make_pair(tablePath, i));

	// 1. page not in map
	if (iterMap == diskPageMap.end())
	{
		// get data from disk to buffer
		Page_Buffer_Item *tempBufferItemPtr = diskToBuffer(tablePath, i);

		// create a page pointing to buffer item and insert to map
		// Page p = {false, tablePath, i, tempBufferItemPtr, 0};
		// diskPageMap[make_pair(tablePath, i)] = p;
		Page *p = new Page;
		p->isAnony = false;
		p->tablePath = tablePath;
		p->pageNum = i;
		p->bufferItemPtr = tempBufferItemPtr;
		p->refCnt = 0;
		// p = {false, tablePath, i, tempBufferItemPtr, 0};
		diskPageMap[make_pair(tablePath, i)] = *p;

		// get page obj
		iterMap = diskPageMap.find(make_pair(tablePath, i)); // {whichTable, i}
	}
	// 2. key in map, but page points to null
	else if (iterMap->second.bufferItemPtr == nullptr)
	{
		// get data from disk to buffer
		Page_Buffer_Item *tempBufferItemPtr = diskToBuffer(tablePath, i);
		iterMap->second.bufferItemPtr = tempBufferItemPtr;
	}

	iterMap->second.bufferItemPtr->isAnony = false;
	iterMap->second.bufferItemPtr->pageNum = i;
	// refCnt increase by one
	iterMap->second.refCnt += 2;

	// make handle
	Page *tempPagePtr = &(iterMap->second);
	MyDB_PageHandleBase phb;
	phb.bm = this;
	phb.position = tempPagePtr;
	return make_shared<MyDB_PageHandleBase>(phb);
}

MyDB_PageHandle MyDB_BufferManager ::getPage()
{
	/*
		get a space from tempfile
		store that page to anonyPageMap
	*/

	clockarmGetSpace(); // we don't call diskToBuffer cuz we don't need to, but we still need a place to store clockArm

	// Page p = {true, "", anonySeq, &(*clockArm), 1};
	// anonyPageMap[anonySeq] = p;
	Page *p = new Page;
	p->isAnony = true;
	p->tablePath = "";
	p->pageNum = anonySeq;
	p->bufferItemPtr = &(*clockArm);
	p->refCnt = 2;
	anonyPageMap[anonySeq] = *p;

	anonyPageMap[anonySeq].bufferItemPtr->isAnony = true;
	anonyPageMap[anonySeq].bufferItemPtr->pageNum = anonySeq;

	// make handle

	Page *tempPagePtr = &(anonyPageMap[anonySeq]);
	anonySeq++;
	MyDB_PageHandleBase phb;
	phb.bm = this;
	phb.position = tempPagePtr;
	return make_shared<MyDB_PageHandleBase>(phb);
}

MyDB_PageHandle MyDB_BufferManager ::getPinnedPage(MyDB_TablePtr whichTable, long i)
{
	// get tablename and loc from my MyDB_TablePtr
	string loc = whichTable->getStorageLoc();
	string tablename = whichTable->getName();
	string tablePath = ("./" + loc + "/" + tablename).c_str();

	/* only do at first time, create repository and table if not exist*/
	set<string>::iterator iterLoc = tableLocations.find(loc);
	set<string>::iterator iterName = tableNames.find(tablename);

	if ((iterLoc == tableLocations.end()))
	{
		mkdir(("./" + loc).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		tableLocations.insert(loc);
	}

	if ((iterName == tableNames.end()))
	{
		int fd_tablePath = open(tablePath.c_str(), O_CREAT | O_RDWR | O_FSYNC, 0666);
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
	iterMap = diskPageMap.find(make_pair(tablePath, i));

	// 1. page not in map
	if (iterMap == diskPageMap.end())
	{
		// get data from disk to buffer
		Page_Buffer_Item *tempBufferItemPtr = diskToBuffer(tablePath, i);

		// create a page pointing to buffer item and insert to map
		Page *p = new Page;
		p->isAnony = false;
		p->tablePath = tablePath;
		p->pageNum = i;
		p->bufferItemPtr = tempBufferItemPtr;
		p->refCnt = 0;
		// p = {false, tablePath, i, tempBufferItemPtr, 0};
		diskPageMap[make_pair(tablePath, i)] = *p;

		// get page obj
		iterMap = diskPageMap.find(make_pair(tablePath, i)); // {whichTable, i}
	}
	// 2. key in map, but page points to null
	else if (iterMap->second.bufferItemPtr == nullptr)
	{
		// get data from disk to buffer
		Page_Buffer_Item *tempBufferItemPtr = diskToBuffer(tablePath, i);
		iterMap->second.bufferItemPtr = tempBufferItemPtr;
	}

	// access buffer, if unpin then pin it
	//@@@如果可pin值還夠的話
	iterMap->second.bufferItemPtr->isPinned = true;
	iterMap->second.bufferItemPtr->isAnony = false;
	iterMap->second.bufferItemPtr->pageNum = i;
	// refCnt increase by one
	iterMap->second.refCnt += 2;

	cout << "##bmcc\t\titerMap->second.refCnt: " << iterMap->second.refCnt << endl;

	MyDB_PageHandleBase phb;
	phb.bm = this;
	phb.position = &(iterMap->second);
	return make_shared<MyDB_PageHandleBase>(phb);
}

MyDB_PageHandle MyDB_BufferManager ::getPinnedPage()
{
	/*
	get a space from tempfile
	store that page to anonyPageMap
	*/
	clockarmGetSpace(); // we don't call diskToBuffer cuz we don't need to, but we still need a place to store clockArm
	// Page p = {true, "", anonySeq, &(*clockArm), 1};
	// anonyPageMap[anonySeq] = p;

	Page *p = new Page;
	p->isAnony = true;
	p->tablePath = "";
	p->pageNum = anonySeq;
	p->bufferItemPtr = &(*clockArm);
	p->refCnt = 2;
	anonyPageMap[anonySeq] = *p;

	// access buffer, if unpin then pin it
	//@@@如果可pin值還夠的話
	anonyPageMap[anonySeq].bufferItemPtr->isPinned = true;
	anonyPageMap[anonySeq].bufferItemPtr->isAnony = true;
	anonyPageMap[anonySeq].bufferItemPtr->pageNum = anonySeq;

	// make handle
	Page *tempPagePtr = &(anonyPageMap[anonySeq]);
	anonySeq++;
	MyDB_PageHandleBase phb;
	phb.bm = this;
	phb.position = tempPagePtr;
	return make_shared<MyDB_PageHandleBase>(phb);
}

void MyDB_BufferManager ::unpin(MyDB_PageHandle ph)
{
	ph->position->bufferItemPtr->isPinned = false;
}

MyDB_BufferManager ::MyDB_BufferManager(size_t pageSize, size_t numPages, string tempFile)
{
	// declare clockBuffer
	this->numPages = numPages;
	this->pageSize = pageSize;
	clockBuffer.assign(numPages, Page_Buffer_Item(pageSize));
	clockArm = clockBuffer.begin();

	// open a tempfile with name: tempFile for anonymous page
	string tempFilePath = "./" + tempFile;
	fd_tempFile = open(tempFilePath.c_str(), O_CREAT | O_RDWR | O_FSYNC, 0666);
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
	close(fd_tempFile);
}

// when Clock_LRU needs to evict page, store the dirty data to disk
void MyDB_BufferManager ::bufferToDisk(Page_Buffer_Item *bufferItem)
{
	cout << "##bmcc\t\t"
		 << "@@@****@@@<bufferToDisk>@@@****@@@" << endl;
	if (!bufferItem->isAnony)
	{
		// store data on dis loc/tablename
		string diskFilePath = bufferItem->tablePath;
		vector<char> data = bufferItem->pageData;

		// open disk
		int fd_disk = open(diskFilePath.c_str(), O_RDWR | O_FSYNC, 0666);
		if (fd_disk < 0)
		{
			cout << "bufferToDisk Unable to open " << diskFilePath << endl;
			exit(1);
		}

		char *writeByte = &data[0];

		// store back to disk
		lseek(fd_disk, pageSize * (bufferItem->pageNum), SEEK_SET);
		int size = write(fd_disk, writeByte, pageSize);
		cout << "##bmcc\t\t"
			 << "writesize=" << size << endl;
		close(fd_disk);
	}
	else
	{
		cout << "*****\n*****\nbufferToDisk\nanonymous\n*****\n*****\n";
		vector<char> data = bufferItem->pageData;
		char *writeByte = &data[0];
		lseek(fd_tempFile, pageSize * (bufferItem->pageNum), SEEK_SET);
		int size = write(fd_tempFile, writeByte, pageSize);
		cout << size << endl;
	}

	// clean and free to overwrite
	bufferItem->isDirty = false;

	// update map to null
	if (bufferItem->isAnony)
	{
		anonyPageMap[bufferItem->pageNum].bufferItemPtr = nullptr;
	}
	else
	{
		diskPageMap[make_pair(bufferItem->tablePath, bufferItem->pageNum)].bufferItemPtr = nullptr;
	}
}

// non anonymous: load data from table in disk to buffer
Page_Buffer_Item *MyDB_BufferManager ::diskToBuffer(string tablePath, long pageNum)
{
	cout << "##bmcc\t\t"
		 << "------<diskToBuffer>-------" << endl;
	// fd_disk = open(diskFilePath.c_str(), O_CREAT | O_RDWR, 0666);
	int fd_disk = open(tablePath.c_str(), O_RDWR | O_FSYNC, 0666);
	if (fd_disk < 0)
	{
		cout << "diskToBuffer Unable to open " << tablePath << endl;
		exit(1);
	}

	clockarmGetSpace();
	char readByte[pageSize];
	lseek(fd_disk, pageSize * pageNum, SEEK_SET);
	int size = read(fd_disk, readByte, pageSize);

	// assign buffer
	clockArm->tablePath = tablePath;
	clockArm->pageNum = pageNum;
	clockArm->acedBit = true;
	clockArm->pageData.assign(readByte, readByte + pageSize);
	close(fd_disk);

	/* for debug */
	cout << "diskToBuffer" << endl;
	cout << "size:" << size << " pageSize: " << pageSize << endl;

	/*char *buff = readByte;
	buff[size] = '\0';
	cout << buff << endl;
	for (int i = 0; i < pageSize; i++)
	{
		cout << clockArm->pageData[i];
	}*/

	return &(*clockArm);
}
Page_Buffer_Item *MyDB_BufferManager ::tempFileToBuffer(long slot)
{
	cout << "##bmcc\t\t"
		 << "------<tempFileToBuffer>-------" << endl;
	clockarmGetSpace();
	char readByte[pageSize];
	lseek(fd_tempFile, pageSize * slot, SEEK_SET);
	int size = read(fd_tempFile, readByte, pageSize);

	clockArm->pageData.assign(readByte, readByte + pageSize);
	close(fd_tempFile);

	/* for debug */
	cout << "tempFileToBuffer" << endl;
	cout << "size:" << size << " pageSize: " << slot << endl;

	/*char *buff = readByte;
	buff[size] = '\0';
	cout << buff << endl;
	for (int i = 0; i < slot; i++)
	{
		cout << clockArm->pageData[i];
	}*/

	return &(*clockArm);
}

void MyDB_BufferManager ::clockarmGetSpace()
{
	clockArm++;
	// clock arm movement
	while (true)
	{
		// currentPage exceed the max_len of bufferPage
		// reset pointer to 0
		if (clockArm == clockBuffer.end())
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
				cout << "##bmcc\t\t"
					 << "*****clockArmGetSpace***"
					 << "cur char = " << clockArm->pageData[0] << endl
					 << endl;
				bufferToDisk(&(*clockArm));
			}
			return;
		}
		// when arm point to a setted item, unset & move arm to next item slot
		(*clockArm).acedBit = false;
		clockArm++;
	}
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
