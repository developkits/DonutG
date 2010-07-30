/**
 *	@file	MainFrame_PreTrnMsg.cpp
 *	@brief	CMainFrame �� PreTranslateMessage�֌W�̏���.
 *	@note
 *		+++ MainFrame.cpp ���番��.
 */

#include "stdafx.h"
#include "MainFrame.h"


#if defined USE_ATLDBGMEM
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#ifndef WM_XBUTTONDOWN
 #define WM_XBUTTONDOWN 				0x020B
 #define WM_XBUTTONUP					0x020C
 #define GET_KEYSTATE_WPARAM(wParam)	( LOWORD(wParam) )
 #define GET_XBUTTON_WPARAM(wParam) 	( HIWORD(wParam) )
 #define MK_XBUTTON1					0x0020
 #define MK_XBUTTON2					0x0040
 #define XBUTTON1						0x0001
 #define XBUTTON2						0x0002
#endif



//�e�E�B���h�E��(��ɃL�[)���b�Z�[�W��]������
BOOL CMainFrame::PreTranslateMessage(MSG *pMsg)
{
  #if 0	//+++ ����
	if (g_pMainWnd && g_pMainWnd->m_hWnd) {
		static CString sav;
		CString tmp = MtlGetWindowText(g_pMainWnd->m_hWnd);
		//if (sav != tmp && tmp != "")
		{
			ErrorLogPrintf(_T("%s\n"), LPCTSTR(tmp));
			sav = tmp;
			MtlSetWindowText(g_pMainWnd->m_hWnd, tmp+"@");
		}
	}
  #endif

	//�R�}���h�o�[(���j���[)
	if ( m_CmdBar.PreTranslateMessage(pMsg) )
		return TRUE;

	//�A�h���X�o�[
	BYTE ptFlag 	= m_AddressBar.PreTranslateMessage(pMsg);
	if (ptFlag == _MTL_TRANSLATE_HANDLE)
		return TRUE;
	else if (ptFlag == _MTL_TRANSLATE_WANT)
		return FALSE;

	//�����o�[
	ptFlag = m_SearchBar.PreTranslateMessage(pMsg);
	if (ptFlag == _MTL_TRANSLATE_HANDLE)
		return TRUE;
	else if (ptFlag == _MTL_TRANSLATE_WANT)
		return FALSE;

	//�y�[�W�������o�[
	ptFlag = m_fayt.PreTranslateMessage(pMsg);
	if (ptFlag == _MTL_TRANSLATE_HANDLE)
		return TRUE;
	else if (ptFlag == _MTL_TRANSLATE_WANT)
		return FALSE;

	//�G�N�X�v���[���o�[
	//if (m_ExplorerBar.PreTranslateMessage(pMsg)) return TRUE;
	ptFlag = m_ExplorerBar.PreTranslateMessage(pMsg);
	if (ptFlag == _MTL_TRANSLATE_HANDLE)
		return TRUE;
	else if (ptFlag == _MTL_TRANSLATE_WANT)
		return FALSE;

  #if 1
	//+++ �A�h���X�o�[or�T�[�`�o�[�Ƀt�H�[�J�X���������Ă��鎞�A���̏����ŗ]�v�Ȃ���(�L�[����)�����Ȃ��悤�ɃK�[�h���Ă݂�.
	int	bFocus = false;
	if (::IsWindow(m_AddressBar.m_hWnd))
		bFocus	|= (::GetFocus() == m_AddressBar.GetEditCtrl());
	if (::IsWindow(m_SearchBar.m_hWnd))
		bFocus	|= (::GetFocus() == m_SearchBar.GetEditCtrl());
  #endif

	// ���N���X
	if (/*bFocus == 0 &&*/ baseClass::PreTranslateMessage(pMsg) )
		return TRUE;

	//�A�N�e�B�u�E�`���C���h�E�E�B���h�E
	HWND hWnd = 0;
	if (m_hWndMDIClient && ::IsWindow(m_hWndMDIClient))
		hWnd = MDIGetActive();

  #if 0	//+++ ���{�^���N���b�N�̃e�X�g
	if ( pMsg->message == WM_LBUTTONDOWN) {
		printf("%d,%d\n",pMsg->wParam, pMsg->lParam);
	}
  #endif

	// ���{�^���N���b�N�̃`�F�b�N
	if ( pMsg->message == WM_MBUTTONDOWN && hWnd != NULL && ::IsChild(hWnd, pMsg->hwnd) && OnMButtonHook(pMsg) )
		return TRUE;

	//�}�E�X�W�F�X�`���[��
	if ( pMsg->message == WM_RBUTTONDOWN && OnRButtonHook(pMsg) )
		return TRUE;

	//�T�C�h�{�^��
	if (pMsg->message == WM_XBUTTONUP) {
		if ( OnXButtonUp( GET_KEYSTATE_WPARAM(pMsg->wParam), GET_XBUTTON_WPARAM(pMsg->wParam),
						 CPoint( GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) ) ) )
			return TRUE;
	}

	// �G�N�X�v���[���o�[�������ŕ\��
	if ( (CMainOption::s_dwExplorerBarStyle & MAIN_EXPLORER_AUTOSHOW) == MAIN_EXPLORER_AUTOSHOW )
		ExplorerBarAutoShow(pMsg);

	//BHO
	if (bFocus == 0 && TranslateMessageToBHO(pMsg) )											//+++ bFocus�`�F�b�N
		return TRUE;

	// hWnd = MDIGetActive();	//+++ �Ӗ��Ȃ��Ǝv�����A�O�̂��ߎ�蒼��...���

	if (bFocus == 0 && hWnd != NULL && ::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM) pMsg) )	//+++ bFocus�`�F�b�N
		return TRUE;

	return FALSE; // IsDialogMessage(pMsg);
}


