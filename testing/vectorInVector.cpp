using namespace std;

#include <vector>

class Page_Buffer_Item
{
public:
    size_t pageSize;
    bool isPinned = false;
    vector<char> pageData;

    Page_Buffer_Item(size_t pageSize)
    {
        this->pageSize = pageSize;
        pageData.reserve(this->pageSize);
    }
};

class Clock_LRU
{
private:
    size_t numPages;
    vector<Page_Buffer_Item> buffPage;
    size_t currBuffPagePtr;

public:
    Clock_LRU (size_t pageSize = 64, size_t numPages = 16) {
        this->numPages = numPages;
        buffPage.assign(numPages, Page_Buffer_Item(pageSize));
        this->currBuffPagePtr = 0;
    }

    vector<Page_Buffer_Item> getBuffPage() {
        return buffPage;
    }
};

int main()
{
    Clock_LRU p1(16, 8);
    Clock_LRU p2();
    p1.getBuffPage()[0].isPinned = true;
    
}