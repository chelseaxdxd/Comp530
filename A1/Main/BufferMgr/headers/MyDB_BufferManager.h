#ifndef BUFFER_MGR_H
#define BUFFER_MGR_H

#include "MyDB_PageHandle.h"
#include "MyDB_Table.h"
#include "Page_Buffer_Item.h"
#include <map>	   // for hashmap
#include <set>	   // store repository and table name
#include <utility> //pair

using namespace std;

class MyDB_BufferManager
{

public:
	// THESE METHODS MUST APPEAR AND THE PROTOTYPES CANNOT CHANGE!

	// gets the i^th page in the table whichTable... note that if the page
	// is currently being used (that is, the page is current buffered) a handle
	// to that already-buffered page should be returned
	MyDB_PageHandle getPage(MyDB_TablePtr whichTable, long i);

	// gets a temporary page that will no longer exist (1) after the buffer manager
	// has been destroyed, or (2) there are no more references to it anywhere in the
	// program.  Typically such a temporary page will be used as buffer memory.
	// since it is just a temp page, it is not associated with any particular
	// table
	MyDB_PageHandle getPage();

	// gets the i^th page in the table whichTable... the only difference
	// between this method and getPage (whicTable, i) is that the page will be
	// pinned in RAM; it cannot be written out to the file
	MyDB_PageHandle getPinnedPage(MyDB_TablePtr whichTable, long i);

	// gets a temporary page, like getPage (), except that this one is pinned
	MyDB_PageHandle getPinnedPage();

	// un-pins the specified page
	void unpin(MyDB_PageHandle ph);

	// creates an LRU buffer manager... params are as follows:
	// 1) the size of each page is pageSize
	// 2) the number of pages managed by the buffer manager is numPages;
	// 3) temporary pages are written to the file tempFile
	MyDB_BufferManager(size_t pageSize, size_t numPages, string tempFile);

	// when the buffer manager is destroyed, all of the dirty pages need to be
	// written back to disk, any necessary data needs to be written to the catalog,
	// and any temporary files need to be deleted
	~MyDB_BufferManager();

	// FEEL FREE TO ADD ADDITIONAL PUBLIC METHODS
	friend Page_Buffer_Item *MyDB_PageHandleBase::reloadFromDisk(string tablePath, long pageNum);
	friend Page_Buffer_Item *MyDB_PageHandleBase::reloadTempFile(long slot);
	// friend void destructBufferItem(Page p);

	friend class MyDB_BufferManager;

private:
	/*可能需要*/

	/*ClockBuffer*/

	// number of pages managed by the buffer manager
	size_t pageSize;
	long numPages;

	vector<Page_Buffer_Item> clockBuffer;

	// point to current page buffer item
	vector<Page_Buffer_Item>::iterator clockArm;

	/* IO */
	set<string> tableLocations;
	set<string> tableNames; // 有需要時可以改成map存 table_fd
	int fd_tempFile;
	long anonySeq = 0;

	// anonymous hash map
	map<long, Page> anonyPageMap; // pair<table_name, page_num>

	// non anonlymous hash map
	map<pair<string, long>, Page> diskPageMap;

	// when Clock_LRU needs to evict page, store the dirty data to disk
	void bufferToDisk(Page_Buffer_Item *bufferItem);

	// non anonymous: load data from table in disk to buffer
	Page_Buffer_Item *diskToBuffer(string tablePath, long pageNum);

	// anonymous: load data from tempfile in disk to buffer
	Page_Buffer_Item *tempFileToBuffer(long slot);

	void clockarmGetSpace();
};

#endif
