/**
 *	@file	MainFrame_OpenFile.cpp
 *	@brief	CMainFrame �� �t�@�C�����J����֌W�̏���.
 *	@note
 *		+++ MainFrame.cpp ���番��.
 */

#include "stdafx.h"
#include "MainFrame.h"
#include "dialog/OpenURLDialog.h"


#if defined USE_ATLDBGMEM
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





bool CMainFrame::OnDDEOpenFile(const CString &strFileName)
{
	//dmfTRACE(_T("CMainFrame::OnDDEOpenFile(%s)\n"), strFileName);
  #if 1 //+++ �g���C��Ԃ���̕��A�ł̃o�O�΍�.
	IfTrayRestoreWindow();							//+++ �g���C��Ԃ������畜��.
	DWORD flags = DonutGetStdOpenFlag();
	OnUserOpenFile( strFileName, flags );
	if (CStartUpOption::s_dwActivate) {
		MtlSetForegroundWindow(m_hWnd);
	}
  #else
	OnUserOpenFile( strFileName, DonutGetStdOpenFlag() );
	if (CStartUpOption::s_dwActivate) {
		//+++ OnUserOpenFile()�̎��_�ōő剻/�t���X�N���[���������ꍇ�̑΍��nCmdShow��n���čl������悤�ɕύX.
		MtlSetForegroundWindow(m_hWnd, m_OnUserOpenFile_nCmdShow);	//+++ ���͂���nCmdShow�͈Ӗ����Ⴄ���̂������Ă��Ă�悤�ȋC������...�����Ƃ܂킵.
	}
  #endif
	// UDT DGSTR ( added by dai
	return true;
}




//public:
// Message handlers
// alternates OpenFile

#if 1	//+++ url�ʊg���v���p�e�B�̏�����ǉ�
HWND CMainFrame::OnUserOpenFile(const CString& strUrl, DWORD dwOpenFlag)
{
  #if 0	//+++ CChildFrame::OnBeforeNavigate2���ŏ�������悤�ɕύX�����̂ŁA�����ok ...�Ƃ�����...
	return UserOpenFile(strUrl, dwOpenFlag);
  #else	//+++ CChildFrame::OnBeforeNavigate2���ŏ�������悤�ɂ����̂ŁA2�d�Ɍ������������Ă��܂����A
		//+++ �I�v�V�������f�̃^�C�~���O�̓s���Ȃ�ׂ������t���O��ݒ肵���ق����悳�����Ȃ̂ŁA���x�]���ɂ���Ȃ�A������

	CString		str(strUrl);
	MtlRemoveStringHeaderAndFooter(str);

   #if 0	//+++ �d�l�Ǝv�������A.url���̊g���v���p�e�B�̌�������̂ŁA�����ł͂ł��Ȃ�
	if ( MtlIsExt( str, _T(".url") ) ) {
		if (MTL::ParseInternetShortcutFile(str) == 0)
			return NULL;
	}
   #endif

	DWORD dwExProp	 = 0xFFFFFFFF;
	DWORD dwExProp2	 = 8;
	if (   (_check_flag(D_OPENFILE_NOCREATE, dwOpenFlag) == 0 || CUrlSecurityOption::activePageToo())
		&& CUrlSecurityOption::FindUrl(str, &dwExProp, &dwExProp2, 0)
	) {
		return OpenUrlWithExProp(str, dwOpenFlag, dwExProp, dwExProp2);
	} else {
		return UserOpenFile(str, dwOpenFlag);
	}
  #endif
}
#endif



