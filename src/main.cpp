#include <memory>
#include <algorithm>
#include <chrono>
#include <csetjmp>
#include <csignal>
#include <fstream>
#include <vector>

#include "basics/stdLibInst.h"
#include "basics/boostInst.h"
#include "basics/templatesInst.h"
#include "hpp/custom_exception.hpp"
#include "utils/Utils.h"

/**
 C++ concepts not yet supported at JetBrains IDE
 @see http://en.cppreference.com/w/cpp/language/constraints
template <class T, class U = T>
concept bool Swappable = requires(T&& t, U&& u) {
    swap(std::forward<T>(t), std::forward<U>(u));
    swap(std::forward<U>(u), std::forward<T>(t));
};
 */

using namespace std;
using namespace chrono;

using cl = chrono::high_resolution_clock;
using cl_point = chrono::high_resolution_clock::time_point;

jmp_buf long_jump_reference;

volatile sig_atomic_t signal_handling_result = 0;

void signal_action_handler(int sig, siginfo_t* info, void*) {
    std::stringstream log;

    if (signal_handling_result == 0)
        printf("Received signal (first time) %d - cod %d - signo %d\n",
               sig, info->si_code, info->si_signo);
    else
        printf("Received signal %d - cod %d - signo %d\n",
               sig, info->si_code, info->si_signo);

    signal_handling_result = 1;
    longjmp(long_jump_reference, 1);
}

void time_it(void(*funct)()) {
    cl_point start = cl::now();
    funct();
    cl_point end = cl::now();
    cout << duration_cast<milliseconds>(end - start).count() << endl;

}
int main(int argc, char **argv) {
    unique_ptr<Utils> utils = make_unique<Utils>();
    const string  dataFile = utils.get()->getDataFile();
    cout << "---- " << dataFile << " " << &dataFile << " ";
    cout << sizeof(dataFile.c_str()[0]) << endl;

    vector<int>& data = utils.get()->getData();

    vector<int> data2;
    copy(data.begin(), data.end(), back_inserter(data2));

    //for_each(results.cbegin(), results.cend(), [] (auto& res) { cout << res << endl;});

}

// Previous checks

int main2(int argc, char **argv) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = signal_action_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGABRT, &act, 0);

    if (setjmp(long_jump_reference) == 0) {
        doMain();
    }

    return 0;
}

void doMain() {

    {
        unique_ptr<templatesInst> inst = make_unique<templatesInst>();

        boostInst::checkType();
        unique_ptr<boostInst> boostImpl = make_unique<boostInst>();
        boostImpl->main();

        cl_point start = cl::now();

        cl_point end = cl::now();
        cout << duration_cast<seconds>(end - start).count() << endl;
    }

    // stdLibInst
    {
        unique_ptr<stdLibInst> impl = make_unique<stdLibInst>("First");
        cout << impl->toString() << endl;
        cl_point start = cl::now();
        impl->main();
        cl_point end = cl::now();
        cout << duration_cast<milliseconds>(end - start).count() << endl;

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
        for (int i = 0; i < size; i++)
            cout << i << " - " << impln[i].toString() << endl;

        cout << "----" << endl;
        unique_ptr<stdLibInst[]> impln2 = make_unique<stdLibInst[]>(size);

        void (*print)(stdLibInst &inst) =
        [](stdLibInst &inst) { std::cout << inst.toString() << " "; };
        for_each(next(impln2.get(), 0), next(impln2.get(), size), print);

    }

    //auto exc = std::make_exception_ptr(new custom_exception());
    //std::rethrow_exception(exc);

    cout << "End of main" << endl;

}


