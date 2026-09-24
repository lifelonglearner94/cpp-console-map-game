#ifndef P2GAME_GOLDCOIN_H
#define P2GAME_GOLDCOIN_H

// Ticket T2a / FR-19 (ADR 0006): the first concrete ItemBase child —
// the Aufgabe 1c gold coin.

#include "ItemBase.h"

#include <string>

namespace p2game {

class GoldCoin : public ItemBase {
public:
    // Default-constructed canonical coin: valid non-empty
    // name/description, weight > 0, value > 0.
    GoldCoin();

    // Explicit attributes, forwarded to the ItemBase constructor
    // (weight <= 0 throws std::invalid_argument, FR-15).
    GoldCoin(std::string name, std::string description, double weight, double value);

    // Renders the coin for console output; non-empty text containing
    // name().
    std::string describe() const override;
};

}  // namespace p2game

#endif  // P2GAME_GOLDCOIN_H
