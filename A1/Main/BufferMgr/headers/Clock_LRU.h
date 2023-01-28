
#ifndef CLOCK_LRU_H
#define CLOCK_LRU_H

#include <vector>
#include <fstream>
#include "Page_Buffer_Item.h"

using namespace std;

// use Page_Buffer_Item as element
class Clock_LRU
{
private:
    long numPages;
    vector<Page_Buffer_Item> buffPagePool;
    long currBuffPageIdx; // clock arm currently points to

	// Update the metadata in ith pageItem
	
	void setPageNum(long pageNum) { this->pageNum = pageNum; }
	long getPageNum() { return this->pageNum; }
	void setPinned() { this->isPinned = true; }
	void unsetPinned() { this->isPinned = false; }
	bool getIsPinned() { return this->isPinned; }
	void setAcedBit() { this->acedBit = true; }
	void unsetAcedBit() { this->acedBit = false; }
	bool getAcedBit() { return this->acedBit; }
	bool getIsDirty() { return this->isDirty; }

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

	// when Clock_LRU needs to evict page, store the dirty data to disk
	void bufferToDisk(long ItemSlotIdx, 
					  MyDB_TablePtr whichTable, 
					  long pageNum, 
					  bool isPinned, 
					  bool isAnony);

	// load data from disk to buffer
	bool diskToBuffer(long ItemSlotIdx, 
					  MyDB_TablePtr whichTable, 
					  long pageNum) {
		ofstream myFile.open(
			"./" + whichTable.getStorageLoc() + "/" + whichTable.getName()	
		)
	}

	/* Clock Algorithm */
    long findItemSlot()  {
		// currentPage exceed the max_len of bufferPage
		// reset pointer to 0
		if (this->currBuffPageIdx >= this->numPages) { this->currBuffPageIdx = 0}

		Page_Buffer_Item * currBuffPage;
		// clock arm movement
        while (true) 
		{
			currBuffPage = buffPagePool[this->currBuffPageIdx];
            
			// if item slot(page) is pinned, just move arm to next item slot
			if (currBuffPage.isPinned)
            {
                this->currBuffPageIdx++;
                continue;
            }
			
			// found the page to evict (acedBit == false)
			// store dirty data to disk/tempFile/delete & set & move arm
			if (!currBuffPage.acedBit)
			{
				// clean the data
                if (currBuffPage.isDirty)
				{
					// Anonymous & no handle 
					// delete
					
					// Anonymous & with handle
					// save to a temp file

					// disk: update disk page
					
				}
				currBuffPage.acedBit = true;

				// return index of available item slot
				// then increment the index; 
				return this->currBuffPageIdx++;
			}
			// when arm point to a setted item, unset & move arm to next item slot
			currBuffPage.acedBit = false;
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


    // (1) buffMgr wants page i, and finds it's not in buffer (points to NULL)
	// (2) buffMgr go to idx i at buffPagePool, and finds the pageNum is inconsistent to its records
	long loadBuffer(MyDB_TablePtr whichTable, long pageNum, bool isPinned, bool isAnony) {
		// return idx of available slot to laod data
		long newItemSlotIdx = findItemSlot(); 
		// load data from disk to buffer
		bufferToDisk(newItemSlotIdx, whichTable, pageNum, isPinned, isAnony);
		// return which slotIdx the page is loaded
		return newItemSlotIdx;
	}
	
	// buffMgr wants to update data in buffer i (points to page i)
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