/********************************************************************
	created:	2013/08/12
	created:	12:8:2013   20:33
	filename: 	RenderUtil_h
	author:		maval
	
	purpose:	������Ⱦ�㷨����
*********************************************************************/
#ifndef RenderUtil_h__
#define RenderUtil_h__

#include "Prerequiestity.h"
#include "GeometryDef.h"

namespace SR
{
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

		///////	����T&L�׶�
		static void	ObjectTnL(RenderObject& obj, SRenderContext& context);

		/////// �����ѡ.ͬʱ�Ա��޳��Ķ������ϱ��,�����ǲ���Ҫ�����������T&L��.
		static void	DoBackfaceCulling(VertexBuffer& vb, FaceList& workingFaces, RenderObject& obj);

		/////// 3D�ü�.�Խ��������вü�.
		static void	Do3DClipping(VertexBuffer& VB, FaceList& faces);

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
		static void	DrawBottomTri_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTextured, bool bPerPixel, const SRenderContext& context);
		static void	DrawTopTri_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTextured, bool bPerPixel, const SRenderContext& context);
		static void DrawScanLines(SScanLineData& scanLineData, bool bTextured, bool bPerPixel, const SRenderContext& context);

		///////	����Lambert����,�ٶ������ѹ�һ��
		static void DoLambertLighting(SColor& result, const VEC3& wNormal, const SMaterial* pMaterial);

		///////	���������εĵ������汾.������̲߳���.
		static void	DrawTri_Scanline_V3(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, const SRenderContext& context, bool bTopTir);
		static void RasTriSetup(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, bool bTopTri, SScanLineData& rasData);
		static void RasterizeTriangle(SScanLineData& scanLineData, const SMaterial* pMaterial);
		static void DrawFragment(SFragment& frag);
	};
}

#endif // RenderUtil_h__