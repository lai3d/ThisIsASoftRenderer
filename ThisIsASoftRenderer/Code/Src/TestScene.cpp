#include "stdafx.h"
#include "Renderer.h"
#include "Scene.h"
#include "OgreMeshLoader.h"
#include "ObjMeshLoader.h"
#include "RayTracer.h"
#include "LightMapper.h"
#include <io.h>


#define ADD_TEST_SCENE($setupFunc, $enterFunc)					\
{																\
	SR::Scene::StrategyFunc setupFunc = $setupFunc;				\
	SR::Scene::StrategyFunc enterFunc = $enterFunc;				\
	SR::Scene* pScene = new SR::Scene(setupFunc, enterFunc);	\
	m_scenes.push_back(pScene);									\
}

void SetupTestScene1(SR::Scene* scene)
{
	SR::RenderObject* obj=  new SR::RenderObject;

	SR::SVertex v1, v2, v3;
	v1.pos = VEC4(-20, -15, 0, 1);
	v2.pos = VEC4(20, -15, 0, 1);
	v3.pos = VEC4(0, 15, 0, 1);

	v1.normal = VEC3::UNIT_Z;
	v2.normal = VEC3::UNIT_Z;
	v3.normal = VEC3::UNIT_Z;

	v1.uv = VEC2(0.0f, 1.0f);
	v2.uv = VEC2(1.0f, 1.0f);
	v3.uv = VEC2(0.5f, 0.0f);

	v1.color.SetAsInt(0xffff0000);
	v2.color.SetAsInt(0xff00ff00);
	v3.color.SetAsInt(0xff0000ff);

	obj->m_verts.push_back(v1);
	obj->m_verts.push_back(v2);
	obj->m_verts.push_back(v3);

	SR::SFace face(0,1,2);
	face.faceNormal = VEC3::UNIT_Z; 
	obj->m_faces.push_back(face);

	SR::SMaterial* mat = new SR::SMaterial;
	g_env.renderer->AddMaterial("MatTriangle", mat);
	obj->m_pMaterial = mat;
	obj->m_bStatic = true;
	obj->SetShader(SR::eRasterizeType_Gouraud);

	scene->AddRenderObject(obj);
}

void EnterTestScene1(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetPosition(VEC3(0,0,200));
	g_env.renderer->m_camera.SetMoveSpeed(3.0f);
	g_env.renderer->m_camera.SetDirection(VEC3::NEG_UNIT_Z);
}

void SetupTestScene2(SR::Scene* scene)
{
	SR::RenderObject* obj = new SR::RenderObject;

	SR::SVertex v1, v2, v3 ,v4;
	v1.pos = VEC4(-500, 0, -500, 1);
	v2.pos = VEC4(500, 0, -500, 1);
	v3.pos = VEC4(-500, 0, 500, 1);
	v4.pos = VEC4(500, 0, 500, 1);

	v1.normal = VEC3::UNIT_Y;
	v2.normal = VEC3::UNIT_Y;
	v3.normal = VEC3::UNIT_Y;
	v4.normal = VEC3::UNIT_Y;

	v1.uv = VEC2(0, 0);
	v2.uv = VEC2(10, 0);
	v3.uv = VEC2(0, 10);
	v4.uv = VEC2(10, 10);

	obj->m_verts.push_back(v1);
	obj->m_verts.push_back(v2);
	obj->m_verts.push_back(v3);
	obj->m_verts.push_back(v4);

	SR::SFace face1(0,2,1);
	SR::SFace face2(1,2,3);
	face1.faceNormal = VEC3::UNIT_Y; 
	face2.faceNormal = VEC3::UNIT_Y; 
	obj->m_faces.push_back(face1);
	obj->m_faces.push_back(face2);

	SR::SMaterial* mat = new SR::SMaterial;
	mat->pDiffuseMap = new SR::STexture;
	mat->pDiffuseMap->LoadTexture(GetResPath("ChesePanel.bmp"), false);
	g_env.renderer->AddMaterial("MatChese", mat);
	obj->m_pMaterial = mat;
	obj->m_bStatic = true;
	obj->SetShader(SR::eRasterizeType_TexturedGouraud);

	scene->AddRenderObject(obj);
}

