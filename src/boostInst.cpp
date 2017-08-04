//
// Created by username on 04.08.17.
//

#include "boostInst.h"
#include <boost>
#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/moment.hpp>

using namespace std;
using namespace boost::accumulators;
void boostInst::main() {

    accumulator_set<double, stats<tag::mean, tag::moment<2>>> acc;
    array<int,20> vec = {1,2,3};
    for_each(vec.begin(), vec.end(), [=] (int value) {acc(value);});
    cout << "Mean: " << mean(acc) << endl;
    cout << "Moment: " << moment(acc) << endl;
    cout << "Median: " << median(acc) << endl;

}
