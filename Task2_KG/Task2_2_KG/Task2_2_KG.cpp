// Task2_2_KG.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Task2_2_KG.h"

#define MAX_LOADSTRING 100


#include <ctime>
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <stack>
using namespace std;

#define NUMP 100


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
    LoadStringW(hInstance, IDC_TASK22KG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK22KG));

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

struct pnt {
    POINT p;
    double angle;
};

struct pnt points[NUMP];


void generate_points(RECT rect) {

    srand((unsigned int)time(NULL));


    for (int i = 0; i < NUMP; i++)
    {
        points[i].p.x = rand() % (rect.right - 20) + 10;
        points[i].p.y = rand() % (rect.bottom - 20) + 10;

    }
};

double polar(int x1, int y1, int x2, int y2)
{
    double x = x2 - x1;
    double y = y2 - y1;

    if (x == 0)
    {
        if (y == 0) return 0;
        if (y > 0)
            return M_PI / 2.0;
        else
            return 3 * M_PI / 2.0;
    }

    if (x < 0) return atan(y / x) + M_PI;

    if (x > 0)
    {
        double angle = atan(y / x);
        if (y >= 0)
            return angle;
        else
            return angle + 2.0 * M_PI;
    }

    return -1;
}

void find_min() {
    int select = 0, miny = points[0].p.y;

    for (int i = 1; i < NUMP; i++)
    {
        if (points[i].p.y < miny)
        {
            select = i;
            miny = points[i].p.y;
        }
    }

    pnt p = points[select];
    points[select] = points[0];
    points[0] = p;
}

int comp(const void* A, const void* B)
{
    return  (*(struct pnt*)A).angle > (*(struct pnt*)B).angle ? 1 : ((*(struct pnt*)A).angle == (*(struct pnt*)B).angle) ? 0 : -1;
}


void sort_points() {
    
    points[0].angle = 0;
    for (int i = 1; i < NUMP; i++)
    {
        points[i].angle = polar(points[0].p.x, points[0].p.y, points[i].p.x, points[i].p.y);
    }
    qsort(points, NUMP, sizeof(struct pnt), comp);
}


int orientation(pnt p, pnt q, pnt r)
{
    int val = (q.p.x - p.p.x) * (r.p.y - q.p.y) - (q.p.y - p.p.y) * (r.p.x - q.p.x);

    return val;
}

pnt nextToTop(stack<pnt>& S)
{
    pnt p = S.top();
    S.pop();
    pnt res = S.top();
    S.push(p);
    return res;
}

stack<pnt> S;



void correct_polygon() 
{
    while (!S.empty()) {     
        S.pop();
    }
    
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);


    for (int i = 3; i < NUMP; i++)
    {
        // Keep removing top while the angle formed by points next-to-top,
        // top, and points[i] makes a non-left turn
        while (orientation(nextToTop(S), S.top(), points[i]) < 0)

            S.pop();
        S.push(points[i]);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK22KG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK22KG);
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);


            RECT r;
            GetClientRect(hWnd, &r);

            generate_points(r);
            find_min();

            sort_points();
            correct_polygon();

            for (int i = 0; i < NUMP; i++)
            {
                Ellipse(hdc, points[i].p.x-3, points[i].p.y-3, points[i].p.x + 3, points[i].p.y + 3);
            }

            pnt p = S.top();
            MoveToEx(hdc, p.p.x, p.p.y, NULL);

            while (!S.empty())
            {
                pnt next = S.top();
                LineTo(hdc, next.p.x, next.p.y);
                S.pop();
            }

            LineTo(hdc, p.p.x, p.p.y);

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
