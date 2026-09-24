#ifndef P2GAME_TILE_H
#define P2GAME_TILE_H

// Ticket T2 / FR-5: a map tile as a small value type: its TileKind plus
// the display glyph from the legend (Start='s', Exit='e', Blocked='b',
// Traversable='x', TreasureChest='t' per ticket T2b / Aufgabe 1c).
// T2b adds the non-owning item view (ADR 0006).

#include "ItemBase.h"
#include "TileKind.h"

namespace p2game {

class Tile {
public:
    // Default-constructed tile (a traversable tile by default).
    Tile();

    // Explicit tile construction from a TileKind.
    explicit Tile(TileKind kind);

    // Copyable: copies preserve kind, glyph and the (non-owning) item
    // view -- the pointer is copied, ownership never changes hands.
    Tile(const Tile& other);
    Tile& operator=(const Tile& other);

    // Accessors.
    TileKind kind() const;
    char glyph() const;

    // Ticket T2b / FR-20 (ADR 0006): the NON-OWNING view of the item
    // sitting on this tile. Ownership lives elsewhere (Map's item
    // storage, ADR 0006); nullptr means the tile holds no item.
    const ItemBase* item() const;
    void setItem(const ItemBase* item);

private:
    TileKind kind_;
    char glyph_;
    const ItemBase* item_;
};

}  // namespace p2game

#endif  // P2GAME_TILE_H
