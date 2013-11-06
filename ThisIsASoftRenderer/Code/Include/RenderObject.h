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

namespace SR
{
	class RenderObject
	{
	public:
		RenderObject();

		// Mip-map level determination
		void	CalcAllFaceTexArea();
		// Calc tangent space for mesh of this object
		void	BuildTangentVectors();

		void	OnFrameMove();

		VertexBuffer	m_verts;
		FaceList		m_faces;
		SMaterial*		m_pMaterial;
		MAT44			m_matWorld;
		MAT44			m_matWorldIT;		//����������ת��,���ڷ��߱任
		AABB			m_localAABB;		//���ذ�Χ��
		AABB			m_worldAABB;		//�����Χ��
		bool			m_bStatic;			//�������ڳ�������ȫ�̶�
	};

	typedef std::vector<RenderObject*>		RenderList;
}

#endif // RenderObject_h__