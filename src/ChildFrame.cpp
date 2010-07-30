/**
 *	@file	ChildFrame.cpp
 *	@brief	�^�u�y�[�W�P�̏���.
 */

#include "stdafx.h"

//#include <vector>
#include "PluginManager.h"
#include "MtlBrowser.h"

#include "FavoriteMenu.h"
#include "DonutFavoritesMenu.h"
#include "DonutView.h"
#include "AddressBar.h"
#include "MDITabCtrl.h"
#include "MDIChildUserMessenger.h"

#include "option/DLControlOption.h"
#include "option/StartUpOption.h"
#include "option/DonutConfirmOption.h"
#include "option/IgnoreURLsOption.h"
#include "option/CloseTitleOption.h"
#include "option/UrlSecurityOption.h"	//+++
#include "ExStyle.h"					//+++

#include "ParseInternetShortcutFile.h"	//+++

#include "FileNotification.h"
#include "DonutPFunc.h" 		//+++
#include "ChildFrame.h"

#include "StyleSheetUtils.h"

//#include "MainFrame.h"			//+++ debug
#include "nsIWebBrowserFocus.h"
#include "nsIWebBrowserFind.h"
#include "nsIContentViewer.h"
#include "nsIMarkupDocumentViewer.h"
#include "nsIDOMNodeSelector.h"
#include "nsIDOMDocumentFragment.h"
#include "nsIDOMHTMLDocument.h"
#include "nsContentCID.h"
#include "nsIDocumentEncoder.h"
#include "nsIDOMHTMLElement.h"
#include "nsIDOMNodeList.h"
#include "nsIDOMHTMLCollection.h"
#include "nsIDOMHTMLAnchorElement.h"
#include "nsIContextMenuListener2.h"
#include "nsIDOMDocumentView.h"
#include "nsIDOMAbstractView.h"
#include "nsIFind.h"
#include "nsIDOMDocumentFragment.h"
#include "nsIDOMDocumentRange.h"

//#include "MDIChildUserMessenger.h"
#ifdef _DEBUG
	const bool _Donut_ChildFrame_traceOn = true;	//false;
	#define dcfTRACE	if (_Donut_ChildFrame_traceOn) ATLTRACE
#else
	#define dcfTRACE
#endif

#if defined USE_ATLDBGMEM
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



static const int	g_LIGHTMAX		= 5;

#if 1 //+++ ������off ... ����ς�A���̂܂�.
#if 0 //def USE_ORG_UNDONUT_INI
static const LPCTSTR		g_lpszLight[] = {
	"<span id='DonutP' style='color:black;background:FFFF00'>",
	"<span id='DonutP' style='color:black;background:00FFFF'>",
	"<span id='DonutP' style='color:black;background:FF00FF'>",
	"<span id='DonutP' style='color:black;background:7FFF00'>",
	"<span id='DonutP' style='color:black;background:1F8FFF'>"
};
#else
static const LPCTSTR		g_lpszLight[] = {
  #if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
	_T("color:black;background:#FFFF00"),
	_T("color:black;background:#00FFFF"),
	_T("color:black;background:#FF00FF"),
	_T("color:black;background:#7FFF00"),
	_T("color:black;background:#1F8FFF"),
  #else
	"<span id='Donut' style='color:black;background:FFFF00'>",
	"<span id='Donut' style='color:black;background:00FFFF'>",
	"<span id='Donut' style='color:black;background:FF00FF'>",
	"<span id='Donut' style='color:black;background:7FFF00'>",
	"<span id='Donut' style='color:black;background:1F8FFF'>"
  #endif
};
#endif
#endif



// ===========================================================================
// �������E�y�[�W�ǂݍ���

CChildFrame::CChildFrame(CMDITabCtrl &MDITab, CDonutAddressBar &addressbar, bool bNewWindow2, DWORD dwDefaultDLControlFlags, DWORD dwDefaultExtendedStyleFlags)
	: m_bNavigateBack(FALSE)
	, m_bNavigateForward(FALSE)
	, m_MDITab(MDITab)
	, m_AddressBar(addressbar)
	, m_bNewWindow2(bNewWindow2)
	, m_wndBrowser(this, 7)
	, m_FavMenu(this, ID_INSERTPOINT_CHILDFAVORITEMENU)
	, m_nProgress(-1)
	, m_nProgressMax(0)
	, m_bPageFocusInitialized(false)
	, m_bClosing(false)
	, m_bWaitNavigateLock(true)
	, m_hWndFocus(NULL)
	, m_view(dwDefaultDLControlFlags, dwDefaultExtendedStyleFlags)	//+++�����ǉ�.
	, m_bOperateDrag(false)
	, m_bExPropLock(false)
	, m_hWndF(NULL)
	, m_hWndA(NULL)
	, m_strBookmark(NULL)
	, m_nPainBookmark(0)
	, m_nSecureLockIcon( secureLockIconUnsecure )
	, m_bPrivacyImpacted( TRUE )
	, m_bInitTravelLog( TRUE )
	, m_nImgWidth( -1 )
	, m_nImgHeight( -1 )
	, m_bNowHilight(FALSE)
	, m_bAutoHilight( false )
  #if 1	//+++
	, m_nCmdType(0)
	, m_bImageAutoSizeReq(0)
	, m_nImgScl(100)
	, m_nImgSclSav(100)
	, m_bImg(0)
	, m_japanCharSet(-1)
  #endif
{
  #if 1	//+++
	{
		static bool bInit				= false;
		static bool bImgAuto_NouseLClk	= false;
		static int  nImgSclSw			= 0;
		if (bInit == 0) {
			CIniFileI pr( g_szIniFileName, _T("ETC") );
			bImgAuto_NouseLClk	= pr.GetValue(_T("ImageAutoSize_NouseLClick")) != 0;
			nImgSclSw			= pr.GetValue(_T("ImageAutoSize_FirstOn")) != 0;
		}
		m_bImgAuto_NouseLClk	= bImgAuto_NouseLClk;
		m_nImgSclSw				= nImgSclSw;
	}
  #endif
}


/** �����N��V�����^�u�ŊJ���A��
 */
void CChildFrame::OnDocHostUIOpenNewWin(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/)
{
	if (wNotifyCode == NM_ON) {
		m_view.m_ViewOption.m_dwExStyle |= DVS_EX_OPENNEWWIN;
		m_MDITab.NavigateLockTab(m_hWnd, true);
	} else if (wNotifyCode == NM_OFF) {
		m_view.m_ViewOption.m_dwExStyle &= ~DVS_EX_OPENNEWWIN;
		m_MDITab.NavigateLockTab(m_hWnd, false);
	} else if (m_view.m_ViewOption.m_dwExStyle & DVS_EX_OPENNEWWIN) {
		m_view.m_ViewOption.m_dwExStyle &= ~DVS_EX_OPENNEWWIN;
		m_MDITab.NavigateLockTab(m_hWnd, false);
	} else {
		m_view.m_ViewOption.m_dwExStyle |= DVS_EX_OPENNEWWIN;
		m_MDITab.NavigateLockTab(m_hWnd, true);
	}
}


void CChildFrame::OnNewWindow(nsIWebBrowser **ppSupp, bool *bCancel)
{
	dcfTRACE( _T("OnNewWindow\n") );

	if (CMainOption::s_bAppClosing) {
		dcfTRACE( _T(" application closing, ignore this event\n") );
		*bCancel = true;
		return;
	}

	DWORD		 exStyle= _GetInheritedExtendedStyleFlags();	//+++ �V�K�Ɏq�������Ƃ��́A���̎q���ɓn�� ExtendedStyle �����߂�...

	m_MDITab.SetLinkState(LINKSTATE_A_ON);
	CChildFrame *pChild = CChildFrame::NewWindow(	m_hWndMDIClient,
													m_MDITab,
													m_AddressBar,
													true,
													_GetInheritedDLControlFlags(),
													exStyle );	//+++ �����ǉ�.

	//m_MDITab.SetLinkState(LINKSTATE_OFF);
	// Note: In this case, must be activated in BeforeNavigate2 strangely
	// pChild->ActivateFrameForNewWindowDelayed();
	pChild->ActivateFrame();
	if (pChild == NULL) {
		*bCancel = true;
		return;
	}

	//Raise Plugin Event
	//CComBSTR bstr;
	//pChild->m_spBrowser->get_LocationURL(&bstr);
	//CPluginManager::ChainCast_PluginEvent(DEVT_TAB_OPENED,(DWORD)(LPCTSTR)CString(bstr),0);

	pChild->m_strSearchWord = m_strSearchWord;

  #if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
	pChild->m_bNowHilight	= m_bNowHilight;
  #endif
	
	*ppSupp = pChild->m_spBrowser;
	//NS_ADDREF(*ppSupp);
}
void CChildFrame::OnLocationChange(const CString& strURL)
{
	if (MDIGetActive() == m_hWnd)
		m_AddressBar.SetWindowText(strURL);
}

void CChildFrame::InsertTypeOf(CMenu& menuPopup,
							   DWORD *menupos,
							   DWORD dwID,
							   CSimpleArray<HMENU> &toDestroy,
							   CSimpleMap<DWORD, DWORD> &toRemove,
							   CSimpleArray<DWORD> &commands)
{
	CString 	strSection;
	strSection.Format(_T("Type%d"), dwID);
	CString 	strPath 	= _GetFilePath( _T("Menu.ini") );
	CIniFileI	pr(strPath, strSection);
	DWORD		dwCount 	= pr.GetValue(_T("FrontCount"), 0 );

	DWORD		 jj;
	for (jj = 0; jj < dwCount; jj++) {
		CString strKey;
		strKey.Format(_T("Front%02d"), jj);
		DWORD	dwCmd = pr.GetValue(strKey, 0);

		DWORD dwID = 0;
		HMENU hSubMenu = 0;

		switch (dwCmd) {
		case ID_FAVORITES_DROPDOWN:
			hSubMenu = (HMENU) ::SendMessage(GetTopLevelWindow(), WM_MENU_GET_FAV	   , 0, 0);
			toRemove.Add(jj + *menupos, dwCmd);
			break;

		case ID_FAVORITES_GROUP_DROPDOWN:
			hSubMenu = (HMENU) ::SendMessage(GetTopLevelWindow(), WM_MENU_GET_FAV_GROUP, 0, 0);
			toRemove.Add(jj + *menupos, dwCmd);
			break;

		case ID_SCRIPT:
			hSubMenu = (HMENU) ::SendMessage(GetTopLevelWindow(), WM_MENU_GET_SCRIPT   , 0, 0);
			toRemove.Add(jj + *menupos, dwCmd);
			break;

		case ID_SEARCHENGINE_MENU:
			{
				CMenuHandle menu = CDonutSearchBar::GetInstance()->GetSearchEngineMenuHandle();
				hSubMenu = menu.m_hMenu;
				toRemove.Add(jj + *menupos, dwCmd);
			}
			break;

		case ID_DLCTL_CHG_MULTI:	dwID = IDR_MULTIMEDIA;		break;
		case ID_DLCTL_CHG_SECU: 	dwID = IDR_SECURITY;		break;
		case ID_VIEW_FONT_SIZE: 	dwID = IDR_VIEW_FONT_SIZE;	break;
		case ID_COOKIE: 			dwID = IDR_COOKIE;			break;
		case ID_HTMLZOOM_MENU: 		dwID = IDR_ZOOM_MENU;		break;
		default:												break;
		}

		if (dwID != 0) {
			CMenuHandle menu;
			menu.LoadMenu(dwID);
			hSubMenu = menu.GetSubMenu(0);
			toDestroy.Add(menu);
		}

		CString strCmd;
		CToolTipManager::LoadToolTipText(dwCmd, strCmd);			//StringTable���當������擾

		if(hSubMenu!=0){
			if (dwCmd == ID_FAVORITES_DROPDOWN) {
				strCmd = _T("���C�ɓ���(&A)"); 			//���C�����j���[���當������擾�ł��Ȃ�����
			} else if (dwCmd == ID_FAVORITES_GROUP_DROPDOWN) {
				strCmd = _T("���C�ɓ���O���[�v(&G)"); 	//��ɓ���
			} else if (dwCmd == ID_SCRIPT) {
				strCmd = _T("�X�N���v�g(&S)"); 			//��ɓ���
			}
			menuPopup.AppendMenu(MF_STRING, hSubMenu, strCmd);
			_BeforeInitSpecialMenu(dwCmd);
		} else if (strCmd.IsEmpty() == FALSE) {
			commands.Add(dwCmd);
			menuPopup.AppendMenu(MF_STRING, dwCmd, strCmd);
		} else {
			menuPopup.AppendMenu(MF_SEPARATOR);
		}
	}
	*menupos += jj;
}

bool CChildFrame::DocHasSelection(nsIDOMDocument *doc)
{
    nsCOMPtr<nsIDOMDocumentView> docview = do_QueryInterface(doc);
    if (!docview) return false;

	nsCOMPtr<nsIDOMAbstractView> abstractView;
	docview->GetDefaultView(getter_AddRefs(abstractView));
	nsCOMPtr<nsIDOMWindow> win = do_QueryInterface(abstractView);
    if (!win) return false;

    nsCOMPtr<nsISelection> selection;
    win->GetSelection(getter_AddRefs(selection));
    if (!selection) return false;

	PRUnichar *hoge;
	selection->ToString(&hoge);
	CString fuga(hoge);
	NS_Free(hoge);
	if(fuga.IsEmpty()) return false;
	else return true;
}

