#pragma  once

#include <string>
#include <iostream>
#include "Base.h"

using namespace std;

namespace Extensions {

template<class T> class Printer {

    public:
        void print(T msg) { cout << msg << endl;  }
};

template <typename Type, Type value> constexpr Type constant = value;
constexpr auto const IntConstant66 = constant<int, 66>;

/// Not compiling yet
// (1)
// template <auto value> constexpr auto constant2 = value;
// constexpr auto const IntConstant66_2 = constant2<66>;

// (2)
// template <auto ... vs > struct HeterogenousValueList {};
// using MyList1 = HeterogenousValueList<42, 'X', 13u>;

// (3)
// template <auto v0, decltype(v0) ... vs> struct HomogenousValueList {};
// using MyList2 = HomogenousValueList<1, 2, 3>;

}

class templatesInst : Base {

    private:
        string m_generic_name;
        string m_generic_name2;

    protected:
        Extensions::Printer<string> m_printer;

    public:
        // Initialization in inverse order is a not defined behaviour
        templatesInst() : m_generic_name2("genericInstance"),
                          m_generic_name("templatesInst") {
            m_printer.print("templatesInst constructor");
            cout << Extensions::IntConstant66  << endl;
            // Initialize the protected member of the Base class
            m_name = "templatesInst 1";
        }

        ~templatesInst() {
            cout << "templatesInst destructor" << endl;
        }

        Base &operator=(const Base& base2) override {
            m_name = base2.m_name;
            return *this;
        }
};
