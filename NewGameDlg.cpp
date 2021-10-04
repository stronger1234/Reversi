#include "stdafx.h"
#include "NewGameDlg.h"
#include "Connection.h"

void thLanGame(bool imHost) {
	EndDialog(hNewGameDlg, 0);
	if (imHost) {
		Reversi::lanGameStart(
			Connection::connSock,
			Connection::pubInfo.name,
			0,
			Connection::rspInfo.name,
			Connection::rspInfo.role == ROLE::Black
		);
	} else {
		Reversi::lanGameStart(
			Connection::connSock,
			Connection::rspInfo.name,
			0,
			Connection::pubInfo.name,
			Connection::rspInfo.role == ROLE::White
		);
	}
}

static TCHAR p1Name[MAX_NAMESTRING + 1], p2Name[MAX_NAMESTRING + 1];
static char p1Type, p2Type;

void thLocalGame() {
	DestroyWindow(hNewGameDlg);
	Reversi::localGameStart(p1Name, p1Type, p2Name, p2Type);
}

void updateGameList() {
	SendMessage(hGameListLB, WM_SETREDRAW, FALSE, 0);
	SendMessage(hGameListLB, LB_RESETCONTENT, 0, 0);
	auto it = Connection::gameList.begin();
	auto it2 = Connection::gameList.end();
	for (auto gi : Connection::gameList) {
		TCHAR str[20];
		_tcscpy_s(str, gi.name);
		if (gi.role == ROLE::Black) {
			if (SendMessage(hMyRoleCB, CB_GETCURSEL, 0, 0) == 0)
				continue;
			_tcscat_s(str, _T(" - 黑方"));
		} else if (gi.role == ROLE::White) {
			if (SendMessage(hMyRoleCB, CB_GETCURSEL, 0, 1) == 1)
				continue;
			_tcscat_s(str, _T(" - 白方"));
		} else if (gi.role == ROLE::Random) {
			_tcscat_s(str, _T(" - 随机"));
		} else {
			continue;
		}
		SendMessage(hGameListLB, LB_ADDSTRING, 0, (LPARAM)str);
	}
	SendMessage(hGameListLB, WM_SETREDRAW, TRUE, 0);
}

