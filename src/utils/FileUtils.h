#pragma once
#include <sys/param.h>
#include <unistd.h>
#include <regex>
#include <chrono>
#include <fstream>
#include "FileUtils.h"
#include "DataFile.h"

template<int nesting>
class FileUtils {
public:
    string getCurrentDir();
    const string getStatsFile();
    const string getStatsFile(string binary_path);

    string getIp();
};



