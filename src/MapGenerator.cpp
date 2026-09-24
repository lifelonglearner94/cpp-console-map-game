#include "MapGenerator.h"

// Ticket T4: definitions of p2game::MapGenerator (declarations in
// MapGenerator.h, per GR-1/GR-2). The carve walk is a bounded random
// walk with monotonically non-decreasing row (ADR 0003): step down
// (preferred, probability-weighted) or step left/right with no
// immediate backtracking and a cap on consecutive horizontal steps,
// never up. It reaches the last row in bounded steps, then moves
// horizontally to the exit column. No pathfinding anywhere.

#include <random>
#include <vector>

namespace p2game {

MapGenerator::MapGenerator() : config_() {}

MapGenerator::MapGenerator(const MapGeneratorConfig& config) : config_(config) {}

Map MapGenerator::generate(unsigned seed) const {
    // Fresh RNG state per call: the same seed reproduces the identical
    // map on this instance and on a fresh instance (FR-12).
    std::mt19937 rng(seed);

    const std::size_t rows = config_.rows;
    const std::size_t cols = config_.cols;
    Map map(rows, cols);

    // Which cells the carve walk visited (Start/Exit/Traversable by
    // construction; the random fill below only touches off-path cells,
    // so the carved path survives any randomization, FR-11 structural).
    std::vector<std::vector<bool>> on_path(
        rows, std::vector<bool>(cols, false));

    // 1. Carve: random start column in row 0, random exit column in the
    //    last row, both from the seeded RNG (FR-4 + FR-10 variation).
    std::uniform_int_distribution<std::size_t> column_dist(0, cols - 1);
    const std::size_t start_col = column_dist(rng);
    const std::size_t exit_col = column_dist(rng);

    std::size_t row = 0;
    std::size_t col = start_col;
    on_path[row][col] = true;

    if (rows > 1) {
        std::size_t consecutive_horizontal = 0;
        int last_horizontal_step = 0;  // 0 = none yet, -1 = left, +1 = right
        std::bernoulli_distribution step_down_dist(0.7);

        while (row + 1 < rows) {
            // Horizontal candidates: in bounds, not the immediate
            // backtracking step.
            std::vector<int> candidates;
            if (col > 0 && last_horizontal_step != 1) {
                candidates.push_back(-1);
            }
            if (col + 1 < cols && last_horizontal_step != -1) {
                candidates.push_back(1);
            }

            // Forced down when no horizontal candidate exists or the
            // consecutive-horizontal cap is reached; otherwise weighted
            // (down preferred).
            bool go_down = candidates.empty()
                || consecutive_horizontal >= config_.horizontal_step_cap;
            if (!go_down) {
                go_down = step_down_dist(rng);
            }

            if (go_down) {
                ++row;
                consecutive_horizontal = 0;
                last_horizontal_step = 0;
            } else {
                std::uniform_int_distribution<std::size_t> pick(
                    0, candidates.size() - 1);
                const int step = candidates[pick(rng)];
                col = static_cast<std::size_t>(
                    static_cast<long long>(col) + step);
                last_horizontal_step = step;
                ++consecutive_horizontal;
            }
            on_path[row][col] = true;
        }

        // In the last row: walk horizontally to the exit column.
        while (col != exit_col) {
            const long long next = static_cast<long long>(col)
                + (exit_col > col ? 1 : -1);
            col = static_cast<std::size_t>(next);
            on_path[row][col] = true;
        }
    }

    // 2. Randomize: every off-path cell Blocked with probability
    //    p_blocked, else Traversable (the fresh Map grid starts all
    //    Traversable, so only Blocked needs writing, but write both for
    //    explicitness).
    std::bernoulli_distribution blocked_dist(config_.p_blocked);
    for (std::size_t r = 0; r < rows; ++r) {
        for (std::size_t c = 0; c < cols; ++c) {
            if (!on_path[r][c]) {
                map.at(r, c) = Tile(blocked_dist(rng)
                    ? TileKind::Blocked : TileKind::Traversable);
            }
        }
    }

    // 3. Pin the carved kinds: exactly one Start in row 0 and exactly
    //    one Exit in the last row (FR-4); the rest of the carved path
    //    stays Traversable.
    map.at(0, start_col) = Tile(TileKind::Start);
    map.at(rows - 1, exit_col) = Tile(TileKind::Exit);

    // Post-check (ADR 0003 step 3): if the fill produced too few
    // Blocked tiles, force the minimum on off-path cells so every map
    // contains all four TileKinds (FR-1).
    std::size_t blocked_count = 0;
    for (std::size_t r = 0; r < rows; ++r) {
        for (std::size_t c = 0; c < cols; ++c) {
            if (map.at(r, c).kind() == TileKind::Blocked) {
                ++blocked_count;
            }
        }
    }
    if (blocked_count < config_.min_blocked) {
        std::size_t missing = config_.min_blocked - blocked_count;
        for (std::size_t r = 0; r < rows && missing > 0; ++r) {
            for (std::size_t c = 0; c < cols && missing > 0; ++c) {
                if (!on_path[r][c]) {
                    map.at(r, c) = Tile(TileKind::Blocked);
                    --missing;
                }
            }
        }
    }

    return map;
}

}  // namespace p2game
