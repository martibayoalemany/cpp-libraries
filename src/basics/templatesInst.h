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

    template<class T>
    constexpr T pi = T(3.14);

    template<class T>
    T circular_area(T r)
    {
        return pi<T> * r * r;
    }

    template<class ... Types> struct Tuple2 {};
    template<int v0, decltype(v0) ... Types> struct Tuple3 {};
    using MyList2 = Tuple3<1,2,'a'>;

}

class templatesInst : Base {

    private:
        string m_generic_name;
        string m_generic_name2;

    protected:
        Extensions::Printer<string> m_printer;

    public:
        // Initialization in inverse order is a not defined behaviour
        templatesInst();

        ~templatesInst();

        Base &operator=(const Base& base2) override;
};
