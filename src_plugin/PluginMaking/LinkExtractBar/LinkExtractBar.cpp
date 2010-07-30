// LinkExtractBar.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "LinkExtractBar.h"
#include "Misc.h"			//+++

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CLinkExtractBarApp

BEGIN_MESSAGE_MAP(CLinkExtractBarApp, CWinApp)
	//{{AFX_MSG_MAP(CLinkExtractBarApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CLinkExtractBarApp �̍\�z
//#include 			"MyToolBar.h"
#include 			"BaseWindow.h"
#include 			"PluginInfo.h"

CLinkExtractBarApp 	theApp;
//CMyToolBar		m_wndToolBar;
CBaseWindow 		g_wndBase;

CLinkExtractBarApp::CLinkExtractBarApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}



int CLinkExtractBarApp::ExitInstance()
{
	g_wndBase.DestroyWindow();
	//m_wndToolBar.DestroyWindow();
	return CWinApp::ExitInstance();
}



//===========================================================================

//+++ exe(dll)�̃p�X��Ԃ�
CString 	Misc::GetExeFileName()
{
	TCHAR	buf[MAX_PATH+4];
	buf[0] = 0;
	::GetModuleFileName(theApp.m_hInstance, buf, MAX_PATH);
	return CString(buf);
}



//===========================================================================
// interface

extern "C" HWND WINAPI CreateToolBar(HWND hWndParent, UINT nID)
{
	g_wndBase.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,CRect(0,0,25,24),CWnd::FromHandle(hWndParent),ID_BASEWINDOW);
	return g_wndBase.m_hWnd;

	//m_wndToolBar.Create(TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|CCS_TOP|WS_CHILD|WS_VISIBLE|
	//	CCS_NORESIZE|CCS_NODIVIDER|CCS_NOPARENTALIGN,
	//	CRect(0,0,100,24),CWnd::FromHandle(hWndParent),nID);

	//return m_wndToolBar.m_hWnd;
}



extern "C" void WINAPI GetPluginInfo(PLUGININFO* pstPluginInfo)
{
	memset( pstPluginInfo, 0, sizeof(PLUGININFO) );
	pstPluginInfo->type = PLT_TOOLBAR;

  #if 1	//+++
	strcpy( pstPluginInfo->name			, "LinkExtractBar" );
	strcpy( pstPluginInfo->version		, "0.3.0 mod.3" );
	strcpy( pstPluginInfo->versionDate	, "2008.07.25" );
	strcpy( pstPluginInfo->authorName	, "�~�j�b�g. modified by +mod" );
	strcpy( pstPluginInfo->authorUrl	, "-----" );
	strcpy( pstPluginInfo->authorEmail	, "-----" );
	strcpy( pstPluginInfo->comment		, "�����N���o�@�\��������c�[���o�[ \r\n" );
  #else
	strcpy( pstPluginInfo->name, "LinkExtractBar" );
	strcpy( pstPluginInfo->version, "0.3.0" );
	strcpy( pstPluginInfo->versionDate, "2003.11.02" );
	strcpy( pstPluginInfo->authorName, "�~�j�b�g" );
	strcpy( pstPluginInfo->authorUrl, "-----" );
	strcpy( pstPluginInfo->authorEmail, "-----" );
	strcpy( pstPluginInfo->comment, "�����N���o�@�\��������c�[���o�[ \r\n" );
  #endif
}



extern "C" void WINAPI PreTranslateMessage(MSG* pMsg)
{
	theApp.PreTranslateMessage(pMsg);
}
