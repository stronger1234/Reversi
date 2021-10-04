#include "stdafx.h"
#include "WinMain.h"
#include "Connection.h"
#include "Reversi.h"

#define	lenGuid		sizeof(GUID)
#define	lenRole		sizeof(ROLE)
#define	lenName		(MAX_NAMESTRING + 1)
#define	lenDump		(lenGuid + lenRole + lenName)
#define	lenAddr		sizeof(IN_ADDR)
#define lenAlive	sizeof(INT)
#define	offGuid		0
#define offRole		lenGuid
#define offName		(lenGuid + lenRole)
#define offAddr		(lenGuid + lenRole + lenName)
#define offAlive	(lenGuid + lenRole + lenName + lenAddr)

std::vector<IN_ADDR>	Connection::nativeAddrList;
std::vector<ConnInfo>	Connection::gameList;
SOCKET					Connection::connSock;
SOCKET					Connection::acptSock;
ConnInfo				Connection::pubInfo;
ConnInfo				Connection::rspInfo;
bool					Connection::publishing;
bool					Connection::collecting;
bool					Connection::connecting;

void ConnInfo::dump(byte *buf) const {
	memcpy_s(buf + offGuid, lenGuid, &guid, lenGuid);
	memcpy_s(buf + offRole, lenRole, &role, lenRole);
#ifdef UNICODE
	size_t converted;
	wcstombs_s(&converted, (char *)(buf + offName), lenName, name, lenName);
#else
	strcpy_s((char *)(buf + offName), lenName, name);
#endif
}

void ConnInfo::undump(const byte *buf) {
	memcpy_s(&guid, lenGuid, buf + offGuid, lenGuid);
	memcpy_s(&role, lenRole, buf + offRole, lenRole);
#ifdef UNICODE
	size_t converted;
	mbstowcs_s(&converted, name, lenName, (char *)(buf + offName), lenName);
#else
	strcpy_s(name, lenName, (char *)(buf + offName));
#endif
}

bool Connection::initialize() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return false;
	}

	char computerName[64];
	gethostname(computerName, sizeof(computerName));

	ADDRINFO hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_socktype = SOCK_DGRAM;
	ADDRINFO *aiListBegin;
	getaddrinfo(computerName, NULL, &hints, &aiListBegin);
	nativeAddrList.clear();
	for (auto ai = aiListBegin; ai != NULL; ai = ai->ai_next) {
		nativeAddrList.push_back(((SOCKADDR_IN *)(ai->ai_addr))->sin_addr);
	}
	return true;
}

void Connection::startPublishGame() {
	publishing = true;
	std::thread(thAccept).detach();
	std::thread(thPublish).detach();
}

void Connection::stopPublishGame() {
	publishing = false;
	closesocket(acptSock);
}

void Connection::startCollectGames() {
	collecting = true;
	std::thread(thCollect).detach();
}

void Connection::stopCollectGames() {
	collecting = false;
}

void Connection::startConnectGame() {
	connecting = true;
	std::thread(thConnect).detach();
}

void Connection::stopConnectGame() {
	connecting = false;
	closesocket(connSock);
}

void Connection::thPublish() {
	BOOL so_broadcast = TRUE;

	SOCKADDR_IN toAddr;
	ZeroMemory(&toAddr, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(6666);
	toAddr.sin_addr.s_addr = INADDR_BROADCAST;

	SOCKADDR_IN myAddr;
	ZeroMemory(&myAddr, sizeof(myAddr));
	myAddr.sin_family = AF_INET;

	byte buf[lenDump];
	pubInfo.dump(buf);

#define sendtoAllBroadcastAddr()															\
for (auto it : nativeAddrList) {															\
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);									\
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&so_broadcast, sizeof(so_broadcast));	\
	myAddr.sin_addr.s_addr = it.s_addr;														\
	bind(s, (SOCKADDR *)&myAddr, sizeof(myAddr));											\
	sendto(s, (char *)buf, sizeof(buf), 0, (SOCKADDR *)&toAddr, sizeof(toAddr));			\
	closesocket(s);																			\
}

	while (publishing) {
		sendtoAllBroadcastAddr();
	}
	pubInfo.role = ROLE::Delete;
	pubInfo.dump(buf);
	sendtoAllBroadcastAddr();

#undef sendtoAllBroadcastAddr
}

