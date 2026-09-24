// Demo unit tests for ticket T6 (p2game portfolio project, Aufgabe 1b).
//
// Pins the console demo contract of ADR 0004 at the planned public seam
// of the future p2game demo module:
//
//   int run_demo(std::ostream& out);
//
// i.e. the demo writes its entire console session to the given ostream
// and returns the process exit code, so this test exercises the demo
// without spawning a process; src/main.cpp is expected to be a thin
// wrapper calling run_demo(std::cout).
//
// Output-format contract pinned here (loose on wording, firm on
// substance), so the implementer knows exactly what must hold:
//
//   1. run_demo returns exactly 0.
//   2. A legend line explaining the glyph set is printed BEFORE the
//      first rendered map row; it names all four glyphs, containing
//      each of the substrings "s=Start", "e=Exit", "b=Blocked",
//      "x=Traversable" (FR-8, FR-5). Surrounding wording is free.
//   3. For each rendered map, the Start and Exit coordinates are
//      reported as (row, col) integers: some line mentions "Start" and
//      carries the row/column as the first two integer runs after the
//      keyword, and likewise for "Exit". The integers must equal the
//      actual position of the 's' / 'e' glyph in the rendered block
//      (FR-8 + FR-4 mirrored). Exact phrasing/parenthesisation is the
//      implementer's choice ("Start: (0, 4)" and "Start at row 0, col 4"
//      both parse); a demo without usable integer coordinates fails.
//   4. Exactly TWO map blocks are rendered (FR-9): each is 15 rows of
//      exactly 15 characters drawn only from {s,e,b,x}; each block's
//      first row contains exactly one 's' and its last row exactly one
//      'e' (FR-4 mirrored at the demo level).
//   5. Reproducibility + randomness (FR-12, FR-10): across two fresh
//      run_demo calls one map block is byte-identical in both (the
//      fixed-seed map) while the other block differs (the
//      random_device-seeded map). Within one call the two blocks also
//      differ. The fixed-seed check is order-agnostic (exactly one of
//      the two block pairings is reproduced) so it does not over-pin
//      the print order. NOTE: two independent random_device draws
//      colliding on the identical 15x15 map is astronomically
//      improbable (~one in far more than 10^30 for these grids), so
//      asserting "differs" has no realistic false-failure risk.
//   6. Console output is plain ASCII: no ANSI escape sequences
//      anywhere, and no map-row character outside {s,e,b,x}.
//
// src/Demo.h does not exist yet, so the expected RED state is a compile
// error on that include (the guarded CMake link keeps the build
// configurable until the implementation lands).
//
// Hand-rolled assert-style main: the project uses no third-party
// libraries (GR-4).

#include <cctype>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Demo.h"

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

