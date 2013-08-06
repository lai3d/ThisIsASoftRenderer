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

#include "Utility.h"

const float		PI			=	3.14159f;
const float		HALF_PI		=	1.57079f;
const float		TWO_PI		=	6.28318f;

namespace Common
{
	/////////////////////////////////////////////////////////////
	//////// 2D Vector
	class Vector2
	{
	public:
		Vector2():x(0),y(0) {}
		Vector2(float _x, float _y):x(_x),y(_y) {}

		inline void Set(float _x, float _y) { x=_x; y=_y; }

		float x, y;
	};

	/////////////////////////////////////////////////////////////
	//////// 3D Vector
	class Vector3
	{
	public:
		Vector3():x(0),y(0),z(0) {}
		Vector3(float _x, float _y, float _z):x(_x),y(_y),z(_z) {}
		Vector3(const Vector3& rhs):x(rhs.x),y(rhs.y),z(rhs.z) {}

		inline void Set(float _x, float _y, float _z) { x=_x; y=_y; z=_z; }

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

		static Vector3		ZERO;
		static Vector3		UNIT_X;
		static Vector3		UNIT_Y;
		static Vector3		UNIT_Z;
		static Vector3		NEG_UNIT_X;
		static Vector3		NEG_UNIT_Y;
		static Vector3		NEG_UNIT_Z;
	};

	/////////////////////////////////////////////////////////////
	//////// 4D Vector
	class Vector4
	{
	public:
		Vector4():x(0),y(0),z(0),w(0) {}
		Vector4(Vector3 pt, float _w):x(pt.x),y(pt.y),z(pt.z),w(_w) {}
		Vector4(float _x, float _y, float _z, float _w):x(_x),y(_y),z(_z),w(_w) {}

		inline void		Set(float _x, float _y, float _z, float _w) { x=_x; y=_y; z=_z; w=_w; }
		inline const Vector3& GetVec3() const	{ return vec3; }
		//��
		inline void	Neg() { x = -x; y = -y; z = -z; w = -w; }

		union
		{
			struct { Vector3 vec3; float w; };
			struct { float x, y, z, w; };
		};

		static Vector4		ZERO;
	};

	/////////////////////////////////////////////////////////////
	//////// 4x4 Matrix
	class Matrix44
	{
	public:
		Matrix44() { MakeIdentity(); }
		Matrix44(	float _m00, float _m01, float _m02, float _m03,
					float _m10, float _m11, float _m12, float _m13,
					float _m20, float _m21, float _m22, float _m23,
					float _m30, float _m31, float _m32, float _m33)
					:m00(_m00),m01(_m01),m02(_m02),m03(_m03)
					,m10(_m10),m11(_m11),m12(_m12),m13(_m13)
					,m20(_m20),m21(_m21),m22(_m22),m23(_m23)
					,m30(_m30),m31(_m31),m32(_m32),m33(_m33) {}

		void		SetRow(int row, const Vector4 vec);
		//��λ����
		void		MakeIdentity();
		//�����
		inline void	MakeZero() { memset(m_arr, 0, sizeof(m_arr)); }
		//����
		Matrix44	Inverse();
		//ת��
		Matrix44	Transpose();
		//���ƽ�Ʋ���
		inline void	ClearTranslation() { m_arr[0][3] = m_arr[1][3] = m_arr[2][3] = 0; m_arr[3][3] = 1; }
		//����ƽ�Ʋ���
		void		SetTranslation(const Vector4& t);
		//��ȡƽ�Ʋ���
		inline Vector4	GetTranslation() const { return std::move(VEC4(m03, m13, m23, m33)); }
		//ͨ����ǶԹ�����ת����,ƽ�Ʋ�����Ϊ0
		void		FromAxisAngle(const Vector3& axis, float angle);
		//ͨ���ṹ������
		void		FromAxises(const Vector3& v1, const Vector3& v2, const Vector3& v3);

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

	//////// ��4x4����任4d����
	void	Transform_Vec4_By_Mat44(Vector4& result, const Vector4& pt, const Matrix44& mat);
	Vector4	Transform_Vec4_By_Mat44(const Vector4& pt, const Matrix44& mat);
	
	//////// ��4x4����任3d����,bPosOrDirΪtrue��ʾ�任���ǵ�,�����Ƿ���
	inline Vector4	Transform_Vec3_By_Mat44(const Vector3& pt, const Matrix44& mat, bool bPosOrDir)
	{
		return Transform_Vec4_By_Mat44(Vector4(pt, bPosOrDir ? 1.0f : 0.0f), mat);
	}

	//////// 4x4�������
	Matrix44	Multiply_Mat44_By_Mat44(const Matrix44& mat1, const Matrix44& mat2);

	//////// 4d�������
	inline Vector4	Add_Vec4_By_Vec4(const Vector4& v1, const Vector4& v2)
	{
		return std::move(Vector4(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w));
	}

