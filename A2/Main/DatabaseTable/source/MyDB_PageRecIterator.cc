#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C
#include "MyDB_PageRecIterator.h"

void MyDB_PageRecIterator::getNext()
{
	if (hasNext())
	{
		void *pos = (char *)pageHandle->getBytes() + totalBytes;
		void *nextPos = recPtr->fromBinary(pos);
		totalBytes += (((char *)nextPos) - ((char *)pos));
	}
	
}

bool MyDB_PageRecIterator::hasNext()
{
	size_t numBytesUsed = *((size_t *)(((char *)pageHandle->getBytes()) + sizeof(size_t)));
	return totalBytes < numBytesUsed;
	// |type|bu|
	//        b
	// |type| 4 |   |   |
	// 0    1   2   3   4->bytesConsumed
}

MyDB_PageRecIterator ::MyDB_PageRecIterator(MyDB_PageHandle pageHandleIn, MyDB_RecordPtr recPtrIn)
{
	totalBytes = sizeof(size_t) * 2;
	pageHandle = pageHandleIn;
	recPtr = recPtrIn;
}
MyDB_PageRecIterator::~MyDB_PageRecIterator(){

};

#endif
