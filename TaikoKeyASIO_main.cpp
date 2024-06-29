// TaikoKeyASIO_main.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "RhythmInputManager.h"
#include "Object/FmodSystem.h"

#include "Utility/StringToVirtualKeyCode.h"

#include "TaikoKeyASIO_main.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.



// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#if defined (DEBUG) | defined (_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // 기본 메시지 루프입니다:
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


static void SetClientRect(HWND hWnd, int width, int height)
{
    RECT crt;
    DWORD Style, ExStyle;

    SetRect(&crt, 0, 0, width, height);
    Style = (DWORD)GetWindowLongPtr(hWnd, GWL_STYLE);
    ExStyle = (DWORD)GetWindowLongPtr(hWnd, GWL_EXSTYLE);
    AdjustWindowRectEx(&crt, Style, GetMenu(hWnd) != NULL, ExStyle);
    if (Style & WS_VSCROLL)crt.right += GetSystemMetrics(SM_CXVSCROLL);
    if (Style & WS_HSCROLL)crt.bottom += GetSystemMetrics(SM_CYVSCROLL);
    SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
        SWP_NOMOVE | SWP_NOZORDER);
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;//MAKEINTRESOURCE(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW&(~WS_THICKFRAME) & (~WS_MAXIMIZEBOX),
      CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   SetClientRect(hWnd, 400, 400);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

static void RegisterKeyHook(const HWND targetWnd)
{
    RAWINPUTDEVICE rawInputDev{};

    // 키보드 RAWINPUTDEVICE 구조체 설정
    rawInputDev.usUsagePage = 0x01;
    rawInputDev.usUsage = 0x06;
    rawInputDev.dwFlags = RIDEV_INPUTSINK;
    rawInputDev.hwndTarget = targetWnd;

    RegisterRawInputDevices(&rawInputDev, 1, sizeof(RAWINPUTDEVICE));
}


static FmodSystem fsys;
RhythmInputManager rhythmInputManager;

