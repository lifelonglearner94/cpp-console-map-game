// MapGenerator unit tests for ticket T4 (p2game portfolio project).
//
// Pins the carve-then-randomize generation contract of ADR 0003 at the
// public seam of the future p2game::MapGenerator class:
//
//   - a no-input run yields a complete, ready-to-use p2game::Map (FR-9);
//   - different seeds produce different maps, the same seed reproduces
//     the identical map (FR-10 / FR-12);
//   - exactly one Start tile in row 0 and exactly one Exit tile in the
//     last row, at randomized columns (FR-4 + FR-10);
//   - all four TileKinds occur on every generated map (FR-1);
//   - a start->exit path through non-Blocked cells survives any
//     randomization, structurally (FR-3 / FR-11) -- checked here by a
//     test-side BFS; the generator itself stays pathfinding-free
//     (assignment 4a separation rule, ADR 0003);
//   - size and p_blocked are configuration, not scattered constants:
//     the default configuration is 15x15, larger sizes are configurable
//     and p_blocked = 1.0 blocks the off-path cells while the carved
//     path stays traversable (FR-2, spec §9).
//
// The configuration seam this file pins is deliberately minimal, per the
// ticket: a default-constructible MapGenerator and a config object with
// rows / cols / p_blocked fields, plus generate(unsigned seed) returning
// a p2game::Map. Everything else about the API shape is the
// implementer's choice.
//
// src/MapGenerator.h does not exist yet, so the expected RED state is a
// compile error on that include (the guarded CMake link keeps the build
// configurable until the implementation lands).
//
// Hand-rolled assert-style main: the project uses no third-party
// libraries (GR-4).

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

