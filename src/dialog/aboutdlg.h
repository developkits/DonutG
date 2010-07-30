/**
 *	@file	AboutDlg.h
 *	@brief	�o�[�W��������\������_�C�A���O
 */

#pragma once

#include "../resource.h"

#ifdef USE_AERO
#include "../Aero.h"
#endif

/// �o�[�W��������\������_�C�A���O�N���X
/// unDonut�{�̂�Windows�AGecko�AIE�̃o�[�W�������擾���ĕ\�����A
/// �R�s�[���邱�Ƃ��ł��܂��B
#ifdef USE_AERO
class CAboutDlg : public CAeroDialogImpl<CAboutDlg>			//+++ Aero����.
#else
class CAboutDlg : public CDialogImpl<CAboutDlg>
#endif
{
protected:
	//�e�L�X�g�{�b�N�X
	CContainedWindow m_wndEdit;

	//�R�}���h�n���h��
	LRESULT 		 OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/);
	LRESULT 		 OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/);
	LRESULT 		 OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/);

	//���̑������֐�
	CString 	GetEnvironInfo();
	CString 	GetGeckoVersion();
	CString 	GetIEVersion();
	CString 	GetUpdateInfo();
	CString 	GetOSInfo();
	CString 	GetVersion();
	CString 	GetOSName();
	CString 	GetOSName_Version3(OSVERSIONINFO &osvi);	//NT3.51
	CString 	GetOSName_Version4(OSVERSIONINFO &osvi);	//95 98 Me NT4
	CString 	GetOSName_Version5(OSVERSIONINFO &osvi);	//2000 XP 2003Server
	CString 	GetOSName_Version6(OSVERSIONINFO &osvi);	//Vista


public:
	//�萔 �_�C�A���O���\�[�XID
  //#ifdef USE_AERO		//+++
	//WORD		IDD;	//+++ Aero�Ń��\�[�X�؂�ւ��邽�ߕϐ���.
  //#else
	enum { IDD = IDD_ABOUTBOX };
  //#endif

	//���b�Z�[�W�}�b�v
	BEGIN_MSG_MAP(CAboutDlg)
	  #ifdef USE_AERO
		CHAIN_MSG_MAP(CAeroDialogImpl<CAboutDlg>)			//+++ Aero.
	  #endif
		MESSAGE_HANDLER   ( WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER( IDOK		 , OnCloseCmd  )
		COMMAND_ID_HANDLER( IDCANCEL	 , OnCloseCmd  )
	ALT_MSG_MAP(1)
		MESSAGE_HANDLER   ( WM_LBUTTONUP , OnLButtonUp )
	END_MSG_MAP()

	//�R���X�g���N�^
	CAboutDlg();
};

/////////////////////////////////////////////////////////////////////////////
