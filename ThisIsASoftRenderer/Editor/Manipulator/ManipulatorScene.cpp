#include "stdafx.h"
#include "ManipulatorScene.h"
#include "../EditorDefine.h"
#include "EditorUtility.h"
#include "Scene.h"

#include <Profiler.h>


ManipulatorScene::ManipulatorScene()
:m_sceneName(L"")
,m_bIsSceneReay(false)
,m_pCurScene(nullptr)
{
	m_pRenderer = g_env.renderer;
}

ManipulatorScene::~ManipulatorScene()
{
	
}
void ManipulatorScene::Init()
{
// 	Ogre::StringVectorPtr loc = Ogre::ResourceGroupManager::getSingleton().findResourceLocation("Scene", "*Scene");
// 	assert(!loc->empty());
// 	m_scenePath = Utility::EngineToUnicode(loc->at(0));
// 	m_scenePath += L"\\";

	m_manipulatorObject = new ManipulatorObject;	// Goes first!
	m_manipulatorCamera = new ManipulatorCamera;
	m_manipulatorResource = new ManipulatorResource;
	m_manipualtorOp		= new ManipulatorOperation;

	m_pRenderer->ToggleScene();
	m_pCurScene = m_pRenderer->GetCurScene();

	m_bIsSceneReay = true;
}

void ManipulatorScene::Shutdown()
{
	SAFE_DELETE(m_manipulatorObject);
	SAFE_DELETE(m_manipulatorCamera);
	SAFE_DELETE(m_manipulatorResource);
	SAFE_DELETE(m_manipualtorOp);
}

void ManipulatorScene::SceneNew(const std::wstring& sceneName)
{
// 	m_pCurScene->New(Utility::UnicodeToEngine(sceneName));
// 
// 	m_sceneName = sceneName;
// 	m_bIsSceneReay = true;
// 
// 	//回调事件
// 	Excute([](ManipulatorSceneEventCallback* callback){ callback->OnSceneNew(); });
// 
// 	OnGizmoNodeReset();
}

void ManipulatorScene::SceneOpen(const std::wstring& filepath)
{
// 	String fullpath(Utility::UnicodeToEngine(filepath)), path, basename, extname;
// 	Ogre::StringUtil::splitFullFilename(fullpath, basename, extname, path);
// 
// 	m_sceneName = Utility::EngineToUnicode(basename);
// 	
// 	m_pCurScene->Load(fullpath, this);
// 
// 	m_bIsSceneReay = true;
// 
// 	//回调事件
// 	Excute([](ManipulatorSceneEventCallback* callback){ callback->OnSceneOpen(); });
// 
// 	OnGizmoNodeReset();
}

void ManipulatorScene::SceneSave()
{
// 	Ogre::String fullPath = Utility::UnicodeToEngine(m_scenePath+m_sceneName);
// 	//创建场景文件夹
// 	::CreateDirectoryA(fullPath.c_str(), NULL);
// 
// 	fullPath += "\\" + Utility::UnicodeToEngine(m_sceneName) + ".scene";
// 	m_pCurScene->Save(fullPath, this);
// 
// 	m_manipulatorGameData->SaveAllXml();
}

void ManipulatorScene::SceneClose()
{
// 	//回调事件
// 	Excute([](ManipulatorSceneEventCallback* callback){ callback->OnSceneClose(); });
// 
// 	m_pCurScene->Reset();
// 	m_bIsSceneReay = false;
}

const std::wstring ManipulatorScene::GenerateSceneFullPath()
{
	assert(!m_scenePath.empty() && !m_sceneName.empty());

	std::wstring fullPath(m_scenePath);
	fullPath += m_sceneName;
	fullPath += L"\\";

	return fullPath;
}

void ManipulatorScene::OnGizmoNodeReset()
{
	m_manipulatorObject->OnGizmoNodeReset();
}

void ManipulatorScene::OnFrameMove( float dt )
{
	m_manipulatorObject->OnFrameMove(dt);

	SR::Renderer* pRenderer = g_env.renderer;
	pRenderer->OnFrameMove();
	pRenderer->RenderOneFrame();
	g_env.profiler->DisplayHelpInfo();
	//swap!!
	pRenderer->Present();
}

void ManipulatorScene::_LoadObjects( rapidxml::xml_node<>* node )
{
	m_manipulatorObject->Load(node);
}

void ManipulatorScene::_SaveObjects( rapidxml::xml_document<>* doc, rapidxml::xml_node<>* XMLNode )
{
	m_manipulatorObject->Serialize(doc, XMLNode);
}


