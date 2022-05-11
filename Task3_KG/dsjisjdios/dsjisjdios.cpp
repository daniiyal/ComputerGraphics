#include "framework.h"
#include "CubeProject.h"
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <wchar.h>
#include <math.h>
#include "Resource.h"

#define MAX_LOADSTRING 100
#define points_count  8	// точек
#define PI 3.14159265358979323846

double cube_template[points_count][3] = { {-25,-25,-25},{25,-25,-25},{25,25,-25},{-25,25,-25},
             {-25,-25,25}, {25,-25,25}, {25,25,25}, {-25,25,25} };

#define faces  6		// граней
#define fvertex  4		// вершин грани

// нумеруем точки составляющие грань из массива cube_template
int grani[faces][fvertex] = { {0,4,5,1},{0,1,2,3},{0,3,7,4},{5,4,7,6},{1,5,6,2},{2,6,7,3} };
// цвета граней

COLORREF colors[faces] = {
    RGB(0,100,255),RGB(100,255,0),RGB(255,100,0),
    RGB(0,255,255),RGB(255,0,255),RGB(255,255,0)
};

double ctn = cos(atan(2.0)) / 2;
double stn = sin(atan(2.0)) / 2;

POINTS points[8];
//= {
//    {(long)floor(cube_template[0][0] * 6 + cube_template[0][2] * ctn * 6 + r.right / 2),(long)floor(cube_template[0][1] * 6 + cube_template[0][2] * stn * 6 + r.bottom / 2)},
//    {(long)floor(cube_template[1][0] * 6 + cube_template[1][2] * ctn * 6 + r.right / 2),(long)floor(cube_template[1][1] * 6 + cube_template[1][2] * stn * 6 + r.bottom / 2)},
//    {(long)floor(cube_template[2][0] * 6 + cube_template[2][2] * ctn * 6 + r.right / 2),(long)floor(cube_template[2][1] * 6 + cube_template[2][2] * stn * 6 + r.bottom / 2)},
//    {(long)floor(cube_template[3][0] * 6 + cube_template[3][2] * ctn * 6 + r.right / 2),(long)floor(cube_template[3][1] * 6 + cube_template[3][2] * stn * 6 + r.bottom / 2)},
//    {(long)floor(cube_template[4][0] * 6 + cube_template[4][2] * ctn * 6 + r.right / 2),(long)floor(cube_template[4][1] * 6 + cube_template[4][2] * stn * 6 + r.bottom / 2)},
//    {(long)floor(cube_template[5][0] * 6 + cube_template[5][2] * ctn * 6 + r.right / 2),(long)floor(cube_template[5][1] * 6 + cube_template[5][2] * stn * 6 + r.bottom / 2)},
//    {(long)floor(cube_template[6][0] * 6 + cube_template[6][2] * ctn * 6 + r.right / 2),(long)floor(cube_template[6][1] * 6 + cube_template[6][2] * stn * 6 + r.bottom / 2)},
//    {(long)floor(cube_template[7][0] * 6 + cube_template[7][2] * ctn * 6 + r.right / 2),(long)floor(cube_template[7][1] * 6 + cube_template[7][2] * stn * 6 + r.bottom / 2)}
//};
POINTS cube[6][4];

