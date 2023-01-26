#ifndef CLOCK_LRU_H
#define CLOCK_LRU_H

#include <vector>
#include "Page_Buffer_Item.h"

using namespace std;

// use Page_Buffer_Item as element
class Clock_LRU
{
private:
    size_t numPages;
    size_t curr;
    // Page_Buffer_Item * buffPage; // head of the Page_Buffer
    vector<Page_Buffer_Item> buffPage; // Page_Buffer vector
    // Page_Buffer_Item * currBuffPagePtr;
    size_t currBuffPagePtr;

public:
	// pageSize: the size of each page
    // numPages: the number of pages managed by the buffer manager
    Clock_LRU (size_t pageSize = 64, size_t numPages = 16) {
        this->numPages = numPages;
        this->curr = 0;
        // buffPage = new Page_Buffer_Item[numPages];
        buffPage.assign(numPages, Page_Buffer_Item(pageSize));
        // currBuffPagePtr = &buffPage[0];
        this->currBuffPagePtr = 0;

    }

    // when the page buffer is destroyed
    // - disk page: 
    // dirty pages need to be written back to disk, 
    // any necessary data needs to be written to the catalog
    // - anonymous page:
    // |-- still have handle on it: written to a temporary file, and return the address
    // |-- no handle: delete    
    ~Clock_LRU () {
        
        // delete [] headPagePtr;
    }
    size_t getCurr() {
        return curr;
    }

    // If page not found in buffer
    // 1. when arm point to a setted item, unset & move arm to next item
    // 2. when arm point to an unsetted item, load data & set & move arm
    // (if point to a pinned pages, just move not next item)
    // return the pointer to item, where the arm currently points to
    size_t faultAndReplace(string tempFile,
					    long pageNum,
					    bool isPinned,
					    bool isAnony,
                        vector<char> newPageData) {
		// currentPage exceed the max_len of bufferPage
		// reset pointer to 0
		if (this->currBuffPagePtr == this->numPages) { this->currBuffPagePtr = 0}

		// clock arm movement
        while (true) 
		{
			Page_Buffer_Item * currBuffPage = buffPage[this->currBuffPagePtr];
            
            // if the page is pinned, skip this round  of the loop
			if (currBuffPage.getIsPinned())
            {
                this->currBuffPagePtr++;
                continue;
            }
			
			// find the page to evict (acedBit == false)
			if (!currBuffPage.getAcedBit())
			{
				// clean the data
                if (currBuffPage.getIsDirty())
				{
					// Anonymous & no handle 
					// delete
					
					// Anonymous & with handle
					// save to a temp file

					// disk: update disk page
					
				}
				// evict & replace with newPageItem
				currBuffPage.updateData(
                    pageNum,
					isPinned,
					false,   // isDirty; data just loaded, it won't be dirty
					isAnony,
					true     // acedBit
                    )
                currBuffPage.updatePagedata(
                    // data from disk
                    )

				// return pointer and then move the pointer 
				return this->currBuffPagePtr++;
			}
			currBuffPage.setAcedBit();
			this->currBuffPagePtr++;
		}
	}
    // long diskToBuffer(long pageNum, bool isPinned, bool isAnony);
    // bool writeBuffer(long pageNum, vector<char> pageData); //return True == succeed
    // vector<char> readBuffer(long pageNum);
    // bool pinThisPage(long pageNum);


}

#endif