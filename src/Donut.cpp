/**
 *	@file	Donut.cpp
 *	@brief	main source file for Donut.exe
 */

#include "stdafx.h"
#include "resource.h"

#include "DonutDefine.h"
#include "ItemIDList.h"
#include "mtl.h"

#include "ExMenu.h"
#include "ToolTipManager.h"
#include "DonutPFunc.h"

#include "DonutFavoritesMenu.h"
#include "DonutOptions.h"
#include "DonutSecurityZone.h"
#include "DonutSimpleEventManager.h"

// drafts
#include "DialogKiller.h"
#include "ReBarVerticalBorder.h"
#include "FileNotification.h"
#include "FlatComboBox.h"
// ENDED

#include "Donut.h"
#include "MainFrameFileDropTarget.h"
#include "DonutView.h"
#include "DonutClipboardBar.h"
#include "DonutRebarCtrl.h"
#include "FavTreeViewCtrl.h"
#include "DonutPanelBar.h"
#include "PluginBar.h"

#include "DonutExplorerBar.h"
#include "DonutLinksBarCtrl.h"
#include "DonutToolBar.h"
#include "DonutStatusBarCtrl.h"

#include "AddressBar.h"
#include "SearchBar.h"
#include "MDITabCtrl.h"

#include "FavoriteMenu.h"

#include "initguid.h"

#include "ie_feature_control.h"
//+++ #include "FileCriticalSection.h"				//+++ �ʂ̕��@�ɂ���
#include "MDIChildUserMessenger.h"
#include "dialog/aboutdlg.h"
#include "ChildFrame.h"								//+++ "ChildFrm.h"
#include "MainFrame.h"								//+++ "MainFrm.h"

#include "ParseInternetShortcutFile.h"				//+++

//#include "API.h"

#ifdef USE_ATL3_BASE_HOSTEX /*_ATL_VER < 0x700*/	//+++
#include "for_ATL3/AtlifaceEx_i.c"
#endif

#if defined USE_ATLDBGMEM
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "mozilla/XPCOM.h"

// Ini file name
TCHAR				g_szIniFileName[MAX_PATH];

//plus7: Whole���Ƃ��Ȃ���̈Ⴂ�͉����H
extern const UINT	g_uDropDownCommandID[] = {
	ID_FILE_NEW,
	ID_VIEW_BACK,
	ID_VIEW_FORWARD,
	ID_FILE_NEW_CLIPBOARD2,
	ID_DOUBLE_CLOSE,
	ID_DLCTL_CHG_MULTI,
	ID_DLCTL_CHG_SECU,
	ID_COOKIE,
	ID_RECENT_DOCUMENT
};

extern const UINT	g_uDropDownWholeCommandID[] = {
	ID_VIEW_FONT_SIZE,
	ID_FAVORITES_DROPDOWN,
	ID_AUTO_REFRESH,
	ID_TOOLBAR,
	ID_EXPLORERBAR,
	ID_MOVE,
	ID_OPTION,
	ID_COOKIE,
	ID_FAVORITES_GROUP_DROPDOWN,
	ID_CSS_DROPDOWN
};


extern const int	g_uDropDownCommandCount 	 = sizeof (g_uDropDownCommandID 	) / sizeof (UINT);
extern const int	g_uDropDownWholeCommandCount = sizeof (g_uDropDownWholeCommandID) / sizeof (UINT);

bool				g_bNoReposition 		  = FALSE;

CServerAppModule	_Module;
CMainFrame *		g_pMainWnd				  = NULL;
//CAPI *				g_pAPI					  = NULL;


/*
BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_API, CAPI)
END_OBJECT_MAP()
*/

static void CommandLineArg(CMainFrame& wndMain, LPTSTR lpstrCmdLine);



