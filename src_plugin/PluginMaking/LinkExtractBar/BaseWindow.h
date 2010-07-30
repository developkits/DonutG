#if !defined(AFX_BASEWINDOW_H__87A925A8_5646_44C3_8CD9_D328421DBE25__INCLUDED_)
#define AFX_BASEWINDOW_H__87A925A8_5646_44C3_8CD9_D328421DBE25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseWindow.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CBaseWindow �E�B���h�E

class CBaseWindow : public CWnd
{
// �R���X�g���N�V����
public:
	CBaseWindow();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CBaseWindow)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	CImageList m_imgList;
	CToolBarCtrl m_wndToolBar;
	virtual ~CBaseWindow();

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CBaseWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnExtractLink();
	afx_msg void OnSendToDownloader();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_BASEWINDOW_H__87A925A8_5646_44C3_8CD9_D328421DBE25__INCLUDED_)