// Test-side structural checker (FR-3 / FR-11): a 4-neighbour BFS over
// non-Blocked cells (via Map::isTraversable) from one cell to another.
// This lives only in the test -- neither Map nor MapGenerator contains
// pathfinding logic.
bool bfs_reaches(const p2game::Map& map,
                 std::size_t start_row, std::size_t start_col,
                 std::size_t exit_row, std::size_t exit_col) {
    if (!map.isTraversable(start_row, start_col)
            || !map.isTraversable(exit_row, exit_col)) {
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
        if (cell.first == exit_row && cell.second == exit_col) {
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

}  // namespace

int main() {
    // Acceptance criterion 1 (FR-9, FR-2): a plain, no-input generator
    // run produces a complete, ready-to-use map -- a default-constructed
    // generator's generate() returns a valid 15x15 map.
    {
        p2game::MapGenerator generator;
        const p2game::Map map = generator.generate(42u);
        expect_true(map.rows() == 15 && map.cols() == 15,
                    "a default-configured generator run automatically yields a 15x15 map (FR-9, FR-2)");
    }

    // Acceptance criterion 2 (FR-2, spec §9): larger map sizes are
    // configuration, not scattered constants -- a custom config of
    // 20x25 produces a 20x25 map that still satisfies the structural
    // invariants.
    {
        p2game::MapGeneratorConfig config;
        config.rows = 20;
        config.cols = 25;
        p2game::MapGenerator generator{config};
        const p2game::Map map = generator.generate(7u);
        expect_true(map.rows() == 20 && map.cols() == 25,
                    "a custom config produces the configured 20x25 map size (FR-2)");
        const std::vector<std::pair<std::size_t, std::size_t>> starts
            = find_kind(map, p2game::TileKind::Start);
        const std::vector<std::pair<std::size_t, std::size_t>> exits
            = find_kind(map, p2game::TileKind::Exit);
        expect_true(starts.size() == 1 && exits.size() == 1
                        && starts[0].first == 0
                        && exits[0].first == map.rows() - 1
                        && bfs_reaches(map, starts[0].first, starts[0].second,
                                       exits[0].first, exits[0].second),
                    "the 20x25 map has one Start in row 0, one Exit in the last row, and a start->exit path (FR-3, FR-4)");
    }

    // Acceptance criteria 3-5 (FR-4, FR-1, FR-3/FR-11): over a sweep of
    // seeds every generated map places exactly one Start in row 0 and
    // exactly one Exit in the last row, contains all four TileKinds, and
    // has a structurally guaranteed traversable start->exit path.
    {
        p2game::MapGenerator generator;
        bool placements_hold = true;
        bool kinds_hold = true;
        bool paths_hold = true;
        for (unsigned seed = 0; seed < 20; ++seed) {
            const p2game::Map map = generator.generate(seed);
            const std::vector<std::pair<std::size_t, std::size_t>> starts
                = find_kind(map, p2game::TileKind::Start);
            const std::vector<std::pair<std::size_t, std::size_t>> exits
                = find_kind(map, p2game::TileKind::Exit);
            if (starts.size() != 1 || exits.size() != 1
                    || starts[0].first != 0
                    || exits[0].first != map.rows() - 1) {
                placements_hold = false;
            }
            if (count_kind(map, p2game::TileKind::Start) == 0
                    || count_kind(map, p2game::TileKind::Exit) == 0
                    || count_kind(map, p2game::TileKind::Blocked) == 0
                    || count_kind(map, p2game::TileKind::Traversable) == 0) {
                kinds_hold = false;
            }
            if (!bfs_reaches(map, starts[0].first, starts[0].second,
                             exits[0].first, exits[0].second)) {
                paths_hold = false;
            }
        }
        expect_true(placements_hold,
                    "every generated map has exactly one Start in row 0 and one Exit in the last row (FR-4)");
        expect_true(kinds_hold,
                    "every generated map contains all four TileKinds (FR-1)");
        expect_true(paths_hold,
                    "a test-side BFS finds a start->exit path through non-Blocked cells on every generated map (FR-3, FR-11 structural)");
    }

    // Acceptance criterion 6 (FR-12): the same seed reproduces the
    // identical map -- within one generator and across fresh generator
    // instances (toString() renders every tile's glyph, so equal
    // renderings mean equal maps).
    {
        p2game::MapGenerator generator;
        const p2game::Map first = generator.generate(123u);
        const p2game::Map second = generator.generate(123u);
        p2game::MapGenerator fresh_generator;
        const p2game::Map from_fresh = fresh_generator.generate(123u);
        expect_true(first.toString() == second.toString(),
                    "two runs of the same generator with the same seed produce identical maps (FR-12)");
        expect_true(first.toString() == from_fresh.toString(),
                    "a fresh generator with the same seed reproduces the identical map (FR-12)");
    }

    // Acceptance criterion 7 (FR-10, FR-4): generation is genuinely
    // procedural -- across seeds the map shape differs and the
    // start/exit columns vary.
    {
        p2game::MapGenerator generator;
        std::vector<std::string> renderings;
        std::vector<std::size_t> start_columns;
        std::vector<std::size_t> exit_columns;
        for (unsigned seed = 0; seed < 30; ++seed) {
            const p2game::Map map = generator.generate(seed);
            renderings.push_back(map.toString());
            const std::vector<std::pair<std::size_t, std::size_t>> starts
                = find_kind(map, p2game::TileKind::Start);
            const std::vector<std::pair<std::size_t, std::size_t>> exits
                = find_kind(map, p2game::TileKind::Exit);
            start_columns.push_back(starts[0].second);
            exit_columns.push_back(exits[0].second);
        }
        std::size_t distinct_renderings = 0;
        for (std::size_t i = 0; i < renderings.size(); ++i) {
            bool seen_before = false;
            for (std::size_t j = 0; j < i; ++j) {
                if (renderings[j] == renderings[i]) {
                    seen_before = true;
                }
            }
            if (!seen_before) {
                ++distinct_renderings;
            }
        }
        expect_true(distinct_renderings >= 2,
                    "different seeds produce different map shapes (FR-10)");
        std::size_t distinct_start_columns = 0;
        std::size_t distinct_exit_columns = 0;
        for (std::size_t i = 0; i < start_columns.size(); ++i) {
            bool start_seen = false;
            bool exit_seen = false;
            for (std::size_t j = 0; j < i; ++j) {
                if (start_columns[j] == start_columns[i]) {
                    start_seen = true;
                }
                if (exit_columns[j] == exit_columns[i]) {
                    exit_seen = true;
                }
            }
            if (!start_seen) {
                ++distinct_start_columns;
            }
            if (!exit_seen) {
                ++distinct_exit_columns;
            }
        }
        expect_true(distinct_start_columns >= 2 && distinct_exit_columns >= 2,
                    "across seeds the Start and Exit columns vary (FR-4 + FR-10)");
    }

    // Acceptance criterion 8 (FR-2, FR-10, FR-3/FR-11): p_blocked is a
    // configuration parameter -- p_blocked = 1.0 blocks the off-path
    // cells (the majority of the map) while the carved start->exit path
    // still survives the randomization.
    {
        p2game::MapGeneratorConfig config;
        config.rows = 15;
        config.cols = 15;
        config.p_blocked = 1.0;
        p2game::MapGenerator generator{config};
        const p2game::Map map = generator.generate(9u);
        const std::size_t total = map.rows() * map.cols();
        expect_true(count_kind(map, p2game::TileKind::Blocked) * 2 > total,
                    "a config with p_blocked = 1.0 yields many Blocked tiles (FR-10 tunable)");
        const std::vector<std::pair<std::size_t, std::size_t>> starts
            = find_kind(map, p2game::TileKind::Start);
        const std::vector<std::pair<std::size_t, std::size_t>> exits
            = find_kind(map, p2game::TileKind::Exit);
        expect_true(starts.size() == 1 && exits.size() == 1
                        && bfs_reaches(map, starts[0].first, starts[0].second,
                                       exits[0].first, exits[0].second),
                    "with p_blocked = 1.0 the carved start->exit path still survives (FR-3, FR-11 structural)");
    }

    if (g_failure_count == 0) {
        std::cout << "All mapgenerator tests passed.\n";
        return 0;
    }
    std::cerr << g_failure_count << " mapgenerator test(s) failed.\n";
    return 1;
}
