// TmpFavGroup.h : TMPFAVGROUP �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_TMPFAVGROUP_H__22DC3708_8055_4550_8892_96A210526428__INCLUDED_)
#define AFX_TMPFAVGROUP_H__22DC3708_8055_4550_8892_96A210526428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��



/////////////////////////////////////////////////////////////////////////////
// CTmpFavGroupApp
// ���̃N���X�̓���̒�`�Ɋւ��Ă� TmpFavGroup.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CTmpFavGroupApp : public CWinApp {
public:
	CTmpFavGroupApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTmpFavGroupApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTmpFavGroupApp)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TMPFAVGROUP_H__22DC3708_8055_4550_8892_96A210526428__INCLUDED_)
