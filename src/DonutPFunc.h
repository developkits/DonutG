/**
 *	@file DonutPFunc.h
 *	@brief	DonutP �̔ėp�֐�.
 */

#ifndef __DONUTPFUNC_H__
#define __DONUTPFUNC_H__

#pragma once

#include "MtlMisc.h"
#include "IniFile.h"
#include "Misc.h"		//+++


extern const TCHAR	g_cSeparater[];



/*+++ �g�p�t�@�C���P�Ȃ̂ł�����Ɉړ�.
//#define PI						3.141592653589793238462643383279
//#define ANGLE_TO_RADIAN( a )	( (a) * PI / 180.0 )
//#define RADIAN_TO_ANGLE( a )	( (a) / PI * 180.0 )
*/


double	_GetAngle(CPoint pt1, CPoint pt2);


BOOL	_CheckOsVersion_98Later();
BOOL	_CheckOsVersion_MELater();
BOOL	_CheckOsVersion_2000Later();
BOOL	_CheckOsVersion_XPLater();
BOOL	_CheckOsVersion_VistaLater();


/// undonut.exe�Ɠ����f�B���N�g���� strFile ��������̂Ƃ��ăt���p�X��Ԃ�.
__inline CString _GetFilePath(const CString& strFile) { return Misc::GetExeDirectory() + strFile; }

CString _GetSkinDir();
CString _GetRecentCloseFile();
bool	_LoadToolTipText(int nCmdID, CString &strText);


// �������̗\��̈���ꎞ�I�ɍŏ����B�E�B���h�E���ŏ��������ꍇ�Ɠ���	//+++ DonutRAPT ���p�N���Ă������[�`��
void RtlSetMinProcWorkingSetSize();

// strText �� nFirst �����ڂ���AnCount ���������擾���ĕԂ��B�}���`�o�C�g�����Ή���
CString RtlMbbStrMid(CString strText, const int &nFirst = 0, const int &nCount = 1);

CString RtlGetSelectedText(const WTL::CEdit &edit);


//EM_GETSEL��VisualStyle�K�p����UNICODE�I���������(UNICODE���`���Ȃ��ꍇ�ł�)�̂łȂ�Ƃ�����֐�
CString _GetSelectTextWtoA(CEdit &edit);
CString _GetSelectText(CEdit &edit);
CString _GetSelectText_OnCursor(CEdit &edit);		//+++

BOOL	_AddSimpleReBarBandCtrl(HWND hWndReBar,
								HWND hWndBand,
								int nID 				= 0,
								LPTSTR lpstrTitle		= NULL,
								BOOL bNewRow			= FALSE,
								int cxWidth 			= 0,
								BOOL bFullWidthAlways	= FALSE);
bool	FileWriteString(const CString& strFile, std::list<CString>* pString);
bool	FileReadString(const CString& strFile, std::list<CString>* pString);

BOOL	_SetCombboxCategory(CComboBox &cmb, HMENU hMenu);
BOOL	_DontUseID(UINT uID);
void	_PickUpCommandSub(HMENU hMenuSub, CComboBox &cmbCmd);
void	_PickUpCommand(HMENU hMenu, int nPopup, CComboBox &cmbCmd);


//minit
BOOL	_ReplaceImageList(CString strBmpFile, CImageList &imgs, DWORD dfltRes=0);
BOOL	_QueryColorString(CIniFileI &pr, COLORREF &col, LPCTSTR lpstrKey);


//+++ .manifest�̑��݃`�F�b�N	�� CThemeDLLLoader ����Ɨ� & ������ƃ��l�[��
#if (defined UNICODE) && (defined USE_INNER_MANIFEST)
inline bool IsExistManifestFile() { return 1; }
#else
inline bool IsExistManifestFile() { return (::GetFileAttributes(Misc::GetExeFileName() + _T(".manifest")) != 0xFFFFFFFF); }
#endif

//+++ -----------------------------------
CString 		GetFavoriteLinksFolder_For_Vista();
int 			GetFontHeight(HFONT hFont);
int 			ForceMessageLoop(HWND hWnd=0);

#endif
