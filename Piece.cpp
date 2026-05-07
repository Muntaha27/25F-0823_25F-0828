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

// king
King::King(int x, int y, PlayerColor color) : Piece(x, y, color, KING) {}

bool King::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    int dx = abs(nx - x), dy = abs(ny - y);
    if (dx > 1 || dy > 1 || (dx == 0 && dy == 0)) return false;
    if (board[nx][ny] && !isEnemy(board[nx][ny]))   return false;
    return true;
}
string King::getWhiteTexturePath() const { return "pieces/white/wk.png"; }
string King::getBlackTexturePath() const { return "pieces/black/bk.png"; }
PieceType King::getType() const { return KING; }

// queen
Queen::Queen(int x, int y, PlayerColor color) : Piece(x, y, color, QUEEN) {}

bool Queen::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    if (x == nx && y == ny) return false;

    if (board[nx][ny] && !isEnemy(board[nx][ny])) return false;

    int dx = abs(nx - x), dy = abs(ny - y);

    bool straight = (x == nx || y == ny);
    bool diagonal = (dx == dy);
    if (!straight && !diagonal) return false;

    return isPathClear(nx, ny, board);
}
string Queen::getWhiteTexturePath() const { return "pieces/white/wq.png"; }
string Queen::getBlackTexturePath() const { return "pieces/black/bq.png"; }
PieceType Queen::getType() const { return QUEEN; }

// rook
Rook::Rook(int x, int y, PlayerColor color) : Piece(x, y, color, ROOK) {}

bool Rook::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    if (x == nx && y == ny) return false;
    if (board[nx][ny] && !isEnemy(board[nx][ny])) return false;
    if (x != nx && y != ny) return false; // must be straight
    return isPathClear(nx, ny, board);
}
string Rook::getWhiteTexturePath() const { return "pieces/white/wr.png"; }
string Rook::getBlackTexturePath() const { return "pieces/black/br.png"; }
PieceType Rook::getType() const { return ROOK; }

// bishop
Bishop::Bishop(int x, int y, PlayerColor color) : Piece(x, y, color, BISHOP) {}

bool Bishop::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    if (x == nx && y == ny) return false;
    if (board[nx][ny] && !isEnemy(board[nx][ny])) return false;
    if (abs(nx - x) != abs(ny - y)) return false; // must be diagonal
    return isPathClear(nx, ny, board);
}
string Bishop::getWhiteTexturePath() const { return "pieces/white/wb.png"; }
string Bishop::getBlackTexturePath() const { return "pieces/black/bb.png"; }
PieceType Bishop::getType() const { return BISHOP; }

// knight
Knight::Knight(int x, int y, PlayerColor color) : Piece(x, y, color, KNIGHT) {}

bool Knight::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    if (board[nx][ny] && !isEnemy(board[nx][ny])) return false;
    int dx = abs(nx - x), dy = abs(ny - y);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}
string Knight::getWhiteTexturePath() const { return "pieces/white/wn.png"; }
string Knight::getBlackTexturePath() const { return "pieces/black/bn.png"; }
PieceType Knight::getType() const { return KNIGHT; }

// pawn
Pawn::Pawn(int x, int y, PlayerColor color) : Piece(x, y, color, PAWN_TYPE) {}

bool Pawn::isMoveValid(int nx, int ny, Piece* board[8][8]) const {
    int dir = (color == WHITE) ? -1 : 1; // White moves up (decreasing y), Black moves down
    int dx = nx - x;
    int dy = ny - y;

    // One step forward
    if (dx == 0 && dy == dir && !board[nx][ny])
        return true;

    // Two steps from starting row
    int startRow = (color == WHITE) ? 6 : 1;
    if (dx == 0 && dy == 2 * dir && y == startRow && !board[nx][ny] && !board[x][y + dir])
        return true;

    // Diagonal capture
    if (abs(dx) == 1 && dy == dir && board[nx][ny] && isEnemy(board[nx][ny]))
        return true;

    return false;
}
string Pawn::getWhiteTexturePath() const { return "pieces//white//wp.png"; }
string Pawn::getBlackTexturePath() const { return "pieces//black//bp.png"; }
PieceType Pawn::getType() const { return PAWN_TYPE; }
