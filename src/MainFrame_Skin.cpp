/**
 *	@file	MainFrame_Skin.cpp
 *	@brief	CMainFrame �� �X�L���֌W�̏���.
 *	@note
 *		+++ MainFrame.cpp ���番��.
 */

#include "stdafx.h"
#include "MainFrame.h"


#if defined USE_ATLDBGMEM
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




// ===========================================================================
// �X�L���֌W

void CMainFrame::InitSkin()
{
	setMainFrameBg(CSkinOption::s_nMainFrameBgColor);			//+++ ���C���t���[����Bg�ݒ�.
	initCurrentIcon();											//+++ �A�C�R��
	m_CmdBar.setMenuBarStyle(m_hWndToolBar, false); 			//+++ ���j���[ (FEVATWH�̒Z���ɂ��邩�ۂ�)

	m_MDITab.SetDrawStyle(CSkinOption::s_nTabStyle);			//�^�u
	m_ReBar.RefreshSkinState(); 								//ReBar
	m_wndStatusBar.ReloadSkin( CSkinOption::s_nStatusStyle		//�X�e�[�^�X�o�[
							 , CSkinOption::s_nStatusTextColor
							 , CSkinOption::s_nStatusBackColor);

	setMainFrameCaptionSw(CSkinOption::s_nMainFrameCaption);	//+++ ���C���t���[���̃L���v�V�����̗L��.
}


HRESULT CMainFrame::OnSkinChange()
{
	InvalidateRect(NULL, TRUE);

	//���b�Z�[�W���u���[�h�L���X�g����ׂ���
	CSkinOption::GetProfile();

	setMainFrameBg(CSkinOption::s_nMainFrameBgColor);			//+++ ���C���t���[����Bg�ݒ�.

	initCurrentIcon();											//+++ �A�C�R��
	m_CmdBar.setMenuBarStyle(m_hWndToolBar, true);				//+++ ���j���[
	m_ReBar.RefreshSkinState(); 								//ReBar
	m_CmdBar.InvalidateRect(NULL, TRUE);						//���j���[�o�[
	m_MDITab.ReloadSkin(CSkinOption::s_nTabStyle);				//�^�u
	m_ToolBar.ReloadSkin(); 									//�c�[���o�[
	m_AddressBar.ReloadSkin(CSkinOption::s_nComboStyle);		//�A�h���X�o�[
	m_SearchBar.ReloadSkin(CSkinOption::s_nComboStyle); 		//�����o�[
	m_LinkBar.InvalidateRect(NULL, TRUE);						//�����N�o�[
	m_ExplorerBar.ReloadSkin(); 								//�G�N�X�v���[���o�[
	m_ExplorerBar.m_PanelBar.ReloadSkin();						//�p�l���o�[
	m_ExplorerBar.m_PluginBar.ReloadSkin(); 					//�v���O�C���o�[
	m_wndStatusBar.ReloadSkin(CSkinOption::s_nStatusStyle		//�X�e�[�^�X�o�[
							, CSkinOption::s_nStatusTextColor
							, CSkinOption::s_nStatusBackColor);

	setMainFrameCaptionSw(CSkinOption::s_nMainFrameCaption);	//+++ �L���v�V����

	//���t���b�V��
	InvalidateRect(NULL, TRUE);

	return 0;
}


#if 1 //+++	���C���t���[����Caption on/off
void CMainFrame::setMainFrameCaptionSw(int sw)
{
	if (sw == 0) {	//+++ �L���v�V�������O���ꍇ
		ModifyStyle(WS_CAPTION, 0);
	} else {
		ModifyStyle(0, WS_CAPTION); 			//+++ �L���v�V����������ꍇ.
	}
	m_mcCmdBar.SetExMode(sw ? 0/*�L*/ : m_hWnd/*��*/);

	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
}
#endif


