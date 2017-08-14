#include "FileUtils.h"
#include <sys/param.h>
#include <unistd.h>
#include <regex>
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/asio.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "../hpp/custom_exception.hpp"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::posix_time;
using namespace boost::date_time;

using boost::asio::ip::tcp;
using boost::date_time::second_clock;

namespace fs = boost::filesystem;

template <int nesting>
string FileUtils::getIp() {
    stringstream result;
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(boost::asio::ip::host_name(), "");
    tcp::resolver::iterator iter = resolver.resolve(query);
    tcp::resolver::iterator end;
    while (iter != end) {
        tcp::endpoint ep = *iter++;
        std::cout << ep << std::endl;
        result << *iter << '_';
    }
    return result.str();
}

template <int nesting>
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

template <int nesting>
const string FileUtils::getStatsFile()  {
    string curDir = getCurrentDir();
    // cout << curDir << " - > " << fs::file_size(curDir.c_str()) << endl;
    return this->getStatsFile(curDir);
}

template<int nesting>
const string FileUtils::getStatsFile(string binary_path) {
    // Cut binary path one level
    stringstream ss_regex;
    for(int i=0; i < nesting; i++)
        ss_regex << "(.*)\\";
    regex pieces_regex(ss_regex.str(), regex_constants::icase);
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

    ptime t<ptime>(second_clock::local_time());

    // Check whether the stats directory exists
    stringstream ss1;
    ss1 << pieces_match[1] <<  "/stats";
    if(!fs::exists(ss1.str()) || ! fs::is_directory(ss1.str())) {
        auto exc = std::make_exception_ptr(new custom_exception("nesting parameter might be wrong"));
        std::rethrow_exception(exc);
    }

    stringstream ss;
    ss << pieces_match[1] <<  "/stats/cpp_sorting_";
    ss <<boost::asio::ip::host_name();
       ss << "_" << to_iso_string(t) <<"_stats.txt";
    return ss.str();
}
