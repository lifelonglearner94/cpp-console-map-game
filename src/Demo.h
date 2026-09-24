#ifndef P2GAME_DEMO_H
#define P2GAME_DEMO_H

// Ticket T6 / ADR 0004 (console-first rendering): the Aufgabe 1b demo
// entry point. The demo writes its entire console session (legend,
// Start/Exit coordinates, rendered maps) to the given ostream and
// returns the process exit code, so src/main.cpp stays a thin wrapper
// calling run_demo(std::cout) and tests can drive the demo through a
// plain ostream. No raylib, no windowing (ADR 0004).

#include <ostream>

namespace p2game {

// Runs one full console demo session on `out` and returns 0 on success.
int run_demo(std::ostream& out);

}  // namespace p2game

#endif  // P2GAME_DEMO_H
