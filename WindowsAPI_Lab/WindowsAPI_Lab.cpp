// WindowsAPI_Lab.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "WindowsAPI_Lab.h"
#include "uxtheme.h"
#include "dwmapi.h"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <cmath>
#include <list>


using namespace std;

#define MAX_LOADSTRING 100
#define ID_FIRSTCHILD  100 
#define ID_SECONDCHILD 101 
#define ID_THIRDCHILD  102 

#define INITTIMER 11
#define RUNNINGTIMER 12
#define PARTICLETIMER 13
#define WM_SELECTED WM_USER+1
#define WM_INITBOARD WM_USER+2
#define WM_RUNNINGINTERVAL WM_USER+3
#define WM_SELECTEDCELL WM_USER+4
#define WM_INITOVERLAY WM_USER+5
#define WM_BINGOTOPARTICLES WM_USER+6
#define BINGO true
#define NOBINGO false

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM				RegisterChildClass(HINSTANCE hInstance);
ATOM				RegisterOverlay(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ChildProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	OverlayProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	StaticDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSAPILAB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	RegisterChildClass(hInstance);
	RegisterOverlay(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSAPILAB));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPILAB));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSAPILAB);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

ATOM RegisterChildClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ChildProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSAPILAB);
	wcex.lpszClassName = TEXT("CHILD");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