#if 1	//+++	�N����������ςȂ̃`�F�b�N.
static bool CheckOneInstance(HINSTANCE hInstance, LPTSTR lpstrCmdLine)
{
	//ChangeWindowMessageFilter(WM_NEWINSTANCE, MSGFLT_ADD); //+++ ���[�U�[���b�Z�[�W��ʉ߂����邽�߂�vista�ł̊J�����͕K�v?(�āA�������������Ȃ�����)
	TCHAR	iniFilePath[MAX_PATH+16];
	iniFilePath[0] = '\0';
	::GetModuleFileName(hInstance, iniFilePath, MAX_PATH);
	size_t	l = ::_tcslen(iniFilePath);
	if (l < 5)
		return false;
	::_tcscpy(&iniFilePath[l-4], _T(".ini"));
	DWORD dwMainExtendedStyle = ::GetPrivateProfileInt(_T("Main"), _T("Extended_Style"), 0xABCD0123, iniFilePath);
	if (dwMainExtendedStyle != 0xABCD0123)
		CMainOption::s_dwMainExtendedStyle = dwMainExtendedStyle;
	if (CMainOption::s_dwMainExtendedStyle & MAIN_EX_ONEINSTANCE) { // guarantee one instance
		//x HWND hWnd = ::FindWindow(_T("WTL:Donut"), NULL);
		HWND hWnd = ::FindWindow(DONUT_WND_CLASS_NAME, NULL);

		if (hWnd) {
			ATOM nAtom = ::GlobalAddAtom(lpstrCmdLine);
			::PostMessage(hWnd, WM_NEWINSTANCE, (WPARAM) nAtom, 0);
			//ErrorLogPrintf("NewInstance(hWnd=%#x atom=%d) %s\n", hWnd, nAtom, lpstrCmdLine);
			return true;
		}
	}
	return false;
}
#endif



static bool _PrivateInit( /*const*/ CString /*strCmdLine*/)
{
	MtlIniFileNameInit(g_szIniFileName, _MAX_PATH);

	CMainOption::GetProfile();
	CDLControlOption::GetProfile();
	CIgnoredURLsOption::GetProfile();
	CCloseTitlesOption::GetProfile();
	CFileNewOption::GetProfile();
	CStartUpOption::GetProfile();
	CUrlSecurityOption::GetProfile();	//+++
	CDonutConfirmOption::GetProfile();
   #ifndef NO_STYLESHEET
	CStyleSheetOption::GetProfile();
   #endif
	ie_feature_control_setting();		//+++

	CFavoritesMenuOption::Init();

	CSkinOption::GetProfile();
  #if 0	//+++ atltheme_d.h�̎g�p����߂�
	CThemeDLLLoader::LoadThemeDLL();
  #endif
	CExMenuManager::Initialize();

	return true;
}



void _PrivateTerm()
{
	CMainOption::WriteProfile();
	CDLControlOption::WriteProfile();
	CIgnoredURLsOption::WriteProfile();
	CCloseTitlesOption::WriteProfile();
	CUrlSecurityOption::WriteProfile();	//+++
	CFileNewOption::WriteProfile();
	CStartUpOption::WriteProfile();
	CDonutConfirmOption::WriteProfile();
   #ifndef NO_STYLESHEET
	CStyleSheetOption::WriteProfile();
   #endif
	CFavoritesMenuOption::Term();

  #if 0	//+++ atltheme_d.h�̎g�p����߂�
	CThemeDLLLoader::UnLoadThemeDLL();
  #endif
	CExMenuManager::Terminate();

	// don't forget
	CHlinkDataObject::Term();
}