void CChildFrame::OnShowContextMenu(PRUint32 aContextFlags, nsIContextMenuInfo *aUtils)
{
	nsCOMPtr<nsIDOMNode> domNode;
	aUtils->GetTargetNode(getter_AddRefs(domNode));
	if(!domNode) return;
    nsCOMPtr<nsIDOMDocument> doc;
    domNode->GetOwnerDocument(getter_AddRefs(doc));
	if(!doc) return;
	bool hasSelection = DocHasSelection(doc);

	nsCOMPtr<nsIDOMEvent> evt;
	aUtils->GetMouseEvent(getter_AddRefs(evt));
	nsCOMPtr<nsIDOMMouseEvent> mouseEvt = do_QueryInterface(evt);
    PRInt32 scrX, scrY;
	mouseEvt->GetScreenX(&scrX);
	mouseEvt->GetScreenY(&scrY);

	CMenu menuPopup;
	menuPopup.CreatePopupMenu();

	CSimpleArray<HMENU> toDestroy;
	CSimpleMap<DWORD, DWORD> toRemove;
	CSimpleArray<DWORD> commands;

	DWORD idx = 0;

	if(hasSelection){
		InsertTypeOf(menuPopup, &idx, CONTEXT_MENU_TEXTSELECT, toDestroy, toRemove, commands);
	}

	if(aContextFlags & nsIContextMenuListener2::CONTEXT_LINK){
		if(idx>0){ menuPopup.AppendMenu(MF_SEPARATOR); idx++; }
		InsertTypeOf(menuPopup, &idx, CONTEXT_MENU_ANCHOR, toDestroy, toRemove, commands);
	}

	if(aContextFlags & nsIContextMenuListener2::CONTEXT_IMAGE){
		if(idx>0){ menuPopup.AppendMenu(MF_SEPARATOR); idx++; }
		InsertTypeOf(menuPopup, &idx, CONTEXT_MENU_IMAGE, toDestroy, toRemove, commands);
	}
	
	if(idx == 0)
		InsertTypeOf(menuPopup, &idx, CONTEXT_MENU_DEFAULT, toDestroy, toRemove, commands);

	m_ctxTargetNode = domNode;
	nsEmbedString tmp;
	aUtils->GetAssociatedLink(tmp);
	m_ctxLink = tmp.get();
	nsCOMPtr<nsIURI> uri;
	aUtils->GetImageSrc(getter_AddRefs(uri));
	if(uri){
		nsEmbedCString tmpc;
		uri->GetSpec(tmpc);
		m_ctxImage = tmpc.get();
	}

	int 	iSelection	= ::TrackPopupMenu(   menuPopup
		, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD
		, scrX
		, scrY
		, 0
		, GetTopLevelWindow()
		, (RECT *) NULL );
	BOOL	bSendFrm = FALSE;

	// �R�}���h�͈�
	if (COMMAND_RANGE_START <= iSelection && iSelection <= COMMAND_RANGE_END)
		bSendFrm = TRUE;

	// ���C�ɓ���A�O���[�v
	else if ( (FAVORITE_MENU_ID_MIN <= iSelection && iSelection <= FAVORITE_MENU_ID_MAX)
			||(FAVGROUP_MENU_ID_MIN <= iSelection && iSelection <= FAVGROUP_MENU_ID_MAX) )
		bSendFrm = TRUE;

	// �X�N���v�g
	else if (ID_INSERTPOINT_SCRIPTMENU <= iSelection && iSelection <= ID_INSERTPOINT_SCRIPTMENU_END)
		bSendFrm = TRUE;

	//+++ �����G���W��
	else if (ID_INSERTPOINT_SEARCHENGINE <= iSelection && iSelection <= ID_INSERTPOINT_SEARCHENGINE_END)
		bSendFrm = TRUE;

	if ( bSendFrm == FALSE && commands.Find( (DWORD) iSelection ) != -1 )
		bSendFrm = TRUE;

	if(bSendFrm) ::SendMessage(GetTopLevelWindow(), WM_COMMAND, iSelection, NULL);

	//Clean up
	m_ctxTargetNode = NULL;
	m_ctxLink = "";
	m_ctxImage = "";

	for(int i = toRemove.GetSize()-1; i>=0; i--){
		menuPopup.RemoveMenu(toRemove.GetKeyAt(i), MF_BYPOSITION);
		_RestoreSpecialMenu(toRemove.GetValueAt(i));
	}
	for(int i=0;i<toDestroy.GetSize();i++){
		//TODO:����̂�����Ȃ��̂��킩��Ȃ�
	}
}


CChildFrame *CChildFrame::NewWindow(
		HWND				hWndMDIClient,
		CMDITabCtrl &		tabMDI,
		CDonutAddressBar &	adBar,
		bool				bNewWindow2 /*= false*/,
		DWORD				dwDLFlags	/*= CDLControlOption::s_dwDLControlFlags*/,
		DWORD				dwESFlags	/*= CDLControlOption::s_dwExtendedStyleFlags*/	//+++ �ǉ�
){
	int nCount = tabMDI.GetItemCount();
	if ( !CMainOption::IsQualify( nCount ) )
		return NULL;

	if (dwDLFlags == (DWORD)-1)
		dwDLFlags = CDLControlOption::s_dwDLControlFlags;

	if (dwESFlags == (DWORD)-1)									//+++
		dwESFlags = CDLControlOption::s_dwExtendedStyleFlags;	//+++

	//ATLTRY(CChildFrame* pChild = new CChildFrame(tabMDI, adBar, bNewWindow2, dwDLFlags));
	CChildFrame *pChild = new CChildFrame(tabMDI, adBar, bNewWindow2, dwDLFlags, dwESFlags);

	//��[?
	if (pChild != NULL) {
		pChild->CreateEx(hWndMDIClient);
		//+++ �������ڂ��Ƃ��...
		//CDonutSearchBar *pSearch = (CDonutSearchBar *) ::SendMessage(pChild->GetTopLevelParent(), WM_GET_SEARCHBAR, 0, 0);
		CDonutSearchBar *pSearch = CDonutSearchBar::GetInstance();
		if (pSearch) {
			pChild->m_strSearchWord = pSearch->RemoveShortcutWord( pSearch->GetSearchStr() );
		}
	}

	return pChild;
}

void CChildFrame::OnHoge(){

}

//void CChildFrame::OnStatusTextChange(const CString &strText){}

//void CChildFrame::OnProgressChange(long progress, long progressMax)[}

void CChildFrame::OnDownloadBegin(){}

//void CChildFrame::OnDownloadComplete(){}

//void CChildFrame::OnTitleChange(const CString &strTitle){}

void CChildFrame::OnNavigateComplete2(nsISupports *pSupp, const CString &strURL){}

void CChildFrame::OnBeforeNavigate(const CString& strURL, bool &cancel)
{

	{
		//�v���O�C���C�x���g - �i�r�Q�[�g�O
		DEVTS_TAB_NAVIGATE stn;
		stn.nIndex			 = m_MDITab.GetTabIndex(m_hWnd);
		stn.lpstrURL		 = (LPCTSTR) strURL;
		stn.lpstrTargetFrame = (LPCTSTR) NULL/*strTargetFrameName*/;
		int 	nRet		 = CPluginManager::ChainCast_PluginEvent(DEVT_TAB_BEFORENAVIGATE, stn.nIndex, (SPARAM) &stn);

		if (nRet == -1) {
			//�i�r�Q�[�g�L�����Z��
			cancel = true;
			return;
		} else if (nRet == -2) {
			//�N���[�Y
			cancel 	  = true;
			m_bNewWindow2 = false;
			m_bClosing	  = true;
			PostMessage(WM_CLOSE);		// It's possible to post twice, but don't care.
			return;
		}
	}

	if (m_view.m_ViewOption.m_dwExStyle & DVS_EX_BLOCK_MAILTO) {
		CString strMailto = strURL.Left(6);
		strMailto.MakeUpper();

		if (strMailto.Compare( _T("MAILTO") ) == 0) {
			cancel = true;
			return;
		}
	}

	if (m_bClosing) {
		cancel = true;
		return;
	}

	// deter popups
	//COMMENT IsPageIWebBrowser
	if ( m_bNewWindow2 && /*IsPageIWebBrowser(pDisp) &&*/ CIgnoredURLsOption::SearchString(strURL) ) {	// close ignored url
		cancel 	  = true;
		m_bNewWindow2 = false;
		m_bClosing	  = true;
		PostMessage(WM_CLOSE);			// It's possible to post twice, but don't care.

		{
			//�v���O�C���C�x���g - �|�b�v�A�b�v�}�~
			CPluginManager::BroadCast_PluginEvent(DEVT_BLOCKPOPUP, (FPARAM) (LPCTSTR) strURL, 0);
		}
		return;
	}

	// javascript����n�܂�NewWindow��close
	if (m_bNewWindow2) {
		CString strJava = strURL.Left(10);
		strJava.MakeUpper();

		if (strJava.Find(_T("JAVASCRIPT")) != -1) {
			cancel 	  = true;
			m_bNewWindow2 = false;
			m_bClosing	  = true;
			PostMessage(WM_CLOSE);		// It's possible to post twice, but don't care.

			DonutOpenFile(m_hWnd, strURL);
			return;
		}
	}

  #if 1	//+++ Url�ʊg���v���p�e�B�ݒ�̏���. ��Ƀ����N�p.(�ʏ��open�̏ꍇ�́ACMainFrame::OnUserOpenFile�ȏ�����ʂ�̂ŁA������Őݒ肸��)
	DWORD exopts = 0xFFFFFFFF, dlCtlFlg = 0xFFFFFFFF, exstyle = 0xFFFFFFFF, autoRefresh = 0xFFFFFFFF, dwExPropOpt = 8;
	if (CUrlSecurityOption::FindUrl( strURL, &exopts, &dwExPropOpt, 0 )) {
		CExProperty  ExProp(CDLControlOption::s_dwDLControlFlags, CDLControlOption::s_dwExtendedStyleFlags, 0, exopts, dwExPropOpt);
		dlCtlFlg	= ExProp.GetDLControlFlags();
		exstyle		= ExProp.GetExtendedStyleFlags();
		autoRefresh = ExProp.GetAutoRefreshFlag();
	}
  #endif

	// Navigate lock
	if ( !m_bExPropLock
	   && _check_flag(DVS_EX_OPENNEWWIN, m_view.m_ViewOption.m_dwExStyle)
	   && m_bWaitNavigateLock == false
	   && !IsRefreshBeforeNavigate() //TODO: plus7: �{���ɕK�v���HIE����ł́H
)
	{
		m_bExPropLock = FALSE;
		dcfTRACE( _T(" Locked, open new window\n") );
		if (dlCtlFlg == 0xFFFFFFFF)
			dlCtlFlg =  _GetInheritedDLControlFlags();
		if (exstyle  == 0xFFFFFFFF)			//+++
			exstyle  = _GetInheritedExtendedStyleFlags();	//+++ �V�K�Ɏq�������Ƃ��́A���̎q���ɓn�� ExtendedStyle �����߂�...
		CChildFrame *pChild = CChildFrame::NewWindow( m_hWndMDIClient, m_MDITab, m_AddressBar, false, dlCtlFlg, exstyle );
		if (pChild) {
			pChild->ActivateFrameForNewWindowDelayed();
			pChild->Navigate2(strURL);
			pChild->m_strSearchWord = m_strSearchWord;
		  #if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
			pChild->m_bNowHilight	= m_bNowHilight;
		  #endif
			pChild->ForceMessageLoop(); 	//*+++

		  #if 1	//+++ url�ʊg���v���p�e�B�̏���.
			if (exopts != 0xFFFFFFFF) {
				pChild->view().PutDLControlFlags( dlCtlFlg );
				pChild->SetViewExStyle( exstyle );
				pChild->view().m_ViewOption.SetAutoRefreshStyle( autoRefresh );
			}
		  #endif
		}
		cancel 	  = true;
		return;
	}

  #if 1	//+++ url�ʊg���v���p�e�B�̏���....
		//+++	�߂�E�i�ނł̊g���v���p�e�B���̕ł��Ƃ̕ۑ����ł��Ă��Ȃ��̂ŁA�j�]����...
		//+++	���A�o�O���Ă����f����邱�Ƃ̂ق����Ӗ����肻���Ȃ̂ŗ��p
	if (CUrlSecurityOption::s_bValid) {
		if (exopts != 0xFFFFFFFF && CUrlSecurityOption::activePageToo()) {
			this->view().PutDLControlFlags( dlCtlFlg );
			this->view().m_ViewOption.SetAutoRefreshStyle( autoRefresh );
			this->SetViewExStyle( exstyle );	//+++����: �}�E�X���{�^���N���b�N�ł́A�����N�ʃ^�u�\���̏ꍇ�A�܂��^�u�ʒu������̂��ߐݒ�ł��Ȃ�...
		//} else {
		}
	}
  #endif

	m_bExPropLock	= FALSE;

	// Note: Some ActivateFrame would go mad, because MDI client window not setup yet.
	if (m_bNewWindow2) {				// delayed activation if OnNewWindow2 called
		ActivateFrameForNewWindowDelayed();
		m_bNewWindow2 = false;
	}

	// UH
	// �n�C���C�g���H���
  #if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
	//m_bNowHilight=FALSE;	// gae: ��x�K�p�����n�C���C�g�͖����I�ɉ��������܂ňێ������
  #else
	m_bNowHilight	= FALSE;
  #endif
	m_nPainBookmark = 0;
	m_strBookmark	= LPCOLESTR(NULL);

	{
		//�C�x���g���� - �i�r�Q�[�g
		DEVTS_TAB_NAVIGATE stn;
		stn.nIndex			 = m_MDITab.GetTabIndex(m_hWnd);
		stn.lpstrURL		 = (LPCTSTR) strURL;
		stn.lpstrTargetFrame = (LPCTSTR) NULL /*strTargetFrameName;*/;
		CPluginManager::BroadCast_PluginEvent(DEVT_TAB_NAVIGATE, stn.nIndex, (SPARAM) &stn);
	}
}

LRESULT CChildFrame::OnCreate(LPCREATESTRUCT lpcreatestruct)
{
	dcfTRACE( _T("CChildFrame::OnCreate\n") );

	//RtlSetMinProcWorkingSetSize();				//+++ ( �������̗\��̈���ꎞ�I�ɍŏ����B�E�B���h�E���ŏ��������ꍇ�Ɠ��� ) ... �Ђ���Ƃ���Ƃ��܂��낵���Ȃ���������... ����ς�N�����͂��܂��낵���Ȃ������ŁA��߂ɂ��Ƃ�...

	// Let me initialize itself
	LRESULT 	lRet		   = DefWindowProc();

	//TODO: Replace with a URL of your choice
	CRect		rc			   = rcDefault;
	__CalcDefaultRect(rc);
	m_hWndClient = m_view.Create(m_hWnd, rc, _T("about:blank"), WS_CHILD | WS_VISIBLE  | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE, ID_DONUTVIEW);

	//�h���b�O�h���b�v�̏�����ݒ�
	_InitDragDropSetting();

    m_spBrowser = m_view.m_spBrowser;    

	m_view.AttachEventListener(this);    

/*	if ( _check_flag(MAIN_EX_REGISTER_AS_BROWSER, CMainOption::s_dwMainExtendedStyle) )
		SetRegisterAsBrowser(true); 	// for target name resolution
*/

	_SetupWindowCloseHook();

	// set up for mdi tab ctrl

	CMenuHandle menu0		   = m_MDITab.m_menuPopup.GetSubMenu(0);
	HMENU		hMenu		   = menu0.GetSubMenu(_nPosFavoriteMenuOfTab);
	m_FavMenu.InstallExplorerMenu(hMenu);

	if (!m_bNewWindow2)
		SetVisible(true);

	CFavoritesMenuOption::Add(m_hWnd);

	m_japanCharSet = -1;

	//m_ieToolBar    = std::auto_ptr<CIEToolBar>(new CIEToolBar(((CWindow*)g_pMainWnd)->m_hWnd/*GetTopLevelParent()*/, m_spBrowser.p, _T("{625AA53D-1F10-44FE-B907-91FE25220D3F}")));	//+++ ����.
	//m_ieToolBar    = std::auto_ptr<CIEToolBar>(new CIEToolBar(((CWindow*)g_pMainWnd)->m_hWnd/*GetTopLevelParent()*/, m_spBrowser.p, _T("32099AAC-C132-4136-9E9A-4E364A424E17")));	//+++ ����.

	return lRet;
}



void CChildFrame::__CalcDefaultRect(CRect &rc)
{
	BOOL bMaximized = FALSE;
	HWND hWndActive = MDIGetActive(&bMaximized);

	if (hWndActive == NULL || bMaximized) {
		::GetClientRect(m_hWndMDIClient, &rc);
	}
}



void CChildFrame::_SetupWindowCloseHook()
{
#if COMMENT
	HWND hWndBrowser;

	::EnumChildWindows(m_view.m_hWnd, (WNDENUMPROC) _EnumProc, (LPARAM) &hWndBrowser);
	ATLASSERT( ::IsWindow(hWndBrowser) );
	m_wndBrowser.SubclassWindow(hWndBrowser);
#endif
}