RECT r;
HBRUSH MyBrush;
HPEN MyPen;
WPARAM key = 0;
HBITMAP bmp;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool Visible(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
    double S = (x0 - x1) * (y0 + y1) + (x1 - x2) * (y1 + y2) + (x2 - x3) * (y2 + y3) + (x3 - x0) * (y3 + y0);

    if (S > 0)
        return false;
    else return true;
}
int visible(int num)
{
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        int j = i == 3 ? 0 : i + 1;
        sum += ((int)cube[num][i].x - cube[num][j].x) * ((int)cube[num][i].y + cube[num][j].y);
    }

    return sum > 0 ? 1 : 0;
}

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
    LoadStringW(hInstance, IDC_CUBEPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CUBEPROJECT));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CUBEPROJECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CUBEPROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

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

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

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
    case WM_KEYDOWN: {

        key = wParam;
        switch (key)
        {
        case VK_LEFT:
        {

            GetClientRect(hWnd, &r);

            for (int i = 0; i < 8; i++)
            {

                double old_x1 = cube_template[i][0];
                double old_coordinate_y1 = cube_template[i][1];
                double old_z1 = cube_template[i][2];

                cube_template[i][0] = (old_x1)*cos(1 * PI / 180.) - (old_z1)*sin(1 * PI / 180.);
                cube_template[i][2] = (old_x1)*sin(1 * PI / 180.) + (old_z1)*cos(1 * PI / 180.);

            }

            InvalidateRect(hWnd, NULL, 1);
            UpdateWindow(hWnd);
            break;
        }
        case VK_RIGHT:
        {
            GetClientRect(hWnd, &r);

            for (int i = 0; i < 8; i++)
            {

                double old_x1 = cube_template[i][0];
                double old_coordinate_y1 = cube_template[i][1];
                double old_z1 = cube_template[i][2];

                cube_template[i][0] = (old_x1)*cos(-1 * PI / 180.) - (old_z1)*sin(-1 * PI / 180.);
                cube_template[i][2] = (old_x1)*sin(-1 * PI / 180.) + (old_z1)*cos(-1 * PI / 180.);

            }

            InvalidateRect(hWnd, NULL, 1);
            UpdateWindow(hWnd);
            break;
        }
        case VK_UP:
        {
            GetClientRect(hWnd, &r);

            for (int i = 0; i < 8; i++)
            {

                double old_x1 = cube_template[i][0];
                double old_coordinate_y1 = cube_template[i][1];
                double old_z1 = cube_template[i][2];

                cube_template[i][1] = (old_coordinate_y1)*cos(-1 * PI / 180.) + (old_z1)*sin(-1 * PI / 180.);
                cube_template[i][2] = (-old_coordinate_y1) * sin(-1 * PI / 180.) + (old_z1)*cos(-1 * PI / 180.);

            }

            InvalidateRect(hWnd, NULL, 1);
            UpdateWindow(hWnd);
            break;
        }
        case VK_DOWN:
        {
            GetClientRect(hWnd, &r);

            for (int i = 0; i < 8; i++)
            {

                double old_x1 = cube_template[i][0];
                double old_coordinate_y1 = cube_template[i][1];
                double old_z1 = cube_template[i][2];

                cube_template[i][1] = (old_coordinate_y1)*cos(1 * PI / 180.) + (old_z1)*sin(1 * PI / 180.);
                cube_template[i][2] = (-old_coordinate_y1) * sin(1 * PI / 180.) + (old_z1)*cos(1 * PI / 180.);

            }

            InvalidateRect(hWnd, NULL, 1);
            UpdateWindow(hWnd);
            break;
        }
        }
    }
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
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        GetClientRect(hWnd, &r);

        for (int i = 0; i < 8; i++)
        {
            // x' = x + z*(1/2)*cos(atan(2)); y' = y + z*(1/2)*sin(atan(2));
            points[i].x = (long)floor(cube_template[i][0] * 6 + cube_template[i][2] * ctn * 6 + r.right / 2);
            points[i].y = (long)floor(cube_template[i][1] * 6 + cube_template[i][2] * stn * 6 + r.bottom / 2);
        }

        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 4; j++)
                cube[i][j] = points[grani[i][j]];

        bmp = (HBITMAP)LoadImage(hInst, L"bitmap.bmp", IMAGE_BITMAP, r.right, r.bottom, LR_DEFAULTSIZE | LR_LOADFROMFILE);

        HDC hdcBuf = CreateCompatibleDC(hdc);

        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcBuf, bmp);

        MoveToEx(hdcBuf, cube[0][0].x, cube[0][0].y, NULL);
        //MyPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));

        for (int i = 0; i < 6; i++)
        {

            //if (Visible(cube[i][0].x, cube[i][0].y, cube[i][1].x, cube[i][1].y, cube[i][2].x, cube[i][2].y, cube[i][3].x, cube[i][3].y))
            if (!visible(i))
            {
                for (int j = 0; j < 4; j++)
                {
                    LineTo(hdcBuf, cube[i][j].x, cube[i][j].y);
                }
                LineTo(hdcBuf, cube[i][0].x, cube[i][0].y);
            }
            //PolyPolyline(hdc, (const POINT*)cube[i], 4);

        }

        BitBlt(hdc, 0, 0, r.right, r.bottom, hdcBuf, 0, 0, SRCCOPY);

        SelectObject(hdcBuf, hOldBmp);
        DeleteDC(hdcBuf);
        DeleteObject(bmp);


        EndPaint(hWnd, &ps);
    }
    break;
    case WM_ERASEBKGND:
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
