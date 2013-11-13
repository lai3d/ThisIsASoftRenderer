/********************************************************************
	created:	2013/07/23
	created:	23:7:2013   10:23
	filename: 	Prerequiestity.h
	author:		maval
	
	purpose:	��Ŀǰ���ļ�
*********************************************************************/
#ifndef Prerequiestity_h__
#define Prerequiestity_h__

const int		SCREEN_WIDTH	=	640;
const int		SCREEN_HEIGHT	=	480;
const int		PIXEL_MODE		=	4;		//����ģʽ(�ֽ���). NB:ֻ֧��32λģʽ,��Ҫ����
// clipping rectangle 
const int		min_clip_x		=	0;                          
const int		max_clip_x		=	(SCREEN_WIDTH-1);
const int		min_clip_y		=	0;
const int		max_clip_y		=	(SCREEN_HEIGHT-1);

#define USE_32BIT_INDEX			0			//�Ƿ�ʹ��32λ��������
#define USE_PERSPEC_CORRECT		1			//�Ƿ�ʹ��͸������
#define USE_PROFILER			1			//�Ƿ�ʹ��Profiler
#define USE_MULTI_THREAD		0			//�Ƿ�ʹ�ö��߳�
#define USE_SIMD				1			//�Ƿ�ʹ��SIMD


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
	struct SScanLinesData;
	struct SScanLine;
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