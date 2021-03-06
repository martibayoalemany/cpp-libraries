#include "stdLibInst.h"

#include <regex>
#include <vector>
#include <bitset>
#include <array>
#include <algorithm>
#include <deque>
#include <chrono>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <zconf.h>
#include <sys/param.h>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

using namespace std;
using namespace chrono;
using namespace boost;

void stdLibInst::main() {

    cout << "----- > vector" << endl;
    vector<int> vec = {1, 2, 3, 4, 5};
    for_each(vec.begin(), vec.end(), [](int i) { cout << i << "\t"; });

    cout << "----- > vector" << endl;
    {
        int sum = 0;
        using Iter = vector<int>::iterator;
        for (Iter it = vec.begin(); it != vec.end(); ++it)
            sum += *it;

        cout << "Sum : " << sum << endl;
    }

    cout << "----- > search in vector" << endl;
    {
        vector<int> cs = {};
        for (int i = 0; i < 2000; i++)
            cs.push_back(rand());
        vector<int> search_for = {};
        for (int i = 20; i < 30; i++)
            search_for.push_back(cs[i]);

        auto result = find_first_of(cs.cbegin(), cs.cend(), search_for.begin(), search_for.end());

        if (result == cs.end())
            cout << " No elements found" << endl;
        else if (result != cs.end())
            cout << " Found a match at " << distance(cs.cbegin(), result) << endl;
    }

    cout << "----- > Merged vectors and search" << endl;
    {
        vector<int> as = {1, 2, 3, 4, 5, 6};
        vector<int> bs = {7, 8, 9, 10};
        for (int i = 0; i < 20; i++)
            for_each(as.begin(), as.end(), [&bs](int i) { bs.push_back(i); });
        cout << bs.size() << " / " << bs.max_size() << endl;
        auto tmp = search(bs.begin(), bs.end(), as.begin(), as.end());
        cout << "search " << (tmp.base() - as.begin().base()) / sizeof(int) << endl;

        cout << "----- > Copy if condition" << endl;

        list<int> target = {};
        auto less_than_3 = [](int i) { return i < 3; };
        copy_if(bs.begin(), bs.end(), target.begin(), less_than_3);
        cout << " less_than_3" << target.size() << " / " << bs.size() << endl;
        for_each(target.begin(), target.end(), [](int value) { cout << value << endl; });
    }

    cout << "----- > array" << endl;
    array<int, 20> arr = {10, 2};
    for_each(arr.begin(), arr.end(), [](int i) { cout << i << "\t"; });

    cout << "----- > bitset " << endl;
    std::bitset<16> bar(0xfa2);
    std::bitset<16> baz(std::string("010111110001"));
    cout << baz << endl;

    cout << "----- > deque push_front" << endl;
    deque<int> dq(10);
    cout << " s : " << dq.size() << " isEmpty : " << dq.empty() << " maxSize : " << dq.max_size() << endl;

    typedef array<int, 20>::iterator It;
    for (It it = arr.begin(); it < arr.end(); it++)
        dq.push_front(*it);
    cout << " s : " << dq.size() << " isEmpty : " << dq.empty() << " maxSize : " << dq.max_size() << endl;

    cout << "----- > deque insert" << endl;
    dq.insert(dq.begin() + 2, arr.begin(), arr.begin() + 2);
    cout << " s : " << dq.size() << " isEmpty : " << dq.empty() << " maxSize : " << dq.max_size() << endl;

    cout << "----- > deque insert" << endl;
    int array[] = {0, 1, 2, 3, 4, 5};
    dq.insert(dq.begin() + 2, array, array + 2);
    cout << " s : " << dq.size() << " isEmpty : " << dq.empty() << " maxSize : " << dq.max_size() << endl;

    cout << "----- > list insert" << endl;
    std::list<int> ints = {1, 2, 3, 4, 5, 6, 8, 9, 20};
    auto it = ints.begin();
    for(;it != ints.end(); it++)
        cout << *it << endl;

    cout << "----- > binary search" << endl;
    auto val = binary_search(ints.begin(), ints.end(), 22);
    cout << val << endl;

    cout << "----- > string reverse" << endl;
    string str = "hello world";
    reverse(str.begin(), str.end());
    cout << str << endl;

    cout << "----- > string reverse" << endl;
    auto any_even = any_of(ints.cbegin(), ints.cend(), [](int i) { return i % 2 == 0; });
    cout << "any even " << any_even << endl;

    cout << "----- > string mismatch" << endl;
    string a = "abcdefghijkl";
    string b = "abcdeghijkl";
    cout << " mismatch -> " << string(a.cbegin(), mismatch(a.cbegin(), a.cend(), b.cbegin()).first) << endl;

    cout << "----- > string search" <<endl;
    {
        string in = "Lorem ipsum dolor sit amet, consectetur adipiscing elit,"
                " sed do eiusmod tempor incididunt ut labore et dolore magna aliqua";
        string needle = "pisci";
        auto its = search(in.begin(), in.end(), needle.begin(), needle.end());
        if (its != in.end())
            cout << "The string " << needle << " found at offset "
                      << its - in.begin() << '\n';
        else
            cout << "The string " << needle << " not found\n";
    }

    cout << "----- > File reading syslog" << endl;
    {
        steady_clock::time_point st_now = steady_clock::now();
        fstream textFile;
        textFile.open("/var/log/syslog", ios::in);
        stringstream buffer;
        buffer << textFile.rdbuf();
        textFile.close();
        string line;
        while(getline(textFile, line))
            cout << line << endl;

        steady_clock::time_point en_now = steady_clock::now();

        cout << " Read file in " << duration_cast<chrono::microseconds>(en_now - st_now).count() << endl;

        cout << "----- > string search" << endl;

        st_now = steady_clock::now();
        string in = buffer.str();
        cout << " File size : " << in.size() << endl;
        string needle = "unity";
        auto its = search(in.begin(), in.end(), needle.begin(), needle.end());
        if (its != in.end())
            cout << "The string " << needle << " found at offset "
                 << its - in.begin() << '\n';
        else
            cout << "The string " << needle << " not found\n";
        en_now = steady_clock::now();
        cout << " Search in file in " << duration_cast<chrono::microseconds>(en_now - st_now).count() << endl;

    }

    cout << "----- > Map" << endl;
    {
        map<string, int> maps;
        maps["key1"] = 2;
        maps["key2"] = 3;
        for (auto &p : maps)
            cout << p.first << " -> " << p.second << endl;

        map<string, int> maps2 = {
                {"k1", 1},
                {"k2", 2},
                {"k3", 3},
                {"k5", 5}};

        for_each(maps.begin(), maps.end(), [](auto &p) { cout << p.first << " " << p.second << endl; });
    }

    cout << "----- > Set" << endl;
    {
        set<string> setOfNumbers = {"first", "second", "first"};
        cout << "setOfNumbers " << setOfNumbers.size() << endl;
    }

    cout << "----- > Stack" << endl;
    {
        stack<int> stacks;
        stacks.push(2);
        stacks.emplace(3);
        stacks.push(4);

        cout << "size " << stacks.size() << endl;
        do {
            int iTmp = stacks.top();
            cout << iTmp << endl;
            stacks.pop();
        } while (stacks.size() > 0);
    }

    cout << "----- > Unordered map" << endl;
    map<std::string, std::string> uno_maps = {{"Washington", "US"}, {"Madrid", "Spain"}, {"London", "UK"}};
    for (auto it : uno_maps)
    {
        cout << it.first << " -> " << it.second << endl;
    }

    std::unordered_map<string, string> un_uno_maps = {{"Washington", "US"}, {"Madrid", "Spain"}, {"London", "UK"}};
    for_each(un_uno_maps.begin(), un_uno_maps.end(), [] (auto & value) { cout << value.first << " -> " << value.second << endl;});

    cout << "----- > Unordered set" <<endl;
    {
        unordered_set<string> setOfNumbers2 = {"first", "second", "first", "third", "zero"};
        set<string> setOfNumbers = {"first", "second", "first"};

        auto print_value = [](auto &value) { cout << value << endl; };
        for_each(setOfNumbers.begin(), setOfNumbers.end(), print_value);
        cout << " ----" << endl;
        for_each(setOfNumbers2.begin(), setOfNumbers2.end(), print_value);
    }

    cout << "----- > Clocks" <<endl;
    {
        system_clock::time_point st_sys_now = system_clock::now();
        steady_clock::time_point st_std_now = steady_clock::now();
        high_resolution_clock::time_point st_high_now = high_resolution_clock::now();
        vector<int> v_rand_nums;
        for (int i = 0; i < 20000000; i++)
            v_rand_nums.push_back(rand());

        system_clock::time_point en_sys_now = system_clock::now();
        steady_clock::time_point en_std_now = steady_clock::now();
        high_resolution_clock::time_point en_high_now = high_resolution_clock::now();

        cout << "system clock : " << duration_cast<milliseconds>(en_sys_now - st_sys_now).count() << endl;
        cout << "steady clock : " << duration_cast<milliseconds>(en_std_now - st_std_now).count() << endl;
        cout << "high clock : " << duration_cast<milliseconds>(en_high_now - st_high_now).count() << endl;

        // Read the binary path
        int len = 10000;
        char* pBuf = new char[len];
        char szTmp[32];
        sprintf(szTmp, "/proc/%d/exe", getpid());
        int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
        if(bytes >= 0)
            pBuf[bytes] = '\0';
        cout << pBuf << endl;
        string binary_path = pBuf;
        cout << "string " << pBuf << endl;

        // Cut binary path one level
        regex pieces_regex("(.*)\\/(.*)\\/(.*)", regex_constants::icase);
        cmatch pieces_match;
        if (regex_match(binary_path.c_str(), pieces_match, pieces_regex)) {
            cout << pBuf << '\n';
            for (size_t i = 0; i < pieces_match.size(); ++i) {
                std::csub_match sub_match = pieces_match[i];
                std::string piece = sub_match.str();
                std::cout << "  submatch " << i << ": " << piece << '\n';
            }
        } else {
            cout << " No match " << endl;
        }

        stringstream ss;
        ss << pieces_match[1] << "/data/random_numbers2.gz" << endl;
        cout <<  ss.str() << endl;

        ofstream dataFile;
        dataFile.open(ss.str(), ofstream::out | ofstream::binary | ofstream::trunc);
        if(!dataFile.is_open())
            throw runtime_error("Not possible to open stream for file");

        // Compression
        /*
        iostreams::filtering_streambuf<boost::iostreams::output> out;
        out.push(iostreams::gzip_compressor());
        out.push(dataFile);
        std::ofstream outf(&out);

        using Iter = vector<int>::iterator;
        for(Iter it = v_rand_nums.begin(); it != v_rand_nums.end(); ++it)
            outf << *it << '\n';
        dataFile.close();
        */

    }


}