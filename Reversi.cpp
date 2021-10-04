#include "stdafx.h"
#include "Reversi.h"
#include "MyVec3Algorithm.h"

Game *Reversi::game;
std::mutex Reversi::gameLock;

void Reversi::localGameStart(const TCHAR *bName, char bType, const TCHAR *wName, char wType) {
	GLBase::drawLock.lock();
	if (game) {
		game->end(false);
		gameLock.lock();
		delete game;
		gameLock.unlock();
	}
	game = new Game(bName, bType, wName, wType);
	GLBase::drawLock.unlock();
	game->start();
}

void Reversi::lanGameStart(SOCKET cs, const TCHAR *myName, char myType, const TCHAR *opName, bool imWhite) {
	GLBase::drawLock.lock();
	if (game) {
		game->end(false);
		gameLock.lock();
		delete game;
		gameLock.unlock();
	}
	game = new Game(cs, myName, myType, opName, imWhite);
	GLBase::drawLock.unlock();
	game->start();
}

void ChessBoard::draw() {
	GLfloat ambdif[] = { 0.8f, 0.8f, 0.8f, 0.5f };
	GLfloat spec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	static GLuint list;
	if (list) {
		glCallList(list);
	} else {
		list = glGenLists(1);
		glNewList(list, GL_COMPILE_AND_EXECUTE);

		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambdif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		glMateriali(GL_FRONT, GL_SHININESS, 30);

		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, GLBase::tex[0]);
		glBegin(GL_QUADS);
		glNormal3d(+0.0, +0.0, -1.0);	// bottom
		glTexCoord2d(0.00, 0.00); glVertex3d(+44.0, +44.0, -4.0);
		glTexCoord2d(0.00, 1.00); glVertex3d(-44.0, +44.0, -4.0);
		glTexCoord2d(1.00, 1.00); glVertex3d(-44.0, -44.0, -4.0);
		glTexCoord2d(1.00, 0.00); glVertex3d(+44.0, -44.0, -4.0);
		glNormal3d(-1.0, +0.0, +0.0);	// outer west
		glTexCoord2d(0.82, 0.00); glVertex3d(-44.0, +44.0, +0.0);
		glTexCoord2d(0.73, 0.00); glVertex3d(-44.0, +44.0, -4.0);
		glTexCoord2d(0.73, 1.00); glVertex3d(-44.0, -44.0, -4.0);
		glTexCoord2d(0.82, 1.00); glVertex3d(-44.0, -44.0, +0.0);
		glTexCoord2d(0.00, 0.00); glVertex3d(+40.0, -40.0, +0.0);	// inner east
		glTexCoord2d(0.00, 0.10); glVertex3d(+40.0, -40.0, -2.0);
		glTexCoord2d(1.00, 0.10); glVertex3d(+40.0, +40.0, -2.0);
		glTexCoord2d(1.00, 0.00); glVertex3d(+40.0, +40.0, +0.0);
		glNormal3d(+0.0, -1.0, +0.0);	// outer south
		glTexCoord2d(0.00, 0.91); glVertex3d(-44.0, -44.0, +0.0);
		glTexCoord2d(1.00, 0.91); glVertex3d(-44.0, -44.0, -4.0);
		glTexCoord2d(0.91, 0.82); glVertex3d(+44.0, -44.0, -4.0);
		glTexCoord2d(0.09, 0.82); glVertex3d(+44.0, -44.0, +0.0);
		glTexCoord2d(0.00, 0.00); glVertex3d(+40.0, +40.0, +0.0);	// inner north
		glTexCoord2d(0.00, 1.00); glVertex3d(+40.0, +40.0, -2.0);
		glTexCoord2d(1.00, 1.00); glVertex3d(-40.0, +40.0, -2.0);
		glTexCoord2d(1.00, 0.00); glVertex3d(-40.0, +40.0, +0.0);
		glNormal3d(+1.0, +0.0, +0.0);	// outer east
		glTexCoord2d(0.55, 1.00); glVertex3d(+44.0, -44.0, +0.0);
		glTexCoord2d(0.64, 1.00); glVertex3d(+44.0, -44.0, -4.0);
		glTexCoord2d(0.64, 0.00); glVertex3d(+44.0, +44.0, -4.0);
		glTexCoord2d(0.55, 0.00); glVertex3d(+44.0, +44.0, +0.0);
		glTexCoord2d(0.00, 0.00); glVertex3d(-40.0, +40.0, +0.0);	// inner west
		glTexCoord2d(0.00, 1.00); glVertex3d(-40.0, +40.0, -2.0);
		glTexCoord2d(1.00, 1.00); glVertex3d(-40.0, -40.0, -2.0);
		glTexCoord2d(1.00, 0.00); glVertex3d(-40.0, -40.0, +0.0);
		glNormal3d(+0.0, +1.0, +0.0);	// outer north
		glTexCoord2d(1.00, 0.18); glVertex3d(+44.0, +44.0, +0.0);
		glTexCoord2d(1.00, 0.09); glVertex3d(+44.0, +44.0, -4.0);
		glTexCoord2d(0.00, 0.09); glVertex3d(-44.0, +44.0, -4.0);
		glTexCoord2d(0.00, 0.18); glVertex3d(-44.0, +44.0, +0.0);
		glTexCoord2d(0.00, 0.00); glVertex3d(-40.0, -40.0, +0.0);	// inner south
		glTexCoord2d(0.00, 1.00); glVertex3d(-40.0, -40.0, -2.0);
		glTexCoord2d(1.00, 1.00); glVertex3d(+40.0, -40.0, -2.0);
		glTexCoord2d(1.00, 0.00); glVertex3d(+40.0, -40.0, +0.0);
		glNormal3d(+0.0, +0.0, +1.0);	// up
		glTexCoord2d(0.91, 0.00); glVertex3d(+40.0, +40.0, -2.0);
		glTexCoord2d(0.09, 0.09); glVertex3d(-40.0, +40.0, -2.0);
		glTexCoord2d(0.09, 0.91); glVertex3d(-40.0, -40.0, -2.0);
		glTexCoord2d(0.91, 0.91); glVertex3d(+40.0, -40.0, -2.0);
		glTexCoord2d(0.82, 0.00); glVertex3d(-44.0, +44.0, +0.0);	// up west
		glTexCoord2d(0.82, 1.00); glVertex3d(-44.0, -44.0, +0.0);
		glTexCoord2d(0.91, 0.91); glVertex3d(-40.0, -40.0, +0.0);
		glTexCoord2d(0.91, 0.09); glVertex3d(-40.0, +40.0, +0.0);
		glTexCoord2d(0.00, 1.00); glVertex3d(-44.0, -44.0, +0.0);	// up south
		glTexCoord2d(1.00, 1.00); glVertex3d(+44.0, -44.0, +0.0);
		glTexCoord2d(0.91, 0.91); glVertex3d(+40.0, -40.0, +0.0);
		glTexCoord2d(0.09, 0.91); glVertex3d(-40.0, -40.0, +0.0);
		glTexCoord2d(0.46, 1.00); glVertex3d(+44.0, -44.0, +0.0);	// up east
		glTexCoord2d(0.46, 0.00); glVertex3d(+44.0, +44.0, +0.0);
		glTexCoord2d(0.55, 0.09); glVertex3d(+40.0, +40.0, +0.0);
		glTexCoord2d(0.55, 0.91); glVertex3d(+40.0, -40.0, +0.0);
		glTexCoord2d(1.00, 0.18); glVertex3d(+44.0, +44.0, +0.0);	// up north
		glTexCoord2d(0.00, 0.18); glVertex3d(-44.0, +44.0, +0.0);
		glTexCoord2d(0.09, 0.27); glVertex3d(-40.0, +40.0, +0.0);
		glTexCoord2d(0.91, 0.27); glVertex3d(+40.0, +40.0, +0.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glEndList();
	}
}

