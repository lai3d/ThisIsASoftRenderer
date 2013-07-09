// ThisIsASoftRenderer.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../../Resource.h"
#include "Renderer.h"
#include "OgreMeshLoader.h"

#define MAX_LOADSTRING 100
const int		SCREEN_WIDTH	=	800;
const int		SCREEN_HEIGHT	=	600;

inline STRING	GetResPath(const STRING filename)
{
	STRING filepath("../../../Res/");
	filepath += filename;
	return std::move(filepath);
}

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

SR::Renderer		g_renderer;					// ����Ⱦ��
Ext::OgreMeshLoader	g_meshLoader;				// mesh������

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
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			g_renderer.RenderOneFrame();

			//��ʾһЩ������Ϣ
			{
				const VEC4& pos = g_renderer.m_camera.GetPos();
				char szText[256];
				sprintf_s(szText, ARRAYSIZE(szText), "CamPos : (%f, %f, %f)", pos.x, pos.y, pos.z);

				auto& renderList = g_renderer.GetRenderList();
				size_t nCulled = std::count_if(renderList.begin(), renderList.end(), [&](const SR::SRenderObj& obj){ return obj.m_bCull; });
				char szText2[128];
				sprintf_s(szText2, ARRAYSIZE(szText2), "      Culled Object : %d.", nCulled);
				strcat_s(szText, sizeof(szText), szText2);

				SR::RenderUtil::DrawText(10, 5, szText, RGB(0,255,0));
			}

			{
				const float speed = g_renderer.m_camera.GetMoveSpeed();
				char szText[256];
				sprintf_s(szText, ARRAYSIZE(szText), 
					"Camera Speed: %f . Press \"+/-\" to increase/decrease camera speed.", speed);

				SR::RenderUtil::DrawText(10, 25, szText, RGB(0,255,0));
			}
		}
	}

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

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   DWORD style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP;
   hWnd = CreateWindow(szWindowClass, szTitle, style,
      CW_USEDEFAULT, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   /////////////////////////////////////////////////////////
   /////////////// Init here
   g_renderer.m_hwnd = hWnd;
   g_renderer.SetRasterizeType(SR::eRasterizeType_FlatWire);

   try
   {
	   g_meshLoader.LoadMeshFile(GetResPath("marine.mesh.xml"));
   }
   catch (std::exception& e)
   {
   		MessageBoxA(hWnd, e.what(), "Error", MB_ICONERROR);
   }

   //�����õ�ͼԪ����renderer
   g_renderer.AddRenderable(g_meshLoader.m_obj);

   g_renderer.m_camera.SetPosition(VEC3(0,0,10));

   RECT rcClient;
   GetClientRect(hWnd, &rcClient);
   int w = rcClient.right - rcClient.left;
   int h = rcClient.bottom - rcClient.top;
   SetCursorPos(rcClient.left+w/2, rcClient.top+h/2);
   ShowWindow(hWnd, SW_SHOWNORMAL);
   UpdateWindow(hWnd);

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
			if(wParam == VK_ADD)
			{
				g_renderer.m_camera.AddMoveSpeed(1.0f);
				return 0;
			}
			else if(wParam == VK_SUBTRACT)
			{
				g_renderer.m_camera.AddMoveSpeed(-0.2f);
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
