#pragma once

#include <iostream>

using namespace std;

// constants
const unsigned int TILE_SIZE = 80;
const unsigned int BOARD_SIZE = 8;

// enums
enum PlayerColor { WHITE, BLACK };
enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN_TYPE, NONE_TYPE };

// abstract base class
class Piece {
protected:
	int x, y;
	PlayerColor color;
	bool hasMoved;
	PieceType type;

public:
	Piece(int x, int y, PlayerColor color, PieceType type);
	virtual ~Piece() {}

	// pure virtuals - every piece MUST implement these
	virtual bool isMoveValid(int nx, int ny, Piece* board[8][8]) const = 0;
	virtual string getWhiteTexturePath() const = 0;
	virtual string getBlackTexturePath() const = 0;
	virtual PieceType getType() const = 0;

	// concrete getters/setters
	int getX() const;
	int getY() const;
	PlayerColor getColor() const;
	bool getHasMoved() const;
	void setPosition(int nx, int ny);
	void setMoved();

	// utility
	bool isEnemy(const Piece* other) const;
	bool isPathClear(int nx, int ny, Piece* board[8][8]) const; // for sliding pieces
};