void CMainFrame::UpdateLayout(BOOL bResizeBars /*= TRUE*/)
{
	CRect	  rc;
	GetClientRect(&rc);

	if (bResizeBars) {
		_UpdateReBarPosition(rc);
	}

	{
		CClientDC dc(m_hWnd);
		_DoPaintReBarBorders(dc.m_hDC);
	}
	UpdateBarsPosition(rc, bResizeBars);
	_InflateReBarBordersRect(rc);

	if (m_hWndClient != NULL) {
		::SetWindowPos( m_hWndClient, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE );
	}
}



BOOL CMainFrame::OnXButtonUp(WORD wKeys, WORD wButton, CPoint point)
{
	CString 	strKey;

	switch (wButton) {
	case XBUTTON1: strKey = _T("Side1"); break;
	case XBUTTON2: strKey = _T("Side2"); break;
	}

	DWORD		dwCommand = 0;
	{
		CIniFileI pr( _GetFilePath( _T("MouseEdit.ini") ), _T("MouseCtrl") );
		pr.QueryValue(dwCommand, strKey);
	}

	if (dwCommand == 0)
		return FALSE;

	::SendMessage(m_hWnd, WM_COMMAND, dwCommand, 0);
	return TRUE;
}



void CMainFrame::ExplorerBarAutoShow(MSG *pMsg)
{
	if (pMsg->message != WM_MOUSEMOVE)
		return;

	if (pMsg->wParam != 0)
		return;

	if (  m_ExplorerBar.IsFavBarVisible()		== false
	   && m_ExplorerBar.IsClipboardBarVisible() == false
	   && m_ExplorerBar.IsPanelBarVisible() 	== false
	   && m_ExplorerBar.IsPluginBarVisible()	== false)
		return;

	CPoint point;
	::GetCursorPos(&point);

	CRect  rcWndCmd;
	::GetWindowRect(m_hWndToolBar, &rcWndCmd);

	if ( rcWndCmd.PtInRect(point) )
		return;

	ScreenToClient(&point);

	CRect  rcWnd;
	GetClientRect(&rcWnd);

	if (m_ExplorerBar.IsExplorerBarHidden() == false) {
		CRect rcWndExp;
		m_ExplorerBar.GetClientRect(&rcWndExp);
		rcWnd.left += rcWndExp.Width() + 5;

		if ( rcWnd.PtInRect(point) )
			m_wndSplit.SetSinglePaneMode(SPLIT_PANE_RIGHT);
	} else {
		if (point.x < 10 && point.x >= 0) {
			m_ExplorerBar.ShowBar(m_ExplorerBar.m_dwExStyle, true);
		}
	}
}


