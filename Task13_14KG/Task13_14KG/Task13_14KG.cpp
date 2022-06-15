#include "framework.h"
#include "Task13_14KG.h"
#include <d3d9.h>
#include <d3dtypes.h>
#include <DirectXMath.h>
#include <math.h> 
#define MAX_LOADSTRING 100
#pragma comment  (lib, "d3d9.lib")
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

IDirect3DTexture9* g_pTexture;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
D3DLIGHT9 light;
D3DCOLOR c = D3DCOLOR_ARGB(255, 255, 255, 255);

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASK1314KG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK1314KG));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_TASK1314KG));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TASK1314KG);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

LPDIRECT3D9 direct3d = NULL;
LPDIRECT3DDEVICE9 device = NULL;
D3DDISPLAYMODE display;
D3DPRESENT_PARAMETERS params;
//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    direct3d = Direct3DCreate9(D3D_SDK_VERSION);
    direct3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display); ZeroMemory(&params, sizeof(params)); params.Windowed = TRUE; params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.BackBufferFormat = display.Format; direct3d->CreateDevice(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device
    );
    // структкура с параметрами девайса
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp)); // обнулим
    d3dpp.BackBufferWidth = 1000; // указываем ширину и
    d3dpp.BackBufferHeight = 1000; // высоту области рендера
    d3dpp.BackBufferCount = 1; // один внеэкранный буфер
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Flags = 0;
    d3dpp.Windowed = TRUE; // рендер в окне, а не в полный экран
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // формат выберет сам DX
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // так лучше делать всегда
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; // формат Z-буфера
    d3dpp.EnableAutoDepthStencil = TRUE; // нам нужен Z-буфер
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    //d3dpp.PresentationInterval      = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;





    // и теперь создадим сам девайс
    direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device);

    D3DMATERIAL9 material;    // create the material struct

    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse = { 1.0f, 1.0f, 1.0f, 0.1f };    // set diffuse color to white
    material.Ambient = { 1.0f, 1.0f, 1.0f, 0.1f };    // set ambient color to white

    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Position = D3DVECTOR{ 0.5f, 0.0f, 0.5f };
    light.Direction = D3DVECTOR{ 1.0f, 1.0f, 1.0f };

    light.Ambient = D3DCOLORVALUE{ (float)(GetRValue(c) * 0.6f / 255), (float)(GetGValue(c) * 0.6f / 255), (float)(GetBValue(c) * 0.6f / 255), 1 };
    light.Diffuse = D3DCOLORVALUE{ (float)(GetRValue(c) / 255.), (float)(GetGValue(c) / 255.), (float)(GetBValue(c) / 255.), 1 };
    light.Specular = D3DCOLORVALUE{ (float)(GetRValue(c) * 0.6f / 255), (float)(GetGValue(c) * 0.6f / 255), (float)(GetBValue(c) * 0.6f / 255), 1 };

    device->SetLight(0, &light);    // send the light struct properties to light #0
    device->SetMaterial(&material);    // set the globably-used material to &material
    device->LightEnable(0, TRUE);    // turn on light #0

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

struct CUSTOMVERTEX
{
    FLOAT X, Y, Z;
    FLOAT nx, ny, nz;
    D3DCOLOR color;
};

IDirect3DVertexBuffer9* vb;
IDirect3DIndexBuffer9* ib;

CUSTOMVERTEX vertices[] =
{ // куб
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, D3DCOLOR_ARGB(255, 255, 255, 255),//back
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  D3DCOLOR_ARGB(255, 255, 255, 255),
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  D3DCOLOR_ARGB(255, 255, 255, 255),
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  D3DCOLOR_ARGB(255, 255, 255, 255),

    0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 0, 255),//right
    0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 0, 255),
    0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 0, 255),
    0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 0, 255),

    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -0.5f,D3DCOLOR_ARGB(0, 0, 255, 255),//front
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -0.5f, D3DCOLOR_ARGB(0, 0, 255, 255),
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -0.5f, D3DCOLOR_ARGB(0, 0, 255, 255),
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -0.5f, D3DCOLOR_ARGB(0, 0, 255, 255),

    -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, D3DCOLOR_ARGB(0, 255, 0, 255),//left
    -0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, D3DCOLOR_ARGB(0,255, 0, 255),
    -0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,D3DCOLOR_ARGB(0, 255, 0, 255),
    -0.5f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, D3DCOLOR_ARGB(0, 255, 0, 255),

    -0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 255),//top
    -0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 255),
    0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 255),
    0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 255),

    0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, D3DCOLOR_ARGB(0, 0, 255, 255),//bottom
    0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f,  D3DCOLOR_ARGB(0, 0, 255, 255),
    -0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, D3DCOLOR_ARGB(0, 0, 255, 255),
    -0.5f, -0.5f, 0.5f, 0.0f, 0.5f, 0.0f, D3DCOLOR_ARGB(0, 0, 255, 255)


};

// грани составлены из треугольников
const unsigned short Idxes[] = {
    0, 2, 1, 2, 3, 1,    // красный
    4, 6, 5, 6, 7, 5,    // зеленый 
    8, 10, 9, 10, 11, 9, // синий
    12, 14, 13, 14, 15, 13,   //желтый
    16, 18, 17, 18, 19, 17, //голубой
    20, 22, 21, 22, 23, 21, // серый

};

float ViewAngle = 0;

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
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
    case WM_PAINT:
        device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);

        device->CreateVertexBuffer(sizeof(vertices), D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &vb, NULL);
        device->CreateIndexBuffer(sizeof(short) * 36, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL);

        device->BeginScene();
        {
            ViewAngle += 0.02f;
            float cv = cos(ViewAngle);
            float sv = sin(ViewAngle);

            D3DMATRIX World = {
                cv * cv, cv * sv, sv, 0,
                -sv, cv, 0, 0,
                -sv * cv, -sv * sv, cv, 0,
                0, 0, 0, 1,
            };

            device->SetTransform(D3DTS_WORLD, &World);

            void* vb_vertices;

            // набор вершин
            vb->Lock(0, sizeof(vertices), &vb_vertices, 0);
            memcpy(vb_vertices, vertices, sizeof(vertices));
            vb->Unlock();

            // набор граней
            ib->Lock(0, sizeof(Idxes), &vb_vertices, 0);
            memcpy(vb_vertices, Idxes, sizeof(Idxes));
            ib->Unlock();
        }




        device->SetLight(0, &light);
        device->LightEnable(0, true);

        device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE);
        device->SetStreamSource(0, vb, 0, sizeof(CUSTOMVERTEX));
        device->SetIndices(ib);
        device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
        device->SetRenderState(D3DRS_LIGHTING, true);
        device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);//
        device->SetRenderState(D3DRS_ZWRITEENABLE, true);//
        device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);

        device->EndScene();
        device->Present(NULL, NULL, NULL, NULL);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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