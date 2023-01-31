#include <string>
#include <unistd.h>    // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);
#include <fcntl.h>     // for open, write, read
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    string tempFile = "tempDSFSD";
    string tempFilePath = "./" + tempFile;
    int fd_tempFile;
    // fd_tempFile = creat(tempFilePath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | O_FSYNC);

    /*
    fd_tempFile = creat(tempFilePath.c_str(), O_RDWR | O_FSYNC);
    if (fd_tempFile < 0)
    {
        cout << "Unable to create " << tempFile << endl;
        exit(EXIT_FAILURE);
    }
    write(fd_tempFile, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", 36);
    // close(fd_tempFile);
    write(fd_tempFile, "123", 3);
    */

    fd_tempFile = open(tempFilePath.c_str(), O_RDWR | O_APPEND);
    write(fd_tempFile, "new", 3);
    close(fd_tempFile);

    // read
    fd_tempFile = open(tempFilePath.c_str(), O_RDWR);
    char buff[37];
    int sz = read(fd_tempFile, buff, 10);
    buff[sz] = '\0';
    cout << buff << endl;
    close(fd_tempFile);

    // lseek
    fd_tempFile = open(tempFilePath.c_str(), O_RDWR);
    lseek(fd_tempFile, 5, SEEK_SET);
    sz = read(fd_tempFile, buff, 10);
    buff[sz] = '\0';
    cout << buff << endl;
    close(fd_tempFile);
    // char buff[37];
    /*
    char *buff;
    lseek(fd_tempFile, 0, SEEK_SET);
    int read_bytes = read(fd_tempFile, buff, 36);

    cout << "read_bytes: " << read_bytes << endl;
    cout << buff << endl;

    close(fd_tempFile);
    */
    return 0;
}
