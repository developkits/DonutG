// Memo.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "PluginInfo.h"
#include "Memo.h"


CMemo      	g_Memo;
CAppModule 	_Module;



BOOL APIENTRY DllMain( HANDLE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID /*lpReserved*/
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		//DLL�����[�h���ꂽ
		HRESULT hRes = ::CoInitialize(NULL);

		if ( FAILED(hRes) )
			return FALSE;

		hRes = _Module.Init(NULL, (HINSTANCE) hModule);

		if ( FAILED(hRes) ) {
			::CoUninitialize();
			return FALSE;
		}

		return TRUE;
	} else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		//DLL���A�����[�h���ꂽ
		if (::IsWindow(g_Memo.m_hWnd))	//+++ �ǂ��������ɂ���܂łɑ��Ȃ��Ȃ��Ă�ꍇ������悤�Ȃ�Ń`�F�b�N�ǉ�
			g_Memo.DestroyWindow(); //�E�B���h�E���폜����
		_Module.Term();
		::CoUninitialize();
		return TRUE;
	}

	return TRUE;
}


//�E�B���h�E���쐬���Ă��̃n���h����Ԃ�
//���̃E�B���h�E���c�[���o�[�Ƃ���unDonut���Ɏ�荞�܂��
extern "C" HWND WINAPI CreateExplorerPane(HWND hWndParent, UINT nID)
{
	//�E�B���h�E�̏�����
	RECT rc = { 0, 0, 1, 1 };   //�T�C�Y��unDonut���ɂ���Č�Œ��������

	return g_Memo.Create(hWndParent, rc, nID);
}


//�v���O�C���̏���unDonut���ɓn���֐�
//������G�N�X�|�[�g���Ȃ��ƃv���O�C���ƔF������Ȃ�
//�\���̂�萔�ɂ��Ă�PluginInfo.h�ɋL�q����
extern "C" void WINAPI GetPluginInfo(PLUGININFO *pstPluginInfo)
{
	memset( pstPluginInfo, 0, sizeof (PLUGININFO) );
	pstPluginInfo->type = PLT_EXPLORERBAR;

	strcpy( pstPluginInfo->name			, "������-1" );
	//strcpy( pstPluginInfo->version	, "1.0.0" );
	strcpy( pstPluginInfo->version		, "1.0.0 +mod.0.2" );
	//strcpy( pstPluginInfo->versionDate, "2004/07/06" );
	strcpy( pstPluginInfo->versionDate	, "2008/08/10" );
	//strcpy( pstPluginInfo->authorName	, "�~�j�b�g" );
	strcpy( pstPluginInfo->authorName	, "�~�j�b�g (���C�� +mod)" );
	strcpy( pstPluginInfo->authorUrl	, "http://www5.ocn.ne.jp/minute/" );
	//strcpy( pstPluginInfo->authorEmail, "sakurabe@violin.ocn.ne.jp" );		//+++ ���̃A�h���X�͂��łɂȂ�?�͂�
	strcpy( pstPluginInfo->authorEmail	, "" );									//+++
	strcpy( pstPluginInfo->comment		, "�������v���O�C�� \r\n" );
}



//DonutP����L�[���͂Ɋւ��郁�b�Z�[�W�������Ă���
extern "C" BOOL WINAPI PreTranslateMessage(MSG *pMsg)
{
	//DLL�ɂ̓��b�Z�[�W���[�v���Ȃ��̂Ŏ����ŕK�v�ȂƂ���ɔz������
	LONG_PTR lRet  = 0;
	//BOOL	 bPass = TRUE;

	//�E�B���h�E�̃��b�Z�[�W����������
	BOOL     bRet  = g_Memo.ProcessWindowMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, lRet, 0);

	if (!bRet) {
		UINT	msg  = pMsg->message;
		//int	vKey = pMsg->wParam;

		if (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP || msg == WM_KEYDOWN) {
			if (g_Memo.m_hWnd && pMsg->hwnd == g_Memo.m_Edit.m_hWnd) {
				//�G�f�B�b�g�R���g���[���̃L�[���͂�����
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);
				return TRUE;
			}

			if (g_Memo.m_hWnd)	//+++ NULL�`�F�b�N�ǉ�
				bRet = g_Memo.IsDialogMessage(pMsg);
		}
	}

	return bRet;
}



//�v���O�C����`�̃��j���[��\������
//�{�̃G�N�X�v���[���o�[�́u���j���[�v���h���b�v�_�E�����ɌĂ΂��
extern "C" void WINAPI ShowExplorerMenu(int x, int y)
{
	HMENU hMenu    = ::LoadMenu( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MENU) );
	if ( !::IsMenu(hMenu) )
		return;

	HMENU hMenuSub = ::GetSubMenu(hMenu, 0);
	if ( !::IsMenu(hMenuSub) )
		return;

	::TrackPopupMenu(hMenuSub, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, x, y, 0, g_Memo.m_hWnd, NULL);

	::DestroyMenu(hMenu);
}
