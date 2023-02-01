#include <iostream>
using namespace std;
int main()
{
    int x = 5;
    int *a = &x;
    int *&ref = a;
    cout << *ref << endl;
    *a = 6;
    cout << *ref << endl;
    (*ref)++;
    cout << *a << endl;
    return 0;
}