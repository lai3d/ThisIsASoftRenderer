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
		void	Init();
		void	SetRasterizeType(eRasterizeType type);
		//�л���Ⱦģʽ(wireframe, flat, gouraud, phong)
		void	ToggleShadingMode();
		//��Ⱦ����
		void	RenderOneFrame();
		//����ǰ�󻺳�
		void	Present();
		//������ȾͼԪ
		void	AddRenderable(const SRenderObj& obj);
		const RenderList&	GetRenderList() const { return m_renderList; }
		DWORD	GetLastFPS() const	{ return m_lastFPS; }

	private:
		void	_Clear(const SColor& color, float depth);
		//�����ѡ.ͬʱ�Ա��޳��Ķ������ϱ��,�����ǲ���Ҫ�����������T&L��.
		VertexBuffer	_DoBackfaceCulling(SRenderObj& obj);

	private:
		std::unique_ptr<Gdiplus::Bitmap>	m_bmBackBuffer;
		std::unique_ptr<Common::PixelBox>	m_backBuffer;
		std::unique_ptr<Common::PixelBox>	m_zBuffer;
		std::unordered_map<eRasterizeType, Rasterizer*>	m_rasLib;		//���п��ù�դ����
		Rasterizer*							m_curRas;					//��ǰʹ�ù�դ����
		RenderList							m_renderList;				//��Ⱦ�б�
		DWORD								m_lastFPS;
	};

	class RenderUtil
	{
	public:
		///////	������Ļ�����ֱ�߲ü�,ȡ��<<3D��̴�ʦ����>>
		static int	ClipLine(int& x1, int& y1, int& x2, int& y2);
		/////// GDI��������
		static void	DrawText(float x, float y, const STRING& text, const Gdiplus::Color& color);
		/////// �������嶥������Χ����. NB:ע�⾡����������������ԭ��ӽ�,�������ϴ�.
		static float	ComputeBoundingRadius(const VertexBuffer& verts);
		///////	Bresenahams�����㷨,ȡ��<<3D��̴�ʦ����>>
		static void	DrawLine_Bresenahams(int x0, int y0, int x1, int y1, SColor color, bool bClip);
		///////	��򵥵�DDA�����㷨
		static void	DrawLine_DDA(int x0, int y0, int x1, int y1, SColor color, bool bClip);
		//////	�������դ��ǰԤ����
		static bool	PreDrawTriangle(const SVertex*& vert0, const SVertex*& vert1, const SVertex*& vert2, eTriangleShape& retType);
		///////	ɨ�����㷨����������
		static void	DrawTriangle_Scanline(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, SColor color);
		///////	ɨ�����㷨����ƽ��������
		static void	DrawBottomTri_Scanline(float x0, float y0, float x1, float y1, float x2, float y2, SColor color);
		///////	ɨ�����㷨����ƽ��������
		static void	DrawTopTri_Scanline(float x0, float y0, float x1, float y1, float x2, float y2, SColor color);
		///////	���ݻ����㷨���������б��������
		static void	SortTris_PainterAlgorithm(const VertexBuffer& verts, FaceList& faces);
		///////	���������εĵڶ����汾.Gouraud��ֵ������ɫ��������ɫ.
		static void	DrawTriangle_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTextured, const STexture* tex);
		static void	DrawBottomTri_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTextured, const STexture* tex);
		static void	DrawTopTri_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTextured, const STexture* tex);
	};
}

extern SR::Renderer		g_renderer;	

#endif // Renderer_h__