#ifndef USE_DIET
///+++ unDonut.ini�t�@�C�����Ȃ��ꍇ�ɁA���̊��t�@�C��������Ă��邩���`�F�b�N.
static bool	HaveEnvFiles()
{
	//CString 	strIniFile;
	//strIniFile.LoadString(IDR_MAINFRAME);
	//strIniFile += ".ini";
	if (Misc::IsExistFile( Misc::GetFullPath_ForExe(g_szIniFileName/*strIniFile*/) ))
		return true;	// unDonut.ini ����������Ă�����A�Ƃ肠����ok�Ƃ��Ƃ�.

	//if (IsExistFile( GetFullPath_ForExe(_T("KeyBoard.ini"))) == 0)
	//	return false;
	if (Misc::IsExistFile( Misc::GetFullPath_ForExe(_T("MouseEdit.ini"))) == 0)
		return false;
	if (Misc::IsExistFile( Misc::GetFullPath_ForExe(_T("Menu.ini"))) == 0)
		return false;
	if (Misc::IsExistFile( Misc::GetFullPath_ForExe(_T("search\\search.ini"))) == 0)
		return false;
	//if (IsExistFile( GetFullPath_ForExe(_T("menu\\usermenu.txt"))) == 0)
	//	return false;
	return true;
}
#endif
static int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT, bool bTray=false)
{
	CMessageLoop theLoop;

	_Module.AddMessageLoop(&theLoop);

	CMainFrame	 wndMain;

	if (wndMain.CreateEx() == NULL) {
		ATLTRACE( _T("Main window creation failed!\n") );
		return 0;
	}

	// g_pMainWnd = &wndMain;	//+++ CreateEx()���Ƀv���O�C���������Ƃ��ŎQ�Ƃ����̂ŁACMainFreame�Őݒ肷��悤�ɕύX.

	// load windowplacement
	wndMain.startupMainFrameStayle(nCmdShow, bTray);

	_Module.Lock();

	if (lpstrCmdLine == 0 || lpstrCmdLine[0] == 0) {	// no command line param
	  #if 1 //+++ �X�^�[�g�y�[�W�̕\�����s���O�ɁA���̏������Ȃ�ׂ��s�킹��.
		if (ForceMessageLoop() == 0)
			return 0;				// ���̒i�K�ŏI�����ꂿ����Ă���A��
	  #endif
		CStartUpOption::StartUp(wndMain);
	} else {
		if (CStartUpOption::s_dwParam)
			CStartUpOption::StartUp(wndMain);
		CommandLineArg(wndMain, lpstrCmdLine);
	}

  #if 1 //+++
   #if 1	//+++ CMainFrame�������ɍs���Ă����A�o�b�N�A�b�v�����̊J�n���A���̃^�C�~���O�ɂ��炵�Ă݂�.
	//+++ �X�^�[�g�y�[�W�̕\�����A��s���郁�b�Z�[�W���ɂ��΂��Ă���.
	//	  ...�Ȃ�ׂ��ŏ��̃y�[�W�\���̏���������ł���A�����X�V�J�n(�ɂȂ��Ăق���..�Ȃ��Ă��Ȃ������)
	if (ForceMessageLoop() == 0)
		return 0;						// ���̒i�K�ŏI�����ꂿ����Ă���A��
	g_pMainWnd->SetAutoBackUp();		//�����X�V����Ȃ�A�J�n.
   #endif

	RtlSetMinProcWorkingSetSize();		//+++ ( �������̗\��̈���ꎞ�I�ɍŏ����B�E�B���h�E���ŏ��������ꍇ�Ɠ��� )

	InitWindowCreator();

	// ���ۂ̃��C�����[�v.
	int nRet = -1;
	try {
		nRet = theLoop.Run();
	} catch (...) {
		ErrorLogPrintf(_T("Donut�̃��C�����[�v�ŗ�O����\n"));
		ATLASSERT(0);
	}

	try {
		_Module.RemoveMessageLoop();
	} catch (...) {
		ErrorLogPrintf(_T("_Module.RemoveMessageLoop�ŗ�O����\n"));
		ATLASSERT(0);
	}
  #else
	int nRet = theLoop.Run();
	_Module.RemoveMessageLoop();
  #endif

	return nRet;		// �� WTL�̃��C�����N���[�Y����1�𐳏�l�Ƃ��ĕԂ��Ă���̂Œ���.
}


static void CommandLineArg(CMainFrame& wndMain, LPTSTR lpstrCmdLine)
{
	// UDT DGSTR�@( dai
	CString 	 strCmdLine(lpstrCmdLine);
	if (strCmdLine.CompareNoCase( _T("/dde") ) != 0) {	// it's not from dde. (if dde, do nothing.)
		ATLTRACE(_T("it's not from dde!(%s)\n"), strCmdLine);
		// UDT DGSTR ( Shell Open )
		// in dup running , check WM_NEWINSTANCE.
		// fixed by JOBBY . thx ( local check , remove quota.
		int nPos = 0;
		if (strCmdLine.Left(4).CompareNoCase( _T("http") ) == 0) {
			while (1) {
				nPos	   = strCmdLine.Find( _T(" ") );
				if (nPos == -1) {
					wndMain.OnUserOpenFile(strCmdLine , 0);
					break;
				} else
					wndMain.OnUserOpenFile(strCmdLine.Left(nPos) , 0);
				strCmdLine = strCmdLine.Mid(nPos + 1);
			}
		} else {
			// remove first & last double-quotation
			if (strCmdLine.GetAt(0) == '"')
				strCmdLine = strCmdLine.Mid(1, strCmdLine.GetLength() - 2);
			if ((strCmdLine[0] == '-' || strCmdLine[0] == '/') && strCmdLine.Mid(1,4).CompareNoCase(_T("tray")) == 0)	//+++ -tray�I�v�V�������X�L�b�v
				;
			else
				wndMain.OnUserOpenFile( strCmdLine, DonutGetStdOpenFlag() );
		}
	}
}




static HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription)
{
	ICatRegister *pcr = NULL;
	HRESULT 	  hr  = S_OK ;

	hr	= CoCreateInstance(CLSID_StdComponentCategoriesMgr,NULL,CLSCTX_INPROC_SERVER,IID_ICatRegister,(void **) &pcr);
	if ( FAILED(hr) )
		return hr;

	// Make sure the HKCR\Component Categories\{..catid...}
	// key is registered.
	CATEGORYINFO  catinfo;
	catinfo.catid	= catid;
	catinfo.lcid	= 0x0409 ; // english

	// Make sure the provided description is not too long.
	// Only copy the first 127 characters if it is.
	int 		  len = (int) wcslen(catDescription);
	if (len > 127)
		len = 127;

	wcsncpy(catinfo.szDescription, catDescription, len);

	// Make sure the description is null terminated.
	catinfo.szDescription[len] = '\0';

	hr	= pcr->RegisterCategories(1, &catinfo);
	pcr->Release();

	return hr;
}



static HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	// Register your component categories information.
	ICatRegister *pcr = NULL ;
	HRESULT 	  hr  = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void **) &pcr);
	if ( SUCCEEDED(hr) ) {
		// Register this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr		   = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}


static HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	ICatRegister *pcr = NULL ;
	HRESULT 	  hr  = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void **) &pcr);
	if ( SUCCEEDED(hr) ) {
		// Unregister this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr		   = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}


//+++ struct SInit1 { SInit1() {Misc::setHeapAllocLowFlagmentationMode();} };
//+++ static SInit1 s_init1;


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
  #if defined (_DEBUG) && defined(_CRTDBG_MAP_ALLOC)
	//���������[�N���o�p
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF
				| _CRTDBG_CHECK_ALWAYS_DF
	);
	//_CrtSetBreakAlloc(874);
  #endif


	Misc::setHeapAllocLowFlagmentationMode();	//+++

	//x ErrorLogPrintf(_T("�e�X�g����\n"));

	if (CheckOneInstance(hInstance, lpstrCmdLine))
		return 0;

	g_pMainWnd	 = NULL;
	//	HRESULT hRes = ::CoInitialize(NULL);
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	ATLASSERT( SUCCEEDED(hRes) );
	hRes		 = ::OleInitialize(NULL);
	// If you are running on NT 4.0 or higher you can use the following call instead to
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT( SUCCEEDED(hRes) );

    StartXPCOM(); //Proxy�̂���

	ATLTRACE(_T("tWinMain : \n"), lpstrCmdLine);

 #if (_WIN32_IE >= 0x0300)
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof (iccx);
	iccx.dwICC	= ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_USEREX_CLASSES;
	int 	ret = ::InitCommonControlsEx(&iccx);
	ret;
	ATLASSERT(ret);
 #else
	::InitCommonControls();
 #endif

	//hRes	= _Module.Init(ObjectMap, hInstance, &LIBID_ATLLib);
	hRes	= _Module.Init(NULL, hInstance);
	ATLASSERT( SUCCEEDED(hRes) );

	int 	nRet		 = 0;
	bool	bRun		 = true;
	bool	bAutomation  = false;
	bool	bTray		 = false;

	try {	//+++ �O�̂��ߗ�O�`�F�b�N.
		nRet = _PrivateInit(lpstrCmdLine);
	} catch (...) {
		ErrorLogPrintf(_T("_PrivateInit�ŃG���[\n"));
		nRet = -1;
	}
	if (nRet <= 0)
		goto END_APP;

	AtlAxWinInit();

	CDonutSimpleEventManager::RaiseEvent(EVENT_PROCESS_START);

	if (bRun) {
		_Module.StartMonitor();
		hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
		ATLASSERT( SUCCEEDED(hRes) );
		hRes = ::CoResumeClassObjects();
		ATLASSERT( SUCCEEDED(hRes) );

		if (bAutomation) {
			CMessageLoop theLoop;
			nRet = theLoop.Run();
		} else {
		  #ifndef USE_DIET	//+++ �N�����̊��t�@�C���`�F�b�N. unDonut.ini���Ȃ����t�@�C��������ĂȂ�������N���y�[�W��about:warning�ɂ���.
			if (lpstrCmdLine == 0 || lpstrCmdLine[0] == 0) {
				if (HaveEnvFiles() == 0)
					lpstrCmdLine = _T("about:warning");
			}
		  #endif
			nRet = Run(lpstrCmdLine, nCmdShow, bTray);
		}
	  #if 1 //+++ WTL�̃��C�����N���[�Y������I�����ɁA�I���R�[�h�Ƃ���1��Ԃ�...
			//+++ OS�ɕԂ��l�Ȃ̂�0�̂ق����悢�͂��ŁAdonut�̑��̕����ł�0�ɂ��Ă���悤�Ȃ̂�
			//+++ �������Ȃ��̂ŁA�����I�ɕϊ�.
		if (nRet == 1)
			nRet = 0;
	  #endif

		_Module.RevokeClassObjects();
		::Sleep(_Module.m_dwPause);
	}

	try {	//+++ �Ӗ��Ȃ����낤���A�C�x�߂ŗ�O�`�F�b�N.
		_PrivateTerm();
	} catch (...) {
		ErrorLogPrintf(_T("_PrivateTerm()�ŗ�O����\n"));
		nRet = -1;
	}

  END_APP:
	try {	//+++ �Ӗ��Ȃ����낤���A�C�x�߂ŗ�O�`�F�b�N.
		_Module.Term();
	} catch (...) {
		ErrorLogPrintf(_T("_Module.Term()�ŗ�O����\n"));	//MessageBox(NULL,"��O���������܂����B","��O",MB_OK);
		nRet = -1;
	}

	try {
		TermXPCOM();
	} catch (...) {
	}

	try {	//+++ �Ӗ��Ȃ����낤���A�C�x�߂ŗ�O�`�F�b�N.
		::OleUninitialize();
	} catch (...) {
		ErrorLogPrintf(_T("::OleUninitialize()�ŗ�O����\n"));
		nRet = -1;
	}

	try {	//+++ �Ӗ��Ȃ����낤���A�C�x�߂ŗ�O�`�F�b�N.
		::CoUninitialize();
	} catch (...) {
		ErrorLogPrintf(_T("::CoUninitialize()�ŗ�O����\n"));
		nRet = -1;
	}

	CDonutSimpleEventManager::RaiseEvent(EVENT_PROCESS_END);

  #if 0 //defined (_DEBUG) && defined(_CRTDBG_MAP_ALLOC)	//+++ �t���O�ŃR�[������Ă邩��s�v������...
    _CrtDumpMemoryLeaks();
  #endif
 	return nRet;
}



