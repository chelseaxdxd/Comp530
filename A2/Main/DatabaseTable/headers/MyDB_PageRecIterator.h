#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

class MyDB_PageRecordIterator : public MyDB_RecordIterator
{
public:
    // put the contents of the next record in the file/page into the iterator record
    // this should be called BEFORE the iterator record is first examined
    void getNext(); // virtual void getNext () = 0;

    // return true iff there is another record in the file/page
    bool hasNext();

    // destructor and contructor
    // MyDB_PageRecordIterator(){}; maybe no need constructor cuz it's not a virtual one in base class
    ~MyDB_PageRecordIterator();
}

#endif