///+++ ���݂̃X�L���̃A�C�R����(��)�ݒ�.
///    �� HICON �̊J�����Ă��Ȃ��Ă悢? �ʖڂŃ��[�N���Ă�?...win�̃��\�[�X�Ǘ����Ă悤�킩���....
void CMainFrame::initCurrentIcon()
{
	//+++ xp �r�W���A���X�^�C�����ꎞ�I��off
  #if 1	//+++ uxtheme.dll �̊֐��̌Ăяo������ύX.
	UxTheme_Wrap::SetWindowTheme(m_hWnd, L" ", L" ");
  #else
	CTheme		theme;
	theme.SetWindowTheme(m_hWnd, L" ", L" ");
  #endif

	/*static*/ HICON hIcon	= 0;
	//if (hIcon)
	//	::CloseHandle(hIcon);
	//hIcon 		= 0;
	CString strDir	= _GetSkinDir();
	m_strIcon		= strDir + "MainFrameBig.ico";
	if (Misc::IsExistFile(m_strIcon) == 0)
		m_strIcon	= strDir + "icon.ico";
	if (Misc::IsExistFile(m_strIcon))
		hIcon = (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), m_strIcon, IMAGE_ICON, 32, 32, LR_SHARED|LR_LOADFROMFILE );
	if (hIcon == 0) {
		m_strIcon.Empty();
		hIcon = (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	}
	if (hIcon)
		::SetClassLongPtr(m_hWnd, GCLP_HICON  , (LONG_PTR)hIcon );
	//::CloseHandle(hIcon);

	/*static*/ HICON hIconSm	= 0;
	//if (hIconSm)
	//	::CloseHandle(hIconSm);
	//hIconSm			= 0;
	m_strIconSm 		= strDir + "MainFrameSmall.ico";
	if (Misc::IsExistFile(m_strIconSm) == 0)
		m_strIconSm 	= strDir + "icon.ico";
	if (Misc::IsExistFile(m_strIconSm))
		hIconSm 	= (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), m_strIconSm, IMAGE_ICON, 16, 16, LR_SHARED|LR_LOADFROMFILE);
	if (hIconSm == 0) {
		m_strIconSm.Empty();
		hIconSm   = (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	}
	if (hIconSm)
		::SetClassLongPtr(m_hWnd, GCLP_HICONSM, (LONG_PTR)hIconSm );
	//::CloseHandle(hIconSm);

	//+++ XP�̃r�W���A���X�^�C���ɖ߂�...����͂���Ȃ�����
  #if 1	//+++ uxtheme.dll �̊֐��̌Ăяo������ύX.
	UxTheme_Wrap::SetWindowTheme(m_hWnd, 0, 0);
  #else
	theme.SetWindowTheme(m_hWnd, 0, 0);
  #endif
}


//+++ ���C���t���[����Bg�ݒ�.
void CMainFrame::setMainFrameBg(int bgColor)
{
	m_nBgColor = bgColor;

	CString 	strPath = _GetSkinDir() + _T("bg.bmp");
	m_bmpBg.Attach( AtlLoadBitmapImage(strPath.GetBuffer(0), LR_LOADFROMFILE) );
}


///+++ bg�`��
bool CMainFrame::drawMainFrameBg(HDC hDC)
{
  #if 1 //+++ bg�^�C���̂�
	if (m_bmpBg.m_hBitmap) {		// bg�摜��~���l�߂ĕ\��
		DrawBmpBg_Tile(hDC);
		return 1;
	} else if (m_nBgColor >= 0) {	// �F�w�肪����΁A���̐F�łׂ��h��
		HBRUSH	hBrushBg = CreateSolidBrush(COLORREF(m_nBgColor));
		RECT	rect;
		GetClientRect(&rect);
		::FillRect( hDC, &rect, hBrushBg );
		DeleteObject(hBrushBg);
		return 1;
	}
	return 0;
  #else //+++ �w�i�F��bg�������ɕ\�������K�v������Ƃ�
	if (m_nBgColor < 0 || m_bmpBg.m_hBitmap == 0)
		return 0;

	if (m_nBgColor >= 0) {
		HBRUSH	hBrushBg = CreateSolidBrush(COLORREF(m_nBgColor));
		RECT	rect;
		GetClientRect(&rect);
		::FillRect( hDC, &rect, hBrushBg );
		DeleteObject(hBrushBg);
	}
	if (m_bmpBg.m_hBitmap) {
		//if (m_nBgStyle == SKIN_BG_STYLE_TILE)
		DrawBmpBg_Tile(hDC);
		//else if (m_nBGStyle == SKN_BG_STYLE_STRETCH)
		//DrawBmpBg_Stretch(hDC);
	}
	return 1;
  #endif
}


