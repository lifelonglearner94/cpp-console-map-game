// Map-rules sweep harness for ticket T5 (p2game portfolio project).
//
// Runs p2game::MapGenerator across a sweep of seeds -- 100 seeds at the
// default 15x15 size plus 20 seeds at a larger 20x25 size -- and asserts
// the map rules on EVERY generated map, at the public seam only:
//
//   - minimum size: every generated map is at least 15x15 (FR-9, FR-2);
//   - placement: exactly one Start tile and it lies in row 0, exactly
//     one Exit tile and it lies in the last row (FR-4);
//   - kind + glyph coverage: all four TileKinds occur on every map
//     (FR-1) and the glyphs of the four kinds, read back from the tiles
//     themselves, are pairwise distinct (FR-5);
//   - reachability: a start->exit path exists, verified by a 4-neighbour
//     BFS over non-Blocked cells (via Map::isTraversable) that lives
//     ONLY in this test file -- the generator stays pathfinding-free
//     (FR-3, assignment 4a separation rule);
//   - seed variation: two different seeds produce different maps and
//     the sweep as a whole yields at least two distinct toString()
//     renderings (FR-10, FR-12);
//   - reproducibility: the same seed, generated twice, produces the
//     identical toString() rendering (FR-12, deterministic side).
//
// The harness file is deliberately NOT registered in CMake yet: wiring
// the CTest target is the next implementation step, so the current
// state stays red (the test is not built/registered) per the ticket.
//
// Hand-rolled assert-style main matching tests/mapgenerator_test.cpp:
// no third-party framework (GR-4), English identifiers (GR-3).

#include <cstddef>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "Map.h"
#include "MapGenerator.h"
#include "Tile.h"
#include "TileKind.h"

namespace {

int g_failure_count = 0;

void expect_true(bool condition, const char* description) {
    if (condition) {
        std::cout << "PASS: " << description << '\n';
    } else {
        std::cerr << "FAIL: " << description << '\n';
        ++g_failure_count;
    }
}

// Test-side structural checker (FR-3, assignment 4a separation rule):
// a 4-neighbour BFS over non-Blocked cells (via Map::isTraversable)
// from one cell to another. This lives only in the test -- neither Map
// nor MapGenerator contains pathfinding logic.
bool bfs_reaches(const p2game::Map& map,
                 std::size_t start_row, std::size_t start_col,
                 std::size_t goal_row, std::size_t goal_col) {
    if (!map.isTraversable(start_row, start_col)
            || !map.isTraversable(goal_row, goal_col)) {
        return false;
    }
    std::vector<std::vector<bool>> visited(
        map.rows(), std::vector<bool>(map.cols(), false));
    std::queue<std::pair<std::size_t, std::size_t>> frontier;
    visited[start_row][start_col] = true;
    frontier.push({start_row, start_col});

    static const int row_steps[4] = {1, -1, 0, 0};
    static const int col_steps[4] = {0, 0, 1, -1};
    while (!frontier.empty()) {
        const std::pair<std::size_t, std::size_t> cell = frontier.front();
        frontier.pop();
        if (cell.first == goal_row && cell.second == goal_col) {
            return true;
        }
        for (int step = 0; step < 4; ++step) {
            const long long next_row
                = static_cast<long long>(cell.first) + row_steps[step];
            const long long next_col
                = static_cast<long long>(cell.second) + col_steps[step];
            if (next_row < 0 || next_col < 0
                    || next_row >= static_cast<long long>(map.rows())
                    || next_col >= static_cast<long long>(map.cols())) {
                continue;
            }
            const std::size_t row = static_cast<std::size_t>(next_row);
            const std::size_t col = static_cast<std::size_t>(next_col);
            if (!visited[row][col] && map.isTraversable(row, col)) {
                visited[row][col] = true;
                frontier.push({row, col});
            }
        }
    }
    return false;
}

std::size_t count_kind(const p2game::Map& map, p2game::TileKind kind) {
    std::size_t count = 0;
    for (std::size_t row = 0; row < map.rows(); ++row) {
        for (std::size_t col = 0; col < map.cols(); ++col) {
            if (map.at(row, col).kind() == kind) {
                ++count;
            }
        }
    }
    return count;
}

std::vector<std::pair<std::size_t, std::size_t>> find_kind(
    const p2game::Map& map, p2game::TileKind kind) {
    std::vector<std::pair<std::size_t, std::size_t>> cells;
    for (std::size_t row = 0; row < map.rows(); ++row) {
        for (std::size_t col = 0; col < map.cols(); ++col) {
            if (map.at(row, col).kind() == kind) {
                cells.push_back({row, col});
            }
        }
    }
    return cells;
}

// Reads the glyph of the first occurrence of each of the four kinds
// straight off the map's tiles and checks the four glyphs are pairwise
// distinct (FR-5, via FR-1 kind coverage). Returns false when any kind
// is missing, so callers can keep this check independent of the count
// assertion.
bool glyphs_pairwise_distinct(const p2game::Map& map) {
    const p2game::TileKind kinds[4] = {
        p2game::TileKind::Start, p2game::TileKind::Exit,
        p2game::TileKind::Blocked, p2game::TileKind::Traversable};
    char glyphs[4] = {'\0', '\0', '\0', '\0'};
    for (int i = 0; i < 4; ++i) {
        const std::vector<std::pair<std::size_t, std::size_t>> cells
            = find_kind(map, kinds[i]);
        if (cells.empty()) {
            return false;
        }
        glyphs[i] = map.at(cells[0].first, cells[0].second).glyph();
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            if (glyphs[i] == glyphs[j]) {
                return false;
            }
        }
    }
    return true;
}

