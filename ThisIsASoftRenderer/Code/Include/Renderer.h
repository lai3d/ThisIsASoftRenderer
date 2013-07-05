/********************************************************************
	created:	30:6:2013   23:46
	filename	Renderer.h
	author:		maval

	purpose:	����Ⱦ��������
*********************************************************************/
#ifndef Renderer_h__
#define Renderer_h__

#include "Camera.h"
#include "MathDef.h"
#include "Rasterizer.h"
#include "GeometryDef.h"

typedef std::string STRING;

extern const int	SCREEN_WIDTH;
extern const int	SCREEN_HEIGHT;

namespace Common
{
	class PixelBox;
}

namespace SR
{
	class Renderer
	{
		friend class RenderUtil;
	public:
		Renderer();
		~Renderer();

		HWND	m_hwnd;

	public:
		void	SetRasterizeType(eRasterizeType type);
		//��Ⱦ����
		void	RenderOneFrame();
		//GDI��������
		void	DrawText(int x, int y, const STRING& text);

	private:
		void	_Clear();
		//����ǰ�󻺳�
		void	_Present();

	private:
		STriangle							m_testTri;
		std::unique_ptr<Common::PixelBox>	m_backBuffer;
		std::unordered_map<eRasterizeType, Rasterizer*>	m_rasLib;		//���п��ù�դ����
		Rasterizer*							m_curRas;		//��ǰʹ�ù�դ����
	};

	class RenderUtil
	{
	public:
		///////	������Ļ�����ֱ�߲ü�,ȡ��<<3D��̴�ʦ����>>
		static int	ClipLine(int& x1, int& y1, int& x2, int& y2);
		///////	Bresenahams�����㷨,ȡ��<<3D��̴�ʦ����>>
		static void	DrawClipLine_Bresenahams(int x0, int y0, int x1, int y1, int color);
		///////	��򵥵�DDA�����㷨
		static void	DrawClipLine_DDA(int x0, int y0, int x1, int y1, int color);
	};
}

extern SR::Renderer		g_renderer;	
extern SR::Camera	g_camera;

#endif // Renderer_h__