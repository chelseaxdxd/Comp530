
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include <queue>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"
#include "MyDB_TableReaderWriter.h"

using namespace std;

MyDB_TableReaderWriter ::MyDB_TableReaderWriter(MyDB_TablePtr tablePtrIn, MyDB_BufferManagerPtr bmPtrIn)
{
	tablePtr = tablePtrIn;
	bmPtr = bmPtrIn;

	// assigning last page for this table reader writer
	if (tablePtr->lastPage() == -1) // first time call the table, table last = -1
	{
		tablePtr->setLastPage(0); // make this table last page index = o

		lastPage = make_shared<MyDB_PageReaderWriter>(*this, tablePtr->lastPage());
		//@@@@lastPage->clear();
	}
	else // reassigning last page
	{
		lastPage = make_shared<MyDB_PageReaderWriter>(*this, tablePtr->lastPage());
	}
}

MyDB_RecordPtr MyDB_TableReaderWriter ::getEmptyRecord()
{
	// cout<<endl<<2<<endl;
	//  use the schema to produce an empty record
	return make_shared<MyDB_Record>(tablePtr->getSchema());
}

void MyDB_TableReaderWriter ::append(MyDB_RecordPtr appendMe)
{
	// cout<<endl<<3<<endl;
	//  try to append the record on the current page...
	if (!lastPage->append(appendMe))
	{

		// if we cannot, then get a new last page and append
		tablePtr->setLastPage(tablePtr->lastPage() + 1);
		lastPage = make_shared<MyDB_PageReaderWriter>(*this, tablePtr->lastPage());
		lastPage->clear();
		lastPage->append(appendMe);
	}
}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter ::getIterator(MyDB_RecordPtr iterateIntoMe)
{
	// cout<<endl<<4<<endl;
	return make_shared<MyDB_TableRecIterator>(this, tablePtr, iterateIntoMe);
}

void MyDB_TableReaderWriter ::loadFromTextFile(string fName)
{
	// cout<<endl<<5<<endl;

	//  empty out the table, and cover the table with data from text file
	tablePtr->setLastPage(0);
	lastPage = make_shared<MyDB_PageReaderWriter>(*this, tablePtr->lastPage());
	lastPage->clear(); // need to clear cuz it may has previous data

	// open text file and read it
	ifstream ifs(fName);
	string line;
	MyDB_RecordPtr tempRec = getEmptyRecord();

	// loop through all of the lines
	while (getline(ifs, line))
	{
		tempRec->fromString(line); // store the line into the record(record::values)
		append(tempRec);
	}
	ifs.close();
}

void MyDB_TableReaderWriter ::writeIntoTextFile(string fName)
{
	// cout<<endl<<6<<endl;
	//  open up the output file
	ofstream output;
	output.open(fName);

	// get an empty record
	MyDB_RecordPtr tempRec = getEmptyRecord();
	;

	// and write out all of the records
	MyDB_RecordIteratorPtr myIter = getIterator(tempRec);
	while (myIter->hasNext())
	{
		myIter->getNext();
	}
	output.close();
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter ::operator[](size_t i)
{
	// cout<<endl<<7<<endl;
	//  see if we are going off of the end of the file... if so, then clear those pages
	while (i > tablePtr->lastPage())
	{
		tablePtr->setLastPage(tablePtr->lastPage() + 1);
		lastPage = make_shared<MyDB_PageReaderWriter>(*this, tablePtr->lastPage());
		lastPage->clear();
	}

	// now get the page
	arrayAccessBuffer = make_shared<MyDB_PageReaderWriter>(*this, i);
	return *arrayAccessBuffer;
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter ::last()
{
	// cout<<endl<<8<<endl;
	arrayAccessBuffer = make_shared<MyDB_PageReaderWriter>(*this, tablePtr->lastPage());
	return *arrayAccessBuffer;
}

int MyDB_TableReaderWriter ::getNumPages()
{
	// cout<<endl<<9<<endl;
	return tablePtr->lastPage() + 1;
}

MyDB_BufferManagerPtr MyDB_TableReaderWriter ::getBufferMgr()
{
	// cout<<endl<<10<<endl;
	return bmPtr;
}

/*
	// gets the physical file for this guy
	string getFileName ();
*/

MyDB_TablePtr MyDB_TableReaderWriter ::getTable()
{
	// cout<<endl<<11<<endl;
	return tablePtr;
}

#endif