void Game::start() {
	cp[3][3]->toBlack();
	cp[4][4]->toBlack();
	cp[3][4]->toWhite();
	cp[4][3]->toWhite();

	wTurn = false;
	endSignal = false;
	Reversi::gameLock.lock();
	while (!endSignal) {
		wDropables = getDropables(true);
		bDropables = getDropables(false);
		if (wDropables.empty() && bDropables.empty()) {
			end(true);
			continue;
		} else if (wDropables.empty() && !bDropables.empty()) {
			wTurn = false;
		} else if (!wDropables.empty() && bDropables.empty()) {
			wTurn = true;
		}
		updateSituation(wTurn);
		if (wTurn) {
			auto dp = pw->drop();
			auto rvrs = dropWillRvrs(dp, true);
			if (rvrs.empty()) {
				for (auto p : wDropables) {
					cp[p.x][p.y]->showDropable(true);
				}
				continue;
			}
			cp[dp.x][dp.y]->toWhite();
			for (auto p : rvrs) {
				cp[p.x][p.y]->toWhite();
			}
			for (auto p : wDropables) {
				cp[p.x][p.y]->showDropable(false);
			}
			if (pw->type != Player::Remote && pb->type == Player::Remote) {
				char buf[2] = { (char)dp.x, (char)dp.y };
				send(sock, buf, 2, 0);
			}
		} else {
			auto dp = pb->drop();
			auto rvrs = dropWillRvrs(dp, false);
			if (rvrs.empty()) {
				for (auto p : bDropables) {
					cp[p.x][p.y]->showDropable(true);
				}
				continue;
			}
			cp[dp.x][dp.y]->toBlack();
			for (auto p : rvrs) {
				cp[p.x][p.y]->toBlack();
			}
			for (auto p : bDropables) {
				cp[p.x][p.y]->showDropable(false);
			}
			if (pb->type != Player::Remote && pw->type == Player::Remote) {
				char buf[2] = { (char)dp.x, (char)dp.y };
				if (send(sock, buf, 2, 0) == SOCKET_ERROR) {
					MessageBox(hMainWnd, _T("连接断开，游戏结束！"), _T("提示"), MB_OK);
					end(false);
				}
			}
		}
		wTurn = !wTurn;
		updateSituation(wTurn);
		Sleep(1000);
	}
	Reversi::gameLock.unlock();
}

