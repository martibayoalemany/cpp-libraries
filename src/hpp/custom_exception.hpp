#ifndef CPP14_MAIN_H
#define CPP14_MAIN_H

#include <iostream>

using namespace std;

class custom_exception : std::exception
{
private:
    string _what;
public:
    custom_exception() { };
    custom_exception(string what) : _what(what){ };
    custom_exception(const custom_exception& e) = delete;
    custom_exception(custom_exception&& e) = delete;

    ~custom_exception() noexcept {
        cout << "destructor called " << _what <<endl;
    };

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return "custom exception";
    };

    string what() {
        return _what;
    }
};

void doMain();


#endif