//+++ bg�`��(�~���l�߂�^�C�v)
void CMainFrame::DrawBmpBg_Tile(HDC hDC)
{
	CRect	rc;
	GetClientRect(&rc);

	CDC 	dcSrc;
	dcSrc.CreateCompatibleDC(hDC);

	HBITMAP hOldbmpSrc	= dcSrc.SelectBitmap(m_bmpBg.m_hBitmap);
	SIZE	size;
	m_bmpBg.GetSize(size);
	DWORD	srcW = size.cx;
	DWORD	srcH = size.cy;
	DWORD	dstW = rc.Width();
	DWORD	dstH = rc.Height();
	for (unsigned y = 0; y < dstH; y += srcH) {
		for (unsigned x = 0; x < dstW; x += srcW) {
			::BitBlt(hDC, x, y, srcW, srcH, dcSrc, 0, 0, SRCCOPY);
		}
	}

	dcSrc.SelectBitmap(hOldbmpSrc);
}


#if 0 //+++ ��ʂ̍X�V���悤�킩��Ȃ��̂ŁA�g�k�͎~��
//+++ ���摜���g�債�ĕ\��.
void CMainFrame::DrawBmpBg_Stretch(HDC hDC)
{
	CRect	rc;
	GetClientRect(&rc);
  #if 0
	InvalidateRect(rc, TRUE);
	UpdateWindow();
  #endif

	CDC 	dcSrc;
	dcSrc.CreateCompatibleDC(hDC);

	HBITMAP hOldbmpSrc	= dcSrc.SelectBitmap(m_bmpBg.m_hBitmap);
	SIZE	size;
	m_bmpBg.GetSize(size);
	DWORD	srcW = size.cx;
	DWORD	srcH = size.cy;
	double	srcR = double(srcW) / double(srcH);
	DWORD	dstW = rc.Width();
	DWORD	dstH = rc.Height();
	double	dstR = double(dstW) / double(dstH);
	if (dstR > srcR) {			//+++ �`��͈͂̂ق����A���摜��������
		srcH = DWORD( srcH * srcR / dstR );
	} else if (dstR < srcR) {	//+++ �`��͈͂̂ق����A���摜�����c��
		srcW = DWORD( srcW * dstR / srcR );
	}
	::StretchBlt(hDC, 0, 0, dstW, dstH, dcSrc , 0, 0, srcW, srcH, SRCCOPY);
	dcSrc.SelectBitmap(hOldbmpSrc);
	//ValidateRect(NULL);
}
#endif


//public:
#if 0 //+++ ���Ƃ�
HICON	CMainFrame::LoadIcon4AboutDialog()
{
	int 	w = ::GetSystemMetrics(SM_CXICON);
	int 	h = ::GetSystemMetrics(SM_CYICON);
	HICON	hIcon = 0;
  #if 1
	if (IsExistFile(m_strIcon))
		hIcon = (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), m_strIcon, IMAGE_ICON, w, h, LR_SHARED|LR_LOADFROMFILE);
	if (hIcon == 0)
  #endif
		hIcon = (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, w, h, LR_DEFAULTCOLOR );
	return hIcon;
}
#endif