void Game::end(bool showResult) {
	endSignal = true;
	closesocket(sock);
	if (!showResult) return;
	TCHAR str[100];
	_stprintf_s(str, _T("黑方 %d : %d 白方\n"), situ.black, situ.white);
	if (situ.black > situ.white) {
		_tcscat_s(str, _T("黑方胜！"));
	} else if (situ.black < situ.white) {
		_tcscat_s(str, _T("白方胜！"));
	} else {
		_tcscat_s(str, _T("平手！"));
	}
	MessageBox(hMainWnd, str, _T("游戏结果"), MB_OK | MB_ICONEXCLAMATION);
}

std::vector<ChessBoard::Point> Game::dropWillRvrs(ChessBoard::Point p, bool white) {
#define sameAsDrop (white ? cp[x][y]->isWhite() : cp[x][y]->isBlack())
#define oppositeDrop (white ? cp[x][y]->isBlack() : cp[x][y]->isWhite())
#define pointInRange (x >= 0 && x < 8 && y >= 0 && y < 8)
#define resetPoint (x = p.x, y = p.y)

	std::vector<ChessBoard::Point> ret;
	int x, y;
	if (p.x < 0 || p.x > 7 || p.y < 0 || p.y > 7) return ret;
	if (!cp[p.x][p.y]->isNone()) return ret;

	// up
	resetPoint;
	while (--y >= 0 && oppositeDrop);
	if (pointInRange && sameAsDrop)
		while (++y < p.y)
			ret.push_back(ChessBoard::Point(x, y));
	// down
	resetPoint;
	while (++y < 8 && oppositeDrop);
	if (pointInRange && sameAsDrop)
		while (--y > p.y)
			ret.push_back(ChessBoard::Point(x, y));
	// left
	resetPoint;
	while (--x >= 0 && oppositeDrop);
	if (pointInRange && sameAsDrop)
		while (++x < p.x)
			ret.push_back(ChessBoard::Point(x, y));
	// right
	resetPoint;
	while (++x < 8 && oppositeDrop);
	if (pointInRange && sameAsDrop)
		while (--x > p.x)
			ret.push_back(ChessBoard::Point(x, y));
	// leftup
	resetPoint;
	while (--x >= 0 && --y >= 0 && oppositeDrop);
	if (pointInRange && sameAsDrop)
		while (++x < p.x && ++y < p.y)
			ret.push_back(ChessBoard::Point(x, y));
	// rightup
	resetPoint;
	while (++x < 8 && --y >= 0 && oppositeDrop);
	if (pointInRange && sameAsDrop)
		while (--x > p.x && ++y < p.y)
			ret.push_back(ChessBoard::Point(x, y));
	// leftdown
	resetPoint;
	while (--x >= 0 && ++y < 8 && oppositeDrop);
	if (pointInRange && sameAsDrop)
		while (++x < p.x && --y > p.y)
			ret.push_back(ChessBoard::Point(x, y));
	// rightdown
	resetPoint;
	while (++x < 8 && ++y < 8 && oppositeDrop);
	if (pointInRange && sameAsDrop)
		while (--x > p.x && --y > p.y)
			ret.push_back(ChessBoard::Point(x, y));

	return ret;

#undef pointInRange
#undef sameAsDrop
#undef oppositeDrop
}

