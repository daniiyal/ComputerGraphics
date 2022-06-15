// Task3_1_KG.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Task3_1_KG.h"

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <wchar.h>

#define MAX_LOADSTRING 100

#define _USE_MATH_DEFINES // for C++
#include <cmath>


#define points_count  8	// точек
#define faces  6		// граней
#define fvertex  4		// вершин грани
// нумеруем точки составляющие грань из массива cube_template
WPARAM key = 0;
HBITMAP bmp;
RECT r;


double cube_template[points_count][3] = {   {-25,-25,-25},{25,-25,-25},{25,25,-25},{-25,25,-25},
                                            {-25,-25,25}, {25,-25,25}, {25,25,25}, {-25,25,25} };  

int grani[faces][fvertex] = { {0,4,5,1},{0,1,2,3},{0,3,7,4},{5,4,7,6},{1,5,6,2},{2,6,7,3} };

int colors[faces][3] = {
    {0,100,255},{100,255,0},{255,100,0},
    {0,255,255},{255,0,255},{255,255,0}
};

POINT points[points_count];

POINT cube[faces][fvertex];



double ctn = cos(atan(2.0)) / 2;
double stn = sin(atan(2.0)) / 2;


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

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
    LoadStringW(hInstance, IDC_TASK31KG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK31KG));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK31KG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK31KG);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
                double old_y1 = cube_template[i][1];
                double old_z1 = cube_template[i][2];

                cube_template[i][0] = (old_x1)*cos(1 * M_PI / 180.) - (old_z1)*sin(1 * M_PI / 180.);
                cube_template[i][2] = (old_x1)*sin(1 * M_PI / 180.) + (old_z1)*cos(1 * M_PI / 180.);

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
                double old_y1 = cube_template[i][1];
                double old_z1 = cube_template[i][2];

                cube_template[i][0] = (old_x1)*cos(-1 * M_PI / 180.) - (old_z1)*sin(-1 * M_PI / 180.);
                cube_template[i][2] = (old_x1)*sin(-1 * M_PI / 180.) + (old_z1)*cos(-1 * M_PI / 180.);

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
                double old_y1 = cube_template[i][1];
                double old_z1 = cube_template[i][2];

                cube_template[i][1] = (old_y1)*cos(-1 * M_PI / 180.) + (old_z1)*sin(-1 * M_PI / 180.);
                cube_template[i][2] = (-old_y1) * sin(-1 * M_PI / 180.) + (old_z1)*cos(-1 * M_PI / 180.);

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
                double old_y1 = cube_template[i][1];
                double old_z1 = cube_template[i][2];

                cube_template[i][1] = (old_y1)*cos(1 * M_PI / 180.) + (old_z1)*sin(1 * M_PI / 180.);
                cube_template[i][2] = (-old_y1) * sin(1 * M_PI / 180.) + (old_z1)*cos(1 * M_PI / 180.);

            }

            InvalidateRect(hWnd, NULL, 1);
            UpdateWindow(hWnd);
            break;
        }
        }
    }
    case WM_ERASEBKGND:
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
     
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

            GetClientRect(hWnd, &r);

            bmp = (HBITMAP)LoadImage(hInst, L"bitmap.bmp", IMAGE_BITMAP, r.right, r.bottom, LR_DEFAULTSIZE | LR_LOADFROMFILE);

            HDC hdcBuf = CreateCompatibleDC(hdc);
            HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcBuf, bmp);

            for (int i = 0; i < points_count; i++)
            {
                    points[i].x = (long)floor(cube_template[i][0] * 6 + cube_template[i][2] * ctn * 6 + r.right / 2);
                    points[i].y = (long)floor(cube_template[i][1] * 6 + cube_template[i][2] * stn * 6 + r.bottom / 2);
             
            }


            for (int i = 0; i < faces; i++)
                for (int j = 0; j < fvertex; j++)
                    cube[i][j] = points[grani[i][j]];


            MoveToEx(hdcBuf, cube[0][0].x, cube[0][0].y, NULL);


            SelectObject(hdcBuf, GetStockObject(DC_PEN));
            SelectObject(hdcBuf, GetStockObject(DC_BRUSH));


            for (int i = 0; i < 6; i++)
            {
                if (!visible(i))
                {
                    SetDCBrushColor(hdcBuf, RGB(colors[i][0], colors[i][1], colors[i][2]));
                    Polygon(hdcBuf, cube[i], 4);
                }
            }

            BitBlt(hdc, 0, 0, r.right, r.bottom, hdcBuf, 0, 0, SRCCOPY);
            SelectObject(hdcBuf, hOldBmp);

            DeleteDC(hdcBuf);
            DeleteObject(bmp);

            EndPaint(hWnd, &ps);
        }
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
