using namespace std;

#include <iostream>
#include <vector>

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
		this->pageNum  = -1;
		this->acedBit  = false; // second change bit, default as unset(false)
		this->isPinned = false;
		this->isDirty  = false;
		this->isAnony  = false;
		pageData.reserve(this->pageSize);
	}
	Page_Buffer_Item(size_t pageSize)
	{
		this->pageSize = pageSize;
		this->pageNum  = -1;
		this->acedBit  = false; // second change bit, default as unset(false)
		this->isPinned = false;
		this->isDirty  = false;
		this->isAnony  = false;
		pageData.reserve(this->pageSize);
	}
	// ~Page_Buffer_Item();
};

class Clock_LRU
{
private:
    long numPages;
    vector<Page_Buffer_Item> buffPagePool;
    long currBuffPageIdx; // clock arm currently points to


public:
    /* constructor & destructor */
    Clock_LRU (size_t numPages = 16, size_t pageSize = 64) {
        this->numPages = numPages;
        buffPagePool.assign(numPages, Page_Buffer_Item(pageSize));
        this->currBuffPageIdx = 0;
    }

    Page_Buffer_Item * getBuffPage(long i) {
        return &buffPagePool.at(i);
    }

    /* getter & setter */
	long getNumPages() { return numPages; } // total number of pages in buffPagePool
	long getCurrBuffPageIdx() { return currBuffPageIdx; } // 可能不需要放在public?

	// long getItemNum(long idx) { } // return pageNum in buffPagePool at poisiton idx

};

int main()
{
    Clock_LRU p1(8, 8);
    Clock_LRU p2();

    // cout << p1.getNumPages() << endl;
    // cout << p1.getCurrBuffPageIdx() << endl;
    
    // cout << p1.getBuffPage()[0].isPinned << endl;
    // p1.getBuffPage()[1].isPinned = true;
    // p1.getBuffPage()[1].pageNum = 16;
    
    // for (int i = 0; i<8; i++) 
    // {  
    //     cout << p1.getBuffPage()[i].pageNum << endl;
    //     // cout << p1.getBuffPage()[i].isPinned << endl;
        
    // }

    Page_Buffer_Item *  page1 = p1.getBuffPage(1);
    cout << (*page1).pageSize << endl;
    (*page1).pageSize = 16;
    cout << (*page1).pageSize << endl;
    // cout << p1.getBuffPage(1).pageSize << endl;
    // p1.getBuffPage(1).isPinned = true;
    // cout << p1.getBuffPage(1).isPinned << endl;
    Page_Buffer_Item * pagePtr;
    for (int i=0;i < 8; i++)
    {
        pagePtr = p1.getBuffPage(i);
        (*pagePtr).pageSize = i;
    }
    for (int i=0;i < 8; i++)
    {
        pagePtr = p1.getBuffPage(i);
        cout << (*pagePtr).pageSize << endl;
    }

    
}