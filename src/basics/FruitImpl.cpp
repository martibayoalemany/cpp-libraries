#include "FruitImpl.h"
#include <fruit/injector.h>
#include <iostream>

using namespace std;
using namespace fruit;



void AListener::notify() {
    cout << " AListener notified " <<endl;
}


void SomeOtherListener::notify() {
    cout << " SomeOther notified " <<endl;
}

