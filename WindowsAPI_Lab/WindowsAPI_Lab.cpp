// WindowsAPI_Lab.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "WindowsAPI_Lab.h"
#include <cstdlib>
#include <time.h>
using namespace std;
#define MAX_LOADSTRING 100
#define ID_FIRSTCHILD  100 
#define ID_SECONDCHILD 101 
#define ID_THIRDCHILD  102 

#define INITTIMER 11
#define RUNNINGTIMER 12

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM				RegisterChildClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ChildProc(HWND, UINT, WPARAM, LPARAM);
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
	//RegisterChildClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPILAB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSAPILAB);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+2);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSAPILAB);
	wcex.lpszClassName = TEXT("CHILD");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	   | WS_SYSMENU,
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
int getWindowLength(int margin, int gap, int size, int row);


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
	int clientWidth, clientHeight;
	static HBRUSH hbrBkgnd = NULL;
	//Childwindow elements
	static HWND hwndRect[rowL * rowL];
	int iw=0, ik, im, cxClient, cyClient;
	//Control identifier
	static bool isGameStarted = false;
	static int color[rowL * rowL];
	static int enumrator = 0;

    switch (message)
    {

	case WM_CREATE:
	{
		TCHAR s[256];
		_stprintf_s(s, 256, _T("BeWindowed 2020 a.k.a. Bejeweled in WinAPI"));
		SetWindowText(hWnd, s);
		CreateChildren(hWnd, hwndRect, sizeM, rowM, gap, margin);

		GetWindowRect(hWnd, &rc);
		border = GetSystemMetrics(SM_CYFRAME);
		caption = GetSystemMetrics(SM_CYCAPTION);

		MoveWindow(hWnd, rc.left, rc.top, getWindowLength(margin, gap, sizeM, rowM) + border * 4,
			getWindowLength(margin, gap, sizeM, rowM) + border * 3 + caption * 2, TRUE);
	}
	return 0;
		break;
    case WM_COMMAND:
        {
			switch (HIWORD(wParam))
			{
			case STN_CLICKED:
			{	
				int hwndId = LOWORD(wParam);
				Debug(LOWORD(wParam), buf, bufSize);
				SetWindowText(hWnd, buf);

				HDC hdc = GetDC(hwndRect[hwndId]);
				//DrawEdge();
			}
				break;
			default:
				break;
			}
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_BORDERSIZE_SMALL:
			{
				GetWindowRect(hWnd, &rc);
				border = GetSystemMetrics(SM_CYFRAME);
				caption = GetSystemMetrics(SM_CYCAPTION);
				EnumChildWindows(hWnd, DestoryChildCallback, lParam);
				CreateChildren(hWnd, hwndRect, sizeS, rowS, gap, margin);
				MoveWindow(hWnd, rc.left, rc.top, getWindowLength(margin, gap, sizeS, rowS) + border * 4,
					getWindowLength(margin, gap, sizeS, rowS) + border * 3 + caption * 2, TRUE);

			}
				break;
			case ID_BORDERSIZE_MEDIUM:
			{
				GetWindowRect(hWnd, &rc);
				border = GetSystemMetrics(SM_CYFRAME);
				caption = GetSystemMetrics(SM_CYCAPTION);
				EnumChildWindows(hWnd, DestoryChildCallback, lParam);
				CreateChildren(hWnd, hwndRect, sizeM, rowM, gap, margin);
				MoveWindow(hWnd, rc.left, rc.top, getWindowLength(margin, gap, sizeM, rowM) + border * 4,
					getWindowLength(margin, gap, sizeM, rowM) + border * 3 + caption * 2, TRUE);
			}
				break;
			case ID_BORDERSIZE_BIG:
			{
				GetWindowRect(hWnd, &rc);
				border = GetSystemMetrics(SM_CYFRAME);
				caption = GetSystemMetrics(SM_CYCAPTION);
				EnumChildWindows(hWnd, DestoryChildCallback, lParam);
				CreateChildren(hWnd, hwndRect, sizeL, rowL, gap, margin);
				MoveWindow(hWnd, rc.left, rc.top, getWindowLength(margin, gap, sizeL, rowL) + border * 4,
					getWindowLength(margin, gap, sizeL, rowL) + border * 3 + caption * 2, TRUE);
			}
				break;
			case IDM_NEWGAME:
			{
				if (!isGameStarted) 
				{
					isGameStarted = true;
					SetTimer(hWnd, INITTIMER, 50, (TIMERPROC)NULL);
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
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...




            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(0, 123, 0));


		if (GetDlgCtrlID((HWND)lParam) == 15)
			hbrBkgnd = CreateSolidBrush(RGB(120, 120, 0));
		else
			hbrBkgnd = CreateSolidBrush((RGB(0, 0, 122)));
		

		return (INT_PTR)hbrBkgnd;
	}
	case WM_TIMER:
	{
		switch (wParam)
		{
		case INITTIMER:
		{

			//Paint the client area
			if (hwndRect[enumrator] != NULL) 
			{
				//hbrBkgnd = CreateSolidBrush((RGB(255, 255, 255)));
				//SetClassLongPtr(hwndRect[enumrator], GCLP_HBRBACKGROUND, (LONG_PTR)hbrBkgnd);

				enumrator++;

			}
			else 
			{
				enumrator = 0;
				KillTimer(hWnd, INITTIMER);	
			}
			return 0;
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_SIZE:
	{

	}
	break;

    case WM_DESTROY:
		KillTimer(hWnd, INITTIMER);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:

		break;

	case WM_SIZE:


		break;


	default:
		HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (stdOut != NULL && stdOut != INVALID_HANDLE_VALUE)
		{
			DWORD written = 0;
			const char *message = "hello world";
			WriteConsoleA(stdOut, message, strlen(message), &written, NULL);
		}
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
				WS_CHILDWINDOW | WS_VISIBLE ,
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

int getWindowLength(int margin, int gap, int size, int row) 
{
	return margin * 2 + gap * (row - 1) + size * row;
}
