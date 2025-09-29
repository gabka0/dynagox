#ifndef RINGMAX_HPP
#define RINGMAX_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

struct RingMax {
    double value;

    static RingMax zero;

    explicit RingMax() : value(0.0) { }

    explicit RingMax(double value) : value(value) { }

    inline bool isZero() const { return value == 0; }

    RingMax& operator+=(const RingMax &r) {
        if (r.value > this->value) {
            this->value = r.value;
        }
        return *this;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, value);
    }
};

RingMax operator*(int alpha, const RingMax &r) {
    return RingMax(r.value);
}

RingMax operator*(long alpha, const RingMax &r) {
    return RingMax(r.value);
}

RingMax operator*(const RingMax &l, long alpha) {
    return RingMax(l.value);
}

RingMax operator*(const RingMax &l, const RingMax &r) {
    return RingMax(l.value * r.value);
}

RingMax operator+(const RingMax &l, const RingMax &r) {
    return RingMax(l.value + r.value);
}

RingMax Ulift(double a) {
    return RingMax(a);
}

#endif