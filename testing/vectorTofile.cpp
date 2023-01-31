#include <stdio.h>
#include <string.h>
#include <unistd.h>	   // for lseek    off_t lseek(int filedes, off_t offset, int whence);
#include <sys/types.h> // for lseek    off_t lseek(int filedes, off_t offset, SEEK_SET);
#include <fcntl.h>	   // for open, write, read
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

int main()
{
    vector<char> vect;
  
    vect.push_back('a');
    vect.push_back('b');
    vect.push_back('c');

    // file.write(vect.data(), vect.size());

    for (int i = 0; i < vect.size(); i++) {
            cout << vect[i] << " ";
    }
    cout << endl;

    int fd_tempFile = open("vectorwrite", O_CREAT | O_RDWR, 0666);
    // int fd_tempFile = creat("vectorwrite", O_RDWR | O_FSYNC);

    // char * buff = &vect[0];
    // char buff[4];
    // for (int i = 0; i < vect.size(); i++) {
    //         buff[i] = vect[i];
    // }
    // buff[3] = '\0';
    char * buff;
    buff = &vect[0];
    cout << "buff: " << buff << endl;

    // write(fd_tempFile, vect.data(), vect.size()); // 1. buff1:0��S
    // write(fd_tempFile, &vect, vect.size());   // 1. buff1:@&�O�
    int bytes = write(fd_tempFile, buff, 3);   // 1. buff1:@&�O�
    // int bytes = write(fd_tempFile, "abc", 3); // -1
    // int bytes = 0;
    // for (int i = 0; i < vect.size(); i++) {
    //         // bytes += 
    //         write(fd_tempFile, vect[i], 1);
    // }
    // for (int i = 0; i < 3; i++)
    // {
    //     // bytes += write(fd_tempFile, buff[i], 1);
    //     bytes += write(fd_tempFile, "i", 1);
    // }
    cout << "byte_write: " << bytes << endl;

    close(fd_tempFile);


    fd_tempFile = open("vectorwrite", O_CREAT | O_RDWR, 0666);
    // char buff2[3];
    char buff2[10];
    lseek(fd_tempFile, 0, SEEK_SET);
    int read_bytes = read(fd_tempFile, buff2, 3);

    cout << "buff2:"<< buff2 << endl;
    cout << "byte_read: " << read_bytes << endl;


    vector<char> readvect;
    lseek(fd_tempFile, 0, SEEK_SET);
    // read_bytes = read(fd_tempFile, &(*readvect.begin()), 3);
    // read_bytes = read(fd_tempFile, &readvect[0], 3);
    // strncpy(readvect, buff2, 3);
    
    for (int i = 0; i < 3; i++) {
            readvect.push_back(buff2[i]);
    }
    // std::string helloWorld = "// Hello World\n";
    // readvect.insert( readvect.begin(), buff2.begin(), buff2.end() );

    // cout << "vector_read: " << read_bytes << endl;
    cout << "readvect: " << endl;
    for (int i = 0; i < readvect.size(); i++) {
            cout << readvect[i];
    }
    cout << endl;


    for (int i = 0; i < 3; i++) {
            // readvect.push_back(buff2[i]);
            readvect.at(i) = buff2[i];
    }
    // readvect.insert(readvect.begin(), buff2, buff2+3);
    
    cout << "readvect: " << endl;
    for (int i = 0; i < readvect.size(); i++) {
            cout << readvect[i];
    }
    cout << endl;
    
    close(fd_tempFile);
    
    return 0;
}