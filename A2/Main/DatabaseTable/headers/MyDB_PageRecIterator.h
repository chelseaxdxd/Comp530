#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include "MyDB_PageHandle.h"
#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"

class MyDB_PageRecIterator : public MyDB_RecordIterator
{
public:
    // put the contents of the next record in the file/page into the iterator record
    // this should be called BEFORE the iterator record is first examined
    void getNext() override;

    // return true iff there is another record in the file/page
    bool hasNext() override;

    // destructor and contructor
    MyDB_PageRecIterator(MyDB_PageHandle myPageIn, MyDB_RecordPtr myRecIn);
    ~MyDB_PageRecIterator();

private:
    MyDB_PageHandle pageHandle;
    MyDB_RecordPtr recPtr;
    int totalBytes;
};

#endif