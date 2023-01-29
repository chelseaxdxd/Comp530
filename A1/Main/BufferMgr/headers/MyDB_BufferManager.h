#ifndef BUFFER_MGR_H
#define BUFFER_MGR_H

#include "MyDB_PageHandle.h"
#include "MyDB_Table.h"
// #include "Page_Map_Item.h" //寫了struct
#include "Page_Buffer_Item.h"
// #include "Clock_LRU.h" //這部分放進buffer manager
#include <map>	   // for hashmap
#include <utility> //pair

using namespace std;
struct Page_Map_Item
{
	long pageNum;
	Page_Buffer_Item *bufferItemPtr = NULL;
	int refCnt = 0;
};

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
	void unpin(MyDB_PageHandle unpinMe);

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
	friend Page_Buffer_Item *reloadBufferItem(MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony);

private:
		/* IO */
	int fd_tempFile;

	// anonymous hash map
	map<pair<string, long>, Page_Map_Item> anonyPageMap; // pair<table_name, page_num>

	// non anonlymous hash map
	map<pair<string, long>, Page_Map_Item> diskPageMap;

	/*ClockBuffer*/
	long numPages; // number of pages managed by the buffer manager is numPages

	vector<Page_Buffer_Item> clockBuffer; // buffPagePoo -> clockBuffer

	vector<Page_Buffer_Item>::iterator clockArm; // clock arm currently points to // currBuffPageIdx -> curClockIdx

	// name updateData -> updateBufferItem
	void updateBufferItem(*Page_Buffer_Item buffItemPtr, long pageNum, bool isPinned, bool isDirty, bool isAnony, bool acedBit);

	// Update pageData
	void updatePagedata(*Page_Buffer_Item buffItemPtr, vector<char> newPageData);

	// when Clock_LRU needs to evict page, store the dirty data to disk
	void bufferToDisk(long ItemSlotIdx, MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony);

	// load data from disk to buffer
	bool diskToBuffer(long ItemSlotIdx, MyDB_TablePtr whichTable, long pageNum);

	//
	vector<Page_Buffer_Item>::iterator getBufferItemSpace();
};

#endif
