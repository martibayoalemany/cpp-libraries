
#include "boostInst.h"

#include <sstream>
#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <memory>

using namespace std;
using namespace boost::accumulators;
void boostInst::main() {

    accumulator_set<double, stats<tag::mean, tag::moment<2>, tag::median >> acc;

    vector<double> vec = {1.2,2.2,3.3,823232,2323,2323,23232};
    using Iter = vector<double>::iterator;
    for(Iter it = vec.begin(); it != vec.end(); it++) {
        acc(*it);
    }

    acc(1.2);
    acc(2.3);
    acc(3.4);
    acc(4.5);
    cout << "Mean: " << mean(acc) << endl;
    cout << "Moment: " << moment<2>(acc) << endl;
    cout << "Median: " << median(acc) << endl;

}

void boostInst::checkType() {
    unique_ptr<boostInst> car = unique_ptr<boostInst> {};
    stringstream sstr;
    sstr << "------------------- " << endl;
    sstr << "Copy assign. " << is_copy_assignable<boostInst>() << endl;
    sstr << "Copy constr. " << is_copy_constructible<boostInst>() << endl;
    sstr << "Triv. copy constr. " << is_trivially_copy_constructible<boostInst>() << endl;
    sstr << "No throw copy constr. " << is_nothrow_copy_constructible<boostInst>() << endl;
    sstr << "Move constr. " << is_move_constructible<boostInst>() << endl;
    sstr << "Triv. move constr. " << is_trivially_move_constructible<boostInst>() << endl;
    sstr << "No throw move constr. " << is_nothrow_move_constructible<boostInst>() << endl;
    cout << sstr.str() << endl;
}