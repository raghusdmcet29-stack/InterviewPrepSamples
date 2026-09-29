//
//  main.c
//  InterpreterPattern
//
//  Created by Anussha on 24/09/26.
//

#include <stdio.h>
#include <stdlib.h>

// Discriminator: which kind of node this is
typedef enum {
    EXPR_NUMBER,
    EXPR_ADD,
    EXPR_SUBTRACT
} ExpressionType;

// Forward-declare so Expression can reference itself via pointer (for children)
typedef struct Expression Expression;

struct Expression {
    ExpressionType type;
    union {
        int value;                       // used when type == EXPR_NUMBER
        struct {                         // used when type == EXPR_ADD or EXPR_SUBTRACT
            Expression *left;
            Expression *right;
        } binary;
    } data;
};

// Constructor: create a number leaf node
Expression *createNumberExpression(int value) {
    Expression *expr = malloc(sizeof(Expression));
    expr->type = EXPR_NUMBER;
    expr->data.value = value;
    return expr;
}

// Constructor: create an add node, taking ownership of left/right
Expression *createAddExpression(Expression *left, Expression *right) {
    Expression *expr = malloc(sizeof(Expression));
    expr->type = EXPR_ADD;
    expr->data.binary.left = left;
    expr->data.binary.right = right;
    return expr;
}

// Constructor: create a subtract node, taking ownership of left/right
Expression *createSubtractExpression(Expression *left, Expression *right) {
    Expression *expr = malloc(sizeof(Expression));
    expr->type = EXPR_SUBTRACT;
    expr->data.binary.left = left;
    expr->data.binary.right = right;
    return expr;
}

// Evaluate: switch on type, recurse into children for Add/Subtract
int evaluate(const Expression *expr) {
    switch (expr->type) {
        case EXPR_NUMBER:
            return expr->data.value;
        case EXPR_ADD:
            return evaluate(expr->data.binary.left) + evaluate(expr->data.binary.right);
        case EXPR_SUBTRACT:
            return evaluate(expr->data.binary.left) - evaluate(expr->data.binary.right);
    }
    return 0; // unreachable, but keeps compiler happy about all paths returning a value
}

// Recursively free a node and all its children
void freeExpression(Expression *expr) {
    if (expr == NULL) {
        return;
    }
    if (expr->type == EXPR_ADD || expr->type == EXPR_SUBTRACT) {
        freeExpression(expr->data.binary.left);
        freeExpression(expr->data.binary.right);
    }
    free(expr);
}

int main(void) {
    Expression *five = createNumberExpression(5);
    Expression *three = createNumberExpression(3);
    Expression *two = createNumberExpression(2);

    Expression *addExpr = createAddExpression(five, three);           // 5 + 3
    Expression *subtractExpr = createSubtractExpression(addExpr, two); // (5+3) - 2

    printf("Result: %d\n", evaluate(subtractExpr));

    freeExpression(subtractExpr); // frees subtractExpr, addExpr, five, three, two — all in one call
    return 0;
}
