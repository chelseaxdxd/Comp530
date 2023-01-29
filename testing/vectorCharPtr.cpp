#include <iostream>
#include <vector>
using namespace std;

int main()
{
    char *buff;
    vector<char> pageData;
    pageData.push_back('a');
    pageData.push_back('b');
    buff = &pageData[0];
    cout << buff << endl;
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