#ifndef TABLE_REC_ITER_H
#define TABLE_REC_ITER_H

#include "MyDB_RecordIterator.h"
#include "MyDB_Record.h"
#include "MyDB_TableReaderWriter.h"
#include "MyDB_Table.h"

class MyDB_TableRecIterator : public MyDB_RecordIterator
{
public:
    // put the contents of the next record in the file/page into the iterator record
    // this should be called BEFORE the iterator record is first examined
    void getNext() override;
    ; // virtual void getNext () = 0;

    // return true iff there is another record in the file/page
    bool hasNext() override;
    ;

    // destructor and contructor
    MyDB_TableRecIterator(MyDB_TableReaderWriter * tableReaderWriterPtrIn, MyDB_TablePtr tablePtrIn, MyDB_RecordPtr recPtrIn);
    ~MyDB_TableRecIterator();

private:
    MyDB_RecordIteratorPtr tableIter;
    int curPage;

    MyDB_TableReaderWriter * tableReaderWriterPtr;
    MyDB_TablePtr tablePtr;
    MyDB_RecordPtr recPtr;
};

#endif