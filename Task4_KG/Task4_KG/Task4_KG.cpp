// Task4_KG.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Task4_KG.h"


#define _USE_MATH_DEFINES // for C++
#include <cmath>


#include <algorithm>
#include <vector>
#define MAX_LOADSTRING 100

using namespace std;

#define points_count  8	// точек
#define faces  6		// граней
#define fvertex  4		// вершин грани
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна


WPARAM key = 0;

struct CVector3
{
    public: float x, y, z;
public: float intencity;
};

struct pnt {
public:
    float x, y;
    float intencity;
};


double amp = 0.9; // яркость источника
double ambient = 0.5; // рассеянный свет
CVector3 light = { -0.5, -0.5, -0.5 }; // координаты источника

float K = 50;


DWORD* pixels;

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
    LoadStringW(hInstance, IDC_TASK4KG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK4KG));

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


float cube_template[points_count][3] = { {-25,-25,-25},{25,-25,-25},{25,25,-25},{-25,25,-25},
                                            {-25,-25,25}, {25,-25,25}, {25,25,25}, {-25,25,25} };

int grani[faces][fvertex] = { {0,4,5,1},{0,1,2,3},{0,3,7,4},{5,4,7,6},{1,5,6,2},{2,6,7,3} };

int colors[faces][3] = {
    {0,100,255},{100,255,0},{255,100,0},
    {0,255,255},{255,0,255},{255,255,0}
};

POINT points[points_count];

POINT cube[faces][fvertex];


HBITMAP bmp;
RECT r;


void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {

    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);

    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;



    SetPixel(hdc, x2, y2, color);
    while (x1 != x2 || y1 != y2)
    {
        SetPixel(hdc, x1, y1, color);
         
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




vector <pnt> vec_pnts;

void drawLine(HDC hdc, int x1, int y1, int x2, int y2, float intencity1, float intencity2) {

    //int x1 = Face1.x;
    //int x2 = Face2.x;
    //int y1 = Face1.y;
    //int y2 = Face2.y;



    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    float grad = 0;
    if (deltaX != 0) {
        float grad = (intencity2 - intencity1) / (x2 - x1);
    }
    else {
        float grad = (intencity2 - intencity1) / (y2 - y1);
    }
    

    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    float start_intencity = intencity1;

    pnt point;

    SetPixel(hdc, x2, y2, RGB(0, 0, 0));
    while (x1 != x2 || y1 != y2)
    {
        SetPixel(hdc, x1, y1, RGB(0, 0, 0));

        point.x = x1;
        point.y = y1;
        point.intencity = start_intencity + grad;
        start_intencity += grad;

        vec_pnts.push_back(point);

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
    point.x = x2;
    point.y = y2;
    point.intencity = start_intencity + grad;
    start_intencity += grad;
    vec_pnts.push_back(point);
}


bool comp(pnt a, pnt  b) {
    return a.y < b.y;
}


void drawHorizontalLine(HDC hdc, int color_init[3]) {

    sort(vec_pnts.begin(), vec_pnts.end(), comp);

    for (int i = 0; i < vec_pnts.size() - 2; i+=2)
    {
        if (vec_pnts[i].y == vec_pnts[i + 1].y) {
            int color[3];

            for (int k = 0; k < 3; k++)
            {
                color[k] = color_init[k] * vec_pnts[i].intencity;
                if (color[k] < 0)
                    color[k] = 0;
                if (color[k] > 255)
                    color[k] = 255;
            }
            drawLine(hdc, vec_pnts[i].x, vec_pnts[i].y, vec_pnts[i + 1].x, vec_pnts[i + 1].y, RGB(color[0], color[1], color[2]));
        }

    }

   /* for (int i = 0; i < vec_pnts.size()-430; i++)
    {
        if (vec_pnts[i].y == vec_pnts[i+430].y) {

            int color[3];

            for (int k = 0; k < 3; k++)
            {
                color[k] = color_init[k] * vec_pnts[i].intencity;
                if (color[k] < 0)
                    color[k] = 0;
                if (color[k] > 255)
                    color[k] = 255;
            }
            drawLine(hdc, vec_pnts[i].x, vec_pnts[i].y, vec_pnts[i + 430].x, vec_pnts[i + 430].y, RGB(color[0], color[1], color[2]));
        }
    }*/
}





CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
    CVector3 vVector = { 0 };


    vVector.x = vPoint1.x - vPoint2.x;
    vVector.y = vPoint1.y - vPoint2.y;
    vVector.z = vPoint1.z - vPoint2.z;

    return vVector;
};

CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
    CVector3 vNormal;


    vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));

    vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));

    vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

    return vNormal;  
};


float Magnitude(CVector3 vNormal)
{
    return (float)sqrt((vNormal.x * vNormal.x) +
        (vNormal.y * vNormal.y) +
        (vNormal.z * vNormal.z));
}


