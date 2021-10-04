#pragma once
#include "stdafx.h"
#include "GLBase.h"
#include "Connection.h"

class Game;

class Cell : public IDrawable {
	enum Piece { None, Black, White, N2B, N2W, W2B, B2W } piece;
	enum Status { Normal, Hover, Press, Dropable, toN, toH, toP, toD } status;
	GLdouble cX, cY;
	GLdouble pieceGrey;
	GLdouble piecePhi;
	GLdouble pieceSize;
	void n2b();	// None to Black
	void n2w();	// None to White
	void w2b();	// White to Black
	void b2w();	// Black to White
	std::array<GLfloat, 4> gAD;
	GLdouble N[5][3] = {
		{ +0.0, +0.0, +1.0 },
		{ -1.0, +0.0, +0.0 },
		{ +0.0, -1.0, +0.0 },
		{ +1.0, +0.0, +0.0 },
		{ +0.0, +1.0, +0.0 },
	};
	GLdouble V[20][3] = {
		{ -4.8, +4.8, +0.0 }, { -4.8, -4.8, +0.0 }, { +4.8, -4.8, +0.0 }, { +4.8, +4.8, +0.0 },
		{ -4.8, +4.8, +0.0 }, { -4.8, +4.8, -2.0 }, { -4.8, -4.8, -2.0 }, { -4.8, -4.8, +0.0 },
		{ -4.8, -4.8, +0.0 }, { -4.8, -4.8, -2.0 }, { +4.8, -4.8, -2.0 }, { +4.8, -4.8, +0.0 },
		{ +4.8, -4.8, +0.0 }, { +4.8, -4.8, -2.0 }, { +4.8, +4.8, -2.0 }, { +4.8, +4.8, +0.0 },
		{ +4.8, +4.8, +0.0 }, { +4.8, +4.8, -2.0 }, { -4.8, +4.8, -2.0 }, { -4.8, +4.8, +0.0 }
	};
	const std::array<GLfloat, 4> gColor[4] = {
		{ 0.8f, 0.6f, 0.3f, 0.5f },
		{ 0.9f, 0.8f, 0.5f, 0.5f },
		{ 0.4f, 0.3f, 0.1f, 0.5f },
		{ 0.3f, 0.9f, 0.3f, 0.5f }
	};

	const double af;	// animation frame
	void chStatus(Status src, Status dst);
	bool downFlag, clickedFlag, dropableFlag;
public:
	void draw() override;
	bool isNone();
	bool isBlack();
	bool isWhite();
	bool operator==(Cell &c);
	bool operator!=(Cell &c);
	void toBlack();
	void toWhite();

	void toNormal();
	void toHover();
	void toPress();
	void toDropable();
	bool clicked();
	void showDropable(bool show);
	void handleMouse(int button, int x, int y);
	Cell(GLdouble centerX, GLdouble centerY);
};

class ChessBoard : public IDrawable {
public:
	struct Point {
		Point(int x, int y) : x(x), y(y) {}
		int x, y;
	};
	void draw() override;
};

class Reversi {
public:
	static Game *game;
	static std::mutex gameLock;
	static void localGameStart(const TCHAR *bName, char bType, const TCHAR *wName, char wType);
	static void lanGameStart(SOCKET cs, const TCHAR *myName, char myType, const TCHAR *opName, bool imWhite);
};

class Player {
public:
	TCHAR name[MAX_NAMESTRING + 1];
	enum Role { Black, White } role;
	enum Type { Human, AI, Remote } type;
	virtual ChessBoard::Point drop() = 0;
};

class HumanPlayer : public Player {
public:
	ChessBoard::Point drop() override;
	HumanPlayer();
};

class AIPlayer : public Player {
public:
	ChessBoard::Point drop() override;
	AIPlayer();
};

class RemotePlayer : public Player {
public:
	ChessBoard::Point drop() override;
	RemotePlayer();
};

class Situation : public IDrawable {
public:
	bool wTurn;
	int black, white;
	void draw() override;
};

class Game {
	Player *pb, *pw;
	bool wTurn;
public:
	bool endSignal;
	SOCKET sock;
	Situation situ;
	ChessBoard *cb;
	Cell *cp[8][8];
	std::vector<ChessBoard::Point> wDropables, bDropables;
	void start();
	void end(bool showResult);
	std::vector<ChessBoard::Point> dropWillRvrs(ChessBoard::Point p, bool white);
	int dropWillRvrsNum(ChessBoard::Point p, bool white);
	std::vector<ChessBoard::Point> getDropables(bool white);
	void updateSituation(bool wTurn);
	Game(SOCKET cs, const TCHAR *myName, char myType, const TCHAR *opName, bool imWhite);
	Game(const TCHAR *bName, char bType, const TCHAR *wName, char wType);
	~Game();
};
