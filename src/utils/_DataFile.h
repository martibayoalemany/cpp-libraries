#pragma  once
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

namespace _DataFile {
    string _getCurrentDir();

    const string _getDataFile(string binary_path);
    shared_ptr<vector<int>> _getData();
    const tuple<string,shared_ptr<vector<int>>> getData();
}
