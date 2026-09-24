// Map unit tests for ticket T3 (p2game portfolio project).
//
// Verifies the T3 acceptance criteria at the public seam of the p2game::Map
// class (FR-6/FR-7, ADR 0002): the size accessors rows()/cols(), the cell
// accessors at(row, col) (mutable and const), the traversal query
// isTraversable(row, col) with out-of-bounds safety, and toString()
// rendering the grid as legend glyphs (FR-5 legend, FR-8 basis).
//
// Hand-rolled assert-style main: the project uses no third-party
// libraries (GR-4).

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Map.h"
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

// isTraversable must answer out-of-bounds queries with false, never by
// throwing; this helper turns a throw into an observable failure instead
// of an aborted test run.
bool safe_is_traversable(const p2game::Map& map, unsigned long row, unsigned long col) {
    try {
        return map.isTraversable(row, col);
    } catch (...) {
        return false;
    }
}

std::vector<std::string> split_lines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream{text};
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

}  // namespace

int main() {
    // Acceptance criterion 1 (FR-2/FR-6): Map is a class constructed from
    // (rows, cols) and exposes the size accessors.
    {
        const p2game::Map map{3, 4};
        expect_true(map.rows() == 3 && map.cols() == 4,
                    "Map{3, 4} exposes rows() == 3 and cols() == 4");
    }
    {
        const p2game::Map map{15, 15};
        expect_true(map.rows() == 15 && map.cols() == 15,
                    "Map{15, 15} exposes rows() == 15 and cols() == 15 (FR-2 minimum size)");
    }

    // Acceptance criterion 2: a fresh map is all-Traversable -- every tile
    // read back through at() has kind Traversable and the legend glyph 'x'
    // (the state MapGenerator T4 carves/randomizes into).
    {
        const p2game::Map map{3, 4};
        bool all_traversable = true;
        for (std::size_t row = 0; row < map.rows(); ++row) {
            for (std::size_t col = 0; col < map.cols(); ++col) {
                const p2game::Tile& tile = map.at(row, col);
                if (tile.kind() != p2game::TileKind::Traversable || tile.glyph() != 'x') {
                    all_traversable = false;
                }
            }
        }
        expect_true(all_traversable,
                    "every tile of a fresh Map is Traversable with glyph 'x'");
    }

    // Acceptance criterion 3: at() is the mutable cell access -- writing a
    // Tile through the non-const overload reads back with the written kind
    // and glyph, and the const overload reads the same cell correctly.
    {
        p2game::Map map{3, 4};
        map.at(1, 2) = p2game::Tile{p2game::TileKind::Start};
        expect_true(map.at(1, 2).kind() == p2game::TileKind::Start
                        && map.at(1, 2).glyph() == 's',
                    "a tile written through at() reads back with its kind and glyph");

        const p2game::Map& const_view = map;
        expect_true(const_view.at(1, 2).kind() == p2game::TileKind::Start
                        && const_view.at(1, 2).glyph() == 's',
                    "const at() reads back a tile written through the non-const overload");
    }

    // Acceptance criterion 4: isTraversable is true exactly for non-Blocked
    // in-bounds tiles -- true for a default Traversable cell, false after
    // setting it to Blocked, and true again after setting it back to each
    // of the non-Blocked kinds.
    {
        p2game::Map map{2, 3};
        expect_true(map.isTraversable(0, 1),
                    "a default Traversable cell is traversable");
        map.at(0, 1) = p2game::Tile{p2game::TileKind::Blocked};
        expect_true(!map.isTraversable(0, 1),
                    "a Blocked cell is not traversable");
        map.at(0, 1) = p2game::Tile{p2game::TileKind::Start};
        expect_true(map.isTraversable(0, 1),
                    "a Start cell is traversable");
        map.at(0, 1) = p2game::Tile{p2game::TileKind::Exit};
        expect_true(map.isTraversable(0, 1),
                    "an Exit cell is traversable");
        map.at(0, 1) = p2game::Tile{p2game::TileKind::Traversable};
        expect_true(map.isTraversable(0, 1),
                    "a Traversable cell is traversable");
    }

    // Acceptance criterion 5: out-of-bounds isTraversable queries return
    // false without throwing (the traversal-query contract for 2b/4a).
    {
        const p2game::Map map{3, 4};
        expect_true(safe_is_traversable(map, map.rows() + 1, 0) == false
                        && safe_is_traversable(map, 0, map.cols() + 1) == false
                        && safe_is_traversable(map, 1000000, 1000000) == false,
                    "out-of-bounds isTraversable queries return false without throwing");
    }

    // Acceptance criterion 6 (FR-5 legend, FR-8 basis): toString() renders
    // the grid as legend glyphs, one row per line separated by '\n', with
    // no trailing newline after the last row. The 2x3 map below has Start
    // at (0,0), Blocked at (1,1), Exit at (1,2) and Traversable
    // elsewhere: row 0 is "sxx" (s x x), row 1 is "xbe" (x b e).
    {
        p2game::Map map{2, 3};
        map.at(0, 0) = p2game::Tile{p2game::TileKind::Start};
        map.at(1, 1) = p2game::Tile{p2game::TileKind::Blocked};
        map.at(1, 2) = p2game::Tile{p2game::TileKind::Exit};
        const std::string rendered = map.toString();
        expect_true(rendered == "sxx\nxbe",
                    "toString() renders the 2x3 Start/Blocked/Exit grid as \"sxx\\nxbe\"");
        expect_true(!rendered.empty() && rendered.back() != '\n',
                    "toString() has no trailing newline after the last row");

        const std::vector<std::string> lines = split_lines(rendered);
        expect_true(lines.size() == map.rows(),
                    "toString() emits exactly one line per row");
        bool every_line_is_row_width = lines.size() == map.rows();
        for (const std::string& line : lines) {
            if (line.size() != map.cols()) {
                every_line_is_row_width = false;
            }
        }
        expect_true(every_line_is_row_width,
                    "each toString() line is exactly cols() glyphs wide");
    }

    if (g_failure_count == 0) {
        std::cout << "All map tests passed.\n";
        return 0;
    }
    std::cerr << g_failure_count << " map test(s) failed.\n";
    return 1;
}
