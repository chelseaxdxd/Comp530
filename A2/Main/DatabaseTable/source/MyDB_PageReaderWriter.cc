#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"

#define BYTES_USED *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)))

void MyDB_PageReaderWriter ::clear()
{
	BYTES_USED = 2 * sizeof(size_t);
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

	// if (size > (pageSize - bytesUsed))
	if (size > (pageSize - BYTES_USED))
	{
		return false;
	}
	else
	{
		void *addr = myPage->getBytes();
		appendMe->toBinary((char *)addr + BYTES_USED);
		BYTES_USED += size;
		// appendMe->toBinary((char *)addr + bytesUsed);
		// bytesUsed += size;
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
