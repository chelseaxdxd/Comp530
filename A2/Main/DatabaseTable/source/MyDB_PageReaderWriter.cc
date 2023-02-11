#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"


void MyDB_PageReaderWriter ::clear()
{
	// we only overwrite it so we do not need to clean it up, we only need to reset numBytesUsed
	size_t &numBytesUsed = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));
	numBytesUsed = 2 * sizeof(size_t); 
	myPage->wroteBytes();
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter ::getIterator(MyDB_RecordPtr iterateIntoMe)
{
	MyDB_RecordIteratorPtr rtn = make_shared<MyDB_PageRecIterator>(myPage, iterateIntoMe);
	return rtn;
}

bool MyDB_PageReaderWriter ::append(MyDB_RecordPtr appendMe)
{
	size_t size = appendMe->getBinarySize();
	size_t &numBytesUsed = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));
	if (size > (pageSize - numBytesUsed))
	{
		return false;
	}
	else
	{
		appendMe->toBinary(((char *) myPage->getBytes()) + numBytesUsed);
		numBytesUsed += size;
		myPage->wroteBytes();
		return true;
	}
}

MyDB_PageType MyDB_PageReaderWriter ::getType()
{
	return MyDB_PageType ::RegularPage;
}

void MyDB_PageReaderWriter ::setType(MyDB_PageType toMe) {}

MyDB_PageReaderWriter ::MyDB_PageReaderWriter(MyDB_TableReaderWriter &myTableRW, int pageNum)
{

	myPage = myTableRW.getBufferMgr()->getPage(myTableRW.getTable(), pageNum);
	pageSize = myTableRW.getBufferMgr()->getPageSize();
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
