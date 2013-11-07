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
		eRasterizeType_TexturedGouraud,
		eRasterizeType_BlinnPhong,
		eRasterizeType_PhongWithNormalMap
	};

	enum eLerpType
	{
		eLerpType_Linear,		//���Բ�ֵ
		eLerpType_Hyper			//˫���߲�ֵ
	};

	/////////////////////////////////////////////////////////////
	//////// Rasterizer����
	class Rasterizer
	{
	public:
		virtual ~Rasterizer() {}

	public:
		virtual void	RasterizeTriangleList(SRenderContext& context);
		virtual eRasterizeType	GetType() = 0;
		//�𶥵����
		virtual void	DoPerVertexLighting(VertexBuffer& workingVB, FaceList& workingFaces, RenderObject& obj) {}
		//�����ع���
		virtual void	DoPerPixelLighting(SColor& result, void* pLightingContext, const SMaterial* pMaterial) {}
		// Only called when opening multi-thread
		virtual void	FragmentPS(SFragment& frag) {}
		////ɨ���߹�դ��
		virtual void	LerpVertexAttributes(SVertex* dest, const SVertex* src1, const SVertex* src2, float t, eLerpType type) = 0;
		virtual void	RasTriangleSetup(SScanLinesData& rasData, const SVertex* v0, const SVertex* v1, const SVertex* v2, eTriangleShape type);
		virtual void	RasLineSetup(SScanLine& scanLine, const SScanLinesData& rasData);
		virtual void	RasterizePixel(SScanLine& scanLine, const SScanLinesData& rasData) = 0;

	protected:
		virtual void	_RasterizeTriangle(const SVertex& vert0, const SVertex& vert1, const SVertex& vert2, const SFace& face, const SRenderContext& context) = 0;
	};

	/////////////////////////////////////////////////////////////
	//////// �߿�
	class RasWireFrame : public Rasterizer
	{
	public:
		virtual eRasterizeType	GetType()	{ return eRasterizeType_Wireframe; }
		virtual void	LerpVertexAttributes(SVertex* dest, const SVertex* src1, const SVertex* src2, float t, eLerpType type) {}
		virtual void	RasTriangleSetup(SScanLinesData& rasData, const SVertex* v0, const SVertex* v1, const SVertex* v2, eTriangleShape type) {}
		virtual void	RasLineSetup(SScanLine& scanLine, const SScanLinesData& rasData) {}
		virtual void	RasterizePixel(SScanLine& scanLine, const SScanLinesData& rasData) {}

	protected:
		virtual void	_RasterizeTriangle(const SVertex& vert0, const SVertex& vert1, const SVertex& vert2, const SFace& face, const SRenderContext& context);
	};

	/////////////////////////////////////////////////////////////
	//////// Flat 
	class RasFlat : public Rasterizer
	{
	public:
		virtual void	RasterizeTriangleList(SRenderContext& context);
		virtual eRasterizeType	GetType()	{ return eRasterizeType_Flat; }
		virtual void	DoPerVertexLighting(VertexBuffer& workingVB, FaceList& workingFaces, RenderObject& obj);
		virtual void	LerpVertexAttributes(SVertex* dest, const SVertex* src1, const SVertex* src2, float t, eLerpType type) {}
		virtual void	RasTriangleSetup(SScanLinesData& rasData, const SVertex* v0, const SVertex* v1, const SVertex* v2, eTriangleShape type) {}
		virtual void	RasLineSetup(SScanLine& scanLine, const SScanLinesData& rasData) {}
		virtual void	RasterizePixel(SScanLine& scanLine, const SScanLinesData& rasData) {}

		virtual void	_RasterizeTriangle(const SVertex& vert0, const SVertex& vert1, const SVertex& vert2, const SFace& face, const SRenderContext& context);
	};

	/////////////////////////////////////////////////////////////
	//////// Gouraud
	struct SScanLinesData 
	{
		VEC3	curP_L, curP_R;		//��Ļ�����x,z,w����
		VEC3	dp_L, dp_R;			//��Ļ�����x,z,w����
		float	inv_dy_L, inv_dy_R;
		VEC3	curPW_L, curPW_R;	//��������
		VEC3	dpw_L, dpw_R;		//������������
		VEC3	curN_L, curN_R;		//���編��
		VEC3	dn_L, dn_R;			//���編������
		int		curY, endY;
		VEC3	clr_L, clr_R;		//����color
		VEC3	dclr_L, dclr_R;		//����color����
		VEC2	curUV_L, curUV_R;	//UV
		VEC2	duv_L, duv_R;		//UV����
		VEC3	curLightDir_L, curLightDir_R;	// Light dir in tangent space
		VEC3	dLightDir_L, dLightDir_R;
		VEC3	curHVector_L, curHVector_R;		// H-vector in tangent space
		VEC3	dHVector_L, dHVector_R;
		int		texLod;
		bool	bClipY;
		float	clip_dy;
		SMaterial*	pMaterial;
	};

	struct SScanLine 
	{
		SColor	pixelColor, curPixelClr;
		VEC3	curClr, curPW, curN, curLightDir, curHVector;
		VEC3	finalPW, finalN, finalLightDir, finalHVector;
		VEC3	deltaClr, deltaPW, deltaN, deltaLightDir, deltaHVector;
		VEC2	curUV, finalUV;
		VEC2	deltaUV;
		int		lineX0, lineX1;
		float	z, dz, w, dw, inv_dx;
		bool	bClipX;
		float	clip_dx;
	};
	

	class RasGouraud : public Rasterizer
	{
	public:
		virtual eRasterizeType	GetType()	{ return eRasterizeType_Gouraud; }
		virtual void	DoPerVertexLighting(VertexBuffer& workingVB, FaceList& workingFaces, RenderObject& obj);
		virtual void	LerpVertexAttributes(SVertex* dest, const SVertex* src1, const SVertex* src2, float t, eLerpType type);
		virtual void	RasTriangleSetup(SScanLinesData& rasData, const SVertex* v0, const SVertex* v1, const SVertex* v2, eTriangleShape type);
		virtual void	RasLineSetup(SScanLine& scanLine, const SScanLinesData& rasData);
		virtual void	RasterizePixel(SScanLine& scanLine, const SScanLinesData& rasData);

	protected:
		virtual void	_RasterizeTriangle(const SVertex& vert0, const SVertex& vert1, const SVertex& vert2, const SFace& face, const SRenderContext& context);
	};

	/////////////////////////////////////////////////////////////
	//////// ����+gouraud
	class RasTexturedGouraud : public RasGouraud
	{
	public:
		virtual eRasterizeType	GetType()	{ return eRasterizeType_TexturedGouraud; }
		virtual void	LerpVertexAttributes(SVertex* dest, const SVertex* src1, const SVertex* src2, float t, eLerpType type);
		virtual void	RasTriangleSetup(SScanLinesData& rasData, const SVertex* v0, const SVertex* v1, const SVertex* v2, eTriangleShape type);
		virtual void	RasLineSetup(SScanLine& scanLine, const SScanLinesData& rasData);
		virtual void	RasterizePixel(SScanLine& scanLine, const SScanLinesData& rasData);

	protected:
		virtual void	_RasterizeTriangle(const SVertex& vert0, const SVertex& vert1, const SVertex& vert2, const SFace& face, const SRenderContext& context);
	};

	/////////////////////////////////////////////////////////////
	//////// Blinn-Phong����,Phongģ���������ع���

	struct SLightingContext_Phong
	{
		virtual ~SLightingContext_Phong() {}

		VEC3*	worldPos;
		VEC3*	worldNormal;
		VEC2*	uv;
	};

	class RasBlinnPhong : public Rasterizer
	{
	public:
		virtual eRasterizeType	GetType()	{ return eRasterizeType_BlinnPhong; }
		virtual void	DoPerPixelLighting(SColor& result, void* pLightingContext, const SMaterial* pMaterial);
		virtual void	FragmentPS(SFragment& frag);
		virtual void	LerpVertexAttributes(SVertex* dest, const SVertex* src1, const SVertex* src2, float t, eLerpType type);
		virtual void	RasTriangleSetup(SScanLinesData& rasData, const SVertex* v0, const SVertex* v1, const SVertex* v2, eTriangleShape type);
		virtual void	RasLineSetup(SScanLine& scanLine, const SScanLinesData& rasData);
		virtual void	RasterizePixel(SScanLine& scanLine, const SScanLinesData& rasData);

	protected:
		virtual void	_RasterizeTriangle(const SVertex& vert0, const SVertex& vert1, const SVertex& vert2, const SFace& face, const SRenderContext& context);
	};

	/////////////////////////////////////////////////////////////
	//////// Phong + ������ͼ

	struct SLightingContext_PhongWithNormalMap : public SLightingContext_Phong
	{
		VEC3*	lightDirTS;
		VEC3*	hVectorTS;
	};

	class RasPhongWithNormalMap : public RasBlinnPhong
	{
	public:
		virtual eRasterizeType	GetType()	{ return eRasterizeType_PhongWithNormalMap; }
		// Use TBN matrix in VS
		virtual void	DoPerVertexLighting(VertexBuffer& workingVB, FaceList& workingFaces, RenderObject& obj);
		// Apply normal mapping in PS
		virtual void	DoPerPixelLighting(SColor& result, void* pLightingContext, const SMaterial* pMaterial);
		virtual void	FragmentPS(SFragment& frag);
		virtual void	LerpVertexAttributes(SVertex* dest, const SVertex* src1, const SVertex* src2, float t, eLerpType type);
		virtual void	RasTriangleSetup(SScanLinesData& rasData, const SVertex* v0, const SVertex* v1, const SVertex* v2, eTriangleShape type);
		virtual void	RasLineSetup(SScanLine& scanLine, const SScanLinesData& rasData);
		virtual void	RasterizePixel(SScanLine& scanLine, const SScanLinesData& rasData);
	};
}


#endif // Rasterizer_h__