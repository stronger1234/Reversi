#pragma once
#include "stdafx.h"
#include "WinMain.h"
#include "Reversi.h"

HWND hNewGameDlg	;
HWND hPlayer1NameEM	;
HWND hPlayer1TypeCB	;
HWND hPlayer2NameEM	;
HWND hPlayer2TypeCB	;
HWND hStartLocalBM	;
HWND hGameListLB	;
HWND hMyNameEM		;
HWND hMyRoleCB		;
HWND hCreateLanBM	;
HWND hEnterLanBM	;

HCURSOR hCursor;

#define DisableAllControl()					\
EnableWindow(hPlayer1NameEM,	FALSE);		\
EnableWindow(hPlayer1TypeCB,	FALSE);		\
EnableWindow(hPlayer2NameEM,	FALSE);		\
EnableWindow(hPlayer2TypeCB,	FALSE);		\
EnableWindow(hStartLocalBM,		FALSE);		\
EnableWindow(hGameListLB,		FALSE);		\
EnableWindow(hMyNameEM,			FALSE);		\
EnableWindow(hMyRoleCB,			FALSE);		\
EnableWindow(hCreateLanBM,		FALSE);		\
EnableWindow(hEnterLanBM,		FALSE);		\


#define EnableAllControl()					\
EnableWindow(hPlayer1NameEM,	TRUE);		\
EnableWindow(hPlayer1TypeCB,	TRUE);		\
EnableWindow(hPlayer2NameEM,	TRUE);		\
EnableWindow(hPlayer2TypeCB,	TRUE);		\
EnableWindow(hStartLocalBM,		TRUE);		\
EnableWindow(hGameListLB,		TRUE);		\
EnableWindow(hMyNameEM,			TRUE);		\
EnableWindow(hMyRoleCB,			TRUE);		\
EnableWindow(hCreateLanBM,		TRUE);		\
EnableWindow(hEnterLanBM,		TRUE);		\

