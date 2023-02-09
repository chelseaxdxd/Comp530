#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C
#include "MyDB_PageRecIterator.h"
// #include "MyDB_PageType.h"

// #define NUM_BYTES_USED *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)))

void MyDB_PageRecIterator::getNext()
{
	void *pos = bytesConsumed + (char *)myPage->getBytes();
	void *nextPos = myRec->fromBinary(pos);
	bytesConsumed += ((char *)nextPos) - ((char *)pos);
}

// return true iff there is another record in the file/page
bool MyDB_PageRecIterator::hasNext()
{
	size_t bytesUsed = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));
	return bytesConsumed != bytesUsed;
	// return bytesConsumed != NUM_BYTES_USED;
}

// destructor and contructor
MyDB_PageRecIterator ::MyDB_PageRecIterator(MyDB_PageHandle myPageIn, MyDB_RecordPtr myRecIn)
{
	bytesConsumed = sizeof(size_t) * 2;
	myPage = myPageIn;
	myRec = myRecIn;
}
MyDB_PageRecIterator::~MyDB_PageRecIterator(){

};

#endif