// Number of distinct values in a list (O(n^2), fine for ~100 strings).
std::size_t count_distinct(const std::vector<std::string>& values) {
    std::size_t distinct = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        bool seen_before = false;
        for (std::size_t j = 0; j < i; ++j) {
            if (values[j] == values[i]) {
                seen_before = true;
                break;
            }
        }
        if (!seen_before) {
            ++distinct;
        }
    }
    return distinct;
}

// One generated map, checked against every map rule. Returns false when
// any rule is broken so the caller keeps a single aggregate flag per
// sweep.
bool map_rules_hold(const p2game::Map& map) {
    // (1) Minimum size: every generated map is at least 15x15
    // (FR-9, FR-2).
    if (map.rows() < p2game::kMinMapRows
            || map.cols() < p2game::kMinMapCols) {
        return false;
    }
    // (2) Placement: exactly one Start in row 0, exactly one Exit in
    // the last row (FR-4).
    const std::vector<std::pair<std::size_t, std::size_t>> starts
        = find_kind(map, p2game::TileKind::Start);
    const std::vector<std::pair<std::size_t, std::size_t>> exits
        = find_kind(map, p2game::TileKind::Exit);
    if (starts.size() != 1 || exits.size() != 1
            || starts[0].first != 0
            || exits[0].first != map.rows() - 1) {
        return false;
    }
    // (4) Kind coverage, explicit: all four TileKinds occur at least
    // once on every generated map (FR-1).
    if (count_kind(map, p2game::TileKind::Start) == 0
            || count_kind(map, p2game::TileKind::Exit) == 0
            || count_kind(map, p2game::TileKind::Blocked) == 0
            || count_kind(map, p2game::TileKind::Traversable) == 0) {
        return false;
    }
    // (3) The four kinds exist with pairwise-distinct glyphs, read back
    // from the map's own tiles (FR-1, FR-5).
    if (!glyphs_pairwise_distinct(map)) {
        return false;
    }
    // (5) Reachability: a start->exit path through non-Blocked cells,
    // verified by the test-side BFS only (FR-3, assignment 4a rule).
    if (!bfs_reaches(map, starts[0].first, starts[0].second,
                     exits[0].first, exits[0].second)) {
        return false;
    }
    return true;
}

}  // namespace

