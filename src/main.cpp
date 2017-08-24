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
#include "utils/FileUtils.h"
#include "basics/FruitImpl.h"

using namespace std;
using namespace chrono;

using cl = chrono::high_resolution_clock;
using cl_point = chrono::high_resolution_clock::time_point;

jmp_buf long_jump_reference;

volatile sig_atomic_t signal_handling_result = 0;

/**
 * It printf the signal received and sets the long_jump_reference to 1 so reentry does not occur
 */
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

/**
 * Measures the time in milliseconds of a <b>function with no parameters</b>
 */
int time_it(string desc, function<void()> funct) {
    cl_point start = cl::now();
    funct();
    cl_point end = cl::now();
    int ms = duration_cast<milliseconds>(end - start).count();
    cout << desc << " " << ms  << " ms " << endl;
    return ms;
}

/**
 * Measures the time in milliseconds of a <b>function with no parameters and return value T</b>
 */
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
void for_each_n_print(vector<T>& v, size_t size) {
    // Not implemented exception
    auto exc = std::make_exception_ptr(new custom_exception());
    std::rethrow_exception(exc);
}

/**
 * It prints all elements of a vector
 */
template<>
void for_each_n_print<int>(vector<int>& v, size_t size) {
    using Iter = vector<int>::iterator;
    int i = 0;
    for(Iter it = v.begin(); i < size; it++, i++)
        cout << *it << '\t';
    cout << endl;
    cout << "=======" << endl;
}

/**
 * Increments one all element contained in the ellipsis
 */
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

void doApiChecks();

template<class T>
fruit::Component<T> getFruitComponent() {
    return fruit::createComponent()
            //.addMultibinding<Listener, AListener>()
            //.addMultibinding<Listener, SomeOtherListener>()
            //.bind<Notifier, NotifierImpl>()
            .bind<Fruit, FruitImpl>();
}

int main(int argc, char **argv) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = signal_action_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGABRT, &act, 0);

    if (setjmp(long_jump_reference) == 0) {
        doApiChecks();

        // TODO: fix linkage errors with fruit
        /*
        {
            fruit::Injector<Fruit> injector(getFruitComponent<Fruit>());
            Fruit* fruit = injector.get<Fruit*>();
            fruit->notify();
        }
         */
    }
}

void doApiChecks() {

    /// Checking iterator ...
    {
        map<int, char> m = {{10, 'c'},
                            {20, 'd'},
                            {34, 'e'},
                            {50, 'e'},
                            {60, 'e'},
                            {80, 'e'}};
        const int bound = 60;
        auto iterator = m.lower_bound(30);
        cout << iterator->first << " " << iterator->second << endl;
        iterator++;
        cout << iterator->first << " " << iterator->second << endl;
    }

    /// Checking templated elipsis ...
    auto vs = increment(1, 2, 3, 4, 102);
    cout << get<0>(vs) << '\t' << get<1>(vs);
    cout << endl;
    cout << vs << endl;

    /// Checking shared_ptr pointer for a non shared class
    shared_ptr<NonSharedDataFile> shared1 = make_shared<NonSharedDataFile>();
    try {
        shared_ptr<NonSharedDataFile> shared2 = shared1;
        cout << "doCheckSharedPtr " << shared1.use_count()  << " - " << shared2.use_count()<< endl;
    } catch(bad_weak_ptr& e) {
        cout << e.what() << endl;
    }

    /// Checking shared_ptr.use_count() and weak pointer
    weak_ptr<DataFile<int>> weakPtr;
    {
        auto sharedPtr = make_shared<DataFile<int>>();
        auto sharedPtr1 = sharedPtr;
        auto sharedPtr2 = sharedPtr1;
        cout << "Shared pointer referencing (3) <=> " << sharedPtr.use_count() << endl;
        auto sharedPtr3 = sharedPtr2;
        auto sharedPtr5 = sharedPtr3;
        auto sharedPtr8 = sharedPtr5;
        weakPtr = sharedPtr;
        cout << "Shared pointer referencing (6) <=> " << sharedPtr.use_count() << endl;

        if(auto tmp = weakPtr.lock())
            cout << tmp->getDataFile() << endl;
        else
            cout << "Weak pointer expired";

        sharedPtr.reset();
        cout << "Shared pointer referencing  =? " << sharedPtr.use_count() << endl;
        cout << "Shared pointer referencing 3  =? " << sharedPtr3.use_count() << endl;
    }

    if(!weakPtr.expired())
        cout << weakPtr.lock().use_count() << endl;
    else
        cout << "Weak pointer expired";

    /// Read ints and timeit
    using DF = DataFile<int>;
    unique_ptr<DF> dataSource = time_it_supplier<unique_ptr<DF>>("DataFile constructor : ",
                                                                 []() { return make_unique<DF>();});

    const string dataFile = dataSource->getDataFile();
    cout << "---- " << dataFile << " " << &dataFile << " ";
    cout << sizeof(dataFile.c_str()[0]) << endl;

    using vectorPtr = shared_ptr<vector<int>>;
    vectorPtr dataPtr = time_it_supplier<vectorPtr>("Load file: ",
                                                    [&] () { return dataSource.get()->getDataPtr();});

    vector<int> dataCopy;
    time_it("Copy vector: ",
            [&]() {copy(dataPtr->begin(), dataPtr->end(), back_inserter(dataCopy));});

    cout << "Vector size: " << dataCopy.size() << endl;
    int elements = dataCopy.size();
    stringstream desc1;
    desc1 << "Copy " << elements << " elements:";

    vector<int> dataCopyReduced;
    time_it(desc1.str(),
            [&]() {copy_n(dataPtr->begin(), elements, back_inserter(dataCopyReduced));});
    for_each_n_print(dataCopyReduced, 20);

    /// for_each_n_print<double> is not implemented
    try {
        vector<double> doubleCheck = {1.2, 3.4};
        for_each_n_print(doubleCheck, 2);
    } catch(...) {
        cout << "for_each_n_print<double> is not implemented" << endl;
    }

    /// for_each_n_print<string> is not implemented
    try {
        vector<string> stringCheck = {"adsad", "asdasd"};
        for_each_n_print(stringCheck, 2);
    }
    catch (const exception& exc) {
        cout << "catch (exception& exc) " << exc.what() << endl;
    }
    catch(custom_exception& exc) {
        cout << "catch (custom_exception&) " << exc.what() << endl;
    }
    catch(const custom_exception* exc) {
        cout << "catch (custom_exception*) " << exc->what() << endl;
    }

    /***
     Sort all elements
     */
    stringstream desc2;
    elements = dataCopy.size();
    desc2 << "Sort " << elements << " elements:";

    time_it(desc2.str(),
            [&] () {sort(dataCopy.begin(), dataCopy.end(), greater<int>());});
    for_each_n_print(dataCopy, 20);

    /// open a file for statistics
    unique_ptr<FileUtils> fu = make_unique<FileUtils>();
    bool has_stats = fu->openStats();

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
        int sorted_in_ms = time_it(desc_iteration.str(),
                [&]() { sort(localCopy.begin(), localCopy.end(), greater<int>()); });

        if(has_stats)
          fu->writeStats(string("sort"), el_it, sorted_in_ms);
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