//+++ url�ʊg���v���p�e�B�Ή��ŁA�{����OnUserOpenFile��UserOpenFile�ɕϖ�. �����𖖂ɒǉ�.
HWND CMainFrame::UserOpenFile(const CString& strFile0, DWORD openFlag, int dlCtrlFlag, int extededStyleFlags)
{
	CString		strFile (strFile0);
	MtlRemoveStringHeaderAndFooter(strFile);

	if ( MtlIsExt( strFile, _T(".url") ) ) {	//+++ ����:url�̂Ƃ��̏���. �g���v���p�e�B�̎擾�Ƃ�����̂ŁA��p��open������...
		return OpenInternetShortcut(strFile, openFlag);
	}

	if (  !MtlIsProtocol( strFile, _T("http") )
	   && !MtlIsProtocol( strFile, _T("https") ) )
	{
		if ( MtlPreOpenFile(strFile) )
			return NULL;	// handled
	  #if 1	//+++	�^�u������Ƃ�
		if (m_MDITab.GetItemCount() > 0) {
			if (strFile.Find(':') < 0 && strFile.Left(1) != _T("/") && strFile.Left(1) != _T("\\")) {	// ���łɋ�̓I�ȃp�X�łȂ��Ƃ��A
				HWND			hWndChild 	= m_MDITab.GetTabHwnd(m_MDITab.GetCurSel());
				CChildFrame*	pChild 		= GetChildFrame(hWndChild);
				if (pChild) {		//+++ �A�N�e�B�u�y�[�W���݂�
					CString strBase  = pChild->GetLocationURL();
					CString str7     = strBase.Left(7);
					if (str7 == "file://") {	//+++ file://��������΁A���[�J���ł̃f�B���N�g���ړ����낤�Ƃ݂Ȃ���
						strFile = Misc::MakeFullPath( strBase, strFile );	//+++ �t���p�X��
					}
				}
			}
		}
	  #endif
	}

	if (strFile.GetLength() > INTERNET_MAX_PATH_LENGTH)
		return NULL;

	// UH (JavaScript dont create window)
	CString 	 strJava = strFile.Left(11);
	strJava.MakeLower();
	if ( strJava == _T("javascript:") ) {
		if ( strFile == _T("javascript:location.reload()") )
			return NULL;
		openFlag |= D_OPENFILE_NOCREATE;
	}

	// dfg files
	if ( MtlIsExt( strFile, _T(".dfg") ) ) {
		if ( !(CMainOption::s_dwMainExtendedStyle & MAIN_EX_NOCLOSEDFG) )
			_LoadGroupOption(strFile, true);
		else
			_LoadGroupOption(strFile, false);
		return NULL;
	}

  #ifndef USE_DIET	//+++	�T�C�^�}����(about:blank�̕s��C���Ō���ăR�����g�A�E�g...
					//+++	��������unDonut+���A�����Ƌ@�\���Ă��Ȃ��悤����?�A�킴�킴��������K�v���Ȃ����ȁA��)
					//+++   ������ƋC���ς�����̂ŏ����t�̎b�蕜��
	// minit(about:* pages)
	{
		CString strAbout = strFile.Left(6);
		if ( strAbout == _T("about:") && strFile != _T("about:blank") ) {
			HWND hWndAbout =  _OpenAboutFile(strFile);
			if (hWndAbout)
				return hWndAbout;
		}
	}
  #endif

	HWND	hWndActive = MDIGetActive();

	int 	nCmdShow   = _check_flag(D_OPENFILE_ACTIVATE, openFlag) ? -1 : SW_SHOWNOACTIVATE;
	if (hWndActive == NULL) {					// no window yet
		nCmdShow = -1;							// always default
	}

	if ( hWndActive != NULL && _check_flag(D_OPENFILE_NOCREATE, openFlag) ) {
		CGeckoBrowser browser = DonutGetNsIWebBrowser(hWndActive);

		if ( !browser.IsBrowserNull() ) {
			browser.Navigate2(strFile);

			if ( !_check_flag(D_OPENFILE_NOSETFOCUS, openFlag) ) {
				// reset focus
				::SetFocus(NULL);
				MtlSendCommand(hWndActive, ID_VIEW_SETFOCUS);
			}

			return NULL;
			//return hWndActive;
		}
	}

 	//+++
	//x if (CMainOption::s_dwMainExtendedStyle & (/*MAIN_EX_NOACTIVATE_NEWWIN|*/MAIN_EX_NEWWINDOW))
	{
		//+++ if (dlCtrlFlag < 0)
		//+++	dlCtrlFlag = CDLControlOption::s_dwDLControlFlags;
		CChildFrame* pChild 	  = CChildFrame::NewWindow(m_hWndMDIClient, m_MDITab, m_AddressBar, false/*true*/, dlCtrlFlag, extededStyleFlags);
		if (pChild == NULL)
			return NULL;

		if ( !strFile.IsEmpty() ) {
			m_OnUserOpenFile_nCmdShow = pChild->ActivateFrame(nCmdShow);
			pChild->Navigate2(strFile);
			pChild->ForceMessageLoop(); 		//+++ �������A���̏�Ń��b�Z�[�W�����΂�. �����s�v�̂͂������A�����Ă����Ȃ��̂Ȃ�A�O�̂��ߎc���Ă���...
		} else {
			m_OnUserOpenFile_nCmdShow = pChild->ActivateFrame(nCmdShow);
		}

		if ( !_check_flag(D_OPENFILE_NOSETFOCUS, openFlag) ) {
			if (MDIGetActive() == pChild->m_hWnd) { // a new window activated, so automatically set focus
				// reset focus
				::SetFocus(NULL);
				MtlSendCommand(pChild->m_hWnd, ID_VIEW_SETFOCUS);
			} else {
				// It's reasonable not to touch a current focus.
			}
		}

		return pChild->m_hWnd;
	}
}