ATOM RegisterOverlay(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = OverlayProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("OVERLAY");
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_CLIPCHILDREN | WS_BORDER
		| WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
BOOL CALLBACK DestoryChildCallback(HWND hwnd, LPARAM lParam);
void CreateChildren(HWND hWnd, HWND hwndRect[], int size, int row, int gap, int margin);
void Debug(const int msg, TCHAR *buf, int bufSize);
bool isNear(int pos, int another, int row);
int getWindowLength(int margin, int gap, int size, int row);
bool isTriple(int row, bool restart);
bool isFinishedBoard(int row);
void Bingo(int index1, int index2, int index3);
void Drop(int row);
//void CreateParticles(list<POINT> pointsList, int rad, int row);
//void MoveParticles(HWND hWnd, list<POINT> pointList, int rad, int row, int offset);
static int board[144];
static HWND hwndRect[12 * 12];
static HDC hdc;


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Use to get the debugging messages.
	const int bufSize = 256;
	TCHAR buf[bufSize];
	static int debugNumber = 0;
	//Constant number
	const static int sizeS = 80, sizeM = 70, sizeL = 60,
		rowS = 8, rowM = 10, rowL = 12,
		gap = 10, margin = 5;
	//Parent Window Elements
	RECT rc;
	int border, caption;
	static HBRUSH hbrBkgnd = NULL;
	//Childwindow elements
	static HWND hWndTop;
	//Control identifier
	static bool isGameStarted = false;
	static int color[rowL * rowL];
	static int enumrator = 0;
	static int swap[2];
	static int row;
	static bool isComboSelected = false;

	switch (message)
	{

	case WM_CREATE:
	{
		TCHAR s[256];
		_stprintf_s(s, 256, _T("BeWindowed 2020 a.k.a. Bejeweled in WinAPI"));
		SetWindowText(hWnd, s);

		GetWindowRect(hWnd, &rc);
		border = GetSystemMetrics(SM_CYFRAME);
		caption = GetSystemMetrics(SM_CYCAPTION);

		MoveWindow(hWnd, rc.left, rc.top, getWindowLength(margin, gap, sizeM, rowM) + border * 4,
			getWindowLength(margin, gap, sizeM, rowM) + border * 3 + caption * 2, TRUE);

		CreateChildren(hWnd, hwndRect, sizeM, rowM, gap, margin);
		row = rowM;

		PostMessage(hWnd, WM_INITOVERLAY, 0, 0);
	}
	return 0;
	break;
	case WM_INITOVERLAY:
	{	
		hWndTop = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
			TEXT("OVERLAY"), NULL, WS_POPUP | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			NULL, NULL, hInst, NULL);
		SetLayeredWindowAttributes(hWndTop, 0xFEEDBEEF, 80, LWA_ALPHA);
		SetWindowPos(hWndTop, HWND_TOPMOST, 0, 0, 
			GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 
			SWP_NOOWNERZORDER);
	}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BORDERSIZE_SMALL:
		{

			GetWindowRect(hWnd, &rc);
			border = GetSystemMetrics(SM_CYFRAME);
			caption = GetSystemMetrics(SM_CYCAPTION);
			row = rowS;
			EnumChildWindows(hWnd, DestoryChildCallback, lParam);
			CreateChildren(hWnd, hwndRect, sizeS, row, gap, margin);
			MoveWindow(hWnd, rc.left, rc.top, getWindowLength(margin, gap, sizeS, row) + border * 4,
				getWindowLength(margin, gap, sizeS, row) + border * 3 + caption * 2, TRUE);
			isGameStarted = false;
			KillTimer(hWnd, INITTIMER);
			KillTimer(hWnd, RUNNINGTIMER);
			
		}
		break;
		case ID_BORDERSIZE_MEDIUM:
		{

			
			GetWindowRect(hWnd, &rc);
			border = GetSystemMetrics(SM_CYFRAME);
			caption = GetSystemMetrics(SM_CYCAPTION);
			row = rowM;
			EnumChildWindows(hWnd, DestoryChildCallback, lParam);
			CreateChildren(hWnd, hwndRect, sizeM, row, gap, margin);
			MoveWindow(hWnd, rc.left, rc.top, getWindowLength(margin, gap, sizeM, row) + border * 4,
				getWindowLength(margin, gap, sizeM, row) + border * 3 + caption * 2, TRUE);
			isGameStarted = false;
			KillTimer(hWnd, INITTIMER);
			KillTimer(hWnd, RUNNINGTIMER);
			
		}
		break;
		case ID_BORDERSIZE_BIG:
		{
			GetWindowRect(hWnd, &rc);
			border = GetSystemMetrics(SM_CYFRAME);
			caption = GetSystemMetrics(SM_CYCAPTION);
			row = rowL;
			EnumChildWindows(hWnd, DestoryChildCallback, lParam);
			CreateChildren(hWnd, hwndRect, sizeL, row, gap, margin);
			MoveWindow(hWnd, rc.left, rc.top, getWindowLength(margin, gap, sizeL, row) + border * 4,
				getWindowLength(margin, gap, sizeL, row) + border * 3 + caption * 2, TRUE);
			isGameStarted = false;
			KillTimer(hWnd, INITTIMER);
			KillTimer(hWnd, RUNNINGTIMER);
			
		}
		break;
		case IDM_NEWGAME:
		{
			srand(time(NULL));
			if (isGameStarted)
			{
				KillTimer(hWnd, RUNNINGTIMER);
				for (int i = 0; i < row*row; i++)
					board[i] = rand() % 6 + 1;

				SetTimer(hWnd, INITTIMER, 25, (TIMERPROC)NULL);
			}
			if (!isGameStarted)
			{
				isGameStarted = true;
				//Initialize the board
				for (int i = 0; i < row*row; i++)
					board[i] = rand() % 6 + 1;

				SetTimer(hWnd, INITTIMER, 25, (TIMERPROC)NULL);
			}
		}
		break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_GAME_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here..
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
	{
		switch (wParam)
		{
		case INITTIMER:
		{
			//Paint the client area
			if (hwndRect[enumrator] != NULL) {
				hdc = GetDC(hWnd);
				GetClientRect(hWnd, &rc);
				FillRect(hdc, &rc, (HBRUSH)COLOR_BACKGROUND + 2);
				PostMessage(hwndRect[enumrator], WM_INITBOARD, board[enumrator], 0);
				enumrator++;
			}
			else
			{
				hdc = GetDC(hWnd);
				GetClientRect(hWnd, &rc);
				FillRect(hdc, &rc, (HBRUSH)COLOR_BACKGROUND + 1);
				KillTimer(hWnd, INITTIMER);
				SetTimer(hWnd, RUNNINGTIMER, 800, (TIMERPROC)NULL);

			}
			return 0;
		}
		break;
		case RUNNINGTIMER:
		{
			if (isGameStarted && !isFinishedBoard(row))
			{
				hdc = GetDC(hWnd);
				GetClientRect(hWnd, &rc);
				FillRect(hdc, &rc, (HBRUSH)COLOR_BACKGROUND + 2);
				Drop(row);
				//SetWindowText(hWnd, _T("Droped!!"));
				for (enumrator = 0; hwndRect[enumrator] != NULL; enumrator++)
				{
					PostMessage(hwndRect[enumrator], WM_RUNNINGINTERVAL, 0, 0);
				}
				return 0;
			}
			if (isGameStarted && isFinishedBoard(row) && isTriple(row, NOBINGO))
			{
				hdc = GetDC(hWnd);
				GetClientRect(hWnd, &rc);
				FillRect(hdc, &rc, (HBRUSH)COLOR_BACKGROUND + 2);
				isTriple(row, BINGO);
				PostMessage(hWndTop, WM_BINGOTOPARTICLES, row, 0);
				for (enumrator = 0; hwndRect[enumrator] != NULL; enumrator++)
				{
					PostMessage(hwndRect[enumrator], WM_RUNNINGINTERVAL, 0, 0);
				}
				//SetWindowText(hWnd, _T("Bingoed"));
				return 0;
			}
			//SetWindowText(hWnd, _T("KillTimer"));
			hdc = GetDC(hWnd);
			GetClientRect(hWnd, &rc);
			FillRect(hdc, &rc, (HBRUSH)COLOR_BACKGROUND + 1);
			KillTimer(hWnd, RUNNINGTIMER);

		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_SELECTED:
	{
		if (isGameStarted && isFinishedBoard(row)) {
			int pos = wParam;
			if (swap[0] == NULL || swap[0] == 0)
			{
				swap[0] = pos;
				//_stprintf_s(buf, bufSize, _T("first swap[0] = %d, swap[1] = %d"), swap[0], swap[1]);
				//SetWindowText(hWnd, buf);
				PostMessage(hwndRect[swap[0]], WM_SELECTEDCELL, 0, 0);
			}
			else if (swap[1] == NULL || swap[1] == 0)
			{
				if (pos == swap[0]) //Diselected
				{
					swap[0] = pos;
					swap[1] = 0;
					PostMessage(hwndRect[swap[0]], WM_SELECTEDCELL, 0, 0);
				}
				swap[1] = pos;

				if (!isNear(pos, swap[0], row))
				{
					swap[0] = pos;
					swap[1] = 0;
					PostMessage(hwndRect[swap[0]], WM_SELECTEDCELL, 0, 0);
				}
				else {
					swap[1] = pos;
					int temp = board[swap[0]];
					board[swap[0]] = board[swap[1]];
					board[swap[1]] = temp;
					if (!isTriple(row, NOBINGO))
					{
						temp = board[swap[0]];
						board[swap[0]] = board[swap[1]];
						board[swap[1]] = temp;
					}
					PostMessage(hwndRect[swap[0]], WM_SELECTEDCELL, 0, 0);
					for (enumrator = 0; hwndRect[enumrator] != NULL; enumrator++)
					{
						PostMessage(hwndRect[enumrator], WM_RUNNINGINTERVAL, 0, 0);
					}
					SetTimer(hWnd, RUNNINGTIMER, 800, (TIMERPROC)NULL);

					swap[0] = 0;
					swap[1] = 0;
				}
			}
		}
	}
	break;
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED) 
		{
			PostMessage(hWndTop, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		if (wParam == SIZE_RESTORED)
		{
			PostMessage(hWndTop, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			SetWindowPos(hWndTop, HWND_TOPMOST, 0, 0,
				GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
				SWP_NOOWNERZORDER);
		}
	}
	break;

	case WM_DESTROY:
		//KillTimer(hWnd, INITTIMER);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rc;
	HWND mainhWnd;
	static int enumrator = 0;
	static HWND oldBoardhWnd;
	static bool isGameShowing = false;
	const static HBRUSH colors[6] =
	{
		CreateSolidBrush(RGB(255, 0, 0)),
		CreateSolidBrush(RGB(0, 255, 0)),
		CreateSolidBrush(RGB(0, 0, 255)),
		CreateSolidBrush(RGB(255, 255, 0)),
		CreateSolidBrush(RGB(0, 255, 255)),
		CreateSolidBrush(RGB(255, 0, 255))
	};
	switch (message)
	{
	case WM_CREATE:
	{
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);


		GetClientRect(hWnd, &rc);
		//HBRUSH bgColor = CreateHatchBrush(HS_CROSS, RGB(0, 0, 255));
		FillRect(hdc, &rc, (HBRUSH)COLOR_BACKGROUND+3);
		if (isGameShowing) 
			FillRect(hdc, &rc, colors[board[GetDlgCtrlID(hWnd)] - 1]);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SIZE:
	{

	}
	break;

	case WM_LBUTTONDOWN:
	{
		HWND mainhWnd = GetParent(hWnd);
		PostMessage(mainhWnd, WM_SELECTED, GetDlgCtrlID(hWnd), 0);
	}
	break;
	case WM_SELECTEDCELL:
	{
		GetWindowRect(hWnd, &rc);
		OffsetRect(&rc, -rc.left, -rc.top);
		hdc = GetWindowDC(hWnd);

		HRGN margin = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 4, 4);
		FrameRgn(hdc, margin, CreateSolidBrush(RGB(0, 0, 0)), 4, 4);
		if (oldBoardhWnd != NULL)
		{
			GetWindowRect(oldBoardhWnd, &rc);
			OffsetRect(&rc, -rc.left, -rc.top);
			hdc = GetWindowDC(oldBoardhWnd);
			HRGN margin = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 4, 4);
			FrameRgn(hdc, margin, CreateSolidBrush(RGB(0, 0, 0)), 0, 0);
		}
		oldBoardhWnd = hWnd;
		return 0;
	}
	break;
	case WM_INITBOARD:
	{
		isGameShowing = true;
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rc);
		FillRect(hdc, &rc, colors[wParam - 1]);
	}
	break;
	case WM_RUNNINGINTERVAL:
	{
		isGameShowing = true;
		enumrator = 0;
		HWND mainhWnd = GetParent(hWnd);
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rc);
		TCHAR s[256];
		//FillRect(hdc, &rc, colors[board[wParam]-1]);
		_stprintf_s(s, 256, _T("%d"), board[GetDlgCtrlID(hWnd)]);
		FillRect(hdc, &rc, colors[board[GetDlgCtrlID(hWnd)] - 1]);

	}
	break;
	case WM_DESTROY:
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK OverlayProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static list<POINT> pointsList;
	const static int rad = 5;
	static bool NewExplosion = false;
	static int row;
	switch (message)
	{ 

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);

	}
		break;
	case WM_SIZE:
	{


	}
		break;
	//case WM_BINGOTOPARTICLES:
	//{
	//	if(pointsList.empty())
	//		SetTimer(hWnd, PARTICLETIMER, 15, NULL);
	//	row = wParam;
	//	CreateParticles(pointsList, rad, row);
	//}
	//	break;
	//case WM_TIMER:
	//	switch (wParam)
	//	{
	//	case PARTICLETIMER:
	//	{
	//		MoveParticles(hWnd, pointsList, rad, row, 5);
	//		if (pointsList.empty())
	//			KillTimer(hWnd, PARTICLETIMER);
	//	}
	//	default:
	//		break;
	//	}
	//	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL CALLBACK DestoryChildCallback(
	HWND   hwnd,
	LPARAM lParam
)
{
	if (hwnd != NULL) {
		DestroyWindow(hwnd);
	}
	return TRUE;
}

