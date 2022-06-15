// Task10_11KG.cpp : Определяет точку входа для приложения.
//
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include "framework.h"
#include "Task10_11KG.h"
#include "math.h"
#include "SOIL.h"

#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")


#define MAX_LOADSTRING 100

void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
void resize(int width, int height);

float a = 2;

struct CVector3
{
    public: float x, y, z;
};


CVector3  v[4] = {
    {0, 0, a * sqrt(6) / 4},
    {a / sqrt(3), 0, -a * sqrt(6) / 12},
    {-a / sqrt(12), a / 2, -a * sqrt(6) / 12},
    {-a / sqrt(12), -a / 2, -a * sqrt(6) / 12}
};


CVector3 Cross(CVector3 vVector1, CVector3 vVector2);

// Возвращает вектор между 2мя точками.
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);

// Возвращает величину нормали или любого другого вектора
float Magnitude(CVector3 vNormal);

// Возвращает нормализованный вектор (его длинна становится равной 1)
CVector3 Normalize(CVector3 vNormal);

// Возвращает нормаль полигона (направление, куда повернут полигон)
CVector3 Normal(CVector3 vTriangle[]);


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
    LoadStringW(hInstance, IDC_TASK1011KG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASK1011KG));

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

GLfloat theta = 0;

void resize(int width, int height)
{
    glViewport(0, 0, width, height);		// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);		// Select The Projection Matrix
    glLoadIdentity();				// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);			// Select The Modelview Matrix
    glLoadIdentity();				// Reset The Modelview Matrix
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASK1011KG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASK1011KG);
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

HGLRC hRC;
HDC hdc;


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   EnableOpenGL(hWnd, &hdc, &hRC);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

CVector3 side1[3] = { v[0], v[1], v[2] };
CVector3 normal1 = Normal(side1);

CVector3 side2[3] = { v[0], v[3], v[1] };
CVector3 normal2 = Normal(side2);

CVector3 side3[3] = { v[0], v[2], v[3] };
CVector3 normal3 = Normal(side3);

CVector3 side4[3] = { v[1], v[3], v[2] };
CVector3 normal4 = Normal(side4);

GLuint texture[1];

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
    case WM_SIZE:
        resize(LOWORD(lParam), HIWORD(lParam));
        break;
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();

            glTranslatef(0.0f, 0.0f, -3.0f);
            glRotatef(theta, 0.8f, 0.5f, 0.2f);

            glBindTexture(GL_TEXTURE_2D, texture[0]);

            glBegin(GL_TRIANGLES);

            //glColor3f(1.0f, 0.0f, 0.0f);
            glNormal3f(normal1.x, normal1.y, normal1.z);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(v[0].x, v[0].y, v[0].z);
            glTexCoord2f(1.0f, 0.0f);glVertex3f(v[1].x, v[1].y, v[1].z);
            glTexCoord2f(0.5f, 1.0f);glVertex3f(v[2].x, v[2].y, v[2].z);


            //glColor3f(0, 0, 1);
            glNormal3f(normal2.x, normal2.y, normal2.z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(v[0].x, v[0].y, v[0].z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(v[3].x, v[3].y, v[3].z);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(v[1].x, v[1].y, v[1].z);

            //glColor3f(1, 0, 1);
            glNormal3f(normal3.x, normal3.y, normal3.z);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(v[0].x, v[0].y, v[0].z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(v[2].x, v[2].y, v[2].z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(v[3].x, v[3].y, v[3].z);

            //glColor3f(1, 1, 0);
            glNormal3f(normal4.x, normal4.y, normal4.z);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(v[1].x, v[1].y, v[1].z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(v[3].x, v[3].y, v[3].z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(v[2].x, v[2].y, v[2].z);

            glEnd();


            theta += 1.0f;
            SwapBuffers(hdc);
            Sleep(1);
            break;

        }
        break;
    case WM_DESTROY:
        DisableOpenGL(hWnd, hdc, hRC);
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

void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    // get the device context (DC)
    *hDC = GetDC(hWnd);
    RECT rect;

    // set the pixel format for the DC
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    format = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, format, &pfd);

    // create and enable the render context (RC)
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);


    texture[0] = SOIL_load_OGL_texture
    (
        "texture.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );
    if (!texture[0])
    {
        MessageBox(hWnd, L"soil failed to load texture", L"Error", MB_OK);
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFrontFace(GL_CW);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);//NICEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    {
        GLfloat ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
        GLfloat light0[4] = { -0.5f, 0.2f, -1.0f, 0.01f };
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_POSITION, light0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, ambient);
    }
    glEnable(GL_COLOR_MATERIAL);

    GetClientRect(hWnd, &rect);
    resize(rect.right, rect.bottom);
}

// Disable OpenGL
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}


CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
    CVector3 vNormal;              

    vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));

    vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));

    vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

    return vNormal;  
}



CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
    CVector3 vVector = { 0 };



    vVector.x = vPoint1.x - vPoint2.x;
    vVector.y = vPoint1.y - vPoint2.y;
    vVector.z = vPoint1.z - vPoint2.z;

    // Теперь возвращаем полученный результат

    return vVector;
}


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
}