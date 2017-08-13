#ifndef CPP_LIBRARIES_BOOSTDI_H
#define CPP_LIBRARIES_BOOSTDI_H


#include <assert.h>

class ctor {
public:
    explicit ctor(int i) : i(i) {}
    int i;
};

struct aggregate {
    double d;
};

class example {
    example(aggregate a, const ctor& c) {
    };
};

class BoostDi {
private:
    BoostDi() = delete;
public:
    static void execute();
};

#endif //CPP_LIBRARIES_BOOSTDI_H
