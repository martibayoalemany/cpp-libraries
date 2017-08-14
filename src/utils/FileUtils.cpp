#include "FileUtils.h"
#include "../hpp/custom_exception.hpp"
#include <regex>

using namespace std;

// TODO: Fix the linkage with boost
/*
#include <boost/filesystem/operations.hpp>
#include <boost/asio.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
using namespace boost;
using namespace boost::asio;
using namespace boost::posix_time;
using namespace boost::date_time;
using boost::asio::ip::tcp;
using boost::date_time::second_clock;

namespace fs = boost::filesystem;
*/
/*
string FileUtils::getIp() {
    stringstream result;
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(boost::asio::ip::host_name(), "");
    tcp::resolver::iterator iter = resolver.resolve(query);
    tcp::resolver::iterator end;
    while (iter != end) {
        iter++;
        result << iter->host_name() << '_';
    }
    return result.str();
}
 */

string FileUtils::getCurrentDir() {
    int len = 10000;
    char* pBuf = new char[len];
    char szTmp[len];
    sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
    return pBuf;
}

const string FileUtils::getStatsFile()  {
    string curDir = getCurrentDir();
    // cout << curDir << " - > " << fs::file_size(curDir.c_str()) << endl;
    return this->getStatsFile(curDir);
}

const string FileUtils::getStatsFile(string binary_path) {
    // Cut binary path one level
    stringstream ss_regex;
    for(int i=0; i < 2; i++)
        ss_regex << "(.*)\\/";
    ss_regex << "(.*)";

    std::regex pieces_regex(ss_regex.str(), std::regex_constants::icase);
    cmatch pieces_match;
    if (regex_match(binary_path.c_str(), pieces_match, pieces_regex)) {
        for (size_t i = 0; i < pieces_match.size(); ++i) {
            csub_match sub_match = pieces_match[i];
            string piece = sub_match.str();
            if(VERBOSE)
                cout << "  submatch: " << i << ": " << piece << '\n';
        }
    } else {
        if(VERBOSE)
            cout << " No match " << endl;
    }

    //auto p = boost::date_time::second_clock::local_time();

    // Check whether the stats directory exists
    stringstream ss1;
    ss1 << pieces_match[1] <<  "/stats";

    /*
    if(!fs::exists(ss1.str()) || ! fs::is_directory(ss1.str())) {
        auto exc = std::make_exception_ptr(new custom_exception("nesting parameter might be wrong"));
        std::rethrow_exception(exc);
    }
    stringstream ss;
    ss << pieces_match[1] <<  "/stats/cpp_sorting_";
     ss <<boost::asio::ip::host_name();
    //ss << "_" << to_iso_string(*p.get());
    ss <<"_stats.txt";
     */

    stringstream ss;
    ss << pieces_match[1] <<  "/stats/cpp_sorting_stats.txt";
    return ss.str();
}

bool FileUtils::openStats() {
    string stats = getStatsFile();
    ofstream* _ofstream = new ofstream();
    _ofstream->open(stats, std::ofstream::out | std::ofstream::app);
    if(!_ofstream->is_open()) {
        cout << "File not opened" << endl;
        return false;
    }
    stringstream headerStream;
    headerStream << "name\t | shuffle\t | elements \t| duration_ms\t | p_duration_s\t | p_duration_ns\t | memory\t" << endl;
    headerStream << "-----\t | -----\t | ----- \t| -----\t | -----\t | -----\t | -----\t" << endl;
    *_ofstream << headerStream.str();
    _ofstream->close();
    return true;
}

bool FileUtils::writeStats(string name, int elements, int ms) {
    string stats = getStatsFile();
    ofstream* ofs = new ofstream();
    ofs->open(stats, std::ofstream::out | std::ofstream::app);
    if(!ofs->is_open()) {
        cout << "File not opened" << endl;
        return false;
    }
    *ofs  << name << "\t|";
    *ofs  << "" << "\t|";
    *ofs  << elements << "\t|";
    *ofs  << ms << "\t|";
    *ofs  << "" << "\t|";  // s
    *ofs  << "" << "\t|";  // ns
    *ofs  << "" << "\t|";  // memory
    *ofs  << endl;
    return true;
}
