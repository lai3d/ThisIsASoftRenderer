/********************************************************************
	created:	14:12:2012   1:24
	filename: 	ManipulatorScene.h
	author:		maval
	
	purpose:	编辑器场景管理
*********************************************************************/
#ifndef ManipulatorScene_h__
#define ManipulatorScene_h__


#include "Singleton.h"
#include "ManipulatorObject.h"
#include "ManipulatorEventCallback.h"
#include "ManipulatorCamera.h"
#include "ManipulatorEditorResource.h"
//#include "SceneSerializer.h"
#include "ManipulatorOperation.h"

//----------------------------------------------------------------------------------------
class ManipulatorScene : 
	public Common::CSingleton<ManipulatorScene>, 
	public EventCallbackManager<ManipulatorSceneEventCallback>
	//private Kratos::SceneSerializer
{
	ManipulatorScene();
	~ManipulatorScene();
	DECLEAR_SINGLETON(ManipulatorScene);

protected:
	///////////////////重写SceneSerializer方法
	virtual void	_LoadObjects(rapidxml::xml_node<>* node);
	virtual void	_SaveObjects(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* XMLNode);

public:
	SR::Scene*	GetScene() { return m_pCurScene; }

	void	SceneNew(const std::wstring& sceneName);
	void	SceneOpen(const std::wstring& filepath);
	void	SceneSave();
	void	SceneClose();

	bool	GetIsSceneReady()	{ return m_bIsSceneReay; }
	//重新绑定SceneNode(比如切换了场景)
	void	OnGizmoNodeReset();

	void	Init();
	void	OnFrameMove(float dt);
	void	Shutdown();
	const std::wstring&	GetScenePath()	{ return m_scenePath; }
	const std::wstring&	GetSceneName()	{ return m_sceneName; }
	//包括场景文件夹的名字(带\\)
	const std::wstring	GenerateSceneFullPath();

	ManipulatorObject&		GetObject()		{ return *m_manipulatorObject;	}
	ManipulatorCamera&		GetCamera()		{ return *m_manipulatorCamera;	}
	ManipulatorResource&	GetResource()	{ return *m_manipulatorResource; }
	ManipulatorOperation&	GetOperation()	{ return *m_manipualtorOp;		}

private:
	SR::Renderer*		m_pRenderer;
	std::wstring		m_scenePath;
	std::wstring		m_sceneName;
	SR::Scene*			m_pCurScene;
	bool				m_bIsSceneReay;
	ManipulatorObject*	m_manipulatorObject;
	ManipulatorCamera*	m_manipulatorCamera;
	ManipulatorResource* m_manipulatorResource;
	ManipulatorOperation* m_manipualtorOp;
};

#define	ManipulatorSystem	ManipulatorScene::GetSingleton()

#endif // ManipulatorScene_h__







