#ifndef CLOCK_LRU_H
#define CLOCK_LRU_H

#include <vector>

using namespace std;


// use Page_Buffer_Item as element
// template <size_t numPages>
class Clock_LRU
{
private:
    Page_Buffer_Item *currentPagePtr;
    Page_Buffer_Item *pageBuffer;
    size_t numPages;


public:
	// numPages: the number of pages managed by the buffer manager
    Clock_LRU (size_t numPages = 16, size_t pageSize) {
        this->numPages = numPages;
        for (size_t i=0; i<numPages, i++)
        {
            pageBuffer[i] = new Page_Buffer_Item(pageSize);
        }
        currentPagePtr = pageBuffer;
    }

    // when the page buffer is destroyed
    // - disk page: 
    // dirty pages need to be written back to disk, 
    // any necessary data needs to be written to the catalog
    // - anonymous page:
    // |-- still have handle on it: written to a temporary file, and return the address
    // |-- no handle: delete    
    ~Clock_LRU ();

    void evict()
    void setAcedBit()

    

    

    


}

#endif