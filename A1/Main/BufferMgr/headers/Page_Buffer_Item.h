#ifndef PAGE_BUFFER_ITEM_H
#define PAGE_BUFFER_ITEM_H

using namespace std;

class Page_Buffer_Item
{
private:
	long pageNum;
	int listNum; //type undefined
	bool isPinned;
	bool isDirty;
	bool isAnony;
	int refCnt;
	size_t pageSize;
	vector<char> pageData;
	
	


public:

}

#endif