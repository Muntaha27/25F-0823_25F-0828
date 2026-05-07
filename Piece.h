#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
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

// derived classes
class King : public Piece {
public:
	King(int x, int y, PlayerColor color);
	bool isMoveValid(int nx, int ny, Piece* board[8][8]) const override;
	string getWhiteTexturePath() const override;
	string getBlackTexturePath() const override;
	PieceType getType() const override;
};

class Queen : public Piece {
public:
	Queen(int x, int y, PlayerColor color);
	bool isMoveValid(int nx, int ny, Piece* board[8][8]) const override;
	string getWhiteTexturePath() const override;
	string getBlackTexturePath() const override;
	PieceType getType() const override;
};

class Rook : public Piece {
public:
	Rook(int x, int y, PlayerColor color);
	bool isMoveValid(int nx, int ny, Piece* board[8][8]) const override;
	string getWhiteTexturePath() const override;
	string getBlackTexturePath() const override;
	PieceType getType() const override;
};

class Bishop : public Piece {
public:
	Bishop(int x, int y, PlayerColor color);
	bool isMoveValid(int nx, int ny, Piece* board[8][8]) const override;
	string getWhiteTexturePath() const override;
	string getBlackTexturePath() const override;
	PieceType getType() const override;
};

class Knight : public Piece {
public:
	Knight(int x, int y, PlayerColor color);
	bool isMoveValid(int nx, int ny, Piece* board[8][8]) const override;
	string getWhiteTexturePath() const override;
	string getBlackTexturePath() const override;
	PieceType getType() const override;
};

class Pawn : public Piece {
public:
	Pawn(int x, int y, PlayerColor color);
	bool isMoveValid(int nx, int ny, Piece* board[8][8]) const override;
	string getWhiteTexturePath() const override;
	string getBlackTexturePath() const override;
	PieceType getType() const override;
};

// board class (owns all pieces + logic)
class chessBoard {
private:
	Piece* grid[8][8];
	PlayerColor currentTurn;
	Piece* selectedPiece;
	int selectedX, selectedY;
	bool gameOver;
	PlayerColor winner;

	// texture cache (one per piece type per color)
	Texture textures[6][2]; // [PieceType][color]
	bool texturesLoaded[6][2];

	// internal helpers
	void clearBoard();
	void placePiece(Piece* p);
	bool isKingInCheck(PlayerColor color) const;
	bool hasAnyValidMove(PlayerColor color) const;
	bool wouldLeaveKingInCheck(int fx, int fy, int tx, int ty, PlayerColor color) const;
	Texture* getTexture(Piece* p);
	void loadTexture(Piece* p);
	bool isInBounds(int x, int y) const;

public:
	chessBoard();
	~chessBoard();

	void initPieces();
	void handleClick(int pixelX, int pixelY);
	bool movePiece(int fx, int fy, int tx, int ty);
	void drawBoard(RenderWindow& window);
	void drawHighlights(RenderWindow& window);
	void drawPieces(RenderWindow& window);

	bool isGameOver() const;
	PlayerColor getCurrentTurn() const;
	PlayerColor getWinner() const;
};
