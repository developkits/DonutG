#pragma once

#include "resource.h"
#include <atlctrls.h>
#include <atldlgs.h>
#include <stdio.h>



class CMemo : public CDialogImpl<CMemo>, public CMessageFilter {
public:
	enum { IDD = IDD_DIALOG_MAIN }; //�_�C�A���O���\�[�X�����ɃE�B���h�E���쐬
	enum { s_cxyBorder = 2 };

	CEdit 	m_Edit;


	BOOL PreTranslateMessage(MSG * /*pMsg*/)
	{
		return FALSE;
	}


	//���b�Z�[�W�}�b�v
	BEGIN_MSG_MAP(CMemo)
		COMMAND_ID_HANDLER(ID_MEMO_FILE_NEW , OnFileNew )
		COMMAND_ID_HANDLER(ID_MEMO_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_MEMO_FILE_SAVE, OnFileSave)
		MESSAGE_HANDLER(WM_SIZE		 , OnSize)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()


	//�_�C�A���O���쐬�����
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL & /*bHandled*/)
	{
		m_Edit = GetDlgItem(IDC_EDIT);
		return 0;
	}


	//�E�B���h�E�̑傫����ύX����悤�v�����ꂽ
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL & /*bHandled*/)
	{
		int   Width  = LOWORD(lParam);
		int   Height = HIWORD(lParam);

		CEdit edit	 = GetDlgItem(IDC_EDIT);

		edit.MoveWindow( s_cxyBorder, s_cxyBorder, Width - s_cxyBorder, Height - s_cxyBorder, TRUE);
		return 0;
	}


	//�V�K�쐬
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
	{
		ResetProcedure();
		return 0;
	}


	//�J��
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
	{
		if ( !ResetProcedure() )
			return 0;

		CFileDialog fdlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
						 _T("�e�L�X�g�t�@�C��(*.txt)\0*.txt\0�S�Ẵt�@�C��(*.*)\0*.*\0"), m_hWnd);

		if (fdlg.DoModal() == IDOK) {
			ReadFromFile(fdlg.m_szFileName);
		}

		return 0;
	}


	//���O�����ĕۑ�
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
	{
		CFileDialog fdlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
						 _T("�e�L�X�g�t�@�C��(*.txt)\0*.txt\0�S�Ẵt�@�C��(*.*)\0*.*\0"), m_hWnd);

		if (fdlg.DoModal() == IDOK) {
			SaveToFile(fdlg.m_szFileName);
		}

		return 0;
	}


	//�߂�l IDYES - �ۑ���K�v�Ƃ���
	//		 IDNO  - �ۑ����Ȃ�
	//		 IDCANCEL - ������
	int _QueryResetData()
	{
		if ( m_Edit.GetModify() ) {
			return MessageBox(_T("���e���ύX����Ă��܂��B�ύX��ۑ����܂����H"), _T("Memo�v���O�C��"), MB_YESNOCANCEL);
		}

		return IDNO;
	}


	//�G�f�B�b�g�R���g���[���̏�����
	void _ResetData()
	{
		m_Edit.SetWindowText( _T("") );
		m_Edit.EmptyUndoBuffer();
		m_Edit.SetModify(FALSE);
	}


	//�V�K�쐬�E�J�����̎葱��
	BOOL ResetProcedure()
	{
		int nRet = _QueryResetData();

		if (nRet == IDCANCEL) {
			return FALSE;
		} else if (nRet == IDYES) {
			CFileDialog fdlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
							 _T("�e�L�X�g�t�@�C��(*.txt)\0*.txt\0�S�Ẵt�@�C��(*.*)\0*.*\0"), m_hWnd);

			if (fdlg.DoModal() == IDOK) {
				SaveToFile(fdlg.m_szFileName);
			} else {
				return FALSE;
			}
		}

		_ResetData();
		return TRUE;
	}


	//�t�@�C���̓ǂݍ��݁@32KB�܂�
	void ReadFromFile(LPCTSTR lpstrFile)
	{
		int   	bufsize = 2 << 15;
		char*	buf	   	= new char[bufsize + 1];		//+++ +1��ǉ�.(readcout==bufsize�̎��A�I�[\0�̂��߂̃��������͈͊O�ɂȂ�̂ŁA\0�p�̃��������m��.)
		int    	readcount;

		FILE*	fp	   	= _tfopen( lpstrFile, _T("rb") );

		if (fp == NULL) {
			MessageBox( _T("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B") );
			goto EXIT;
		}

		//�K���ɓǂݍ��݂܂�
		readcount	   = fread(buf, 1, bufsize, fp);
		buf[readcount] = '\0';
	  #ifdef UNICODE	//+++ �ǂݍ��܂ꂽ�̂�sjis�e�L�X�g�Ȃ̂ŁA�ϊ����K�v. ��sjis->utf16�ϊ���CString�ɔC����
		{
			CString	strBuf(buf);
			if ( !m_Edit.SetWindowText(strBuf) ) {
				MessageBox( _T("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B") );
				goto EXIT;
			}
		}
	  #else
		//�ʓ|�Ȃ̂�UNICODE�Ƃ��l���Ȃ�
		if ( !m_Edit.SetWindowText(buf) ) {
			MessageBox( _T("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B") );
			goto EXIT;
		}
	  #endif

	  EXIT:
		if (fp)
			fclose(fp);

		if (buf)
			delete[] buf;
	}


	//�t�@�C���ɕۑ�
	BOOL SaveToFile(LPCTSTR lpstrFile)
	{
		BOOL		 bRet	= TRUE;
		unsigned int length = m_Edit.GetWindowTextLength();

		if (length == 0)
			return FALSE;

		TCHAR*		 buf	= new TCHAR[length + 1 + 16];
		FILE *		 fp 	= _tfopen( lpstrFile, _T("wb") );
		if (fp == NULL) {
			MessageBox( _T("�t�@�C���̕ۑ��Ɏ��s���܂����B") );
			bRet = FALSE;
			goto EXIT;
		}

		m_Edit.GetWindowText(buf, length + 1);			//+++ UNICODE�Ή����Ă��Ȃ����...

	  #ifdef UNICODE
		{
			const int	 BUF2SZ = length * 2 + 1 + 15;
			char*		 buf2	= new char[ BUF2SZ ];
			::WideCharToMultiByte(932, 0, buf, length+1, buf2, BUF2SZ, 0, 0);
			length  = strlen(buf2);
			bool rc = fwrite(buf2, 1, length, fp) == length;
			delete[] buf2;
			if (rc == 0) {
				MessageBox( _T("�t�@�C���̕ۑ��Ɏ��s���܂����B") );
				bRet = FALSE;
				goto EXIT;
			}
		}
	  #else
		if (fwrite(buf, 1, length, fp) != length) {
			MessageBox( _T("�t�@�C���̕ۑ��Ɏ��s���܂����B") );
			bRet = FALSE;
			goto EXIT;
		}
	  #endif

	  EXIT:
		if (fp)
			fclose(fp);

		if (buf)
			delete[] buf;

		return bRet;
	}
};
