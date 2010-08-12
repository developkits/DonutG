/**
 *	@file	MainFrame_Dfg.cpp
 *	@brief	CMainFrame ��dfg���[�h��Z�[�u�֌W.
 *	@note
 *		+++ MainFrame.cpp ����dfg���[�h��Z�[�u�֌W�𕪗�.
 */

#include "stdafx.h"
#include "MainFrame.h"

#if defined USE_ATLDBGMEM
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//private:
//+++ �O���[�v�I�v�V�����̃Z�[�u��������������.
//+++ - �t�@�C���̔r����������₷�����邽�߁A�����W�ƁAini�t�@�C���ւ̏������݂̃^�C�~���O�𕪗�.
//+++ - �r�����䎩�̂�CIniFile�ɔC����.
//+++ - ���́A�q���̏����W�̎擾���܂ߕʃX���b�h���ŏ������Ă������A�ǂ������̕ӂ��܂���
//+++	���낢��s����̌��ɂȂ��Ă����͗l.�i"�i�ށE�߂�"��TravelLog�������ɁAie�R���|�[�l���g(dll)��
//+++	���Ńn���O���Ă��������j

// �^�u�P�̏����擾
struct CMainFrame::_Function_EnumChildSaveOption {
	CMainFrame *				m_pMainFrame;
	HWND						m_hWndActive;
	std::list<SDfgSaveInfo>&	m_rDfgSaveInfoList;
	int 						m_nIndex;
	bool						m_bSaveFB;
	int 						m_nActiveIndex;

	_Function_EnumChildSaveOption(
			CMainFrame *				pMainFrame,
			HWND						hWndActive,
			std::list<SDfgSaveInfo>&	rDrfSaveInfoList,
			int 						nIndex		= 0,
			bool						bSaveFB 	= FALSE)
		:	m_pMainFrame(pMainFrame)
		,	m_hWndActive(hWndActive)
		,	m_rDfgSaveInfoList(rDrfSaveInfoList)
		,	m_nIndex(nIndex)
		,	m_bSaveFB(bSaveFB)
		,	m_nActiveIndex(-1)
	{
	}

	void operator ()(HWND hWnd)
	{
		if (hWnd == m_hWndActive)
			m_nActiveIndex = m_nIndex;

		CString 	 strSection;
		strSection.Format(_T("Window%d"), m_nIndex);

		//3�ȏ�̈����𑗂�̂��ʓ|�Ȃ̂Œ���CChildFrame�𑀍�
		CChildFrame *pChild = m_pMainFrame->GetChildFrame(hWnd);
		if (!pChild) {
			ErrorLogPrintf(_T("dfg�̃Z�[�u�ł̏��擾��%s(�q�E�B���h�E%#x)�̎擾�Ɏ��s\n"), LPCTSTR(strSection), hWnd);
			return;
		}

		try {	//+++ �O�̂��ߗ�O�`�F�b�N.
			m_rDfgSaveInfoList.push_back( SDfgSaveInfo() );
			m_rDfgSaveInfoList.back().m_section = strSection;
			pChild->view().m_ViewOption.GetDfgSaveInfo( m_rDfgSaveInfoList.back(), m_bSaveFB );
		} catch(...) {
			ErrorLogPrintf(_T("dfg�̃Z�[�u��%s(�q�E�B���h�E%#x)�̃Z�[�u���ɗ�O����\n"), LPCTSTR(strSection), hWnd);
		}
		++m_nIndex;
	}
};


CMainFrame::CSaveGroupOptionToFile::CSaveGroupOptionToFile()
	:	m_bOldMaximized(0)
	,	m_bSaveTravelLog(0)
	,	m_nIndex(0)
	,	m_nActiveIndex(0)
	,	m_phThread(0)
	,	m_strFileName()
	,	m_dfgSaveInfoList()
{
}

CMainFrame::CSaveGroupOptionToFile::~CSaveGroupOptionToFile() {
	if (m_phThread && *m_phThread) {	//+++ �X���b�h�������Ă���ƁAm_dfgSaveInfoList�̃f�X�g���N�g���܂�������...
		DWORD dwResult = ::WaitForSingleObject(*m_phThread, DONUT_BACKUP_THREADWAIT);
		//x CloseHandle(*m_phThread);
	}
}


