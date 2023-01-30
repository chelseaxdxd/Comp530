#ifndef PAGE_MAP_ITEM_H
#define PAGE_MAP_ITEM_H
#include "Page_Buffer_Item.h"

using namespace std;

// class Page_Map_Item
// {
// private:
	
// 	long pageNum;
// 	Page_Buffer_Item* bufferItemPtr; 

// 	// 應該放在 Page_Map_Item? 而不是 Page_Buffer_Item
// 	// 如果資料 evict, 上面那些資料不需要存到disk，
// 	// 但有幾個handle指向這個page 需要被記著
// 	int refCnt = 0; 



// public:

// }
class Page
{
public:
	long pageNum;
	Page_Buffer_Item *bufferItemPtr = NULL;
	int refCnt = 0;
};

#endif