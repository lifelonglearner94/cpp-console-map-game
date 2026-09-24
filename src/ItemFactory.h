#ifndef P2GAME_ITEMFACTORY_H
#define P2GAME_ITEMFACTORY_H

// Ticket T2a (ADR 0006): the seeded, deterministic item catalog. Draws
// items from a fixed catalog using only a caller-supplied std::mt19937,
// so a given rng state always yields the same item sequence (same seed
// -> same items, different seed -> different items). Feeds FR-21
// (MapGenerator item placement) and FR-43 (demo items) in later
// tickets.

#include "ItemBase.h"

#include <memory>
#include <random>
#include <string>
#include <vector>

namespace p2game {

class ItemFactory {
public:
    // Builds the fixed catalog.
    ItemFactory();

    // Draws the next item from the catalog using only the passed rng
    // (no hidden global state, no time/random_device): deterministic
    // for a given rng state.
    std::unique_ptr<ItemBase> create(std::mt19937& rng);

private:
    // Helper record type for one catalog entry (the permitted helper
    // exception to one-class-per-header).
    struct CatalogEntry {
        std::string name;
        std::string description;
        double weight;
        double value;
    };

    std::vector<CatalogEntry> catalog_;
};

}  // namespace p2game

#endif  // P2GAME_ITEMFACTORY_H