	//////// 4d�������Գ���
	inline Vector4	Multiply_Vec4_By_K(const Vector4& v, float k)
	{
		return std::move(Vector4(v.x * k, v.y * k, v.z * k, v.w * k));
	}

	//////// 4d�������
	inline Vector4	Sub_Vec4_By_Vec4(const Vector4& v1, const Vector4& v2)
	{
		return std::move(Vector4(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w));
	}

	//////// 3d�������
	inline Vector3	Sub_Vec3_By_Vec3(const Vector3& v1, const Vector3& v2)
	{
		return std::move(Vector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z));
	}

	//////// 2d�������
	inline void	Add_Vec2_By_Vec2(Vector2& result, const Vector2& v1, const Vector2& v2)
	{
		float x = v1.x + v2.x;
		float y = v1.y + v2.y;

		result.Set(x, y);
	}

	//////// 3d�������
	inline void	Add_Vec3_By_Vec3(Vector3& result, const Vector3& v1, const Vector3& v2)
	{
		float x = v1.x + v2.x;
		float y = v1.y + v2.y;
		float z = v1.z + v2.z;

		result.Set(x, y, z);
	}

	inline Vector3	Add_Vec3_By_Vec3(const Vector3& v1, const Vector3& v2)
	{
		return std::move(Vector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z));
	}

	//////// 3d�������
	inline float	DotProduct_Vec3_By_Vec3(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	//////// 3d�������
	inline Vector3	CrossProduct_Vec3_By_Vec3(const Vector3& v1, const Vector3& v2)
	{
		Vector3 ret;

		ret.x = v1.y * v2.z - v1.z * v2.y;
		ret.y = v1.z * v2.x - v1.x * v2.z;
		ret.z = v1.x * v2.y - v1.y * v2.x;

		return std::move(ret);
	}

	//////// 3d�������Գ���
	inline Vector3	Multiply_Vec3_By_K(const Vector3& v, float k)
	{
		return std::move(Vector3(v.x * k, v.y * k, v.z * k));
	}

	//////// 2d�������Գ���
	inline Vector2	Multiply_Vec2_By_K(const Vector2& v, float k)
	{
		return std::move(Vector2(v.x * k, v.y * k));
	}

	//////// �Ƕ�ת����
	inline float	Angle_To_Radian(float angle)
	{
		return angle * PI / 180;
	}

	//////// ��������
	inline float	Vec3_Distance(const Vector3& v1, const Vector3& v2)
	{
		float dx = v1.x - v2.x, dy = v1.y - v2.y, dz = v1.z - v2.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}
}

namespace Ext
{
	//�ػ�Vector2
	template<> inline void LinearLerp(VEC2& result, const VEC2& s, const VEC2& e, float t)
	{
		LinearLerp(result.x, s.x, e.x, t);
		LinearLerp(result.y, s.y, e.y, t);
	}
	//�ػ�Vector3
	template<> inline void LinearLerp(VEC3& result, const VEC3& s, const VEC3& e, float t)
	{
		LinearLerp(result.x, s.x, e.x, t);
		LinearLerp(result.y, s.y, e.y, t);
		LinearLerp(result.z, s.z, e.z, t);
	}
	//�ػ�Vector4
	template<> inline void LinearLerp(VEC4& result, const VEC4& s, const VEC4& e, float t)
	{
		LinearLerp(result.x, s.x, e.x, t);
		LinearLerp(result.y, s.y, e.y, t);
		LinearLerp(result.z, s.z, e.z, t);
		LinearLerp(result.w, s.w, e.w, t);
	}

	//�ػ�Vector2
	template<> inline void HyperLerp(VEC2& result, const VEC2& s, const VEC2& e, float t, float ws, float we)
	{
		HyperLerp(result.x, s.x, e.x, t, ws, we);
		HyperLerp(result.y, s.y, e.y, t, ws, we);
	}
	//�ػ�Vector3
	template<> inline void HyperLerp(VEC3& result, const VEC3& s, const VEC3& e, float t, float ws, float we)
	{
		HyperLerp(result.x, s.x, e.x, t, ws, we);
		HyperLerp(result.y, s.y, e.y, t, ws, we);
		HyperLerp(result.z, s.z, e.z, t, ws, we);
	}
	//�ػ�Vector4
	template<> inline void HyperLerp(VEC4& result, const VEC4& s, const VEC4& e, float t, float ws, float we)
	{
		HyperLerp(result.x, s.x, e.x, t, ws, we);
		HyperLerp(result.y, s.y, e.y, t, ws, we);
		HyperLerp(result.z, s.z, e.z, t, ws, we);
		HyperLerp(result.w, s.w, e.w, t, ws, we);
	}
}

#endif // MathDef_h__