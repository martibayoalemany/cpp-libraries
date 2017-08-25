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
    virtual void notify() = 0;
};

class FruitImpl : public Fruit
{
    Notifier* notifierImpl;
    int n;

public:

    INJECT(FruitImpl(ANNOTATED(NotifierImpl, Notifier*) notifierImpl )) : notifierImpl(notifierImpl) {

    }

    ~FruitImpl()  {

    };

    void notify() override {
        //fruit::NormalizedComponent<Listener> normComp(getListenerComponent);
        //fruit::Injector<Listener> injector(getListenerComponent);
        //const std::vector<Listener *> &listeners = injector.getMultibindings<Listener>();
        const vector<Listener *> listeners;
        if(notifierImpl != nullptr)
            notifierImpl->notify(listeners);
        else
            cout << "Notifier not injected " << n << endl;
    }


};





#endif //CPP_LIBRARIES_FRUITINST_H
