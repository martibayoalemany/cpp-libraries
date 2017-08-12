#ifndef CPP_LIBRARIES_BASE_H
#define CPP_LIBRARIES_BASE_H

#include <iostream>

using namespace std;
class templatesInst;

class Base {
    protected:
        string m_name;
    public:
        /// default constructor
        Base() = default;
        /// Copy operator
        virtual Base &operator=(const Base& base2) = 0;
        /// Move operator
        Base &operator=(Base&& base2) = delete;

        virtual ~Base() {
            cout << "Base virtual destructor " << endl;
        }
    ///
    friend class templatesInst;
};


#endif //CPP_LIBRARIES_BASE_H
