#ifndef TABLE_REC_ITER_C
#define TABLE_REC_ITER_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"

void MyDB_TableRecIterator::getNext()
{
	if (hasNext())
		myRecIter->getNext();
}

// return true if there is another record in the file/page
bool MyDB_TableRecIterator::hasNext()
{
	if (myRecIter->hasNext())
		return true;

	if (currPageNum == tablePtr->lastPage())
		return false;

	currPageNum++;
	myRecIter = myTableRW[currPageNum].getIterator(recPtr);
	return hasNext();
}

// destructor and contructor
MyDB_TableRecIterator ::MyDB_TableRecIterator(MyDB_TableReaderWriter &myTableRWIn, MyDB_TablePtr tablePtrIn, MyDB_RecordPtr recPtrIn) : myTableRW(myTableRWIn)
{
	tablePtr = tablePtrIn;
	recPtr = recPtrIn;
	currPageNum = 0;
	myRecIter = myTableRW[currPageNum].getIterator(recPtr);
}
MyDB_TableRecIterator::~MyDB_TableRecIterator(){

};

#endif
