/********************************************************************
	created:	2:7:2013   22:35
	filename	Camera.h
	author:		maval

	purpose:	���������
*********************************************************************/
#ifndef Camera_h__
#define Camera_h__

#include "MathDef.h"

namespace SR
{
	class Camera
	{
	public:
		Camera();

	public:
		void	Update();
		float	GetNearClip()	{ return m_nearClip; }

		const Common::SMatrix44&	GetViewMatrix() const	{ return m_matView; }
		const Common::SVector4&		GetViewPt() const		{ return m_viewPt;	}

	private:
 		Common::SVector4	m_viewPt;		//�������λ��

		float				m_nearClip;
		float				m_farClip;
		float				m_fov;

		Common::SMatrix44	m_matCamWorld;	//�����������ת����
		Common::SMatrix44	m_matView;
	};
}

#endif // Camera_h__