int Game::dropWillRvrsNum(ChessBoard::Point p, bool white) {
#define sameAsDrop (white ? cp[x][y]->isWhite() : cp[x][y]->isBlack())
#define oppositeDrop (white ? cp[x][y]->isBlack() : cp[x][y]->isWhite())
#define pointInRange (x >= 0 && x < 8 && y >= 0 && y < 8)
#define resetPoint (x = p.x, y = p.y)

	int x, y, ret = 0;

	if (!cp[p.x][p.y]->isNone()) return ret;

	// up
	resetPoint;
	while (--y >= 0 && oppositeDrop);
	if (pointInRange && sameAsDrop) ret += p.y - y - 1;
	// down
	resetPoint;
	while (++y < 8 && oppositeDrop);
	if (pointInRange && sameAsDrop) ret += y - p.y - 1;
	// left
	resetPoint;
	while (--x >= 0 && oppositeDrop);
	if (pointInRange && sameAsDrop) ret += p.x - x - 1;
	// right
	resetPoint;
	while (++x < 8 && oppositeDrop);
	if (pointInRange && sameAsDrop) ret += x - p.x - 1;
	// leftup
	resetPoint;
	while (--x >= 0 && --y >= 0 && oppositeDrop);
	if (pointInRange && sameAsDrop) ret += p.x - x - 1;
	// rightup
	resetPoint;
	while (++x < 8 && --y >= 0 && oppositeDrop);
	if (pointInRange && sameAsDrop) ret += p.y - y - 1;
	// leftdown
	resetPoint;
	while (--x >= 0 && ++y < 8 && oppositeDrop);
	if (pointInRange && sameAsDrop) ret += y - p.y - 1;
	// rightdown
	resetPoint;
	while (++x < 8 && ++y < 8 && oppositeDrop);
	if (pointInRange && sameAsDrop) ret += x - p.x - 1;

	return ret;

#undef pointInRange
#undef sameAsDrop
#undef oppositeDrop
}

std::vector<ChessBoard::Point> Game::getDropables(bool white) {
	std::vector<ChessBoard::Point> ret;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			ChessBoard::Point p(i, j);
			if (dropWillRvrsNum(p, white))
				ret.push_back(p);
		}
	}
	return ret;
}

void Game::updateSituation(bool wTurn) {
	situ.black = situ.white = 0;
	situ.wTurn = wTurn;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (cp[i][j]->isBlack()) situ.black++;
			else if (cp[i][j]->isWhite()) situ.white++;
		}
	}
}

Game::Game(SOCKET cs, const TCHAR *myName, char myType, const TCHAR *opName, bool imWhite) : sock(cs) {
	Player *p1, *p2;
	if (myType == 0) p1 = new HumanPlayer();
	else p1 = new AIPlayer();
	p2 = new RemotePlayer();
	_tcscpy_s(p1->name, myName);
	_tcscpy_s(p2->name, opName);
	if (imWhite) {
		p1->role = Player::White; pw = p1;
		p2->role = Player::Black; pb = p2;
	} else {
		p1->role = Player::Black; pb = p1;
		p2->role = Player::White; pw = p2;
	}
	cb = new ChessBoard();
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			cp[i][j] = new Cell(-35.0 + 10.0 * i, 35.0 - 10.0 * j);
}

Game::Game(const TCHAR *bName, char bType, const TCHAR *wName, char wType) {
	if (bType == 0) pb = new HumanPlayer();
	else pb = new AIPlayer();
	if (wType == 0) pw = new HumanPlayer();
	else pw = new AIPlayer();
	_tcscpy_s(pb->name, bName);
	_tcscpy_s(pw->name, wName);
	pb->role = Player::Black;
	pw->role = Player::White;
	cb = new ChessBoard();
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			cp[i][j] = new Cell(-35.0 + 10.0 * i, 35.0 - 10.0 * j);
}

Game::~Game() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			delete cp[i][j];
	delete cb;
	delete pb;
	delete pw;
}

