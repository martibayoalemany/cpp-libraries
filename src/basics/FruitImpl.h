#ifndef CPP_LIBRARIES_FRUITINST_H
#define CPP_LIBRARIES_FRUITINST_H

#include <iostream>
#include <fruit/fruit.h>
#include <fruit/component.h>
#include <fruit/injector.h>

using namespace std;


class Listener {
public:
    virtual void notify() = 0;

};

class AListener : public Listener {
public:
    void notify();
};

class SomeOtherListener : public Listener {
public:
    void notify() override;
};

class Notifier {
public:
    virtual void notify(vector<Listener*> listeners) = 0;
};

class NotifierImpl : public Notifier {
public:
    INJECT(NotifierImpl()) = default;

    void notify(vector<Listener*> listeners) override {
        if(listeners.empty())
            cout << "No listeners to notify" << endl;
        for(Listener* listener : listeners)
            listener->notify();
    }
};

class Fruit {
public:
    Fruit() = default;
    virtual void notify() = 0;
};

class FruitImpl : public Fruit
{
    Notifier* notifier;
    int n;

public:
    FruitImpl(Notifier* notifier, int n) : notifier(notifier), n(n) {

    };

    using Inject = FruitImpl(Notifier*, fruit::Assisted<int>);


    ~FruitImpl()  {

    };

    void notify() {
        //fruit::NormalizedComponent<Listener> normComp(getListenerComponent);
        //fruit::Injector<Listener> injector(getListenerComponent);
        //const std::vector<Listener *> &listeners = injector.getMultibindings<Listener>();
        const vector<Listener *> listeners;
        if(notifier != nullptr)
            notifier->notify(listeners);
        else
            cout << "Notifier not injected " << n << endl;
    }


};





#endif //CPP_LIBRARIES_FRUITINST_H
