#pragma once

#include <sys/param.h>
#include <unistd.h>
#include <regex>
#include <chrono>
#include <fstream>
#include "FileUtils.h"
#include "DataFile.h"

class FileUtils  {
    ofstream* _ofstream;
public:
    FileUtils() = default;
    virtual ~FileUtils() = default;

    bool openStats();
    /// "name\t | shuffle\t | elements \t| duration_ms\t | p_duration_s\t | p_duration_ns\t | memory\t";
    bool writeStats(string name, int elements, int ms);
    string getCurrentDir();
    const string getStatsFile();
    const string getStatsFile(string binary_path);
    //string getIp();
};



