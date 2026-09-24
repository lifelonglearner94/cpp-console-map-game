// Chest / item-ownership unit tests for ticket T2b (p2game portfolio
// project).
//
// Verifies the T2b acceptance criteria at the public seam of
// p2game::TileKind / p2game::Tile / p2game::Map (FR-13/FR-20, ADR
// 0006): the fifth tile kind TreasureChest exists, is pairwise distinct
// from the four increment-1 kinds and renders the legend glyph 't'; a
// chest tile is traversable; Tile carries a non-owning item view
// (item()/setItem(), nullptr by default) that survives copying, and Map
// answers itemAt(row, col) for that view (nullptr on an empty tile);
// Tile stays a copyable value type. Item ownership itself lives in
// Map's std::vector<std::unique_ptr<ItemBase>> storage (ADR 0006): a
// hand-built Map exposes no item-placement hook in this ticket, so the
// test owns that storage locally and hands the tile only the non-owning
// view, exactly as T2c's generator will.
//
// Hand-rolled assert-style main: the project uses no third-party
// libraries (GR-4).

#include <cstddef>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include "ItemBase.h"
#include "ItemFactory.h"
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

}  // namespace

int main() {
    using p2game::TileKind;

    // Acceptance criterion 1 (FR-13): TreasureChest exists and is a
    // distinct enumerator -- equality against every increment-1 kind
    // must be false already at compile time.
    static_assert(TileKind::TreasureChest != TileKind::Start,
                  "TreasureChest must be distinct from Start");
    static_assert(TileKind::TreasureChest != TileKind::Exit,
                  "TreasureChest must be distinct from Exit");
    static_assert(TileKind::TreasureChest != TileKind::Blocked,
                  "TreasureChest must be distinct from Blocked");
    static_assert(TileKind::TreasureChest != TileKind::Traversable,
                  "TreasureChest must be distinct from Traversable");

    // Acceptance criterion 2 (FR-13): the chest uses the legend glyph 't'.
    const p2game::Tile chest_tile{TileKind::TreasureChest};
    expect_true(chest_tile.kind() == TileKind::TreasureChest,
                "Tile{TreasureChest} keeps its kind");
    expect_true(chest_tile.glyph() == 't',
                "Tile{TreasureChest}.glyph() is the legend glyph 't'");

    // Acceptance criterion 3 (FR-13): a chest is a traversable tile, so
    // it blocks no path and the map renders it as 't'.
    p2game::Map map{3, 3};
    map.at(1, 1) = p2game::Tile{TileKind::TreasureChest};
    expect_true(map.at(1, 1).kind() == TileKind::TreasureChest,
                "map.at(row, col) = Tile{TreasureChest} stores the chest kind");
    expect_true(map.isTraversable(1, 1),
                "isTraversable(row, col) is true for a TreasureChest tile");
    expect_true(map.toString().find('t') != std::string::npos,
                "toString() renders the chest tile as glyph 't'");

    // Acceptance criterion 4 (FR-20, ADR 0006): the item view round-trips
    // through both accessors on a hand-built Map. Map owns items in a
    // std::vector<std::unique_ptr<ItemBase>> (ADR 0006); this test owns
    // that storage and hands the chest tile only the non-owning view.
    p2game::ItemFactory factory;
    std::mt19937 rng{2024u};
    std::vector<std::unique_ptr<p2game::ItemBase>> map_storage;
    map_storage.push_back(factory.create(rng));
    expect_true(map_storage.front() != nullptr,
                "ItemFactory::create returns a concrete item");

    p2game::ItemBase* placed = map_storage.front().get();
    map.at(1, 1).setItem(placed);
    expect_true(map.at(1, 1).item() == placed,
                "Tile::item() returns the pointer handed to setItem()");
    expect_true(map.itemAt(1, 1) == placed,
                "Map::itemAt(row, col) returns the tile's item view");
    expect_true(map.at(1, 1).item() == map.itemAt(1, 1),
                "map.at(row, col).item() and map.itemAt(row, col) agree");

    // Value semantics: Tile stays copy-constructible/assignable and a
    // copy preserves kind, glyph and the item view.
    static_assert(std::is_copy_constructible<p2game::Tile>::value,
                  "Tile must stay copy-constructible");
    static_assert(std::is_copy_assignable<p2game::Tile>::value,
                  "Tile must stay copy-assignable");

    const p2game::Tile copied{map.at(1, 1)};
    expect_true(copied.kind() == TileKind::TreasureChest,
                "a Tile copy preserves the chest kind");
    expect_true(copied.glyph() == 't',
                "a Tile copy preserves the chest glyph");
    expect_true(copied.item() == placed,
                "a Tile copy preserves the item view");

    p2game::Tile assigned;
    assigned = map.at(1, 1);
    expect_true(assigned.kind() == TileKind::TreasureChest &&
                    assigned.glyph() == 't' && assigned.item() == placed,
                "Tile copy-assignment preserves kind, glyph and item view");

    // map.at(row, col) = Tile(...) must keep compiling (value assignment
    // into the grid).
    map.at(0, 2) = p2game::Tile{TileKind::TreasureChest};
    expect_true(map.at(0, 2).kind() == TileKind::TreasureChest &&
                    map.at(0, 2).glyph() == 't',
                "assigning a fresh Tile into the grid keeps kind and glyph");

    // Acceptance criterion 5: no item means nullptr at both seams.
    const p2game::Tile default_tile;
    expect_true(default_tile.item() == nullptr,
                "a default Tile has no item view (nullptr)");
    p2game::Map empty_map{2, 2};
    expect_true(empty_map.itemAt(0, 0) == nullptr,
                "Map::itemAt(row, col) is nullptr on an empty tile");

    if (g_failure_count == 0) {
        std::cout << "All chest/item-ownership tests passed.\n";
        return 0;
    }
    std::cerr << g_failure_count << " chest/item-ownership test(s) failed.\n";
    return 1;
}
