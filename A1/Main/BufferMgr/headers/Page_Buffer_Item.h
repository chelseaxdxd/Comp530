#ifndef PAGE_BUFFER_ITEM_H
#define PAGE_BUFFER_ITEM_H

using namespace std;

class Page_Buffer_Item
{
private:
	size_t pageSize;
	long pageNum = -1;
	// int listNum; //type undefined // clock algo don't need listNum?
	bool isPinned = false;
	bool isDirty = false;
	bool isAnony = false;
	// 應該放在 Page_Map_Item? 
	// 如果資料 evict, 上面那些資料不需要存到disk，
	// 但有幾個handle指向這個page 需要被記著
	// int refCnt = 0; 
	// second change bit, default as unset(false)
	bool acedBit = false;
	vector<char> pageData;

	 
	

public:
	// void setPageNum(long pageNum) { this->pageNum = pageNum; }
	// long getPageNum() { return this->pageNum; }
	void setPinned() { this->isPinned = true; }
	void unsetPinned() { this->isPinned = false; }
	bool getIsPinned() { return this->isPinned; }
	void setAcedBit() { this->acedBit = true; }
	void unsetAcedBit() { this->acedBit = false; }
	bool getAcedBit() { return this->acedBit; }
	bool getIsDirty() { return this->isDirty; }


	Page_Buffer_Item()
	{
		this->pageSize = 64;
		pageData.reserve(this->pageSize);
	}
	Page_Buffer_Item(size_t pageSize)
	{
		this->pageSize = pageSize;
		pageData.reserve(this->pageSize);
	}
	~Page_Buffer_Item();

	void loadFromDisk(string tempFile,
					  long pageNum,
					  bool isPinned,
					  bool isAnony) {
		
		// arguments that are passed
		this->updateData(
			pageNum, 
			isPinned, 
			false,   // isDirty; data just loaded, it won't be dirty
			isAnony, // isAnony = false? or combine loadFromDisk and loadFromTempFile
			true     // acedBit
			)

		// load data from disk
		this->updatePagedata(
			// data from disk
		)
		
		
		// set the reference to 0? 
		// depend on whether there is handle on the disk
		// refCnt = 0;
	}
	void loadFromTempFile(string tempFile,
						  long pageNum,
						  bool isPinned,
						  bool isAnony);
	void storeToDisk();


	// If page found in buffer, updates the acedBit to true
    bool hitAndSet() {
        this->acedBit = true;
		return true;
    }

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

}

#endif