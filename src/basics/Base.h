


#ifndef CPP_LIBRARIES_BASE_H
#define CPP_LIBRARIES_BASE_H
#include <iostream>

using namespace std;
class genericInstance;
class Base {
    protected:
        string m_name;
    public:
        Base() = default;
        virtual Base &operator=(const Base& base2) = 0;
        Base &operator=(Base&& base2) = delete;
        virtual ~Base() {
            cout << "Base virtual destructor " << endl;
        }
    friend class genericInstance;
};


#endif //CPP_LIBRARIES_BASE_H
