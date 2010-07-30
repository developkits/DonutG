/**
 *	@file	DLControlOption.cpp
 *	brief	donut�̃I�v�V���� : "�u���E�U"
 */

#include "stdafx.h"
#include "DLControlOption.h"
#include "../IniFile.h"
#include "MainOption.h" 				//+++ �����Ώ�


#if defined USE_ATLDBGMEM
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace MTL;


//+++ Donut.cpp���ړ�.
TCHAR	CDLControlOption::s_szUserAgent[MAX_PATH];					// UDT DGSTR
TCHAR	CDLControlOption::s_szUserAgent_cur[MAX_PATH];				//+++



DWORD CDLControlOption::s_dwDLControlFlags	   = DLCTL_DEFAULT/*DLCTL_BGSOUNDS | DLCTL_VIDEOS | DLCTL_DLIMAGES*/;	//+++ 0�ȊO�̒l�ɕύX.
DWORD CDLControlOption::s_dwExtendedStyleFlags = DVS_EX_FLATVIEW | DVS_EX_MOUSE_GESTURE ;							//+++ 0�ȊO�̒l�ɕύX.



void CDLControlOption::GetProfile()
{
	CIniFileI	pr( g_szIniFileName, _T("Browser") );

	if (pr.QueryValue( s_dwDLControlFlags, _T("DLControl_Flags") ) != ERROR_SUCCESS)
		s_dwDLControlFlags = DLCTL_DEFAULT;

  #if 1	//+++
	CString tmp = pr.GetStringUW(_T("UserAgent"));
	if (!tmp.IsEmpty())
		::lstrcpyn( s_szUserAgent, tmp, MAX_PATH );

	char	szDefUserAgent[MAX_PATH*2];
	szDefUserAgent[0]	= 0;	//+++
	DWORD	size = MAX_PATH;
	::ObtainUserAgentString( 0 , szDefUserAgent , &size);
	::lstrcpyn(s_szUserAgent_cur, LPCTSTR(CString(szDefUserAgent)), MAX_PATH);
  #else
	// UDT DGSTR
	DWORD		dwCount = MAX_PATH;
	if (pr.QueryValue(s_szUserAgent, _T("UserAgent"), &dwCount) != ERROR_SUCCESS)
		::lstrcpy( s_szUserAgent, _T("") );
	// ENDE
  #endif

	// UH
	pr.QueryValue( s_dwExtendedStyleFlags, _T("ViewStyle") );
}



void CDLControlOption::WriteProfile()
{
	CIniFileO	pr( g_szIniFileName, _T("Browser") );
	pr.SetValue ( s_dwDLControlFlags, _T("DLControl_Flags") );
	pr.SetStringUW( s_szUserAgent, _T("UserAgent") ); // UDT DGSTR

	// UH
	pr.SetValue ( s_dwExtendedStyleFlags, _T("ViewStyle") );
}



const TCHAR*	CDLControlOption::userAgent()
{
	return (CMainOption::s_dwMainExtendedStyle2 & MAIN_EX2_USER_AGENT_FLAG) ? s_szUserAgent : s_szUserAgent_cur;
}




/////////////////////////////////////////////////////////////////////////////
// CDLControlPropertyPage



// Constructor
CDLControlPropertyPage::CDLControlPropertyPage(HWND hMainWnd)
{
	m_hMainWnd = hMainWnd;
	_SetData();
}



// Overrides
BOOL CDLControlPropertyPage::OnSetActive()
{
	SetModified(TRUE);

	// UDT DGSTR
	if (m_edit.m_hWnd == NULL)
		m_edit.Attach( GetDlgItem(IDC_EDIT_USER_AGENT) );
	// ENDE

	return DoDataExchange(FALSE);
}



BOOL CDLControlPropertyPage::OnKillActive()
{
	return DoDataExchange(TRUE);
}



BOOL CDLControlPropertyPage::OnApply()
{
	if ( DoDataExchange(TRUE) ) {
		_GetData();
		return TRUE;
	} else {
		return FALSE;
	}
}



