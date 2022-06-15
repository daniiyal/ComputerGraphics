// IndividTask1_KG.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "IndividTask1_KG.h"

#define _USE_MATH_DEFINES // for C++
#include <cmath>


#define MAX_LOADSTRING 100

#define COLOR RGB(255, 0, 0)

const int   rect_width = 400, 
            rect_height = 300, 
            RAD = 40;


#define ID_TIMER_1 100

RECT r;
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
    LoadStringW(hInstance, IDC_INDIVIDTASK1KG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INDIVIDTASK1KG));

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


void drawLine(int x1, int y1, int x2, int y2, HDC hdc) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    SetPixel(hdc, x2, y2, RGB(255, 0, 0));
    while (x1 != x2 || y1 != y2)
    {
        SetPixel(hdc, x1, y1, RGB(255, 0, 0));
        int error2 = error * 2;
        if (error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
}


struct pnt {
    POINT p;
    double angle;
};

struct pnt points[7];

struct pnt new_points[7];




void drawRectangle(pnt points[], HDC hdc) {
    
    drawLine(new_points[0].p.x, new_points[0].p.y, new_points[1].p.x, new_points[1].p.y, hdc);
    drawLine(new_points[1].p.x, new_points[1].p.y, new_points[2].p.x, new_points[2].p.y, hdc);
    drawLine(new_points[2].p.x, new_points[2].p.y, new_points[3].p.x, new_points[3].p.y, hdc);
    drawLine(new_points[3].p.x, new_points[3].p.y, new_points[0].p.x, new_points[0].p.y, hdc);
}

void drawCircle(int x0, int y0, int radius, HDC hdc) {
    int x = 0;
    int y = radius;

    new_points[4].p.x = x0;
    new_points[4].p.y = y0;

    int delta = 1 - 2 * radius;
    int error = 0;
    while (y >= 0) {
        SetPixel(hdc, x0 + x, y0 + y, COLOR);
        SetPixel(hdc, x0 + x, y0 - y, COLOR);
        SetPixel(hdc, x0 - x, y0 + y, COLOR);
        SetPixel(hdc, x0 - x, y0 - y, COLOR);
        error = 2 * (delta + y) - 1;
        if (delta < 0 && error <= 0) {
            ++x;
            delta += 2 * x + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;
        if (delta > 0 && error > 0) {
            --y;
            delta += 1 - 2 * y;
            continue;
        }
        ++x;
        delta += 2 * (x - y);
        --y;
    }
}

int count = 1;

void rotate(double x_pivot, double y_pivot, double angle) {

    const double g2r = M_PI / 180.;
    //angle *= g2r;

    for (int i = 0; i < 7; i++)
    {
        double x_shifted = points[i].p.x - x_pivot;
        double y_shifted = points[i].p.y - y_pivot;

        
        new_points[i].p.x = x_pivot + (x_shifted * cos(angle * g2r *count) + y_shifted * sin(angle * g2r * count));
        new_points[i].p.y = y_pivot + (-x_shifted * sin(angle * g2r * count) + y_shifted * cos(angle * g2r * count));
    }
    if (count == 360/angle)
        count = 0;
    else
        count++;
}

void swap(pnt points1, pnt points2) {
    pnt temp = points1;
    points1 = points2;
    points2 = temp;
}

void Reverse() {
    for (int i = 0; i < 7; i++)
    {
        new_points[i].p.x = -new_points[i].p.x;
        points[i].p.x = -points[i].p.x;
    }
}

void drawFigure(HDC hdc) {

    drawRectangle(new_points, hdc);

    drawCircle(new_points[4].p.x, new_points[4].p.y, RAD, hdc);

    drawLine(new_points[5].p.x, new_points[5].p.y, new_points[6].p.x, new_points[6].p.y, hdc);
}


void InitPoints(int x0, int y0) {
    if (points[0].p.x == 0) {
        int x1 = x0 - rect_width / 2;
        int y1 = y0 - rect_height / 2;

        int x2 = x0 + rect_width / 2;
        int y2 = y0 - rect_height / 2;

        int x3 = x0 + rect_width / 2;
        int y3 = y0 + rect_height / 2;

        int x4 = x0 - rect_width / 2;
        int y4 = y0 + rect_height / 2;

        int x5 = x2 - RAD;
        int y5 = y2 + RAD;

        points[0].p.x = x1;
        points[0].p.y = y1;

        points[1].p.x = x2;
        points[1].p.y = y2;

        points[2].p.x = x3;
        points[2].p.y = y3;

        points[3].p.x = x4;
        points[3].p.y = y4;

        points[4].p.x = x5;
        points[4].p.y = y5;

        points[5].p.x = x1;
        points[5].p.y = y0;

        points[6].p.x = x0;
        points[6].p.y = y3;


        for (int i = 0; i < 7; i++)
        {
            new_points[i] = points[i];
        }
    }

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INDIVIDTASK1KG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_INDIVIDTASK1KG);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   //InitPoints(500, 500);
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
    HDC hdc;
    
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
    case WM_LBUTTONDOWN:
        SetTimer(hWnd, ID_TIMER_1, 10, nullptr);
        break;
    case WM_RBUTTONDOWN:
        KillTimer(hWnd, ID_TIMER_1);
        break;

    case WM_TIMER:
        rotate(150, 50, 1);
        InvalidateRect(hWnd, NULL, 1);
        UpdateWindow(hWnd);
        break;
    case WM_MBUTTONDOWN:
        Reverse();
    case WM_PAINT:
        {
            GetClientRect(hWnd, &r);

            InitPoints(r.right / 2, r.bottom / 2);

            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);

            drawFigure(hdc);

            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
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