int main() {
    // FR-5 legend pin at the Tile seam: each kind renders its distinct
    // legend glyph ('s', 'e', 'b', 'x').
    {
        const p2game::Tile start(p2game::TileKind::Start);
        const p2game::Tile exit(p2game::TileKind::Exit);
        const p2game::Tile blocked(p2game::TileKind::Blocked);
        const p2game::Tile traversable(p2game::TileKind::Traversable);
        expect_true(start.glyph() == 's' && exit.glyph() == 'e'
                        && blocked.glyph() == 'b'
                        && traversable.glyph() == 'x',
                    "the four TileKinds render the distinct legend glyphs 's', 'e', 'b', 'x' (FR-5)");
    }

    // Sweep 1 (FR-9, FR-2, FR-1, FR-3, FR-4, FR-5): 100 seeds at the
    // default 15x15 configuration; every generated map must satisfy
    // every map rule, and the sweep must show at least two distinct
    // toString() renderings (FR-10, FR-12 variation side).
    {
        p2game::MapGenerator generator;
        const unsigned seed_count = 100;
        bool all_rules_hold = true;
        std::vector<std::string> renderings;
        for (unsigned seed = 0; seed < seed_count; ++seed) {
            const p2game::Map map = generator.generate(seed);
            if (!map_rules_hold(map)) {
                all_rules_hold = false;
            }
            renderings.push_back(map.toString());
        }
        expect_true(all_rules_hold,
                    "every 15x15 default-config map passes all map rules: min size 15x15 (FR-9, FR-2), one Start in row 0 / one Exit in the last row (FR-4), all four kinds present (FR-1) with distinct glyphs (FR-5), and a BFS-verified start->exit path (FR-3, test-only BFS)");
        expect_true(count_distinct(renderings) >= 2,
                    "across the 100-seed 15x15 sweep at least two distinct map renderings occur (FR-10, FR-12 variation)");
    }

    // Sweep 2 (FR-2, FR-9, FR-1, FR-3, FR-4, FR-5): 20 seeds at the
    // larger 20x25 configuration via MapGeneratorConfig; the same map
    // rules must hold at every generated map of the larger size, and
    // this sweep must also show at least two distinct renderings.
    {
        p2game::MapGeneratorConfig config;
        config.rows = 20;
        config.cols = 25;
        p2game::MapGenerator generator{config};
        const unsigned seed_count = 20;
        bool all_rules_hold = true;
        std::vector<std::string> renderings;
        for (unsigned seed = 0; seed < seed_count; ++seed) {
            const p2game::Map map = generator.generate(seed);
            if (!map_rules_hold(map)) {
                all_rules_hold = false;
            }
            renderings.push_back(map.toString());
        }
        expect_true(all_rules_hold,
                    "every 20x25 configured map passes all map rules: min size (FR-9, FR-2), Start/Exit placement (FR-4), all four kinds with distinct glyphs (FR-1, FR-5), and a BFS-verified start->exit path (FR-3, test-only BFS)");
        expect_true(count_distinct(renderings) >= 2,
                    "across the 20-seed 20x25 sweep at least two distinct map renderings occur (FR-10, FR-12 variation)");
    }

    // Seed variation, explicit pair (FR-10, FR-12): two DIFFERENT seeds
    // produce different maps (compared via toString() renderings), at
    // both the default and the larger configuration.
    {
        p2game::MapGenerator generator;
        const p2game::Map seed_one = generator.generate(1u);
        const p2game::Map seed_two = generator.generate(2u);
        expect_true(seed_one.toString() != seed_two.toString(),
                    "two different seeds at the default 15x15 size produce different maps (FR-10)");
        expect_true(seed_one.rows() == 15 && seed_one.cols() == 15,
                    "the default configuration generates a 15x15 map (FR-9, FR-2)");

        p2game::MapGeneratorConfig config;
        config.rows = 20;
        config.cols = 25;
        p2game::MapGenerator large_generator{config};
        const p2game::Map large_seed_one = large_generator.generate(1u);
        const p2game::Map large_seed_two = large_generator.generate(2u);
        expect_true(large_seed_one.toString() != large_seed_two.toString(),
                    "two different seeds at the 20x25 size produce different maps (FR-10)");
    }

    // Reproducibility, deterministic side (FR-12): the same seed,
    // generated twice, produces the identical toString() rendering --
    // within one generator instance.
    {
        p2game::MapGenerator generator;
        bool reproducibility_holds = true;
        const unsigned seeds[3] = {0u, 42u, 12345u};
        for (int i = 0; i < 3; ++i) {
            const p2game::Map first = generator.generate(seeds[i]);
            const p2game::Map second = generator.generate(seeds[i]);
            if (first.toString() != second.toString()) {
                reproducibility_holds = false;
            }
        }
        expect_true(reproducibility_holds,
                    "the same seed generated twice produces the identical map rendering (FR-12 deterministic side)");
    }

    if (g_failure_count == 0) {
        std::cout << "All map_rules tests passed.\n";
        return 0;
    }
    std::cerr << g_failure_count << " map_rules test(s) failed.\n";
    return 1;
}
