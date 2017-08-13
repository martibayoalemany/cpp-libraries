#ifndef CPP_LIBRARIES_UTILS_H
#define CPP_LIBRARIES_UTILS_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Utils {
public:
    vector<int> data;

public:
    Utils() = default;

    static string getCurrentDir();

    static const string getDataFile(basic_string<char> binary_path);

    static const string getDataFile();

    std::vector<int> &getData();
};


#endif //CPP_LIBRARIES_UTILS_H
