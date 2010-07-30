// TabMoveBar.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "TabMoveBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
/*
	�O���[�o���ȕϐ��y�ъ֐����`�E�������郂�W���[��
*/
//
#include "PluginInfo.h"
#include "MyToolBar.h"
#include "MyWindow.h"

CTabMoveBarApp theApp;		// �B��� CTabMoveBarApp �I�u�W�F�N�g
CMyToolBar  m_wndToolBar;	// �c�[���o�[�I�u�W�F�N�g






// CTabMoveBarApp

BEGIN_MESSAGE_MAP(CTabMoveBarApp, CWinApp)
	//{{AFX_MSG_MAP(CTabMoveBarApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTabMoveBarApp::CTabMoveBarApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

//�I�������E�����ŃE�B���h�E�Ȃǂ��폜���Ă���
int CTabMoveBarApp::ExitInstance() 
{
	m_wndToolBar.DestroyWindow();
	return CWinApp::ExitInstance();
}
/////////////////////////////////////////////////////////////////////////////



//�G�N�X�|�[�g����֐�
//������DonutP����Ă΂��


//�E�B���h�E���쐬���Ă��̃n���h����Ԃ�
//���̃E�B���h�E���c�[���o�[�Ƃ���DonutP���Ɏ�荞�܂��
extern "C" HWND WINAPI CreateToolBar(HWND hWndParent, UINT nID)
{
	//�c�[���o�[�E�B���h�E�𐶐�
	m_wndToolBar.Create(TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|CCS_TOP|WS_CHILD|WS_VISIBLE|
		CCS_NORESIZE|CCS_NODIVIDER|CCS_NOPARENTALIGN,
		CRect(0,0,100,24),CWnd::FromHandle(hWndParent),nID);

	return m_wndToolBar.m_hWnd;
}


//�v���O�C���̏���DonutP���ɓn���֐�
//������G�N�X�|�[�g���Ȃ��ƃv���O�C���ƔF������Ȃ�
//�\���̂�萔�ɂ��Ă�PluginInfo.h�ɋL�q����
extern "C" void WINAPI GetPluginInfo(PLUGININFO* pstPluginInfo)
{
	memset( pstPluginInfo, 0, sizeof(PLUGININFO) );
	pstPluginInfo->type = PLT_TOOLBAR;

	strcpy( pstPluginInfo->name, "TabMoveBar" );
	strcpy( pstPluginInfo->version, "1.1.1" );
	strcpy( pstPluginInfo->versionDate, "2003.07.24" );
	strcpy( pstPluginInfo->authorName, "�~�j�b�g" );
	strcpy( pstPluginInfo->authorUrl, "-----" );
	strcpy( pstPluginInfo->authorEmail, "-----" );
	strcpy( pstPluginInfo->comment, "�^�u�ړ��@�\��p�c�[���o�[ \r\n" );
}

//����������Ă����ƃN���X��PreTranslateMessage()�����p�ł���悤�ɂȂ�
//�����Ă��悢�����܂������Ȃ��Ȃ邱�Ƃ�����
extern "C" void WINAPI PreTranslateMessage(MSG* pMsg)
{
	theApp.PreTranslateMessage(pMsg);
}