//static
BOOL CALLBACK CChildFrame::_EnumProc(HWND hWnd, LPARAM lParam)
{
	HWND *phWnd = (HWND *) lParam;
	ATLASSERT( phWnd != 0 );
	*phWnd = hWnd;
	return FALSE;
}



void CChildFrame::_InitDragDropSetting()
{
	CIniFileI	pr( _GetFilePath("MouseEdit.ini"), _T("MouseCtrl") );
	DWORD			dwCommand = pr.GetValue(_T("DragDrop"), 0);
	m_bOperateDrag = (dwCommand != 0);
	pr.Close();
	if (m_bOperateDrag) {
		m_view.SetOperateDragDrop(TRUE, dwCommand);
	}
}



//public:
void CChildFrame::OnBeforeNavigate2(
					IDispatch *			pDisp,
					const CString &		strURL,
					DWORD				nFlags,
					const CString &		strTargetFrameName,
					CSimpleArray<BYTE>&	baPostedData,
					const CString &		strHeaders,
					bool &				bCancel )
{
	dcfTRACE(_T("CChildFrame::BeforeNavigate2 url(%s) frame(%s)\n"), strURL.Left(100), strTargetFrameName);

  #if 1	//+++ �������T�C�Y�\�ȃy�[�W�ɐV���ɊG�łȂ��y�[�W��\������ƁA���̃y�[�W�����T�C�Y�\�ɂȂ��Ă��܂��Ă����̂�Ώ�.
	if (m_nImgWidth >= 0) {
		m_nImgWidth			= -1;		//+++ �g���q(��)�`�F�b�N�̂��߂ɁA������
		m_bImg				= 0;
		m_bImageAutoSizeReq = 1;
	}
  #endif

	{
		//�v���O�C���C�x���g - �i�r�Q�[�g�O
		DEVTS_TAB_NAVIGATE stn;
		stn.nIndex			 = m_MDITab.GetTabIndex(m_hWnd);
		stn.lpstrURL		 = (LPCTSTR) strURL;
		stn.lpstrTargetFrame = (LPCTSTR) strTargetFrameName;
		int 	nRet		 = CPluginManager::ChainCast_PluginEvent(DEVT_TAB_BEFORENAVIGATE, stn.nIndex, (SPARAM) &stn);

		if (nRet == -1) {
			//�i�r�Q�[�g�L�����Z��
			bCancel = true;
			return;
		} else if (nRet == -2) {
			//�N���[�Y
			bCancel 	  = true;
			m_bNewWindow2 = false;
			m_bClosing	  = true;
			PostMessage(WM_CLOSE);		// It's possible to post twice, but don't care.
			return;
		}
	}

	if (m_view.m_ViewOption.m_dwExStyle & DVS_EX_BLOCK_MAILTO) {
		CString strMailto = strURL.Left(6);
		strMailto.MakeUpper();

		if (strMailto.Compare( _T("MAILTO") ) == 0) {
			bCancel = true;
			return;
		}
	}

	if (m_bClosing) {
		bCancel = true;
		return;
	}

	// deter popups
	//COMMENT IsPageIWebBrowser
	if ( m_bNewWindow2 && /*IsPageIWebBrowser(pDisp) &&*/ CIgnoredURLsOption::SearchString(strURL) ) {	// close ignored url
		bCancel 	  = true;
		m_bNewWindow2 = false;
		m_bClosing	  = true;
		PostMessage(WM_CLOSE);			// It's possible to post twice, but don't care.

		{
			//�v���O�C���C�x���g - �|�b�v�A�b�v�}�~
			CPluginManager::BroadCast_PluginEvent(DEVT_BLOCKPOPUP, (FPARAM) (LPCTSTR) strURL, 0);
		}
		return;
	}

	// javascript����n�܂�NewWindow��close
	if (m_bNewWindow2) {
		CString strJava = strURL.Left(10);
		strJava.MakeUpper();

		if (strJava.Find(_T("JAVASCRIPT")) != -1) {
			bCancel 	  = true;
			m_bNewWindow2 = false;
			m_bClosing	  = true;
			PostMessage(WM_CLOSE);		// It's possible to post twice, but don't care.

			DonutOpenFile(m_hWnd, strURL);
			return;
		}
	}

  #if 1	//+++ Url�ʊg���v���p�e�B�ݒ�̏���. ��Ƀ����N�p.(�ʏ��open�̏ꍇ�́ACMainFrame::OnUserOpenFile�ȏ�����ʂ�̂ŁA������Őݒ肸��)
	DWORD exopts = 0xFFFFFFFF, dlCtlFlg = 0xFFFFFFFF, exstyle = 0xFFFFFFFF, autoRefresh = 0xFFFFFFFF, dwExPropOpt = 8;
	if (CUrlSecurityOption::FindUrl( strURL, &exopts, &dwExPropOpt, 0 )) {
		CExProperty  ExProp(CDLControlOption::s_dwDLControlFlags, CDLControlOption::s_dwExtendedStyleFlags, 0, exopts, dwExPropOpt);
		dlCtlFlg	= ExProp.GetDLControlFlags();
		exstyle		= ExProp.GetExtendedStyleFlags();
		autoRefresh = ExProp.GetAutoRefreshFlag();
	}
  #endif

	// Navigate lock
	if ( !m_bExPropLock
	   && _check_flag(DVS_EX_OPENNEWWIN, m_view.m_ViewOption.m_dwExStyle)
	   && m_bWaitNavigateLock == false
	   && !IsRefreshBeforeNavigate() //TODO: plus7: �{���ɕK�v���HIE����ł́H
)
	{
		m_bExPropLock = FALSE;
		dcfTRACE( _T(" Locked, open new window\n") );
		if (dlCtlFlg == 0xFFFFFFFF)
			dlCtlFlg =  _GetInheritedDLControlFlags();
		if (exstyle  == 0xFFFFFFFF)			//+++
			exstyle  = _GetInheritedExtendedStyleFlags();	//+++ �V�K�Ɏq�������Ƃ��́A���̎q���ɓn�� ExtendedStyle �����߂�...
		CChildFrame *pChild = CChildFrame::NewWindow( m_hWndMDIClient, m_MDITab, m_AddressBar, false, dlCtlFlg, exstyle );
		if (pChild) {
			pChild->ActivateFrameForNewWindowDelayed();
			pChild->Navigate2(strURL);
			pChild->m_strSearchWord = m_strSearchWord;
		  #if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
			pChild->m_bNowHilight	= m_bNowHilight;
		  #endif
			pChild->ForceMessageLoop(); 	//*+++

		  #if 1	//+++ url�ʊg���v���p�e�B�̏���.
			if (exopts != 0xFFFFFFFF) {
				pChild->view().PutDLControlFlags( dlCtlFlg );
				pChild->SetViewExStyle( exstyle );
				pChild->view().m_ViewOption.SetAutoRefreshStyle( autoRefresh );
			}
		  #endif
		}
		bCancel 	  = true;
		return;
	}

  #if 1	//+++ url�ʊg���v���p�e�B�̏���....
		//+++	�߂�E�i�ނł̊g���v���p�e�B���̕ł��Ƃ̕ۑ����ł��Ă��Ȃ��̂ŁA�j�]����...
		//+++	���A�o�O���Ă����f����邱�Ƃ̂ق����Ӗ����肻���Ȃ̂ŗ��p
	if (CUrlSecurityOption::s_bValid) {
		if (exopts != 0xFFFFFFFF && CUrlSecurityOption::activePageToo()) {
			this->view().PutDLControlFlags( dlCtlFlg );
			this->view().m_ViewOption.SetAutoRefreshStyle( autoRefresh );
			this->SetViewExStyle( exstyle );	//+++����: �}�E�X���{�^���N���b�N�ł́A�����N�ʃ^�u�\���̏ꍇ�A�܂��^�u�ʒu������̂��ߐݒ�ł��Ȃ�...
		//} else {
		}
	}
  #endif

	m_bExPropLock	= FALSE;

	// Note: Some ActivateFrame would go mad, because MDI client window not setup yet.
	if (m_bNewWindow2) {				// delayed activation if OnNewWindow2 called
		ActivateFrameForNewWindowDelayed();
		m_bNewWindow2 = false;
	}

	// UH
	// �n�C���C�g���H���
  #if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
	//m_bNowHilight=FALSE;	// gae: ��x�K�p�����n�C���C�g�͖����I�ɉ��������܂ňێ������
  #else
	m_bNowHilight	= FALSE;
  #endif
	m_nPainBookmark = 0;
	m_strBookmark	= LPCOLESTR(NULL);

	{
		//�C�x���g���� - �i�r�Q�[�g
		DEVTS_TAB_NAVIGATE stn;
		stn.nIndex			 = m_MDITab.GetTabIndex(m_hWnd);
		stn.lpstrURL		 = (LPCTSTR) strURL;
		stn.lpstrTargetFrame = (LPCTSTR) strTargetFrameName;
		CPluginManager::BroadCast_PluginEvent(DEVT_TAB_NAVIGATE, stn.nIndex, (SPARAM) &stn);
	}
}



DWORD CChildFrame::_GetInheritedDLControlFlags()
{
	DWORD dwDLFlags;

	if (this &&  _check_flag(MAIN_EX_INHERIT_OPTIONS, CMainOption::s_dwMainExtendedStyle) )
		dwDLFlags = m_view._GetDLControlFlags();
	else
		dwDLFlags = CDLControlOption::s_dwDLControlFlags;

	// default
	return dwDLFlags;
}


//+++ �V�K�Ɏq�������Ƃ��́A���̎q���ɓn�� ExtendedStyle �����߂�...
DWORD CChildFrame::_GetInheritedExtendedStyleFlags()
{
	DWORD dwExFlags;

	if (this &&  _check_flag(MAIN_EX_INHERIT_OPTIONS, CMainOption::s_dwMainExtendedStyle) ) {
		dwExFlags = m_view._GetExtendedStypeFlags();
	  #if 1	//+++ �i�r�Q�[�g���b�N�Ɋւ��Ă͌p�����Ȃ�....
		dwExFlags &= ~DVS_EX_OPENNEWWIN;											//+++ off
		dwExFlags |= CDLControlOption::s_dwExtendedStyleFlags & DVS_EX_OPENNEWWIN;	//+++ ���ǁA�f�t�H���g�ݒ肪����΁A����𔽉f.
	  #endif
	} else {
		dwExFlags = CDLControlOption::s_dwExtendedStyleFlags;
	}

	// default
	return dwExFlags;
}



int CChildFrame::ActivateFrameForNewWindowDelayed(int nCmdShow /*= -1*/)
{
	dcfTRACE( _T("CChildFrame::ActivateFrameForNewWindowDelayed - now activating!\n") );

	HWND	hWndActive = MDIGetActive();
	if (hWndActive != NULL && CMainOption::s_dwMainExtendedStyle & MAIN_EX_NOACTIVATE_NEWWIN)
		nCmdShow = SW_SHOWNOACTIVATE;

	return ActivateFrame(nCmdShow);
}



int CChildFrame::ActivateFrame(int nCmdShow /*= -1*/)
{
	ATLASSERT( ::IsWindow(m_hWnd) );
	if (m_MDITab.GetTabIndex(m_hWnd) == -1) 	//�Ȃ񂾂�MDITab��MDIActivate�̕�����ɌĂяo����Ă��܂��̂�
		m_MDITab.OnMDIChildCreate(m_hWnd);		//���낢��Ə��Ԃ������Ă���

	if ( _check_flag(m_view.m_ViewOption.m_dwExStyle, DVS_EX_OPENNEWWIN)) {
		m_MDITab.NavigateLockTab(m_hWnd, true);
	}

	//Raise Plugin Event
	int nNewIndex = m_MDITab.GetTabIndex(m_hWnd);
	CPluginManager::ChainCast_PluginEvent(DEVT_TAB_OPENED, nNewIndex, 0);

	return baseClass::ActivateFrame(nCmdShow);
}


void CChildFrame::OnDocumentComplete(nsISupports *pSupp, const CString &strURL)
{
	if ( true/*IsPageIWebBrowser(pDisp)*/ ) {	// it means a page complete downloading	//COMMENT IsPageIWebBrowser
		dcfTRACE( _T("CChildFrame::OnDocumentComplete\n") );
		m_bWaitNavigateLock = false;	// you are allowed!
		m_bExPropLock		= FALSE;

		if ( MDIGetActive() == m_hWnd && DonutBrowserCanSetFocus(m_hWnd) ) {
			_SetPageFocus();
		}
	}

	//�߂�E�i�ނ̗��������[�h����
	SetTravelLogData();

	//�I�[�g�n�C���C�g
	if (m_bAutoHilight)
		CDonutSearchBar::GetInstance()->ForceSetHilightBtnOn(true);

	bool bHilight = m_bAutoHilight || CDonutSearchBar::GetInstance()->GetHilightSw();
	//if (bHilight && !m_bNowHilight)
	if (bHilight)
	{

	  #if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
		if (!m_strSearchWord.IsEmpty()){
			nsCOMPtr<nsIWebBrowser> wb = do_QueryInterface(pSupp);
			if(wb) OnHilightOnce(wb, m_strSearchWord);
		}
	  #else
		if (!m_strSearchWord.IsEmpty())
			OnHilight(m_strSearchWord);
	  #endif
	}

	//::PostMessage(GetTopLevelParent(), WM_COMMAND, (WPARAM)ID_SEARCHBAR_HILIGHT, 0);

	{
		//�v���O�C���C�x���g - ���[�h����
		nsCOMPtr<nsIWebBrowser> wb = do_QueryInterface(pSupp);
		CGeckoBrowser b = wb;
		if (wb) {
			DEVTS_TAB_DOCUMENTCOMPLETE	dc;
			dc.lpstrURL   = (LPCTSTR) strURL;
			dc.lpstrTitle = (LPCTSTR) b.GetLocationName();
			dc.nIndex	  = m_MDITab.GetTabIndex(m_hWnd);
			dc.bMainDoc   = true/*IsPageIWebBrowser(pDisp);*/; //COMMENT IsPageIWebBrowser
			CPluginManager::BroadCast_PluginEvent(DEVT_TAB_DOCUMENTCOMPLETE, dc.nIndex, (DWORD_PTR) &dc);
		}
	}
}



void CChildFrame::OnDownloadComplete()
{
	dcfTRACE( _T("CChildFrame::OnDownloadComplete\n") );
}



