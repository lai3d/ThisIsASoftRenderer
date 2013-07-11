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
#include "Utility.h"

extern const int	SCREEN_WIDTH;
extern const int	SCREEN_HEIGHT;
extern const int	PIXEL_MODE;

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

		HWND		m_hwnd;
		Camera		m_camera;
		SDirectionLight	m_testLight;	//���Է����

	public:
		//������Ⱦģʽ(wireframe, flat, gouround, phong)
		void	SetRasterizeType(eRasterizeType type);
		//��Ⱦ����
		void	RenderOneFrame();
		//������ȾͼԪ
		void	AddRenderable(const SRenderObj& obj);
		const RenderList&	GetRenderList() const { return m_renderList; }

	private:
		void	_Clear();
		//�����ѡ.ͬʱ�Ա��޳��Ķ������ϱ��,�����ǲ���Ҫ�����������T&L��.
		VertexBuffer	_DoBackfaceCulling(SRenderObj& obj);
		//����ǰ�󻺳�
		void	_Present();

	private:
		std::unique_ptr<Common::PixelBox>	m_backBuffer;
		std::unordered_map<eRasterizeType, Rasterizer*>	m_rasLib;		//���п��ù�դ����
		Rasterizer*							m_curRas;					//��ǰʹ�ù�դ����
		RenderList							m_renderList;				//��Ⱦ�б�
	};

	class RenderUtil
	{
	public:
		///////	������Ļ�����ֱ�߲ü�,ȡ��<<3D��̴�ʦ����>>
		static int	ClipLine(int& x1, int& y1, int& x2, int& y2);
		/////// GDI��������
		static void	DrawText(int x, int y, const STRING& text, const COLORREF color);
		/////// �������嶥������Χ����. NB:ע�⾡����������������ԭ��ӽ�,�������ϴ�.
		static float	ComputeBoundingRadius(const VertexBuffer& verts);
		///////	Bresenahams�����㷨,ȡ��<<3D��̴�ʦ����>>
		static void	DrawLine_Bresenahams(int x0, int y0, int x1, int y1, int color, bool bClip);
		///////	��򵥵�DDA�����㷨
		static void	DrawLine_DDA(int x0, int y0, int x1, int y1, int color, bool bClip);
		///////	ɨ�����㷨����������. NB:˳��Ӧ������ʱ��.
		static void	DrawTriangle_Scanline(int x0, int y0, int x1, int y1, int x2, int y2, int color);
		///////	ɨ�����㷨����ƽ��������
		static void	DrawBottomTri_Scanline(int x0, int y0, int x1, int y1, int x2, int y2, int color);
		///////	ɨ�����㷨����ƽ��������
		static void	DrawTopTri_Scanline(int x0, int y0, int x1, int y1, int x2, int y2, int color);
	};
}

extern SR::Renderer		g_renderer;	

#endif // Renderer_h__