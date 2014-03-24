#include "stdafx.h"
#include "SceneEdit.h"
#include "MainFrm.h"
#include "View.h"
#include "EditorDefine.h"
#include "EditorUtility.h"
#include "Manipulator/ManipulatorScene.h"
#include "Manipulator/ManipulatorAction.h"
#include "Action/ActionBase.h"
#include "UI/ObjectPropertyPane.h"
#include "UI/EffectPropertyPane.h"

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CXTPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTPFrameWnd)
	ON_WM_CREATE()
	ON_XTP_CREATECONTROL()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, _AttachDockPane)
	ON_COMMAND(IDC_Object_Move, OnObjectEdit<ManipulatorObject::eEditMode_Move>)
	ON_COMMAND(IDC_Object_Rotate, OnObjectEdit<ManipulatorObject::eEditMode_Rotate>)
	ON_COMMAND(IDC_Object_Scale, OnObjectEdit<ManipulatorObject::eEditMode_Scale>)
	ON_COMMAND(IDC_Object_Select, OnObjectEdit<ManipulatorObject::eEditMode_Select>)
	ON_UPDATE_COMMAND_UI(IDC_Object_Move, OnUpdateUI_ObjectEdit<ManipulatorObject::eEditMode_Move>)
	ON_UPDATE_COMMAND_UI(IDC_Object_Rotate, OnUpdateUI_ObjectEdit<ManipulatorObject::eEditMode_Rotate>)
	ON_UPDATE_COMMAND_UI(IDC_Object_Scale, OnUpdateUI_ObjectEdit<ManipulatorObject::eEditMode_Scale>)
	ON_UPDATE_COMMAND_UI(IDC_Object_Select, OnUpdateUI_ObjectEdit<ManipulatorObject::eEditMode_Select>)
	ON_COMMAND(IDC_Effect_Shadow, OnShadowOnOff)
	ON_UPDATE_COMMAND_UI(IDC_Effect_Shadow, OnUpdateUI_ShadowOnOff)
	ON_COMMAND(IDC_Effect_SSAO, OnSSAOOnOff)
	ON_UPDATE_COMMAND_UI(IDC_Effect_SSAO, OnUpdateUI_SSAOOnOff)
	ON_UPDATE_COMMAND_UI(IDC_Object_Remove, OnUpdateUI_ObjectRemove)
	ON_COMMAND(IDC_Object_Remove, OnObjectRemove)
	ON_UPDATE_COMMAND_UI(IDC_Effect_Sharpen, OnUpdateUI_SharpenOnOff)
	ON_UPDATE_COMMAND_UI(IDC_Effect_FXAA, OnUpdateUI_FXAAOnOff)
	ON_COMMAND(IDC_Effect_Sharpen, OnSharpenOnOff)
	ON_COMMAND(IDC_Effect_FXAA, OnFXAAOnOff)
END_MESSAGE_MAP()


CMainFrame::CMainFrame()
:m_wndView(NULL)
,m_propertyObject(new PropertyPaneObject)
,m_propertyEffect(new PropertyPaneEffect)
,m_paneObject(NULL)
,m_paneEffect(NULL)
{
}