LRESULT CMainFrame::OnOpenWithExProp(_EXPROP_ARGS *pArgs)
{
	if (!pArgs)
		return 0;

	HWND		 hWndNew = OpenUrlWithExProp(pArgs->strUrl, pArgs->dwOpenFlag, pArgs->strIniFile, pArgs->strSection);
	if (!hWndNew)
		hWndNew = MDIGetActive();

	CChildFrame *pChild  = GetChildFrame(hWndNew);

	if (pChild) {
		// �����I�v�V�������擾
		DWORD		dwStatus;
		{
			CIniFileI pr( g_szIniFileName, _T("Search") );
			dwStatus  = pr.GetValue( _T("Status"), 0 );
		}

		CString 	str 	 = pArgs->strSearchWord;

		// �u�S�p�𔼊p�ɒu���v���`�F�b�N����Ă���ϊ����Ďq�E�B���h�E�ɕ������n���B
		if (dwStatus & STS_TEXT_FILTER)
			m_SearchBar.FilterString(str);

	  	//+++ �q���Ɍ����ݒ�𔽉f (�֐���)
	   #if 1 //defined USE_UNDONUT_G_SRC
		if (dwStatus & STS_AUTOHILIGHT)
			pChild->SetSearchWordAutoHilight( str, 1 );
	   #else
		pChild->SetSearchWordAutoHilight( str, (dwStatus & STS_AUTOHILIGHT) != 0 );
	   #endif
	}

	return (LRESULT) hWndNew;
}


HWND CMainFrame::OpenInternetShortcut(CString strUrlFile, DWORD dwOpenFlag)
{
	CString strUrl = strUrlFile;

	if ( !MTL::ParseInternetShortcutFile(strUrl) )
		return NULL;

	return OpenUrlWithExProp(strUrl, dwOpenFlag, strUrlFile);
}


HWND CMainFrame::OpenUrlWithExProp(CString strUrl, DWORD dwOpenFlag, DWORD dwExProp, DWORD dwExProp2)
{
	if ( _check_flag(D_OPENFILE_NOCREATE, dwOpenFlag) ) {
		return OpenExPropertyActive(strUrl, dwExProp, dwExProp2, dwOpenFlag);		//�����̃^�u�ŊJ��
	} else {
		return OpenExPropertyNew(strUrl, dwExProp, dwExProp2, dwOpenFlag); 		//�V�K�ɊJ��
	}
}


