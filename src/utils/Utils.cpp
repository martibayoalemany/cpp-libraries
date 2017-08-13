#include "Utils.h"
#include <sys/param.h>
#include <unistd.h>
#include <regex>
#include <boost/filesystem.hpp>

using namespace std;
using fs = boost::filesystem;

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

const string Utils::getDataFile() {
    string curDir = getCurrentDir();
    cout << curDir << " - > " << fs::file_size(curDir.c_str()) << endl;
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