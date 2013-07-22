#include "stdafx.h"
#include "ObjMeshLoader.h"
#include <fstream>

using namespace std;

namespace Ext
{
	bool ObjMeshLoader::LoadImpl( const STRING& filename )
	{
		std::ifstream file(filename.c_str());
		if(file.fail())
			return false;

		m_objs.clear();

		//.obj��ʽÿ�������f��������Ȼ�Ǵ�ǰ�������������ۼӵ�.
		//ܳ,��Щ����������ȥ�ۼӲ���,̫�鷳��
		DWORD nTotalPosCount, nTotalUvCount, nTotalNormalCount;
		nTotalPosCount = nTotalUvCount = nTotalNormalCount = 0;

		//each object
		for (;;)
		{
			if(file.eof())
				break;

			//����һ��ֱ�ӿ�����..ܳ
			if(m_objs.size() > 10)
				break;

			m_objs.push_back(SR::SRenderObj());
			SR::SRenderObj& obj = m_objs.back();

			//��ȡ������,���ⷴ������洢�ռ�
			DWORD nPos, nUv, nNormal, nFace;
			nPos = nUv = nNormal = nFace = 0;
			_PreReadObject(file, nPos, nUv, nNormal, nFace);

			vector<VEC4> vecPos(nPos);
			vector<VEC2> vecUv(nUv);
			vector<VEC3> vecNormal(nNormal);

			obj.VB.reserve(nFace*3);
			obj.faces.resize(nFace);
			m_vecComp.clear();
			m_vecComp.reserve(nFace*3);

			//��ʽ��ȡ����
			DWORD curVert, curUv, curNormal, curFace;
			curVert = curUv = curNormal = curFace = 0;
			STRING command;
			bool bFlush = false;

			//each command
			for(;;)
			{
				if(file.eof())
					break;

				file >> command;

				//TODO: ��һ��table+switch-case��첻��
				if (strcmp(command.c_str(), "v") == 0)
				{
					if(bFlush)
					{
						//������һ������
						file.seekg(-1, ios_base::cur);
						break;
					}

					VEC4& pos = vecPos[curVert++];
					file >> pos.x >> pos.y >> pos.z;
					pos.w = 1;
				}
				else if (strcmp(command.c_str(), "vt") == 0)
				{
					VEC2& uv = vecUv[curUv++];
					file >> uv.x >> uv.y;
				}
				else if (strcmp(command.c_str(), "vn") == 0)
				{
					VEC3& normal = vecNormal[curNormal++];
					file >> normal.x >> normal.y >> normal.z;
				}
				else if (strcmp(command.c_str(), "f") == 0)
				{
					DWORD idxPos[3] = {0};
					DWORD idxUv[3] = {0};
					DWORD idxNormal[3] = {0};
					SR::Index idxVert[3] = {0};

					for (int i=0; i<3; ++i)
					{
						file >> idxPos[i];
						file.ignore(10, '/');
						file >> idxUv[i];
						file.ignore(10, '/');
						file >> idxNormal[i];

						//.obj�����Ǵ�1��ʼ��
						idxPos[i] -= 1; idxUv[i] -= 1; idxNormal[i] -= 1;
						//����ǰ����˵,��ȥ�ۼӵĲ���
						idxPos[i] -= nTotalPosCount;
						idxUv[i] -= nTotalUvCount;
						idxNormal[i] -= nTotalNormalCount;

						SR::SVertex vertex;
						vertex.pos		= vecPos[idxPos[i]];
						vertex.uv		= vecUv[idxUv[i]];
						vertex.normal	= vecNormal[idxNormal[i]];

						SVertCompare comp = { idxPos[i], idxUv[i], idxNormal[i] };

						_DefineVertex(vertex, comp, obj, idxVert[i]);
					}

					SR::SFace& face = obj.faces[curFace++];
					face.index1 = idxVert[0]; face.index2 = idxVert[1]; face.index3 = idxVert[2];
				}
				else if (strcmp(command.c_str(), "g") == 0)
				{
					bFlush = true;
				}
				// 			else if (strcmp(command.c_str(), "matlib") == 0)
				// 			{
				// 
				// 			}
				// 			else if (strcmp(command.c_str(), "usemtl") == 0)
				// 			{
				// 			}

				//����һ��
				file.ignore(1000, '\n');
			}

			nTotalPosCount += nPos;
			nTotalUvCount += nUv;
			nTotalNormalCount += nNormal;
		}
				
		return true;
	}

	void ObjMeshLoader::_PreReadObject( std::ifstream& file, DWORD& nPos, DWORD& nUv, DWORD& nNormal, DWORD& nFace )
	{
		//��¼�µ�ǰλ��,���ص�ʱ����˵���λ��
		streamoff pos = file.tellg();

		bool bFlush = false;
		STRING command;
		while (1)
		{
			if(file.eof())
			{
				//NB: ����ĩβ����Ҫ���״̬�ٻ���
				file.clear();
				file.seekg(pos, ios_base::beg);
				return;
			}

			file >> command;

			if (strcmp(command.c_str(), "v") == 0)
			{
				if(bFlush)
				{
					file.seekg(pos, ios_base::beg);
					return;
				}
				++nPos;
			}
			else if (strcmp(command.c_str(), "vt") == 0)
			{
				++nUv;
			}
			else if (strcmp(command.c_str(), "vn") == 0)
			{
				++nNormal;
			}
			else if (strcmp(command.c_str(), "f") == 0)
			{
				++nFace;
			}
			else if (strcmp(command.c_str(), "g") == 0)
			{
				bFlush = true;
			}

			//����һ��
			file.ignore(1000, '\n');
		}
	}

	void ObjMeshLoader::_DefineVertex( const SR::SVertex& vert, const SVertCompare& comp, SR::SRenderObj& obj, SR::Index& retIdx )
	{
		//.obj������Ķ��岻��ֱ�Ӹ���������,���Ǹ��ɷֶ����������.���Թ���VBҪ�鷳Щ.
		//1. �ڵ�ǰ�����б��в���,�Ƿ�����ͬ����
		auto iter = std::find(m_vecComp.begin(), m_vecComp.end(), comp);

		if(iter != m_vecComp.end())
		{
			//2. �����,����ظ�����
			retIdx = std::distance(m_vecComp.begin(), iter);
		}
		else
		{
			//3. ���û��,������¶��㵽ĩβ
			obj.VB.push_back(vert);
			m_vecComp.push_back(comp);
			retIdx = obj.VB.size() - 1;
		}
	}

}