BOOL CMainFrame::OnMButtonHook(MSG* pMsg)
{
	DWORD	dwLinkOpenBtnM;
	{
		CIniFileI	 pr( _GetFilePath( _T("MouseEdit.ini") ), _T("MouseCtrl") );
		dwLinkOpenBtnM = pr.GetValue(_T("LinkOpenBtnM"), 0);
	}

	// �{�^�����ݒ肳��Ă��Ȃ�������A�I���.
	if (dwLinkOpenBtnM == 0)
		return FALSE;
	//+++ �����F�f�t�H���g�ł� dwLinkOpenBtmM = IDM_FOLLOWLINKN
	// ATLASSERT(dwLinkOpenBtnM == IDM_FOLLOWLINKN);

  #if 0
	//+++ ����:���̕����̏����́ADonutP��DonutRAPT,unDonut�ł��قǃ\�[�X�ɈႢ������킯�łȂ�����.
	::SendMessage(pMsg->hwnd, WM_LBUTTONDOWN, 0, pMsg->lParam); 	//+++ ���܂��Ȃ�.
	int 	nTabCnt = m_MDITab.GetItemCount();						//+++ �����N���N���b�N�������ǂ����̃`�F�b�N�p.
	::SendMessage(pMsg->hwnd, WM_COMMAND, dwLinkOpenBtnM, 0);		//+++ �����N��V�������ɂЂ炭
	if (m_MDITab.GetItemCount() != nTabCnt) 						//+++ �^�u�������Ă�����A�����N���N���b�N�����̂��낤
		return TRUE;												//+++ true��Ԃ��Ē��{�^���N���b�N�̏�������������Ƃɂ���.
	::SendMessage(pMsg->hwnd, WM_LBUTTONUP, 0, pMsg->lParam);		//+++ �����N�ȊO���N���b�N�����ꍇ���܂��Ȃ��̍��N���b�N�������I�����Ă���.
	return FALSE;													//+++ false��Ԃ����ƂŁAIE�R���|�[�l���g�ɃE�B�[���N���b�N�̏�����C����.

	//+++ ����: �����炭��L�̉E�ɏ������R�����g�̂悤�ȓ����z�肵�����̂��Ǝv����.
	//		"���܂��Ȃ�"�A�́AWM_COMMAND+IDM_FOLLOWLINKN�݂̂𑗂����ꍇ�A
	//		�Ȃ����J��������1�N���b�N�����悤�ŁA����̒��날�킹�Ƃ���
	//		���N���b�N�����������Ƃɂ��Ă���悤��(...�悭�킩��Ȃ�����ǁA���Ԃ�).
	//		�����A�����N�ȊO�̉ӏ����N���b�N�����Ƃ��́A�͈͑I���̊J�n�Ƃ��ċ@�\���Ă��܂����߁A
	//		���̏ꍇ�́A���U���{�^���N���b�N���I�������鏈�������Ă���B
	//
	//		���́A�N���b�N�������ǂ����̔���ŁA�^�u�̌���IDM_FOLLOWLINKN�ɂ����
	//		������(�ϓ�����)���ǂ��������Ă�̂���...
	//		�~�j�b�g���ȑO�̊J���҂̊��ł͗L���̂悤�����Aundonut+��+mod�̊J�����ł́A
	//		�����N�̗L���ɂ�����炸���̎��_�ł̓^�u�����ς��Ȃ��͗l... ��
	//		FALSE��Ԃ������ɗ���A���N���b�N�L�����Z���̏��������J�N���b�N�Ɠ��l�ɍ�p����
	//		���܂��A���ʓI�ɓ����y�[�W���Q�J���Ă��܂��A�Ƃ����o�O�ɉ����Ă��܂��Ă���.
	//
	//		�󋵓I�ɂ�::SendMessage(pMsg->hwnd, WM_COMMAND, dwLinkOpenBtnM, 0);�̎��s��
	//		�V�����^�u���ł��Ă��畜�A���Ă����m���A���N�G�X�g�������s���Ă����߂��Ă��鏈��
	//		�ɕς���Ă��܂����A�Ƃ������ƂɎv����B
	//
	//		���������CChildFrame���ł� �����o�[��ActivateFrame()�̏����̒��Ń^�u�ւ̒ǉ���
	//		��������̂����AWM_COMMAND+dwLinkOpenBtnM��CChildFrame�������ꍇ�A
	//		CChildFrame::OnNewWindow2() ���Ă΂�A���̃��[�g�ł�ActivateFrame()�͒��ɂ͌Ă΂ꂸ�A
	//		���Ƃ�ActivateFrame()���Ă΂��悤�ɂ��Ă���B���̏�Ԃ�OnNewWindow2�͏I���̂ŁA
	//		SendMessage���I����Ă��^�u�̐��͕ς��Ȃ��A�Ƃ������̏�Ԃ́A
	//		�\�[�X�I�ɂ͓��R�Ɏv����̂���... (���̂ւ�̑�؂�donutP�ɂ����̂ڂ��Ă�����)
	//
	//		�ŁAActivateFrame�����ƂŒN���ĂԂ��Ƃ����� CChildFrame::OnBeforeNavigate2() �ŁA
	//		����� MtlBrawser.h��IWebBrowserEvents2Impl::OnBeforeNavigate2����Ă΂�Ă���
	//		�܂�A���܂������Ă���J�����ł́ADonut�\�[�X���Ŗ����I�ɂ��̊֐����Ă�ł��Ȃ�����ǁA
	//		SendMessage���I������܂łɂ�������s�����A�Ƃ������ƂȂ̂��낤.
	//		(�� �Ђ���Ƃ���ƁA�������� OnNewWindow2�ȊO��ChildFrame����Ă����\�������邩��������)
	//
	//		OS,IE,PSDK/ATL,WTL, unDonut �̂ǂ����̏����������Ȃ����̂��낤���A�����悭�킩��Ȃ�
	//		(2007-11-23)
	//		�ǂ���� AtlHost(Ex)�ł�CreateControlLicEx ����,Navigate2()���܂�else�����c���Ă���̂�
	//		�܂��������͗l�B����else���폜�����璼��. (2008-01-01)
  #else //+++ �����N�����������ǂ����̔���ɃX�e�[�^�X�o�[�Ƀ����N�悪�ݒ肳��Ă��邩�ǂ����A��p����悤�ɂ���.
	HWND			hWnd	= MDIGetActive();
	if ( ::IsWindow(hWnd) == 0 )
		return FALSE;
	CChildFrame*	pChild	= GetChildFrame(hWnd);
	bool			bLink	= 0;
	if (pChild) {																//+++ �J�[�\���������N�������Ă�����statusBar�p�̃��b�Z�[�W�����邱�Ƃ𗘗p.
		const CString& str = pChild->strStatusBar();
		if (str.IsEmpty() == 0 && str != _T("�y�[�W���\������܂���"))			//+++ �����N�̂Ȃ��y�[�W�ł�"�y�[�W���\������܂���"�Ƃ������b�Z�[�W���ݒ肳��Ă���̂ŏ��O.
			bLink = true;
	}
	::SendMessage(pMsg->hwnd, WM_LBUTTONDOWN, 0, pMsg->lParam); 				//+++ ���܂��Ȃ�.
	int 	nTabCnt = m_MDITab.GetItemCount();									//+++ �����N���N���b�N�������ǂ����̃`�F�b�N�p.
	::SendMessage(pMsg->hwnd, WM_COMMAND, dwLinkOpenBtnM, 0);					//+++ �����N��V�������ɂЂ炭
	int 	nTabCnt2 = m_MDITab.GetItemCount();
	if (nTabCnt != nTabCnt2 || bLink)											//+++ �����N���b�Z�[�W�����邩�A�^�u�������Ă�����A�����N���N���b�N�����Ƃ���.
		return TRUE;															//+++ true��Ԃ��Ē��{�^���N���b�N�̏�������������Ƃɂ���.
	::SendMessage(pMsg->hwnd, WM_LBUTTONUP, 0, pMsg->lParam);					//+++ �����N�ȊO���N���b�N�����ꍇ���܂��Ȃ��̍��N���b�N�������I�����Ă���.
	return FALSE;																//+++ false��Ԃ����ƂŁAIE�R���|�[�l���g�ɃE�B�[���N���b�N�̏�����C����.
  #endif
}


