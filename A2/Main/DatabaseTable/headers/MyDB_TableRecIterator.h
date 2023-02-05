#ifndef TABLE_REC_ITER_H
#define TABLE_REC_ITER_H

class MyDB_TableRecordIterator : public MyDB_RecordIterator
{
public:
    // put the contents of the next record in the file/page into the iterator record
    // this should be called BEFORE the iterator record is first examined
    void getNext(); // virtual void getNext () = 0;

    // return true iff there is another record in the file/page
    bool hasNext();

    // destructor and contructor
    ~MyDB_TableRecordIterator();
}

#endif