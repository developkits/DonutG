#if !defined(AFX_EXSTYLEDIALOG_H__EE1BA1EE_8DB6_4A42_AFE8_8F1A0A5B74E4__INCLUDED_)
#define AFX_EXSTYLEDIALOG_H__EE1BA1EE_8DB6_4A42_AFE8_8F1A0A5B74E4__INCLUDED_

#include "ExStyle.h"	// ClassView �ɂ���Ēǉ�����܂����B
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExStyleDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CExStyleDialog �_�C�A���O

class CExStyleDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CExStyleDialog(CApiExStyle& style, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CExStyleDialog)
	enum { IDD = IDD_STYLE };

	BOOL	m_bDLActiveX;
	BOOL	m_bImage;
	BOOL	m_bJava;
	BOOL	m_bNaviLock;
	BOOL	m_bRunActiveX;
	BOOL	m_bScript;
	BOOL	m_bScrollBar;
	BOOL	m_bSound;
	BOOL	m_bVideo;
	BOOL	m_bBlockMailTo;
	BOOL	m_bGesture;
	BOOL	m_bFilter;
	int		m_nReload;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CExStyleDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	CApiExStyle& m_ExStyle;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CExStyleDialog)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EXSTYLEDIALOG_H__EE1BA1EE_8DB6_4A42_AFE8_8F1A0A5B74E4__INCLUDED_)