HWND CMainFrame::OpenUrlWithExProp(CString strUrl, DWORD dwOpenFlag, CString strIniFile, CString strSection /*= DONUT_SECTION*/)
{
	DWORD dwExProp = 0xAAAAAA;		//+++ �����l�ύX
	DWORD dwExProp2= 0x8;			//+++ �g���v���p�e�B�𑝐�.

	if ( CExProperty::CheckExPropertyFlag(dwExProp, dwExProp2, strIniFile, strSection) ) {
		if ( _check_flag(D_OPENFILE_NOCREATE, dwOpenFlag) ) {
			return OpenExPropertyActive(strUrl, dwExProp, dwExProp2, dwOpenFlag);	//�����̃^�u�ŊJ��
		} else {
			return OpenExPropertyNew(strUrl, dwExProp, dwExProp2, dwOpenFlag); 	//�V�K�ɊJ��
		}
	} else {
	  #if 1	//+++	URL�ʊg���v���p�e�B�̃`�F�b�N������.
			//		...���������ACChildFrame::OnBeforeNavigate2���ŏ�������悤�ɕύX�����̂ŁA�����͂Ȃ�
			//		...�ɂ��������������A�ǂ����Adl�I�v�V�������f�̃^�C�~���O�Ƀ��O������悤�Ȃ�ŁA
			//		�Ȃ�ׂ��������f�����悤�ɁA���x�]���Ŕ��f.
		if (   (_check_flag(D_OPENFILE_NOCREATE, dwOpenFlag) == 0 || CUrlSecurityOption::activePageToo())
			&& CUrlSecurityOption::FindUrl(strUrl, &dwExProp, &dwExProp2, 0)
		) {
			return OpenUrlWithExProp(strUrl, dwOpenFlag, dwExProp, dwExProp2);
		}
	  #endif
		return OpenExPropertyNot(strUrl, dwOpenFlag);					//�W���I�v�V�����ŊJ��
	}
}


//�����^�u�Ƀi�r�Q�[�g�����̂��Ɋg���ݒ��K�p����.  //+++ dwExProp2����.
HWND CMainFrame::OpenExPropertyActive(CString &strUrl, DWORD dwExProp, DWORD dwExProp2, DWORD dwOpenFlag)
{
	dwOpenFlag |= D_OPENFILE_NOCREATE;

	//�A�N�e�B�u�ȃ^�u���i�r�Q�[�g���b�N����Ă��邩���m�F
	HWND		 hWndActive = MDIGetActive();
	if ( hWndActive && ::IsWindow(hWndActive) ) {
		CChildFrame *pChild = GetChildFrame(hWndActive);

		if (pChild) {
			DWORD dwExFlag = pChild->view().m_ViewOption.m_dwExStyle;

			if (dwExFlag & DVS_EX_OPENNEWWIN)
				return OpenExPropertyNew(strUrl, dwExProp, dwExProp2, dwOpenFlag);
		}
	}

	//+++ (dwExProp2���݂�UserOpenFile�̌�ɂ������̂�O�Ɉړ�.)
	CExProperty  ExProp(CDLControlOption::s_dwDLControlFlags, CDLControlOption::s_dwExtendedStyleFlags, 0, dwExProp, dwExProp2);

	//�擾����URL���A�N�e�B�u�ȃ^�u�ŊJ������i�W�������ɔC����j
	BOOL		 	bOpened		= FALSE;
	HWND 			hWndNew		= UserOpenFile(strUrl, dwOpenFlag, ExProp.GetDLControlFlags(), ExProp.GetExtendedStyleFlags() );

	if (hWndNew && !hWndActive) {
		hWndActive = hWndNew;											//�E�B���h�E�����������̂ŐV�K�ɊJ����
		bOpened    = TRUE;
	}

	//�g���v���p�e�B��K�p����
	if (hWndActive == NULL)
		return NULL;
	CChildFrame*	pChild 	= GetChildFrame(hWndActive);
	if (!pChild)
		return NULL;

	pChild->view().PutDLControlFlags( ExProp.GetDLControlFlags() );
	pChild->SetViewExStyle(ExProp.GetExtendedStyleFlags(), TRUE);
	pChild->view().m_ViewOption.SetAutoRefreshStyle( ExProp.GetAutoRefreshFlag() );

	if (bOpened)
		return hWndActive;

	return NULL;
}


