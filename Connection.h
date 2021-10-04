#pragma once
#include "stdafx.h"
#include "WinMain.h"

enum class ROLE {
	Black, White, Random, Delete
};

class IDumpable {
public:
	virtual void dump(byte *buf) const = 0;
	virtual void undump(const byte *buf) = 0;
};

struct ConnInfo : public IDumpable {
	GUID	guid;
	ROLE	role;
	TCHAR	name[MAX_NAMESTRING + 1];
	IN_ADDR	addr;
	INT		alive;

	void dump(byte *buf) const override;
	void undump(const byte *buf) override;
};

class Connection {
	static std::vector<IN_ADDR> nativeAddrList;

	static void thAccept();
	static void thPublish();
	static void thCollect();
	static void thConnect();

public:
	static std::vector<ConnInfo> gameList;
	static SOCKET acptSock, connSock;
	static ConnInfo pubInfo, rspInfo;
	static bool publishing, collecting, connecting;

	static bool initialize();
	static void startPublishGame();
	static void stopPublishGame();
	static void startCollectGames();
	static void stopCollectGames();
	static void startConnectGame();
	static void stopConnectGame();
};

extern void updateGameList();
extern void thLanGame(bool imHost);