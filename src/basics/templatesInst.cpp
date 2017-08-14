#include "templatesInst.h"

templatesInst::templatesInst() : m_generic_name2("genericInstance"),
                                 m_generic_name("templatesInst") {
    m_printer.print("templatesInst constructor");
    cout << Extensions::IntConstant66  << endl;
    cout << "Circular area double :" << Extensions::circular_area(2.2) << endl;
    cout << "Circular area int :" << Extensions::circular_area(2) << endl;
    // Initialize the protected member of the Base class
    m_name = "templatesInst 1";
    Extensions::MyList2 m;
    cout << " templatesInst " << endl;

}

Base &templatesInst::operator=(const Base &base2) {
    m_name = base2.m_name;
    return *this;
}

templatesInst::~templatesInst() {
    cout << "templatesInst destructor" << endl;
}