void CDLControlPropertyPage::_GetData()
{
	// update dl control flags
	CDLControlOption::s_dwDLControlFlags = 0;

	if (m_nImage)		CDLControlOption::s_dwDLControlFlags |= GECKO_DLCTL_IMAGES;
	if (m_nImage)		CDLControlOption::s_dwDLControlFlags |= GECKO_DLCTL_FRAMES;
	if (m_nPlugin)		CDLControlOption::s_dwDLControlFlags |= GECKO_DLCTL_PLUGINS;
	if (m_nScript)		CDLControlOption::s_dwDLControlFlags |= GECKO_DLCTL_SCRIPTS;
	if (m_nRedir)		CDLControlOption::s_dwDLControlFlags |= GECKO_DLCTL_REDIR;
	if (m_nDNS)			CDLControlOption::s_dwDLControlFlags |= GECKO_DLCTL_DNS;

	// UH
	CDLControlOption::s_dwExtendedStyleFlags = 0;
	if (m_nNaviLock    /* == 1*/)	CDLControlOption::s_dwExtendedStyleFlags |= DVS_EX_OPENNEWWIN;
	if (m_nFlatView    /* == 1*/)	CDLControlOption::s_dwExtendedStyleFlags |= DVS_EX_FLATVIEW;
	if (m_bMsgFilter   /* == 1*/)	CDLControlOption::s_dwExtendedStyleFlags |= DVS_EX_MESSAGE_FILTER;
	if (m_bMouseGesture/* == 1*/)	CDLControlOption::s_dwExtendedStyleFlags |= DVS_EX_MOUSE_GESTURE;
	if (m_bBlockMailto /* == 1*/)	CDLControlOption::s_dwExtendedStyleFlags |= DVS_EX_BLOCK_MAILTO;

  #if 1 //+++ �ʂ̃I�v�V�������Ǘ����Ă���ϐ����Ԏ؂肵�Ēǉ�.
	if (m_nNoCloseNL			)	CMainOption::s_dwMainExtendedStyle2 	 |= MAIN_EX2_NOCLOSE_NAVILOCK;		//+++ �ǉ�.
	else							CMainOption::s_dwMainExtendedStyle2 	 &= ~MAIN_EX2_NOCLOSE_NAVILOCK; 	//+++ �ǉ�.
	if (m_bUserAgent			)	CMainOption::s_dwMainExtendedStyle2 	 |= MAIN_EX2_USER_AGENT_FLAG;		//+++ �ǉ�.
	else							CMainOption::s_dwMainExtendedStyle2 	 &= ~MAIN_EX2_USER_AGENT_FLAG;	 	//+++ �ǉ�.
  #endif

	SendMessage(m_hMainWnd, WM_COMMAND, ID_RESIZED, 0);
	m_edit.GetWindowText(CDLControlOption::s_szUserAgent, MAX_PATH);	// UDT DGSTR
}



void CDLControlPropertyPage::_SetData()
{
	m_nImage 		  = (CDLControlOption::s_dwDLControlFlags & GECKO_DLCTL_IMAGES) != 0;
	m_nFrame 		  = (CDLControlOption::s_dwDLControlFlags & GECKO_DLCTL_FRAMES) != 0;
	m_nPlugin 		  = (CDLControlOption::s_dwDLControlFlags & GECKO_DLCTL_PLUGINS) != 0;
	m_nScript 		  = (CDLControlOption::s_dwDLControlFlags & GECKO_DLCTL_SCRIPTS) != 0;
	m_nRedir 		  = (CDLControlOption::s_dwDLControlFlags & GECKO_DLCTL_REDIR) != 0;
	m_nDNS 			  = (CDLControlOption::s_dwDLControlFlags & GECKO_DLCTL_DNS) != 0;

	m_nNaviLock 	  = (CDLControlOption::s_dwExtendedStyleFlags & DVS_EX_OPENNEWWIN	 ) != 0;	//+++ ? 1 : 0;
	m_nFlatView 	  = (CDLControlOption::s_dwExtendedStyleFlags & DVS_EX_FLATVIEW 	 ) != 0;	//+++ ? 1 : 0;
	m_bMsgFilter	  = (CDLControlOption::s_dwExtendedStyleFlags & DVS_EX_MESSAGE_FILTER) != 0;	//+++ ? 1 : 0;
	m_bMouseGesture   = (CDLControlOption::s_dwExtendedStyleFlags & DVS_EX_MOUSE_GESTURE ) != 0;	//+++ ? 1 : 0;
	m_bBlockMailto	  = (CDLControlOption::s_dwExtendedStyleFlags & DVS_EX_BLOCK_MAILTO  ) != 0;	//+++ ? 1 : 0;

  #if 1 //+++ �ʂ̃I�v�V�������Ǘ����Ă���ϐ����Ԏ؂肵�Ēǉ�.
	m_nNoCloseNL	   = (CMainOption::s_dwMainExtendedStyle2 & MAIN_EX2_NOCLOSE_NAVILOCK	  ) != 0;		//+++ �ǉ�.
	m_bUserAgent	   = (CMainOption::s_dwMainExtendedStyle2 & MAIN_EX2_USER_AGENT_FLAG	  ) != 0;		//+++ �ǉ�.
  #endif

	// UDT DGSTR
	if (lstrlen(CDLControlOption::s_szUserAgent) != 0) {
		m_strUserAgent = CDLControlOption::s_szUserAgent;
	} else {
		char	szDefUserAgent[MAX_PATH*2];
		szDefUserAgent[0]	= 0;	//+++
		DWORD	size = MAX_PATH;
		::ObtainUserAgentString( 0 , szDefUserAgent , &size);
		// NOTE: Here , the other way.
		m_strUserAgent = szDefUserAgent;
	}
	// ENDE
}

