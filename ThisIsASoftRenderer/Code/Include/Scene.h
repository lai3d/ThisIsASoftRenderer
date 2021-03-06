/********************************************************************
	created:	6:8:2013   22:00
	filename	Scene.h
	author:		maval

	purpose:	渲染场景简单封装
*********************************************************************/
#ifndef Scene_h__
#define Scene_h__

#include "Prerequiestity.h"
#include "RenderObject.h"
#include "Intersection.h"

namespace SR
{
	class Scene
	{
	public:
		typedef std::function<void(Scene*)>	StrategyFunc;

	public:
		Scene(StrategyFunc& setupFunc, StrategyFunc& enterFunc);
		~Scene();

		RenderList	m_renderList_solid;		//非透明物体列表
		RenderList	m_renderList_trans;		//透明物体列表
		RayTraceRenderList	m_renderList_RayTrace;	//光线跟踪物体列表

	public:
		void	Enter();
		void	AddRenderObject(RenderObject* obj);
		void	AddRayTraceObject(RayTraceRenderable* obj);
		void	EnableRayTracing(bool bEnable) { m_bUseRayTrace = bEnable; }
		bool	IsEnableRayTracing() const	{ return m_bUseRayTrace; }

	private:
		StrategyFunc	m_setupFunc;
		StrategyFunc	m_enterFunc;
		bool			m_bSetup;
		bool			m_bUseRayTrace;
	};
}

#endif // Scene_h__