CMainFrame::~CMainFrame()
{
	SAFE_DELETE(m_wndView);
	SAFE_DELETE(m_propertyObject);
	SAFE_DELETE(m_propertyEffect);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	cs.cx = EDITOR_CLIENT_W;
	cs.cy = EDITOR_CLIENT_H;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CXTPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnClose()
{
	ManipulatorSystem.GetObject().RemoveCallback(this);
	ManipulatorScene::GetSingleton().RemoveCallback(this);

	KillTimer(0);

	CSceneEditApp* app = (CSceneEditApp*)AfxGetApp();
	app->m_app.Shutdown();

	CXTPFrameWnd::OnClose();
}

bool CMainFrame::_OnCreateRibbon()
{
	// Initialize the command bars
	if (!InitCommandBars())
		return false;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
		return false;

	CXTPPaintManager::SetTheme(xtpThemeRibbon);

	//关闭默认菜单
	::SetMenu(m_hWnd, nullptr);
	m_hMenuDefault = nullptr;
	m_dwMenuBarState = AFX_MBS_HIDDEN;
	
	//初始化图标
	_LoadIcon();

	///Ribbon
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pCommandBars->Add(_T("Ribbon"), xtpBarTop, RUNTIME_CLASS(CXTPRibbonBar));
	if (!pRibbonBar)
		return false;
	pRibbonBar->EnableDocking(0);
	//pRibbonBar->GetTabPaintManager()->m_bSelectOnButtonDown = FALSE;

	///创建系统按钮
	CXTPControlPopup* pControlFile = (CXTPControlPopup*)pRibbonBar->AddSystemButton();
	pControlFile->SetIconId(IDB_GEAR);

	CMenu sysMenu;
	sysMenu.LoadMenu(IDR_MAINFRAME);
	CXTPPopupBar* pCommandBar = new CXTPRibbonSystemPopupBar();
	pCommandBar->SetCommandBars(pCommandBars);
	pControlFile->SetCommandBar(pCommandBar);
	pCommandBar->InternalRelease();
	pCommandBar->LoadMenu(sysMenu.GetSubMenu(0));
	pCommandBar->SetIconSize(CSize(32, 32));

	///RibbonHome
	CXTPRibbonTab* pTab = pRibbonBar->AddTab(L"Home");

	//RibbonHome - GroupObject
	CXTPRibbonGroup* pGroup = pTab->AddGroup(L"Object");
	//RibbonHome - GroupObject - Select
	pGroup->Add(xtpControlButton, IDC_Object_Select);
	//RibbonHome - GroupObject - Move
	pGroup->Add(xtpControlButton, IDC_Object_Move);
	//RibbonHome - GroupObject - Rotate
	pGroup->Add(xtpControlButton, IDC_Object_Rotate);
	//RibbonHome - GroupObject - Scale
	pGroup->Add(xtpControlButton, IDC_Object_Scale);
	//RibbonHome - GroupObject - Rmove
	pGroup->Add(xtpControlButton, IDC_Object_Remove);

	///RibbonEffect
	pTab = pRibbonBar->AddTab(L"Effect");

	//RibbonEffect - GroupSwitch
	pGroup = pTab->AddGroup(L"Switch");
	//RibbonEffect - GroupSwitch - Shadow
	pGroup->Add(xtpControlButton, IDC_Effect_Shadow);
	//RibbonEffect - GroupSwitch - SSAO
	pGroup->Add(xtpControlButton, IDC_Effect_SSAO);
	//RibbonEffect - GroupSwitch - Sharpen
	pGroup->Add(xtpControlButton, IDC_Effect_Sharpen);
	//RibbonEffect - GroupSwitch - FXAA
	pGroup->Add(xtpControlButton, IDC_Effect_FXAA);
	
	return true;
}

void CMainFrame::_LoadIcon()
{
	CXTPImageManager* pImageMgr = GetCommandBars()->GetImageManager();

	UINT uiSystemMenu[] = { ID_FILE_NEW, ID_FILE_OPEN, ID_FILE_SAVE };	
	pImageMgr->SetIcons(IDB_SYSTEMMENULARGE, uiSystemMenu, _countof(uiSystemMenu), CSize(32, 32));

	UINT uCommand = {IDB_GEAR};				pImageMgr->SetIcons(IDB_GEAR, &uCommand, 1, CSize(0, 0), xtpImageNormal);
	UINT icon[1] = {IDC_Object_Move};		pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
	icon[0] = IDC_Object_Rotate;			pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
	icon[0] = IDC_Object_Scale;				pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
	icon[0] = IDC_Object_Select;			pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
	icon[0] = IDC_Object_Remove;			pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
	icon[0] = IDC_Effect_Shadow;			pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
	icon[0] = IDC_Effect_SSAO;				pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
	icon[0] = IDC_Effect_Sharpen;			pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
	icon[0] = IDC_Effect_FXAA;				pImageMgr->SetIcons(IDB_Button, icon, _countof(icon), CSize(32, 32));
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
	m_wndView = new CEditorView;
	if (!m_wndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, lpcs->cx, lpcs->cy), this, AFX_IDW_PANE_FIRST, NULL))
		return FALSE;
	SetActiveView(m_wndView);

	return TRUE;
}

void CMainFrame::_CreateDockPane()
{
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2010);
	m_paneManager.SetClientMargin(6);
	m_paneManager.SetThemedFloatingFrames(TRUE);
	m_paneManager.SetAlphaDockingContext(TRUE);
	m_paneManager.SetShowDockingContextStickers(TRUE);
	m_paneManager.SetShowContentsWhileDragging(TRUE);
	m_paneManager.SetDefaultPaneOptions(xtpPaneNoHideable);

	m_paneObject		= m_paneManager.CreatePane(IDR_Pane_ObjectProperty, CRect(0, 0, 250, 120), xtpPaneDockRight);
	m_paneEffect		= m_paneManager.CreatePane(IDR_Pane_EffectProperty, CRect(0, 0, 250, 120), xtpPaneDockRight);

	m_paneManager.AttachPane(m_paneEffect, m_paneObject);
}