// Splits on '\n'; a trailing empty piece (output ending in a newline)
// is dropped, interior empty lines are kept.
std::vector<std::string> split_lines(const std::string& text) {
    std::vector<std::string> lines;
    std::string current;
    for (const char c : text) {
        if (c == '\n') {
            lines.push_back(current);
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    if (!current.empty()) {
        lines.push_back(current);
    }
    return lines;
}

bool is_map_glyph(const char c) {
    return c == 's' || c == 'e' || c == 'b' || c == 'x';
}

// A rendered map row per the contract: exactly 15 characters, all of
// them legend glyphs.
bool is_map_line(const std::string& line) {
    if (line.size() != 15) {
        return false;
    }
    for (const char c : line) {
        if (!is_map_glyph(c)) {
            return false;
        }
    }
    return true;
}

std::size_t count_glyph(const std::string& line, const char glyph) {
    std::size_t count = 0;
    for (const char c : line) {
        if (c == glyph) {
            ++count;
        }
    }
    return count;
}

// Parses the first two integer runs after the first occurrence of
// `keyword` on the line. "Start: (0, 4)" and "Start at row 0 col 4"
// both yield (0, 4). Returns false when the keyword is absent or fewer
// than two integers follow it. Runs are capped at 9 digits so a
// pathological line cannot overflow the parse.
bool coords_after(const std::string& line, const char* keyword,
                  long long& row, long long& col) {
    const std::size_t keyword_pos = line.find(keyword);
    if (keyword_pos == std::string::npos) {
        return false;
    }
    const std::string tail = line.substr(keyword_pos + std::strlen(keyword));
    long long numbers[2] = {0, 0};
    int found = 0;
    std::size_t i = 0;
    while (i < tail.size() && found < 2) {
        if (std::isdigit(static_cast<unsigned char>(tail[i])) != 0) {
            std::size_t run = i;
            while (run < tail.size() && run - i < 9
                       && std::isdigit(static_cast<unsigned char>(tail[run])) != 0) {
                ++run;
            }
            numbers[found] = std::stoll(tail.substr(i, run - i));
            ++found;
            i = run;
        } else {
            ++i;
        }
    }
    if (found < 2) {
        return false;
    }
    row = numbers[0];
    col = numbers[1];
    return true;
}

// All rendered map rows in the session, in order of appearance.
std::vector<std::string> map_rows(const std::string& output) {
    std::vector<std::string> rows;
    for (const std::string& line : split_lines(output)) {
        if (is_map_line(line)) {
            rows.push_back(line);
        }
    }
    return rows;
}

// True when at least one line with index < before_index contains needle.
bool has_line_before(const std::vector<std::string>& lines,
                     const std::size_t before_index, const std::string& needle) {
    for (std::size_t i = 0; i < before_index && i < lines.size(); ++i) {
        if (lines[i].find(needle) != std::string::npos) {
            return true;
        }
    }
    return false;
}

// True when the 15-row block at a_offset differs from the 15-row block
// at b_offset; false when the two blocks are byte-identical.
bool blocks_differ(const std::vector<std::string>& rows, const std::size_t a_offset,
                   const std::vector<std::string>& other, const std::size_t b_offset) {
    for (std::size_t i = 0; i < 15; ++i) {
        if (rows[a_offset + i] != other[b_offset + i]) {
            return true;
        }
    }
    return false;
}

}  // namespace

int main() {
    // First demo session: everything below inspects this output.
    std::ostringstream first_out;
    const int first_rc = p2game::run_demo(first_out);
    const std::string first_output = first_out.str();
    const std::vector<std::string> first_lines = split_lines(first_output);
    const std::vector<std::string> first_rows = map_rows(first_output);

    // Acceptance criterion 1: the demo's exit code is exactly 0.
    expect_true(first_rc == 0, "run_demo returns exactly 0");

    // Acceptance criterion 2 (FR-8): a legend naming all four glyphs is
    // printed before the first rendered map row.
    {
        std::size_t first_map_index = first_lines.size();
        for (std::size_t i = 0; i < first_lines.size(); ++i) {
            if (is_map_line(first_lines[i])) {
                first_map_index = i;
                break;
            }
        }
        const bool legend_ok
            = has_line_before(first_lines, first_map_index, "s=Start")
                  && has_line_before(first_lines, first_map_index, "e=Exit")
                  && has_line_before(first_lines, first_map_index, "b=Blocked")
                  && has_line_before(first_lines, first_map_index, "x=Traversable");
        expect_true(legend_ok,
                    "a legend naming s=Start, e=Exit, b=Blocked, x=Traversable precedes the first map row (FR-8)");
    }

    // Acceptance criterion 4 (FR-9, FR-4 mirrored): exactly two map
    // blocks are rendered; each block's first row holds exactly one 's'
    // and its last row exactly one 'e'. The glyph set of every row is
    // enforced by is_map_line itself (criterion 6).
    expect_true(first_rows.size() == 30,
                "the demo renders exactly two 15x15 map blocks (FR-9)");
    bool fr4_ok = first_rows.size() == 30;
    // Actual glyph positions, per block: first = (row, col) of 's' in
    // the block's first row, second = (row, col) of 'e' in its last row.
    std::vector<std::pair<long long, long long>> start_positions(
        2, std::make_pair(-1LL, -1LL));
    std::vector<std::pair<long long, long long>> exit_positions(
        2, std::make_pair(-1LL, -1LL));
    if (first_rows.size() == 30) {
        for (int block = 0; block < 2; ++block) {
            const std::size_t offset = static_cast<std::size_t>(block) * 15;
            const std::string& top_row = first_rows[offset];
            const std::string& bottom_row = first_rows[offset + 14];
            const std::size_t s_col = top_row.find('s');
            const std::size_t e_col = bottom_row.find('e');
            if (count_glyph(top_row, 's') != 1) {
                fr4_ok = false;
            }
            if (count_glyph(bottom_row, 'e') != 1) {
                fr4_ok = false;
            }
            start_positions[block] = std::make_pair(
                0LL, static_cast<long long>(s_col));
            exit_positions[block] = std::make_pair(
                14LL, static_cast<long long>(e_col));
        }
    }
    expect_true(fr4_ok,
                "each map block's first row holds exactly one 's' and its last row exactly one 'e' (FR-4 mirrored)");

    // Acceptance criterion 3 (FR-8): the Start and Exit coordinates of
    // each rendered map are reported as (row, col) integers that match
    // the rendered glyphs. Order-agnostic about where the coordinate
    // lines sit relative to the map blocks.
    bool coords_ok = first_rows.size() == 30;
    if (coords_ok) {
        for (int block = 0; block < 2; ++block) {
            bool start_reported = false;
            bool exit_reported = false;
            for (const std::string& line : first_lines) {
                long long row = 0;
                long long col = 0;
                if (!start_reported && coords_after(line, "Start", row, col)
                        && row == start_positions[block].first
                        && col == start_positions[block].second) {
                    start_reported = true;
                }
                if (!exit_reported && coords_after(line, "Exit", row, col)
                        && row == exit_positions[block].first
                        && col == exit_positions[block].second) {
                    exit_reported = true;
                }
            }
            if (!start_reported || !exit_reported) {
                coords_ok = false;
            }
        }
    }
    expect_true(coords_ok,
                "each rendered map has its Start and Exit (row, col) coordinates reported as integers matching the glyphs (FR-8)");

    // Acceptance criterion 6 (FR-8, ADR 0004 console-only): plain ASCII
    // session -- no ANSI escape sequences anywhere in the output, and
    // no map-row character outside the legend glyph set.
    bool ascii_ok = first_output.find('\x1b') == std::string::npos;
    for (const std::string& row : first_rows) {
        for (const char c : row) {
            if (!is_map_glyph(c)) {
                ascii_ok = false;
            }
        }
    }
    expect_true(ascii_ok,
                "the console session is plain ASCII: no ANSI escapes, map glyphs only from {s,e,b,x} (FR-8)");

    // Acceptance criterion 5 (FR-12, FR-10): a second, fresh session
    // reproduces the fixed-seed map byte-identically while the
    // random_device-seeded map differs. Order-agnostic: exactly one of
    // the two block pairings is reproduced, and within one session the
    // two blocks differ from each other (see the header note on the
    // astronomically small false-failure risk of the "differs" checks).
    std::ostringstream second_out;
    const int second_rc = p2game::run_demo(second_out);
    const std::vector<std::string> second_rows = map_rows(second_out.str());
    expect_true(second_rc == 0, "a second run_demo call also returns exactly 0");
    expect_true(second_rows.size() == 30,
                "the second session also renders exactly two 15x15 map blocks (FR-9)");
    const bool first_pair_identical
        = first_rows.size() == 30 && second_rows.size() == 30
              && !blocks_differ(first_rows, 0, second_rows, 0);
    const bool second_pair_identical
        = first_rows.size() == 30 && second_rows.size() == 30
              && !blocks_differ(first_rows, 15, second_rows, 15);
    expect_true(first_rows.size() == 30 && second_rows.size() == 30
                    && (first_pair_identical != second_pair_identical),
                "across two calls the fixed-seed map block is identical and the random_device-seeded block differs (FR-12, FR-10)");
    expect_true(first_rows.size() == 30
                    && blocks_differ(first_rows, 0, first_rows, 15),
                "within one session the fixed-seed and random_device-seeded blocks differ (FR-10)");

    if (g_failure_count == 0) {
        std::cout << "All demo tests passed.\n";
        return 0;
    }
    std::cerr << g_failure_count << " demo test(s) failed.\n";
    return 1;
}
