#ifndef P2GAME_ITEMBASE_H
#define P2GAME_ITEMBASE_H

// Ticket T2a / FR-14..FR-18 (ADR 0006): the abstract base class of all
// items. Every item carries a name, a description, a weight (> 0, kg)
// and a value/price; describe() renders it for console output. The
// class is abstract through the pure virtual describe() (FR-14).

#include <string>

namespace p2game {

class ItemBase {
public:
    virtual ~ItemBase() = default;

    // FR-15: weight must be strictly positive (kg); weight <= 0
    // (negative OR zero) throws std::invalid_argument. Validated in
    // the constructor body so the throw actually happens.
    ItemBase(std::string name, std::string description, double weight, double value);

    // FR-16..FR-18 accessors: they return exactly what was passed at
    // construction.
    const std::string& name() const;
    const std::string& description() const;
    double weight() const;
    double value() const;

    // FR-14: renders the item for console output; pure virtual, so
    // ItemBase cannot be instantiated directly. The override returns
    // non-empty text containing the item's name.
    virtual std::string describe() const = 0;

private:
    std::string name_;
    std::string description_;
    double weight_;
    double value_;
};

}  // namespace p2game

#endif  // P2GAME_ITEMBASE_H
