#include "stdafx.h"
#include "RenderObject.h"

namespace SR
{
	RenderObject::RenderObject()
	:m_bStatic(false)
	,m_pMaterial(nullptr)
	{

	}

	void RenderObject::OnFrameMove()
	{
		if (!m_bStatic)
		{
			//���±任���ߵ���ת�þ���
			m_matWorldIT = m_matWorld.Inverse();
			m_matWorldIT = m_matWorldIT.Transpose();

			//���������Χ��
			m_worldAABB = m_localAABB;
			m_worldAABB.Transform(m_matWorld);
		}
	}

}