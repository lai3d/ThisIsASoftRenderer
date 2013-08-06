/********************************************************************
	created:	30:6:2013   23:46
	filename	Renderer.h
	author:		maval

	purpose:	����Ⱦ��������
*********************************************************************/
#ifndef Renderer_h__
#define Renderer_h__

#include "Prerequiestity.h"
#include "Camera.h"
#include "MathDef.h"
#include "Rasterizer.h"
#include "GeometryDef.h"
#include "Utility.h"
#include "RenderObject.h"

extern const int	SCREEN_WIDTH;
extern const int	SCREEN_HEIGHT;
extern const int	PIXEL_MODE;

namespace SR
{
	/////////////////////////////////////////////////////////////
	struct SRenderContext 
	{
		SRenderContext():verts(nullptr),faces(nullptr),pMaterial(nullptr) {}

		VertexBuffer*	verts;
		FaceList*		faces;
		SMaterial*		pMaterial;
	};

	/////////////////////////////////////////////////////////////
	class Renderer
	{
		friend class RenderUtil;
	public:
		Renderer();
		~Renderer();

		//��Ⱦ����ͳ��
		struct SFrameStatics 
		{
			void Reset()
			{
				nObjCulled = nBackFace = nFaceCulled = nRenderedFace = 0;
			}
			DWORD	nObjCulled;
			DWORD	nBackFace;
			DWORD	nFaceCulled;
			DWORD	nRenderedFace;
			DWORD	lastFPS;
		};

		SFrameStatics	m_frameStatics;
		Camera			m_camera;
		SDirectionLight	m_testLight;	//���Է����
		SColor			m_ambientColor;	

	public:
		void	Init();
		void	SetRasterizeType(eRasterizeType type);
		void	OnFrameMove();
		//�л���Ⱦģʽ(wireframe, flat, gouraud, phong)
		void	ToggleShadingMode();
		//��Ⱦ����
		void	RenderOneFrame();
		//����ǰ�󻺳�
		void	Present();
		//�����Ⱦ����
		void	AddRenderable(const RenderObject& obj);
		void	AddRenderObjs(const RenderList& objs);
		//��Ӳ���
		void	AddMaterial(const STRING& name, const SMaterial* mat);
		//��ȡ����
		SMaterial*	GetMaterial(const STRING& name);

	private:
		void	_Clear(const SColor& color, float depth);
		//�����ѡ.ͬʱ�Ա��޳��Ķ������ϱ��,�����ǲ���Ҫ�����������T&L��.
		VertexBuffer	_DoBackfaceCulling(FaceList& workingFaces, RenderObject& obj);
		//3D�ü�.�Խ��������вü�.
		void			_Do3DClipping(VertexBuffer& VB, FaceList& faces);

	private:
		std::unique_ptr<Gdiplus::Bitmap>	m_bmBackBuffer;
		std::unique_ptr<Common::PixelBox>	m_backBuffer;
		std::unique_ptr<Common::PixelBox>	m_zBuffer;

		std::unordered_map<eRasterizeType, Rasterizer*>	m_rasLib;		//���п���shader
		Rasterizer*							m_curRas;					//��ǰʹ��shader

		std::unordered_map<STRING, SMaterial*>	m_matLib;				//���ʿ�

		RenderList							m_renderList;				//��Ⱦ�б�
	};

	class RenderUtil
	{
	public:
		///////	������Ļ�����ֱ�߲ü�,ȡ��<<3D��̴�ʦ����>>
		static int	ClipLine(int& x1, int& y1, int& x2, int& y2);
		/////// GDI��������
		static void	DrawText(float x, float y, const STRING& text, const Gdiplus::Color& color);
		/////// �������ж�����������Χ��
		static void	ComputeAABB(RenderObject& obj);
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
		///////	���������εĵڶ����汾.���Բ�ֵ�����attribute
		static void	DrawTriangle_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTextured, bool bPerPixel, const SRenderContext& context);
		static void	DrawBottomTri_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTextured, bool bPerPixel, const SRenderContext& context, RasGouraud::SScanLineData& scanLineData);
		static void	DrawTopTri_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTextured, bool bPerPixel, const SRenderContext& context, RasGouraud::SScanLineData& scanLineData);
		static void DrawScanLines(RasGouraud::SScanLineData& scanLineData, bool bTextured, bool bPerPixel, const SRenderContext& context);
	};
}

#endif // Renderer_h__