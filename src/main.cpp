#include <memory>
#include <algorithm>
#include <chrono>
#include <csetjmp>
#include <csignal>

#include "stdLibInst.h"
#include "boostInst.h"
#include "genericInstance.h"

using namespace std;
using namespace chrono;

jmp_buf long_jump_reference;

volatile sig_atomic_t signal_handling_result = 0;

void signal_action_handler(int sig, siginfo_t* info, void*) {
    std::stringstream log;
    if(signal_handling_result == 0)
        printf("Received signal (first time) %d - cod %d - signo %d\n", sig, info->si_code, info->si_signo);
    else
        printf("Received signal %d - cod %d - signo %d\n", sig, info->si_code, info->si_signo);

    signal_handling_result = 1;
    longjmp(long_jump_reference, 1);
}

void doMain();

int main(int argc, char **argv) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = signal_action_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGABRT, &act, 0);

    if (setjmp(long_jump_reference) == 0) {
        doMain();
    }
}

void doMain() {
    unique_ptr<genericInstance> inst = make_unique<genericInstance>();

    boostInst::checkType();
    unique_ptr<boostInst> boostImpl = make_unique<boostInst>();
    boostImpl->main();

    using cl = high_resolution_clock ;
    using cl_point = high_resolution_clock::time_point;
    cl_point start = cl::now();

    cl_point end = cl::now();
    cout << duration_cast<seconds>(end - start).count() << endl;

    // stdLibInst
    {
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
        for (int i = 0; i < size; i++)
            cout << i << " - " << impln[i].toString() << endl;

        cout << "----" << endl;
        unique_ptr<stdLibInst[]> impln2 = make_unique<stdLibInst[]>(size);

        void (*print)(stdLibInst &inst) = [](stdLibInst &inst) { std::cout << inst.toString() << " "; };
        for_each(next(impln2.get(), 0), next(impln2.get(), size), print);

        impl->main();
    }

    cout << "End of main" << endl;
}

/**
 C++ concepts not yet supported at JetBrains IDE
 @see http://en.cppreference.com/w/cpp/language/constraints
template <class T, class U = T>
concept bool Swappable = requires(T&& t, U&& u) {
    swap(std::forward<T>(t), std::forward<U>(u));
    swap(std::forward<U>(u), std::forward<T>(t));
};
 */
