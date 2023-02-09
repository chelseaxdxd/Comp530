#ifndef TABLE_REC_ITER_C
#define TABLE_REC_ITER_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"

void MyDB_TableRecIterator::getNext()
{
	myIter->getNext ();
}

// return true iff there is another record in the file/page
bool MyDB_TableRecIterator::hasNext()
{
	if (myIter->hasNext())
		return true;

	if (curPage == myTable->lastPage ())
		return false;

	curPage++;
	// myIter = myParent[curPage].getIterator (myRec);
	myIter = myTableRW[curPage].getIterator(myRec);
	// myIter = (*myTableRW)[curPage].getIterator(myRec);
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
	curPage = 0;
	// myIter = myParent[curPage].getIterator (myRec);
	myIter = myTableRW[curPage].getIterator(myRec);
	// myIter = (*myTableRW)[curPage].getIterator(myRec);
}
MyDB_TableRecIterator::~MyDB_TableRecIterator(){

};

#endif