#ifndef P2GAME_TILEKIND_H
#define P2GAME_TILEKIND_H

// Ticket T2 / FR-1: the four kinds a map tile can have. Enum-only header
// (permitted exception to one-class-per-header, ADR 0005): pure enum, no
// includes, so it can be included from anywhere.

namespace p2game {

enum class TileKind { Start, Exit, Blocked, Traversable };

}  // namespace p2game

#endif  // P2GAME_TILEKIND_H