void CChildFrame::OnStatusTextChange(const CString &strText)
{
	{
		//�v���O�C���C�x���g - �X�e�[�^�X�e�L�X�g�ύX
		int nRet = CPluginManager::ChainCast_PluginEvent(DEVT_CHANGESTATUSTEXT, (FPARAM) (LPCTSTR) strText, 0);
		if (nRet < 0)
			return;
	}

	BOOL bFilter = FALSE;
	if (m_view.m_ViewOption.m_dwExStyle & DVS_EX_MESSAGE_FILTER) {
		if (strText.Find(_T("http"  )) != -1)						bFilter = TRUE;
		if (strText.Find(_T("ftp"	)) != -1)						bFilter = TRUE;
		if (strText.Find(_T("�ڑ�"  )) != -1)						bFilter = TRUE;
		if (strText.Find(_T("mailto")) != -1)						bFilter = TRUE;
		if (strText.Find(_T("file"  )) != -1)						bFilter = TRUE;
		if (strText.Find(_T("java"  )) != -1)						bFilter = TRUE;
		if ( strText.IsEmpty()	 )									bFilter = TRUE;
		if (strText.Compare(_T("�y�[�W���\������܂���")) == 0) 	bFilter = TRUE; 	//minit
		if (bFilter == FALSE)										return;
	}

	if (m_strStatusBar != strText)
		m_strStatusBar = strText;

	if (MDIGetActive() == m_hWnd) {
		CWindow wndFrame   = GetTopLevelParent();
		HWND	hWndStatus = wndFrame.GetDlgItem(ATL_IDW_STATUS_BAR);
		int 	nMode	   = (int) ::SendMessage(hWndStatus, WM_GET_OWNERDRAWMODE, 0, 0);
		if (nMode)
			MtlSetStatusText(hWndStatus, strText);
		else
			MtlSetWindowText(hWndStatus, strText);

		// UDT DGSTR ( update
		LPCTSTR lpszText   = strText;
		SendMessage(GetTopLevelParent(), WM_UPDATE_TITLEBAR, (WPARAM) lpszText, (LPARAM) 0);
		// ENDE
	}
}



void CChildFrame::OnTitleChange(const CString &strTitle)
{
	CString strURL = GetLocationURL();

	if ( CCloseTitlesOption::SearchString(strTitle) ) {
		m_bClosing = true;
		PostMessage(WM_CLOSE);
		{
			//�v���O�C���C�x���g - �^�C�g���}�~
			CPluginManager::BroadCast_PluginEvent(DEVT_BLOCKTITLE, (FPARAM) (LPCTSTR) strTitle, 0);
		}
		return;
	}

	dcfTRACE( _T("OnTitleChange : %s\n"), strTitle.Left(100) );
	// change title
	SetWindowText( MtlCompactString(strTitle, 255) );

  #if 0 //+++ �����ɁA�y�[�W���J������about:blank�ݒ���L�����Z������... +mod.1.17�ȑO�̕s��΍�ŁA�ǂ����ȍ~�ł͋A���ĕs��ɉ�����ꍇ�����肻�Ȃ�ŁA�~��.
	// change address bar
	if (MDIGetActive() == m_hWnd) {
		if ((strURL == "about:blank" || strURL.IsEmpty()) && IsWaitBeforeNavigate2Flag()) { //+++
			;
		} else {	//+++ �ʏ�.
			m_AddressBar.SetWindowText(strURL);
		}
	}
  #else
	// change address bar
	if (MDIGetActive() == m_hWnd)
		m_AddressBar.SetWindowText(strURL);
  #endif
}


void CChildFrame::OnCommandStateChange(long Command, bool bEnable)
{
	if (Command == CSC_NAVIGATEBACK) {
		m_bNavigateBack 	= bEnable /*? TRUE : FALSE*/;
	} else if (Command == CSC_NAVIGATEFORWARD) {
		m_bNavigateForward	= bEnable /*? TRUE : FALSE*/;
	} else if (Command == CSC_UPDATECOMMANDS) {
	}
}


void CChildFrame::OnStateConnecting()
{
	m_MDITab.SetConnecting(m_hWnd);
	m_bPrivacyImpacted = TRUE;	// �y�[�W�ǂݍ��݌��OnStateConnecting���Ă΂��y�[�W������̂�
								// �N�b�L�[�����A�C�R�����\������Ȃ��y�[�W������
	//m_nSecureLockIcon = secureLockIconUnsecure;
}


//void CChildFrame::OnStateDownloading()
//{
//	m_MDITab.SetDownloading(m_hWnd);
//}


void CChildFrame::OnStateCompleted()
{
	m_MDITab.SetComplete(m_hWnd);

	HWND hWndA2 = ::GetActiveWindow();
	HWND hWndF2 = ::GetFocus();

	if (m_hWndA)
		::SetActiveWindow(m_hWndA);

	if (m_hWndF)
		::SetFocus(m_hWndF);
}



#if 1 //+++ �����I�ɁA���̏�Ń��b�Z�[�W�����΂�...�����s�v����...
int CChildFrame::ForceMessageLoop()
{
	HWND	hWnd	= m_hWnd;
	HWND	hWndA = ::GetActiveWindow();
	if (hWnd != hWnd)
		return 1;
	int 	ret;
	MSG 	msg 	= {0};
	int 	n		= 0;
	while ((ret = ::PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE)) != 0) {
		if (!GetMessage (&msg,hWnd,0,0))	/* ���b�Z�[�W����. QUIT��������A���U������*/
			return 0;	//x return msg.wParam ;
		if (OnForwardMsg(&msg, 0) == 0)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		++n;
	}
	return 1;
}
#endif



// ===========================================================================
// �I������

/** +++����:�����ȊO�̃^�u�����
 */
LRESULT CChildFrame::OnWindowCloseExcept(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	if ( !CDonutConfirmOption::OnCloseAllExcept( GetTopLevelParent() ) )
		return 0;

	CWaitCursor 		 cur;
	CLockRedrawMDIClient lock(m_hWndMDIClient);
	CLockRedraw 		 lock2(m_MDITab);
	MtlCloseAllMDIChildrenExcept(m_hWndMDIClient, m_hWnd);
	return 0;
}



/** +++����:�^�u�̉E��/���������ׂĕ���
 */
LRESULT CChildFrame::OnLeftRightClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	bool	bLeft	= (wID == ID_LEFT_CLOSE);

	if ( !CDonutConfirmOption::OnCloseLeftRight( GetTopLevelParent(), bLeft ) )
		return 0;

	CWaitCursor cur;

	HWND		hWndActive = (HWND) ::SendMessage(m_hWndMDIClient, WM_MDIGETACTIVE, 0, (LPARAM) NULL);
	int 		nCurSel    = m_MDITab.GetTabIndex(m_hWnd);
	int 		nCurSel2   = m_MDITab.GetTabIndex(hWndActive);
	int 		nCurSel3   = m_MDITab.GetTabIndex(m_hWndMDIClient);

	if (nCurSel == -1)
		return 0;

	::SendMessage(GetTopLevelParent(), WM_USER_WINDOWS_CLOSE_CMP, (WPARAM) nCurSel, (LPARAM) bLeft);
	return 0;
}


/** �S�Ẵ^�u�����
 */
void CChildFrame::OnWindowCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	::SendMessage(::GetParent( ::GetParent(m_hWnd) ), WM_COMMAND, ID_WINDOW_CLOSE_ALL, 0);
}




///+++����:�u���E�U�����
void CChildFrame::OnBrowserClose()
{
	dcfTRACE( _T("CChildFrame::OnBrowserClose\n") );
	SetMsgHandled(FALSE);
	m_bClosing = true;
	PostMessage(WM_CLOSE);
}


/** +++����:�t�@�C���̕���.
 */
void CChildFrame::OnFileClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
  #if 1 //+++ �^�u���b�N�΍�... OnClose���̂����ő��v�̂悤������ǁA�Ƃ肠����.
	if ( s_bMainframeClose == 0 												//+++ unDonut�I�����ȊO��
		&& _check_flag(m_view.m_ViewOption.m_dwExStyle, DVS_EX_OPENNEWWIN)		//+++ �i�r�Q�[�g���b�N�̃y�[�W��
		&& (CMainOption::s_dwMainExtendedStyle2 & MAIN_EX2_NOCLOSE_NAVILOCK)	//+++ �i�r�Q�[�g���b�N�̃y�[�W����Ȃ��A�̎w�肪�����
	){
		m_bClosing = false;
		return; 																//+++ �����ɋA��
	}
  #endif
	m_bClosing = true;
	PostMessage(WM_CLOSE);
}



//======================

/** +++����:����
 */
void CChildFrame::OnClose()
{
  #if 1 //+++ �^�u���b�N�΍�...
	if ( s_bMainframeClose == 0 												//+++ unDonut�I�����ȊO��
		&& _check_flag(m_view.m_ViewOption.m_dwExStyle, DVS_EX_OPENNEWWIN)		//+++ �i�r�Q�[�g���b�N�̃y�[�W��
		&& (CMainOption::s_dwMainExtendedStyle2 & MAIN_EX2_NOCLOSE_NAVILOCK)	//+++ �i�r�Q�[�g���b�N�̃y�[�W����Ȃ��A�̎w�肪�����
	){
		m_bClosing = false;
		return; 																//+++ �����ɋA��
	}
  #endif

	SetMsgHandled(FALSE);
	m_bClosing = true;

	int  nIndex 		= m_MDITab.GetTabIndex(m_hWnd);

	// for mdi tab ctrl
	HWND hWndActiveNext = NULL;

	if (MDIGetActive() == m_hWnd) {
		int nIndex = m_MDITab.ManageClose(m_hWnd);

		if (nIndex != -1) {
			hWndActiveNext = m_MDITab.GetTabHwnd(nIndex);
			ATLASSERT( ::IsWindow(hWndActiveNext) );
		}
	}

	m_MDITab.OnMDIChildDestroy(m_hWnd);

	if (hWndActiveNext)
		MDIActivate(hWndActiveNext);

	if ( !CMainOption::s_bAppClosing ||  !(CStartUpOption::s_dwFlags == CStartUpOption::STARTUP_LATEST) ) { 	//minit
		CString strURL	 = GetLocationURL();
		if ( !strURL.IsEmpty() ) {	//+++ ��Ƀ`�F�b�N.
			CString strTitle = GetLocationName();
			SDfgSaveInfo::List	arrFore;
			SDfgSaveInfo::List	arrBack;

			if (CMainOption::s_bTravelLogClose) {
				m_view.m_ViewOption._OutPut_TravelLogs(arrFore, arrBack);
			}
			if (CMainOption::s_pMru && strURL.Left(12) != _T("view-source:"))	//+++
				CMainOption::s_pMru->AddToList(strURL, &arrFore, &arrBack, strTitle);
		}
	}

	{
		//�v���O�C���C�x���g - �N���[�Y
		CPluginManager::BroadCast_PluginEvent(DEVT_TAB_CLOSE, nIndex, 0);
	}

	//+++ destroy�� //+++ DonutRAPT���p�N( �������̗\��̈���ꎞ�I�ɍŏ����B�E�B���h�E���ŏ��������ꍇ�Ɠ��� )
	//+++           RtlSetMinProcWorkingSetSize();
}


/** �I���J��
 */
void CChildFrame::OnDestroy()
{
	dcfTRACE( _T("CChildFrame::OnDestroy\n") );
	SetMsgHandled(FALSE);
	m_bClosing							  = true;

	if (m_bOperateDrag)
		m_view.SetOperateDragDrop(FALSE, 0);

//	SetRegisterAsBrowser(false);

	// m_view.DestroyWindow makes offline state reset, so
	// save global offline state
	bool bGlobalOffline = MtlIsGlobalOffline();

	// Before AtlAdviseSinkMap(this, false) destroy view,
	// cuz I wanna handle OnNewWindow2 event
	// And while destroying view, a meaningless WM_DRAWCLIPBOARD may be sent, so ignore it.

	CMainOption::s_bIgnoreUpdateClipboard = true;
	dcfTRACE( _T(" m_view.Destroy\n") );

	if ( m_view.IsWindow() )
		m_view.DestroyWindow();

	CMainOption::s_bIgnoreUpdateClipboard = false;

	dcfTRACE( _T(" DispEventUnadvise\n") );

	m_view.DetachEventListener();

	// restore global offline state
	MtlSetGlobalOffline(bGlobalOffline);

	//+++ DonutRAPT���p�N( �������̗\��̈���ꎞ�I�ɍŏ����B�E�B���h�E���ŏ��������ꍇ�Ɠ��� )
	RtlSetMinProcWorkingSetSize();
}



CChildFrame::~CChildFrame()
{
	dcfTRACE( _T("CChildFrame::~CChildFrame\n") );

	if ( m_wndBrowser.IsWindow() )				// scary
		m_wndBrowser.UnsubclassWindow();

	ATLASSERT(m_hMenu == NULL); 				// no menu
}



// ===========================================================================
// ��{�I�ȏ���


int CChildFrame::_GetShowCmd()
{
	CWindowPlacement	wndpl;
	if ( m_hWnd && ::GetWindowPlacement(m_hWnd, &wndpl) ) {
		return wndpl.showCmd;
	}
	return -1;
}

#if 0	//+++	���s
void CChildFrame::OnLButtonDown(WORD wparam, const WTL::CPoint& pt)
{
	CRect	rect;
	GetClientRect(&rect);
	if (rect.PtInRect(pt)) {
		MtlSendCommand(m_hWnd, ID_HTMLZOOM_100TOGLE);
	}
}
#endif

//+++
LRESULT CChildFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	BOOL	bMaxActive = FALSE;
	HWND	hWndActive = MDIGetActive(&bMaxActive);

	if (bMaxActive == FALSE)
		bMaxActive = ::IsZoomed(hWndActive) != 0;

	int 	nCmdType   = m_nCmdType;
	m_nCmdType = 0;

   #ifndef USE_DIET
	//CheckImageAutoSize(NULL, FALSE);		//+++ ���̃^�C�~���O�ōX�V����͕̂s�����݂���.
	m_bImageAutoSizeReq = true;				//+++ �ʂ̃^�C�~���O�ŃT�C�Y�������郊�N�G�X�g
   #endif

	dcfTRACE(_T("CChildFrame::OnSize type(%d)\n"), wParam);

	if ( CMainOption::s_bTabMode && (wParam == SIZE_RESTORED || wParam == SIZE_MINIMIZED) ) {
		dcfTRACE( _T(" restored\n") );
		bHandled = TRUE;			// eat it!!
		return 0;
	} else if ( bMaxActive
				&& (   (nCmdType != SC_RESTORE && nCmdType != SC_MINIMIZE)
					&& (wParam == SIZE_RESTORED || wParam == SIZE_MINIMIZED) ) )
	{
		dcfTRACE( _T(" restored\n") );
		bHandled = TRUE;			// eat it!!
		return 0;
	} else {
		bHandled = FALSE;
		return -1;
	}
}


void CChildFrame::OnSysCommand(UINT uCmdType, CPoint pt)
{
  #if 1	//+++ CTRL(+SHIFT)+TAB�ł̃^�u�ړ��̏��Ԃ������ڒʂ�ɂ��鏈��
	if ((m_MDITab.GetMDITabExtendedStyle() & MTB_EX_CTRLTAB_MDI) == 0) {	//+++ ����MDI���ɂ���̂łȂ����
		if (uCmdType == SC_PREVWINDOW) {
			m_MDITab.LeftTab();
			SetMsgHandled(TRUE);
			return;
		} else if (uCmdType == SC_NEXTWINDOW) {
			m_MDITab.RightTab();
			SetMsgHandled(TRUE);
			return;
		}
	}
  #endif

	m_nCmdType = uCmdType;

	if ( CMainOption::s_bTabMode && (uCmdType == SC_MINIMIZE || uCmdType == SC_RESTORE) )
		SetMsgHandled(TRUE);
	else
		SetMsgHandled(FALSE);
}



