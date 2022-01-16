#ifndef FUZZY_H
#define FUZZY_H

#include <algorithm>
#include <compare>
#include <set>

typedef double real_t;

class TriFuzzyNum {
public:
    TriFuzzyNum() = delete;
    constexpr TriFuzzyNum(real_t a, real_t b, real_t c) : l(a), m(b), u(c) {
        restore_order();
    }

    // Konstruktory copy i move są generowanane przez kompilator.
    // Operator przypisania jest generowany przez kompilator.

    constexpr real_t lower_value() const { return l; }
    constexpr real_t modal_value() const { return m; }
    constexpr real_t upper_value() const { return u; }

    TriFuzzyNum& operator+=(const TriFuzzyNum &other);
    TriFuzzyNum& operator-=(const TriFuzzyNum &other);
    TriFuzzyNum& operator*=(const TriFuzzyNum &other);

    const TriFuzzyNum operator+(const TriFuzzyNum &other) const;
    const TriFuzzyNum operator-(const TriFuzzyNum &other) const;
    const TriFuzzyNum operator*(const TriFuzzyNum &other) const;

    constexpr bool operator==(const TriFuzzyNum &other) const {
        return l == other.lower_value() &&
               m == other.modal_value() &&
               u == other.upper_value();
    }
    std::partial_ordering operator<=>(const TriFuzzyNum &other) const;

    friend std::ostream& operator<<(std::ostream &os, const TriFuzzyNum &num);

private: 
	real_t l;
    real_t m;
    real_t u;

    constexpr void restore_order() {
        if (l > m) std::swap(l, m);
        if (l > u) std::swap(l, u);
        if (m > u) std::swap(m, u);
    }
};

class TriFuzzyNumSet {
public:
    TriFuzzyNumSet() : fnum_set() {}
    TriFuzzyNumSet(std::initializer_list<TriFuzzyNum> list) : fnum_set(list) {}

    // Konstruktory copy i move są generowanane przez kompilator.
    // Operator przypisania jest generowany przez kompilator.

    void insert(TriFuzzyNum const &fnum);
    void insert(TriFuzzyNum &&fnum);
    void remove(TriFuzzyNum fnum);

    TriFuzzyNum arithmetic_mean() const;

private:
    std::multiset<TriFuzzyNum> fnum_set;
};

consteval TriFuzzyNum crisp_number(real_t v) { return TriFuzzyNum(v, v, v); }

constinit const TriFuzzyNum crisp_zero = crisp_number(0);

#endif /* FUZZY_H */

