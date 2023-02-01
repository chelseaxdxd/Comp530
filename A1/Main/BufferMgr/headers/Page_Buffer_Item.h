#ifndef PAGE_BUFFER_ITEM_H
#define PAGE_BUFFER_ITEM_H

#include <vector>
#include <string>
using namespace std;

class Page_Buffer_Item
{
public:
	/* pageBufferItem member */
	string tablePath;
	size_t pageSize;
	long pageNum;
	bool acedBit; // second change bit, default as unset(false)
	bool isPinned;
	bool isDirty;
	bool isAnony;
	vector<char> pageData;

	Page_Buffer_Item(size_t pageSize)
	{
		this->tablePath = "";
		this->pageSize = pageSize;
		this->pageNum = -1;
		this->acedBit = false; // second change bit, default as unset(false)
		this->isPinned = false;
		this->isDirty = false;
		this->isAnony = false;
		pageData.resize(this->pageSize);
	}
	~Page_Buffer_Item(){};
};

#endif