LRESULT CChildFrame::OnForwardMsg(LPMSG pMsg, DWORD)
{
  #if 1	//+++	�摜�\���̂Ƃ��A�����ŃN���b�N������A�g�嗦��؂�ւ���
	if (m_bImg && m_bImgAuto_NouseLClk == 0 && pMsg->message == WM_LBUTTONDOWN /*&& pMsg->hwnd == m_hWnd*/) {
		CPoint 	pt;
		::GetCursorPos(&pt);
		CRect	rect;
		//CRect	rectB;
		::GetClientRect(pMsg->hwnd, &rect);
		//CClientDC dc(pMsg->hwnd);
		//::GetBoundsRect(dc, &rectB, 0/*DCB_RESET*/);
		ClientToScreen(&rect);
		rect.right -= GetSystemMetrics(SM_CXVSCROLL);
		rect.bottom-= GetSystemMetrics(SM_CYHSCROLL);
		if (rect.PtInRect(pt)) {
			MtlSendCommand(m_hWnd, ID_HTMLZOOM_100TOGLE);
		}
	}
  #endif

  #if 1	//+++	F5�Ń����[�h�����Ƃ��ɁAZOOM���߂��ꂿ�Ⴄ�̂��������邽�߂ɁA������蒲��
	if ( baseClass::PreTranslateMessage(pMsg) )
		return TRUE;
	LRESULT lr = m_view.PreTranslateMessage(pMsg);

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F5 && m_nImgScl != 100) {
		m_bImageAutoSizeReq = true;
		//SendMessage(WM_PAINT);
	  #if _ATL_VER >= 0x700		//+++ atl3�ł̓o�O�邱�ƂɂȂ邯�ǁA�������Ȃ�
		SetTimer(1, 20, OncePaintReq);
	  #endif
	}

	//if (m_ieToolBar.get())						//+++ ����.
	//	m_ieToolBar->PreTranslateMessage(pMsg); 	//+++ ����.

	return lr;
  #else
	if ( baseClass::PreTranslateMessage(pMsg) )
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
  #endif
}


//+++ �^�C�}�[�Ŗ������܂��āA��ʂ����T�C�Y�ŕ`��
void CALLBACK CChildFrame::OncePaintReq(HWND hWnd, UINT wparam, UINT_PTR lparam, DWORD )
{
	//m_bImageAutoSizeReq = true;
	::SendMessage(hWnd, WM_PAINT, wparam, lparam);
	::KillTimer(hWnd, 1);
}



// It will be a template method.
void CChildFrame::OnMDIActivate(HWND hwndChildDeact, HWND hwndChildAct)
{
	dcfTRACE( _T("CChildFrame::OnMDIActivate\n") );
	SetMsgHandled(FALSE);

	if (hwndChildAct == m_hWnd) {		// I'm activated
	  #if 0 //+++ �����ɁA�y�[�W���J������about:blank�ݒ���L�����Z������... +mod.1.17�ȑO�̕s��΍�ŁA�ǂ����ȍ~�ł͋A���ĕs��ɉ�����ꍇ�����肻�Ȃ�ŁA�~��.
		CString strURL = GetLocationURL();
		if ((strURL == "about:blank" || strURL.IsEmpty()) && IsWaitBeforeNavigate2Flag()) { //+++
			;
		} else {	//+++ �ʏ�.
			m_AddressBar.SetWindowText(strURL);
		}
	  #else
		m_AddressBar.SetWindowText( GetLocationURL() );
	  #endif
		_SetPageFocus();
		_RestoreFocus();
		return;
	}

	if (hwndChildDeact == m_hWnd) { 	// I'm deactivated
		_SaveFocus();
	}

	{
		//�v���O�C���C�x���g - �A�N�e�B�u�^�u�ύX
		int nIndexDeact = m_MDITab.GetTabIndex(hwndChildDeact);
		int nIndexAct	= m_MDITab.GetTabIndex(hwndChildAct);
		CPluginManager::BroadCast_PluginEvent(DEVT_TAB_CHANGEACTIVE, nIndexDeact, nIndexAct);
	}
}



///+++ ����:ID_VIEW_SETFOCUS:"���݂̃y�[�W�̃t�H�[�J�X�����������܂��B�t�H�[�J�X�̃��Z�b�g"
void CChildFrame::OnViewSetFocus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	// NOTE. _SetPageFoucs would fail if document not completed.
	m_bPageFocusInitialized = false;			// reset focus
	_SetPageFocus();
}



void CChildFrame::_SetPageFocus()
{
	if (m_bPageFocusInitialized)
		return;

	dcfTRACE( _T("CChildFrame::_SetPageFocus(src:%d)\n"), ::GetFocus() );

	if ( !MtlIsApplicationActive(m_hWnd) )
		return;

	if ( IsBrowserNull() )
		return;

	nsCOMPtr<nsIWebBrowserFocus> focus = do_QueryInterface(m_spBrowser);
    nsresult rv = focus->Activate();

	dcfTRACE( _T("focus->Activate(): %d\n"), rv );

	nsCOMPtr<nsIDOMWindow> win;
	m_spBrowser->GetContentDOMWindow(getter_AddRefs(win));

	focus->SetFocusAtFirstElement();

	/*nsCOMPtr<nsIWindowWatcher> ww = do_GetService("@mozilla.org/embedcomp/window-watcher;1");
	if(!ww) return;

	nsCOMPtr<nsIWebBrowserChrome> chrome;
	ww->GetChromeForWindow(win, getter_AddRefs(chrome));
	if(!chrome) return ;

	nsCOMPtr<nsIEmbeddingSiteWindow> siteWindow = do_QueryInterface(chrome);
	if(!siteWindow) return ;

	HWND *hWnd;
	siteWindow->GetSiteWindow((void**)&hWnd);

	::SetFocus(*hWnd);*/

	m_bPageFocusInitialized = true;
}






void CChildFrame::_RestoreFocus()
{
	if (m_hWndFocus && IsChild(m_hWndFocus) && ::GetFocus() != m_hWndFocus)
	{
		::SetFocus(m_hWndFocus);
	}
}

// ===========================================================================
// �ʊ֌W


#if 1	//+++ url�ʊg���v���p�e�B�̏���.
void CChildFrame::SetUrlSecurityExStyle(LPCTSTR lpszFile)
{
	DWORD opts  = 0;
	DWORD opts2 = 0;
	if (CUrlSecurityOption::FindUrl( lpszFile, &opts, &opts2, 0 )) {
		CExProperty  ExProp(CDLControlOption::s_dwDLControlFlags, CDLControlOption::s_dwExtendedStyleFlags, 0, opts, opts2);
		this->view().PutDLControlFlags( ExProp.GetDLControlFlags() );
		this->SetViewExStyle(ExProp.GetExtendedStyleFlags(), TRUE);
		this->view().m_ViewOption.SetAutoRefreshStyle( ExProp.GetAutoRefreshFlag() );
	}
}
#endif



/**+++����:ID_VIEW_REFRESH:"���݂̃y�[�W�̃R���e���c���ŐV�̏��ɍX�V���܂��B"
 */
void CChildFrame::OnViewRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	if (MDIGetActive() == m_hWnd) {
		m_hWndA = NULL;
		m_hWndF = NULL;
	} else {
		m_hWndA = ::GetActiveWindow();
		m_hWndF = ::GetFocus();
	}

	if (::GetAsyncKeyState(VK_CONTROL) < 0) 	// Inspired by DOGSTORE, Thanks
		Refresh2(REFRESH_COMPLETELY);
	else
		Refresh();
}



/**+++����:ID_VIEW_STOP:"���݂̃y�[�W�̓ǂݍ��݂𒆎~���܂��B"
 */
void CChildFrame::OnViewStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	Stop();
	// make sure
	//TODO: m_nDownloadCounter = 0;
	OnStateCompleted();
}

/**+++����:ID_FAVORITE_ADD:"���݂̃y�[�W�����C�ɓ���̈ꗗ�ɒǉ����܂��B"
 */
void CChildFrame::OnFavoriteAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	bool bOldShell = _check_flag(MAIN_EX_ADDFAVORITEOLDSHELL, CMainOption::s_dwMainExtendedStyle);

	MtlAddFavorite(GetLocationURL(), MtlGetWindowText(m_hWnd), bOldShell, DonutGetFavoritesFolder(), m_hWnd);

	::SendMessage(GetTopLevelParent(), WM_REFRESH_EXPBAR, 0, 0);
}


/**+++����:ID_FAVORITE_GROUP_ADD:"���̃y�[�W�����C�ɓ���O���[�v�ɒǉ����܂��B"
 */
void CChildFrame::OnFavoriteGroupAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	TCHAR		szOldPath[MAX_PATH];			// save current directory
	szOldPath[0]	= 0;	//+++
	::GetCurrentDirectory(MAX_PATH, szOldPath);

	CString		dir	= DonutGetFavoriteGroupFolder();
	::SetCurrentDirectory( dir );

	const TCHAR szFilter[] = _T("Donut Favorite Group�t�@�C��(*.dfg)\0*.dfg\0\0");
	CFileDialog fileDlg(FALSE, _T("dfg"), NULL, OFN_HIDEREADONLY, szFilter);
	fileDlg.m_ofn.lpstrInitialDir = dir;
	fileDlg.m_ofn.lpstrTitle	  = _T("���C�ɓ���O���[�v�ɒǉ�");

	if (fileDlg.DoModal() == IDOK) {
		_AddGroupOption(fileDlg.m_szFileName);
		::SendMessage(GetTopLevelParent(), WM_REFRESH_EXPBAR, 1, 0);
	}

	// restore current directory
	::SetCurrentDirectory(szOldPath);
}



/**+++�����F���C�ɓ���O���[�v�ɒǉ��y�[�W�̃I�v�V��������ǉ�
 */
int  CChildFrame::_AddGroupOption(const CString& strFileName)
{
	CIniFileIO	pr( strFileName, _T("Header") );
	DWORD		dwCount = pr.GetValue( _T("count"), 0 );
	OnSaveOption(strFileName, dwCount);
	++dwCount;
	pr.SetValue( dwCount, _T("count") );
	return dwCount;
}



/** �q���̃I�v�V��������ini�ɃZ�[�u
 */
void CChildFrame::OnSaveOption(LPCTSTR lpszFileName, int nIndex)
{
	m_view.m_ViewOption.WriteProfile(lpszFileName, nIndex);
}



//vvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// U.H
/**+++����:ID_WINDOW_REFRESH_EXCEPT:"���̃E�B���h�E�ȊO���X�V"
 */
LRESULT CChildFrame::OnWindowRefreshExcept(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	CWaitCursor 		 cur;
	CLockRedrawMDIClient lock(m_hWndMDIClient);
	CLockRedraw 		 lock2(m_MDITab);

	MtlRefreshAllMDIChildrenExcept(m_hWndMDIClient, m_hWnd);
	return 0;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


/**+++����:���C�ɓ���o�[�̍X�V
 */
LRESULT CChildFrame::OnViewRefreshFavBar(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	m_FavMenu.SetRootDirectoryPath( DonutGetFavoritesFolder() );
	m_FavMenu.RefreshMenu();
	return 0;
}


/**+++����:ID_EDIT_IGNORE:"�|�b�v�A�b�v�}�~�ɒǉ����ĕ��܂��B"
 */
void CChildFrame::OnEditIgnore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	dcfTRACE( _T("CChildFrame::OnEditIgnore\n") );
	CIgnoredURLsOption::Add( GetLocationURL() );
	m_bClosing = true;
	PostMessage(WM_CLOSE);
}


// UDT DGSTR ( close Title , but don't post WM_CLOSE so far. )
/**+++ ����:�^�C�g���}�~�ɒǉ�
 */
void CChildFrame::OnEditCloseTitle(WORD , WORD , HWND )
{
	ATLTRACE2( atlTraceGeneral, 4, _T("CChildFrame::OnEditCloseTitle\n") );
	CCloseTitlesOption::Add( MtlGetWindowText(m_hWnd) );
	m_bClosing = true;
	PostMessage(WM_CLOSE);
}
// ENDE



/** +++����:n�y�[�W�߂�
 */
void CChildFrame::OnViewBackX(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	int 		nStep = (wID - ID_VIEW_BACK1) + 1;
	CLockRedraw lock(m_hWnd);

	GoBack(nStep);
}



/** +++����:n�y�[�W�i��
 */
void CChildFrame::OnViewForwardX(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	int 		nStep = (wID - ID_VIEW_FORWARD1) + 1;
	CLockRedraw lock(m_hWnd);

    GoForward(nStep);
}



/**+++����:ID_TITLE_COPY:"�^�C�g�����N���b�v�{�[�h�ɃR�s�[���܂��B"
 */
void CChildFrame::OnTitleCopy(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	MtlSetClipboardText(MtlGetWindowText(m_hWnd), m_hWnd);
}



/**+++����:ID_URL_COPY:"�A�h���X���N���b�v�{�[�h�ɃR�s�[���܂��B"
 */
void CChildFrame::OnUrlCopy(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	MtlSetClipboardText(GetLocationURL(), m_hWnd);
}



/**+++����:ID_COPY_TITLEANDURL:"�^�C�g���ƃA�h���X���N���b�v�{�[�h�ɃR�s�[���܂��B"
 */
void CChildFrame::OnTitleAndUrlCopy(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	CString strText = MtlGetWindowText(m_hWnd) + _T("\r\n");

	strText += GetLocationURL();
	MtlSetClipboardText(strText, m_hWnd);
}



/** �v���O���X�y�C���̕���ݒ�
 */
BOOL CChildFrame::SetProgressPaneWidth(int cxWidth)
{
  #if _ATL_VER >= 0x700
	CStatusBarCtrl statusbar   = GetTopLevelParent().GetDlgItem(ATL_IDW_STATUS_BAR);
  #else //+++
	CStatusBarCtrl statusbar   = CWindow( GetTopLevelParent() ).GetDlgItem(ATL_IDW_STATUS_BAR);
  #endif

	const int		nPanes	   = 5;
	//+++ �v���O���X�y�C���ƃv���L�V�y�C���̌������̕s��C��.
	int 			nIndex	   = (g_bSwapProxy == 0) ? 1 : 4;

	// get pane positions
	int *			pPanesPos  = (int *) _alloca( nPanes * sizeof (int) );

	statusbar.GetParts(nPanes, pPanesPos);

	// calculate offset
	int 		   cxPaneWidth = pPanesPos[nIndex] - ( (nIndex == 0) ? 0 : pPanesPos[nIndex - 1] );
	int 		   cxOff	   = cxWidth - cxPaneWidth;

	// find variable width pane
	int 		   nDef 	   = 0;

	// resize
	if (nIndex < nDef) {						// before default pane
		for (int i = nIndex; i < nDef; i++)
			pPanesPos[i] += cxOff;
	} else {									// after default one
		for (int i = nDef; i < nIndex; i++)
			pPanesPos[i] -= cxOff;
	}

	// set pane postions
	return	statusbar.SetParts(nPanes, pPanesPos) != 0;
}



// ==========================================================================
// UI

// Update Command UI Handlers


#ifndef NO_STYLESHEET
void CChildFrame::OnStyleSheetBaseUI(CCmdUI *pCmdUI)
{
	if (pCmdUI->m_menuSub.m_hMenu) {		// popup menu
		pCmdUI->m_menu.EnableMenuItem(pCmdUI->m_nIndex, MF_BYPOSITION | MF_ENABLED);
	}
}
#endif