ChessBoard::Point HumanPlayer::drop() {
	ChessBoard::Point ret = { -1, -1 };
	while (!Reversi::game->endSignal) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (Reversi::game->cp[i][j]->clicked()) {
					ret.x = i; ret.y = j;
					return ret;
				}
			}
		}
	}
	return ret;
}

HumanPlayer::HumanPlayer() {
	type = Human;
}

ChessBoard::Point AIPlayer::drop() {
	ChessBoard::Point ret(-1, -1);
	auto dropableList =
		role == White ? Reversi::game->wDropables : Reversi::game->bDropables;

	for (auto dp : dropableList) {
		if ((dp.x == 0 || dp.x == 7) && (dp.y == 0 || dp.y == 7)) {
			ret.x = dp.x; ret.y = dp.y; return ret;
		}
	}
	for (auto dp : dropableList) {
		if (dp.x == 0 || dp.x == 7 || dp.y == 0 || dp.y == 7) {
			ret.x = dp.x; ret.y = dp.y; return ret;
		}
	}
	for (auto dp : dropableList) {
		if (dp.x != 1 && dp.x != 6 && dp.y != 1 && dp.y != 6) {
			ret.x = dp.x; ret.y = dp.y; return ret;
		}
	}
	if (dropableList.empty()) {
		ret.x = -1; ret.y = -1; return ret;
	}
	auto dp = dropableList[rand() % dropableList.size()];
	ret.x = dp.x; ret.y = dp.y; return ret;
}

AIPlayer::AIPlayer() {
	type = AI;
}

ChessBoard::Point RemotePlayer::drop() {
	char buf[2];
	if (recv(Reversi::game->sock, buf, 2, 0) == SOCKET_ERROR) {
		MessageBox(hMainWnd, _T("连接断开，游戏结束！"), _T("提示"), MB_OK);
		Reversi::game->end(false);
	}
	ChessBoard::Point ret(-1, -1);
	ret.x = buf[0];
	ret.y = buf[1];
	return ret;
}

RemotePlayer::RemotePlayer() {
	type = Remote;
}

void Cell::n2b() {
	pieceGrey = 0.1;
	piecePhi = 0.0;
	pieceSize = pieceSize < 1.0 ? pieceSize + 0.05 : 1.0;
	if (pieceSize == 1.0) piece = Black;
}

void Cell::n2w() {
	pieceGrey = 0.9;
	piecePhi = 0.0;
	pieceSize = pieceSize < 1.0 ? pieceSize + 0.05 : 1.0;
	if (pieceSize == 1.0) piece = White;
}

void Cell::w2b() {
	pieceGrey = pieceGrey > 0.1 ? pieceGrey - 0.04 : 0.1;
	piecePhi = piecePhi < 180.0 ? piecePhi + 9.0 : 180.0;
	pieceSize = pieceSize < 1.0 ? pieceSize + 0.05 : 1.0;
	if (pieceGrey == 0.1 && piecePhi == 180.0 && pieceSize == 1.0) {
		piece = Black;
		piecePhi = 0.0;
	}
}

void Cell::b2w() {
	pieceGrey = pieceGrey < 0.9 ? pieceGrey + 0.04 : 0.9;
	piecePhi = piecePhi < 180.0 ? piecePhi + 9.0 : 180.0;
	pieceSize = pieceSize < 1.0 ? pieceSize + 0.05 : 1.0;
	if (pieceGrey == 0.9 && piecePhi == 180.0 && pieceSize == 1.0) {
		piece = White;
		piecePhi = 0.0;
	}
}

void Cell::chStatus(Status src, Status dst) {
	std::array<GLfloat, 3> gInc = {
		abs(gColor[dst][0] - gColor[src][0]) / (GLfloat)af,
		abs(gColor[dst][1] - gColor[src][1]) / (GLfloat)af,
		abs(gColor[dst][2] - gColor[src][2]) / (GLfloat)af
	};
	for (int i = 0; i < 3; i++) {
		if (gAD[i] + gInc[i] < gColor[dst][i]) {
			gAD[i] += gInc[i];
		} else if (gAD[i] - gInc[i] > gColor[dst][i]) {
			gAD[i] -= gInc[i];
		} else {
			gAD[i] = gColor[dst][i];
		}
	}
	if (gAD[0] == gColor[dst][0] &&
		gAD[1] == gColor[dst][1] &&
		gAD[2] == gColor[dst][2]) {
		status = dst;
	}
}

