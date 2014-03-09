/********************************************************************
	created:	2013/07/22
	created:	22:7:2013   17:16
	filename: 	MeshLoaderBase.h
	author:		maval
	
	purpose:	ģ�ͼ���������
*********************************************************************/
#ifndef MeshLoaderBase_h__
#define MeshLoaderBase_h__

#include "Prerequiestity.h"
#include "RenderObject.h"

namespace Ext
{
	class MeshLoader
	{
	public:
		virtual ~MeshLoader() {}

		/** param 
			bStatic: Ҫ���ص�ģ���Ƿ��ڳ������Ǿ�̬��(���������ʼ��Ϊ��λ����),������һ���Լ���������Χ��,�����Ժ�ÿ֡����
			bFlipUV: �Ƿ�ת��������V,����.bmp����
		**/
		bool	LoadMeshFile(const STRING& filename, bool bStatic, bool bFlipUV = false);

		SR::RenderList	m_objs;

	protected:
		virtual bool LoadImpl(const STRING& filename, bool bFlipUV) = 0;
	};
}

#endif // MeshLoaderBase_h__