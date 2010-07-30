// BaseWindow.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"

#include "TmpFavGroup.h"
#include "TmpFavTreeView.h"
#include "BaseWindow.h"
#include "DonutApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char 		   THIS_FILE[] = __FILE__;
#endif

extern CTmpFavGroupApp theApp;



/////////////////////////////////////////////////////////////////////////////
// CBaseWindow


CBaseWindow*	CBaseWindow::s_pThis_ = 0;


CBaseWindow::CBaseWindow()
{
	s_pThis_	= this;		//+++ �ǉ�.
}


CBaseWindow::~CBaseWindow()
{
	s_pThis_	= 0;		//+++
}


BEGIN_MESSAGE_MAP(CBaseWindow, CWnd)
	//{{AFX_MSG_MAP(CBaseWindow)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_OPEN_ACTIVE		, OnOpenActive		)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_OPEN_DBLCLICK		, OnOpenDblclick	)
	ON_COMMAND(ID_OPENANDCLOSE		, OnOpenandclose	)
	ON_COMMAND(ID_ADDITEMTOGROUP    , OnAddItemToGroup	)
	ON_COMMAND(ID_ADDITEMTOGROUP_ALL, OnAddItemToGroup_all )	//+++
	ON_COMMAND(ID_ADD_TMPGROUP		, OnAddTmpgroup		)
	ON_COMMAND(ID_GROUP_DELETE		, OnGroupDelete		)
	ON_COMMAND(ID_RELOAD_FOLDER		, OnReloadFolder	)
	ON_COMMAND(ID_GROUP_OPEN		, OnGroupOpen		)
	ON_COMMAND(ID_ITEM_PROPERTY		, OnItemProperty	)
	ON_COMMAND(ID_ITEM_DELETE		, OnItemDelete		)
	ON_COMMAND(ID_RENAME_GROUP		, OnRenameGroup		)
	ON_COMMAND(ID_SAVE_NAME_SW  	, OnSaveNameSw      )
	ON_COMMAND(ID_SET_DATA_DIR  	, OnSetDataDir      )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CBaseWindow ���b�Z�[�W �n���h��

int CBaseWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�f�[�^�t�H���_�̃p�X���擾����
	m_baseDataPath		= GetBaseDataFolderPath();	//+++ �����o�[�ϐ��ɕύX

	ReadProfile();								//�ݒ��ǂݍ���

	//�c���[�r���[���쐬����
	RECT	rc		= { 0, 0, lpCreateStruct->cx , lpCreateStruct->cy };
	m_Tree.Create(WS_CHILD | WS_VISIBLE | TVS_INFOTIP | TVS_SHOWSELALWAYS | TVS_SINGLEEXPAND | TVS_FULLROWSELECT | TVS_TRACKSELECT, rc, this, 1);

	//�C���[�W���X�g�̐ݒ������
	SetImageList();

	//�f�[�^�����[�h����
	m_Tree.LoadFavGroups(m_userDataPath/*strPath*/);

	//�����ɗ]�T����������
	m_Tree.SetItemHeight(m_Tree.GetItemHeight() + 4);

	//���X�|���X�������̂ł��炩���ߍ��ڂ�I�����Ă���
	m_Tree.SelectItem( m_Tree.GetChildItem(TVI_ROOT) );

	m_Tree.setRenameEndFunc( CBaseWindow::ReloadFolder );

	return 0;
}


//DestroyWindow���ꂽ�Ƃ�
void CBaseWindow::OnDestroy()
{
	CWnd::OnDestroy();
	WriteProfile(); 							//�ݒ��ۑ�
}


//�t�H�[�J�X���ݒ肳�ꂽ�Ƃ�
void CBaseWindow::OnSetFocus(CWnd *pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	::SetFocus(m_Tree.m_hWnd);					//�c���[�r���[�Ƀt�H�[�J�X�����킹��
}


//�T�C�Y�ύX�v��������
void CBaseWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	m_Tree.MoveWindow(0, 0, cx, cy, TRUE);		//�v�����ꂽ�T�C�Y�ɍ��킹��
}