CVector3 Normalize(CVector3 vNormal)
{
    float magnitude = Magnitude(vNormal);

    vNormal.x /= magnitude;
    vNormal.y /= magnitude;
    vNormal.z /= magnitude;

    return vNormal;
}

CVector3 Normal(CVector3 vTriangle[])
{
    CVector3 vVector1 = Vector(vTriangle[2], vTriangle[0]);
    CVector3 vVector2 = Vector(vTriangle[1], vTriangle[0]);

    CVector3 vNormal = Cross(vVector1, vVector2);

    vNormal = Normalize(vNormal);


    return vNormal;
};



CVector3 vertexes[8];
CVector3 FaceVertexes[4];


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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK4KG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK4KG);
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

            //for (int i = 0; i < points_count; i++)
            //{
            //        points[i].x = (long)floor(cube_template[i][0] * 6 + cube_template[i][2] * ctn * 6 + r.right / 2);
            //        points[i].y = (long)floor(cube_template[i][1] * 6 + cube_template[i][2] * stn * 6 + r.bottom / 2);
            // 
            //}

            for (int i = 0; i < points_count; i++)
            {
                points[i].x = (long)floor(cube_template[i][0] * 6 / (1 - (cube_template[i][2] * 6) / 500) + r.right / 2);
                points[i].y = (long)floor(cube_template[i][1] * 6 / (1 - (cube_template[i][2] * 6) / 500) + r.bottom / 2);

            }


            for (int i = 0; i < faces; i++)
                for (int j = 0; j < fvertex; j++)
                    cube[i][j] = points[grani[i][j]];

           

            MoveToEx(hdcBuf, cube[0][0].x, cube[0][0].y, NULL);


            SelectObject(hdcBuf, GetStockObject(DC_PEN));
            SelectObject(hdcBuf, GetStockObject(DC_BRUSH));

            for (int i = 0; i < 8; i++)
            {
                vertexes[i] = { cube_template[i][0] ,cube_template[i][1],  cube_template[i][2] };
            }
            int grani[faces][fvertex] = { {0,4,5,1},{0,1,2,3},{0,3,7,4},{5,4,7,6},{1,5,6,2},{2,6,7,3} };

            for (int i = 0; i < 6; i++)
            {
                float intencity[4] = { 0, 0, 0, 0 };
                CVector3 normal = { 0 };
                FaceVertexes[0] = vertexes[grani[i][0]];
                FaceVertexes[1] = vertexes[grani[i][1]];
                FaceVertexes[2] = vertexes[grani[i][2]];
                FaceVertexes[3] = vertexes[grani[i][3]];

                normal = Normal(FaceVertexes);

                CVector3 vector_l = Vector(vertexes[i], light);

                float d = sqrt((normal.x - light.x)* (normal.x - light.x) + (normal.y - light.y)  * (normal.y - light.y) + (normal.z - light.z) * (normal.z - light.z));
                
                float sum_intencity = 0;

                //for (int i = 0; i < 4; i++)
                //{
                //    float cos = light.x* FaceVertexes[i].x + light.y * FaceVertexes[i].y + light.z * FaceVertexes[i].z;
                //    FaceVertexes[i].intencity = ambient + amp * cos / (d + K);
                //    sum_intencity += FaceVertexes[i].intencity;
                //}
                
                for (int j = 0; j < 4; j++)
                {
                    float cos = light.x * FaceVertexes[j].x + light.y * FaceVertexes[j].y + light.z * FaceVertexes[j].z;
                    FaceVertexes[j].intencity = ambient + amp * cos / (d + K);
                }

                if (!visible(i))
                {
                    //SetDCBrushColor(hdcBuf, RGB(color[0], color[1], color[2]));
                    vec_pnts.clear();

                    drawLine(hdcBuf, cube[i][0].x, cube[i][0].y, cube[i][1].x, cube[i][1].y, FaceVertexes[0].intencity, FaceVertexes[1].intencity);
                    drawLine(hdcBuf, cube[i][1].x, cube[i][1].y, cube[i][2].x, cube[i][2].y, FaceVertexes[1].intencity, FaceVertexes[2].intencity);
                    drawLine(hdcBuf, cube[i][2].x, cube[i][2].y, cube[i][3].x, cube[i][3].y, FaceVertexes[2].intencity, FaceVertexes[3].intencity);
                    drawLine(hdcBuf, cube[i][3].x, cube[i][3].y, cube[i][0].x, cube[i][0].y, FaceVertexes[3].intencity, FaceVertexes[0].intencity);
                    drawHorizontalLine(hdcBuf, colors[i]);
                    
                    //Polygon(hdcBuf, cube[i], 4);
                }
            }

            //for (int i = 0; i < 6; i++)
            //{
            //    if (!visible(i))
            //    {
            //        SetDCBrushColor(hdcBuf, RGB(colors[i][0], colors[i][1], colors[i][2]));
            //        Polygon(hdcBuf, cube[i], 4);
            //    }
            //}

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
