#include "Utils.h"
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


string Utils::getCurrentDir() {
    int len = 10000;
    char* pBuf = new char[len];
    char szTmp[len];
    sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
    return pBuf;
}

std::vector<int>& Utils::getData() {
    if( data.size() > 0 ) return Utils::data;

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
    cout << "Read file in " << duration_cast<milliseconds>( end - start ).count() << " ms " << endl;
    return data;
}

const string Utils::getDataFile() {
    string curDir = getCurrentDir();
    // cout << curDir << " - > " << fs::file_size(curDir.c_str()) << endl;
    return getDataFile(curDir);
}

const string Utils::getDataFile(string binary_path) {
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
    ss << pieces_match[1] <<  "/data/random_numbers.txt";
    return ss.str();
}