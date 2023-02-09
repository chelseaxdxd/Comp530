#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C
#include "MyDB_PageRecIterator.h"

void MyDB_PageRecIterator::getNext()
{
	void *pos = (char *)myPage->getBytes() + bytesUsed;
	void *nextPos = myRec->fromBinary(pos);
	bytesUsed += (((char *)nextPos) - ((char *)pos));
}

bool MyDB_PageRecIterator::hasNext()
{
	size_t bytes = *((size_t *)(((char *)myPage->getBytes()) + sizeof(size_t)));
	return bytesUsed != bytes;
}

MyDB_PageRecIterator ::MyDB_PageRecIterator(MyDB_PageHandle myPageIn, MyDB_RecordPtr myRecIn)
{
	bytesUsed = sizeof(size_t) * 2;
	myPage = myPageIn;
	myRec = myRecIn;
}
MyDB_PageRecIterator::~MyDB_PageRecIterator(){

};

#endif