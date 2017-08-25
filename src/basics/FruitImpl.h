#ifndef CPP_LIBRARIES_FRUITINST_H
#define CPP_LIBRARIES_FRUITINST_H

#include <iostream>
#include <fruit/fruit.h>
#include <fruit/component.h>
#include <fruit/injector.h>

using namespace std;


class IListener {
public:
    virtual void notify() = 0;

};

class AListener : public IListener {
public:
    INJECT(AListener()) = default;
    void notify() override;
};

class SomeOtherListener : public IListener {
public:
    INJECT(SomeOtherListener()) = default;
    void notify() override;
};

class Notifier {
public:
    virtual void notify(IListener* listener) = 0;
};


class NotifierImpl : public Notifier {
    vector<IListener*> listeners;
    IListener* listenerImpl;
public:

    /*
    INJECT(NotifierImpl(vector<IListener*> listeners)) : listeners(listeners) {

    };
     */

    INJECT(NotifierImpl(IListener* listenerImpl)) : listenerImpl(listenerImpl) {

    };

    void notify(IListener* listener) override {
        if(listenerImpl != nullptr)
            listenerImpl->notify();

        for(IListener* listener : listeners)
            listener->notify();

        if(listener != nullptr)
            listener->notify();
        else
            cout << "No more listeners to notify" << endl;
    }
};


struct DefaultNotifier {};


class Fruit {
public:
    virtual void notify() = 0;
};

class FruitImpl : public Fruit
{
    Notifier* notifierImpl;
    IListener* listenerImpl;
    vector<IListener*> listenersImpl;
    int n;

public:

    /*
    INJECT(FruitImpl(ANNOTATED(NotifierImpl, Notifier*) notifierImpl )) : notifierImpl(notifierImpl) {

    }*/

    /*
    INJECT(FruitImpl(Notifier* notifierImpl, IListener* listenerImpl, ASSISTED(int) n)) :
            notifierImpl(notifierImpl), listenerImpl(listenerImpl), n(n) {

    }*/

    INJECT(FruitImpl(Notifier* notifierImpl, IListener* listenerImpl)) :
            notifierImpl(notifierImpl), listenerImpl(listenerImpl), n(n) {

    }

    virtual ~FruitImpl()  {

    };

    void notify() override {

        if(notifierImpl != nullptr)
            notifierImpl->notify(listenerImpl);
        else
            cout << "Notifier not injected " << n << endl;
    }


};


template<class T>
fruit::Component<T> getFruitComponent() {
    return fruit::createComponent()
            .addMultibinding<IListener, AListener>()
            .addMultibinding<IListener, SomeOtherListener>()
            .bind<IListener, SomeOtherListener>()
            .bind<Notifier, NotifierImpl>()
            .bind<Fruit, FruitImpl>();
}




#endif //CPP_LIBRARIES_FRUITINST_H
