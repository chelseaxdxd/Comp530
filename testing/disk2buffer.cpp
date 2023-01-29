
// #include <memory>
#include <string>
#include <fstream>
#include <iostream>

// create a smart pointer for database tables
using namespace std;

// this class encapsulates the notion of a database table
// DO NOT MODIFY!
class MyDB_Table {

public:
	// creates a new table with the given name, at the given storage location
	MyDB_Table (string name, string storageLoc) {
	tableName = name;
	storageLoc = storageLoc;
    }
	// get the name of the table
	string &getName (){
	return tableName;}
	// get the storage location of the table
	string &getStorageLoc () {
	return storageLoc;
    }
	// kill the dude
	~MyDB_Table () {}

private:
	// the name of the table
	string tableName;
	// the location where it is stored
	string storageLoc;
};



int main()
{
    // MyDB_Table table1("tempTable", "foobar");
    // string tablePath = table1.getStorageLoc() + "/" + table1.getName();
    // ofstream myFile(tablePath.c_str());
    string a1 =  "temp";
    string a2 = "temp.txt";
    cout << a1 + "/" + a2 << endl;
    // ofstream myFile((a1 + "/" + "table1/").c_str());
    // ofstream myFile((table1.getStorageLoc() + "/" + table1.getName()).c_str());
    // myFile.open(
	// 		(table1.getStorageLoc() + "/" + table1.getName()).c_str());
    // if (myFile.is_open()) {
    //         cout << "what the fuck" << endl;
    //         myFile << "this is a test" << endl;
    //         myFile.close();
    //     }

    ofstream myFile("~/storage-home/c/ch102/Comp530/testing/temp/filename.txt");
    myFile << "File cna be tricky" << endl;
    myFile.close();

}