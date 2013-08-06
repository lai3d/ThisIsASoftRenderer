// ThisIsASoftRenderer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../../Resource.h"
#include "Renderer.h"
#include "OgreMeshLoader.h"
#include "ObjMeshLoader.h"
#include "Utility.h"

#define MAX_LOADSTRING 100
const int		SCREEN_WIDTH	=	800;
const int		SCREEN_HEIGHT	=	600;

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

ULONG_PTR			g_gdiplusToken;
SR::Renderer		g_renderer;			
Ext::OgreMeshLoader	g_meshLoader;		
Ext::ObjMeshLoader	g_objLoader;
SGlobalEnv			g_env;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_THISISASOFTRENDERER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THISISASOFTRENDERER));

	// ����Ϣѭ��:
	while (1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else
		{
			// Render a frame during idle time (no messages are waiting)

			//////////////////////////////////////////////////////////
			/////////////// main game processing goes here
			g_renderer.OnFrameMove();
			g_renderer.RenderOneFrame();

			//��ʾһЩ������Ϣ
			{
				const VEC4& pos = g_renderer.m_camera.GetPos();
				char szText[128];
				sprintf_s(szText, ARRAYSIZE(szText), "lastFPS : %d, CamPos : (%f, %f, %f), Current Shade Mode : %s", 
					g_renderer.m_frameStatics.lastFPS, pos.x, pos.y, pos.z, g_renderer.GetCurShadingModeName());

				SR::RenderUtil::DrawText(10, 10, szText, RGB(0,255,0));
			}

			{
				char szText[128];
				sprintf_s(szText, ARRAYSIZE(szText), "RenderedTris : %d, Culled Object : %d, Backface : %d, Culled Face : %d", 
					g_renderer.m_frameStatics.nRenderedFace, g_renderer.m_frameStatics.nObjCulled, g_renderer.m_frameStatics.nBackFace, g_renderer.m_frameStatics.nFaceCulled);

				SR::RenderUtil::DrawText(10, 35, szText, RGB(0,255,0));
			}

			{
				const float speed = g_renderer.m_camera.GetMoveSpeed();
				char szText[128];
				sprintf_s(szText, ARRAYSIZE(szText), 
					"Camera Speed: %f . Press \"+/-\" to increase/decrease camera speed. Press R to toggle shade mode!", speed);

				SR::RenderUtil::DrawText(10, 60, szText, RGB(0,255,0));
			}

			//swap!!
			g_renderer.Present();
		}
	}

	//GdiplusShutdown(g_gdiplusToken);

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THISISASOFTRENDERER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(GetStockObject(BLACK_BRUSH));
	wcex.lpszMenuName	= /*MAKEINTRESOURCE(IDC_THISISASOFTRENDERER)*/nullptr;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance;

   //����������Ⱦ���ڴ�С
   RECT rcClient;
   rcClient.top = 0;
   rcClient.left = 0;
   rcClient.right = SCREEN_WIDTH;
   rcClient.bottom = SCREEN_HEIGHT;

   DWORD style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

   AdjustWindowRect(&rcClient, style, FALSE);

   const int realWidth = rcClient.right - rcClient.left;
   const int realHeight = rcClient.bottom - rcClient.top;

   int windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
   int windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;
 
   hWnd = CreateWindow(szWindowClass, szTitle, style,
      windowLeft, windowTop, realWidth, realHeight, NULL, NULL, hInstance, NULL);

   if (!hWnd)
      return FALSE;

   ShowWindow(hWnd, SW_SHOWNORMAL);
   UpdateWindow(hWnd);

   /////////////////////////////////////////////////////////
   /////////////// Init here
   Gdiplus::GdiplusStartupInput gdiplusStartupInput;
   GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

   //init globals
   g_env.hwnd = hWnd;
   g_env.renderer = &g_renderer;
   g_env.meshLoader = &g_meshLoader;
   g_env.objLoader = &g_objLoader;

   //init renderer
   g_env.renderer->Init();

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
#include <WinUser.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
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
		break;

	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_ERASEBKGND:
		return 0;

	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ADD:
				{
					g_env.renderer->m_camera.AddMoveSpeed(1.0f);
					return 0;
				}
				break;
			case VK_SUBTRACT:
				{
					g_env.renderer->m_camera.AddMoveSpeed(-0.2f);
					return 0;
				}
				break;
			case VK_ESCAPE:
				{
					DestroyWindow(hWnd);	
					return 0;
				}
			default: return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

	case WM_CHAR:
		{
			if(wParam == 'r')
			{
				g_env.renderer->ToggleShadingMode();
				return 0;
			}
			else if(wParam == 't')
			{
				g_env.renderer->ToggleScene();
				return 0;
			}
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

// �����ڡ������Ϣ�������
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
