#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C
#include "MyDB_PageRecIterator.h"
#include "MyDB_PageType.h"

void MyDB_PageRecIterator::getNext()
{
	void *pos = bytesConsumed + (char *)pageHandle->getBytes();
	void *nextPos = recPtr->fromBinary(pos);
	bytesConsumed += ((char *)nextPos) - ((char *)pos);
}

// return true iff there is another record in the file/page
bool MyDB_PageRecIterator::hasNext()
{
	size_t numBytesUsed = *((size_t *)(((char *)pageHandle->getBytes()) + sizeof(size_t)));
	return bytesConsumed != numBytesUsed;
	// |type|nbu|
	//        b
	// |type| 4 |   |   |
	// 0    1   2   3   4->bytesConsumed
}

// destructor and contructor
MyDB_PageRecIterator ::MyDB_PageRecIterator(MyDB_PageHandle pageHandleIn, MyDB_RecordPtr recPtrIn)
{
	bytesConsumed = sizeof(size_t) * 2; // initialize to front(first size_t is pagetype, seconde is numBytesUsed)
	pageHandle = pageHandleIn;
	recPtr = recPtrIn;
}
MyDB_PageRecIterator::~MyDB_PageRecIterator(){

};
// |  |nbu|
//        b
// |  |4  |   |   |
//                b

#endif