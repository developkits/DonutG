#if !defined(AFX_MYWINDOW_H__FD081264_9589_4C1A_9D2E_F3086D9214F5__INCLUDED_)
#define AFX_MYWINDOW_H__FD081264_9589_4C1A_9D2E_F3086D9214F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyWindow.h : �w�b�_�[ �t�@�C��
//
#include "MoveTab.h"

/////////////////////////////////////////////////////////////////////////////
// CMyWindow �E�B���h�E

class CMyWindow : public CWnd {
// �R���X�g���N�^
public:
	CMyWindow();

//�����o�ϐ�
public:
	CMoveTab	m_tab;
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

protected:
	//{{AFX_VIRTUAL(CMyWindow)
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMyWindow();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMyWindow)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MYWINDOW_H__FD081264_9589_4C1A_9D2E_F3086D9214F5__INCLUDED_)
