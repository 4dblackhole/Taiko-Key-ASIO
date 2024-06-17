// WindowsProject1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowsProject1.h"

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

static FMOD::System* _system(nullptr);
static FMOD::Sound* sound(nullptr);
static FMOD::Sound* don(nullptr);
static FMOD::Sound* kat(nullptr);
UINT donLength{}, katLength{};
static FMOD::Channel* donChannel(nullptr);
static FMOD::Channel* katChannel(nullptr);
static FMOD_RESULT       result;
static unsigned int      version;
void* extradriverdata(nullptr);  // 본 예제에서는 사용 x

std::queue<FMOD::Channel*>channels;

HBITMAP bitmapFmodasio, oldbit;

void ReleaseChannel(FMOD::Channel* ch, UINT soundLength)
{
    using namespace FMOD;
    constexpr UINT playsoundoverhead = 30;
    
    this_thread::sleep_for(chrono::milliseconds(soundLength + playsoundoverhead));
    ch->stop();
}

static void PlayDon()
{
    // stop the music if the same file is running
    thread st([&](FMOD::Channel* ch) {ch->stop(); }, donChannel);
    _system->playSound(don, 0, false, nullptr);
    thread th(ReleaseChannel, donChannel, donLength);
    st.detach();
    th.detach();
}

static void PlayKat()
{
    // stop the music if the same file is running
    thread st([&](FMOD::Channel* ch) {ch->stop(); }, katChannel);
    _system->playSound(kat, 0, false, &katChannel);
    thread th(ReleaseChannel, katChannel, katLength);
    st.detach();
    th.detach();
}

RhythmInputManager kddk;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
        RegisterKeyHook(hWnd);
        result = FMOD::System_Create(&_system);
        if (result != FMOD_OK) return -1;  

        result = _system->getVersion(&version);
        if (result != FMOD_OK) return -1;

        int drivenums;
        _system->getNumDrivers(&drivenums);
        for (int i = 0; i < drivenums; ++i)
        {
            char name[256];
            _system->getDriverInfo(i, (char*)name, sizeof(name), 0, 0, 0, 0);
            
            WCHAR uni[256]{};
            int nLen = MultiByteToWideChar(CP_UTF8, 0, name, (int)strlen(name), NULL, NULL);
            MultiByteToWideChar(CP_UTF8, 0, name, (int)strlen(name), uni, nLen);
            int d = 3;
            //MessageBox(hWnd, uni, _T("Audio Driver"), MB_OK);
        }

        unsigned int bufferlen;
        int buffers;
        _system->getDSPBufferSize(&bufferlen, &buffers);
        _system->setDSPBufferSize(64, 4); 
        _system->getDSPBufferSize(&bufferlen, &buffers);

        FMOD_OUTPUTTYPE t;
        _system->getOutput(&t);
        _system->setOutput(FMOD_OUTPUTTYPE_ASIO);
        _system->getOutput(&t);

        result = _system->init(64, FMOD_INIT_NORMAL, extradriverdata);
        if (result != FMOD_OK) return -1;
        
        //result = _system->createSound("singing.wav", FMOD_LOOP_NORMAL, 0, &sound);
        result = _system->createSound("HitSounds/don.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &don); // wav 파일로부터 sound 생성
        result = _system->createSound("HitSounds/kat.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &kat); // wav 파일로부터 sound 생성
        if (result != FMOD_OK) return -1;

        don->getLength(&donLength, FMOD_TIMEUNIT_MS);
        kat->getLength(&katLength, FMOD_TIMEUNIT_MS);

        //result = _system->playSound(sound,0, false, &channel); // 재생. 단 이때 딱 한번만 실행되므로 제대로 사운드가 끝까지 재생되지 않는다.  무한루프 안에서 시스템 객체를 계~~속 업데이트 시켜줘야 함.
        //if (result != FMOD_OK) return -1;
        kddk.RegisterAction(0, PlayKat);
        kddk.RegisterAction(1, PlayDon);
        kddk.RegisterAction(2, PlayDon);
        kddk.RegisterAction(3, PlayKat);
    
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
                        kddk.OnKeyDown(ri.data.keyboard.VKey);
                        break;
                    case RI_KEY_BREAK: //KeyUp
                        kddk.OnKeyUp(ri.data.keyboard.VKey);
                        break;
                    }

                }
            }
        }
        break;
    case WM_KEYDOWN:
        {
            kddk.OnKeyDown((UINT)wParam);
        }
        break;
    case WM_KEYUP:
         {
            kddk.OnKeyUp((UINT)wParam);
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
            sound->release();
            don->release();
            kat->release();
            _system->close();
            _system->release();
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
