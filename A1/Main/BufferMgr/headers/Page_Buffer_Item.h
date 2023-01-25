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

	bool acedBit = false; // second change bit, default as unset(false)
	

public:
	Page_Buffer_Item(size_t pageSize)
	{
		this->pageSize = pageSize;
	}

	// If page found in buffer, updates the acedBit to true
    bool hitAndSet() {
        acedBit = true;
    }

	// If page not found in buffer
    // 1. when arm point to a setted item, unset & move arm to next item
    // 2. when arm point to an unsetted item, load data & set & move arm
    // (if point to a pinned pages, just move not next item)
    // return the pointer to item, where the arm currently points to
    Page_Buffer_Item * faultAndLoad(Page_Buffer_Item *currentPagePtr, 
							Page_Buffer_Item newPageItem) {
		// currentPage is null
		// reset to head
		// 要放在 Page_Buffer_Itme 還是 Clock_LRU?
		if (currentPagePtr == nullptr)
		{

		}

		while (true) 
		{
			// find the page to evict (acedBit == false)
			if (!(*currentPagePtr->acedBit))
			{
				// if dirty
				if (isDirty)
				{
					// Anonymous & no handle 
					// delete
					
					// Anonymous & with handle
					// save to a temp file

					// disk: update disk page

					
				}
				// evict & replace with newPageItem
				*currentPagePtr = newPageItem;

				// return updated pointer
				return (currentPagePtr+1);
			}
			acedBit = true;
			currentPagePtr++;
		}
	}

}

#endif