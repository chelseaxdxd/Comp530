
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"
#include "MyDB_TableReaderWriter.h"

using namespace std;

MyDB_TableReaderWriter :: MyDB_TableReaderWriter (MyDB_TablePtr forMeIn, MyDB_BufferManagerPtr myBufferIn) {
	forMe = forMeIn;
	myBuffer = myBufferIn;

	if (forMe->lastPage () == -1) {
		forMe->setLastPage (0);
		lastPage = make_shared <MyDB_PageReaderWriter> (*this, forMe->lastPage ());
		lastPage->clear ();
	} else {
		lastPage = make_shared <MyDB_PageReaderWriter> (*this, forMe->lastPage ());	
	}
}

MyDB_RecordPtr MyDB_TableReaderWriter :: getEmptyRecord () {

	// use the schema to produce an empty record
	return make_shared <MyDB_Record> (forMe->getSchema ());
}


void MyDB_TableReaderWriter :: append (MyDB_RecordPtr appendMe) {

	// try to append the record on the current page...
	if (!lastPage->append (appendMe)) {

		// if we cannot, then get a new last page and append
		forMe->setLastPage (forMe->lastPage () + 1);
		lastPage = make_shared <MyDB_PageReaderWriter> (*this, forMe->lastPage ());
		lastPage->clear ();
		lastPage->append (appendMe);
	}
}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter :: getIterator (MyDB_RecordPtr iterateIntoMe) {
	return make_shared <MyDB_TableRecIterator> (*this, forMe, iterateIntoMe);
}

void MyDB_TableReaderWriter :: loadFromTextFile (string fName) {

	// empty out the database file
	forMe->setLastPage (0);
	lastPage = make_shared <MyDB_PageReaderWriter> (*this, forMe->lastPage ());
	lastPage->clear ();

	// try to open the file
	string line;
	ifstream myfile (fName);

	// if we opened it, read the contents
	MyDB_RecordPtr tempRec = getEmptyRecord ();
	if (myfile.is_open()) {

		// loop through all of the lines
		while (getline (myfile,line)) {
			tempRec->fromString (line);		
			append (tempRec);
		}
		myfile.close ();
	}
}

void MyDB_TableReaderWriter :: writeIntoTextFile (string fName) {
	
	// open up the output file
	ofstream output;
	output.open (fName);

	// get an empty record
	MyDB_RecordPtr tempRec = getEmptyRecord ();;		

	// and write out all of the records
	MyDB_RecordIteratorPtr myIter = getIterator (tempRec);
	while (myIter->hasNext ()) {
		myIter->getNext ();
	}
	output.close ();
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter :: operator [] (size_t i) {
	
	// see if we are going off of the end of the file... if so, then clear those pages
	while (i > forMe->lastPage ()) {
		forMe->setLastPage (forMe->lastPage () + 1);
		lastPage = make_shared <MyDB_PageReaderWriter> (*this, forMe->lastPage ());
		lastPage->clear ();	
	}

	// now get the page
	arrayAccessBuffer = make_shared <MyDB_PageReaderWriter> (*this, i);
	return *arrayAccessBuffer;
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter :: last () {
	arrayAccessBuffer = make_shared <MyDB_PageReaderWriter> (*this, forMe->lastPage ());
	return *arrayAccessBuffer;
}

int MyDB_TableReaderWriter :: getNumPages () {
	return forMe->lastPage () + 1;
}


MyDB_BufferManagerPtr MyDB_TableReaderWriter :: getBufferMgr () {
	return myBuffer;
}

/*
	// gets the physical file for this guy
	string getFileName ();
*/
	
MyDB_TablePtr MyDB_TableReaderWriter :: getTable () {
	return forMe;
}


#endif
