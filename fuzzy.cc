#include "fuzzy.h"
#include <cmath>
#include <iostream>
#include <tuple>

using namespace std;

namespace {
    typedef tuple<real_t, real_t, real_t> rank_t;

    // Pomocnicza funckja służąca obliczaniu rangi liczby rozmytej.

    rank_t fuzzy_rank(const TriFuzzyNum& num) {

        real_t l = num.lower_value();
        real_t m = num.modal_value();
        real_t u = num.upper_value();

        real_t z = (u - l) + 
                    sqrt(1 + (u - m) * (u - m)) + 
                    sqrt(1 + (m - l) * (m - l));
        real_t x = ((u - l) * m + 
                    sqrt(1 + (u - m) * (u - m)) * l + 
                    sqrt(1 + (m - l) * (m - l)) * u) / z;
        real_t y = (u - l) / z;

        return {x - y / 2, 1 - y, m};
    }
}

// Operatory arytmetyczne przypisania.

TriFuzzyNum& TriFuzzyNum::operator+=(const TriFuzzyNum &other) {
    l += other.lower_value();
    m += other.modal_value();
    u += other.upper_value();
    return *this;
}

TriFuzzyNum& TriFuzzyNum::operator-=(const TriFuzzyNum &other) {
    l -= other.upper_value();
    m -= other.modal_value();
    u -= other.lower_value();
    restore_order();
    return *this;
}

TriFuzzyNum& TriFuzzyNum::operator*=(const TriFuzzyNum &other) {
    l *= other.lower_value();
    m *= other.modal_value();
    u *= other.upper_value();
    restore_order();
    return *this;
}

// Operatory arytmetyczne binarne.

const TriFuzzyNum TriFuzzyNum::operator+(const TriFuzzyNum &other) const {
    return TriFuzzyNum(*this) += other;
}

const TriFuzzyNum TriFuzzyNum::operator-(const TriFuzzyNum &other) const {
    return TriFuzzyNum(*this) -= other;
}

const TriFuzzyNum TriFuzzyNum::operator*(const TriFuzzyNum &other) const {
    return TriFuzzyNum(*this) *= other;
}

// Operator porównania.

partial_ordering TriFuzzyNum::operator<=>(const TriFuzzyNum &other) const {
    return fuzzy_rank(*this) <=> fuzzy_rank(other);
}

ostream& operator<<(ostream &os, const TriFuzzyNum &num) {
    os  << "(" << num.lower_value() << ", " << num.modal_value() << ", " 
        << num.upper_value() << ")";
    return os;
}

// Wstawianie/usuwanie liczb z kolekcji.

void TriFuzzyNumSet::insert(TriFuzzyNum const &fnum) {
    fnum_set.emplace(fnum);
}

void TriFuzzyNumSet::insert(TriFuzzyNum &&fnum) {
    fnum_set.insert(move(fnum));
}

void TriFuzzyNumSet::remove(TriFuzzyNum fnum) {
    auto idx = fnum_set.find(fnum);
    if (idx != fnum_set.end())
        fnum_set.erase(idx);
}

TriFuzzyNum TriFuzzyNumSet::arithmetic_mean() const {
    real_t avg_l = 0;
    real_t avg_m = 0;
    real_t avg_u = 0;

    if (fnum_set.empty()) {
        throw length_error("TriFuzzyNumSet::arithmetic_mean - the set is empty.");
    }

    for (auto val : fnum_set) {
        avg_l += val.lower_value();
        avg_m += val.modal_value();
        avg_u += val.upper_value();
    }
    
    avg_l /= fnum_set.size();
    avg_m /= fnum_set.size();
    avg_u /= fnum_set.size();

    return {avg_l, avg_m, avg_u};
}