// Mouse Gesture
BOOL CMainFrame::OnRButtonHook(MSG *pMsg)
{
	HWND		hChildWnd = MDIGetActive();

	if (hChildWnd) {
		BOOL	bUsedMouseGesture = ::SendMessage(hChildWnd, WM_USER_USED_MOUSE_GESTURE, 0, 0) != 0;
		if (bUsedMouseGesture == FALSE)
			return FALSE;
	}

	::SetCapture(m_hWnd);

	CPoint		ptDown;
	ptDown.x = GET_X_LPARAM(pMsg->lParam);
	ptDown.y = GET_Y_LPARAM(pMsg->lParam);
	::ClientToScreen(pMsg->hwnd, &ptDown);

	CPoint		ptBefor   = ptDown;
	CPoint		ptUp	  = ptDown;
	int 		nAng1	  = -1;
	CString 	strMove;
	CString 	strMark1;
	CString 	strMark2;
	DWORD		dwTime	  = -1;
	HWND		hWndHist  = m_hWnd;
	HWND		hWnd	  = NULL;
	BOOL		bNoting   = TRUE;
	MSG 		msg;
	ZeroMemory(&msg, sizeof msg);

	do {
		if (::GetCapture() != hWndHist)
			break;

		if (m_hWnd != hWndHist)
			break;

		if (::GetMessage(&msg, NULL, 0, 0) == FALSE)
			continue;

		int 	nDiff		= 0;
		DWORD	dwCommand	= 0;
		BOOL	bNeedFresh	= FALSE,
				bBreak		= FALSE;

		switch (msg.message) {
		case WM_MOUSEWHEEL:
			dwCommand = GetMouseWheelCmd( (short) HIWORD(msg.wParam) );
			break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
			dwCommand = GetMouseButtonUpCmd(msg.message);
			break;

		case WM_XBUTTONUP:
			dwCommand = GetMouseButtonUpCmd( msg.message, GET_XBUTTON_WPARAM(msg.wParam) );
			break;

		case WM_MOUSEMOVE:
			ptUp.x	  = GET_X_LPARAM(msg.lParam);
			ptUp.y	  = GET_Y_LPARAM(msg.lParam);
			::ClientToScreen(msg.hwnd, &ptUp);

			nDiff	  = (int) ( sqrt( pow(float (ptBefor.x - ptUp.x), 2.0f) + pow(float (ptBefor.y - ptUp.y), 2.0f) ) );
			if (nDiff < 10)
				break;

			nAng1	  = (int) _GetAngle(ptBefor, ptUp);
			ptBefor   = ptUp;

			if		(nAng1 <  45 || nAng1 >  315)
				strMark1 = _T("��");
			else if (nAng1 >= 45 && nAng1 <= 135)
				strMark1 = _T("��");
			else if (nAng1 > 135 && nAng1 <  225)
				strMark1 = _T("��");
			else if (nAng1 >= 225 && nAng1 <= 315)
				strMark1 = _T("��");

			if (strMark1 == strMark2) {
				DWORD dwTimeNow = ::GetTickCount();

				if ( (dwTimeNow - dwTime) > 300 ) {
					strMark2 = _T("");
					dwTime	 = dwTimeNow;
				}
			}

			if (strMark1 != strMark2) {
				strMove += strMark1;
				strMark2 = strMark1;

				DWORD	dwCommand = 0;
				CIniFileI	pr( _GetFilePath( _T("MouseEdit.ini") ), _T("MouseCtrl") );
				pr.QueryValue(dwCommand, strMove);
				pr.Close();
				CString strCmdName;

				if (dwCommand != 0) {
					CString strTemp;
					CToolTipManager::LoadToolTipText(dwCommand, strTemp);
					strCmdName.Format(_T("[ %s ]"), strTemp);
				}

				CString 	strMsg;
				strMsg.Format(_T("�W�F�X�`���[ : %s %s"), strMove, strCmdName);
				::SetWindowText(m_hWndStatusBar, strMsg);
			}

			dwTime = ::GetTickCount();
			break;

		case WM_KEYDOWN:
			hWnd   = MDIGetActive();
			if (hWnd != NULL)
				SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM) &msg);

			break;

		default:
			DispatchMessage(&msg);
			break;
		}

		switch (dwCommand) {
		case ID_FILE_CLOSE:
			hWnd	   = MDIGetActive();
			if (hWnd == NULL)
				break;

			::PostMessage(hWnd, WM_COMMAND, ID_FILE_CLOSE, 0);
			//::PostMessage(hWnd, WM_CLOSE, 0, 0);
			bNoting    = FALSE;
			bNeedFresh = TRUE;
			//goto NEXT;
			break;

		case ID_GET_OUT:				// �ޔ�
		case ID_VIEW_FULLSCREEN:		// �S�̕\��
		case ID_VIEW_UP:				// ���
		case ID_VIEW_BACK:				// �O�ɖ߂�
		case ID_VIEW_FORWARD:			// ���ɐi��
		case ID_VIEW_STOP_ALL:			// ���ׂĒ��~
		case ID_VIEW_REFRESH_ALL:		// ���ׂčX�V
		case ID_WINDOW_CLOSE_ALL:		// ���ׂĕ���
		case ID_WINDOW_CLOSE_EXCEPT:	// ����ȊO����
			::PostMessage(m_hWnd, WM_COMMAND, dwCommand, 0);
			bNoting    = FALSE;
			bBreak	   = TRUE;
			break;

		case 0:
			break;

		default:
			::PostMessage(m_hWnd, WM_COMMAND, dwCommand, 0);
			bNoting    = FALSE;
			bNeedFresh = TRUE;
			break;
		}

		if (bNeedFresh) {
			hWnd = MDIGetActive();
			if (hWnd)
				::RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW);
		}

		if (!(GetAsyncKeyState(VK_RBUTTON) & 0x80000000) && msg.message != WM_RBUTTONUP) {
			MSG msgR;
			if (::PeekMessage(&msgR, NULL, 0, 0, PM_NOREMOVE) == 0) {
				break;
			}
		}
	} while (msg.message != WM_RBUTTONUP);

	::ReleaseCapture();

	if (bNoting) {
		ptUp.x = GET_X_LPARAM(msg.lParam);
		ptUp.y = GET_Y_LPARAM(msg.lParam);
		::ClientToScreen(msg.hwnd, &ptUp);

		::SetWindowText( m_hWndStatusBar, _T("") );

		DWORD dwCommand = 0;
		CIniFileI	pr( _GetFilePath( _T("MouseEdit.ini") ), _T("MouseCtrl") );
		pr.QueryValue(dwCommand, strMove);
		pr.Close();

		if (dwCommand != 0) {
			::SendMessage(m_hWnd, WM_COMMAND, dwCommand, 0);
			bNoting = FALSE;
		} else if (dwCommand == -1)
			return TRUE;
	}

	if ( bNoting && strMove.IsEmpty() ) {
		::ScreenToClient(pMsg->hwnd, &ptUp);
		pMsg->lParam = MAKELONG(ptUp.x, ptUp.y);

		::PostMessage(pMsg->hwnd, WM_RBUTTONUP, pMsg->wParam, pMsg->lParam);
	}

	return !bNoting;
}


