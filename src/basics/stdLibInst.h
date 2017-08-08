#ifndef CPP14_BASICS_STD_LIB_H
#define CPP14_BASICS_STD_LIB_H

#include <iostream>
#include <sstream>

using namespace std;

class stdLibInst {
    private:
        std::string m_name;

    public:
        void main();

    stdLibInst()  {
        m_name = {"unnamed"};
        cout << m_name << " construct" << endl;
    }

    explicit stdLibInst(std::string name) : m_name(name) {
        cout << m_name << " construct" << endl;
    }

    ~stdLibInst() {
        cout << m_name << " destructor" << endl;
    }

    inline std::string toString() {
        stringstream ss;
        ss << m_name << " stdLibInst" << endl;
        return ss.str();
    }

    void vectorsCode();
};


#endif //CPP14_BASICS_STD_LIB_H
