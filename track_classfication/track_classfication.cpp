//track_classfication.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "track_classfication.h"

#include <stdio.h>
#include <string>
#include <time.h>
#include <math.h>
#include <commdlg.h>
#include <CommCtrl.h>
#include "avg_dis_k_m.h"

#define MAX_LOADSTRING 100
#define KIND 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名


// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int COLOR[] = 
{
	0xff0000,
	0xff00ff,
	0xffff00,
	0x00ff00,
	0x00ffff,
	0x0000ff,
	0xf0f00f,
	0x0f00f,
	0x0ff0f,
};

#define DATA_PATH "TrackData\\TrackData-3.txt"

int g_classfication[GROUPCNT][GROUPCNT];
double g_alldis[GROUPCNT][GROUPCNT];
int g_cen[GROUPCNT];
PosData g_point;
HINSTANCE g_hInst;
HDC g_hdc;
HWND g_hWnd;
HWND g_hTxt;

int g_kind;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	g_hInst = hInstance;
    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRACKCLASSFICATION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRACKCLASSFICATION));

    MSG msg;

    // 主消息循环: 
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRACKCLASSFICATION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TRACKCLASSFICATION);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int kind = 5;
    switch (message)
    {
	case WM_CREATE:
		g_hWnd = hWnd;
		g_hdc = GetDC(g_hWnd);
		memset(g_classfication, 0x00, sizeof(g_classfication));
		memset(g_alldis, 0x00, sizeof(g_alldis));
		memset(g_cen, 0x00, sizeof(g_cen));
		ReadData(g_point ,DATA_PATH);
		TryBestClass(g_point, g_cen, g_alldis, g_classfication, kind);

		g_hTxt = CreateWindowEx(NULL, WC_EDIT, "5",
			WS_CHILD | WS_VISIBLE | WS_BORDER
			, 800, 0, 50, 20, g_hWnd, (HMENU)KIND, g_hInst, 0);
		CreateWindowEx(NULL, WC_BUTTON, "重新分类",
			WS_CHILD | WS_VISIBLE | WS_BORDER
			, 860, 0, 80, 25, g_hWnd, (HMENU)(KIND + 1), g_hInst, 0);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
			case KIND + 1:
			case IDM_ABOUT:
			{
				char buf[10];
				GetWindowText(g_hTxt, buf, 10);
				kind = atoi(buf);
				if (kind < 1 || kind >= g_point.groupcnt)
				{
					MessageBox(hWnd,"分类数量错误,不能超过线条数量或小于1！","error!",1);
					break;
				}
				TryBestClass(g_point, g_cen, g_alldis, g_classfication, kind);
				InvalidateRect(hWnd,NULL,TRUE);
			}break;
			break;
			case ID_32772:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case ID_32773:
			{
				char szBuffer[MAX_PATH] = { 0 };
				char crtpath[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, crtpath);
				OPENFILENAME ofn = { 0 };
				ofn.lStructSize = sizeof(ofn);
				ofn.lpstrFilter = "*.*\0*.*\0\0";//要选择的文件后缀   
				ofn.lpstrInitialDir = crtpath;//默认的文件路径   
				ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
				ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
				ofn.nFilterIndex = 0;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
				BOOL bSel = GetOpenFileName(&ofn);
				if (bSel)
				{
					ReadData(g_point, szBuffer);
					TryBestClass(g_point, g_cen, g_alldis, g_classfication, kind);
					RECT rect;
					GetClientRect(hWnd,&rect);
					InvalidateRect(hWnd, &rect, TRUE);
				}
			}
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
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			char buf[128] = { 0 };
			sprintf(buf, "当前分类数量:", kind);
			TextOut(hdc, 700, 2, buf, strlen(buf));
			sprintf(buf, "tips:同色表示同一类");
			TextOut(hdc, 700, 30, buf, strlen(buf));
			for (size_t i = 0; i < kind; i++)
			{
				SelectObject(hdc, CreatePen(PS_SOLID, 2, COLOR[i%9]));
				for (size_t j = 0; j < g_point.groupcnt; j++)
				{
					if (g_classfication[g_cen[i]][j])
					{
						MoveToEx(hdc, g_point.pt[j][0].x, g_point.pt[j][0].y, NULL);
						buf[0] = '0' + j + 1;
						TextOut(hdc, g_point.pt[j][0].x, g_point.pt[j][0].y, buf, 1);
						for (size_t n = 1; n < g_point.ptcnt[j]; n++)
						{
							LineTo(hdc, g_point.pt[j][n].x, g_point.pt[j][n].y);
						}
					}

				}
			}
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

// “关于”框的消息处理程序。
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