DWORD CMainFrame::GetMouseWheelCmd(short nWheel)
{
	CString 	strKey;
	if (nWheel > 0)
		strKey = _T("WHEEL_UP");
	else
		strKey = _T("WHEEL_DOWN");

	CIniFileI	pr( _GetFilePath( _T("MouseEdit.ini") ), _T("MouseCtrl") );
	DWORD		dwCommand = pr.GetValue(strKey, 0);
	return dwCommand;
}


DWORD CMainFrame::GetMouseButtonUpCmd(UINT uMsg, UINT nXButton/* = 0*/)
{
	CString 	strKey;
	switch (uMsg) {
	case WM_LBUTTONUP:	strKey = _T("LButtonUp");					break;
	case WM_MBUTTONUP:	strKey = _T("MButtonUp");					break;
	case WM_XBUTTONUP:	strKey.Format(_T("XButtonUp%d"), nXButton); break;
	}

	CIniFileI	pr( _GetFilePath( _T("MouseEdit.ini") ), _T("MouseCtrl") );
	DWORD		dwCommand = pr.GetValue(strKey, 0);
	return dwCommand;
}



BOOL CMainFrame::TranslateMessageToBHO(MSG *pMsg)
{
  #if 1	//+++ �������ŁA��������if������߂�...��߂�Ȃ�?
	if (  (WM_KEYFIRST		 <= pMsg->message) && (pMsg->message <= WM_KEYLAST	)
	   || (WM_IME_SETCONTEXT <= pMsg->message) && (pMsg->message <= WM_IME_KEYUP)
	  #if 0	//+++ ������...
	   || (WM_MOUSEFIRST     <= pMsg->message) && (pMsg->message <= WM_MOUSELAST)
	   || (WM_NCMOUSEMOVE    <= pMsg->message) && (pMsg->message <= 0xAD/*WM_NCXBUTTONDBLCLK*/)
	  #endif
	)
  #endif 
	{
		int nCount = CPluginManager::GetCount(PLT_TOOLBAR);

		for (int i = 0; i < nCount; i++) {
			if ( CPluginManager::Call_PreTranslateMessage(PLT_TOOLBAR, i, pMsg) )
				return TRUE;
		}
	}

	return FALSE;
}

