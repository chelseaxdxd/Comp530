#include <unistd.h>
#include <string>
#include <fcntl.h> 
#include <iostream>
// #include <filesystem>
#include <sys/stat.h>

using namespace std;

int main()
{
    string tempFile = "./testrepo/temp";
    // namespace fs = experimental::filesystem;
    // if (!fs::is_directory("testrepo") || !fs::exists("testrepo")) 
    // {
    //     fs::create_directory("testrepo");
    // }


  
    int fd = open(tempFile.c_str(),  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    write(fd, "s3", 2);
    write(fd, "12345", 5);

    // // 不能close 再 write()
    close(fd);

    fd = open(tempFile.c_str(),  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    write(fd, "ddddd", 2);
    write(fd, "aaaaa", 5);

    // // 不能close 再 write()
    close(fd);
    

    return 0;

}
