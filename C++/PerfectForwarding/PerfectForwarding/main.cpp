//
//  main.cpp
//  PerfectForwarding
//
//  Created by Anussha on 28/08/26.
//

#include <iostream>
#include <utility>

class Widget {
public:
    Widget() { std::cout << "Default constructor\n"; }
    Widget(const Widget&) { std::cout << "Copy constructor\n"; }
    Widget(Widget&&) noexcept { std::cout << "Move constructor\n"; }
};

template<typename T>
Widget makeWidgetBroken(T&& arg) {
    return Widget(arg); // BUG: just passes arg directly, no forwarding
}

template<typename T>
Widget makeWidgetForwarded(T&& arg) {
    return Widget(std::forward<T>(arg));
}

void takesRvalueOnly(Widget&& w) {
    std::cout << "Got an rvalue-only param\n";
}

int main() {
    Widget w1;
    Widget w2 = w1;            // should print Copy constructor
    Widget w3 = std::move(w1); // should print Move constructor
    Widget src;
    std::cout << "--- calling with lvalue ---\n";
    Widget r1 = makeWidgetBroken(src);
    std::cout << "--- calling with rvalue (std::move) ---\n";
    Widget r2 = makeWidgetBroken(std::move(src));
    Widget src2;
    std::cout << "--- forwarded, lvalue ---\n";
    Widget r3 = makeWidgetForwarded(src2);
    std::cout << "--- forwarded, rvalue ---\n";
    Widget r4 = makeWidgetForwarded(std::move(src2));
    
    Widget src3;
    //takesRvalueOnly(src3); // try this — should fail to compile
    return 0;
}