LRESULT CMainFrame::_AttachDockPane( WPARAM wParam, LPARAM lParam )
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_Pane_ObjectProperty:
				pPane->Attach(m_propertyObject);
				break;

			case IDR_Pane_EffectProperty:
				pPane->Attach(m_propertyEffect);
				break;

			default: assert(0);
			}
		}
		return 1;
	}

	return 0;
}

bool CMainFrame::CreateEditorMainUI()
{
	if(!_OnCreateRibbon())
		return false;

// 	CImageList iconList;
// 	Ogre::StringVectorPtr meshNames;
// 	ManipulatorSystem.GetResource().RenderAllMeshIcons(iconList, meshNames);
// 
// 	if(!_CreateMeshPanel(iconList, meshNames))
// 		return FALSE;

	m_propertyObject->Create(L"STATIC", NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), this, IDR_Pane_ObjectProperty);
	m_propertyEffect->Create(L"STATIC", NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), this, IDR_Pane_EffectProperty);

	m_propertyObject->m_wndPropertyGrid.SetTheme(xtpGridThemeVisualStudio2010);
	m_propertyEffect->m_wndPropertyGrid.SetTheme(xtpGridThemeVisualStudio2010);

	_CreateDockPane();

	ManipulatorSystem.GetResource().PrepareAllIcons();

	ManipulatorScene::GetSingleton().AddCallback(this);
	ManipulatorSystem.GetObject().AddCallback(this);

	return true;
}

void CMainFrame::OnSceneNew()
{
	m_propertyEffect->UpdateAllFromEngine();
	m_propertyEffect->EnableMutableProperty(TRUE);
}

void CMainFrame::OnSceneOpen()
{
	m_propertyEffect->UpdateAllFromEngine();
	m_propertyEffect->EnableMutableProperty(TRUE);
}

void CMainFrame::OnSceneClose()
{
	m_propertyObject->EnableMutableProperty(FALSE);
	m_propertyEffect->EnableMutableProperty(FALSE);
}

int CMainFrame::OnCreateControl( LPCREATECONTROLSTRUCT lpCreateControl )
{
	return FALSE;
}

template<int mode>
void CMainFrame::OnUpdateUI_ObjectEdit( CCmdUI* pCmdUI )
{
// 	if(!ManipulatorSystem.GetIsSceneReady())
// 	{
// 		pCmdUI->Enable(FALSE);
// 		return;
// 	}
// 
// 	if(mode == ManipulatorObject::eEditMode_Select)
// 		pCmdUI->Enable(TRUE);
// 	else
// 		pCmdUI->Enable(ManipulatorSystem.GetObject().GetSelection() != nullptr);
// 
// 	bool bActive = ManipulatorSystem.GetObject().GetCurEditMode() == mode;
// 	pCmdUI->SetCheck(bActive);
}

template<int mode>
void CMainFrame::OnObjectEdit()
{
	bool bActive = ManipulatorSystem.GetObject().GetCurEditMode() == mode;
	bActive = !bActive;

	eActionType action;
	if(mode == ManipulatorObject::eEditMode_Select)
		action = bActive ? eActionType_ObjectSelect : eActionType_None;
	else 
		action = bActive ? eActionType_ObjectEdit : eActionType_None;

	ManipulatorSystem.GetObject().SetCurEditMode((ManipulatorObject::eEditMode)mode);
	ManipulatorAction::GetSingleton().SetActiveAction(action);
}

