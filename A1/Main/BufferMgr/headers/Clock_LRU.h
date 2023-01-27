
#ifndef CLOCK_LRU_H
#define CLOCK_LRU_H

#include <vector>
#include "Page_Buffer_Item.h"

using namespace std;

// use Page_Buffer_Item as element
class Clock_LRU
{
private:
    long numPages;
    vector<Page_Buffer_Item> buffPagePool;
    long currBuffPageIdx; // clock arm currently points to

	// Update the metadata in page
	void updateData(long pageNum,
					bool isPinned,
					bool isDirty,
					bool isAnony,
					bool acedBit) {
		this->pageNum = pageNum;
		this->isPinned = isPinned;
		this->isDirty = isDirty;
		this->isAnony = isAnony;
		this->acedBit = acedBit;
	}
	// Update pageData
	void updatePagedata(vector<char> newPageData) {
		this->pageData = newPageData // depend on how pageData is defined
	}

	// !!! can be private, currently only Clock_LRU would use it
	// when Clock_LRU needs to evict page, store the dirty data to disk
	void bufferToDisk(long pageNum, ...);

	// load data from dist to buffer
	bool diskToBuffer(long pageNum, ...)

	/* Clock Algorithm */
    // 0. if point to a pinned pages, just move arm to next item
    // 1. when arm point to a setted item, unset & move arm to next item
    // 2. when arm point to an unsetted item, 
	//    store dirty data to disk & set & move arm
    // return the pointer to item, where the arm currently points to
    long findBuffPageSlot(string tempFile,
					    long pageNum,
					    bool isPinned,
					    bool isAnony,
                        vector<char> newPageData) {
		// currentPage exceed the max_len of bufferPage
		// reset pointer to 0
		if (this->currBuffPageIdx == this->numPages) { this->currBuffPageIdx = 0}

		// clock arm movement
        while (true) 
		{
			Page_Buffer_Item * currBuffPage = buffPage[this->currBuffPageIdx];
            
            // if the page is pinned, skip this round  of the loop
			if (currBuffPage.getIsPinned())
            {
                this->currBuffPageIdx++;
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
				return this->currBuffPageIdx++;
			}
			currBuffPage.setAcedBit();
			this->currBuffPageIdx++;
		}
	}
    
public:
	/* constructor & destructor */
    Clock_LRU (size_t numPages = 16, size_t pageSize = 64) {
        this->numPages = numPages;
        buffPagePool.assign(numPages, Page_Buffer_Item(pageSize));
        this->currBuffPageIdx = 0;
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

    /* getter & setter */
	long getNumPages() { return numPages; } // total number of pages in buffPagePool
	long getCurrBuffPageIdx() { return currBuffPageIdx; } // 可能不需要放在public?

	long getItemNum(long idx) { } // return pageNum in buffPagePool at poisiton idx


    // (1) buffMgr wants page i, and finds it not in buffer (points to NULL)
	// (2) buffMgr go to idx i at buffPagePool, and finds the pageNum is inconsistent to its records
	// !!!! rename to loadBuffer???
	long loadBuffer(MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony) {
		long newSlot = findBuffPageSlot(); // evict and return idx to laod data
		// load data from disk to buffer

		// return index of the page


	}
	
	// buffMgr wants to update data in page i (points to page i)
	// update newPage, mark isDirty & acedBit to true, return True == succeed
    bool writeBuffer(long pageNum, vector<char> pageData); 
    
	// buffMgr wants to access page i data (points to page i)
	// mark acedBit to true
	vector<char> readBuffer(long pageNum);

	// buffMgr marks page i as pinned (points to page i)
    bool pinThisPage(long pageNum);

    


    // void loadFromDisk(string tempFile,
	// 				  long pageNum,
	// 				  bool isPinned,
	// 				  bool isAnony) {
		
	// 	// arguments that are passed
	// 	this->updateData(
	// 		pageNum, 
	// 		isPinned, 
	// 		false,   // isDirty; data just loaded, it won't be dirty
	// 		isAnony, // isAnony = false? or combine loadFromDisk and loadFromTempFile
	// 		true     // acedBit
	// 		)

	// 	// load data from disk
	// 	this->updatePagedata(
	// 		// data from disk
	// 	)
		
		
	// 	// set the reference to 0? 
	// 	// depend on whether there is handle on the disk
	// 	// refCnt = 0;
	// }
	


	


}

#endif