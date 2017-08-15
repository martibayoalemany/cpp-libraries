#include "FruitInst.h"
#include <fruit/injector.h>
#include <iostream>

using namespace std;

fruit::Component<> FruitInst::getListenerComponent() {
    return fruit::createComponent()
            .addMultibinding<Listener, AListener>()
            .addMultibinding<Listener, SomeOtherListener>();
}

void FruitInst::notify() {
    const std::vector<Listener *> &listeners = injector.getMultibindings<Listener>();
    for (Listener *listener : listeners) {
        listener->notify();
    }
}

void AListener::notify() {
    cout << " AListener notified " <<endl;
}

void SomeOtherListener::notify() {
    Listener::notify();
}
