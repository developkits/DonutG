#if !defined(AFX_MYTOOLBAR_H__B336C5E6_2EA6_43E0_9D11_08C509DBB5A7__INCLUDED_)
#define AFX_MYTOOLBAR_H__B336C5E6_2EA6_43E0_9D11_08C509DBB5A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyToolBar.h : �w�b�_�[ �t�@�C��
//
#include "MyWindow.h"

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar �E�B���h�E

class CMyToolBar : public CToolBarCtrl {
// �R���X�g���N�^
public:
	CMyToolBar();

//�����o�ϐ�
private:
	CMyWindow	m_wnd;
	CImageList	m_ilist;
private:
public:
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMyToolBar)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMyToolBar();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMyToolBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MYTOOLBAR_H__B336C5E6_2EA6_43E0_9D11_08C509DBB5A7__INCLUDED_)
