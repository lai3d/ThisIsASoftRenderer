#include "stdafx.h"
#include "Camera.h"
#include "Renderer.h"

namespace SR
{
	Camera::Camera()
	:m_viewPt(0, 0, 0, 1)
	,m_nearClip(1)
	,m_farClip(1000)
	,m_fixYawAxis(true)
	,m_moveSpeed(1.0f)
	{
		m_fov = Common::Angle_To_Radian(45);
		m_aspectRatio = SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	}

	void Camera::Update()
	{
		//��������
		POINT curCursorPos;
		GetCursorPos(&curCursorPos);
		static POINT lastCursorPos = curCursorPos;

		long dx = curCursorPos.x - lastCursorPos.x;
		long dy = curCursorPos.y - lastCursorPos.y;

		float yawDelta = 0, pitchDelta = 0;
		if(dx) yawDelta = -dx/5.0f;
		if(dy) pitchDelta = -dy/5.0f;

		lastCursorPos = curCursorPos;

		//�����ת
		if(dx)
		{
			Yaw(yawDelta);
		}
		if(dy)
		{
			MAT44 rotX;
			rotX.FromAxisAngle(VEC3::UNIT_X, pitchDelta);
			//pitch
			m_matRot = Common::Multiply_Mat44_By_Mat44(m_matRot, rotX);
		}

		//����ƶ�
		VEC4 forward = GetDirection();
		VEC4 right = GetRight();

		forward = Common::Multiply_Vec4_By_K(forward, m_moveSpeed);
		right = Common::Multiply_Vec4_By_K(right, m_moveSpeed);
		
		if(GetAsyncKeyState('W') < 0)		m_viewPt = Add_Vec4_By_Vec4(m_viewPt, forward);
		else if(GetAsyncKeyState('S') < 0)	m_viewPt = Sub_Vec4_By_Vec4(m_viewPt, forward);
		if(GetAsyncKeyState('A') < 0)		m_viewPt = Sub_Vec4_By_Vec4(m_viewPt, right);
		else if(GetAsyncKeyState('D') < 0)	m_viewPt = Add_Vec4_By_Vec4(m_viewPt, right);

		m_viewPt.w = 1;

		_BuildViewMatrix();
		_BuildProjMatrix();
	}

	void Camera::SetPosition( const VEC3& pos )
	{
		m_viewPt = VEC4(pos, 1);
	}

	void Camera::SetDirection( const VEC3& dir )
	{
		assert(m_fixYawAxis && "Error! Currently only support fix yaw axis mode..");
		
		VEC3 zAxis(dir);
		//NB: ���z��Ĭ���Ƿ���
		zAxis.Neg();
		zAxis.Normalize();

		VEC3 xAxis, yAxis;
		xAxis = Common::CrossProduct_Vec3_By_Vec3(VEC3::UNIT_Y, zAxis);
		xAxis.Normalize();

		yAxis = Common::CrossProduct_Vec3_By_Vec3(zAxis, xAxis);
		yAxis.Normalize();

		m_matRot.FromAxises(xAxis, yAxis, zAxis);
	}

	void Camera::_BuildViewMatrix()
	{
		m_matView = m_matRot.Transpose();
		
		VEC4 trans;
		trans = Common::Transform_Vec4_By_Mat44(m_viewPt, m_matView);
 		m_matView.SetTranslation(VEC4(-trans.x, -trans.y, -trans.z, 1));
	}

	void Camera::_BuildProjMatrix()
	{
		/*	���ǵ�һ���汾��͸��ͶӰ����,��"����"��ⷨ.
			����ȱ������Ϊ�˱任��xy���궼��[-1, 1]��CVV�ռ�,
			����������������:
			1.�Ӿ�d=1.
			2.fov=90��.
			3.�ӿ�AspectRatio=1.
			����任����CVV.

			MatProj = ( 1, 0,	0,	0
						0, 1,	0,	0
						0,  0,	1,	0
						0,  0, 1/d,  0 )

			ͶӰ�任����γ�����:
			x' = x * d / z, y' = y * d / z	*/

		//���ʰ�ͶӰ����.�Ƶ���: http://blog.csdn.net/popy007/article/details/1797121
		float r,l,t,b;
		r = m_nearClip*std::tan(m_fov/2);
		l = -r;
		t = r/m_aspectRatio;
		b= -t;

		m_matProj.m00 = 2*m_nearClip/(r-l);
		m_matProj.m01 = 0;
		m_matProj.m02 = (r+l)/(r-l);
		m_matProj.m03 = 0;

		m_matProj.m10 = 0;
		m_matProj.m11 = 2*m_nearClip/(t-b);
		m_matProj.m12 = (t+b)/(t-b);
		m_matProj.m13 = 0;

		m_matProj.m20 = 0;
		m_matProj.m21 = 0;
		m_matProj.m22 = -(m_farClip+m_nearClip)/(m_farClip-m_nearClip);
		m_matProj.m23 = -2*m_farClip*m_nearClip/(m_farClip-m_nearClip);

		m_matProj.m30 = 0;
		m_matProj.m31 = 0;
		m_matProj.m32 = -1;
		m_matProj.m33 = 0;
	}

	VEC4 Camera::GetDirection() const
	{
		//NB: �����ʼz���Ƿ���
		return Common::Transform_Vec3_By_Mat44(VEC3::NEG_UNIT_Z, m_matRot, false);
	}

	VEC4 Camera::GetRight() const	
	{
		return Common::Transform_Vec3_By_Mat44(VEC3::UNIT_X, m_matRot, false);
	}

	bool Camera::ObjectFrustumCulling( const SRenderObj& obj )
	{
		//��������ת��������ռ���вü�
		VEC4 pos = obj.matWorld.GetTranslation();
		pos = Common::Transform_Vec4_By_Mat44(pos, GetViewMatrix());

		float n = GetNearClip();
		float f = GetFarClip();
		float fov = GetFov();
		float half_w = n * std::tan(fov/2);
		float half_h = half_w / GetAspectRatio();

		//���ǰ����
		if(pos.z >= -n || pos.z <= -f)
			return true;

		//���������
		float planeX = half_w * pos.z / -n;
		if(pos.x - planeX >= obj.boundingRadius ||
			pos.x + obj.boundingRadius <= -planeX)
			return true;

		//���������
		float planeY = half_h * pos.z / -n;
		if(pos.y - planeY >= obj.boundingRadius ||
			pos.y + obj.boundingRadius <= -planeY)
			return true;

		return false;
	}

	void Camera::Yaw( float angle )
	{
		MAT44 rot;
		rot.FromAxisAngle(VEC3::UNIT_Y, angle);
		if(m_fixYawAxis)	m_matRot = Common::Multiply_Mat44_By_Mat44(rot, m_matRot);
		else				m_matRot = Common::Multiply_Mat44_By_Mat44(m_matRot, rot);
	}

	void Camera::AddMoveSpeed( float delta )
	{
		m_moveSpeed += delta;
		m_moveSpeed = max(min(m_moveSpeed, 10), 0.1f);
	}

}

