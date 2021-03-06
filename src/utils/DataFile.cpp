#ifndef  DATAFILE_IMPL_CPP
#define DATAFILE_IMPL_CPP

#include "DataFile.h"
#include <sys/param.h>
#include <unistd.h>
#include <regex>
#include <chrono>
#include <fstream>
//#include <boost/filesystem.hpp>

using namespace std;
//using fs = boost::filesystem;
using namespace chrono;

using cl = chrono::high_resolution_clock;
using cl_point = chrono::high_resolution_clock::time_point;

template <class T>
shared_ptr<vector<T>> DataFile<T>::getDataPtr() {
    shared_ptr<vector<T>> result = make_shared<vector<T>>(getData());
    return result;
}

template <class T>
std::vector<T>& DataFile<T>::getData() {
    if( this->data.size() > 0 ) return this->data;

    string dataFile = getDataFile();
    fstream file;
    file.open(dataFile, ios::in);
    string line;
    cl_point start = cl::now();
    while (getline(file, line)) {
        int result = atoi(line.c_str());
        data.push_back(result);
    }
    cl_point end = cl::now();
    cout << "Read file: " << duration_cast<milliseconds>( end - start ).count() << " ms " << endl;
    return data;
}

template <class T>
string DataFile<T>::getCurrentDir() {
    int len = 10000;
    char* pBuf = new char[len];
    char szTmp[len];
    sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
    return pBuf;
}

template <class T>
const string DataFile<T>::getDataFile() {
    string curDir = getCurrentDir();
    // cout << curDir << " - > " << fs::file_size(curDir.c_str()) << endl;
    return this->getDataFile(curDir);
}

template <class T>
const string DataFile<T>::getDataFile(string binary_path) {
    // Cut binary path one level
    regex pieces_regex("(.*)\\/(.*)\\/(.*)", regex_constants::icase);
    cmatch pieces_match;
    if (regex_match(binary_path.c_str(), pieces_match, pieces_regex)) {
        for (size_t i = 0; i < pieces_match.size(); ++i) {
            csub_match sub_match = pieces_match[i];
            string piece = sub_match.str();
            cout << "  submatch: " << i << ": " << piece << '\n';
        }
    } else {
        cout << " No match " << endl;
    }

    stringstream ss;
    ss << pieces_match[1] <<  "/data/" << string(DATA_FILENAME);
    return ss.str();
}

#endif