void CChildFrame::OnUpdateFontSmallestUI(CCmdUI *pCmdUI)
{
	nsCOMPtr<nsIDocShell> docShell(do_GetInterface(m_spBrowser));
	nsCOMPtr<nsIContentViewer> cv;
	docShell->GetContentViewer(getter_AddRefs(cv));
	nsCOMPtr<nsIMarkupDocumentViewer> mv = do_QueryInterface(cv);
	float zoom;
	mv->GetFullZoom(&zoom);
	pCmdUI->SetRadio(zoom == 0.5);
}


void CChildFrame::OnUpdateFontSmallerUI(CCmdUI *pCmdUI)
{
	nsCOMPtr<nsIDocShell> docShell(do_GetInterface(m_spBrowser));
	nsCOMPtr<nsIContentViewer> cv;
	docShell->GetContentViewer(getter_AddRefs(cv));
	nsCOMPtr<nsIMarkupDocumentViewer> mv = do_QueryInterface(cv);
	float zoom;
	mv->GetFullZoom(&zoom);
	pCmdUI->SetRadio(zoom == 0.75);
}


void CChildFrame::OnUpdateFontMediumUI(CCmdUI *pCmdUI)
{
	nsCOMPtr<nsIDocShell> docShell(do_GetInterface(m_spBrowser));
	nsCOMPtr<nsIContentViewer> cv;
	docShell->GetContentViewer(getter_AddRefs(cv));
	nsCOMPtr<nsIMarkupDocumentViewer> mv = do_QueryInterface(cv);
	float zoom;
	mv->GetFullZoom(&zoom);
	pCmdUI->SetRadio(zoom == 1.0);
}


void CChildFrame::OnUpdateFontLargerUI(CCmdUI *pCmdUI)
{
	nsCOMPtr<nsIDocShell> docShell(do_GetInterface(m_spBrowser));
	nsCOMPtr<nsIContentViewer> cv;
	docShell->GetContentViewer(getter_AddRefs(cv));
	nsCOMPtr<nsIMarkupDocumentViewer> mv = do_QueryInterface(cv);
	float zoom;
	mv->GetFullZoom(&zoom);
	pCmdUI->SetRadio(zoom == 1.5);
}


void CChildFrame::OnUpdateFontLargestUI(CCmdUI *pCmdUI)
{
	if (pCmdUI->m_menuSub.m_hMenu) { // popup menu
		pCmdUI->m_menu.EnableMenuItem(pCmdUI->m_nIndex, MF_BYPOSITION | MF_ENABLED);
	} else {
		nsCOMPtr<nsIDocShell> docShell(do_GetInterface(m_spBrowser));
		nsCOMPtr<nsIContentViewer> cv;
		docShell->GetContentViewer(getter_AddRefs(cv));
		nsCOMPtr<nsIMarkupDocumentViewer> mv = do_QueryInterface(cv);
		float zoom;
		mv->GetFullZoom(&zoom);
		pCmdUI->SetRadio(zoom == 2.0);
	}
}

void CChildFrame::OnUpdateStatusBarUI(CCmdUI *pCmdUI)
{
	pCmdUI->SetText(m_strStatusBar);
}


void CChildFrame::OnUpdateSecureUI(CCmdUI *pCmdUI)
{
	CStatusBarCtrl wndStatus = pCmdUI->m_wndOther;

	if (m_nSecureLockIcon != secureLockIconUnsecure) {
		wndStatus.SendMessage(WM_STATUS_SETICON, MAKEWPARAM(ID_SECURE_PANE, 0), 0);

		// �c�[���`�b�v�̃Z�b�g
		CString tip = _T("SSL �ی�t�� ");

		switch (m_nSecureLockIcon) {
		case secureLockIconUnsecure:			tip  = "";					break;
		case secureLockIconMixed:				tip += _T("(�����j");		break;
		case secureLockIconSecureUnknownBits:	tip += _T("(�s���j");		break;
		case secureLockIconSecure40Bit: 		tip += _T("(40�r�b�g)");	break;
		case secureLockIconSecure56Bit: 		tip += _T("(56�r�b�g)");	break;
		case secureLockIconSecureFortezza:		tip += _T("(Fortezza)");	break;
		case secureLockIconSecure128Bit:		tip += _T("(128�r�b�g)");	break;
		default:								tip  = "";
		}
		wndStatus.SendMessage( WM_STATUS_SETTIPTEXT, (WPARAM) ID_SECURE_PANE, (LPARAM) tip.GetBuffer(0) );

	} else {
		wndStatus.SendMessage(WM_STATUS_SETICON, MAKEWPARAM(ID_SECURE_PANE, -1), 0);
	}
}


void CChildFrame::OnUpdatePrivacyUI(CCmdUI *pCmdUI)
{
	CStatusBarCtrl wndStatus = pCmdUI->m_wndOther;

	if (m_bPrivacyImpacted == FALSE)
		wndStatus.SendMessage(WM_STATUS_SETICON, MAKEWPARAM(ID_PRIVACY_PANE, 1), 0);
	else
		wndStatus.SendMessage(WM_STATUS_SETICON, MAKEWPARAM(ID_PRIVACY_PANE, -1), 0);

	wndStatus.SendMessage( WM_STATUS_SETTIPTEXT, (WPARAM) ID_PRIVACY_PANE, (LPARAM) _T("�v���C�o�V�[ ���|�[�g") );
}


///+++ ����:�y�[�W�ǂݍ��݃v���O���X�o�[�̏�ԍX�V.
void CChildFrame::OnProgressChange(long progress, long progressMax)
{
	m_nProgress    = progress;
	m_nProgressMax = progressMax;
}



///+++ ����:�y�[�W�ǂݍ��݃v���O���X�o�[�̕\��
void CChildFrame::OnUpdateProgressUI(CCmdUI *pCmdUI)
{
	CProgressBarCtrl progressbar = pCmdUI->m_wndOther;
	CStatusBarCtrl	 statusbar	 = pCmdUI->m_wndOther.GetParent();


	if ( m_nProgress == -1 || (m_nProgress == 0 && m_nProgressMax == 0) ) {
		if (g_bSwapProxy == 0) {		//+++ �ʏ�(�����ɂ���)���̂ݕ���0�ɂ���. �v���L�V�ƈʒu���������Ă�Ƃ��͂��.
			SetProgressPaneWidth(0);
			progressbar.ShowWindow(SW_HIDE);
		} else {						//+++ �������Ă�Ƃ��́A�Ƃ肠�����A����0�ŏo�����ςȂ��őΏ�.
			progressbar.SetPos(0);		//+++ �������Q�[�W�o�[���̂͏����Ă���
		}
		return;
	}

	CRect			 rcProgressPart;
	if (g_bSwapProxy == false)
		statusbar.GetRect(1, rcProgressPart);
	else
		statusbar.GetRect(4, rcProgressPart);

	rcProgressPart.DeflateRect(2, 2);
	progressbar.MoveWindow(rcProgressPart, TRUE);
	progressbar.SetRange32(0, m_nProgressMax);
	//x 0�ɂȂ��Ă���Ƃ��ɖ��t���[��SetPos(0)����悤�ɂ����̂ŁA�����ł͕s�v
	//x (�Ƃ������A�������ł��ő�l�`�悪�s����悤�ɂ��邽�߁A�����ق����悢)
	//x if (m_nProgress >= m_nProgressMax && m_nProgressMax && g_bSwapProxy)
	//x progressbar.SetPos(0);
	//x else
	progressbar.SetPos(m_nProgress);
	progressbar.ShowWindow(SW_SHOWNORMAL);

	//+++ �T�C�Y�Œ�ŃI�v�V�����ݒ�ł̃T�C�Y�����f����Ă��Ȃ������̂ł���𔽉f. �O���[�o���ϐ��Ȃ̂͂Ƃ肠����...
	SetProgressPaneWidth( g_dwProgressPainWidth );
}



// ==========================================================================
// �g���v���p�e�B

BYTE CChildFrame::GetTabItemState(HWND hTarWnd)
{
	int  nTabIndex = m_MDITab.GetTabIndex(hTarWnd);
	BYTE bytData   = 0;

	m_MDITab.GetItemState(nTabIndex, bytData);
	return bytData;
}



LRESULT CChildFrame::OnGetExtendedTabStyle()
{
	DWORD dwFlags		 = 0;

	DWORD dwDLFlags 	 = m_view.GetDLControlFlags();
	DWORD dwViewExStyle  = m_view.m_ViewOption.m_dwExStyle;
	DWORD dwRefreshStyle = m_view.m_ViewOption.m_dwAutoRefreshStyle;

	if (dwDLFlags & GECKO_DLCTL_IMAGES) 				dwFlags |= FLAG_SE_IMAGE;
	if (dwDLFlags & GECKO_DLCTL_FRAMES) 				dwFlags |= FLAG_SE_FRAME;
	if (dwDLFlags & GECKO_DLCTL_PLUGINS) 				dwFlags |= FLAG_SE_PLUGIN;
	if (dwDLFlags & GECKO_DLCTL_SCRIPTS) 				dwFlags |= FLAG_SE_SCRIPT;
	if (dwDLFlags & GECKO_DLCTL_REDIR) 					dwFlags |= FLAG_SE_REDIR;
	if (dwDLFlags & GECKO_DLCTL_DNS) 					dwFlags |= FLAG_SE_DNS;

	if (dwViewExStyle & DVS_EX_OPENNEWWIN)			dwFlags |= FLAG_SE_NAVIGATELOCK;
	if (dwViewExStyle & DVS_EX_FLATVIEW)			dwFlags |= FLAG_SE_FLATVIEW;
	if (dwViewExStyle & DVS_EX_MESSAGE_FILTER)		dwFlags |= FLAG_SE_MSGFILTER;
	if (dwViewExStyle & DVS_EX_MOUSE_GESTURE)		dwFlags |= FLAG_SE_MOUSEGESTURE;
	if (dwViewExStyle & DVS_EX_BLOCK_MAILTO)		dwFlags |= FLAG_SE_BLOCKMAILTO;

	BYTE bytState = GetTabItemState(m_hWnd);
	if ( !(bytState & TCISTATE_INACTIVE) )			dwFlags |= FLAG_SE_VIEWED;

	if (dwRefreshStyle == 0)
		dwFlags |= FLAG_SE_REFRESH_NONE;
	else
		dwFlags |= ( dwRefreshStyle * ( FLAG_SE_REFRESH_15 / DVS_AUTOREFRESH_15SEC) );

	return dwFlags;
}



void CChildFrame::OnSetExtendedTabStyle(DWORD dwStyle)
{
	DWORD dwDLFlags 	 = m_view.GetDLControlFlags();
	DWORD dwViewExStyle  = m_view.m_ViewOption.m_dwExStyle;
	DWORD dwRefreshStyle = ( dwStyle /	( FLAG_SE_REFRESH_15 / DVS_AUTOREFRESH_15SEC) ) & DVS_AUTOREFRESH_OR;

	if (dwStyle & FLAG_SE_IMAGE) 		dwDLFlags |=  GECKO_DLCTL_IMAGES;
	else								dwDLFlags &= ~GECKO_DLCTL_IMAGES;
	if (dwStyle & FLAG_SE_FRAME) 		dwDLFlags |=  GECKO_DLCTL_FRAMES;
	else								dwDLFlags &= ~GECKO_DLCTL_FRAMES;
	if (dwStyle & FLAG_SE_PLUGIN) 		dwDLFlags |=  GECKO_DLCTL_PLUGINS;
	else								dwDLFlags &= ~GECKO_DLCTL_PLUGINS;
	if (dwStyle & FLAG_SE_SCRIPT) 		dwDLFlags |=  GECKO_DLCTL_SCRIPTS;
	else								dwDLFlags &= ~GECKO_DLCTL_SCRIPTS;
	if (dwStyle & FLAG_SE_REDIR) 		dwDLFlags |=  GECKO_DLCTL_REDIR;
	else								dwDLFlags &= ~GECKO_DLCTL_REDIR;
	if (dwStyle & FLAG_SE_DNS) 	 		dwDLFlags |=  GECKO_DLCTL_DNS;
	else								dwDLFlags &= ~GECKO_DLCTL_DNS;

	if (dwStyle & FLAG_SE_NAVIGATELOCK) 	dwViewExStyle |=  DVS_EX_OPENNEWWIN;
	else									dwViewExStyle &= ~DVS_EX_OPENNEWWIN;
	if (dwStyle & FLAG_SE_FLATVIEW) 		dwViewExStyle |=  DVS_EX_FLATVIEW;
	else									dwViewExStyle &= ~DVS_EX_FLATVIEW;			//* +++ ������ ~ ���Ȃ��̂̓��U�ƂȂ̂��o�O�Ȃ̂�? �����炭�o�O���낤��~���Ƃ�.
	if (dwStyle & FLAG_SE_MSGFILTER)		dwViewExStyle |=  DVS_EX_MESSAGE_FILTER;
	else									dwViewExStyle &= ~DVS_EX_MESSAGE_FILTER;
	if (dwStyle & FLAG_SE_MOUSEGESTURE) 	dwViewExStyle |=  DVS_EX_MOUSE_GESTURE;
	else									dwViewExStyle &= ~DVS_EX_MOUSE_GESTURE;
	if (dwStyle & FLAG_SE_BLOCKMAILTO)		dwViewExStyle |=  DVS_EX_BLOCK_MAILTO;
	else									dwViewExStyle &= ~DVS_EX_BLOCK_MAILTO;

	int nIndex = m_MDITab.GetTabIndex(m_hWndA);

	if (dwStyle & FLAG_SE_VIEWED)
		m_MDITab.SetItemActive(nIndex);
	else
		m_MDITab.SetItemInactive(nIndex);

	//�i�r�Q�[�g���b�N�̕ύX�𔽉f������
	// if ( (m_view.m_ViewOption.m_dwExStyle & DVS_EX_OPENNEWWIN) != (dwStyle & FLAG_SE_NAVIGATELOCK) )
	//+++ �Ȃɂ��悭�킩��ɂ�����������...DVS_EX_OPENNEWWIN=0x01, FLAG_SE_NAVIGATELOCK=0x80�Ȃ̂ŁA
	//+++ !=���U�ɂȂ�̂͗��ӂ�����0�̂Ƃ��̂�...�Ȃ̂ŁA�ǂ��炩��on�ɂȂ��Ă����if���̒��ɂ͂���.
	//+++ �Ƃ肠�����A���̂悤�ɋL�q���Ȃ���
	if ( (m_view.m_ViewOption.m_dwExStyle & DVS_EX_OPENNEWWIN) || (dwStyle & FLAG_SE_NAVIGATELOCK) )
	{
		m_MDITab.NavigateLockTab(m_hWnd, (dwStyle & FLAG_SE_NAVIGATELOCK) != 0/*? true : false*/);
	}

	m_MDITab.InvalidateRect(NULL);

	//�t���O��ύX����
	m_view.PutDLControlFlags(dwDLFlags);
	m_view.m_ViewOption.m_dwExStyle = dwViewExStyle;
	m_view.m_ViewOption.SetAutoRefreshStyle(dwRefreshStyle);

	//�ύX��K�p���邽�߂Ƀ��t���b�V������
	//m_view.Navigate2(m_view.GetLocationURL());
	if ( !(dwStyle & FLAG_SE_NOREFRESH) )
		::SendMessage(m_hWnd, WM_COMMAND, (WPARAM) (ID_VIEW_REFRESH & 0xFFFF), 0);
}



