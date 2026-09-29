//
//  main.cpp
//  InterpreterPattern
//
//  Created by Anussha on 24/09/26.
//

#include <iostream>
#include <memory>

// Abstract base: every expression node must implement evaluate()

class Expression {
public:
    virtual int evaluate() const = 0;
    virtual ~Expression() = default;
};

// Leaf node: wraps a literal number
class NumberExpression : public Expression{
public:
    NumberExpression(int value) : value(value){}
    
    int evaluate() const override{
        return value;
    }
private:
    int value;
};

// Composite node: owns two sub-expressions, adds their evaluated results
class AddExpression : public Expression{
public:
    AddExpression(std::unique_ptr<Expression>left,std::unique_ptr<Expression>right)
    : left(std::move(left)),right(std::move(right)){}
    
    int evaluate() const override{
        return left->evaluate() + right->evaluate();
    }
private:
    std::unique_ptr<Expression>left;
    std::unique_ptr<Expression>right;
};

// Composite node: owns two sub-expressions, subtracts their evaluated results
class SubtractExpression : public Expression{
public:
    SubtractExpression(std::unique_ptr<Expression>left,std::unique_ptr<Expression>right)
    : left(std::move(left)),right(std::move(right)){}
    
    int evaluate() const override {
        return left->evaluate() - right->evaluate();
    }
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
};


int main() {
    auto five = std::make_unique<NumberExpression>(5);
    auto three = std::make_unique<NumberExpression>(3);
    auto two = std::make_unique<NumberExpression>(2);

    auto addExpr = std::make_unique<AddExpression>(std::move(five), std::move(three));       // 5 + 3
    auto subtractExpr = std::make_unique<SubtractExpression>(std::move(addExpr), std::move(two)); // (5+3) - 2

    std::cout << "Result: " << subtractExpr->evaluate() << std::endl;

    return 0;
}
