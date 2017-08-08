#ifndef CPP14_MAIN_H
#define CPP14_MAIN_H

#include <iostream>

using namespace std;

class custom_exception : std::exception
{
public:
    custom_exception() { };
    custom_exception(const custom_exception& e) = delete;
    custom_exception(custom_exception&& e) = delete;

    ~custom_exception() noexcept {
        cout << "destructor called" <<endl;
    };

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return "custom exception";
    };
};

void doMain();


#endif