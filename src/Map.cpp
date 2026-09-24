#include "Map.h"

// Ticket T3: definitions for the Map class. A plain rows x cols grid of
// Tile value cells plus the traversal-query surface; deliberately no
// generation or pathfinding logic here (spec architecture boundary; 4a
// separation rule).

#include <cstddef>
#include <string>
#include <vector>

namespace p2game {

Map::Map(std::size_t rows, std::size_t cols)
    : rows_(rows), cols_(cols), grid_(rows, std::vector<tile_type>(cols)) {}

std::size_t Map::rows() const {
    return rows_;
}

std::size_t Map::cols() const {
    return cols_;
}

Tile& Map::at(std::size_t row, std::size_t col) {
    return grid_[row][col];
}

const Tile& Map::at(std::size_t row, std::size_t col) const {
    return grid_[row][col];
}

bool Map::isTraversable(std::size_t row, std::size_t col) const {
    if (row >= rows_ || col >= cols_) {
        return false;
    }
    return grid_[row][col].kind() != TileKind::Blocked;
}

std::string Map::toString() const {
    std::string rendered;
    for (std::size_t row = 0; row < rows_; ++row) {
        for (std::size_t col = 0; col < cols_; ++col) {
            rendered += grid_[row][col].glyph();
        }
        if (row + 1 < rows_) {
            rendered += '\n';
        }
    }
    return rendered;
}

}  // namespace p2game
