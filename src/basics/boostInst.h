
#ifndef CPP_LIBRARIES_BOOSTINST_H
#define CPP_LIBRARIES_BOOSTINST_H

#include <iostream>
using namespace std;

class boostInst {
    public:
        boostInst() = default;

        boostInst(const boostInst& other) noexcept = default;

        boostInst(boostInst&& other) noexcept {
            cout << "Move constructor" << endl;
        }

        ~boostInst() {
            cout << "destructor" << endl;
        }

        boostInst &operator=(boostInst const & copy) = default;
        boostInst &operator=(boostInst && move) = default;

        void main();
        static void checkType();
};


#endif //CPP14_BASICS_BOOSTINST_H