//�O���̃r�b�g�}�b�v�t�@�C������g�p����A�C�R����ǂݍ���Ŏg�p
void CBaseWindow::SetImageList()
{
	CString 	strIcoPath = m_baseDataPath + _T("ico.bmp");

	HBITMAP hBmp;
	hBmp = (HBITMAP) LoadImage(theApp.m_hInstance, strIcoPath.GetBuffer(0), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTCOLOR);
	if (hBmp == NULL)
		hBmp = LoadBitmap( theApp.m_hInstance, MAKEINTRESOURCE(IDB_TREEICON) );		//�O���t�@�C���������Ƃ��̓��\�[�X����ǂݍ���
	if (hBmp == NULL)
		return;

	CBitmap 	bmp;
	bmp.Attach(hBmp);
	m_imgs.Create(16, 16, ILC_MASK | ILC_COLOR24, 1, 1);
	m_imgs.Add( &bmp, RGB(255, 0, 255) );

	m_Tree.SetImageList(&m_imgs, TVSIL_NORMAL); //�C���[�W���X�g���Z�b�g
}


//�f�B���N�g���̃p�X����
CString CBaseWindow::GetSelfDir()
{
	CString strPath = GetSelfPath();
	int 	nPos	= strPath.ReverseFind('\\');
	if (nPos)
		strPath = strPath.Left(nPos + 1);
	else
		return _T("");

	return strPath;
}


CString CBaseWindow::GetSelfPath()
{
	CString strPath;
	DWORD	dwSize = MAX_PATH;
	::GetModuleFileName(theApp.m_hInstance, strPath.GetBuffer(dwSize), dwSize);
	strPath.ReleaseBuffer();
	return strPath;
}


CString CBaseWindow::GetBaseDataFolderPath()
{
	CString strPath  = GetSelfPath();
	int 	nPos	 = strPath.ReverseFind('\\');
	CString strTitle = strPath.Mid(nPos + 1);
	strPath  = strPath.Left(nPos + 1);
	if ( (nPos = strTitle.ReverseFind('.')) > 0 )
		strTitle = strTitle.Left(nPos);

	strPath += strTitle;
	strPath += "_Dat\\";

	return strPath;
}



//�ݒ�t�@�C������I�v�V������ǂݍ���
void CBaseWindow::ReadProfile()
{
	CString 	strPath = m_baseDataPath;
	if ( strPath.IsEmpty() )
		return;
	strPath += _T("setting.ini");

	CIniFileI pr( strPath, _T("Main") );
	DWORD		dwStyle = pr.GetValue( _T("ExtendedStyle") );
  #if 1	//+++ �f�[�^�t�H���_���Z�[�u
	m_userDataPath = pr.GetString(_T("datadir"));
	if (m_userDataPath.IsEmpty() || m_userDataPath[0] == 0) {
		m_userDataPath = m_baseDataPath;
	}
  #endif
	pr.Close();

	m_Tree.SetExtendedStyle(dwStyle);
}


//�ݒ�t�@�C���ɃI�v�V�����������o��
void CBaseWindow::WriteProfile()
{
	DWORD		dwStyle = m_Tree.GetExtendedStyle();
	CString 	strPath = m_baseDataPath;
	if ( strPath.IsEmpty() )
		return;

	strPath += _T("setting.ini");

	CIniFileO	pr( strPath, _T("Main") );
	pr.SetValue( dwStyle, _T("ExtendedStyle") );
  #if 1	//+++	��t�H���_��������A�f�[�^�t�H���_����ɂ��Ă���
	if (m_userDataPath.CompareNoCase(m_baseDataPath) != 0)
		pr.SetStringUW( LPCTSTR(m_userDataPath), _T("datadir") );
	else
		pr.SetString( _T(""), _T("datadir") );
  #endif
	pr.Close();
}


void CBaseWindow::SetCheckOfMenu(CMenu &menu, UINT uItemID, DWORD dwFlag)
{
	BOOL bCheck = (m_Tree.GetExtendedStyle() & dwFlag) != 0;	//? TRUE : FALSE;

	menu.CheckMenuItem( uItemID, MF_BYCOMMAND | ( (bCheck) ? MF_CHECKED : MF_UNCHECKED ) );
}


