// Tile unit tests for ticket T2 (p2game portfolio project).
//
// Verifies the T2 acceptance criteria at the public seam of the domain
// types: the p2game::TileKind enum (FR-1) and the p2game::Tile value
// class with its kind and glyph accessors (FR-5 legend).
//
// Hand-rolled assert-style main: the project uses no third-party
// libraries (GR-4).

#include <iostream>

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

// FR-5: every tile displays exactly one of the four legend glyphs.
bool is_legend_glyph(char glyph) {
    return glyph == 's' || glyph == 'e' || glyph == 'b' || glyph == 'x';
}

}  // namespace

int main() {
    // FR-1: p2game::TileKind is an enum with exactly the four enumerators
    // Start, Exit, Blocked and Traversable. The static_asserts force every
    // enumerator to compile and pin the four kinds as pairwise distinct
    // values, so the test can only compile when all four exist.
    static_assert(p2game::TileKind::Start != p2game::TileKind::Exit,
                  "TileKind::Start must differ from TileKind::Exit");
    static_assert(p2game::TileKind::Start != p2game::TileKind::Blocked,
                  "TileKind::Start must differ from TileKind::Blocked");
    static_assert(p2game::TileKind::Start != p2game::TileKind::Traversable,
                  "TileKind::Start must differ from TileKind::Traversable");
    static_assert(p2game::TileKind::Exit != p2game::TileKind::Blocked,
                  "TileKind::Exit must differ from TileKind::Blocked");
    static_assert(p2game::TileKind::Exit != p2game::TileKind::Traversable,
                  "TileKind::Exit must differ from TileKind::Traversable");
    static_assert(p2game::TileKind::Blocked != p2game::TileKind::Traversable,
                  "TileKind::Blocked must differ from TileKind::Traversable");
    std::cout << "PASS: TileKind has the four distinct enumerators Start, Exit, Blocked, Traversable\n";

    // Acceptance criterion 2: Tile is a value class -- default-constructible,
    // constructible from a TileKind, and exposes kind() and glyph().
    const p2game::Tile default_tile{};
    expect_true(is_legend_glyph(default_tile.glyph()),
                "default-constructed Tile displays one of the legend glyphs");

    const p2game::Tile constructed_tile{p2game::TileKind::Exit};
    expect_true(constructed_tile.kind() == p2game::TileKind::Exit,
                "Tile constructed from TileKind exposes its kind through kind()");

    // Acceptance criterion 3 (FR-5 glyph legend).
    expect_true(p2game::Tile{p2game::TileKind::Start}.glyph() == 's',
                "Start tiles display the glyph 's'");
    expect_true(p2game::Tile{p2game::TileKind::Exit}.glyph() == 'e',
                "Exit tiles display the glyph 'e'");
    expect_true(p2game::Tile{p2game::TileKind::Blocked}.glyph() == 'b',
                "Blocked tiles display the glyph 'b'");
    expect_true(p2game::Tile{p2game::TileKind::Traversable}.glyph() == 'x',
                "Traversable tiles display the glyph 'x'");

    // Acceptance criterion 4 (FR-5): the Start and Exit glyphs are distinct
    // from each other and from every other tile's glyph.
    expect_true(p2game::Tile{p2game::TileKind::Start}.glyph()
                    != p2game::Tile{p2game::TileKind::Exit}.glyph(),
                "Start and Exit glyphs are distinct from each other");
    expect_true(p2game::Tile{p2game::TileKind::Start}.glyph()
                    != p2game::Tile{p2game::TileKind::Blocked}.glyph(),
                "Start and Blocked glyphs are distinct");
    expect_true(p2game::Tile{p2game::TileKind::Start}.glyph()
                    != p2game::Tile{p2game::TileKind::Traversable}.glyph(),
                "Start and Traversable glyphs are distinct");
    expect_true(p2game::Tile{p2game::TileKind::Exit}.glyph()
                    != p2game::Tile{p2game::TileKind::Blocked}.glyph(),
                "Exit and Blocked glyphs are distinct");
    expect_true(p2game::Tile{p2game::TileKind::Exit}.glyph()
                    != p2game::Tile{p2game::TileKind::Traversable}.glyph(),
                "Exit and Traversable glyphs are distinct");

    // Acceptance criterion 5: value-class copy semantics -- copies preserve
    // kind and glyph (both copy construction and copy assignment).
    const p2game::Tile source_tile{p2game::TileKind::Blocked};
    const p2game::Tile copy_constructed{source_tile};
    expect_true(copy_constructed.kind() == source_tile.kind()
                    && copy_constructed.glyph() == source_tile.glyph(),
                "copy-constructed Tile preserves kind and glyph");

    p2game::Tile copy_assigned{};
    copy_assigned = source_tile;
    expect_true(copy_assigned.kind() == source_tile.kind()
                    && copy_assigned.glyph() == source_tile.glyph(),
                "copy-assigned Tile preserves kind and glyph");

    if (g_failure_count == 0) {
        std::cout << "All tile tests passed.\n";
        return 0;
    }
    std::cerr << g_failure_count << " tile test(s) failed.\n";
    return 1;
}