void Connection::thCollect() {
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	int recvTimeOut = 1000;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&recvTimeOut, sizeof(recvTimeOut));

	SOCKADDR_IN fromAddr;
	ZeroMemory(&fromAddr, sizeof(fromAddr));
	fromAddr.sin_family = AF_INET;
	fromAddr.sin_port = htons(6666);
	fromAddr.sin_addr.s_addr = INADDR_ANY;
	bind(s, (SOCKADDR *)&fromAddr, sizeof(fromAddr));

	gameList.clear();
	while (collecting) {
		ConnInfo pubInfo;
		byte buf[lenDump];

		int fromAddrLen = sizeof(fromAddr);
		auto res = recvfrom(s, (char *)buf, sizeof(buf), 0, (SOCKADDR *)&fromAddr, &fromAddrLen);

		auto it = gameList.begin();
		if (res == SOCKET_ERROR) {
			while (it != gameList.end()) {
				if (--it->alive == 0) {
					it = gameList.erase(it);
					updateGameList();
				}
				else it++;
			}
			continue;
		}
		pubInfo.undump(buf);
		pubInfo.addr = fromAddr.sin_addr;
		pubInfo.alive = 3;

		it = gameList.begin();
		while (it != gameList.end() && it->guid != pubInfo.guid) it++;
		if (it != gameList.end()) {
			if (pubInfo.role == ROLE::Delete) {
				gameList.erase(it);
				updateGameList();
			}
			continue;
		}

		if (pubInfo.role != ROLE::Delete) {
			gameList.push_back(pubInfo);
			updateGameList();
		}
	}
	closesocket(s);
	gameList.clear();
}

void Connection::thAccept() {
	acptSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN opAddr;
	ZeroMemory(&opAddr, sizeof(opAddr));
	opAddr.sin_family = AF_INET;
	opAddr.sin_port = htons(6666);
	opAddr.sin_addr.s_addr = INADDR_ANY;

	bind(acptSock, (SOCKADDR *)&opAddr, sizeof(opAddr));
	listen(acptSock, 10);
	connSock = accept(acptSock, NULL, NULL);
	stopPublishGame();
	if (connSock == INVALID_SOCKET) return;

	byte buf[lenDump];
	recv(connSock, (char *)buf, sizeof(buf), 0);
	rspInfo.undump(buf);
	pubInfo.role = rspInfo.role == ROLE::Black ? ROLE::White : ROLE::Black;
	TCHAR str[100];
	_tcscpy_s(str, _T("对手（"));
	_tcscat_s(str, rspInfo.role == ROLE::White ? _T("白方）：") : _T("黑方）："));
	_tcscat_s(str, rspInfo.name);
	_tcscat_s(str, _T("\n我（"));
	_tcscat_s(str, rspInfo.role == ROLE::Black ? _T("白方）：") : _T("黑方）："));
	_tcscat_s(str, pubInfo.name);
	_tcscat_s(str, _T("\n是否准备开始？"));
	switch (MessageBox(hMainWnd, str, _T("准备"), MB_YESNO | MB_ICONQUESTION)) {
	case IDYES:	std::thread(thLanGame, true).detach(); break;
	case IDNO:	
	default:	closesocket(connSock); break;
	}
}

void Connection::thConnect() {
	connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN toAddr;
	ZeroMemory(&toAddr, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(6666);
	toAddr.sin_addr = pubInfo.addr;

	if (connect(connSock, (SOCKADDR *)&toAddr, sizeof(toAddr)) == SOCKET_ERROR) {
		stopConnectGame();
		return;
	}
	connecting = false;

	byte buf[lenDump];
	rspInfo.dump(buf);
	send(connSock, (char *)buf, sizeof(buf), 0);
	pubInfo.role = rspInfo.role == ROLE::Black ? ROLE::White : ROLE::Black;

	TCHAR str[100];
	_tcscpy_s(str, _T("对手（"));
	_tcscat_s(str, rspInfo.role == ROLE::Black ? _T("白方）：") : _T("黑方）："));
	_tcscat_s(str, pubInfo.name);
	_tcscat_s(str, _T("\n我（"));
	_tcscat_s(str, rspInfo.role == ROLE::White ? _T("白方）：") : _T("黑方）："));
	_tcscat_s(str, rspInfo.name);
	_tcscat_s(str, _T("\n是否准备开始？"));
	switch (MessageBox(hMainWnd, str, _T("准备"), MB_YESNO | MB_ICONQUESTION)) {
	case IDYES:	std::thread(thLanGame, false).detach(); break;
	case IDNO:	
	default:	closesocket(connSock); break;
	}
}
