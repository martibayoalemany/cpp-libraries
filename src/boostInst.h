
#ifndef CPP14_BASICS_BOOSTINST_H
#define CPP14_BASICS_BOOSTINST_H

#include <iostream>
using namespace std;

class boostInst {
    public:
        boostInst() {
            cout << "Constructor" << endl;
        }

        boostInst(boostInst& other) {
            cout << "Copy constructor" << endl;
        }

        boostInst(boostInst&& other) {
            cout << "Move constructor" << endl;
        }

        ~boostInst() {
            cout << "destructor" << endl;
        }

        void main();
        static void checkType();
};


#endif //CPP14_BASICS_BOOSTINST_H
