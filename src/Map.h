#ifndef P2GAME_MAP_H
#define P2GAME_MAP_H

// Ticket T3 / FR-6, FR-7: the game map as a rows x cols grid of Tile
// value cells (not templated, per ADR 0002 -- the tile type is fixed by
// the alias below, keeping a later Map<T> refactor mechanical). The map
// is a pure data + traversal-query structure: no pathfinding, no
// generation, no I/O beyond toString() (spec architecture boundary; the
// 4a separation rule). The stable query surface for later Aufgaben
// 2b/4a (spec §8) is: rows(), cols(), at(row, col), isTraversable(row,
// col), toString(). Ticket T2b / ADR 0006: Map additionally OWNS the
// items lying on the map (a vector of unique_ptr) and exposes a
// read-only item view per cell via itemAt(row, col); the tiles hold
// only non-owning views.

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "ItemBase.h"
#include "Tile.h"

namespace p2game {

class Map {
public:
    // Explicit map construction from dimensions; the fresh grid is
    // all-Traversable (MapGenerator T4 carves/randomizes into it).
    explicit Map(std::size_t rows, std::size_t cols);

    // Size accessors.
    std::size_t rows() const;
    std::size_t cols() const;

    // Cell access: mutable and const views of one tile.
    Tile& at(std::size_t row, std::size_t col);
    const Tile& at(std::size_t row, std::size_t col) const;

    // Traversal query: true iff (row, col) is in bounds and its tile is
    // not Blocked. Out-of-bounds queries answer false, never throw.
    bool isTraversable(std::size_t row, std::size_t col) const;

    // Renders the grid as legend glyphs, one line per row, lines
    // separated by '\n' with no trailing newline after the last row.
    std::string toString() const;

    // Ticket T2b / FR-20 (ADR 0006): the non-owning view of the item on
    // cell (row, col), or nullptr if the cell has no item. Out-of-bounds
    // queries answer nullptr, never throw (consistent with
    // isTraversable's convention).
    const ItemBase* itemAt(std::size_t row, std::size_t col) const;

private:
    // ADR 0002 extension point: fixing the cell type here keeps a future
    // Map<T> refactor mechanical.
    using tile_type = Tile;

    std::size_t rows_;
    std::size_t cols_;
    std::vector<std::vector<tile_type>> grid_;

    // Ticket T2b / ADR 0006: Map owns the items on the map; the tiles
    // hold non-owning views into this storage. Owning storage makes
    // Map non-copyable but movable -- intended: maps are only ever
    // move-initialized (NRVO from MapGenerator::generate).
    std::vector<std::unique_ptr<ItemBase>> items_;
};

}  // namespace p2game

#endif  // P2GAME_MAP_H
