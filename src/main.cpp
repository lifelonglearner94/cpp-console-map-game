#include <iostream>

namespace p2game {

int run_demo() {
    std::cout << "p2game increment 1 demo\n";
    return 0;
}

}  // namespace p2game

int main() {
    return p2game::run_demo();
}
