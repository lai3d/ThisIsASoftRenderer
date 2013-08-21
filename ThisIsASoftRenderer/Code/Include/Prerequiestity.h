/********************************************************************
	created:	2013/07/23
	created:	23:7:2013   10:23
	filename: 	Prerequiestity.h
	author:		maval
	
	purpose:	��Ŀǰ���ļ�
*********************************************************************/
#ifndef Prerequiestity_h__
#define Prerequiestity_h__

const int		SCREEN_WIDTH	=	800;
const int		SCREEN_HEIGHT	=	600;
const int		PIXEL_MODE		=	4;		//����ģʽ(�ֽ���). NB:ֻ֧��32λģʽ,��Ҫ����

#define USE_32BIT_INDEX			0			//�Ƿ�ʹ��32λ��������
#define USE_PERSPEC_CORRECT		1			//�Ƿ�ʹ��͸������
#define USE_OPENMP				0			//ʹ��OpenMP����ָ����ж�˲���
#define USE_PROFILER			1			//�Ƿ�ʹ��Profiler
#define USE_MULTI_THREAD		1			//�Ƿ�ʹ�ö��߳�(ֻ����sponza��������)

#if USE_OPENMP == 1 && USE_MULTI_THREAD == 1
# error Can't enable OpenMP and Multi-Thread at the same time!
#endif

typedef std::string STRING;

namespace Common
{
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix44;
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
	class PixelBox;
	class Scene;
	class Camera;
	class RenderObject;
	class Rasterizer;
	class RasWireFrame;
	class RasFlat;
	class RasGouraud;
	class RasTexturedGouraud;
	struct SRenderContext;
	struct SScanLineData;
	struct SVertex;
	struct SFragment;
	struct STexture;
	struct SMaterial;
	struct MyJobParam;
	template<class T> class CFThreadPool;
}

namespace Ext
{
	class MeshLoader;
	class OgreMeshLoader;
	class ObjMeshLoader;
	class Profiler;
}

//������Ŀֱ��ȡ�õ�ȫ�ֱ���
struct SGlobalEnv 
{
	HWND					hwnd;					// �����ھ��
	SR::Renderer*			renderer;				// ��Ⱦ��
	Ext::OgreMeshLoader*	meshLoader;				// .mesh������
	Ext::ObjMeshLoader*		objLoader;				// .obj������
	Ext::Profiler*			profiler;				// ���������
	SR::CFThreadPool<void*>* jobMgr;	// �̳߳�	
};
extern SGlobalEnv	g_env;

#endif // Prerequiestity_h__