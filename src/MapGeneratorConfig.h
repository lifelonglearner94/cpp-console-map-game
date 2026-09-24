#ifndef P2GAME_MAPGENERATORCONFIG_H
#define P2GAME_MAPGENERATORCONFIG_H

// Ticket T4 / FR-2: generation configuration. Own header so the
// one-class-per-header rule (ADR 0005) holds while the test's single
// "MapGenerator.h" include still provides both types.

#include <cstddef>

namespace p2game {

// FR-9: the minimum map size the generator accepts. Rows/cols below
// this are rejected with std::invalid_argument in MapGenerator's
// config-taking constructor.
inline constexpr std::size_t kMinMapRows = 15;
inline constexpr std::size_t kMinMapCols = 15;

// Tunables of the carve-then-randomize generation (ADR 0003). The
// defaults are the assignment's minimum map size (FR-9) with a typical
// wall density. rows/cols must meet the FR-9 minimum (kMinMapRows x
// kMinMapCols, i.e. 15x15): smaller values throw std::invalid_argument
// in MapGenerator's config-taking constructor.
struct MapGeneratorConfig {
    std::size_t rows = 15;
    std::size_t cols = 15;

    // Probability that an off-path cell becomes Blocked (FR-10).
    double p_blocked = 0.3;

    // Carve-walk tunable: the maximum number of consecutive left/right
    // steps before a step down is forced (keeps the walk bounded).
    std::size_t horizontal_step_cap = 3;

    // Post-check minimum (ADR 0003 step 3): if the random fill produced
    // fewer Blocked tiles than this, force the remainder on off-path
    // cells so every map contains all four kinds (FR-1).
    std::size_t min_blocked = 1;
};

}  // namespace p2game

#endif  // P2GAME_MAPGENERATORCONFIG_H