void CMainFrame::CSaveGroupOptionToFile::Run(CMainFrame* pMainWnd, const CString &strFileName, bool bDelay) // modified by minit
{
	if (pMainWnd == 0)
		return;
	if (pMainWnd->m_hGroupThread) {
		DWORD dwReturnCode = 0;
		::GetExitCodeThread(pMainWnd->m_hGroupThread, &dwReturnCode);
		if (dwReturnCode == STILL_ACTIVE) {
			ErrorLogPrintf(_T("defalut.dfg�̑O��̃Z�[�u�ɗ\�z�ȏ�Ɏ��Ԃ��������Ă���悤�Ȃ̂ō���͌�����.\n"));
			return;
		}
		//+++ ��U�I��.
		::CloseHandle(pMainWnd->m_hGroupThread);
		pMainWnd->m_hGroupThread = NULL;
	}

	m_phThread			= &pMainWnd->m_hGroupThread;
	m_strFileName		= strFileName;
	m_bOldMaximized		= FALSE;
	m_bSaveTravelLog	= CMainOption::s_bTravelLogGroup != 0/*? TRUE : FALSE*/;
	m_dfgSaveInfoList.clear();
	HWND	hWndActive	= pMainWnd->MDIGetActive(&m_bOldMaximized);

	//+++ �܂��͏��擾�����s��.
	_Function_EnumChildSaveOption	f(pMainWnd, hWndActive, m_dfgSaveInfoList, 0, m_bSaveTravelLog);

	try {	//+++ �Ӗ��Ȃ��Ȃ������ǔO�̂��ߗ�O�`�F�b�N.
		// f=
		pMainWnd->m_MDITab.ForEachWindow( f );		// MtlForEachMDIChild(m_hWndMDIClient, f);
	} catch (...) {
		ErrorLogPrintf(_T("�O���[�v�I�v�V�����Z�[�u���ɗ�O����\n"));
	}

	m_nIndex		= f.m_nIndex;
	m_nActiveIndex	= f.m_nActiveIndex;

	if (bDelay == 0) {	//+++ ini�t�@�C���ւ̏������݂𓯊��ł���ꍇ.
		WriteIniFile();
	} else {			//+++ ini�t�@�C���ւ̏������݂�񓯊��ɂ���ꍇ.
		UINT	dwThreadID = 0;
		pMainWnd->m_hGroupThread = (HANDLE) _beginthreadex(NULL, 0, CSaveGroupOptionToFile::WriteIniFile_Thread, (LPVOID) this, 0, &dwThreadID);
	}
}

//private:
UINT __stdcall CMainFrame::CSaveGroupOptionToFile::WriteIniFile_Thread(void* pParam)
{
	CSaveGroupOptionToFile* pThis = (CSaveGroupOptionToFile*)pParam;
	pThis->WriteIniFile();
	_endthreadex(0);
	return 0;
}


void CMainFrame::CSaveGroupOptionToFile::WriteIniFile()
{
  #if 1	//+++ �j�P���̃A�C�f�A�ŁA�o�b�N�A�b�v�t�@�C������ .dfg.bak �łȂ� .bak.dfg �ɂ��Ă݂�.
	CString		strFileName	= m_strFileName;
	CString 	strBakName  = Misc::GetFileNameNoExt(strFileName) + ".bak.dfg";
	if (::GetFileAttributes(strBakName) != 0xFFFFFFFF)
		::DeleteFile(strBakName);					// �Â��o�b�N�A�b�v�t�@�C�����폜.
	if (::GetFileAttributes(strFileName) != 0xFFFFFFFF)
		::MoveFile(strFileName, strBakName);		// �����̃t�@�C�����o�b�N�A�b�v�t�@�C���ɂ���.
  #endif

	//+++ dfg�t�@�C���ɃZ�[�u
	CIniFileO	pr( m_strFileName, _T("Window") );
	//pr.RemoveFileToBak();			// strFileName ���폜(.bak��)

	//+++ �S�̂Ɋւ���ݒ��ۑ�
	pr.ChangeSectionName( _T("Header") );
	pr.SetValue( m_nIndex				, _T("count")	  );
	pr.SetValue( m_nActiveIndex 		, _T("active")	  );	// active index
	pr.SetValue( (m_bOldMaximized != 0)	, _T("maximized") );

	//+++ �����Ƃ̐ݒ��ۑ�
	for (std::list<SDfgSaveInfo>::iterator it = m_dfgSaveInfoList.begin();
			it != m_dfgSaveInfoList.end();
			++it)
	{
		it->WriteProfile(pr, m_bSaveTravelLog);
	}
}



