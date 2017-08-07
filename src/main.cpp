#include <memory>
#include <algorithm>
#include <chrono>
#include "stdLibInst.h"
#include "boostInst.h"
using namespace std;
using namespace chrono;


int main(int argc, char **argv) {
    boostInst::checkType();
    unique_ptr<boostInst> impl = make_unique<boostInst>();
    impl->main();

    using cl = high_resolution_clock ;
    using cl_point = high_resolution_clock::time_point;
    cl_point start = cl::now();

    cl_point end = cl::now();
    cout << duration_cast<seconds>(end - start).count() << endl;

}

int main2(int argc, char **argv) {
    unique_ptr<stdLibInst> impl = make_unique<stdLibInst>("First");
    cout << impl->toString() << endl;

    cout << "----" << endl;
    unique_ptr<stdLibInst> impl2 = make_unique<stdLibInst>("Second");
    impl.swap(impl2);
    cout << impl->toString() << endl;
    cout << impl2->toString() << endl;
    impl2.reset(nullptr);

    cout << "----" << endl;
    unique_ptr<stdLibInst> impl3 = make_unique<stdLibInst>("Third");
    impl.swap(impl3);
    cout << impl->toString() << endl;

    cout << "----" << endl;
    int size = 20;
    unique_ptr<stdLibInst[]> impln = make_unique<stdLibInst[]>(size);
    for(int i = 0; i < size; i++)
        cout << i << " - " << impln[i].toString() << endl;

    cout << "----" << endl;
    unique_ptr<stdLibInst[]> impln2 = make_unique<stdLibInst[]>(size);

    void (*print)(stdLibInst& inst) = [](stdLibInst& inst) { std::cout << inst.toString() << " "; };
    for_each(next(impln2.get(), 0), next(impln2.get(), size), print);

    impl->main();

    cout << "End of main" << endl;
}