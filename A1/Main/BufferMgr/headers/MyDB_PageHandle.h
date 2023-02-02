
#ifndef PAGE_HANDLE_H
#define PAGE_HANDLE_H

#include <memory> // for shared_ptr
#include "Page_Buffer_Item.h"

class MyDB_BufferManager;
class MyDB_PageHandleBase;

// page handles are basically smart pointers
using namespace std;
typedef shared_ptr<MyDB_PageHandleBase> MyDB_PageHandle;

struct Page
{
	bool isAnony;
	string tablePath;
	long pageNum;
	Page_Buffer_Item *bufferItemPtr;
	int refCnt;
};

class MyDB_PageHandleBase
{

public:
	/* point to page location*/
	Page *position;

	MyDB_BufferManager *bm = nullptr;

	// THESE METHODS MUST BE IMPLEMENTED WITHOUT CHANGING THE DEFINITION

	// access the raw bytes in this page... if the page is not currently
	// in the buffer, then the contents of the page are loaded from
	// secondary storage.
	void *getBytes();

	// let the page know that we have written to the bytes.  Must always
	// be called once the page's bytes have been written.  If this is not
	// called, then the page will never be marked as dirty, and the page
	// will never be written to disk.
	void wroteBytes();

	Page_Buffer_Item *reloadFromDisk(string tablePath, long pageNum);
	Page_Buffer_Item *reloadTempFile(long slot);
	// There are no more references to the handle when this is called...
	// this should decrmeent a reference count to the number of handles
	// to the particular page that it references.  If the number of
	// references to a pinned page goes down to zero, then the page should
	// become unpinned.

	~MyDB_PageHandleBase();

	// FEEL FREE TO ADD ADDITIONAL PUBLIC METHODS

private:
	// YOUR CODE HERE
};

#endif
