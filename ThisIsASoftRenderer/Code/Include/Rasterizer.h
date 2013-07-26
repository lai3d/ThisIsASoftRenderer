/********************************************************************
	created:	3:7:2013   0:00
	filename	Rasterizer.h
	author:		maval

	purpose:	һϵ�й�դ����
*********************************************************************/
#ifndef Rasterizer_h__
#define Rasterizer_h__

#include "Prerequiestity.h"
#include "GeometryDef.h"

namespace SR
{
	enum eRasterizeType
	{
		eRasterizeType_Wireframe,
		eRasterizeType_Flat,
		eRasterizeType_Gouraud,
		eRasterizeType_TexturedGouraud
	};

	/////////////////////////////////////////////////////////////
	//////// Rasterizer����
	class Rasterizer
	{
	public:
		virtual ~Rasterizer() {}

	public:
		virtual void	RasterizeTriangleList(SRenderContext& context) = 0;
		virtual eRasterizeType	GetType() = 0;
		virtual void	DoLighting(VertexBuffer& workingVB, FaceList& workingFaces, RenderObject& obj, const SDirectionLight& light) = 0;
	};

	/////////////////////////////////////////////////////////////
	//////// �߿�
	class RasWireFrame : public Rasterizer
	{
	public:
		virtual void	RasterizeTriangleList(SRenderContext& context);
		virtual eRasterizeType	GetType()	{ return eRasterizeType_Wireframe; }
		virtual void	DoLighting(VertexBuffer&, FaceList&, RenderObject&, const SDirectionLight&) {}
	};

	/////////////////////////////////////////////////////////////
	//////// Flat 
	class RasFlat : public Rasterizer
	{
	public:
		virtual void	RasterizeTriangleList(SRenderContext& context);
		virtual eRasterizeType	GetType()	{ return eRasterizeType_Flat; }
		virtual void	DoLighting(VertexBuffer& workingVB, FaceList& workingFaces, RenderObject& obj, const SDirectionLight& light);
	};

	/////////////////////////////////////////////////////////////
	//////// Gouraud
	class RasGouraud : public Rasterizer
	{
	public:
		virtual void	RasterizeTriangleList(SRenderContext& context);
		virtual eRasterizeType	GetType()	{ return eRasterizeType_Gouraud; }
		virtual void	DoLighting(VertexBuffer& workingVB, FaceList& workingFaces, RenderObject& obj, const SDirectionLight& light);

		struct SScanLineData 
		{
			VEC2	curP_L, curP_R;
			VEC2	dp_L, dp_R;
			int		curY, endY;
			float	y0;
			VEC3	clr_L, clr_R;
			VEC3	dclr_L, dclr_R;
			VEC2	curUV_L, curUV_R;
			VEC2	duv_L, duv_R;
		};

	protected:
		SScanLineData	m_scanLineData;
	};

	/////////////////////////////////////////////////////////////
	//////// ����+gouraud
	class RasTexturedGouraud : public RasGouraud
	{
	public:
		virtual void	RasterizeTriangleList(SRenderContext& context);
		virtual eRasterizeType	GetType()	{ return eRasterizeType_TexturedGouraud; }
	};
}


#endif // Rasterizer_h__