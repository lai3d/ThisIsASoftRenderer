/********************************************************************
	created:	2013/07/23
	created:	23:7:2013   10:23
	filename: 	Prerequiestity.h
	author:		maval
	
	purpose:	��Ŀǰ���ļ�
*********************************************************************/
#ifndef Prerequiestity_h__
#define Prerequiestity_h__

#define USE_PERSPEC_CORRECT		1			//�Ƿ�ʹ��͸������
#define USE_OPENMP				0			//ʹ��OpenMP����ָ����ж�˲���

typedef std::string STRING;

namespace Common
{
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix44;
	class PixelBox;
	class AxisAlignBBox;
}

typedef Common::Vector2		VEC2;
typedef Common::Vector3		VEC3;
typedef Common::Vector4		VEC4;
typedef Common::Matrix44	MAT44;
typedef Common::AxisAlignBBox	AABB;

namespace SR
{
	class Renderer;
	class Scene;
	class Camera;
	class RenderObject;
	class Rasterizer;
	class RasWireFrame;
	class RasFlat;
	class RasGouraud;
	class RasTexturedGouraud;
	struct SRenderContext;
	struct STexture;
	struct SMaterial;
}

namespace Ext
{
	class MeshLoader;
	class OgreMeshLoader;
	class ObjMeshLoader;
}

//������Ŀֱ��ȡ�õ�ȫ�ֱ���
struct SGlobalEnv 
{
	HWND					hwnd;					// �����ھ��
	SR::Renderer*			renderer;				// ��Ⱦ��
	Ext::OgreMeshLoader*	meshLoader;				// .mesh������
	Ext::ObjMeshLoader*		objLoader;				// .obj������
};
extern SGlobalEnv	g_env;

#endif // Prerequiestity_h__