
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <string>
#include <unistd.h> // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);

using namespace std;

MyDB_PageHandle MyDB_BufferManager :: getPage (MyDB_TablePtr, long) {
	// page not exist, store into buffer and return handle

	// page exist in buffer, return the handle

	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPage () {
	// find an unpin or pinned anonymous page in buffer, return handle

	// an anonymous page not existin buffer, create an anonlymous(temp) page,
	 return the handle
	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage (MyDB_TablePtr, long) {
	// page not exist, store into buffer, if unpin then pin it and return handle

	// page exist in buffer, if unpin then pin it and return the handle
	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage () {
	// find an anonymous pinned page in buffer, return handle

	// an anonymous pinned page not existin buffer, create an anonlymous(temp) page, pin it, and return the handle
	return nullptr;		
}

void MyDB_BufferManager :: unpin (MyDB_PageHandle unpinMe) {
}

MyDB_BufferManager :: MyDB_BufferManager (size_t, size_t, string) {
	//declare buffer pool

	//
}

MyDB_BufferManager :: ~MyDB_BufferManager () {
}
	
#endif