void Debug(const int msg, TCHAR *buf, int bufSize)
{
	_stprintf_s(buf, bufSize, _T("%d"), msg);
}

void CreateChildren(HWND hWnd, HWND hwndRect[], int size, int row, int gap, int margin)
{
	int iw = 0;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < row; j++) {
			hwndRect[iw++] = CreateWindowW(TEXT("CHILD"), NULL,
				WS_CHILDWINDOW | WS_VISIBLE,
				margin + (size + gap) * j,
				margin + (size + gap) * i,
				size, size,
				hWnd, (HMENU)iw,
				hInst,
				NULL
			);
		}
	}
}

bool isNear(int pos, int another, int row)
{
	int abs = pos - another;
	if (abs == 1 || abs == row || abs == -1 || abs == -row)
		return true;
	return false;
}
bool isTriple(int row, bool toBingo)
{
	bool is = false;
	for (int j = 0; j < row; j++) {
		for (int i = 0; i < row - 2; i++) {
			if (board[i + j * row] == board[(i + 1) + j * row] &&
				board[(i + 1) + j * row] == board[(i + 2) + j * row] &&
				board[i + j * row] == board[(i + 2) + j * row] &&
				board[i + j * row] != 0 &&
				board[i + 1 + j * row] != 0 &&
				board[i + 2 + j * row] != 0)
			{
				if (toBingo) {
					int in = i;
					while (--in >= 0) 
					{
						if (board[in + j * row] == board[i + j * row])
							board[in + j * row] = 0;
						else break;
					}
					in = i + 2;
					while (++in < row)
					{
						if (board[in + j * row] == board[i + j * row])
							board[in + j * row] = 0;
						else break;
					}
					Bingo(i + j * row, i + 1 + j * row, i + 2 + j * row);
				}
				is = true;
			}
		}
		if (j < row - 2)
		{
			for (int i = 0; i < row; i++) {
				if (board[j*row + i] == board[j*row + row + i] &&
					board[j*row + row + i] == board[j*row + row * 2 + i] &&
					board[j*row + i] == board[j*row + row * 2 + i] &&
					board[j*row + i] != 0 &&
					board[j*row + row + i] != 0 &&
					board[j*row + row * 2 + i] != 0)
				{
					if (toBingo) 
					{
						int jn = j;
						while (--jn >= 0)
						{
							if (board[i + jn * row] == board[j*row + i])
								board[i + jn * row] = 0;
							else break;
						}
						jn = j + 2;
						while (++jn < row)
						{
							if (board[i + jn * row] == board[j*row + i])
								board[i + jn * row] = 0;
							else break;
						}
						Bingo(j*row + i, j*row + row + i, j*row + row * 2 + i);
					}
					is = true;
				}
			}
		}
	}
	return is;
}