// ===========================================================================

///+++
void CMainFrame::_SaveGroupOption(const CString &strFileName, bool bDelay /*=false*/)
{
	m_saveGroupOptionToFile.Run(this, strFileName, bDelay);

	RtlSetMinProcWorkingSetSize();				//+++ ( �������̗\��̈���ꎞ�I�ɍŏ����B�E�B���h�E���ŏ��������ꍇ�Ɠ��� ) ... �Ђ���Ƃ���Ƃ��܂��낵���Ȃ���������...
}



void CMainFrame::_LoadGroupOption(const CString &strFileName, bool bClose)
{
	//dmfTRACE( _T("CMainFrame::_LoadGroupOption\n") );

	CLockRedrawMDIClient	 lock(m_hWndMDIClient);
	CMDITabCtrl::CLockRedraw lock2(m_MDITab);
	CWaitCursor 			 cur;

	if (bClose)
		MtlCloseAllMDIChildren(m_hWndMDIClient);

	DWORD			dwCount 	= 0;
  #if INISECTION_USE_MUTEX != 1
	CIniFileI		pr( strFileName, _T("Header") );
	if (pr.QueryValue( dwCount, _T("count") ) != ERROR_SUCCESS)
		return;
	DWORD			dwActive	= pr.GetValue( _T("active")   , 0 );
	DWORD			dwMaximized = pr.GetValue( _T("maximized"), 0 );
	pr.Close();
  #else
	CIniFileI		pr( strFileName, _T("Header") );
	if (pr.QueryValue( dwCount, _T("count") ) != ERROR_SUCCESS)
		return;
	DWORD			dwActive	= pr.GetValue( _T("active")   , 0 );
	DWORD			dwMaximized = pr.GetValue( _T("maximized"), 0 );
  #endif

	bool			bActiveChildExistAlready = (MDIGetActive() != NULL);

	m_MDITab.m_bInsertHere		= true;
	m_MDITab.m_nInsertIndex 	= m_MDITab.GetItemCount();

	CChildFrame *	pChildActive = NULL;
	for (DWORD dw = 0; dw < dwCount; ++dw) {
		CChildFrame *pChild = CChildFrame::NewWindow(m_hWndMDIClient, m_MDITab, m_AddressBar);
		// if no active child, as there is no client edge in MDI client window,
		// so GetClientRect is different a little and a resizing will occur when switching.
		// That is, only the first child window is activated.
		if (pChild == NULL)
			continue;

		++m_MDITab.m_nInsertIndex;

		// activate now!
		pChild->ActivateFrame(MDIGetActive() != NULL ? SW_SHOWNOACTIVATE : -1);

		// if tab mode, no need to load window placement.
		pChild->view().m_ViewOption.GetProfile(strFileName, dw, !CMainOption::s_bTabMode);

		if (dw == dwActive)
			pChildActive = pChild;
	}

	m_MDITab.m_bInsertHere	= false;
	m_MDITab.m_nInsertIndex = -1;

	if (pChildActive == NULL)
		return;

	if (!bActiveChildExistAlready) {						// there was no active window
		MDIActivate(pChildActive->m_hWnd);
		if (CMainOption::s_bTabMode || dwMaximized == 1) {	// in tab mode
			MDIMaximize(pChildActive->m_hWnd);
		}
	} else {												// already an active window exists
		if ( !(CMainOption::s_dwMainExtendedStyle & MAIN_EX_NOACTIVATE) )
			MDIActivate(pChildActive->m_hWnd);
	}
}



