// LinkExtractBar.h : LINKEXTRACTBAR �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_LINKEXTRACTBAR_H__4033A31B_4E8A_4949_8301_908341E6E13F__INCLUDED_)
#define AFX_LINKEXTRACTBAR_H__4033A31B_4E8A_4949_8301_908341E6E13F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CLinkExtractBarApp
// ���̃N���X�̓���̒�`�Ɋւ��Ă� LinkExtractBar.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CLinkExtractBarApp : public CWinApp
{
public:
	CLinkExtractBarApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CLinkExtractBarApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLinkExtractBarApp)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_LINKEXTRACTBAR_H__4033A31B_4E8A_4949_8301_908341E6E13F__INCLUDED_)
