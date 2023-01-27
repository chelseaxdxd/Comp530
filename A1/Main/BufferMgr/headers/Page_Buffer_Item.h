#ifndef PAGE_BUFFER_ITEM_H
#define PAGE_BUFFER_ITEM_H

#include <vector>
using namespace std;

class Page_Buffer_Item
{
public:
	/* pageBufferItem member */ 
	MyDB_TablePtr whichTable; // type undetermined
	size_t pageSize;
	long pageNum = -1;
	bool acedBit = false; // second change bit, default as unset(false)
	bool isPinned = false;
	bool isDirty = false;
	bool isAnony = false;
	vector<char> pageData;

	/* getter & setter */ 
	// void setPageNum(long pageNum) { this->pageNum = pageNum; }
	// long getPageNum() { return this->pageNum; }
	// void setPinned() { this->isPinned = true; }
	// void unsetPinned() { this->isPinned = false; }
	// bool getIsPinned() { return this->isPinned; }
	// void setAcedBit() { this->acedBit = true; }
	// void unsetAcedBit() { this->acedBit = false; }
	// bool getAcedBit() { return this->acedBit; }
	// bool getIsDirty() { return this->isDirty; }

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
	~Page_Buffer_Item();
}

#endif