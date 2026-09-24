#include "Tile.h"

// Ticket T2: definitions for the Tile value class. Kept deliberately
// simple: kind + glyph. Extension point (spec §8, Aufgabe 1c): a later
// item payload can be added as a new private member here without
// changing any Map-facing API.

namespace p2game {

namespace {

// FR-5 glyph legend: Start='s', Exit='e', Blocked='b', Traversable='x'.
char legendGlyph(TileKind kind) {
    switch (kind) {
        case TileKind::Start:
            return 's';
        case TileKind::Exit:
            return 'e';
        case TileKind::Blocked:
            return 'b';
        case TileKind::Traversable:
            return 'x';
    }
    return 'x';
}

}  // namespace

Tile::Tile() : kind_(TileKind::Traversable), glyph_(legendGlyph(kind_)) {}

Tile::Tile(TileKind kind) : kind_(kind), glyph_(legendGlyph(kind)) {}

Tile::Tile(const Tile& other) : kind_(other.kind_), glyph_(other.glyph_) {}

Tile& Tile::operator=(const Tile& other) {
    kind_ = other.kind_;
    glyph_ = other.glyph_;
    return *this;
}

TileKind Tile::kind() const {
    return kind_;
}

char Tile::glyph() const {
    return glyph_;
}

}  // namespace p2game
