#include "Tile.h"

// Ticket T2: definitions for the Tile value class. Kept deliberately
// simple: kind + glyph. Extension point (spec §8, Aufgabe 1c): a later
// item payload can be added as a new private member here without
// changing any Map-facing API.

namespace p2game {

namespace {

// FR-5 glyph legend: Start='s', Exit='e', Blocked='b', Traversable='x',
// TreasureChest='t' (ticket T2b / Aufgabe 1c; ADR 0006). The glyph is
// NOT affected by whether an item view is set.
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
        case TileKind::TreasureChest:
            return 't';
    }
    return 'x';
}

}  // namespace

Tile::Tile() : kind_(TileKind::Traversable), glyph_(legendGlyph(kind_)), item_(nullptr) {}

Tile::Tile(TileKind kind) : kind_(kind), glyph_(legendGlyph(kind)), item_(nullptr) {}

// Copies copy the (non-owning) item view pointer: value-type semantics,
// ownership never changes hands (ADR 0006).
Tile::Tile(const Tile& other) : kind_(other.kind_), glyph_(other.glyph_), item_(other.item_) {}

Tile& Tile::operator=(const Tile& other) {
    kind_ = other.kind_;
    glyph_ = other.glyph_;
    item_ = other.item_;
    return *this;
}

TileKind Tile::kind() const {
    return kind_;
}

char Tile::glyph() const {
    return glyph_;
}

const ItemBase* Tile::item() const {
    return item_;
}

void Tile::setItem(const ItemBase* item) {
    item_ = item;
}

}  // namespace p2game
