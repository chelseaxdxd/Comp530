#include <unistd.h>
#include <string>
#include <fcntl.h> 
#include <iostream>
// #include <filesystem>
#include <sys/stat.h>

using namespace std;

int main()
{
    string temprepo = "ttt";
    string tempfile = "file";
    // namespace fs = experimental::filesystem;
    // if (!fs::is_directory("testrepo") || !fs::exists("testrepo")) 
    // {
    //     fs::create_directory("testrepo");
    // }

    mkdir(("./"+temprepo).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    // string a = "ttt";
    // int fd = open(a.c_str(),  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    // int fd = open("abc",  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    string path = "./"+temprepo+"/"+tempfile;
    int fd = open(path.c_str(),  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
    // int fd = open(("./"+temprepo+"/"+tempfile).c_str(),  O_RDWR | (O_APPEND | O_CREAT) | O_FSYNC);
     write(fd, "s3", 2);
     write(fd, "12345", 5);

    // // 不能close 再 write()
    close(fd);

    

    return 0;

}
