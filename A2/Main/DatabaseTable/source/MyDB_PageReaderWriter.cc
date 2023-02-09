
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"

#define NUM_BYTES_USED *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)))
#define NUM_BYTES_LEFT (pageSize - NUM_BYTES_USED)

void MyDB_PageReaderWriter ::clear()
{
	NUM_BYTES_USED = 2 * sizeof(size_t);
	// bytesUsed = 2 * sizeof(size_t);
	myPage->wroteBytes();
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter ::getIterator(MyDB_RecordPtr iterateIntoMe)
{
	return make_shared<MyDB_PageRecIterator>(myPage, iterateIntoMe);
}

bool MyDB_PageReaderWriter ::append(MyDB_RecordPtr appendMe)
{
	size_t size = appendMe->getBinarySize();
	// bytesUsed = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));
	// size_t bytesLeft = pageSize - bytesUsed;

	if (size > NUM_BYTES_LEFT)
	// if (size > bytesLeft)
	{
		return false;
	}

	void *address = myPage->getBytes();
	appendMe->toBinary(NUM_BYTES_USED + (char *)address);
	// appendMe->toBinary(bytesUsed + (char *)address);
	NUM_BYTES_USED += size;
	// bytesUsed += size;
	myPage->wroteBytes();
	return true;
}

MyDB_PageType MyDB_PageReaderWriter ::getType()
{
	return MyDB_PageType ::RegularPage;
}

void MyDB_PageReaderWriter ::setType(MyDB_PageType toMe){}

MyDB_PageReaderWriter ::MyDB_PageReaderWriter(MyDB_TableReaderWriter &myTableRW, int pageNum)
{

	// get the actual page
	myPage = myTableRW.getBufferMgr()->getPage(myTableRW.getTable(), pageNum);
	pageSize = myTableRW.getBufferMgr()->getPageSize();

	//
	// bytesUsed = 2 * sizeof(size_t);
}

size_t MyDB_PageReaderWriter ::getPageSize()
{
	return pageSize;
}

void *MyDB_PageReaderWriter ::getBytes()
{
	return myPage->getBytes();
}

#endif
