// #include <unistd.h>
// #include <stdio.h>
// int main()
//  {
//    printf("Current working dir: %s\n", get_current_dir_name());
//    return 0;
//  }


#include <bits/stdc++.h> // for strcat 
#include <unistd.h>
#include <string>
#include <fcntl.h> 
#include <iostream>
// #include <filesystem>
#include <sys/stat.h>

using namespace std;

int main()
{
    // string tempFile = "./testrepo/temp";
    string temprepo = "ttt";
    string tempFile = "temp";
    // namespace fs = experimental::filesystem;
    // if (!fs::is_directory("testrepo") || !fs::exists("testrepo")) 
    // {
    //     fs::create_directory("testrepo");
    // }

    // string path = get_current_dir_name() + "dd";
    // cout << path;
    char * path = get_current_dir_name();
    // strcat(path, get_current_dir_name());
    strcat(path, "/");
    strcat(path, temprepo.c_str());
    strcat(path, "/");
    strcat(path, tempFile.c_str());
    // strcat(path, ".txt");
    cout << path << endl;

    // int fd = open(path,  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    // // int fd = open(tempFile.c_str(),  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    // write(fd, "s3", 2);
    // write(fd, "12345", 5);

    // // // // 不能close 再 write()
    // close(fd);

    // fd = open(path,  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    // // fd = open(tempFile.c_str(),  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    // write(fd, "ddddd", 2);
    // write(fd, "aaaaa", 5);

    // // // // 不能close 再 write()
    // close(fd);

    const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = creat(path, mode);

    write(fd, "abc", 3);
    close(fd);

    fd = open(path, O_RDWR | O_APPEND);
    write(fd, "TTT", 3);

    close(fd);

    
    // fd = open(path, O_RDONLY);
    // char * p;

    
    // read(fd, p, 7);
    // cout << "dddd" << endl;
    // cout << *p << endl;

    // close(fd);




    

    return 0;

}
