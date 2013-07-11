/********************************************************************
	created:	3:7:2013   0:00
	filename	Rasterizer.h
	author:		maval

	purpose:	һϵ�й�դ����
*********************************************************************/
#ifndef Rasterizer_h__
#define Rasterizer_h__

#include "GeometryDef.h"

namespace SR
{
	enum eRasterizeType
	{
		eRasterizeType_Wireframe,
		eRasterizeType_Flat
	};

	/////////////////////////////////////////////////////////////
	//////// Rasterizer����
	class Rasterizer
	{
	public:
		virtual ~Rasterizer() {}

	public:
		virtual void	RasterizeTriangleList(const VertexBuffer& vb, FaceList& faces) = 0;
	};

	/////////////////////////////////////////////////////////////
	//////// �߿�
	class RasWireFrame : public Rasterizer
	{
	public:
		virtual void	RasterizeTriangleList(const VertexBuffer& vb, FaceList& faces);
	};

	/////////////////////////////////////////////////////////////
	//////// Flat 
	class RasFlat : public Rasterizer
	{
	public:
		virtual void	RasterizeTriangleList(const VertexBuffer& vb, FaceList& faces);

	public:
		
	};
}


#endif // Rasterizer_h__