void EnterTestScene2(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetFarClip(10000);
	g_env.renderer->m_camera.SetPosition(VEC3(-20,706,1706));
	g_env.renderer->m_camera.SetMoveSpeed(5.0f);
	g_env.renderer->m_camera.SetDirection(VEC3(0,-1,-2));
}

void SetupTestScene3(SR::Scene* scene)
{
	SR::RenderObject* obj = new SR::RenderObject;

	// Construct terrain likely grids
	const int vertsPerSide = 11;
	const float dimension = 1000;
	const float halfDim = dimension / 2;
	const float cellSpace = dimension / (vertsPerSide - 1.0f);
	obj->m_verts.resize(vertsPerSide * vertsPerSide);
	float posZ = halfDim;

	// Wrap uv
	const int uvTileCnt = 10;
	float uvInc = cellSpace / dimension * uvTileCnt;

	for (int z=0; z<vertsPerSide; ++z)
	{
		float posX = -halfDim;
		posZ -= cellSpace;
		for (int x=0; x<vertsPerSide; ++x)
		{
			posX += cellSpace;
			int idx = z*vertsPerSide+x;
			obj->m_verts[idx].pos = VEC4(posX, 0, posZ, 1);
			obj->m_verts[idx].uv = VEC2(x*uvInc, z*uvInc);
			obj->m_verts[idx].normal = VEC3::UNIT_Y;
		}
	}

	// Index buffer
	int nFaces = (vertsPerSide - 1) * (vertsPerSide - 1) * 2;
	int iFace = 0;
	obj->m_faces.resize(nFaces);
	for (int z=0; z<vertsPerSide-1; ++z)
	{
		for (int x=0; x<vertsPerSide-1; ++x)
		{
			obj->m_faces[iFace].index1 = z * vertsPerSide + x;
			obj->m_faces[iFace].index2 = (z + 1) * vertsPerSide + x;
			obj->m_faces[iFace].index3 = z * vertsPerSide + x + 1;
			obj->m_faces[iFace+1].index1 = z * vertsPerSide + x + 1;
			obj->m_faces[iFace+1].index2 = (z + 1) * vertsPerSide + x;
			obj->m_faces[iFace+1].index3 = (z + 1) * vertsPerSide + x + 1;

			obj->m_faces[iFace].faceNormal = VEC3::UNIT_Y;
			obj->m_faces[iFace+1].faceNormal = VEC3::UNIT_Y;

			iFace += 2;
		}
	}

	SR::SMaterial* mat = new SR::SMaterial;
	mat->pDiffuseMap = new SR::STexture;
	mat->pDiffuseMap->LoadTexture(GetResPath("grid.bmp"), true);
	mat->pDiffuseMap->lodBias = 2;
	g_env.renderer->AddMaterial("MatGrid", mat);
	obj->m_pMaterial = mat;
	obj->m_bStatic = true;
	obj->SetShader(SR::eRasterizeType_TexturedGouraud);	

	scene->AddRenderObject(obj);
}

void EnterTestScene3(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetFarClip(10000);
	g_env.renderer->m_camera.SetPosition(VEC3(-20,300,700));
	g_env.renderer->m_camera.SetMoveSpeed(10.0f);
	g_env.renderer->m_camera.SetDirection(VEC3(0,-1,-2));
}

void SetupTestScene4(SR::Scene* scene)
{
	try
	{
		if(!g_env.meshLoader->LoadMeshFile(GetResPath("marine.mesh.xml"), true, true))
			throw std::logic_error("Error, Load .mesh file failed!");

		SR::SMaterial* mat = new SR::SMaterial;
		mat->pDiffuseMap = new SR::STexture;
		mat->pDiffuseMap->LoadTexture(GetResPath("marine_diffuse_blood.bmp"), false);
		mat->bUseHalfLambert = true;
		mat->bUseBilinearSampler = true;
		mat->ambient.Set(0.5f, 0.5f, 0.5f);
		mat->diffuse.Set(0.8f, 0.8f, 0.8f);
		mat->specular.Set(0.3f, 0.3f, 0.3f);

		g_env.renderer->AddMaterial("MatMarine", mat);
		g_env.meshLoader->m_objs[0]->m_pMaterial = mat;
		g_env.meshLoader->m_objs[0]->SetShader(SR::eRasterizeType_TexturedGouraud);
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return;
	}

	scene->AddRenderObject(g_env.meshLoader->m_objs[0]);
}

