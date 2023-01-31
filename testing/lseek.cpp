#include <stdio.h>
#include <string.h>
#include <unistd.h>	   // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);
#include <fcntl.h>	   // for open, write, read
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    string tempFile = "tempDSFSD";
    string tempFilePath = "./" + tempFile;
	// int fd_tempFile = creat(tempFilePath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | O_FSYNC);
    int fd_tempFile = creat(tempFilePath.c_str(), O_RDWR | O_FSYNC);
	if (fd_tempFile < 0) {
        cout << "Unable to create " << tempFile << endl;
        exit(EXIT_FAILURE);
    }
    write(fd_tempFile, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 36);
    close(fd_tempFile);


    char* s2;
    // s2 = (char*)malloc(20);
 
    // strncpy(s2, s, 20);
    // return (char*)s2;

    fd_tempFile = open(tempFilePath.c_str(), O_RDWR | O_FSYNC);
    char buff[11];
    char * buff2 = (char *) malloc(20);

    lseek(fd_tempFile, 0, SEEK_SET);
    int read_bytes = read(fd_tempFile,buff,10);
    // buff[read_bytes] = '\0';
    
    cout << "read_bytes: " << read_bytes << endl;
    buff[read_bytes] = '\0';
    cout << "1. buff1:"<< buff << endl;
    cout << &buff << endl;

    // buff2 = buff;
    strncpy(buff2, buff, 10);
    cout << "2. buff2:" << buff2 << endl;
    cout << &buff2 << endl;

    // lseek
    lseek(fd_tempFile, 10, SEEK_SET);
    read_bytes = read(fd_tempFile,buff,10);
    cout << "read_bytes: " << read_bytes << endl;
    cout << "3. buff1:"<< buff << endl;
    cout << &buff << endl;

    // buff2 = buff;
    strncpy(buff2, buff, 10);
    cout << "4. buff2:" << buff2 << endl;
    cout << &buff2 << endl;
    close(fd_tempFile);

    //////
    fd_tempFile = open(tempFilePath.c_str(), O_RDWR | O_FSYNC);
    lseek(fd_tempFile, 20, SEEK_SET);
    read_bytes = read(fd_tempFile,buff,10);
    // buff[read_bytes] = '\0';
    
    cout << "read_bytes: " << read_bytes << endl;
    cout << "5. buff1:"<< buff << endl;
    cout << &buff << endl;






    close(fd_tempFile);

}