void drawChess(float a, float b, float r, GLuint slice, GLuint stack) {

#define sin2(x) pow(sin(x), 2)
#define cos2(x) pow(cos(x), 2)
#define pos(x, y) ((i + y) * slice + j + x)

	const float a2 = a * a, b2 = b * b, r1 = r;
	struct Vec3f { GLfloat e[3]; };
	std::vector<Vec3f> vertices, normals;
	std::vector<GLuint> indices;
	indices.reserve(slice * stack * 4);
	float θ = 0.0f;
	for (auto i = 0u; i <= stack; i++, θ += (GLfloat)PI / stack) {
		float φ = 0.0f;
		for (auto j = 0u; j < slice; j++, φ += (GLfloat)PI * 2 / slice) {
			const auto tmp = sqrt(a2 * sin2(θ) + b2 * cos2(θ));
			vertices.push_back({
				(float)(r1 * sin(θ) * a2 / tmp * cos(φ)),
				(float)(r1 * sin(θ) * a2 / tmp * sin(φ)),
				(float)(r1 * cos(θ) * b2 / tmp)
			});
			normals.push_back({
				(float)(sin(θ) * cos(φ)),
				(float)(sin(θ) * sin(φ)),
				(float)(cos(θ))
			});
			if (i == stack) continue;
			indices.push_back((i + 0) * slice + (j + 0) % slice);
			indices.push_back((i + 1) * slice + (j + 0) % slice);
			indices.push_back((i + 1) * slice + (j + 1) % slice);
			indices.push_back((i + 0) * slice + (j + 1) % slice);
		}
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glDrawElements(GL_QUADS, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

#undef pos
#undef cos2
#undef sin2

}

void Cell::draw() {
	switch (status) {
	case toN: chStatus(Hover, Normal); break;
	case toH: chStatus(Normal, Hover); break;
	case toP: chStatus(Normal, Press); break;
	case toD: chStatus(Normal, Dropable); break;
	}
	GLfloat gSpec[] = { 0.2f, 0.2f, 0.2f, 0.5f };
	glColor4fv(gAD.data());
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gAD.data());
	glMaterialfv(GL_FRONT, GL_SPECULAR, gSpec);
	glMateriali(GL_FRONT, GL_SHININESS, 20);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(cX, cY, 0.0);

	glBindTexture(GL_TEXTURE_2D, GLBase::tex[1]);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	static GLuint list;
	if (list) {
		glCallList(list);
	} else {
		list = glGenLists(1);
		glNewList(list, GL_COMPILE_AND_EXECUTE);
		glBegin(GL_QUADS);
		for (int i = 0, j = 0; i < 20; i++) {
			if (i % 4 == 0)
				glNormal3dv(N[j++]);
			switch (i % 4) {
			case 0: glTexCoord2d(0.0, 0.0); break;
			case 1: glTexCoord2d(1.0, 0.0); break;
			case 2: glTexCoord2d(1.0, 1.0); break;
			case 3: glTexCoord2d(0.0, 1.0); break;
			}
			glVertex3dv(V[i]);
		}
		glEnd();
		glEndList();
	}
	
	glPopMatrix();

	//GLint polygonMode[4];
	//glGetIntegerv(GL_POLYGON_MODE, polygonMode);
	//if (polygonMode[0] == GL_LINE) return;

	switch (piece) {
	default:
	case None: return;
	case Black: pieceGrey = 0.1; piecePhi = 0.0; pieceSize = 1.0; break;
	case White: pieceGrey = 0.9; piecePhi = 0.0; pieceSize = 1.0; break;
	case N2B: n2b(); break;
	case N2W: n2w(); break;
	case W2B: w2b(); break;
	case B2W: b2w(); break;
	}
	GLfloat grey = (float)pieceGrey;
	GLfloat ambdif[] = { grey, grey, grey, 0.5f };
	GLfloat spec[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(cX, cY, 1.0);
	glRotated(piecePhi, 0.0, 1.0, 0.0);
	glScaled(pieceSize, pieceSize, pieceSize);
	glLineWidth(0.1f);

	//GLUnurbs *nurbs = gluNewNurbsRenderer();
	//gluBeginSurface(nurbs);
	//gluNurbsCurve(nurbs, 8, ...);

	glScaled(1.0, 1.0, 0.3);
	auto quad = gluNewQuadric();
	glEnable(GL_NORMALIZE);
	gluSphere(quad, 4.5, 32, 16);
	glDisable(GL_NORMALIZE);
	gluDeleteQuadric(quad);

	//static GLuint list2;
	//if (list2) {
	//	glCallList(list2);
	//} else {
	//	list2 = glGenLists(1);
	//	glNewList(list2, GL_COMPILE_AND_EXECUTE);
	//	drawChess(1.0f, 0.3f, 4.5, 32, 16);
	//	glEndList();
	//}

	glPopMatrix();
	glLineWidth(1.0f);
}

bool Cell::isNone() {
	return piece == None;
}

bool Cell::isBlack() {
	return piece == Black || piece == N2B || piece == W2B;
}

bool Cell::isWhite() {
	return piece == White || piece == N2W || piece == B2W;
}

bool Cell::operator==(Cell & c) {
	return this->isNone() && c.isNone()
		|| this->isBlack() && c.isBlack()
		|| this->isWhite() && c.isWhite();
}

bool Cell::operator!=(Cell & c) {
	return !(*this == c);
}

void Cell::toBlack() {
	piece = isWhite() ? W2B : isNone() ? N2B : Black;
}

void Cell::toWhite() {
	piece = isBlack() ? B2W : isNone() ? N2W : White;
}

void Cell::toNormal() {
	status = toN;
}

void Cell::toHover() {
	status = toH;
}

void Cell::toPress() {
	status = toP;
}

void Cell::toDropable() {
	status = toD;
}

bool Cell::clicked() {
	auto ret = clickedFlag;
	clickedFlag = false;
	return ret;
}

void Cell::showDropable(bool show) {
	dropableFlag = show;
	if (show) toDropable();
	else toNormal();
}

void Cell::handleMouse(int button, int x, int y) {
	GLdouble mvMat[16], pjMat[16], nearV[3], farV[3];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, mvMat);
	glGetDoublev(GL_PROJECTION_MATRIX, pjMat);
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluUnProject(
		(GLdouble)x, (GLdouble)(viewport[3] - y - 1), 0.0,
		mvMat, pjMat, viewport,
		nearV, nearV + 1, nearV + 2
	);
	gluUnProject(
		(GLdouble)x, (GLdouble)(viewport[3] - y - 1), 1.0,
		mvMat, pjMat, viewport,
		farV, farV + 1, farV + 2
	);
	GLdouble polyV[4][3] = {
		cX + V[0][0], cY + V[0][1], V[0][2],
		cX + V[1][0], cY + V[1][1], V[1][2],
		cX + V[2][0], cY + V[2][1], V[2][2],
		cX + V[3][0], cY + V[3][1], V[3][2]
	};
	if (isRayIntersectPolygon3v((const GLdouble *)nearV, (const GLdouble *)farV, (const GLdouble *)polyV, 4U)) {
		if (button & MK_LBUTTON) {
			downFlag = true;
			toPress();
		} else {
			if (downFlag) {
				downFlag = false;
				clickedFlag = true;
			} else {
				clickedFlag = false;
			}
			toHover();
		}
	} else {
		if (dropableFlag) toDropable();
		else toNormal();
		downFlag = false;
		clickedFlag = false;
	}
}

Cell::Cell(GLdouble centerX, GLdouble centerY)
	: cX(centerX), cY(centerY), af(5.0), downFlag(false), clickedFlag(false), dropableFlag(false) {
	piece = None;
	pieceGrey = 0.0;
	piecePhi = 0.0;
	pieceSize = 0.0;
	status = Normal;
	gAD = gColor[Normal];
}

void Situation::draw() {
	char b[10], w[10], str[20];
	GLBase::drawCNString("", 40, DEFAULT_CHARSET, "黑体", 0, 0);
	strcpy_s(str, "黑方：");
	sprintf_s(b, "%d", black);
	strcat_s(str, b);
	if (!wTurn) strcat_s(str, "←");
	GLBase::drawCNString(str, 40, DEFAULT_CHARSET, "黑体", 0, 0);
	strcpy_s(str, "白方：");
	sprintf_s(w, "%d", white);
	strcat_s(str, w);
	if (wTurn) strcat_s(str, "←");
	GLBase::drawCNString(str, 40, DEFAULT_CHARSET, "黑体", 0, 0.2);
}
