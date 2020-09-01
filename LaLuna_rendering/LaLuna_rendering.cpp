// LaLuna_rendering.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LaLuna_rendering.h"
#include "LMatrix.h"
#include"LVector.h"
#include"LunaScene.h"
#include"LVert.h"
#include"myDefine.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LunaScene* scene;
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//----begin yang chao
	//----create console
	AllocConsole();   // 建立控制台
	SetConsoleTitle(_T("Debug Output"));      // 设置控制台窗口标题
	// 重定向 STDIN
	{
		FILE*fp;
		errno_t eno = freopen_s(&fp, "CONIN$", "r+t", stdin);
		if (eno != 0){//不成功
			MessageBox(NULL, _T("重定向stdin失败!"), _T("error"), MB_OK | MB_ICONEXCLAMATION);
			exit(0);
		}
	}
	// 重定向STDOUT
	{
		FILE*fp;
		errno_t eno = freopen_s(&fp, "CONOUT$", "w+t", stdout);
		if (eno != 0){//不成功
			MessageBox(NULL, _T("重定向stdout失败!"), _T("error"), MB_OK | MB_ICONEXCLAMATION);
			exit(0);
		}

	}

	//----end yang chao
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LALUNA_RENDERING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LALUNA_RENDERING));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	FreeConsole();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LALUNA_RENDERING));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LALUNA_RENDERING);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
      CW_USEDEFAULT, 0, g_windowswidth, g_windowheight, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //create a scene which shows a obeject
   
   //set the scene size:viewport
   RECT rect;
   GetClientRect(hWnd, &rect);
   cout << "clientRect:" << endl;
   cout << "rect top = " << rect.top << endl;
   cout << "rect bottom = " << rect.bottom << endl;
   cout << "rect left = " << rect.left << endl;
   cout << "rect right = " << rect.right << endl;


   LVector4 viewport(rect.top, rect.left, rect.right - rect.left,rect.bottom - rect.top);//(0,0，width,height)
   scene = new LunaScene();
   scene->init(viewport);




   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	static int cxClient, cyClient;
	static HDC hdc_back_buffer;
	static HBITMAP h_bitmap;
	static HBITMAP h_old_bitmap;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CREATE:{
		RECT rect;
		GetClientRect(hWnd, &rect);
		cxClient = rect.right;
		cyClient = rect.bottom;

		hdc_back_buffer = CreateCompatibleDC(NULL);
		HDC hdc = GetDC(hWnd);
			h_bitmap = CreateCompatibleBitmap(hdc, cxClient, cyClient);
		h_old_bitmap = (HBITMAP)SelectObject(hdc_back_buffer, h_bitmap);
		ReleaseDC(hWnd, hdc);

		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		BitBlt(hdc_back_buffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, BLACKNESS);
		//double render buffer?

		//rasteration
		cout << "while" << endl;
		scene->softRasterization(hdc_back_buffer);			
		BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdc_back_buffer, 0,0, SRCCOPY);
		EndPaint(hWnd, &ps);
		
	}
	break;
	case WM_MOUSEMOVE:
	{
		scene->m_camera->updateVector();
		InvalidateRect(hWnd, NULL, false);
	}
	break;
	case WM_DESTROY:
	{

		SelectObject(hdc_back_buffer, h_old_bitmap);
		DeleteDC(hdc_back_buffer);
		DeleteObject(h_bitmap);
		delete scene;
		PostQuitMessage(0);
	}

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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