static HWND hCombo, hComboDesc;
static HWND hButtonKeyLoad;
static HWND hTrackbarVolume, hVolumeDesc;
static HBITMAP bitmapFmodasio, oldbit;
static HBRUSH asioBrush;
HFONT fontSize20, fontSize16;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static FMOD_RESULT result;

    switch (message)
    {
    case WM_CREATE:
        {
        RegisterKeyHook(hWnd); //it catches key input when this window has not focus

        asioBrush = CreateSolidBrush(RGB(6, 6, 6));

        //Init Ctrls
        hCombo = CreateWindow(_T("combobox"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL| CBS_DROPDOWNLIST ,
            70, DriverComboY, 300, 200, hWnd, (HMENU)HwndID::COMBO_DRIVER, hInst, NULL);
        hComboDesc = CreateWindow(_T("static"), _T("Driver:"), WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, DriverComboY, 60, 24, hWnd, (HMENU)HwndID::STATIC_COMBO_DRIVER, hInst, NULL);
        hButtonKeyLoad = CreateWindow(_T("button"), _T("Reload ini File"), WS_CHILD | WS_VISIBLE | BS_CENTER,
            10, ReloadButtonY, 110, 30, hWnd, (HMENU)HwndID::BUTTON_RELOAD_INI, hInst, NULL);
        hTrackbarVolume = CreateWindow(TRACKBAR_CLASS, _T("Volume"), WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_BOTH | TBS_NOTICKS | WS_TABSTOP,
            240, ReloadButtonY, 150, 30, hWnd, (HMENU)HwndID::TRACKBAR_VOLUME, hInst, NULL);
        hVolumeDesc = CreateWindow(_T("static"), _T("Volume: 100"), WS_CHILD | WS_VISIBLE | SS_LEFT,
            130, ReloadButtonY+4, 100, 24, hWnd, (HMENU)HwndID::STATIC_VOLUME, hInst, NULL);

        SendMessage(hTrackbarVolume, TBM_SETRANGE, FALSE, MAKELPARAM(0, 100));
        SendMessage(hTrackbarVolume, TBM_SETPOS, TRUE, 100);

        //font
        HDC dc = GetDC(hWnd);
        LOGFONT lf;
        HFONT oldFont = (HFONT)GetCurrentObject(dc, OBJ_FONT);
        GetObject(oldFont, sizeof(LOGFONT), &lf);
        lf.lfHeight = 20;
        lf.lfWidth = 8;
        lf.lfWeight = FW_BOLD;
        _stprintf_s(lf.lfFaceName, _T("sans"));
        fontSize20 = CreateFontIndirect(&lf);
        SendMessage(hComboDesc, WM_SETFONT, (WPARAM)fontSize20, MAKELPARAM(TRUE, 0));
        lf.lfHeight = 16;
        lf.lfWidth = 7;
        fontSize16 = CreateFontIndirect(&lf);
        SendMessage(hVolumeDesc, WM_SETFONT, (WPARAM)fontSize16, MAKELPARAM(TRUE, 0));
        ReleaseDC(hWnd, dc);

        //fmod system init
        fsys.Init();
        fsys.EnumDriverListToComboBox(hCombo);
        SendMessage(hCombo, CB_SETCURSEL, fsys.GetDriveIdx(), 0);

        result = rhythmInputManager.Init(fsys.System());
        if (result != FMOD_OK) return -1;

        }
        break;
    //change ctrl color
    case WM_CTLCOLORSTATIC:
        if ((HWND)lParam == hComboDesc)
        {
            SetTextColor((HDC)wParam, RGB(255, 255, 255));
            SetBkMode((HDC)wParam, TRANSPARENT);
            return (INT_PTR)(HBRUSH)GetStockObject(NULL_BRUSH);
        }
        else if ((HWND)lParam == hTrackbarVolume || (HWND)lParam == hVolumeDesc)
        {
            SetTextColor((HDC)wParam, RGB(255, 255, 255));
            SetBkMode((HDC)wParam, TRANSPARENT);
            return (INT_PTR)asioBrush;
        }
        else return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case (int)HwndID::COMBO_DRIVER:
            {
                switch (HIWORD(wParam)) 
                {
                case CBN_SELCHANGE:
                    {
                        fsys.ChangeDrive((int)SendMessage(hCombo, CB_GETCURSEL, 0, 0));
                    }  
                    break;
                }
            }
            break;

            case (int)HwndID::BUTTON_RELOAD_INI:
            {
                rhythmInputManager.ReadIniFile();
            }
            break;
            

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

    case WM_HSCROLL:
        {   
            if ((HWND)lParam == hTrackbarVolume)
            {
                int vol = (int)SendMessage(hTrackbarVolume, TBM_GETPOS, 0, 0);
                TCHAR tempStr[60];
                _stprintf_s(tempStr, _T("Volume: %d"), vol);
                SetWindowText(hVolumeDesc, tempStr);
                rhythmInputManager.ChangeVolume((float)vol / 100.0f);
            }
            else return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    //key hook
    case WM_INPUT:
        {
            if (wParam == RIM_INPUTSINK)
            {
                RAWINPUT ri;
                UINT sizeri = sizeof(ri);
                GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &ri, &sizeri, sizeof(RAWINPUTHEADER));
                if (ri.header.dwType == RIM_TYPEKEYBOARD)
                {
                    switch(ri.data.keyboard.Flags)
                    {
                    case RI_KEY_MAKE: //KeyDown
                        rhythmInputManager.OnKeyDown(ri.data.keyboard.VKey);
                        break;
                    case RI_KEY_BREAK: //KeyUp
                        rhythmInputManager.OnKeyUp(ri.data.keyboard.VKey);
                        break;
                    }

                }
            }
        }
        break;
    case WM_LBUTTONDOWN:
        {
            SetFocus(hWnd);
        }
        break;
    case WM_KEYDOWN:
        {
            rhythmInputManager.OnKeyDown((UINT)wParam);
        }
        break;
    case WM_KEYUP:
        {
            rhythmInputManager.OnKeyUp((UINT)wParam);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            bitmapFmodasio = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_FMODASIO));
            HDC hMemDC = CreateCompatibleDC(hdc);
            oldbit = (HBITMAP)SelectObject(hMemDC, bitmapFmodasio);
            BitBlt(hdc, 0, 0, 400, 400, hMemDC, 0, 0, SRCCOPY);
            SelectObject(hdc, oldbit);
            DeleteObject(bitmapFmodasio);
            DeleteDC(hMemDC);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        {
            rhythmInputManager.Release();
            fsys.Release();
            DeleteObject(asioBrush);
            DeleteObject(fontSize20);
            DeleteObject(fontSize16);
            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