// ===========================================================================

void CMainFrame::OnBackUpOptionChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	if (m_nBackUpTimerID != 0) {
		KillTimer(m_nBackUpTimerID);
		m_nBackUpTimerID = 0;
	}

	if (CMainOption::s_dwMainExtendedStyle & MAIN_EX_BACKUP)
		m_nBackUpTimerID = SetTimer(1, 1000 * 60 * CMainOption::s_dwBackUpTime);
}


//private:
void CMainFrame::OnFavoriteGroupSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	if (MDIGetActive() == NULL)
		return;

	TCHAR		szOldPath[MAX_PATH];	// save current directory
	szOldPath[0]	= 0;	//+++
	::GetCurrentDirectory(MAX_PATH, szOldPath);

	CString dir = DonutGetFavoriteGroupFolder();
	::SetCurrentDirectory( LPCTSTR(dir) );

	const TCHAR szFilter[] = _T("Donut Favorite Group�t�@�C��(*.dfg)\0*.dfg\0\0");
	CFileDialog fileDlg(FALSE, _T("dfg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	fileDlg.m_ofn.lpstrInitialDir = dir;
	fileDlg.m_ofn.lpstrTitle	  = _T("���C�ɓ���O���[�v�̕ۑ�");

	if (fileDlg.DoModal() == IDOK) {
		_SaveGroupOption(fileDlg.m_szFileName);
		::SendMessage(m_hWnd, WM_REFRESH_EXPBAR, 1, 0);
	}

	// restore current directory
	::SetCurrentDirectory(szOldPath);
}


void CMainFrame::OnFavoriteGroupAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	::SendMessage(MDIGetActive(), WM_COMMAND, (WPARAM) ID_FAVORITE_GROUP_ADD, 0);
}


void CMainFrame::OnFavoriteGroupOrganize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	::ShellExecute(m_hWnd, NULL, DonutGetFavoriteGroupFolder(), NULL, NULL, SW_SHOWNORMAL);
}



#if 1 //+++ WS_CAPTION�������t���X�N���[���A�Ƃ����O����̂Ă��̂ŁAMtl�̊O���֐��������̂�������ֈړ�
template <class _Profile>
static void /*Mtl*/WriteProfileStatusBarState( _Profile &__profile, HWND hWndStatusBar, const CString &strPrefix = _T("statusbar.") )
{
	ATLASSERT( ::IsWindow(hWndStatusBar) );

	// �e���A�ޔ����͕ۑ����Ȃ�
	HWND hWndParent = ::GetParent(hWndStatusBar);
	if (hWndParent) {
		if (::IsWindowVisible(hWndParent) == FALSE)
			return;
		//if (IsFullScreen())
		if (g_pMainWnd->IsFullScreen())		//+++
			return;
	}
	LONG lRet		= __profile.SetValue( ::IsWindowVisible(hWndStatusBar) != 0, strPrefix + _T("Visible") );
	ATLASSERT(lRet == ERROR_SUCCESS);
}
#endif


void CMainFrame::_WriteProfile()
{
	// save frame and status bar
	CIniFileO	pr( g_szIniFileName, _T("Main") );
	MtlWriteProfileMainFrameState(pr, m_hWnd);
	/*Mtl*/WriteProfileStatusBarState(pr, m_hWndStatusBar);

	// save MDI tab
	pr.ChangeSectionName(_T("MDITab"));
	MtlWriteProfileMDITab(pr, m_MDITab);

	// save rebar
	pr.ChangeSectionName(_T("ReBar"));
	MtlWriteProfileReBarBandsState(pr, m_hWndToolBar);
	pr.Close();

  #if 1 //+++ OnClose�̂�����ςȂɍs���悤�ɕύX... ����ς肱����
	// save group options
	_SaveGroupOption( CStartUpOption::GetDefaultDFGFilePath() );
  #endif
}

