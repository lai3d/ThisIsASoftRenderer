/********************************************************************
	created:	21:7:2013   17:54
	filename	ObjMeshLoader.h
	author:		maval

	purpose:	.obj .mtl 加载器.
				艹,obj格式太恶心了,不便利的感觉
*********************************************************************/

#ifndef ObjMeshLoader_h__
#define ObjMeshLoader_h__

#include "MeshLoaderBase.h"

namespace Ext
{
	class ObjMeshLoader : public MeshLoader
	{
	public:
		struct SVertCompare 
		{
			bool operator== (const SVertCompare& rhs) { return memcmp(this, &rhs, sizeof(SVertCompare)) == 0; }

			int idxPos, idxUv, idxNormal;
		};

	protected:
		virtual bool LoadImpl(const STRING& filename);

	private:
		void	_PreReadObject(std::ifstream& file, DWORD& nVert, DWORD& nUv, DWORD& nNormal, DWORD& nFace);
		void	_DefineVertex(const SR::SVertex& vert, const SVertCompare& comp, SR::SRenderObj& obj, SR::Index& retIdx);

		std::vector<SVertCompare>	m_vecComp;
	};
}

#endif // ObjMeshLoader_h__
