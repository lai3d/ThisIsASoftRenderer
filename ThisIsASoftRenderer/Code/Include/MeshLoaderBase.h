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

		//��2��������˵��Ҫ���ص�ģ���Ƿ��ڳ������Ǿ�̬��(���������ʼ��Ϊ��λ����)
		//������һ���Լ���������Χ��,�����Ժ�ÿ֡����
		bool	LoadMeshFile(const STRING& filename, bool bStatic);

		SR::RenderList	m_objs;

	protected:
		virtual bool LoadImpl(const STRING& filename) = 0;
	};
}

#endif // MeshLoaderBase_h__