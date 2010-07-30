// TmpFavGroup.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "TmpFavGroup.h"
#include "BaseWindow.h"
#include "PluginInfo.h"
#include "Misc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	����!
//
//		���� DLL �� MFC DLL �ɑ΂��ē��I�Ƀ����N�����ꍇ�A
//		MFC ���ŌĂяo����邱�� DLL ����G�N�X�|�[�g���ꂽ
//		�ǂ̊֐����֐��̍ŏ��ɒǉ������ AFX_MANAGE_STATE
//		�}�N�����܂�ł��Ȃ���΂Ȃ�܂���B
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �ʏ�֐��̖{�̂͂��̈ʒu�ɂ���܂�
//		}
//
//		���̃}�N�����e�֐��Ɋ܂܂�Ă��邱�ƁAMFC ����
//		�ǂ̌Ăяo�����D�悷�邱�Ƃ͔��ɏd�v�ł��B
//		����͊֐����̍ŏ��̃X�e�[�g�����g�łȂ���΂�
//		��Ȃ����Ƃ��Ӗ����܂��A�R���X�g���N�^�� MFC
//		DLL ���ւ̌Ăяo�����s���\��������̂ŁA�I�u
//		�W�F�N�g�ϐ��̐錾�����O�łȂ���΂Ȃ�܂���B
//
//		�ڍׂɂ��Ă� MFC �e�N�j�J�� �m�[�g 33 �����
//		58 ���Q�Ƃ��Ă��������B
//

/////////////////////////////////////////////////////////////////////////////
// CTmpFavGroupApp

BEGIN_MESSAGE_MAP(CTmpFavGroupApp, CWinApp)
	//{{AFX_MSG_MAP(CTmpFavGroupApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CTmpFavGroupApp �̍\�z

CTmpFavGroupApp::CTmpFavGroupApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}



/////////////////////////////////////////////////////////////////////////////

//�O���[�o���E�I�u�W�F�N�g
CTmpFavGroupApp theApp;		// �B��� CTmpFavGroupApp �I�u�W�F�N�g
CBaseWindow 	g_Base;		// �v���O�C���̍ŏ�ʃE�B���h�E



int CTmpFavGroupApp::ExitInstance()
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	// �E�B���h�E���폜����
	g_Base.DestroyWindow();
	return CWinApp::ExitInstance();
}



//�G�N�X�|�[�g����֐�
//������DonutP����Ă΂��

//�E�B���h�E���K�v�ɂȂ����ۂɌĂяo�����
//�E�B���h�E���쐬���Ă��̃n���h����Ԃ�
//���̃E�B���h�E���G�N�X�v���[���o�[�̈ꕔ�Ƃ���DonutP���Ɏ�荞�܂��
extern "C" HWND WINAPI CreateExplorerPane(HWND hWndParent, UINT nID)
{
	//�G�N�X�v���[���o�[�E�y�C���𐶐�
	RECT rc = { 0,0,1,1 }; //�傫����unDonut��������ɒ�������̂œK���ł���
	CWnd wnd; wnd.Attach(hWndParent);

	LPCTSTR lpstrClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,0,0,0);
	g_Base.Create(lpstrClassName,_T(""),WS_CHILD|WS_VISIBLE,rc,&wnd,nID);
	wnd.Detach();

	return g_Base.m_hWnd;
}



//�v���O�C���̏�񂪕K�v�ɂȂ����Ƃ��ɌĂяo�����
//������G�N�X�|�[�g���Ȃ��ƃv���O�C���ƔF������Ȃ�
//�\���̂�萔�ɂ��Ă�PluginInfo.h�ɋL�q����
extern "C" void WINAPI GetPluginInfo(PLUGININFO* pstPluginInfo)
{
	memset( pstPluginInfo, 0, sizeof(PLUGININFO) );
	pstPluginInfo->type = PLT_EXPLORERBAR; //�G�N�X�v���[���o�[�E�v���O�C�������

	strcpy( pstPluginInfo->name			, "�X�i�b�v�V���b�g" );
	strcpy( pstPluginInfo->version		, "1.0 +mod.5" );
	//strcpy( pstPluginInfo->versionDate, "2004.7.31" );
	strcpy( pstPluginInfo->versionDate	, "2009.01.11" );
	strcpy( pstPluginInfo->authorName	, "�~�j�b�g. modified by +mod" );
	strcpy( pstPluginInfo->authorUrl	, "-----" );
	strcpy( pstPluginInfo->authorEmail	, "-----" );
	strcpy( pstPluginInfo->comment		, "�X�i�b�v�V���b�g�v���O�C�� \r\n"
									 	  "���C�ɓ���O���[�v�̈ꎞ�t�@�C�����Ǘ�����v���O�C���ł��B\r\n"
										  "unDonut+/+mod �Ɩ{��unDonut�ƂŃf�[�^�ɔ�݊������邽�߁A\r\n"
										  "+mod�p�ɍăR���p�C���������m�ł��B\r\n"
										  "(+mod.1.34�ł�unicode�΍�(utf8��%HH��������)�ɑΉ�)"
									      );
}



//�L�[���͂Ɋւ��郁�b�Z�[�W������Ƃ��ɌĂяo�����
//MFC�ɂ����Ă�theApp�̓����֐����Ăяo�����ƂŖ��Ȃ����������
extern "C" void WINAPI PreTranslateMessage(MSG* pMsg)
{
	theApp.PreTranslateMessage(pMsg);
}



//�v���O�C����`�̃��j���[��\������
//�{�̃G�N�X�v���[���o�[�́u���j���[�v���h���b�v�_�E�����ɌĂ΂��
extern "C" void WINAPI ShowExplorerMenu(int x, int y)
{
	HMENU hMenu = g_Base.LoadPluginMenu();
	if (!hMenu)
		return;

	HMENU hMenuSub = ::GetSubMenu(hMenu,0);
	if (!::IsMenu(hMenuSub))
		return;

	::TrackPopupMenu(hMenuSub,TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, x, y, 0, g_Base.m_hWnd, NULL);

	::DestroyMenu(hMenu);
}



//===========================================================================

//+++ exe(dll)�̃p�X��Ԃ�
const CString 	Misc::GetExeFileName()
{
	TCHAR	buf[MAX_PATH+4];
	buf[0] = 0;
	::GetModuleFileName(theApp.m_hInstance, buf, MAX_PATH);
	return CString(buf);
}

