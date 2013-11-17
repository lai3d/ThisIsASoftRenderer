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
		///////	������Ļ�����ֱ�߲ü�. Sutherland-hogdman�㷨
		static bool	ClipLine(int& x1, int& y1, int& x2, int& y2);

		/////// GDI��������
		static void	DrawText(float x, float y, const STRING& text, const Gdiplus::Color& color);

		/////// �������ж�����������Χ��
		static void	ComputeAABB(RenderObject& obj);

		///////	DDA�����㷨
		static void	DrawLine_DDA(int x0, int y0, int x1, int y1, const SColor& color);

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
		static void	DrawTriangle_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, const SRenderContext& context);
		static void	DrawBottomTri_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, const SRenderContext& context);
		static void	DrawTopTri_Scanline_V2(const SVertex* vert0, const SVertex* vert1, const SVertex* vert2, const SRenderContext& context);
		static void RasterizeScanLines(SScanLinesData& scanLineData);

		///////	����Lambert����,�ٶ����ߺ͹�Դ�������ѹ�һ��
		static void DoLambertLighting(SColor& result, const VEC3& normal, const VEC3& lightDir, const SMaterial* pMaterial);
	};
}

#endif // RenderUtil_h__