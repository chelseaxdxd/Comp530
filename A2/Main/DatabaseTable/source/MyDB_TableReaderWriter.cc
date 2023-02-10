
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include <queue>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"
#include "MyDB_TableRecIterator.h"

using namespace std;

MyDB_TableReaderWriter ::MyDB_TableReaderWriter(MyDB_TablePtr forMeIn, MyDB_BufferManagerPtr myBufferIn)
{
	forMe = forMeIn;
	myBuffer = myBufferIn;

	// if lastPage has never been written, set the last page
	if (forMe->lastPage() == -1)
		forMe->setLastPage(0);
	lastPage = make_shared<MyDB_PageReaderWriter>(*this, forMe->lastPage());
}

MyDB_RecordPtr MyDB_TableReaderWriter ::getEmptyRecord()
{
	// create emptyRecord by schema setted
	MyDB_RecordPtr rtn = make_shared<MyDB_Record>(forMe->getSchema());
	return rtn;
}

void MyDB_TableReaderWriter ::append(MyDB_RecordPtr appendMe)
{
	// append record to last page
	if (!lastPage->append(appendMe))
	{
		// if no enough space in lastPage
		// create a new lastPage
		forMe->setLastPage(forMe->lastPage() + 1);
		lastPage = make_shared<MyDB_PageReaderWriter>(*this, forMe->lastPage());
		lastPage->clear();
		lastPage->append(appendMe);
	}
}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter ::getIterator(MyDB_RecordPtr iterateIntoMe)
{
	MyDB_RecordIteratorPtr rtn = make_shared<MyDB_TableRecIterator>(*this, forMe, iterateIntoMe);
	return rtn;
}

void MyDB_TableReaderWriter ::loadFromTextFile(string fromMe)
{
	// clear the pages
	forMe->setLastPage(0);
	lastPage = make_shared<MyDB_PageReaderWriter>(*this, forMe->lastPage());
	lastPage->clear();

	// load the data
	string line;
	ifstream f(fromMe);
	MyDB_RecordPtr temp = getEmptyRecord();
	if (f)
	{
		while (getline(f, line))
		{
			temp->fromString(line);
			append(temp);
		}
		f.close();
	}
}

void MyDB_TableReaderWriter ::writeIntoTextFile(string toMe)
{
	ofstream f;
	f.open(toMe);
	MyDB_RecordPtr temp = getEmptyRecord();
	MyDB_RecordIteratorPtr ptr = getIterator(temp);
	while (ptr->hasNext())
	{
		ptr->getNext();
	}
	f.close();
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter ::operator[](size_t i)
{
	// check if i in range
	if (i > forMe->lastPage())
	{
		exit(1);
	}
	else
	{
	// if i is valid, return
	currPage = make_shared<MyDB_PageReaderWriter>(*this, i);
	return *currPage;
	}
}

MyDB_PageReaderWriter &MyDB_TableReaderWriter ::last()
{
	currPage = make_shared<MyDB_PageReaderWriter>(*this, forMe->lastPage());
	return *currPage;
}

MyDB_TablePtr MyDB_TableReaderWriter ::getTable()
{
	return forMe;
}

MyDB_BufferManagerPtr MyDB_TableReaderWriter ::getBufferMgr()
{
	return myBuffer;
}

int MyDB_TableReaderWriter ::getNumPages()
{
	return (forMe->lastPage() + 1);
}

#endif