//�V�K�^�u���J�����̂��g���ݒ��K�p����
HWND CMainFrame::OpenExPropertyNew(CString &strUrl, DWORD dwExProp, DWORD dwExProp2, DWORD dwOpenFlag)
{
  #if 1	//+++
	dwOpenFlag &= ~D_OPENFILE_NOCREATE;

	CExProperty  ExProp(CDLControlOption::s_dwDLControlFlags, CDLControlOption::s_dwExtendedStyleFlags, 0, dwExProp, dwExProp2);
	int			 dlCtrlFlag        = ExProp.GetDLControlFlags();
	int			 extendedStyleFlag = ExProp.GetExtendedStyleFlags();
	//URL�ŐV�K�^�u���J��
	HWND 			hWndNew		= UserOpenFile(strUrl, dwOpenFlag, dlCtrlFlag, extendedStyleFlag);
	if ( hWndNew == 0 || !::IsWindow(hWndNew) )
		return NULL;

	//�g���v���p�e�B��K�p����
	CChildFrame *pChild  = GetChildFrame(hWndNew);
	if (!pChild)
		return NULL;

	pChild->view().PutDLControlFlags( dlCtrlFlag );
	pChild->SetViewExStyle(ExProp.GetExtendedStyleFlags(), TRUE);
	pChild->view().m_ViewOption.SetAutoRefreshStyle( ExProp.GetAutoRefreshFlag() );

	return hWndNew;
  #else
	dwOpenFlag &= ~D_OPENFILE_NOCREATE;

	//URL�ŐV�K�^�u���J��
	HWND 			hWndNew		= UserOpenFile(strUrl, dwOpenFlag);
	if ( hWndNew == 0 || !::IsWindow(hWndNew) )
		return NULL;

	//�g���v���p�e�B��K�p����
	CChildFrame *pChild  = GetChildFrame(hWndNew);
	if (!pChild)
		return NULL;
	CExProperty  ExProp(CDLControlOption::s_dwDLControlFlags, CDLControlOption::s_dwExtendedStyleFlags, 0, dwExProp);

	pChild->view().PutDLControlFlags( ExProp.GetDLControlFlags() );
	pChild->SetViewExStyle(ExProp.GetExtendedStyleFlags(), TRUE);
	pChild->view().m_ViewOption.SetAutoRefreshStyle( ExProp.GetAutoRefreshFlag() );

	return hWndNew;
  #endif
}


//�^�u���J�����̂��A�W���̐ݒ��K�p����
HWND CMainFrame::OpenExPropertyNot(CString &strUrl, DWORD dwOpenFlag)
{
	//�A�N�e�B�u�ȃ^�u���i�r�Q�[�g���b�N����Ă��邩���m�F
	HWND hWndActive = MDIGetActive();

	if ( ::IsWindow(hWndActive) ) {
		CChildFrame *pChild = GetChildFrame(hWndActive);

		if (pChild) {
			DWORD dwExFlag = pChild->view().m_ViewOption.m_dwExStyle;

			if (dwExFlag & DVS_EX_OPENNEWWIN)
				dwOpenFlag &= ~D_OPENFILE_NOCREATE; 			//�V�K�^�u�ŊJ���悤��
		}
	}

	//�擾����URL���J������
	HWND 			hWndNew		= UserOpenFile(strUrl, dwOpenFlag);
	if (hWndNew) {
		//�V�K�ɊJ�����^�u�Ȃ̂ŉ������Ȃ��Ă悢
		return hWndNew;
	} else {
		//�I�v�V������W���̂��̂ŏ㏑��
		CChildFrame *pChild = GetChildFrame( MDIGetActive() );

		if (!pChild)
			return NULL;

		pChild->view().PutDLControlFlags(CDLControlOption::s_dwDLControlFlags);
		pChild->SetViewExStyle(CDLControlOption::s_dwExtendedStyleFlags, TRUE);
		pChild->view().m_ViewOption.SetAutoRefreshStyle(0);
	}

	return NULL;
}



LRESULT CMainFrame::OnFileRecent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	// get file name from the MRU list
	if (ID_FILE_MRU_FIRST <= wID && wID <= ID_FILE_MRU_LAST)						//���͈�ID����V�͈�ID�֕ϊ�
		wID = wID - ID_FILE_MRU_FIRST + ID_RECENTDOCUMENT_FIRST;

	CRecentDocumentListFixed::RecentDocEntry Entry;

	if ( CMainOption::s_pMru->GetFromList(wID, &Entry) ) {
		DWORD dwOpenFlag = DonutGetStdOpenCreateFlag(); /*DonutGetStdOpenFlag()*/	// Force New Window // UDT DGSTR
		HWND  hWndNew	 = OnUserOpenFile(Entry.szDocName, dwOpenFlag);

		if ( ::IsWindow(hWndNew) ) {
			if (CMainOption::s_bTravelLogClose) {
				CChildFrame *pChild = GetChildFrame(hWndNew);

				if (pChild) {
					_Load_OptionalData2(pChild, Entry.arrFore, Entry.arrBack);
				}
			}
		}

		CMainOption::s_pMru->RemoveFromList(wID);									// UDT DGSTR
	} else {
		::MessageBeep(MB_ICONERROR);
	}

	return 0;
}



