/********************************************************************
	created:	30:9:2013   21:16
	filename	MainFrm.h
	author:		maval

	purpose:	MFC main frame window
*********************************************************************/
#ifndef MainFrm_h__
#define MainFrm_h__


#include "Manipulator/ManipulatorEventCallback.h"

class CEditorView;
class CPropertiesPane;
class PropertyPaneTerrain;
class PropertyPaneObject;
class PropertyPaneEffect;
class PropertyPaneParticle;
class PropertyPaneDLight;
class PropertyPaneEntityEffect;
class DialogBehaviorTreeEditor;
class DialogGameDataBuilding;

//----------------------------------------------------------------------------------------
class CMainFrame 
	: public CXTPFrameWnd
	, public ManipulatorSceneEventCallback
	, public ManipulatorObjectEventCallback
{
	
public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL	OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	/////////////////////////////////////////////////////////
	///事件回调
	virtual	void	OnSceneNew();
	virtual	void	OnSceneOpen();
	virtual void	OnSceneClose();

// 	virtual void	OnObjectSetSelection(Ogre::Entity* pObject);
// 	virtual void	OnObjectClearSelection(Ogre::Entity* pObject);
// 	virtual void	OnObjectPropertyChanged(Ogre::Entity*);
	
#ifdef _DEBUG
	virtual void	AssertValid() const;
	virtual void	Dump(CDumpContext& dc) const;
#endif

	//初始化编辑器界面
	bool			CreateEditorMainUI();
	//刷新Ribbon面板
	void			RefreshRibbonBar();

protected:
	DECLARE_DYNAMIC(CMainFrame)
	virtual BOOL	OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	afx_msg int		OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);

private:
	DECLARE_MESSAGE_MAP()
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSetFocus(CWnd *pOldWnd);
	afx_msg void	OnClose();

	void			_LoadIcon();
	bool			_OnCreateRibbon();
	void			_CreateDockPane();
	LRESULT			_AttachDockPane(WPARAM wParam, LPARAM lParam);

	///Object
	template<int mode> void OnObjectEdit();
	template<int mode> void	OnUpdateUI_ObjectEdit(CCmdUI* pCmdUI);
	void			OnUpdateUI_ObjectRemove(CCmdUI* pCmdUI);
	void			OnObjectRemove();

	///Effect
	void			OnUpdateUI_ShadowOnOff(CCmdUI* pCmdUI);
	void			OnUpdateUI_SSAOOnOff(CCmdUI* pCmdUI);
	void			OnUpdateUI_SharpenOnOff(CCmdUI* pCmdUI);
	void			OnUpdateUI_FXAAOnOff(CCmdUI* pCmdUI);
	void			OnShadowOnOff();
	void			OnSSAOOnOff();
	void			OnSharpenOnOff();
	void			OnFXAAOnOff();

private:
	CXTPDockingPaneManager	m_paneManager;
	CEditorView*			m_wndView;
	PropertyPaneObject*		m_propertyObject;
	PropertyPaneEffect*		m_propertyEffect;
	
	CXTPDockingPane*		m_paneObject;
	CXTPDockingPane*		m_paneEffect;
};

#endif // MainFrm_h__