// void CMainFrame::OnObjectPropertyChanged( Ogre::Entity* pEntity )
// {
// 	Ogre::Entity* curSel = ManipulatorSystem.GetObject().GetSelection();
// 	if(!curSel)
// 	{
// 		m_propertyObject->EnableMutableProperty(FALSE);
// 	}
// 	else if(curSel == pEntity)
// 	{
// 		m_propertyObject->UpdateAllFromEngine();
// 		m_propertyObject->EnableMutableProperty(TRUE);
// 	}	
// }
//
// void CMainFrame::OnObjectSetSelection( Ogre::Entity* pObject )
// {
// 	assert(pObject);
// 	m_propertyObject->UpdateAllFromEngine();
// 	m_propertyObject->EnableMutableProperty(TRUE);
// 
// 	//如果带骨骼动画,则显示动画Ribbon
// 	if (pObject->hasSkeleton())
// 	{
// 		m_animTab->Select();
// 		m_animTab->SetVisible(TRUE);
// 
// 		ManipulatorObject& manObject = ManipulatorSystem.GetObject();
// 		ManipulatorEffect& manEffect = ManipulatorSystem.GetEffect();
// 
// 		const std::string meshname = manObject.GetSelection()->getMesh()->getName();
// 		const auto vecNames = manEffect.GetMeshAnimNames(meshname);
// 
// 		//动画列表
// 		m_animList->ResetContent();
// 		for(size_t i=0; i<vecNames.size(); ++i)
// 			m_animList->AddString(vecNames[i].c_str());
// 		m_animList->SetCurSel(0);
// 	}
// 
// 	RefreshRibbonBar();
// }
// 
// void CMainFrame::OnObjectClearSelection( Ogre::Entity* pObject )
// {
// 	assert(pObject);
// 	m_propertyObject->UpdateAllFromEngine();
// 	m_propertyObject->EnableMutableProperty(FALSE);
// 
// 	if (pObject->getSkeleton())
// 	{
// 		//隐藏动画Ribbon
// 		m_animTab->SetVisible(FALSE);
// 
// 		ManipulatorEffect& manEffect = ManipulatorSystem.GetEffect();
// 		manEffect.PlayAnimation(pObject, m_animList->GetCurSel(), false);
// 		manEffect.OnAnimSelectChange("");
// 		_OnAttachmentPaneChange(FALSE, TRUE);
// 	}
// 
// 	RefreshRibbonBar();
// }

void CMainFrame::OnUpdateUI_ShadowOnOff( CCmdUI* pCmdUI )
{
// 	if(!ManipulatorSystem.GetIsSceneReady())
// 	{
// 		pCmdUI->Enable(FALSE);
// 		return;
// 	}
// 
// 	pCmdUI->Enable(TRUE);
// 	pCmdUI->SetCheck(ManipulatorSystem.GetEffect().GetShadowEnable());
}

void CMainFrame::OnShadowOnOff()
{
// 	bool bEnable = !ManipulatorSystem.GetEffect().GetShadowEnable();
// 	ManipulatorSystem.GetEffect().SetShadowEnable(bEnable);
}

void CMainFrame::OnUpdateUI_SSAOOnOff( CCmdUI* pCmdUI )
{
// 	if(!ManipulatorSystem.GetIsSceneReady())
// 	{
// 		pCmdUI->Enable(FALSE);
// 		return;
// 	}
// 
// 	pCmdUI->Enable(TRUE);
// 	pCmdUI->SetCheck(ManipulatorSystem.GetEffect().GetSSAOEnable());
}

void CMainFrame::OnSSAOOnOff()
{
// 	bool bEnable = !ManipulatorSystem.GetEffect().GetSSAOEnable();
// 	ManipulatorSystem.GetEffect().SetSSAOEnable(bEnable);
}

void CMainFrame::OnUpdateUI_ObjectRemove( CCmdUI* pCmdUI )
{
// 	if(!ManipulatorSystem.GetIsSceneReady())
// 	{
// 		pCmdUI->Enable(FALSE);
// 		return;
// 	}
// 
// 	pCmdUI->Enable(ManipulatorSystem.GetObject().GetSelection() != nullptr);
}

void CMainFrame::OnObjectRemove()
{
// 	ManipulatorObject& manObject = ManipulatorSystem.GetObject();
// 	manObject.RemoveEntity(manObject.GetSelection());
}

void CMainFrame::OnUpdateUI_SharpenOnOff( CCmdUI* pCmdUI )
{
// 	if(!ManipulatorSystem.GetIsSceneReady())
// 	{
// 		pCmdUI->Enable(FALSE);
// 		return;
// 	}
// 
// 	pCmdUI->Enable(TRUE);
// 	pCmdUI->SetCheck(ManipulatorSystem.GetEffect().GetSharpenEnable());
}

void CMainFrame::OnUpdateUI_FXAAOnOff( CCmdUI* pCmdUI )
{
// 	if(!ManipulatorSystem.GetIsSceneReady())
// 	{
// 		pCmdUI->Enable(FALSE);
// 		return;
// 	}
// 
// 	pCmdUI->Enable(TRUE);
// 	pCmdUI->SetCheck(ManipulatorSystem.GetEffect().GetFXAAEnable());
}

void CMainFrame::OnSharpenOnOff()
{
// 	bool bEnable = !ManipulatorSystem.GetEffect().GetSharpenEnable();
// 	ManipulatorSystem.GetEffect().SetSharpenEnable(bEnable);
}

void CMainFrame::OnFXAAOnOff()
{
// 	bool bEnable = !ManipulatorSystem.GetEffect().GetFXAAEnable();
// 	ManipulatorSystem.GetEffect().SetFXAAEnable(bEnable);
}

void CMainFrame::RefreshRibbonBar()
{
	GetCommandBars()->RedrawCommandBars();
}
