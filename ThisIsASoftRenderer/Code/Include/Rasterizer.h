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
		eRasterizeType_FlatWire
	};

	/////////////////////////////////////////////////////////////
	//////// Rasterizer����
	class Rasterizer
	{
	public:
		virtual ~Rasterizer() {}

	public:
		virtual void	RasterizeTriangleList(const VertexBuffer& vb, const IndexBuffer& ib) = 0;
	};

	/////////////////////////////////////////////////////////////
	//////// Flat+�߿�
	class RasWireFrame : public Rasterizer
	{
	public:
		virtual void	RasterizeTriangleList(const VertexBuffer& vb, const IndexBuffer& ib);
	};
}


#endif // Rasterizer_h__