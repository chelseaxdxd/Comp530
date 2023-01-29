#include <iostream>
#include <unistd.h> // for get_current_dir_name
#include <bits/stdc++.h> // for strcat 
#include <fcntl.h> // for create, write, read
using namespace std;

int main()
{
    // open a tempfile with this name for anonymous page
	// fd_tempFile = open(tempFile.c_str(), O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);

    string tempFile = "tempDSFSD";
    tempFile = "./" + tempFile;
    cout << tempFile << endl;
    
    // char * filePath = get_current_dir_name();
    // strcat(filePath, tempFile.c_str());
    // cout << filePath << endl;

    // User has read & write perms, group and others have read permission
    int fd_tempFile = creat(tempFile.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd_tempFile < 0) {
        cout << "Unable to open " << tempFile << endl;
        exit(EXIT_FAILURE);
    }
    cout << fd_tempFile << endl;


    write(fd_tempFile, "012", 3);
    close(fd_tempFile);

    fd_tempFile = open(tempFile.c_str(), O_RDWR | O_APPEND);
    write(fd_tempFile, "34567", 5);
    close(fd_tempFile);

    fd_tempFile = open(tempFile.c_str(), O_RDWR);
    lseek(fd_tempFile, 10, SEEK_CUR); // open idx 0 + 10 =  start from 11
    write(fd_tempFile, "DDD", 3);
    lseek(fd_tempFile, 15, SEEK_CUR); // last insert DDD end at 13 + 15 = start from 28
    write(fd_tempFile, "GGG", 3);
    lseek(fd_tempFile, 20, SEEK_SET);
    write(fd_tempFile, "HHH", 3);
    close(fd_tempFile);

}