void CChildFrame::SetViewExStyle(DWORD dwStyle, BOOL bExProp /*= FALSE*/)
{
	bool bNavigateLock = (dwStyle & DVS_EX_OPENNEWWIN) != 0/*? true : false*/;

	m_MDITab.NavigateLockTab(m_hWnd, bNavigateLock);
	//m_view.m_dwDefaultExtendedStyleFlags	= dwStyle;			//+++
	m_view.m_ViewOption.m_dwExStyle = dwStyle;
	m_bExPropLock					= (bNavigateLock && bExProp) /*? true : false*/;
}



// ==========================================================================
// �i�ޖ߂�


void	CChildFrame::SetDfgFileNameSection(const CString &strFileName, const CString &strSection)
{
	m_strDfgFileName = strFileName;
	m_strSection	 = strSection;
}



void CChildFrame::SetArrayHist(
			std::vector< std::pair<CString, CString> > &	ArrayFore,
			std::vector< std::pair<CString, CString> > &	ArrayBack
){
	m_ArrayHistFore = ArrayFore;
	m_ArrayHistBack = ArrayBack;
}



void CChildFrame::SetTravelLogData()
{
	if (m_bInitTravelLog) {
		if (m_ArrayHistBack.size() == 0 && m_ArrayHistFore.size() == 0) {
			//ini����ǂݍ���
			std::vector<std::pair<CString, CString> > ArrFore;
			std::vector<std::pair<CString, CString> > ArrBack;
			_Load_TravelData(ArrFore, ArrBack, m_strDfgFileName, m_strSection);
			_Load_TravelLog(ArrFore, m_spBrowser, TRUE);
			_Load_TravelLog(ArrBack, m_spBrowser, FALSE);
		} else {
			//�z��̃f�[�^���g��
			_Load_TravelLog(m_ArrayHistFore, m_spBrowser, TRUE);
			_Load_TravelLog(m_ArrayHistBack, m_spBrowser, FALSE);
			m_ArrayHistBack.clear();
			m_ArrayHistFore.clear();
		}
	}
	m_bInitTravelLog = FALSE;
}



BOOL CChildFrame::_Load_TravelData(
	std::vector<std::pair<CString, CString> >&	arrFore,
	std::vector<std::pair<CString, CString> >&	arrBack,
	CString & 									strFileName,
	CString & 									strSection)
{
	if ( strFileName.IsEmpty() )
		return FALSE;

	if ( strSection.IsEmpty() )
		return FALSE;

	CIniFileI	pr(strFileName, strSection);
	CString 		strKey;
	for (unsigned i = 0; i < 0x7fffffff; ++i) {
		strKey.Format(_T("Fore_URL%d"), i);
		CString strURL = pr.GetString(strKey/*, NULL, MAX_PATH*/);
		if (strURL.IsEmpty())
			break;
		strKey.Format(_T("Fore_Title%d"), i);
		CString strTitle = pr.GetStringUW(strKey/*, NULL, 1024*/);
		if (strTitle.IsEmpty())
			break;
		arrFore.push_back( std::make_pair(strTitle, strURL) );
	}

	for (unsigned i = 0; i < 0x7fffffff; ++i) {
		strKey.Format(_T("Back_URL%d"), i);
		CString strURL = pr.GetString(strKey/*, NULL, MAX_PATH*/);
		if (strURL.IsEmpty())
			break;
		strKey.Format(_T("Back_Title%d"), i);
		CString strTitle = pr.GetStringUW(strKey/*, NULL, 1024*/);
		if (strTitle.IsEmpty())
			break;
		arrBack.push_back( std::make_pair(strTitle, strURL) );
	}

	return TRUE;
}

BOOL CChildFrame::_Load_TravelLog(
	std::vector<std::pair<CString, CString> >&	arrLog,
	nsCOMPtr<nsIWebBrowser>                     pWB,
	BOOL 										bFore)
{
#if COMMENT
	if (arrLog.size() == 0)
		return FALSE;

	HRESULT 					 hr;
	int 						 nDir;

	nDir   = bFore ? TLEF_RELATIVE_FORE : TLEF_RELATIVE_BACK;

	if (pWB2 == NULL)
		return FALSE;

	int		nLast	= ( 10 > arrLog.size() ) ? (int) arrLog.size() : 10;
	for (int i = 0; i < nLast; i++) {

		std::vector<wchar_t>	title = Misc::tcs_to_wcs( LPCTSTR( arrLog[i].first ) );
		std::vector<wchar_t>	url   = Misc::tcs_to_wcs( LPCTSTR( arrLog[i].second) );
	}
#endif
	return TRUE;
}



#if 0	//+++ UNICODE�Ή��̐܁A�s�v��
LPOLESTR CChildFrame::_ConvertString(LPCTSTR lpstrBuffer, int nBufferSize)
{
	LPOLESTR pszwDest = new OLECHAR[ nBufferSize ];

	::MultiByteToWideChar( CP_ACP, 0, lpstrBuffer, nBufferSize, pszwDest, nBufferSize * sizeof (OLECHAR) );
	return pszwDest;
}
#endif




// ===========================================================================
// �g��֌W

///+++
LRESULT CChildFrame::OnHtmlZoomMenu(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	::SetForegroundWindow(m_hWnd);
	CMenu/*Handle*/ 	menu0;
	menu0.LoadMenu(IDR_ZOOM_MENU);
	if (menu0.m_hMenu == NULL)
		return 0;
	CMenuHandle menu = menu0.GetSubMenu(0);
	if (menu.m_hMenu == NULL)
		return 0;

	// �|�b�v�A�b�v���j���[���J��.
	POINT 	pt;
	::GetCursorPos(&pt);
	HRESULT hr = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON| TPM_RETURNCMD, pt.x, pt.y, m_hWnd, NULL);
	if (hr)
		MtlSendCommand(m_hWnd, WORD(hr));
	return 0;
}



///+++
LRESULT CChildFrame::OnHtmlZoomAdd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	SetBodyStyleZoom(10, 0);
	return 0;
}



///+++
LRESULT CChildFrame::OnHtmlZoomSub(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	SetBodyStyleZoom(-10, 0);
	return 0;
}



///+++ 100% �Ƃ��̑��̔䗦���g�O���ؑ�.
LRESULT CChildFrame::OnHtmlZoom100Togle(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL &)
{
	if (m_nImgScl == 100) {
		m_nImgScl	 = m_nImgSclSav;
		m_nImgSclSw	 = 1;
	} else {
		m_nImgSclSav = m_nImgScl;
		m_nImgScl    = 100;
		m_nImgSclSw	 = 0;
	}
	SetBodyStyleZoom(0, m_nImgScl);
	return 0;
}



///+++
void CChildFrame::OnHtmlZoom(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	ATLASSERT(wID >= ID_HTMLZOOM_400 && wID <= ID_HTMLZOOM_050);
	static const int scls[] = { 400, 200, 150, 125, 100, 75, 50 };
	int  n = wID - ID_HTMLZOOM_400;
	if (n < 0 || n > ID_HTMLZOOM_050-ID_HTMLZOOM_400)
		return;
	SetBodyStyleZoom(0, scls[n]);
	return;
}



///+++ html���g��k��
void CChildFrame::SetBodyStyleZoom(int addSub, int scl)
{
	//TODO:
}

// ===========================================================================
// �X�^�C���V�[�g

#ifndef NO_STYLESHEET

LRESULT CChildFrame::OnStyleSheet(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	CString strSheet;

	nsCOMPtr<nsIDOMWindow> win;
	m_spBrowser->GetContentDOMWindow(getter_AddRefs(win));

	switch (wID) {
	case ID_STYLESHEET_BASE:
		SwitchStyleSheetInFrame(win, "_default");
		break;
	case ID_STYLESHEET_OFF:
		SwitchStyleSheetInFrame(win, "_nostyle");
		break;
	}


	//StyleSheet( strSheet, bOff, _T("") );
	//SetDefaultStyleSheet( _T("") );

	return 1;
}

void CChildFrame::SetDefaultStyleSheet(const CString& strStyleSheet)
{
	CIniFileIO	pr( g_szIniFileName, _T("StyleSheet") );
	DWORD		dwSetDefault = pr.GetValue( _T("SetDefault") );
	if (dwSetDefault)
		pr.SetString( strStyleSheet, _T("Default") );
}

//private:
void CChildFrame::ApplyDefaultStyleSheet()
{
	CIniFileI pr( g_szIniFileName, _T("StyleSheet") );
	CString 	strSheet = pr.GetString(_T("Default"));
	pr.Close();

	if ( !strSheet.IsEmpty() ) {
		//StyleSheet(MtlGetFileName(strSheet), FALSE, strSheet);
	}
}

#endif	// NO_STYLESHEET



// ===========================================================================
// �͈͑I���֌W


///+++ �t���[�����̊e�y�[�W�́A�I��͈͂̕�������擾(���ׂĂ��r�߂邪�A�󋵓I�Ɍ�����̂͂P�̂͂��j
struct Function_GetSelectedText {

	Function_GetSelectedText(CString& rSel) : m_rStrSelText(rSel) {;}

	void operator ()(nsIDOMWindow *pWin)
	{
		nsCOMPtr<nsISelection> sel;
		nsresult rv = pWin->GetSelection(getter_AddRefs(sel));
		if(NS_FAILED(rv)) return;
		PRInt32 rcount = 0;
		sel->GetRangeCount(&rcount);
		for(int i=0;i<rcount;i++){
			nsCOMPtr<nsIDOMRange> r;
			sel->GetRangeAt(i, getter_AddRefs(r));
			nsEmbedString str;
			r->ToString(str);
			m_rStrSelText += CString(str.get());
		}
	}

private:
	CString&		m_rStrSelText;
};


///+++ �I��͈͂����邩�ǂ�����Ԃ�
bool CChildFrame::HaveSelectedText() const
{
	if (this == 0)
		return false;
	CString 	strSelText;

	MtlForEachDOMWindow( m_spBrowser, Function_GetSelectedText(strSelText) );

	return strSelText.IsEmpty() == 0;
}


///+++ �I��͈͂̕�������擾(�t���[���Ή���)
CString CChildFrame::GetSelectedText()
{
	CString 	strSelText;
	//m_MDITab.SetLinkState(LINKSTATE_B_ON);

	MtlForEachDOMWindow( m_spBrowser, Function_GetSelectedText(strSelText) );

	//m_MDITab.SetLinkState(LINKSTATE_OFF);

	if (strSelText.IsEmpty()) {
		//+++ �I��͈͂��Ȃ��ꍇ�A�E�N���b�N���j���[�o�R�̂��Ƃ�����̂ŁA����΍��������...
		CCustomContextMenu* pMenu = CCustomContextMenu::GetInstance();
		if (pMenu && pMenu->GetContextMenuMode() == CONTEXT_MENU_ANCHOR && pMenu->GetAnchorUrl().IsEmpty() == 0) {
			//bstrText		= pMenu->GetAnchorUrl();
			//bstrLocationUrl = pMenu->GetAnchorUrl();
			strSelText = CString(pMenu->GetAnchorUrl());
		}
	}
	return strSelText;
}



//+++ _OpenSelectedText()��蕪��.
CString CChildFrame::GetSelectedTextLine()
{
	CString str = GetSelectedText();
	if (str.IsEmpty() == 0) {
		int 	n	= str.Find(_T("\r"));
		int 	n2	= str.Find(_T("\n"));
		if ((n < 0 || n > n2) && n2 >= 0)
			n = n2;
		if (n >= 0) {
			str = str.Left(n);
		}
	}
	return str;
}




struct CChildFrame::_Function_OpenSelectedRef {
private:
	HWND			m_hWnd;
	CChildFrame*	m_pChild;

public:
	_Function_OpenSelectedRef(HWND hWnd, CChildFrame* pChild) : m_hWnd(hWnd), m_pChild(pChild) { }

	void operator ()(nsIDOMWindow *pWin)
	{
		nsCOMPtr<nsISelection> sel;
		nsresult rv = pWin->GetSelection(getter_AddRefs(sel));
		if(NS_FAILED(rv)) return;

		nsCOMPtr<nsIDOMDocument> doc;
		rv = pWin->GetDocument(getter_AddRefs(doc));
		if(NS_FAILED(rv)) return;

		//�I��͈͂̃\�[�X�𓾂�D��΃p�X�ɕϊ����邽�߂�nsIDocumentEncoder���g��
		nsCOMPtr<nsIDocumentEncoder> docEncoder = do_CreateInstance(NS_HTMLCOPY_ENCODER_CONTRACTID, &rv);
		if(NS_FAILED(rv)) return;
		docEncoder->Init(doc, NS_LITERAL_STRING("text/html"),
			             nsIDocumentEncoder::OutputAbsoluteLinks |
                         nsIDocumentEncoder::OutputEncodeW3CEntities);
		docEncoder->SetSelection(sel);
		nsEmbedString context, info, html;
		docEncoder->EncodeToStringWithContext(context, info, html);

		dcfTRACE( _T("CChildFrame::_Function_OpenSelectedRef\n") );
		dcfTRACE(html.get());
		dcfTRACE( _T("\n") );

		//�e���|������HTMLDocument�𐶐�
		const nsCID hoge = NS_HTMLDOCUMENT_CID;
		nsCOMPtr<nsIDOMHTMLDocument> htmlDocument(do_CreateInstance(hoge, &rv));
		if(NS_FAILED(rv)) return;

		//html, body��}��
		nsCOMPtr<nsIDOMElement> element, body;
		nsCOMPtr<nsIDOMNode> dummy;
		rv = htmlDocument->CreateElement(NS_LITERAL_STRING("html"), getter_AddRefs(element));
		if(NS_FAILED(rv)) return;
		rv = htmlDocument->AppendChild(element, getter_AddRefs(dummy));
		if(NS_FAILED(rv)) return;
		rv = htmlDocument->CreateElement(NS_LITERAL_STRING("body"), getter_AddRefs(body));
		if(NS_FAILED(rv)) return;
		rv = element->AppendChild(body, getter_AddRefs(dummy));
		if(NS_FAILED(rv)) return;

		//�������؂����Ă���html�̒f�Ђ�}��
		nsCOMPtr<nsIDOMNSHTMLElement> nsbody = do_QueryInterface(body, &rv);
		if(NS_FAILED(rv)) return;
		rv = nsbody->SetInnerHTML(html); //�{���ɓ˂�����ő��v�Ȃ񂾂낤��(script�Ƃ����s����Ȃ����낤��)
		if(NS_FAILED(rv)) return;

		//�����N���J��
		nsCOMPtr<nsIDOMNodeList> l;
		htmlDocument->GetElementsByTagName(NS_LITERAL_STRING("a"), getter_AddRefs(l));
		PRUint32 count;
		l->GetLength(&count);
		nsCOMPtr<nsIDOMNode> node;
		nsCOMPtr<nsIDOMHTMLAnchorElement> a;
		//DWORD dwOpenFlags = DonutGetStdOpenFlag();
		DWORD dwOpenFlags = 0;
		for(unsigned int i=0;i<count;i++){
			l->Item(i, getter_AddRefs(node));
			a = do_QueryInterface(node);
			nsEmbedString str;
			a->GetHref(str);
			CString uri(str.get());
			if(uri.Left(4) == "http")
				DonutOpenFile(m_hWnd, uri, dwOpenFlags);
		}
	}
};


