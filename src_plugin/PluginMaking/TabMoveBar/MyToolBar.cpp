// MyToolBar.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "TabMoveBar.h"
#include "MyToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//+++ 32/64�r�b�g�ŕʂ̃v���O�C���t�H���_
#ifdef WIN64
#define PLUGIN_DIR		"plugin64\\"
#else
#define PLUGIN_DIR		"plugin32\\"
#endif



/////////////////////////////////////////////////////////////////////////////
// CMyToolBar

CMyToolBar::CMyToolBar()
{
}


CMyToolBar::~CMyToolBar()
{
}


BEGIN_MESSAGE_MAP(CMyToolBar, CToolBarCtrl)
	//{{AFX_MSG_MAP(CMyToolBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CMyToolBar ���b�Z�[�W �n���h��



int CMyToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBarCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//��֐e�E�B���h�E��ݒ�
	//�c�[���o�[�̒ʒm���b�Z�[�W���󂯂邾���̔�\���E�B���h�E
	m_wnd.Create(NULL, NULL, WS_CHILD, CRect(0, 0, 1, 1), this, MY_WINDOW);
	SetOwner(&m_wnd);

	// �C���[�W���X�g�̍쐬
	CBitmap  bitmap;
	HBITMAP  hBmp;
	m_ilist.Create(16, 16, ILC_COLOR24 | ILC_MASK, TOOLBTNCOUNT, 1);
	//+++ plugin32/64�t�H���_���܂��݂�
	hBmp = (HBITMAP) ::LoadImage(AfxGetResourceHandle(), PLUGIN_DIR "TabMoveBarRes\\toolbar.bmp",
									 IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE );
	if (hBmp == NULL) {	//+++ �Ȃ���΁Aplugin/ ���݂�
		hBmp = (HBITMAP) ::LoadImage(AfxGetResourceHandle(), "Plugin\\TabMoveBarRes\\toolbar.bmp",
									 IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE );
	}
	if (hBmp == NULL) {
		bitmap.LoadBitmap(IDB_BITMAP);
		m_ilist.Add( &bitmap, RGB(255, 0, 255) );
	} else {
		m_ilist.Add( bitmap.FromHandle(hBmp), RGB(255, 0, 255) );
	}
	SetImageList(&m_ilist);

	//�{�^���̓o�^
	TBBUTTON 	bt[TOOLBTNCOUNT];

	for (int i = 0; i < TOOLBTNCOUNT; i++) {
		bt[i].iBitmap	= i;
		bt[i].idCommand = MY_TOOLBTN1 + i;
		bt[i].fsStyle	= BTNS_BUTTON;
		bt[i].fsState	= TBSTATE_ENABLED;
		bt[i].dwData	= 0;
		bt[i].iString	= NULL;
	}
	AddButtons(TOOLBTNCOUNT, bt);

	return 0;
}
