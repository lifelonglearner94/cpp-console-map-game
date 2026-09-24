#ifndef P2GAME_TILE_H
#define P2GAME_TILE_H

// Ticket T2 / FR-5: a map tile as a small value type: its TileKind plus
// the display glyph from the legend (Start='s', Exit='e', Blocked='b',
// Traversable='x').

#include "TileKind.h"

namespace p2game {

class Tile {
public:
    // Default-constructed tile (a traversable tile by default).
    Tile();

    // Explicit tile construction from a TileKind.
    explicit Tile(TileKind kind);

    // Copyable: copies preserve both kind and glyph.
    Tile(const Tile& other);
    Tile& operator=(const Tile& other);

    // Accessors.
    TileKind kind() const;
    char glyph() const;

private:    TileKind kind_;
    char glyph_;
};

}  // namespace p2game

#endif  // P2GAME_TILE_H