void CChildFrame::OnEditOpenSelectedRef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	dcfTRACE( _T("CChildFrame::OnEditOpenSelectedRef\n") );
	m_MDITab.SetLinkState(LINKSTATE_B_ON);

	MtlForEachDOMWindow( m_spBrowser, _Function_OpenSelectedRef(m_hWnd, this) );

	m_MDITab.SetLinkState(LINKSTATE_OFF);
}

void CChildFrame::OnEditOpenSelectedText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	dcfTRACE( _T("CChildFrame::OnEditOpenSelectedTitle\n") );
	CString strText = GetSelectedText();
	if (strText.IsEmpty())
		return;

	std::vector<CString> lines;
	lines.reserve(20);
	Misc::SeptTextToLines(lines, strText);
	size_t	size = lines.size();
  #if 1	//+++ �I�𕶎��񒆂�url�ۂ����̂��Ȃ�������A�����񌟍��ɂ��Ă݂�.
	bool	f	= 0;
	for (unsigned i = 0; i < size; ++i) {
		CString& strUrl = lines[i];
		f |= (strUrl.Find(_T(':')) >= 0) || (strUrl.Find(_T('/')) >= 0) || (strUrl.Find(_T('.')) >= 0) ||  (strUrl.Find(_T('\\')) >= 0);
		if (f)
			break;
	}
	if (f == 0) {	// url�ۂ������񂪂Ȃ�����...
		CDonutSearchBar*	pSearchBar = CDonutSearchBar::GetInstance();
		if (pSearchBar) {
			CEdit edit = pSearchBar->GetEditCtrl();
			edit.SendMessage(WM_CHAR, 'P');
			CString str		= lines[0];
			LPCTSTR		strExcept  = _T(" \t\"\r\n�@");
			str.TrimLeft(strExcept);
			str.TrimRight(strExcept);
			edit.SetWindowText(str);
			pSearchBar->SearchWeb();
			return;
		}
	}
  #endif
	for (unsigned i = 0; i < size; ++i) {
		CString& strUrl = lines[i];
		Misc::StrToNormalUrl(strUrl);		//+++ �֐���
		DonutOpenFile(m_hWnd, strUrl);
		::Sleep(10);						//+++ �Ȃ�ƂȂ��ǉ�... ����Ȃ�����...
	}
}

// ===========================================================================
// ����

//public:
void CChildFrame::searchEngines(const CString &strKeyWord )
{
	CString 	strSearchWord = strKeyWord;

	if (m_AddressBar.GetAddressBarExtendedStyle() & ABR_EX_SEARCH_REPLACE)
		strSearchWord.Replace( _T("�@"), _T(" ") );

	//_ReplaceCRLF(strSearchWord,CString(_T(" ")));
	//strSearchWord.Replace('\n',' ');
	//strSearchWord.Remove('\r');
	strSearchWord.Replace( _T("\r\n"), _T("") );

	CIniFileI	pr( g_szIniFileName, STR_ADDRESS_BAR );
	CString 		strEngin = pr.GetStringUW( _T("EnterCtrlEngin"), NULL, 256 );
	pr.Close();
	::SendMessage(GetTopLevelParent(), WM_SEARCH_WEB_SELTEXT, (WPARAM) (LPCTSTR) strSearchWord, (LPARAM) (LPCTSTR) strEngin);

	//CWebBrowserCommandHandler<CChildFrame>::searchEngines(strSearchWord);
}


#if 0
//private:
CString CChildFrame::_ReplaceCRLF(CString &str, CString &strRep)
{
	CString strRet;
	TCHAR	ch;
	int 	len = str.GetLength();
	for (int i = 0; i < len; i++) {
		ch = str.GetAt(i);

		if ( (ch & 0xFF) == 0xa ) {
			strRet += strRep;
		} else if ( (ch & 0xFF) == 0xd ) {

		} else {
			strRet += ch;
		}
	}

	return strRep;
}
#endif



//
// �y�[�W������
//
LRESULT CChildFrame::OnFindKeyWord(LPCTSTR lpszKeyWord, BOOL bFindDown)
{
	// �A�N�e�B�u�E�B���h�E�̎擾
	HWND	hWndActive = MDIGetActive();
	if (hWndActive == NULL)
		return 0;

	if (!m_spBrowser)
		return 0;
	nsCOMPtr<nsIDocShell> docShell(do_GetInterface(m_spBrowser));
	nsCOMPtr<nsIWebBrowserFind> find = do_GetInterface(docShell);
	if(!find) return 0;

	find->SetSearchString(lpszKeyWord);
	find->SetFindBackwards(!bFindDown);
	PRBool r;
	find->FindNext(&r);

	return 1;
}

#if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
// gae _Function_Hilight�ƈ����̈Ӗ����Ⴄ�̂Œ���
struct CChildFrame::_Function_Hilight2 {
	LPCTSTR 	m_lpszKeyWord;
	BOOL		m_bHilight;

	_Function_Hilight2(LPCTSTR lpszKeyWord, BOOL bHilight)
		: m_lpszKeyWord(lpszKeyWord), m_bHilight(bHilight)
	{
	}

	void operator ()(nsIDOMWindow* pWin)
	{
		if (m_bHilight) {
			if ( !FindHilight(pWin) ) {
				MakeHilight(pWin);
			}
		} else {
			RemoveHilight(pWin);
		}
	}


	void MakeHilight(nsIDOMWindow* pWin)
	{
		// // �ҋ@���̓J�[�\���������v�ɕύX����
		// HCURSOR hCursor 	= SetCursor( LoadCursor(NULL, IDC_WAIT) );

		// �L�[���[�h�̍ŏ��̈����擾
		CString		strKeyWord(m_lpszKeyWord);

		//+++ �P���؂�𒲐�
		LPCTSTR		strExcept	= _T(" \t\"\r\n�@");
		strKeyWord = _tcstok( (LPTSTR) strKeyWord.GetBuffer(0), LPCTSTR(strExcept) );
		strKeyWord.TrimLeft(strExcept);
		strKeyWord.TrimRight(strExcept);

		int 	nLightIndex = 0;
//		nsresult rv;
		
		nsCOMPtr<nsIDOMDocument> doc;
		nsCOMPtr<nsIDOMHTMLDocument> htmlDoc;
		nsCOMPtr<nsIDOMDocumentRange> docRange;
		nsCOMPtr<nsIDOMHTMLElement> body;
		nsCOMPtr<nsIDOMElement> elm, containerElm;
		nsCOMPtr<nsIDOMRange> foundRange, searchRange, start, end;
		nsCOMPtr<nsIFind> finder;
		nsCOMPtr<nsIDOMNode> dummy, container, parent, parent2;

		pWin->GetDocument(getter_AddRefs(doc));
		if(!doc) return;

		docRange = do_QueryInterface(doc);
		if(!docRange) return;

		htmlDoc = do_QueryInterface(doc);
		if(!htmlDoc) return;

		htmlDoc->GetBody(getter_AddRefs(body));

		docRange->CreateRange(getter_AddRefs(searchRange));
		docRange->CreateRange(getter_AddRefs(start));
		docRange->CreateRange(getter_AddRefs(end));


		finder = do_CreateInstance("@mozilla.org/embedcomp/rangefind;1");
		if(!finder) return;

		// �L�[���[�h����ɂȂ�܂ŌJ��Ԃ�
		while ( !strKeyWord.IsEmpty() ) {

			searchRange->SelectNode(body);
			start->SelectNode(body);
			start->Collapse(PR_TRUE);
			end->SelectNode(body);
			end->Collapse(PR_FALSE);

			do
			{
				finder->Find(strKeyWord, searchRange, start, end, getter_AddRefs(foundRange));
				if(foundRange){

					//Input�v�f�Ȃǂ̒��������ł��Ă��܂��̂ŁC��������o���ĉ������
					foundRange->GetStartContainer(getter_AddRefs(container));
					container->GetParentNode(getter_AddRefs(parent));
					if(parent) parent->GetParentNode(getter_AddRefs(parent2));
					containerElm = do_QueryInterface(parent2/*dummy*/);
					if(containerElm){
						nsEmbedString tagName;
						containerElm->GetTagName(tagName);
						dcfTRACE(tagName.get());
						if(!tagName.Compare(_T("textarea")) ||
 						   !tagName.Compare(_T("TEXTAREA")) ||
						   !tagName.Compare(_T("INPUT")) ||
						   !tagName.Compare(_T("input")) )
						{
							foundRange->Collapse(PR_FALSE);
							foundRange->CloneRange(getter_AddRefs(start));
							continue;//����
						}
					}

					//�F�Â�
					nsCOMPtr<nsIDOMDocumentFragment> fragment;
					foundRange->ExtractContents(getter_AddRefs(fragment));
					foundRange->DeleteContents();
					doc->CreateElement(NS_LITERAL_STRING("span"), getter_AddRefs(elm));
					elm->SetAttribute(NS_LITERAL_STRING("class"), NS_LITERAL_STRING("donut_g_hilight"));
					elm->SetAttribute(NS_LITERAL_STRING("style"), nsEmbedString(g_lpszLight[nLightIndex]));
					elm->AppendChild(fragment, getter_AddRefs(dummy));
					foundRange->InsertNode(elm);

					//���̌����͈͂̐ݒ�
					start->SetStartAfter(elm);
					start->Collapse(PR_TRUE);
				}
			}while(foundRange);

			++nLightIndex;

			if (nLightIndex >= g_LIGHTMAX)
				nLightIndex = 0;

			strKeyWord = _tcstok(NULL, LPCTSTR(strExcept) );
			strKeyWord.TrimLeft(strExcept);
			strKeyWord.TrimRight(strExcept);
		}

		// // �J�[�\�������ɖ߂�
		// SetCursor(hCursor);
	}

	void RemoveHilight(nsIDOMWindow* pWin)
	{
		nsCOMPtr<nsIDOMDocument> doc;
		pWin->GetDocument(getter_AddRefs(doc));
		if(!doc) return;

		nsCOMPtr<nsIDOMDocumentRange> docRange;
		docRange = do_QueryInterface(doc);
		if(!docRange) return;

		nsCOMPtr<nsIDOMRange> range;
		docRange->CreateRange(getter_AddRefs(range));
		if(!range) return;

		nsCOMPtr<nsIDOMNodeSelector> nodeSelector = do_QueryInterface(doc);
		if(!nodeSelector) return;

		nsEmbedString cls(_T(".donut_g_hilight"));
		nsCOMPtr<nsIDOMNode> node;
		nsCOMPtr<nsIDOMNodeList> list;
		nodeSelector->QuerySelectorAll(cls, getter_AddRefs(list));
		PRUint32 len;
		list->GetLength(&len);
		for(int i=len-1;i>=0;i--){
			list->Item(i, getter_AddRefs(node));
			range->SelectNodeContents(node);
			nsCOMPtr<nsIDOMDocumentFragment> fragment;
			range->ExtractContents(getter_AddRefs(fragment));
			range->SetStartBefore(node);
			range->InsertNode(fragment);
			range->SelectNode(node);
			range->DeleteContents();
		}
	}

	// �n�C���C�g�����łɂ���Ă��邩�m�F����
	BOOL FindHilight(nsIDOMWindow* pWin)
	{
		nsCOMPtr<nsIDOMDocument> doc;
		pWin->GetDocument(getter_AddRefs(doc));
		if(!doc) return FALSE;

		nsCOMPtr<nsIDOMNodeSelector> nodeSelector = do_QueryInterface(doc);
		if(!nodeSelector) return FALSE;

		nsEmbedString cls(_T(".donut_g_hilight"));
		nsCOMPtr<nsIDOMElement> elm;
		nodeSelector->QuerySelector(cls, getter_AddRefs(elm));
		if(elm) return TRUE;
		else return FALSE;
	}
};
#endif	// KIKE_UNDONUT_G





struct CChildFrame::_Function_SelectEmpt {
	_Function_SelectEmpt(){ }

	void operator ()(nsIDOMWindow *pWin)
	{
		nsCOMPtr<nsISelection> sel;
		pWin->GetSelection(getter_AddRefs(sel));
		sel->CollapseToStart();
	}
};

//+++
void CChildFrame::SetSearchWordAutoHilight( const CString& str, bool autoHilightSw )
{
	m_strSearchWord = str;
	m_bAutoHilight	= autoHilightSw ;
}

//vvv
// UH
LRESULT CChildFrame::OnHilight(LPCTSTR lpszKeyWord)	//+++ ���g�p�Ȉ������폜. , BOOL bToggle /*= TRUE*/, BOOL bFlag /*= FALSE*/)
{
	HWND hWndActive = MDIGetActive();
	if (hWndActive == NULL)
		return 0;

  #if 0	//+++ �������g�p�̎d�g�݂��폜
	if (!bToggle)
		m_bNowHilight = bFlag;
  #endif
  #if 1	//+++
	bool bHilightSw = CDonutSearchBar::GetInstance()->GetHilightSw();
	//if (m_bNowHilight != bHilightSw)
	{
		//MtlForEachDOMWindow( m_spBrowser, _Function_SelectEmpt() );
		//MtlForEachDOMWindow( m_spBrowser, _Function_Hilight(lpszKeyWord, m_bNowHilight) );
		m_bNowHilight	= bHilightSw;
		if (bHilightSw)
			m_strSearchWord = lpszKeyWord;
		else
			m_strSearchWord.Empty();

		MtlForEachDOMWindow(m_spBrowser, _Function_SelectEmpt());
		MtlForEachDOMWindow(m_spBrowser, _Function_Hilight2(lpszKeyWord, bHilightSw));

	}
  #elif 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
   #if 1	//+++
	m_bNowHilight = CDonutSearchBar::GetInstance()->GetHilightSw();
   #else
	m_bNowHilight = !m_bNowHilight;
   #endif
	if (m_bNowHilight)
		m_strSearchWord = lpszKeyWord;
	else
		m_strSearchWord.Empty();

	MtlForEachDOMWindow(m_spBrowser, _Function_SelectEmpt());
	MtlForEachDOMWindow(m_spBrowser, _Function_Hilight2(lpszKeyWord, m_bNowHilight));

  #else

	MtlForEachDOMWindow( m_spBrowser, _Function_SelectEmpt() );
	MtlForEachDOMWindow( m_spBrowser, _Function_Hilight(lpszKeyWord, m_bNowHilight) );

	+++ m_bNowHilight = !m_bNowHilight;
  #endif
	return 1;
}


#if 1 //def USE_UNDONUT_G_SRC		//+++ gae����unDonut_g �̏����𔽉f���Ă݂�ꍇ.
LRESULT CChildFrame::OnHilightOnce(nsIWebBrowser *pWb, LPCTSTR lpszKeyWord)
{
	if (pWb!=NULL) {
		nsCOMPtr<nsIDOMWindow> win;
		pWb->GetContentDOMWindow(getter_AddRefs(win));
		if(win){

			_Function_SelectEmpt	se;
			se(win);

			_Function_Hilight2		hl(lpszKeyWord, TRUE);
			hl(win);

		}
	}
	return 1;
}
#endif



#if 0	//+++
void CChildFrame::OnSearchWeb_engineId(UINT code, int id, HWND hWnd)
{
	ATLASSERT(ID_INSERTPOINT_SEARCHENGINE <= id && id <= ID_INSERTPOINT_SEARCHENGINE_END);
	MtlSendCommand(CDonutSearchBar::GetInstance()->m_hWnd, WORD(id));
}
#endif