void EnterTestScene4(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetPosition(VEC3(0,0,10));
	g_env.renderer->m_camera.SetMoveSpeed(0.1f);
	g_env.renderer->m_camera.SetDirection(VEC3::NEG_UNIT_Z);
}

void SetupTestScene5(SR::Scene* scene)
{
	try
	{
		if(!g_env.meshLoader->LoadMeshFile(GetResPath("teapot.mesh.xml"), true, true))
			throw std::logic_error("Error, Load .mesh file failed!");

		SR::SMaterial* mat = new SR::SMaterial;
		mat->ambient.Set(0.3f, 0.3f, 0.3f);
		mat->diffuse.Set(0.5f, 0.5f, 0.5f);
		mat->specular.Set(0.3f, 0.3f, 0.3f);
		mat->shiness = 50;

		g_env.renderer->AddMaterial("MatTeapot", mat);
		g_env.meshLoader->m_objs[0]->m_pMaterial = mat;
		g_env.meshLoader->m_objs[0]->SetShader(SR::eRasterizeType_BlinnPhong);
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return;
	}

	scene->AddRenderObject(g_env.meshLoader->m_objs[0]);
}

void EnterTestScene5(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetPosition(VEC3(0,0,200));
	g_env.renderer->m_camera.SetMoveSpeed(3.0f);
	g_env.renderer->m_camera.SetDirection(VEC3::NEG_UNIT_Z);
}

void SetupTestScene6(SR::Scene* scene)
{
	try
	{
		if(!g_env.meshLoader->LoadMeshFile(GetResPath("teapot.mesh.xml"), true, true))
			throw std::logic_error("Error, Load .mesh file failed!");

		SR::SMaterial* mat = new SR::SMaterial;
		mat->pDiffuseMap = new SR::STexture;
		mat->pDiffuseMap->LoadTexture(GetResPath("RustedMetal.bmp"), false);
		mat->bUseBilinearSampler = true;

		mat->pNormalMap = new SR::STexture;
		mat->pNormalMap->LoadTexture(GetResPath("NormalMap.bmp"), false);

		mat->ambient.Set(0.3f, 0.3f, 0.3f);
		mat->diffuse.Set(0.5f, 0.5f, 0.5f);
		mat->specular.Set(0.9f, 0.9f, 0.9f);
		mat->shiness = 50;

		g_env.renderer->AddMaterial("MatNormalMap", mat);
		g_env.meshLoader->m_objs[0]->m_pMaterial = mat;
		g_env.meshLoader->m_objs[0]->BuildTangentVectors();
		g_env.meshLoader->m_objs[0]->SetShader(SR::eRasterizeType_NormalMap);
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return;
	}

	scene->AddRenderObject(g_env.meshLoader->m_objs[0]);
}

void EnterTestScene6(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetPosition(VEC3(0,0,200));
	g_env.renderer->m_camera.SetMoveSpeed(3.0f);
	g_env.renderer->m_camera.SetDirection(VEC3::NEG_UNIT_Z);
}

void SetupTestScene7(SR::Scene* scene)
{
	try
	{
		// Teapot 1
		{
			if(!g_env.meshLoader->LoadMeshFile(GetResPath("teapot.mesh.xml"), true, true))
				throw std::logic_error("Error, Load .mesh file failed!");

			SR::SMaterial* mat = new SR::SMaterial;
			mat->bTwoSide = true;
			mat->bTransparent = true;
			mat->transparency = 0.7f;

			mat->pDiffuseMap = new SR::STexture;
			mat->pDiffuseMap->LoadTexture(GetResPath("Hex.bmp"), false, true);

			mat->ambient.Set(0.4f, 0.0f, 0.0f);
			mat->diffuse.Set(0.7f, 0.0f, 0.0f);
			mat->specular.Set(0.3f, 0.0f, 0.0f);
			mat->shiness = 50;

			SR::RenderObject* pTeapot = g_env.meshLoader->m_objs[0];
			pTeapot->m_matWorld.FromAxisAngle(VEC3::UNIT_Y, -30.0f);
			pTeapot->m_matWorld.SetTranslation(VEC4(10.0f, 10.0f, -15.0f, 1.0f));
			pTeapot->m_pMaterial = mat;
			pTeapot->SetShader(SR::eRasterizeType_BlinnPhong);

			scene->AddRenderObject(pTeapot);
		}
		// Teapot 2
		{
			if(!g_env.meshLoader->LoadMeshFile(GetResPath("teapot.mesh.xml"), true, true))
				throw std::logic_error("Error, Load .mesh file failed!");

			SR::SMaterial* mat = new SR::SMaterial;
			mat->bTwoSide = true;
			mat->bTransparent = true;
			mat->transparency = 0.7f;

			mat->pDiffuseMap = new SR::STexture;
			mat->pDiffuseMap->LoadTexture(GetResPath("Hex.bmp"), false, true);

			mat->ambient.Set(0.0f, 0.4f, 0.0f);
			mat->diffuse.Set(0.0f, 0.7f, 0.0f);
			mat->specular.Set(0.0f, 0.3f, 0.0f);
			mat->shiness = 50;

			SR::RenderObject* pTeapot = g_env.meshLoader->m_objs[0];
			pTeapot->m_pMaterial = mat;
			pTeapot->SetShader(SR::eRasterizeType_BlinnPhong);

			scene->AddRenderObject(pTeapot);
		}
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return;
	}
}