//+++ �����F�����N�������ŁA�ʂ̃v���O������������t���ŋN�����ꂽ�ꍇ�ɁA�����ɂ���B
//+++       ���̊O������̒ǉ��\����DDE�o�R�ōs����悤�ŁA������ɂ��邱�Ƃ͂Ȃ�����...?
void CMainFrame::OnNewInstance(ATOM nAtom)			// WM_NEWINSTANCE
{
	enum { NAME_LEN = 0x4000 };
	TCHAR szBuff[NAME_LEN+2];
	szBuff[0]	= 0;
	bool	bActive = !(CMainOption::s_dwMainExtendedStyle & MAIN_EX_NOACTIVATE);

	if (::GlobalGetAtomName(nAtom, szBuff, NAME_LEN) != 0) {
		//dmfTRACE(_T("CMainFrame::OnNewInstance: %s\n"), szBuff);
		CString					strPath;
		std::vector<CString>	strs;
		Misc::SeptTextToWords(strs, szBuff);
		for (unsigned i = 0; i < strs.size(); ++i) {
			CString&	str = strs[i];
			int 		c   = str[0];
			if (c == _T('-') || c == _T('/')) {
				if (str.CompareNoCase(_T("-tray")) == 0 || str.CompareNoCase(_T("/tray"))) {
					bActive = false;
				}
			} else {
				if (strPath.IsEmpty())
					strPath = str;
				else
					strPath += _T(' ') + str;
			}
		}
		OnUserOpenFile( strPath, DonutGetStdOpenFlag() );
		::GlobalDeleteAtom(nAtom);
	}

	if ( bActive ) {
		MtlSetForegroundWindow(m_hWnd); 						//�E�C���h�E���A�N�e�B�u�ɂ���
		IfTrayRestoreWindow();									//+++ �g���C��Ԃ������畜��.
		if (m_bOldMaximized == 0 && m_bFullScreen == 0) 		//+++
			ShowWindow_Restore(1);	//ShowWindow(SW_RESTORE);	//+++ �T�C�Y��߂�.
	}
}



////////////////////////////////////////////////////////////////////////////////
//�t�@�C�����j���[
//�@�R�}���h�n���h��
////////////////////////////////////////////////////////////////////////////////
////�V�K�쐬

//�|�[���y�[�W
void CMainFrame::OnFileNewHome(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	HWND hWndChild = OnUserOpenFile( CString(), DonutGetStdOpenActivateFlag() );

	if (hWndChild) {
		CGeckoBrowser browser = DonutGetNsIWebBrowser(hWndChild);

		if (browser.m_spBrowser != NULL)
			browser.GoHome();
	}
}


//���݂̃y�[�W
void CMainFrame::OnFileNewCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	HWND hWndActive = MDIGetActive();

	if (hWndActive) {
		CGeckoBrowser browser = DonutGetNsIWebBrowser(hWndActive);

		if (browser.m_spBrowser != NULL) {
			CString strURL = browser.GetLocationURL();

			if ( !strURL.IsEmpty() )
				OnUserOpenFile( strURL, DonutGetStdOpenActivateFlag() );
		}
	}
}


LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	if (CFileNewOption::s_dwFlags == FILENEW_BLANK) {
		OnFileNewBlank(0, 0, 0);
	} else if (CFileNewOption::s_dwFlags == FILENEW_COPY) {
		if (MDIGetActive() != NULL)
			SendMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_NEW_COPY, 0), 0);
		else
			OnFileNewBlank(0, 0, 0);
	} else if (CFileNewOption::s_dwFlags == FILENEW_HOME) {
		OnFileNewHome(0, 0, 0);
	} else if (CFileNewOption::s_dwFlags == FILENEW_USER) {		//+++ ���[�U�[�w��̃y�[�W���J��
		//CIniFileI 	pr( g_szIniFileName, _T("Main") );
		//CString		str = pr.GetStringUW( _T("File_New_UsrPage") );
		//pr.Close();
		CString&	str	= CFileNewOption::s_strUsr;
		if ( !str.IsEmpty() )
			OnUserOpenFile(str, 0);
		else
			OnFileNewBlank(0, 0, 0);
	} else {
		ATLASSERT(FALSE);
		OnFileNewBlank(0, 0, 0);
	}

	return 0;
}


//�󔒃y�[�W
void CMainFrame::OnFileNewBlank(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	OnUserOpenFile( _T("about:blank"), DonutGetStdOpenActivateFlag() );
	SendMessage(WM_COMMAND, ID_SETFOCUS_ADDRESSBAR, 0);
}


//�N���b�v�{�[�h
void CMainFrame::OnFileNewClipBoard(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	CString strText = MtlGetClipboardText();

	if ( strText.IsEmpty() )
		return;

	OnUserOpenFile( strText, DonutGetStdOpenActivateFlag() );
}


//Default.dfg
// UDT DGSTR
LRESULT CMainFrame::OnFileOpenDef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	CString 	strPath = Misc::GetExeDirectory() + _T("Default.dfg");
	ATLTRACE2( atlTraceGeneral, 4, _T("CMainFrame::OnFileOpenDef\n") );
	OnUserOpenFile( strPath, DonutGetStdOpenCreateFlag() );
	return 0;
}
// ENDE



void CMainFrame::OnFileNewClipBoard2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	CString strText = MtlGetClipboardText();

	if ( strText.IsEmpty() )
		return;

	OnUserOpenFile( strText, DonutGetStdOpenFlag() );	// allow the option
}



LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	//dmfTRACE( _T("CMainFrame::OnFileOpen\n") );
	COpenURLDlg dlg;

	if ( dlg.DoModal() == IDOK && !dlg.m_strEdit.IsEmpty() ) {
		OnUserOpenFile( dlg.m_strEdit, DonutGetStdOpenFlag() );
	}

	return 0;
}



/// DonutG �̔z�z�T�C�g���J��
LRESULT CMainFrame::OnJumpToWebSite(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	//CString strSite = _T("http://www5.ocn.ne.jp/~minute/tab/"); //unDonut�Â���
	//CString strSite = _T("http://undonut.sakura.ne.jp/"); // unDonut�V������
	//CString strSite = _T("http://tekito.genin.jp/undonut+.html"); //unDonut+
	//CString strSite = _T("http://ichounonakano.sakura.ne.jp/64/undonut/"); //unDonut+mod.	��
	//CString strSite = _T("http://undonut.undo.jp/"); //unDonut+mod.
	CString strSite = _T("http://donut.tnose.net/"); //DonutG

	OnUserOpenFile( strSite, DonutGetStdOpenFlag() );
	return 0;
}


///+++ .exe�̂���t�H���_���J��
LRESULT CMainFrame::OnOpenExeDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
{
	CString		progDir = Misc::GetExeDirectory();
	ShellExecute(NULL, _T("open"), progDir, progDir, NULL, SW_SHOW);
	return 0;
}



#ifndef USE_DIET	//+++ saitama��p�������̂��O���t�@�C���Ǎ��ɂ��Ĕėp��.
HWND CMainFrame::_OpenAboutFile(CString strFile)
{

	if (strFile.Left(6) != _T("about:"))
		return NULL;
	strFile     = strFile.Mid(6);		// about:
	bool  bWarn = (strFile == "warning");
	if (bWarn == 0) {
		strFile     = Misc::GetFullPath_ForExe( "help\\about\\" + strFile );	//
		if (Misc::IsExistFile(strFile) == 0)	//�t�@�C�����Ȃ������炩����
			return NULL;
	}
//plus7: Gecko�̏ꍇ�͕ʂ̗��V��about�v���g�R����񋟂��邱�ƂɂȂ�B����Ă���ȍ~�폜�B���Ƃōl����B
	return 0;
}
#endif