INT_PTR CALLBACK NewGameDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		hPlayer1NameEM = GetDlgItem(hDlg, IDC_PLAYER1_NAME);
		hPlayer1TypeCB = GetDlgItem(hDlg, IDC_PLAYER1_TYPE);
		hPlayer2NameEM = GetDlgItem(hDlg, IDC_PLAYER2_NAME);
		hPlayer2TypeCB = GetDlgItem(hDlg, IDC_PLAYER2_TYPE);
		hStartLocalBM = GetDlgItem(hDlg, IDC_START_LOCAL);
		hGameListLB = GetDlgItem(hDlg, IDC_GAMELIST);
		hMyNameEM = GetDlgItem(hDlg, IDC_MYNAME);
		hMyRoleCB = GetDlgItem(hDlg, IDC_MYROLE);
		hCreateLanBM = GetDlgItem(hDlg, IDC_CREATE_LAN);
		hEnterLanBM = GetDlgItem(hDlg, IDC_ENTER_LAN);
		hNewGameDlg = hDlg;
		hCursor = LoadCursor(NULL, IDC_ARROW);

		SendMessage(hPlayer1NameEM, EM_LIMITTEXT, MAX_NAMESTRING, 0);
		SendMessage(hPlayer1NameEM, WM_SETTEXT, 0, (LPARAM)_T("Player1"));
		SendMessage(hPlayer1TypeCB, CB_ADDSTRING, 0, (LPARAM)_T("玩家"));
		SendMessage(hPlayer1TypeCB, CB_ADDSTRING, 0, (LPARAM)_T("电脑"));
		SendMessage(hPlayer1TypeCB, CB_SETCURSEL, 0, 0);
		SendMessage(hPlayer2NameEM, EM_LIMITTEXT, MAX_NAMESTRING, 0);
		SendMessage(hPlayer2NameEM, WM_SETTEXT, 0, (LPARAM)_T("Player1"));
		SendMessage(hPlayer2TypeCB, CB_ADDSTRING, 0, (LPARAM)_T("玩家"));
		SendMessage(hPlayer2TypeCB, CB_ADDSTRING, 0, (LPARAM)_T("电脑"));
		SendMessage(hPlayer2TypeCB, CB_SETCURSEL, 0, 0);
		SendMessage(hMyNameEM, EM_LIMITTEXT, MAX_NAMESTRING, 0);
		SendMessage(hMyNameEM, WM_SETTEXT, 0, (LPARAM)_T("Reversi"));
		SendMessage(hMyRoleCB, CB_ADDSTRING, 0, (LPARAM)_T("黑方"));
		SendMessage(hMyRoleCB, CB_ADDSTRING, 0, (LPARAM)_T("白方"));
		SendMessage(hMyRoleCB, CB_ADDSTRING, 0, (LPARAM)_T("随机"));
		SendMessage(hMyRoleCB, CB_SETCURSEL, 2, 0);

		Connection::collecting = false;
		Connection::connecting = false;
		Connection::publishing = false;

		Connection::startCollectGames();
		return (INT_PTR)TRUE;
	case WM_DESTROY:
		Connection::stopPublishGame();
		Connection::stopCollectGames();
		return (INT_PTR)TRUE;
	case WM_SETCURSOR:
		SetCursor(hCursor);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		} else if (LOWORD(wParam) == IDC_MYROLE) {
			updateGameList();
			return (INT_PTR)TRUE;
		} else if (LOWORD(wParam) == IDC_START_LOCAL) {
			GetWindowText(hPlayer1NameEM, p1Name, sizeof(p1Name));
			GetWindowText(hPlayer2NameEM, p2Name, sizeof(p2Name));
			p1Type = (char)SendMessage(hPlayer1TypeCB, CB_GETCURSEL, 0, 0);
			p2Type = (char)SendMessage(hPlayer2TypeCB, CB_GETCURSEL, 0, 0);
			std::thread(thLocalGame).detach();
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		} else if (LOWORD(wParam) == IDC_CREATE_LAN) {
			if (Connection::publishing) {
				Connection::stopPublishGame();
				hCursor = LoadCursor(NULL, IDC_ARROW);
				EnableAllControl();
				SendMessage(hCreateLanBM, WM_SETTEXT, 0, (LPARAM)_T("创建游戏"));
			} else {
				auto &info = Connection::pubInfo;
				CoCreateGuid(&info.guid);
				switch (SendMessage(hMyRoleCB, CB_GETCURSEL, 0, 0)) {
				case 0:		info.role = ROLE::Black;	break;
				case 1:		info.role = ROLE::White;	break;
				case 2:		info.role = ROLE::Random;	break;
				default:	info.role = ROLE::Delete;	break;
				}
				SendMessage(hMyNameEM, WM_GETTEXT, MAX_NAMESTRING + 1, (LPARAM)&info.name);
				Connection::startPublishGame();

				hCursor = LoadCursor(NULL, IDC_WAIT);
				DisableAllControl();
				EnableWindow(hCreateLanBM, TRUE);
				SendMessage(hCreateLanBM, WM_SETTEXT, 0, (LPARAM)_T("取消"));
			}
			return (INT_PTR)TRUE;
		} else if (LOWORD(wParam) == IDC_ENTER_LAN) {
			if (Connection::connecting) {
				Connection::stopConnectGame();
				hCursor = LoadCursor(NULL, IDC_ARROW);
				EnableAllControl();
				SendMessage(hCreateLanBM, WM_SETTEXT, 0, (LPARAM)_T("加入游戏"));
			} else {
				auto i = SendMessage(hGameListLB, LB_GETCURSEL, 0, 0);
				if (i < 0) return (INT_PTR)TRUE;
				auto &pubInfo = Connection::pubInfo;
				pubInfo = Connection::gameList[i];
				auto &rspInfo = Connection::rspInfo;
				rspInfo.guid = pubInfo.guid;
				SendMessage(hMyNameEM, WM_GETTEXT, MAX_NAMESTRING + 1, (LPARAM)rspInfo.name);
				switch (SendMessage(hMyRoleCB, CB_GETCURSEL, 0, 0)) {
				case 0:
					rspInfo.role = ROLE::Black; break;
				case 1:
					rspInfo.role = ROLE::White; break;
				case 2:
					if (pubInfo.role == ROLE::Black) rspInfo.role = ROLE::White;
					else if (pubInfo.role == ROLE::White) rspInfo.role = ROLE::Black;
					else rspInfo.role = rand() % 2 ? ROLE::Black : ROLE::White;
					break;
				}
				Connection::startConnectGame();

				hCursor = LoadCursor(NULL, IDC_WAIT);
				DisableAllControl();
				EnableWindow(hCreateLanBM, TRUE);
				SendMessage(hCreateLanBM, WM_SETTEXT, 0, (LPARAM)_T("取消"));
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}