//�v���O�C����`�̃��j���[�����[�h���Ă��̃n���h����n��
//�g���I�������DeleteMenu���邱��
HMENU CBaseWindow::LoadPluginMenu()
{
	CMenu 	menu;

	menu.LoadMenu(IDR_MENU);
	SetCheckOfMenu(menu, ID_OPEN_ACTIVE  , TMPFAV_ACTIVE);
	SetCheckOfMenu(menu, ID_OPENANDCLOSE , TMPFAV_CLOSE);
	SetCheckOfMenu(menu, ID_OPEN_DBLCLICK, TMPFAV_DBLCLICK);
	SetCheckOfMenu(menu, ID_SAVE_NAME_SW , TMPFAV_SAVENAME);	//+++

	HMENU hMenu = menu.Detach();
	return hMenu;
}


void CBaseWindow::ToggleOptionFlag(DWORD dwFlag)
{
	DWORD dwStyle = m_Tree.GetExtendedStyle();
	dwStyle 	 ^= dwFlag;
	m_Tree.SetExtendedStyle(dwStyle);
}


//�I�v�V�����̕ύX
void CBaseWindow::OnOpenActive()
{
	ToggleOptionFlag(TMPFAV_ACTIVE);
}


void CBaseWindow::OnOpenDblclick()
{
	ToggleOptionFlag(TMPFAV_DBLCLICK);
}


void CBaseWindow::OnOpenandclose()
{
	ToggleOptionFlag(TMPFAV_CLOSE);
}


//�R�}���h�E�n���h���i�قƂ��TreeView���ŗp�ӂ����֐����Ăяo�������j

void CBaseWindow::OnAddItemToGroup()
{
	m_Tree.AddItemToGroup();
}


void CBaseWindow::OnAddItemToGroup_all()	//+++
{
	m_Tree.AddItemToGroup(1);
}


void CBaseWindow::OnAddTmpgroup()
{
	m_Tree.SaveGroup();
}


void CBaseWindow::OnGroupDelete()
{
	m_Tree.DeleteGroup();
}


void CBaseWindow::OnReloadFolder()
{
	m_Tree.DeleteAllItems();
	m_Tree.LoadFavGroups(m_userDataPath);
}


void CBaseWindow::ReloadFolder()	//+++
{
	s_pThis_->OnReloadFolder();
}


void CBaseWindow::OnGroupOpen()
{
	m_Tree.OpenFavGroup();
}


void CBaseWindow::OnItemProperty()
{
	m_Tree.SetItemOption();
}


void CBaseWindow::OnItemDelete()
{
	m_Tree.DeleteGroupItem();
}


void CBaseWindow::OnRenameGroup()
{
	m_Tree.RenameGroup();
}


//+++
void CBaseWindow::OnSaveNameSw()
{
	ToggleOptionFlag(TMPFAV_SAVENAME);
}


//+++ �f�[�^�t�H���_��ݒ�
void CBaseWindow::OnSetDataDir()
{
	TCHAR			name[MAX_PATH];
	lstrcpyn(name, m_userDataPath, MAX_PATH);
	BROWSEINFO		bi = {
		m_hWnd,
		NULL,
		name,
		_T("�X�i�b�v�V���b�g��ۑ�����t�H���_"),
		BIF_RETURNONLYFSDIRS,
		BaseDir_BrowseCallbackProc,
		(LPARAM)name,
		0
	};
    LPITEMIDLIST	idlist = ::SHBrowseForFolder(&bi);
	if (idlist)
		::SHGetPathFromIDList(idlist, name);         //ITEMIDLIST����p�X�𓾂�
    CoTaskMemFree(idlist);                        //ITEMIDLIST�̉��     99/11/03����
	CString		strPath = name;
	if (strPath.IsEmpty())
		return;
	if (::GetFileAttributes(strPath) == 0xFFFFFFFF)
		return;
	m_userDataPath = strPath + _T('\\');
	OnReloadFolder();
}


//+++
int CALLBACK CBaseWindow::BaseDir_BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED) {
		::SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
    }

    return 0;
}
