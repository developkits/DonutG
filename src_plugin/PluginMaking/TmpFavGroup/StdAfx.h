// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__AC0F7B10_5199_4E99_8E2A_05EEC6FDD7B8__INCLUDED_)
#define AFX_STDAFX_H__AC0F7B10_5199_4E99_8E2A_05EEC6FDD7B8__INCLUDED_

//+++ win2k�ȍ~�����ǁA������Ƃ���win9x���C�ɂ��Ă݂�.
#ifndef WINVER
//#define WINVER		0x0500	//+++ unDonut+mod��win2k�ȍ~�Ƃ��Ă���̂�(�Ƃ�����vs2005�ȍ~�ł̃R���p�C���̂���)
#define WINVER					0x0400
#define _WIN32_WINNT			0x0400
#define _WIN32_IE				0x0501
//#define _RICHEDIT_VER			0x0100
#endif

//+++ �{��unDonut�����ɃR���p�C������ꍇ�͂�����`����.
//    ��`���Ȃ��ꍇ�� undonut+/+mod����.
//#define USE_ORG_UNDONUT_INI

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC �� OLE �N���X
#include <afxodlgs.h>       // MFC �� OLE �_�C�A���O �N���X
#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC �f�[�^�x�[�X �N���X
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO �f�[�^�x�[�X �N���X
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcoll.h> //�R���N�V�����N���X
//#include <afx.h> //CFileFind

//�����̃p�X�͊e���̊��ɍ��킹�Đݒ肵�Ă�������
#ifdef _WIN64
#import "..\..\..\executeenv\64unDonut.exe" no_namespace named_guids
#else
#import "..\..\..\ExecuteEnv\unDonut.exe" no_namespace named_guids
// #import "C:\\Workspace\\project\\unDonut\\unDonut.exe" no_namespace named_guids
#endif
#include "resource.h"
#include "DonutApi.h"
#include "misc.h"		//+++
//+++ #include "MtlProfile.h"
#include "IniFile.h"	//+++
#include "ExStyle.h"
#include "ExStyleDialog.h"
#include <comdef.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__AC0F7B10_5199_4E99_8E2A_05EEC6FDD7B8__INCLUDED_)
