/********************************************************************
	created:	2:7:2013   22:38
	filename	MathDef.h
	author:		maval

	purpose:	��д��ѧ��.
				����Ŀ������������ϵ�����������.
				��ѧ����һ�ɲ���C����,����C++����������ص�����
*********************************************************************/
#ifndef MathDef_h__
#define MathDef_h__

const float		PI			=	3.14159f;
const float		HALF_PI		=	1.57079f;
const float		TWO_PI		=	6.28318f;

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

		inline void	Normalize()
		{
			float mod = std::sqrt(x * x + y * y + z * z);
			float invMode = 1 / mod;
			x *= invMode;
			y *= invMode;
			z *= invMode;
		}
		//��
		inline void	Neg() { x = -x; y = -y; z = -z; }

		float x, y, z;

		static SVector3		ZERO;
		static SVector3		UNIT_X;
		static SVector3		UNIT_Y;
		static SVector3		UNIT_Z;
		static SVector3		NEG_UNIT_X;
		static SVector3		NEG_UNIT_Y;
		static SVector3		NEG_UNIT_Z;
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
		inline void	Neg() { x = -x; y = -y; z = -z; w = -w; }

		float x, y, z, w;

		static SVector4		ZERO;
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

		void		SetRow(int row, const SVector4 vec);
		//��λ����
		void		MakeIdentity();
		//�����
		void		MakeZero();
		//����
		void		Inverse()	{ /*TODO..*/ assert(0); }
		//ת��
		SMatrix44	Transpose();
		//���ƽ�Ʋ���
		void		ClearTranslation();
		//����ƽ�Ʋ���
		void		SetTranslation(const SVector4& t);
		//��ȡƽ�Ʋ���
		SVector4	GetTranslation() const;
		//ͨ����ǶԹ�����ת����,ƽ�Ʋ�����Ϊ0
		void		FromAxisAngle(const SVector3& axis, float angle);
		//ͨ���ṹ������
		void		FromAxises(const SVector3& v1, const SVector3& v2, const SVector3& v3);

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

	/////////////////////////////////////////////////////////////
	//////// 4d�������Գ���
	SVector4	Multiply_Vec4_By_K(const SVector4& v, float k);

	/////////////////////////////////////////////////////////////
	//////// 4d�������
	SVector4	Sub_Vec4_By_Vec4(const SVector4& v1, const SVector4& v2);

	/////////////////////////////////////////////////////////////
	//////// 3d�������
	SVector3	CrossProduct_Vec3_By_Vec3(const SVector3& v1, const SVector3& v2);

	/////////////////////////////////////////////////////////////
	//////// �Ƕ�ת����
	inline float Angle_To_Radian(float angle)
	{
		return angle * PI / 180;
	}
}

typedef Common::SVector2	VEC2;
typedef Common::SVector3	VEC3;
typedef Common::SVector4	VEC4;
typedef Common::SMatrix44	MAT44;

#endif // MathDef_h__