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

// board
chessBoard::chessBoard()
    : currentTurn(WHITE), selectedPiece(nullptr),
    selectedX(-1), selectedY(-1), gameOver(false), winner(WHITE)
{
    clearBoard();
    for (int t = 0; t < 6; t++)
        for (int c = 0; c < 2; c++)
            texturesLoaded[t][c] = false;
}

chessBoard::~chessBoard() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j]) { delete grid[i][j]; grid[i][j] = nullptr; }
}

void chessBoard::clearBoard() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = nullptr;
}

void chessBoard::placePiece(Piece* p) {
    grid[p->getX()][p->getY()] = p;
}

bool chessBoard::isInBounds(int x, int y) const {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// init standard chess layout
void chessBoard::initPieces() {
    // Clear any existing pieces
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (grid[i][j]) { delete grid[i][j]; grid[i][j] = nullptr; }

    // White back rank (row 7)
    placePiece(new Rook(0, 7, WHITE));
    placePiece(new Knight(1, 7, WHITE));
    placePiece(new Bishop(2, 7, WHITE));
    placePiece(new Queen(3, 7, WHITE));
    placePiece(new King(4, 7, WHITE));
    placePiece(new Bishop(5, 7, WHITE));
    placePiece(new Knight(6, 7, WHITE));
    placePiece(new Rook(7, 7, WHITE));

    // White pawns (row 6)
    for (int i = 0; i < 8; i++)
        placePiece(new Pawn(i, 6, WHITE));

    // Black back rank (row 0)
    placePiece(new Rook(0, 0, BLACK));
    placePiece(new Knight(1, 0, BLACK));
    placePiece(new Bishop(2, 0, BLACK));
    placePiece(new Queen(3, 0, BLACK));
    placePiece(new King(4, 0, BLACK));
    placePiece(new Bishop(5, 0, BLACK));
    placePiece(new Knight(6, 0, BLACK));
    placePiece(new Rook(7, 0, BLACK));

    // Black pawns (row 1)
    for (int i = 0; i < 8; i++)
        placePiece(new Pawn(i, 1, BLACK));
}

// texture loading
void chessBoard::loadTexture(Piece* p) {
    int t = (int)p->getType();
    int c = (int)p->getColor();

    if (texturesLoaded[t][c]) return;

    string path = (c == WHITE) ? p->getWhiteTexturePath() : p->getBlackTexturePath();

    if (!textures[t][c].loadFromFile(path))
        cerr << "Failed to load: " << path << "\n";
    else
        texturesLoaded[t][c] = true;
}

Texture* chessBoard::getTexture(Piece* p) {
    loadTexture(p);
    return &textures[(int)p->getType()][(int)p->getColor()];
}


// simulate move to check if it leaves king in check
bool chessBoard::wouldLeaveKingInCheck(int fx, int fy, int tx, int ty, PlayerColor color) const {
    // Temporarily make the move
    Piece* temp = grid[tx][ty];
    Piece* mover = grid[fx][fy];

    const_cast<chessBoard*>(this)->grid[tx][ty] = mover;
    const_cast<chessBoard*>(this)->grid[fx][fy] = nullptr;
    if (mover) const_cast<Piece*>(mover)->setPosition(tx, ty);

    bool inCheck = isKingInCheck(color);

    // Undo the move
    const_cast<chessBoard*>(this)->grid[fx][fy] = mover;
    const_cast<chessBoard*>(this)->grid[tx][ty] = temp;
    if (mover) const_cast<Piece*>(mover)->setPosition(fx, fy);

    return inCheck;
}
