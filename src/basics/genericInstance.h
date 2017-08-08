


#ifndef CPP_LIBRARIES_GENERICINSTANCE_H
#define CPP_LIBRARIES_GENERICINSTANCE_H

#include <iostream>
#include "Base.h"

using namespace std;

namespace Extensions {
    template<class T>
    class Printer {
        public:
            void print(T msg) { cout << msg << endl;  }
    };

    template <typename Type, Type value> constexpr Type constant = value;
    constexpr auto const IntConstant66 = constant<int,66>;

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

class genericInstance : Base {
    private:
        string m_generic_name;
        string m_generic_name2;
    protected:
        Extensions::Printer<string> m_printer;
    public:
        genericInstance() : m_generic_name2("genericInstance"), m_generic_name("genericInstance") {
            m_printer.print("genericInstance constructor");

            cout << Extensions::IntConstant66  << endl;
            m_name = "Instance 1";
        }

        ~genericInstance() {
            cout << "genericInstance destructor" << endl;
        }

        Base &operator=(const Base& base2) override {
            m_name = base2.m_name;
            return *this;
        }
};


#endif //CPP_LIBRARIES_GENERICINSTANCE_H
