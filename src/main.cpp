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
#include "utils/DataFile.h"

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

void time_it(string desc, std::function<void()> funct) {
    cl_point start = cl::now();
    funct();
    cl_point end = cl::now();
    cout << desc << " " << duration_cast<milliseconds>(end - start).count()  << " ms " << endl;
}

template <class T>
T time_it_supplier(string desc, std::function<T()> funct) {
    cl_point start = cl::now();
    T result = funct();
    cl_point end = cl::now();
    cout << desc << " " << duration_cast<milliseconds>(end - start).count()  << " ms " << endl;
    return result;
}

tuple<int, int, int> foo() {
    return {1,2,3 };
};

template<typename T>
void for_each_n_print(vector<T> v, size_t size) {
    auto exc = std::make_exception_ptr(new custom_exception());
    std::rethrow_exception(exc);
}

template<>
void for_each_n_print<int>(vector<int> v, size_t size) {
    using Iter = vector<int>::iterator;
    int i = 0;
    for(Iter it = v.begin(); i < size; it++, i++)
        cout << *it << '\t';
    cout << endl;
    cout << "=======" << endl;
}

template<typename... T>
tuple<T ...> increment(T... its) {
    return make_tuple((++its)...);
}

template<std::size_t> struct int_{};

template <class Tuple, size_t Pos>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, int_<Pos> ) {
    out << std::get< std::tuple_size<Tuple>::value-Pos >(t) << ',';
    return print_tuple(out, t, int_<Pos-1>());
}

template <class Tuple>
std::ostream& print_tuple(std::ostream& out, const Tuple& t, int_<1> ) {
    return out << std::get<std::tuple_size<Tuple>::value-1>(t);
}

template <class... Args>
ostream& operator<<(ostream& out, const std::tuple<Args...>& t) {
    out << '(';
    print_tuple(out, t, int_<sizeof...(Args)>());
    return out << ')';
}

void safeApiChecks();
void doApiChecks();

int main(int argc, char **argv) {

    safeApiChecks();
    //doApiChecks();
}

void safeApiChecks() {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = signal_action_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGABRT, &act, 0);

    if (setjmp(long_jump_reference) == 0)
        doApiChecks();
}

/**
 * It generates a SIG_ABORT with gcc
 */
void doAbort() {
    shared_ptr<NonSharedDataFile> shareNSD = make_shared<NonSharedDataFile>();
    try {
        shared_ptr<NonSharedDataFile> shareNSD2 = shareNSD->getPtr();
    } catch(bad_weak_ptr& e) {
        cout << e.what() << endl;
    }
}

void doApiChecks() {

    // Checking ...
    auto vs = increment(1,2,3,4, 102);
    cout << get<0>(vs) << '\t' << get<1>(vs);
    cout << endl;
    cout << vs << endl;

    // Checking shared_ptr
    //doAbort();
    shared_ptr<DataFile<int>> shareSD = make_shared<DataFile<int>>();
    shared_ptr<DataFile<int>> shareSD2 = shareSD->getptr();
    shared_ptr<DataFile<int>> shareSD3 = shareSD2;
    shared_ptr<DataFile<int>> shareSD8 = shareSD3;
    shared_ptr<DataFile<int>> shareSD9 = shareSD8;
    cout << "Shared pointer referencing " << shareSD2.use_count() << " - " << shareSD3.use_count() << endl;
    {
        weak_ptr<DataFile<int>> shareSD12 = shareSD3;
        shared_ptr<DataFile<int>> shareSD13 = shareSD3;
        cout << "With weak pointer and shared_ptr " << shareSD3.use_count() << " - " << shareSD8.use_count() << endl;
    }
    cout << shareSD3.use_count() << endl;

    // Opening a file to read ints
    using DF = DataFile<int>;
    unique_ptr<DF> dataSource = time_it_supplier<unique_ptr<DF>>("DataFile constructor : ", []() { return make_unique<DF>();});

    const string  dataFile = dataSource.get()->getDataFile();
    cout << "---- " << dataFile << " " << &dataFile << " ";
    cout << sizeof(dataFile.c_str()[0]) << endl;

    using vectorPtr = shared_ptr<vector<int>>;
    vectorPtr dataPtr = time_it_supplier<vectorPtr>("Load file: ",
                                                    [&] () { return dataSource.get()->getDataPtr();});
    vectorPtr  dataPtr2 = dataPtr;
    cout << "Shared pointer count: " << dataPtr2.use_count() << endl;
    vector<int> dataCopy;
    time_it("Copy vector: ",
            [&]() {copy(dataPtr.get()->begin(), dataPtr.get()->end(), back_inserter(dataCopy));});

    cout << "Vector size: " << dataCopy.size() << endl;
    int elements = dataCopy.size() / 10;
    stringstream desc1;
    desc1 << "Copy " << elements << " elements:";

    vector<int> dataCopyReduced;
    time_it(desc1.str(),
            [&]() {copy_n(dataPtr.get()->begin(), elements, back_inserter(dataCopyReduced));});
    for_each_n_print(dataCopyReduced, 20);

    /// Exception double is not implemented
    try {
        vector<double> doubleCheck = {1.2, 3.4};
        for_each_n_print(doubleCheck, 2);
    } catch(...) {
        cout << "Double check okey " << endl;
    }

    /// Basic sort all elements
    stringstream desc2;
    desc2 << "Sort " << elements << " elements:";

    time_it(desc2.str(),
            [&] () {sort(dataCopy.begin(), dataCopy.end(), greater<int>());});
    for_each_n_print(dataCopy, 20);

    /// Copy and  sort all elements in different step size
    std::ofstream ofs;
    ofs.open ("test.txt", std::ofstream::out | std::ofstream::app);

    int step = 10000;
    for(int el_it = step; el_it < elements; el_it += step ) {
        cout << "=================" << endl;
        vector<int> localCopy;
        stringstream desc_iteration;
        desc_iteration.imbue(std::locale(""));
        desc_iteration <<  "Copy " << fixed << el_it << " elements:";

        time_it(desc_iteration.str(),
                [&]() { copy_n(dataPtr->begin(), el_it, back_inserter(localCopy)); });

        for_each_n_print(localCopy, 20);

        desc_iteration.clear();
        desc_iteration.str(string());
        desc_iteration << "Sort " <<  el_it << " elements:";
        time_it(desc_iteration.str(),
                [&]() { sort(localCopy.begin(), localCopy.end(), greater<int>()); });
        cout << "Local copy has " << localCopy.size() << " elements" << endl;
        for_each_n_print(localCopy, 20);
    }

    /// Checking types
    {
        unique_ptr<templatesInst> inst = make_unique<templatesInst>();

        boostInst::checkType();
        unique_ptr<boostInst> boostImpl = make_unique<boostInst>();
        boostImpl->main();

        cl_point start = cl::now();

        cl_point end = cl::now();
        cout << duration_cast<seconds>(end - start).count() << endl;
    }

    /// stdLibInst
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


    cout << "End of main" << endl;

}


