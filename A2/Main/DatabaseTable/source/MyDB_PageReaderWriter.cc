
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"

#define PAGE_TYPE *((MyDB_PageType *)((char *)myPage->getBytes()))

MyDB_PageReaderWriter ::MyDB_PageReaderWriter(MyDB_TableReaderWriter &parent, int whichPage)
{
	// get the actual page
	myPage = parent.getBufferMgr()->getPage(parent.getTable(), whichPage);
	pageSize = parent.getBufferMgr()->getPageSize();
}

void MyDB_PageReaderWriter ::clear()
{
	// we only overwrite it so we do not need to clean it up, we only need to reset NUM_BYTES_USED
	size_t numBytesUsed = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));
	numBytesUsed = 2 * sizeof(size_t); //????

	// reset pagetype
	PAGE_TYPE = MyDB_PageType ::RegularPage;

	// since we clear out this page, we need to wrote back to disk
	myPage->wroteBytes();
}

MyDB_PageType MyDB_PageReaderWriter ::getType()
{
	return PAGE_TYPE; // MyDB_PageType :: RegularPage;
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
	PAGE_TYPE = toMe;
	myPage->wroteBytes();
}

bool MyDB_PageReaderWriter ::append(MyDB_RecordPtr appendMe)
{
	size_t numBytesUsed = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));
	size_t numBytesLeft = (pageSize - numBytesUsed);

	size_t recSize = appendMe->getBinarySize();
	if (recSize > numBytesLeft)
		return false;

	// write at the end
	void *address = myPage->getBytes();
	appendMe->toBinary(numBytesUsed + (char *)address);
	numBytesUsed += recSize;
	myPage->wroteBytes();
	return true;
}

void *MyDB_PageReaderWriter ::appendAndReturnLocation(MyDB_RecordPtr appendMe)
{
	size_t numBytesUsed = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));
	void *recLocation = numBytesUsed + (char *)myPage->getBytes();
	if (append(appendMe))
		return recLocation;
	else
		return nullptr;
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
