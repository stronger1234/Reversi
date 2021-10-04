// Reversi.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "WinMain.h"
#include "GLBase.h"
#include "Connection.h"
#include "Reversi.h"

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
TCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND hMainWnd;
HMENU hMenu;
bool fullscreen;


// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    NewGameDlg(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_		HINSTANCE	hInstance,
					   _In_opt_	HINSTANCE	hPrevInstance,
					   _In_		LPTSTR		lpCmdLine,
					   _In_		int			nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_REVERSI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_REVERSI));

	// 主消息循环: 
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_REVERSI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL/*(HBRUSH)(COLOR_WINDOW+1)*/;
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_REVERSI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDC_REVERSI));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	DWORD dwExStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	RECT wndRect = { 0, 0, 960, 540 };
	AdjustWindowRectEx(&wndRect, dwStyle, TRUE, dwExStyle);
	auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
	auto screenHeight = GetSystemMetrics(SM_CYSCREEN);
	auto clientWidth = wndRect.right - wndRect.left;
	auto clientHeight = wndRect.bottom - wndRect.top;
	auto wndLeft = (screenWidth - clientWidth) / 2;
	auto wndTop = (screenHeight - clientHeight) / 2 + 20;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowEx(
		dwExStyle,
		szWindowClass, szTitle, 
		dwStyle,
		wndLeft, wndTop, clientWidth, clientHeight,
		nullptr, nullptr, hInstance, nullptr
	);

	if (!hWnd || !GLBase::initialize(hWnd)) {
		return FALSE;
	}
	GLBase::initGLScene();

	Connection::initialize();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	hMenu = GetMenu(hWnd);
	hMainWnd = hWnd;
	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 分析菜单选择: 
			switch (wmId) {
			case IDM_NEWGAME:
				ValidateRect(hWnd, NULL);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_NEWGAME), hWnd, NewGameDlg);
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			case IDM_FULLSCREEN:
				fullscreen = !fullscreen;
				if (fullscreen) {
					auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
					auto screenHeight = GetSystemMetrics(SM_CYSCREEN);
					SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
					SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, screenWidth, screenHeight, SWP_SHOWWINDOW);
					SetMenu(hWnd, NULL);
				} else {
					DWORD dwExStyle = WS_EX_APPWINDOW;
					DWORD dwStyle = WS_OVERLAPPEDWINDOW;
					RECT wndRect = { 0, 0, 960, 540 };
					AdjustWindowRectEx(&wndRect, dwStyle, TRUE, dwExStyle);
					auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
					auto screenHeight = GetSystemMetrics(SM_CYSCREEN);
					auto clientWidth = wndRect.right - wndRect.left;
					auto clientHeight = wndRect.bottom - wndRect.top;
					auto wndLeft = (screenWidth - clientWidth) / 2;
					auto wndTop = (screenHeight - clientHeight) / 2 + 20;
					SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
					AdjustWindowRectEx(&wndRect, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_APPWINDOW);
					SetWindowPos(hWnd, HWND_NOTOPMOST, wndLeft, wndTop, clientWidth, clientHeight, SWP_SHOWWINDOW);
					SetMenu(hWnd, hMenu);
				}
				break;
			case IDM_OPTIONS:
				break;
			case IDM_ABOUT:
				ValidateRect(hWnd, NULL);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_KEYDOWN:
		GLBase::keyEvent(true, wParam);
		break;
	case WM_KEYUP:
		GLBase::keyEvent(false, wParam);
		break;
	case WM_MOUSEMOVE:
		GLBase::mouseEvent((int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONDOWN:
		GLBase::mouseEvent((int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
		GLBase::mouseEvent((int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_PAINT:
		GLBase::drawGLScene();
		break;
	case WM_SIZE:
		GLBase::resizeGLScene(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		if (Reversi::game)
			Reversi::game->end(false);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		ShowWindow(hDlg, 1);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