void EnterTestScene7(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetPosition(VEC3(0,0,200));
	g_env.renderer->m_camera.SetMoveSpeed(3.0f);
	g_env.renderer->m_camera.SetDirection(VEC3::NEG_UNIT_Z);
}

void SetupTestScene8(SR::Scene* scene)
{
	try
	{
		if(!g_env.objLoader->LoadMeshFile(GetResPath("Sponza\\sponza.obj"), true))
			throw std::logic_error("Error, Load .obj file failed!");
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return;
	}

	std::for_each(g_env.objLoader->m_objs.begin(), g_env.objLoader->m_objs.end(), [&](SR::RenderObject* obj)
	{
		obj->m_pMaterial->bUseBilinearSampler = true;
		scene->AddRenderObject(obj);
	});
}

void EnterTestScene8(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetPosition(VEC3(-1.8f, 6.6f, -4.7f));
	g_env.renderer->m_camera.SetMoveSpeed(2.0f);
	g_env.renderer->m_camera.SetDirection(VEC3::NEG_UNIT_Z);
}

void SetupTestScene9(SR::Scene* scene)
{
// 	// Shadow caster
// 	SR::RayTrace_Box* box1 = new SR::RayTrace_Box(VEC3(-40,5,-20), VEC3(40,30,20));
// 	// Shadow receiver
// 	SR::RayTrace_Box* box2 = new SR::RayTrace_Box(VEC3(-200,-1,-200), VEC3(200,1,200));
// 
// 	box1->m_bCastShadow = true;
// 	box2->m_bCastShadow = false;
// 
// 	box1->m_color = SR::SColor::RED;
// 	box2->m_color = SR::SColor::YELLOW;
// 
// 	scene->AddRayTraceObject(box1);
// 	scene->AddRayTraceObject(box2);
// 	scene->EnableRayTracing(true);
// 
// 	SR::SPointLight* pLight = g_env.renderer->GetRayTracer()->m_pLight;
// 	pLight->pos = VEC3(-90, 121, -65);
// 	pLight->color = SR::SColor::WHITE;

	bool bOk = g_env.meshLoader->LoadMeshFile(GetResPath("RayTrace\\Box01.mesh.xml"), true, false);
	assert(bOk);

	SR::RayTraceRenderable* box = g_env.meshLoader->m_objs[0];
	box->m_bCastShadow = false;
	box->m_color = SR::SColor::YELLOW;

	scene->AddRayTraceObject(box);
	scene->EnableRayTracing(true);

	SR::SPointLight* pLight = g_env.renderer->GetRayTracer()->m_pLight;
	pLight->pos = VEC3(-90, 121, -65);
	pLight->color = SR::SColor::WHITE;
}

void EnterTestScene9(SR::Scene* scene)
{
// 	g_env.renderer->m_camera.SetPosition(VEC3(-93,165,28));
// 	g_env.renderer->m_camera.SetMoveSpeed(3.0f);
// 	g_env.renderer->m_camera.SetDirection(VEC3(93,-165,-28));
// 	g_env.renderer->m_camera.SetNearClip(1.0f);
	g_env.renderer->m_camera.SetPosition(VEC3(12.865f, -31.042f, -2.012f));
	g_env.renderer->m_camera.SetMoveSpeed(5.0f);
	g_env.renderer->m_camera.SetDirection(VEC3(0,0,1));
	g_env.renderer->m_camera.SetNearClip(1.0f);
}


