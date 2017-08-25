#pragma  once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;


/**
 * It reads the file at "ROOT_DIR/data/random_numbers.txt"
 * as a vector of T
 */
template <class T>
class DataFile : public enable_shared_from_this<DataFile<T>> {

    const char* DATA_FILENAME = "random_numbers.txt";
    vector<T> data;

    string getCurrentDir();
    const string getDataFile(basic_string<char> binary_path);

public:
    DataFile<T>()  {

    };

    virtual ~DataFile<T>() {
        cout << "Destructor DataFile" << endl;
    }

    vector<T> &getData();
    shared_ptr<vector<T>>  getDataPtr();
    const string getDataFile();

    shared_ptr<DataFile<T>> getptr() {
        return enable_shared_from_this<DataFile<T>>::shared_from_this();
    }

};

class NonSharedDataFile {
public:
    NonSharedDataFile() = default;
    ~NonSharedDataFile() {
    }

    inline shared_ptr<NonSharedDataFile> getPtr() {
        return shared_ptr<NonSharedDataFile>(this);
    }
};


#include "DataFile.cpp"


