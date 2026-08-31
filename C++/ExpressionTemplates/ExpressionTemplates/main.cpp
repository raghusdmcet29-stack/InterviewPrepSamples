//
//  main.cpp
//  ExpressionTemplates
//
//  Created by Anussha on 31/08/26.
//

// naive_vector.cpp
#include <iostream>
#include <vector>

/*
//The Naive Problem (proves temporaries exist)

class Vector{
public:
    static int allocationCount;
    
    Vector(size_t size, double fill = 0.0) : data_(size, fill) {
            allocationCount++;
        }
    Vector(const Vector& other) : data_(other.data_){
        allocationCount++;
    }
    double operator[](size_t i) const { return data_[i]; }
    double& operator[](size_t i) { return data_[i]; }
    size_t size() const { return data_.size(); }
    
    // Naive operator+: builds and returns a brand new Vector (heap alloc)
    Vector operator+(const Vector& rhs) const {
        Vector result(size());
        for (size_t i = 0; i < size(); ++i)
            result[i] = data_[i] + rhs[i];
        return result;
     }
private:
    std::vector<double> data_;
};

int Vector::allocationCount = 0;

int main() {
    Vector a(5, 1.0), b(5, 2.0), c(5, 3.0);
    Vector::allocationCount = 0; // reset after ctor calls above

    Vector d = a + b + c;   // a+b creates a temporary, then +c creates another

    std::cout << "d[0] = " << d[0] << "\n";
    std::cout << "Naive allocationCount for (a+b+c): " << Vector::allocationCount << "\n";
    return 0;
}
*/

//Expression Templates (lazy, zero temporaries)
// expr_template_vector.cpp
#include <iostream>
#include <vector>
#include <cstddef>

class Vector {
public:
    static int allocationCount;

    Vector(size_t size, double fill = 0.0) : data_(size, fill) {
        allocationCount++;
    }

    // Construct from ANY expression type (this is where evaluation actually happens)
    template <typename Expr>
    Vector(const Expr& expr) : data_(expr.size()) {
        allocationCount++;
        for (size_t i = 0; i < expr.size(); ++i)
            data_[i] = expr[i];   // pulls values lazily, one pass, no temporaries
    }

    double operator[](size_t i) const { return data_[i]; }
    double& operator[](size_t i) { return data_[i]; }
    size_t size() const { return data_.size(); }

private:
    std::vector<double> data_;
};

int Vector::allocationCount = 0;

// ---- Expression node: represents "LHS + RHS" without computing it ----

template <typename LHS, typename RHS>
class VecSum {
public:
    VecSum(const LHS& lhs, const RHS& rhs) : lhs_(lhs), rhs_(rhs) {}

    // Computed on-demand, per index, only when actually read
    double operator[](size_t i) const { return lhs_[i] + rhs_[i]; }
    size_t size() const { return lhs_.size(); }

private:
    const LHS& lhs_;
    const RHS& rhs_;
};

// operator+ now returns a lightweight expression object, NOT a Vector
template <typename LHS, typename RHS>
VecSum<LHS, RHS> operator+(const LHS& lhs, const RHS& rhs) {
    return VecSum<LHS, RHS>(lhs, rhs);
}

int main() {
    Vector a(5, 1.0), b(5, 2.0), c(5, 3.0),d(5, 4.0);
    Vector::allocationCount = 0; // reset after ctor calls above

    // a + b + c builds a VecSum<VecSum<Vector,Vector>, Vector> — a TYPE, not data.
    // Nothing is computed yet. Computation happens only in the Vector(Expr) ctor below.
    Vector f = a + b + c + d;

    std::cout << "f[0] = " << f[0] << "\n";
    std::cout << "Expression-template allocationCount for (a+b+c+d): "
              << Vector::allocationCount << "\n";
    return 0;
}