/////////////////////////////////////////////////////////////////////////////


// CChildFrame
void CChildFrame::PreDocumentComplete( /*[in]*/ IDispatch *pDisp, /*[in]*/ VARIANT *URL)
{
	return;
	/*if (!g_pAPI)
		return;

	int nTabIndex = m_MDITab.GetTabIndex(m_hWnd);
	g_pAPI->Fire_DocumentComplete( nTabIndex, pDisp, V_BSTR(URL) );*/
}


/////////////////////////////////////////////////////////////////////////////
// CAPI
/*HRESULT STDMETHODCALLTYPE CAPI::Advise(IUnknown *pUnk, DWORD *pdwCookie)
{
	HRESULT hr = CProxyIDonutPEvents<CAPI>::Advise(pUnk, pdwCookie);

	if ( SUCCEEDED(hr) ) {
		g_pAPI = this;
		m_aryCookie.Add(pdwCookie);
	}

	//CString str;
	//str.Format("Advise pUnk=%p cookie=%u",pUnk,*pdwCookie);
	//::MessageBox(NULL,str,_T("check"),MB_OK);
	return hr;
}



HRESULT STDMETHODCALLTYPE CAPI::Unadvise(DWORD dwCookie)
{
	HRESULT hr = CProxyIDonutPEvents<CAPI>::Unadvise(dwCookie);

	if ( SUCCEEDED(hr) )
		g_pAPI = NULL;

	//CString str;
	//str.Format("Unadvise cookie=%u",dwCookie);
	//::MessageBox(NULL,_T("unadvise"),_T("check"),MB_OK);
	return hr;
}
*/



/////////////////////////////////////////////////////////////////////////////
///+++ ���݂̃A�N�e�B�u�łőI�𒆂̃e�L�X�g��Ԃ�.
///+++ �� CSearchBar�����ɗp��. �{���� g_pMainWnd-> �̓����֐����Ăׂ΂������������A
///+++	  include �̈ˑ��֌W���ʓ|�Ȃ̂�...
CString Donut_GetActiveSelectedText()
{
	return g_pMainWnd->GetActiveSelectedText();
}


///+++
bool	Donut_HaveActiveSelectedText()
{
	CChildFrame *pChild = g_pMainWnd->GetActiveChildFrame();
	return pChild->HaveSelectedText();
}



///+++
CString Donut_GetActiveStatusStr()
{
	return g_pMainWnd->GetActiveChildFrame()->strStatusBar();
}


#if 0
///+++ About�_�C�A���O�p�ɃA�C�R�����[�h (���ƂłȂ�Ƃ�����>����)
HICON Donut_LoadIcon4AboutDialog()
{
	return g_pMainWnd->LoadIcon4AboutDialog();
}
#endif


///+++
void  Donut_ExplorerBar_RefreshFavoriteBar()
{
	CDonutExplorerBar::GetInstance()->RefreshExpBar(0);
}

