// winapiDigiWatch.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "winapiDigiWatch.h"
#pragma warning(disable : 4996)

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void TimerProc(HWND, UINT, UINT_PTR, DWORD);


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
    LoadStringW(hInstance, IDC_WINAPIDIGIWATCH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIDIGIWATCH));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIDIGIWATCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIDIGIWATCH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
fireworks* mgr = nullptr;
HBITMAP hbuff;
//graphic
void TimerProc(HWND hWnd, UINT msg, UINT_PTR nTimer, DWORD time_arg) {
    PAINTSTRUCT ps;
    HDC hdc = GetDC(hWnd);
    RECT tbox;
    GetClientRect(hWnd, &tbox);
    if (hbuff == NULL) {
        hbuff = CreateCompatibleBitmap(hdc, tbox.right,tbox.bottom);
    }
    HDC hMemDC = CreateCompatibleDC(hdc);
    FillRect(hMemDC, &tbox, GetSysColorBrush(COLOR_WINDOW));
    HBITMAP oldMap = (HBITMAP)SelectObject(hMemDC, hbuff);
    HBRUSH oldBrush;
    HPEN oldPen;
    
    HPEN noPen = (HPEN)GetStockObject(NULL_PEN);
    oldPen = (HPEN)SelectObject(hMemDC, noPen);
    HBRUSH mint = CreateSolidBrush(RGB(0x00, 0xB9, 0x92));
    oldBrush = (HBRUSH)SelectObject(hMemDC, mint);

    Rectangle(hMemDC, 0, 0, tbox.right + 1, tbox.bottom + 1);

    SelectObject(hMemDC, oldBrush);
    const int margin = 80;
    Rectangle(hMemDC, margin, margin, tbox.right - margin, tbox.bottom - margin);

    POINT center = { tbox.right / 2, tbox.bottom / 2 };

    HBRUSH blk = (HBRUSH)CreateSolidBrush(RGB(0xFF, 0x00, 0x00));
    oldBrush = (HBRUSH)SelectObject(hMemDC, blk);

    HRGN crt = CreateRectRgnIndirect(&tbox);
    HRGN rgn = CreateRectRgn(200, center.y - 50, tbox.right - 200, center.y + 50);
    /*
    SelectObject(hdc, oldPen);
    for (int i = 0; i < 3; i++) {
        MoveToEx(hdc, 200,(center.y - 50) + i*50 ,NULL);
        LineTo(hdc, tbox.right - 200, (center.y - 50) + i * 50);
    }
    oldPen = (HPEN)SelectObject(hdc, noPen);
    */



    if (mgr != nullptr) {
        mgr->draw(hWnd,hMemDC);
    }

    SelectClipRgn(hMemDC, rgn);

    //Rectangle(hdc, 0, 0, tbox.right, tbox.bottom);

    HFONT hf = CreateFont(80, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, NULL);
    HFONT oldfont = (HFONT)SelectObject(hMemDC, hf);
    SetTextAlign(hMemDC, TA_CENTER | TA_TOP);

    char* strTime;
    time_t mytime;
    time(&mytime);
    strTime = ctime(&mytime);

    char hh[3] = { 0,0,'\n' };
    char mm[3] = { 0,0,'\n' };
    char ss[3] = { 0,0,'\n' };

    strncpy(hh, strTime + 11, 2);
    strncpy(mm, strTime + 14, 2);
    strncpy(ss, strTime + 17, 2);
    int s = atoi(ss);
    int m = atoi(mm);
    int h = atoi(hh);
    static BOOL hscrolling = 0;
    static BOOL mscrolling = 0;
    static BOOL sscrolling = 0;
    int tick = (time_arg + 427) % 1000;

    //scrollup hh
    if (m == 0 && s == 0 && tick < 20 && hscrolling == 0) {
        hscrolling = 1;
        //mgr->setFireWorks(200, tbox.bottom, 3, 1, 3.14 / 2, 500);
    }
    if (hscrolling) {
        int yoff = tick / 5;
        int oldh;
        if (h == 0) oldh = 23;
        else oldh = h - 1;
        char t[3] = {0,0,0};
        TextOutA(hMemDC, center.x - 120, center.y - 40 - yoff, itoa(oldh,t,10), 2);
        TextOutA(hMemDC, center.x - 120, center.y - 40 + 100 - yoff, hh, 2);
        if (yoff > 100) hscrolling = 0;
    }
    else {
        TextOutA(hMemDC, center.x - 120, center.y - 40, hh, 2);
    }
    TextOut(hMemDC, center.x - 60, center.y - 40, TEXT(":"), 1);
    //scrollup mm
    if (s == 0 && tick < 20 && mscrolling == 0) {
        mscrolling = 1;
        //mgr->setFireWorks(200, tbox.bottom, 3, 1, 3.14 / 2, 500);
    }
    if (mscrolling) {
        int yoff = tick / 5;
        int oldm;
        if (m == 0) oldm = 59;
        else oldm = m - 1;
        char t[3] = { 0,0,0 };
        TextOutA(hMemDC, center.x, center.y - 40 - yoff, itoa(oldm,t,10), 2);
        TextOutA(hMemDC, center.x, center.y - 40 + 100 - yoff, mm, 2);
        if (yoff > 100) mscrolling = 0;
    }
    else {
        TextOutA(hMemDC, center.x, center.y - 40, mm, 2);
    }
    TextOut(hMemDC, center.x + 60, center.y - 40, TEXT(":"), 1);
    TextOutA(hMemDC, center.x + 120, center.y - 40, ss, 2);
    /*
    if (tick < 20) sscrolling = 1;
    if (sscrolling) {
        int yoff = tick / 10;
        int olds;
        if (s == 0) olds = 59;
        else olds = s - 1;
        char t[3] = { 0,0,0 };
        TextOutA(hMemDC, center.x + 120, center.y - 40 - yoff, itoa(olds, t, 10), 2);
        TextOutA(hMemDC, center.x + 120, center.y - 40 + 100 - yoff, ss, 2);
        if (yoff > 100) sscrolling = 0;
    }else {
        TextOutA(hMemDC, center.x + 120, center.y - 40, ss, 2);
    }*/
    

    SelectObject(hMemDC, oldfont);
    SelectClipRgn(hMemDC, crt);
    SelectObject(hMemDC, oldBrush);
    SelectObject(hMemDC, oldPen);
    DeleteObject(hf);
    DeleteObject(blk);
    DeleteObject(mint);
    DeleteObject(noPen);

    SelectObject(hMemDC, oldMap);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd, hdc);
    InvalidateRect(hWnd, NULL, FALSE);
    
}

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
    
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
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
        }
        break;
    case WM_LBUTTONDOWN:
        mgr->setFireWorks(300, 500, 2, 0.1, 3.14 / 2, 2000);
        break;
    case WM_CREATE:
        mgr = fireworks::getMgr(10);
        SetTimer(hWnd, 1, 20, (TIMERPROC)TimerProc);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            RECT crt;
            GetClientRect(hWnd, &crt);
            HDC hMem = CreateCompatibleDC(hdc);
            HBITMAP oldbit = (HBITMAP)SelectObject(hMem, hbuff);
            BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMem, 0, 0, SRCCOPY);

            SelectObject(hMem, oldbit);
            DeleteDC(hMem);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
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
