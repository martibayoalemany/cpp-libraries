#ifndef CPP_LIBRARIES_FRUITINST_H
#define CPP_LIBRARIES_FRUITINST_H

#include <fruit/component.h>
#include <fruit/injector.h>

class Listener {
public:
    void notify() = 0;
};

class AListener : Listener {
public:
    void notify();
};

class SomeOtherListener : Listener {
public:
    void notify();
};

class Injector;

class FruitInst : Listener{
private:
    Injector injector;
public:
    fruit::Component<> getListenerComponent();
    void notify();

};


#endif //CPP_LIBRARIES_FRUITINST_H
