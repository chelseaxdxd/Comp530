#ifndef TABLE_REC_ITER_C
#define TABLE_REC_ITER_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"

void MyDB_TableRecIterator::getNext()
{
	myRecIter->getNext();
}

// return true iff there is another record in the file/page
bool MyDB_TableRecIterator::hasNext()
{
	if (myRecIter->hasNext())
		return true;

	if (currPageNum == myTable->lastPage())
		return false;

	currPageNum++;
	// myRecIter = myParent[currPageNum].getIterator (myRec);
	myRecIter = myTableRW[currPageNum].getIterator(myRec);
	// myRecIter = (*myTableRW)[currPageNum].getIterator(myRec);
	return hasNext ();
}

// destructor and contructor
// MyDB_TableRecIterator :: MyDB_TableRecIterator (MyDB_TableReaderWriter &myParent, MyDB_TablePtr myTableIn,
MyDB_TableRecIterator :: MyDB_TableRecIterator (MyDB_TableReaderWriter &myTableRWIn, MyDB_TablePtr myTableIn,
// MyDB_TableRecIterator :: MyDB_TableRecIterator (MyDB_TableReaderWriter * myTableRWIn, MyDB_TablePtr myTableIn,
	// MyDB_RecordPtr myRecIn): myParent(myParent){
	MyDB_RecordPtr myRecIn): myTableRW(myTableRWIn){
	// myTableRW = myTableRWIn; // ++
	myTable = myTableIn;
	myRec = myRecIn;
	currPageNum = 0;
	// myRecIter = myParent[currPageNum].getIterator (myRec);
	myRecIter = myTableRW[currPageNum].getIterator(myRec);
	// myRecIter = (*myTableRW)[currPageNum].getIterator(myRec);
}
MyDB_TableRecIterator::~MyDB_TableRecIterator(){

};

#endif