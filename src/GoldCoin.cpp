#include "GoldCoin.h"

// Ticket T2a: definitions for the GoldCoin item (FR-19, ADR 0006).

#include <sstream>
#include <string>
#include <utility>

namespace p2game {

GoldCoin::GoldCoin()
    : ItemBase("Gold coin", "A shiny gold coin", 0.02, 1.0) {}

GoldCoin::GoldCoin(std::string name, std::string description, double weight, double value)
    : ItemBase(std::move(name), std::move(description), weight, value) {}

std::string GoldCoin::describe() const {
    std::ostringstream stream;
    stream << name() << " (" << description() << "), " << weight()
           << " kg, worth " << value() << " gold";
    return stream.str();
}

}  // namespace p2game
