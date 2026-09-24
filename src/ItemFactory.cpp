#include "ItemFactory.h"

// Ticket T2a: definitions for the seeded item catalog (ADR 0006).

#include "GoldCoin.h"

#include <cstddef>
#include <random>

namespace p2game {

ItemFactory::ItemFactory() {
    // Small fixed catalog: GoldCoin variants with varied attributes.
    // Kept self-contained on purpose — later tickets (equipment,
    // chests) extend it. Every entry constructs with weight > 0 and
    // non-empty name/description. The first entry is built from the
    // default-constructed canonical GoldCoin, so the canonical coin
    // attributes live in exactly one place (GoldCoin's default
    // constructor); the other three entries stay literals.
    const GoldCoin canonical;
    catalog_.push_back({canonical.name(), canonical.description(),
                        canonical.weight(), canonical.value()});
    catalog_.push_back({"Ducat", "A small gold ducat", 0.03, 3.0});
    catalog_.push_back({"Crown", "A heavy golden crown", 0.5, 25.0});
    catalog_.push_back({"Ring", "A plain gold ring", 0.05, 5.0});
}

std::unique_ptr<ItemBase> ItemFactory::create(std::mt19937& rng) {
    std::uniform_int_distribution<std::size_t> pick(0, catalog_.size() - 1);
    const CatalogEntry& entry = catalog_[pick(rng)];
    return std::make_unique<GoldCoin>(entry.name, entry.description,
                                      entry.weight, entry.value);
}

}  // namespace p2game
