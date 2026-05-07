#include "Piece.h"
#include <cmath>
#include <cstring>


// base piece
Piece::Piece(int x, int y, PlayerColor color, PieceType type)
    : x(x), y(y), color(color), type(type), hasMoved(false) {
}

int  Piece::getX() const { return x; }
int  Piece::getY() const { return y; }
PlayerColor Piece::getColor() const { return color; }
bool Piece::getHasMoved() const { return hasMoved; }
void Piece::setMoved() { hasMoved = true; }
void Piece::setPosition(int nx, int ny) { x = nx; y = ny; }

bool Piece::isEnemy(const Piece* other) const {
    if (!other) return false;
    return other->color != color;
}

// Used by Rook, Bishop, Queen to check if the path is clear
bool Piece::isPathClear(int nx, int ny, Piece* board[8][8]) const {
    int dx = (nx - x == 0) ? 0 : (nx - x > 0 ? 1 : -1);
    int dy = (ny - y == 0) ? 0 : (ny - y > 0 ? 1 : -1);

    int cx = x + dx, cy = y + dy;
    while (cx != nx || cy != ny) {
        if (board[cx][cy] != nullptr) return false;
        cx += dx;
        cy += dy;
    }
    return true;
}