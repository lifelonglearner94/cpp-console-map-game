#include "ItemBase.h"

// Ticket T2a: definitions for the abstract item base class (ADR 0006,
// FR-14..FR-18).

#include <stdexcept>
#include <utility>

namespace p2game {

ItemBase::ItemBase(std::string name, std::string description, double weight, double value)
    : name_(std::move(name)),
      description_(std::move(description)),
      weight_(weight),
      value_(value) {
    // FR-15: weight must be strictly positive (kg). Checked in the
    // constructor body (not the member-init list) so the throw happens
    // before the object can be used.
    if (weight_ <= 0.0) {
        throw std::invalid_argument("ItemBase: weight must be > 0 (kg)");
    }
}

const std::string& ItemBase::name() const {
    return name_;
}

const std::string& ItemBase::description() const {
    return description_;
}

double ItemBase::weight() const {
    return weight_;
}

double ItemBase::value() const {
    return value_;
}

}  // namespace p2game
