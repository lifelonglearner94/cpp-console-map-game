// Ticket T6 / ADR 0004: the console demo for Aufgabe 1b. Builds two
// 15x15 maps with MapGenerator -- one from a fixed seed constant (the
// reproducible map, FR-12) and one from a std::random_device seed (the
// random map, FR-10) -- then writes a legend line (FR-8), per map a
// seed label plus the Start/Exit coordinates, and each map rendered via
// Map::toString(). Plain ASCII only, no raylib/windowing; no
// pathfinding here (assignment 4a separation: path logic stays out of
// the demo).

#include "Demo.h"

#include <cstddef>
#include <random>

#include "Map.h"
#include "MapGenerator.h"
#include "Tile.h"
#include "TileKind.h"

namespace p2game {
namespace {

// The fixed seed of the reproducible demo map (FR-12).
constexpr unsigned kFixedSeed = 42u;

void print_map(std::ostream& out, const char* label, unsigned seed,
               const Map& map) {
    // Locate the Start and Exit glyphs via the Map's own query surface.
    std::size_t start_row = 0;
    std::size_t start_col = 0;
    std::size_t exit_row = 0;
    std::size_t exit_col = 0;
    for (std::size_t row = 0; row < map.rows(); ++row) {
        for (std::size_t col = 0; col < map.cols(); ++col) {
            const TileKind kind = map.at(row, col).kind();
            if (kind == TileKind::Start) {
                start_row = row;
                start_col = col;
            } else if (kind == TileKind::Exit) {
                exit_row = row;
                exit_col = col;
            }
        }
    }

    out << label << " (seed " << seed << ")\n";
    out << "Start: (" << start_row << ", " << start_col << ")\n";
    out << "Exit: (" << exit_row << ", " << exit_col << ")\n";
    out << map.toString() << '\n';
}

}  // namespace

int run_demo(std::ostream& out) {
    const MapGeneratorConfig config;  // 15x15 default (FR-9)
    const MapGenerator generator(config);

    const Map fixed_map = generator.generate(kFixedSeed);
    const unsigned random_seed = std::random_device{}();
    const Map random_map = generator.generate(random_seed);

    out << "p2game Aufgabe 1b demo\n";
    out << "Legend: s=Start, e=Exit, b=Blocked, x=Traversable\n";
    print_map(out, "Map A (fixed seed)", kFixedSeed, fixed_map);
    print_map(out, "Map B (random_device seed)", random_seed, random_map);
    return 0;
}

}  // namespace p2game