bool isFinishedBoard(int row)
{
	for (int i = 0; i < row*row; i++) {
		if (board[i] == 0)
			return false;
	}
	return true;
}

void Bingo(int index1, int index2, int index3)
{
	board[index1] = 0;
	board[index2] = 0;
	board[index3] = 0;
}

void Drop(int row)
{
	srand(time(NULL));
	bool finished_col[12];
	for (int i = 0; i < row; i++)
		finished_col[i] = false;
	for (int i = row - 1; i >= 0; i--) {
		for (int j = 0; j < row; j++) {
			if (board[j + i * row] == 0 && !finished_col[j])
			{
				int ir = i;
				do
				{
					board[j + ir * row] = board[j + (ir - 1)*row];
				} while (ir-- > 0);
				board[j] = rand() % 6 + 1;
				finished_col[j] = true;
			}
		}
	}
}

//void CreateParticles(list<POINT> pointsList, int rad, int row) 
//{
//	POINT pt;
//	RECT rc;
//	for(int i = 0; i < row*row; i++)
//	{
//		if (board[i] == 0) 
//		{	
//			GetWindowRect(hwndRect[i], &rc);
//			pt.x = (rc.left + rc.right) / 2;
//			pt.y = (rc.top + rc.bottom) / 2;
//			pointsList.push_back(pt);
//		}		
//	}
//}
//void MoveParticles(HWND hWnd, list<POINT> pointsList, int rad, int row, int offset)
//{
//	hdc = GetDC(hWnd);
//
//	HBRUSH brushBk = CreateSolidBrush(COLOR_BACKGROUND + 2);
//	HBRUSH oldBrushBk = (HBRUSH)SelectObject(hdc, &brushBk);
//	list <POINT>::iterator iter = pointsList.begin();
//	while (iter != pointsList.end())
//	{
//		POINT pt = *iter;//		Ellipse(hdc, pt.x - rad, pt.y - rad, pt.x + rad, pt.y + rad);
//		*iter++;
//	}
//	SelectObject(hdc, oldBrushBk);
//	DeleteObject(brushBk);
//	HBRUSH brush = CreateSolidBrush(RGB(128, 128, 0));
//	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, &brush);
//	//UpdateLayeredWindow(hWnd, hdc, );
//	iter = pointsList.begin();
//	while (iter != pointsList.end())
//	{
//		POINT pt = *iter;//		pt.x = pt.x + offset;//		pt.y += pt.y + offset;//		list <POINT>::iterator lsave;//		if (pt.x > GetSystemMetrics(SM_CXSCREEN) || pt.x < 0 ||
//			pt.y > GetSystemMetrics(SM_CYSCREEN) || pt.y < 0)
//		{
//			lsave = iter;
//			pointsList.erase(lsave);
//		}
//		iter++;//		Ellipse(hdc, pt.x - rad, pt.y - rad, pt.x + rad, pt.y + rad);
//
//
//	}
//	SelectObject(hdc, oldBrush);
//	DeleteObject(brush);
//
//}

int getWindowLength(int margin, int gap, int size, int row)
{
	return margin * 2 + gap * (row - 1) + size * row;
}
