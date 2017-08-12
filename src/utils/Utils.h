#ifndef CPP_LIBRARIES_UTILS_H
#define CPP_LIBRARIES_UTILS_H

#include <iostream>
using namespace std;

class Utils {

public:
    static string getCurrentDir();

    static const string getDataFile(basic_string<char> binary_path);

    static const string getDataFile();
};


#endif //CPP_LIBRARIES_UTILS_H
