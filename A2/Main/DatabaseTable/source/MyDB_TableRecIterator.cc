#ifndef TABLE_REC_ITER_C
#define TABLE_REC_ITER_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"

void MyDB_TableRecIterator::getNext()
{
	tableIter->getNext();
}

// return true iff there is another record in the file/page
bool MyDB_TableRecIterator::hasNext()
{
	if (tableIter->hasNext())
		return true;

	if (curPage == tablePtr->lastPage())
		return false;

	curPage++;
	tableIter = (*tableReaderWriterPtr)[curPage].getIterator(recPtr);
	return hasNext();
}

// destructor and contructor
MyDB_TableRecIterator ::MyDB_TableRecIterator(MyDB_TableReaderWriter *tableReaderWriterPtrIn, MyDB_TablePtr tablePtrIn,
											  MyDB_RecordPtr recPtrIn)
{
	tableReaderWriterPtr = tableReaderWriterPtrIn;
	tablePtr = tablePtrIn;
	recPtr = recPtrIn;
	curPage = 0;
	tableIter = (*tableReaderWriterPtr)[curPage].getIterator(recPtr);
}
MyDB_TableRecIterator::~MyDB_TableRecIterator(){

};

#endif