#ifndef P2GAME_MAPGENERATOR_H
#define P2GAME_MAPGENERATOR_H

// Ticket T4 / ADR 0003: the map generator. Carve-then-randomize:
// a seeded random walk with monotonically non-decreasing row carves a
// guaranteed start->exit path (Traversable), the remaining cells are
// randomized against p_blocked, and the Start/Exit kinds are pinned on
// the carved endpoints. The walk is a bounded random walk, NOT a
// pathfinding algorithm (assignment 4a separation rule); the generator
// contains no search-from-start behaviour.
//
// Seeding: a fresh std::mt19937 is derived from the seed inside every
// generate() call, so the generator keeps no mutable RNG state -- the
// same seed reproduces the identical map on the same instance and on a
// fresh instance alike (FR-12).

#include <cstddef>

#include "Map.h"
#include "MapGeneratorConfig.h"

namespace p2game {

class MapGenerator {
public:
    // Default configuration: 15x15, p_blocked 0.3 (FR-2, FR-10).
    MapGenerator();

    // Explicit configuration. Throws std::invalid_argument when
    // rows/cols are below the FR-2 minimum (15x15).
    explicit MapGenerator(const MapGeneratorConfig& config);

    // Generates a complete, ready-to-use Map from the configured
    // dimensions. Deterministic in (config, seed).
    Map generate(unsigned seed) const;

private:
    MapGeneratorConfig config_;
};

}  // namespace p2game

#endif  // P2GAME_MAPGENERATOR_H
