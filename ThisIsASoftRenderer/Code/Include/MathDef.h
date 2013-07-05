/********************************************************************
	created:	2:7:2013   22:38
	filename	MathDef.h
	author:		maval

	purpose:	������ѧ����.
				����Ŀ������������ϵ�����������.
				��ѧ����һ�ɲ���C����,����C++����������ص�����
*********************************************************************/
#ifndef MathDef_h__
#define MathDef_h__

const float		PI	=	3.14159f;

namespace Common
{
	/////////////////////////////////////////////////////////////
	//////// 2D Vector
	struct SVector2
	{
		SVector2() {}
		SVector2(float _x, float _y):x(_x),y(_y) {}

		float x, y;
	};

	/////////////////////////////////////////////////////////////
	//////// 3D Vector
	struct SVector3
	{
		SVector3() {}
		SVector3(float _x, float _y, float _z):x(_x),y(_y),z(_z) {}
		SVector3(const SVector3& rhs):x(rhs.x),y(rhs.y),z(rhs.z) {}

		float x, y, z;
	};

	/////////////////////////////////////////////////////////////
	//////// 4D Vector
	struct SVector4
	{
		SVector4() {}
		SVector4(SVector3 pt, float _w):x(pt.x),y(pt.y),z(pt.z),w(_w) {}
		SVector4(float _x, float _y, float _z, float _w):x(_x),y(_y),z(_z),w(_w) {}

		SVector3	GetVec3()	{ return std::move(SVector3(x,y,z)); }
		//��
		void		Neg();

		float x, y, z, w;
	};

	/////////////////////////////////////////////////////////////
	//////// 4x4 Matrix
	struct SMatrix44
	{
		SMatrix44() { MakeIdentity(); }
		SMatrix44(	float _m00, float _m01, float _m02, float _m03,
					float _m10, float _m11, float _m12, float _m13,
					float _m20, float _m21, float _m22, float _m23,
					float _m30, float _m31, float _m32, float _m33)
					:m00(_m00),m01(_m01),m02(_m02),m03(_m03)
					,m10(_m10),m11(_m11),m12(_m12),m13(_m13)
					,m20(_m20),m21(_m21),m22(_m22),m23(_m23)
					,m30(_m30),m31(_m31),m32(_m32),m33(_m33) {}

		void		MakeIdentity();
		void		Inverse()	{ /*TODO..*/ assert(0); }
		SMatrix44	Transpose();
		//���ƽ�Ʋ���
		void		ClearTranslation();
		//����ƽ�Ʋ���
		void		SetTranslation(const SVector4& t);
		//ͨ����ǶԹ�����ת����,ƽ�Ʋ�����Ϊ0
		void		FromAxisAngle(const SVector3& axis, float angle);

		union
		{
			struct  
			{
				float	m00, m01, m02, m03,
						m10, m11, m12, m13,
						m20, m21, m22, m23,
						m30, m31, m32, m33;
			};

			float m_arr[4][4];
		};
	};

	/////////////////////////////////////////////////////////////
	//////// ��4x4����任3d����,bPosOrDirΪtrue��ʾ�任���ǵ�,�����Ƿ���
	SVector4	Transform_Vec3_By_Mat44(const SVector3& pt, const SMatrix44& mat, bool bPosOrDir);

	/////////////////////////////////////////////////////////////
	//////// ��4x4����任4d����
	SVector4	Transform_Vec4_By_Mat44(const SVector4& pt, const SMatrix44& mat);

	/////////////////////////////////////////////////////////////
	//////// 4x4�������
	SMatrix44	Multiply_Mat44_By_Mat44(const SMatrix44& mat1, const SMatrix44& mat2);

	/////////////////////////////////////////////////////////////
	//////// 4d�������
	SVector4	Add_Vec4_By_Vec4(const SVector4& v1, const SVector4& v2);
}

#endif // MathDef_h__