void SetupTestScene10(SR::Scene* scene)
{
	try
	{
		// Load or auto gen light map
		SR::LightMapper lightMapGen;
		lightMapGen.m_pLight->pos = VEC3(-2.65f, 5.41f, 10.75f);
		lightMapGen.m_pLight->color = SR::SColor::WHITE;

		SR::STexture* pLightMap = nullptr;

		const STRING lightmapName[2] = 
		{ 
			"../../../Res/LightMap/lightmap1.bmp",
			"../../../Res/LightMap/lightmap2.bmp"
		};

		const STRING meshName[2] = 
		{
			"../../../Res/LightMap/Box01.mesh.xml",
			"../../../Res/LightMap/Plane01.mesh.xml"
		};

		for (int i=0; i<2; ++i)
		{
			SR::RenderObject* obj = nullptr;
			SR::SMaterial* mat = new SR::SMaterial;

			if (_access(lightmapName[i].c_str(), 0) == -1)
			{
				if(!g_env.meshLoader->LoadMeshFile(meshName[i], true, false))
					throw std::logic_error("Error, Load .mesh file failed!");

				obj = g_env.meshLoader->m_objs[0];
				obj->m_pMaterial = mat;
				scene->AddRenderObject(obj);

				pLightMap = lightMapGen.GenerateLightMap(128, 128, obj, scene->m_renderList_solid);
				pLightMap->texData[0]->SaveToFile(lightmapName[i]);

				mat->pDiffuseMap = pLightMap;
			}
			else
			{
				if(!g_env.meshLoader->LoadMeshFile(meshName[i], true, true))
					throw std::logic_error("Error, Load .mesh file failed!");

				obj = g_env.meshLoader->m_objs[0];
				obj->m_pMaterial = mat;
				scene->AddRenderObject(obj);

				mat->pDiffuseMap = new SR::STexture;
				mat->pDiffuseMap->LoadTexture(lightmapName[i], false);
			}

			obj->m_bStatic = true;
			obj->SetShader(SR::eRasterizeType_LightMap);
		}
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return;
	}
}

void EnterTestScene10(SR::Scene* scene)
{
	g_env.renderer->m_camera.SetPosition(VEC3(-2.65f, 4.41f, 10.75f));
	g_env.renderer->m_camera.SetMoveSpeed(0.3f);
	g_env.renderer->m_camera.SetDirection(VEC3::NEG_UNIT_Z);
}

namespace SR
{
	void Renderer::_InitAllScene()
	{
		//// Test Scene 1: Triangle with gouraud shading mode
// 		ADD_TEST_SCENE(SetupTestScene1, EnterTestScene1);
// 
// 		//// Test Scene 2: 透视校正纹理映射
// 		ADD_TEST_SCENE(SetupTestScene2, EnterTestScene2);
// 
// 		//// Test Scene 3: 纹理mip-mapping
// 		ADD_TEST_SCENE(SetupTestScene3, EnterTestScene3);
// 
// 		//// Test Scene 4: marine.mesh
// 		ADD_TEST_SCENE(SetupTestScene4, EnterTestScene4);
// 
// 		//// Test Scene 5: teapot.mesh + Phong模型
// 		ADD_TEST_SCENE(SetupTestScene5, EnterTestScene5);
// 
// 		//// Test Scene 6: Normal Map
// 		ADD_TEST_SCENE(SetupTestScene6, EnterTestScene6);
// 
// 		//// Test Scene 7: OIT
// 		ADD_TEST_SCENE(SetupTestScene7, EnterTestScene7);
// 
// 		//// Test Scene 8: sponza.obj
// 		ADD_TEST_SCENE(SetupTestScene8, EnterTestScene8);

		//// Test Scene 9: Ray tracing
		ADD_TEST_SCENE(SetupTestScene9, EnterTestScene9);

		//// Test Scene 10: Lighting mapping
		ADD_TEST_SCENE(SetupTestScene10, EnterTestScene10);
	}
}

