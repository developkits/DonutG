// TabMoveBar.h : TABMOVEBAR �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_TABMOVEBAR_H__93BE99C7_0041_4126_8BFD_08FE58A3B403__INCLUDED_)
#define AFX_TABMOVEBAR_H__93BE99C7_0041_4126_8BFD_08FE58A3B403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CTabMoveBarApp
// ���̃N���X�̓���̒�`�Ɋւ��Ă� TabMoveBar.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CTabMoveBarApp : public CWinApp
{
public:
	CTabMoveBarApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTabMoveBarApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTabMoveBarApp)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TABMOVEBAR_H__93BE99C7_0041_4126_8BFD_08FE58A3B403__INCLUDED_)



