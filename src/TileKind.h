#ifndef P2GAME_TILEKIND_H
#define P2GAME_TILEKIND_H

// Ticket T2 / FR-1: the four kinds a map tile can have. Ticket T2b /
// Aufgabe 1c adds the fifth kind TreasureChest (ADR 0006). Enum-only
// header (permitted exception to one-class-per-header, ADR 0005): pure
// enum, no includes, so it can be included from anywhere.

namespace p2game {

enum class TileKind { Start, Exit, Blocked, Traversable, TreasureChest };

}  // namespace p2game

#endif  // P2GAME_TILEKIND_H
