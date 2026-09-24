// Ticket T6: a thin main for the Aufgabe 1b console demo (ADR 0004).
// All demo behaviour lives in run_demo so tests can drive it through a
// plain ostream; main only wires it to std::cout.

#include <iostream>

#include "Demo.h"

int main() {
    return p2game::run_demo(std::cout);
}
