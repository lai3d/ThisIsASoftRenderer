/********************************************************************
	created:	3:7:2013   0:00
	filename	Rasterizer.h
	author:		maval

	purpose:	一系列光栅化器
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
		eRasterizeType_TexturedGouraud,
		eRasterizeType_BlinnPhong
	};

	/////////////////////////////////////////////////////////////
	//////// Rasterizer基类
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
	//////// 线框
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
			VEC3	curP_L, curP_R;		//pos的x,z,w分量
			VEC3	dp_L, dp_R;			//pos的x,z,w差分
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
	//////// 纹理+gouraud
	class RasTexturedGouraud : public RasGouraud
	{
	public:
		virtual void	RasterizeTriangleList(SRenderContext& context);
		virtual eRasterizeType	GetType()	{ return eRasterizeType_TexturedGouraud; }
	};

	/////////////////////////////////////////////////////////////
	//////// Blinn-Phong光照
	class RasBlinnPhong : public RasTexturedGouraud
	{
	public:
		virtual void	DoLighting(VertexBuffer& workingVB, FaceList& workingFaces, RenderObject& obj, const SDirectionLight& light);
		virtual eRasterizeType	GetType()	{ return eRasterizeType_BlinnPhong; }
	};
}


#endif // Rasterizer_h__