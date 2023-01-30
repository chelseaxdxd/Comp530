#include <vector>
#include <map>
#include <string>
#include <unistd.h>	   // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);
#include <fcntl.h>	   // for open, write, read
#include <iostream>
#include <cstdlib>
#include <memory>

using namespace std;
class MyDB_PageHandleBase;
class MyDB_Table;
// typedef shared_ptr <MyDB_Table> MyDB_TablePtr;
typedef MyDB_Table * MyDB_TablePtr;
class MyDB_Table {

public:

	// creates a new table with the given name, at the given storage location
	MyDB_Table (string name, string storageLocIn) {
        tableName = name;
        storageLoc = storageLocIn;
    }
	// get the name of the table
	string &getName ()  {
        return tableName;
    }
	// get the storage location of the table
	string &getStorageLoc () {
        return storageLoc;
    }
	// kill the dude
	~MyDB_Table () {}

private:
	// the name of the table
	string tableName;

	// the location where it is stored
	string storageLoc;
};


class Page_Buffer_Item
{
public:
	/* pageBufferItem member */
	// MyDB_TablePtr whichTable; // type undetermined
	size_t pageSize;
	long pageNum;
	bool acedBit; // second change bit, default as unset(false)
	bool isPinned;
	bool isDirty;
	bool isAnony;
	vector<char> pageData;

	Page_Buffer_Item()
	{
		this->pageSize = 64;
		this->pageNum = -1;
		this->acedBit = false; // second change bit, default as unset(false)
		this->isPinned = false;
		this->isDirty = false;
		this->isAnony = false;
		pageData.reserve(this->pageSize);
	}
	Page_Buffer_Item(size_t pageSize)
	{
		this->pageSize = pageSize;
		this->pageNum = -1;
		this->acedBit = false; // second change bit, default as unset(false)
		this->isPinned = false;
		this->isDirty = false;
		this->isAnony = false;
		pageData.reserve(this->pageSize);
	}
	// ~Page_Buffer_Item();
};

struct Page
{
	long pageNum;
	Page_Buffer_Item *bufferItemPtr = NULL;
	int refCnt = 0;
};




class MyDB_BufferManager 
{
private:
    /*ClockBuffer*/
	long numPages; // number of pages managed by the buffer manager is numPages
	vector<Page_Buffer_Item> clockBuffer; // buffPagePoo -> clockBuffer
	vector<Page_Buffer_Item>::iterator clockArm; // clock arm currently points to // currBuffPageIdx -> curClockIdx

    /* Page Map*/
    // anonymous hash map
	int anonySeq;
    map<pair<string, long>, Page> anonyPageMap; // pair<table_name, page_num>
    // non anonlymous hash map
	map<pair<string, long>, Page> diskPageMap;

    /* IO */
	int fd_tempFile;

public:
    MyDB_BufferManager(size_t pageSize, size_t numPages, string tempFile)
    {
        // declare buffer pool
        this->numPages = numPages;
        clockBuffer.assign(numPages, Page_Buffer_Item(pageSize));
        clockArm = clockBuffer.begin();

        // open a tempfile with name: tempFile for anonymous page
        string tempFilePath = "./" + tempFile;
        fd_tempFile = creat(tempFilePath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd_tempFile < 0)
        {
            cout << "Unable to create " << tempFile << endl;
            exit(EXIT_FAILURE);
        }

        // anonymous save page sequence number
        this->anonySeq = 0;
    }
    ~MyDB_BufferManager()
    {

        close(fd_tempFile);
    }

    


};





int main()
{
    MyDB_BufferManager myMgr (64, 16, "tempDSFSD");
    // MyDB_TablePtr table1 = &MyDB_Table ("tempTable", "foobar");
    MyDB_Table table1 ("tempTable", "foobar");

    // MyDB_PageHandleBase * pinnedPage = myMgr.getPinnedPage(&table1, 0);
}