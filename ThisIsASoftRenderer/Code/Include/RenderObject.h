/********************************************************************
	created:	2013/07/23
	created:	23:7:2013   9:33
	filename: 	RenderObject.h
	author:		maval
	
	purpose:	��Ⱦ����߲��װ
*********************************************************************/
#ifndef RenderObject_h__
#define RenderObject_h__

#include "Prerequiestity.h"
#include "MathDef.h"
#include "GeometryDef.h"
#include "AABB.h"
#include "Rasterizer.h"
#include "Intersection.h"

namespace SR
{
	class RenderObject : public RayTraceRenderable
	{
	public:
		RenderObject();

	public:
		/*************** Override interfaces of RayTraceRenderable ***************/
		virtual bool	DoRayIntersect(VEC3& oIntersectPt, const RAY& ray) const;
		virtual VEC3	GetNormal(const VEC3& surfacePt) const;
		virtual void	UpdateWorldAABB();
		/************************************************************************/

		// Mip-map level determination
		void	CalcAllFaceTexArea();
		// Calc tangent space for mesh of this object
		void	BuildTangentVectors();

		void	OnFrameMove();
		void	SetShader(eRasterizeType shader);

		VertexBuffer	m_verts;
		FaceList		m_faces;
		SMaterial*		m_pMaterial;
		MAT44			m_matWorld;
		MAT44			m_matWorldIT;		//����������ת��,���ڷ��߱任
		AABB			m_localAABB;		//���ذ�Χ��
		AABB			m_worldAABB;		//�����Χ��
		bool			m_bStatic;			//�������ڳ�������ȫ�̶�
		Rasterizer*		m_pShader;

	private:
		// From Irrlicht
		void	_CalcTangentSpace(VEC3& oNormal, VEC3& oTangent, VEC3& oBinormal, 
			const VEC3& vt1, const VEC3& vt2, const VEC3& vt3,
			const VEC2& tc1, const VEC2& tc2, const VEC2& tc3);
	};

	typedef std::vector<RenderObject*>		RenderList;
}

#endif // RenderObject_h__