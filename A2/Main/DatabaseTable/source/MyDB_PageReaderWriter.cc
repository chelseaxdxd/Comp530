
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"

// #define PAGE_TYPE *((MyDB_PageType *)((char *)myPage->getBytes()))
#define NUM_BYTES_USED *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)))
#define NUM_BYTES_LEFT (pageSize - NUM_BYTES_USED)

MyDB_PageReaderWriter ::MyDB_PageReaderWriter(MyDB_TableReaderWriter &parent, int whichPage)
{

	// get the actual page
	myPage = parent.getBufferMgr()->getPage(parent.getTable(), whichPage);
	pageSize = parent.getBufferMgr()->getPageSize();

	//
	bytesUsed = 2 * sizeof(size_t);
}

void MyDB_PageReaderWriter ::clear()
{
	NUM_BYTES_USED = 2 * sizeof(size_t);
	// bytesUsed = 2 * sizeof(size_t);
	// PAGE_TYPE = MyDB_PageType ::RegularPage;
	myPage->wroteBytes();
}

MyDB_PageType MyDB_PageReaderWriter ::getType()
{
	// return PAGE_TYPE;
	return MyDB_PageType ::RegularPage;
}

/*
MyDB_RecordIteratorAltPtr MyDB_PageReaderWriter :: getIteratorAlt () {
	return make_shared <MyDB_PageRecIteratorAlt> (myPage);
}

MyDB_RecordIteratorAltPtr getIteratorAlt (vector <MyDB_PageReaderWriter> &forUs) {
	return make_shared <MyDB_PageListIteratorAlt> (forUs);
}
*/

MyDB_RecordIteratorPtr MyDB_PageReaderWriter ::getIterator(MyDB_RecordPtr iterateIntoMe)
{
	return make_shared<MyDB_PageRecIterator>(myPage, iterateIntoMe);
}

void MyDB_PageReaderWriter ::setType(MyDB_PageType toMe)
{
	// PAGE_TYPE = toMe;
	// myPage->wroteBytes();
}

bool MyDB_PageReaderWriter ::append(MyDB_RecordPtr appendMe)
{
	size_t recSize = appendMe->getBinarySize();
	// size_t bytesUsed = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));

	if (recSize > NUM_BYTES_LEFT)
	// if (recSize > (pageSize - bytesUsed))
	{
		return false;
	}

	// write at the end
	void *address = myPage->getBytes();
	appendMe->toBinary(NUM_BYTES_USED + (char *)address);
	// appendMe->toBinary(bytesUsed + (char *)address);
	NUM_BYTES_USED += recSize;
	// bytesUsed += recSize;
	myPage->wroteBytes();
	return true;
}

// 這個老師新建的func，應該還沒用到
// void *MyDB_PageReaderWriter ::appendAndReturnLocation(MyDB_RecordPtr appendMe)
// {
// 	// void *recLocation = NUM_BYTES_USED + (char *)myPage->getBytes();
// 	void *recLocation = (char *)myPage->getBytes() + bytesUsed;
// 	if (append(appendMe))
// 		return recLocation;
// 	else
// 		return nullptr;
// }

size_t MyDB_PageReaderWriter ::getPageSize()
{
	return pageSize;
}

void *MyDB_PageReaderWriter ::getBytes()
{
	return myPage->getBytes();
}

#endif
