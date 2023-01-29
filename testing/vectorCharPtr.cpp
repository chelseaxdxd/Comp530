#include <iostream>
#include <vector>
using namespace std;

int main()
{
    char *buff;
    vector<char> pageData;
    pageData.push_back('a');
    pageData.push_back('b');
    pageData.push_back('c');
    buff = &pageData[0];
    cout << buff << endl;
    buff[0] = '1';
    buff[1] = '2';
    buff[2] = '3';
    cout << pageData[0];
    cout << pageData[1];
    cout << pageData[2];

    return 0;
}
/*
// char* buffer;
// ...
fread (buffer, 1, lSize, pFile);
// ...

ans

std::vector<char> data(buffer, buffer + size);


*/