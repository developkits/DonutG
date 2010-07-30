// TmpFavTreeView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "TmpFavGroup.h"
#include "TmpFavTreeView.h"
//#include "MtlProfile.h"
#include "IniFile.h"
//#include "Misc.h"			//+++


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CTmpFavTreeView

CTmpFavTreeView::CTmpFavTreeView()
	: m_dwExStyle(0)
	, m_nNewCount(0)		//+++
	, m_renameEndFunc(0)	//+++
{
}


CTmpFavTreeView::~CTmpFavTreeView()
{
	ASSERT(!m_nNewCount);
}


BEGIN_MESSAGE_MAP(CTmpFavTreeView, CTreeCtrl)
	//{{AFX_MSG_MAP(CTmpFavTreeView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_RCLICK			, OnRclick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN		, OnKeydown)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT	, OnEndlabeledit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




// ***************************************************************************
// �I�[�o�[���C�h�����֐�
// ***************************************************************************


//���ڂ̃A�v���P�[�V������`�f�[�^�̈��new�ŕ�����p�̃��������m�ۂ����̂�
//�W���̊֐��ō��ڂ��폜����ƃ��������[�N��������
//����č��ڂ��폜����ꍇ�̓I�[�o�[���C�h�����ȉ��̊֐���p����
BOOL CTmpFavTreeView::DeleteItem(HTREEITEM hItem)
{
	BOOL		  bRet	= DeleteChildItems(hItem);
	if (!bRet)
		return FALSE;

	FavGroupInfo *pInfo = (FavGroupInfo *) GetItemData(hItem);
	if (pInfo)
		FreeDataStruct(pInfo);

	return CTreeCtrl::DeleteItem(hItem);
}


BOOL CTmpFavTreeView::DeleteAllItems()
{
	return DeleteChildItems(TVI_ROOT);
}



// ***************************************************************************
// Private�����o
// ***************************************************************************

//�w��A�C�e�����̍��ڂ�S�č폜����
BOOL CTmpFavTreeView::DeleteChildItems(HTREEITEM hParent)
{
	//�ċA�I�Ɏq�A�C�e�����폜����
	HTREEITEM	hChild 	= GetChildItem(hParent);
	if (hChild) {
		BOOL	bRet = DeleteChildItems(hChild);
		if (!bRet)
			return FALSE;

		HTREEITEM	hChildNext;
		while ( ( hChildNext = GetNextSiblingItem(hChild) ) != NULL ) {
			bRet   = DeleteItem(hChild);
			if (!bRet)
				return FALSE;

			bRet   = DeleteChildItems(hChildNext);
			if (!bRet)
				return FALSE;

			hChild = hChildNext;
		}

		bRet = DeleteItem(hChild);
		if (!bRet)
			return FALSE;
	}

	return TRUE;
}


//���C�ɓ���O���[�v���c���[�ɒǉ�����
BOOL CTmpFavTreeView::InsertFavGroup(const CString &strGroupTitle, const CString &strFilePath)
{
	HTREEITEM hItem = InsertItem(strGroupTitle, 0, 1, TVI_ROOT);
	if (!hItem)
		return FALSE;

	SetItemData(hItem, (DWORD_PTR) NULL);

	return InsertFavGroupItem(strFilePath, hItem);
}


//���C�ɓ���O���[�v�̍��ڂ݂̂��w�肵���O���[�v�A�C�e���̉��ɒǉ�����
BOOL CTmpFavTreeView::InsertFavGroupItem(const CString &strFilePath, HTREEITEM hParent)
{
	int 		   nCount = 0;
	FavGroupInfo **infos  = ReadFavGroupFile(strFilePath, &nCount); 	//����͕K��delete []����

	if (infos == NULL)
		return FALSE;

	for (int i = 0; i < nCount; i++) {
		FavGroupInfo *pInfo = infos[i];

		HTREEITEM	   hItemChild;
		if ( pInfo->strTitle.IsEmpty() ) {
			hItemChild = InsertItem(pInfo->strLocation, 2, 2, hParent);
		} else {
			hItemChild = InsertItem(pInfo->strTitle, 2, 2, hParent);	//�^�C�g���������Ƃ���URL��\��
		}

		SetItemData(hItemChild, (DWORD_PTR) pInfo);
	}

	delete[] infos; 													//����
	return TRUE;
}


//���C�ɓ���O���[�v�t�@�C�����J���Ă��̃f�[�^���i�[�����������z���Ԃ�
//���̔z��͎g���I�������delete []���邱��
FavGroupInfo **CTmpFavTreeView::ReadFavGroupFile(const CString &strFilePath, int *pnCount)
{
	CIniFileI   pr( strFilePath, _T("Header") );
	DWORD		dwCount = 0;
	pr.QueryValue( dwCount, _T("Count") );
	pr.Close();

	if (dwCount == 0)
		return NULL;

	FavGroupInfo**	infos   = new FavGroupInfo *[dwCount];				//�z��̃������m��

	CString 	   	strLocation;
	CString			strTitle;
	int 		   	nCount  = dwCount;

	for (int i = 0; i < nCount; i++) {
		GetGroupItemData(strFilePath, i, strLocation, strTitle);
		infos[i]			   = AllocDataStruct(); 					//�A�C�e���̃������m��
		infos[i]->strIniFile   = strFilePath;
		infos[i]->strLocation  = strLocation;
		infos[i]->strTitle	   = strTitle;
		infos[i]->strSection.Format(_T("%s%d"), PREFIXSECTION, i);
		infos[i]->bUseIniStyle = TRUE;
	}

	*pnCount = nCount;
	return infos;
}


//ini�t�@�C������A�C�e���Ɋւ�������擾����
void CTmpFavTreeView::GetGroupItemData(const CString &strDfgFile, int nSection, CString &strLocation, CString &strTitle)
{
	//const int	bufsize   = 1024;
	//DWORD		dwGetSize = bufsize;

	CString 	strKey;
	strKey.Format(_T("%s%d"), PREFIXSECTION, nSection);

	CIniFileI	 pr(strDfgFile, strKey);
	strTitle    = pr.GetStringUW(_T("Title"));
	strLocation = pr.GetString(_T("Location_URL"));
	//pr.Close();
}


//�G�N�X�v���[���E�X�^�C���ȃt�@�C��������s��
BOOL CTmpFavTreeView::FileOperation(const CString &strFromFilePath, const CString &strToFilePath, int nOperate, int nFlag)
{
	if ( strFromFilePath.IsEmpty() )
		return FALSE;

	TCHAR		bufFrom[MAX_PATH + 2];
	ZeroMemory(bufFrom, sizeof(TCHAR) * MAX_PATH + 2); 				//������2�̃k������
	//+++ CopyMemory( bufFrom, strFromFilePath.GetBuffer(0), strFromFilePath.GetLength() );
	CopyMemory( bufFrom, LPCTSTR(strFromFilePath), sizeof(TCHAR) * strFromFilePath.GetLength() );			//+++ UNICODE �Ή�
	LPTSTR	lpstrFrom = bufFrom;

	LPTSTR		lpstrTo;
	TCHAR		bufTo[MAX_PATH + 2];
	if ( !strToFilePath.IsEmpty() ) {
		ZeroMemory(bufTo, sizeof(TCHAR) * MAX_PATH + 2);				//������2�̃k������
		//+++ CopyMemory( bufTo, strToFilePath.GetBuffer(0), strToFilePath.GetLength() );
		CopyMemory( bufTo, LPCTSTR(strToFilePath), sizeof(TCHAR) * strToFilePath.GetLength() );				//+++ UNICODE �Ή�
		lpstrTo = bufTo;
	} else {
		lpstrTo = NULL;
	}

	SHFILEOPSTRUCT fop = {
		m_hWnd, nOperate, lpstrFrom, lpstrTo,
		nFlag,	FALSE,	  NULL, 	 NULL,
	};
	return (::SHFileOperation(&fop) == 0 && !fop.fAnyOperationsAborted );
}


//�A�C�e���̏���ۑ�����\���̂̃��������m�ۂ���
FavGroupInfo *CTmpFavTreeView::AllocDataStruct()
{
	m_nNewCount++;
	return new FavGroupInfo;
}


//��Ŋm�ۂ������������J������
void CTmpFavTreeView::FreeDataStruct(FavGroupInfo *ptr)
{
	m_nNewCount--;
	delete ptr;
}



// ***************************************************************************
// Public�����o
// ***************************************************************************



/////////////////////////////////////////////////////////////////////////////
//�S�ʏ���
/////////////////////////////////////////////////////////////////////////////

//�w��t�H���_�̂��C�ɓ���O���[�v���r���[�ɓo�^����
BOOL CTmpFavTreeView::LoadFavGroups(const CString &strFolder)
{
	if (::GetFileAttributes(strFolder) == 0xFFFFFFFF)
		return FALSE;

	if ( !DeleteAllItems() )
		return FALSE;

	CString   strMatch = strFolder + "*.dfg";

	//.dfg�ŏI���t�@�C����T���ēo�^
	CFileFind find;

	if ( find.FindFile(strMatch) ) {
		BOOL bContinue = TRUE;

		while (bContinue) {
			bContinue = find.FindNextFile();

			if ( !find.IsDirectory() )
				InsertFavGroup( find.GetFileTitle(), find.GetFilePath() );
		}
	}

	//���łɃ\�[�g����
	SortChildren(NULL);

	m_strDataFolder = strFolder;

	return TRUE;
}


//�g���X�^�C���擾
DWORD CTmpFavTreeView::GetExtendedStyle()
{
	return m_dwExStyle;
}


//�g���X�^�C���ݒ�
void CTmpFavTreeView::SetExtendedStyle(DWORD dwStyle)
{
	m_dwExStyle = dwStyle;
}



/////////////////////////////////////////////////////////////////////////////
//���C�ɓ���O���[�v����
/////////////////////////////////////////////////////////////////////////////

//�I�������O���[�v���J��
BOOL CTmpFavTreeView::OpenFavGroup()
{
	HTREEITEM 	hItem = GetSelectedItem();
	if (hItem == NULL) {
		return FALSE;
	} else if ( GetItemData(hItem) ) {
		hItem = GetParentItem(hItem);
		if (hItem == NULL)
			return FALSE;
	}

	return OpenFavGroup(hItem);
}


//�w�肵���O���[�v���J��
BOOL CTmpFavTreeView::OpenFavGroup(HTREEITEM hItem)
{
	CString strFileName = m_strDataFolder + GetItemText(hItem) + _T(".dfg");	//+++ .dfg������ǉ�
	if (::GetFileAttributes(strFileName) == 0xFFFFFFFF)
		return FALSE;
	return OpenFavGroup(strFileName);
}


//�w�肵�����C�ɓ���O���[�v�t�@�C�����J��
BOOL CTmpFavTreeView::OpenFavGroup(const CString &strFileName)
{
	CDonutApi 	Donut;
	if ( Donut.IsNull() )
		return FALSE;
	_bstr_t   bstrFileName = strFileName;
	BOOL	  bFlag 	   = (m_dwExStyle & TMPFAV_CLOSE) ? TRUE : FALSE;
	Donut.m_pDonut->LoadGroup(bstrFileName, bFlag);

	return TRUE;
}


//�J����Ă���^�u���X�i�b�v�V���b�g�Ƃ��ĕۑ�����
void CTmpFavTreeView::SaveGroup()
{
	CDonutApi Donut;
	if ( Donut.IsNull() )
		return;
	CTime	  time		    = CTime::GetCurrentTime();
	CString   strTitle		= time.Format( _T("%Y%m%d_%H%M%S") );	//�t�@�C�����͎��ԂɊ�Â�
	CString	  strFileName	= strTitle + ".dfg";
	//+++ strFileName		= m_strDataFolder +	strFileName;
  #if 1	//+++
	if (m_dwExStyle & TMPFAV_SAVENAME) {
		CFileDialog		fileDialog(FALSE, _T("dfg"), strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
		fileDialog.m_ofn.lpstrInitialDir = m_strDataFolder;
		if (fileDialog.DoModal() != IDOK)
			return;
		strFileName			= fileDialog.GetPathName();
		strTitle			= Misc::GetFileNameNoExt(Misc::GetFileBaseName(strFileName));
	} else {
		strFileName			= m_strDataFolder +	strFileName;
	}
  #endif
	_bstr_t   bstrFileName	= strFileName;
	Donut.m_pDonut->SaveGroup(bstrFileName);
	InsertFavGroup(strTitle, strFileName);
}


//�I������Ă���O���[�v���ڂ��폜
void CTmpFavTreeView::DeleteGroup()
{
	HTREEITEM hItem = GetSelectedItem();
	if (!hItem)
		return;
	if ( GetItemData(hItem) ) {
		hItem = GetParentItem(hItem);
		if (!hItem)
			return;
	}

	DeleteGroup(hItem);
}


//�w�肵���O���[�v���폜�i�t�@�C�����폜�j
void CTmpFavTreeView::DeleteGroup(HTREEITEM hItem)
{
	CString strDfgFile = m_strDataFolder + GetItemText(hItem) + _T(".dfg");
	int 	nFlag	   = 0;
	if ( !(::GetAsyncKeyState(VK_SHIFT) & 0x80000000) ) 			//�V�t�g�����Ă���Ƃ��͒��ō폜
		nFlag |= FOF_ALLOWUNDO;										//�����łȂ��Ƃ��̓S�~���s��
	BOOL	bOk 	   = FileOperation(strDfgFile, CString(), FO_DELETE, nFlag);
	if (bOk)
		DeleteItem(hItem);
}


//�I�������O���[�v�̖��O��ύX�i�t�@�C���ɂ��K�p�j
void CTmpFavTreeView::RenameGroup()
{
	HTREEITEM hItem = GetSelectedItem();
	if (!hItem)
		return;
	if ( GetItemData(hItem) )
		return;
	RenameGroup(hItem);
}


//�w�肵���O���[�v�̖��O��ύX�i�t�@�C���ɂ��K�p�j
void CTmpFavTreeView::RenameGroup(HTREEITEM hItem)
{
	//���x���ҏW�ł���悤�ɂ���
	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);

	lStyle |= TVS_EDITLABELS;
	SetWindowLong(m_hWnd, GWL_STYLE, lStyle);
	EditLabel(hItem);
	//OnEndLabelEdit�ł܂��ҏW�ł��Ȃ��悤�ɂ���
}



/////////////////////////////////////////////////////////////////////////////
//���C�ɓ���O���[�v�̎q�A�C�e������
/////////////////////////////////////////////////////////////////////////////

//�O���[�v���̎w�肵�����ڂ��J��
BOOL CTmpFavTreeView::OpenGroupItem(HTREEITEM hItem)
{
	CDonutApi Donut;

	if ( !Donut.IsNull() ) {
		FavGroupInfo *pInfo = (FavGroupInfo *) GetItemData(hItem);

		if (pInfo) {
			_bstr_t bstrLocation = pInfo->strLocation;
			BOOL	bFlagActive  = (m_dwExStyle & TMPFAV_ACTIVE) != 0;	//? TRUE : FALSE;

			//�d�l�����J�̈����̎g�����͉�����܂��񂪂���Ȋ����Ł�
			_ExInfo ExInfo;
		  #if 1	//+++
			ExInfo.lpstrIniFile = LPTSTR( LPCTSTR(pInfo->strIniFile) );
			ExInfo.lpstrSection = LPTSTR( LPCTSTR(pInfo->strSection) );
		  #else
			ExInfo.lpstrIniFile = pInfo->strIniFile.GetBuffer(0);
			ExInfo.lpstrSection = pInfo->strSection.GetBuffer(0);
		  #endif
			ExInfo.bUseIniStyle = TRUE;
			Donut.m_pDonut->NewWindow3(bstrLocation, bFlagActive, 0, (int *) &ExInfo);

			return TRUE;
		}
	}

	return FALSE;
}


/** �O���[�v�ւ̃A�N�e�B�u�^�u�̒ǉ�.
 * +++ ������1�Ȃ�S�^�u�ǉ����[�h����.
 */
void CTmpFavTreeView::AddItemToGroup(bool bAll)
{
	//�O���[�v�̎擾
	HTREEITEM	  hItem 	  = GetSelectedItem();

	if (!hItem)
		return;

	if (GetItemData(hItem) != 0) {
		hItem = GetParentItem(hItem);

		if (!hItem)
			return;
	}

	//�O���[�v�ւ̒ǉ�
	CDonutApi	  Donut;

	if ( Donut.IsNull() )
		return;

	if (Donut.m_pDonut->TabIndex == -1)
		return;

	CString 	  strDfgFile  = m_strDataFolder + GetItemText(hItem) + _T(".dfg");
	_bstr_t 	  bstrDfgFile = strDfgFile;
	int			  tabIdx_bgn  = Donut.m_pDonut->TabIndex;			//+++
	int			  tabIdx_end  = tabIdx_bgn+1;						//+++
	if (bAll) {
		tabIdx_bgn = 0;
		tabIdx_end = Donut.m_pDonut->GetTabCount();
		if (tabIdx_end <= 0)
			return;
	}

	for (int tabIdx = tabIdx_bgn; tabIdx < tabIdx_end; ++tabIdx) {	//+++ �J�����g�����łȂ��A�S�^�u�Z�[�u���s����悤�ɏC��.
	 #if 1	//+++ �ǂ��� AddGroupItem�̋A�l�� ���ۂ̎����́A�C���f�b�N�X�łȂ��A�����̖͗l... �Ȃ̂ŁA���̂悤�ɏC��.
		int 		nNum	= Donut.m_pDonut->AddGroupItem(bstrDfgFile, tabIdx/*Donut.m_pDonut->TabIndex*/);
		int			nIndex	= nNum - 1;
	 #else
		int 		nIndex	= Donut.m_pDonut->AddGroupItem(bstrDfgFile, tabIdx/*Donut.m_pDonut->TabIndex*/);
	 #endif
		if (nIndex < 0)
			return;

		//���X�g�ւ̔��f
		CString 	  strLocation, strTitle;
		//+++ GetGroupItemData(strDfgFile, nIndex - 1, strLocation, strTitle);	//+++ nIndex�����łȂ��Y�����ԍ��ɏC�������̂�-1�͕s�v.
		GetGroupItemData(strDfgFile, nIndex, strLocation, strTitle);

		HTREEITEM	  hItemChild;

		if ( strTitle.IsEmpty() )
			hItemChild = InsertItem(strLocation, 2, 2, hItem);
		else
			hItemChild = InsertItem(strTitle, 2, 2, hItem);

		FavGroupInfo*	pInfo 	= AllocDataStruct();
		pInfo->strIniFile		= strDfgFile;
		pInfo->strSection.Format(_T("%s%d"), PREFIXSECTION, nIndex);
		pInfo->strLocation 		= strLocation;
		pInfo->strTitle    		= strTitle;
		SetItemData(hItemChild, (DWORD_PTR) pInfo);
	}	//+++
}


//�I�������A�C�e�����폜
void CTmpFavTreeView::DeleteGroupItem()
{
	//�O���[�v�̎擾
	HTREEITEM hItem = GetSelectedItem();

	if (!hItem)
		return;

	if (GetItemData(hItem) == 0)
		return;

	DeleteGroupItem(hItem);
}


//�w�肵���A�C�e�����폜
void CTmpFavTreeView::DeleteGroupItem(HTREEITEM hItem)
{
	CDonutApi	  Donut;
	if ( Donut.IsNull() )
		return;

	FavGroupInfo *pInfo 		= (FavGroupInfo *) GetItemData(hItem);

	if (!pInfo)
		return;

	int 		  nIndex		= _ttoi( pInfo->strSection.Mid(PREFIXSIZE) );
	HTREEITEM	  hParent		= GetParentItem(hItem);
	if (!hParent)
		return;

	CString 	  strGroupFile	= m_strDataFolder + GetItemText(hParent) + _T(".dfg");
	_bstr_t 	  bstrGroupFile = strGroupFile;
	int 		  lRet			= Donut.m_pDonut->DeleteGroupItem(bstrGroupFile, nIndex);

	if (lRet == 0)
		return;

	DeleteChildItems(hParent);
	InsertFavGroupItem(strGroupFile, hParent);
}


//�I�������A�C�e���̐ݒ���s��
void CTmpFavTreeView::SetItemOption()
{
	HTREEITEM hItem = GetSelectedItem();

	if (hItem == NULL || GetItemData(hItem) == 0)
		return;

	SetItemOption(hItem);
}


//�w�肵���A�C�e���̐ݒ���s��
void CTmpFavTreeView::SetItemOption(HTREEITEM hItem)
{
	FavGroupInfo * info = (FavGroupInfo *) GetItemData(hItem);
	if (info == NULL)
		return;
  #if 1	//+++
	CApiExStyle    apiStyle;
	CIniFileIO		pr(info->strIniFile	, info->strSection);
	DWORD dwDLStyle		= pr.GetValue( _T("DL_Control_Flags") );
	DWORD dwReloadStyle	= pr.GetValue( _T("Auto_Refresh_Style") );
	DWORD dwExStyle		= pr.GetValue( _T("Extended_Style") );
  #else
	DWORD		   dwDLStyle, dwReloadStyle, dwExStyle;
	CApiExStyle    apiStyle;
	CIniFileIO		pr(info->strIniFile	, info->strSection);
	pr.QueryValue( dwDLStyle	, _T("DL_Control_Flags") );
	pr.QueryValue( dwReloadStyle, _T("Auto_Refresh_Style") );
	pr.QueryValue( dwExStyle	, _T("Extended_Style") );
  #endif

	apiStyle.SetApiExtendedStyle(dwDLStyle, dwExStyle, dwReloadStyle);
	CExStyleDialog dlg(apiStyle);

	if (dlg.DoModal() != IDOK)
		return;						//�_�C�A���O�\��

	//�X�V
	pr.SetValue( apiStyle.GetDLControlFlags()	 , _T("DL_Control_Flags") );
	pr.SetValue( apiStyle.GetAutoRefreshFlag()	 , _T("Auto_Refresh_Style") );
	pr.SetValue( apiStyle.GetExtendedStyleFlags(), _T("Extended_Style") );

	pr.Close();
}



// ***************************************************************************
// ���b�Z�[�W�n���h��
// ***************************************************************************

//���b�Z�[�W���n���h�����O����
BOOL CTmpFavTreeView::PreTranslateMessage(MSG *pMsg)
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST) {
		//�L�[���b�Z�[�W�͎����ŏ�������
		TranslateMessage(pMsg);
		DispatchMessage(pMsg);
		return TRUE;
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}


//�E�B���h�E�����������
int CTmpFavTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style &= ~TVS_EDITLABELS;	//���[�U�[�̃t�@�C�����ҏW���֎~
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


//�E�B���h�E���폜�����
void CTmpFavTreeView::OnDestroy()
{
	DeleteAllItems();							//�폜����Ă��܂��܂��ɃA�C�e����j�����ă��������J��
	CTreeCtrl::OnDestroy(); 					//�폜�����
}


//���N���b�N����
void CTmpFavTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT	  nRetFlag = 0;
	HTREEITEM hItem    = HitTest(point, &nRetFlag);
	if (hItem) {
		OpenGroupItem(hItem);					//�J�[�\�����ɍ��ڂ�����ΊJ��
	}
	CTreeCtrl::OnLButtonDown(nFlags, point);
}


//���_�u���N���b�N����
void CTmpFavTreeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HTREEITEM hItem = HitTest(point);

	if (hItem && m_dwExStyle & TMPFAV_DBLCLICK) {
		//�I�v�V�����ݒ�ɂ���Ă͂��C�ɓ���O���[�v���J��
		CString strText    = GetItemText(hItem);
		CString strDfgFile = m_strDataFolder + strText + _T(".dfg");
		OpenFavGroup(strDfgFile);
	}

	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}


//�E�N���b�N����
void CTmpFavTreeView::OnRclick(NMHDR * /*pNMHDR*/, LRESULT *pResult)
{
	CPoint	  point, pointClient;

	GetCursorPos(&point);
	pointClient = point;
	ScreenToClient(&pointClient);

	//�O���[�v���A�C�e�����𔻕ʂ��Ă��ꂼ��̃��j���[��\������
	HTREEITEM hItem = HitTest(pointClient);

	if (hItem) {
		SelectItem(hItem);

		if ( GetItemData(hItem) ) {
			//�q�i�A�C�e���j
			CMenu  menu;
			menu.LoadMenu(IDR_ITEM);
			CMenu *pmenuSub = menu.GetSubMenu(0);
			pmenuSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, GetParent(), NULL);
			*pResult = 1;
			return;

		} else {
			//�e�i�O���[�v�j
			CMenu	  menu;
			menu.LoadMenu(IDR_GROUP);
			CDonutApi Donut;
			BOOL	  bTabExist = ( !Donut.IsNull() && (Donut.m_pDonut->GetTabIndex() != -1) );
			if (!bTabExist) {
				menu.EnableMenuItem(ID_ADDITEMTOGROUP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				menu.EnableMenuItem(ID_ADDITEMTOGROUP_ALL, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			}
			CMenu *   pmenuSub	= menu.GetSubMenu(0);
			pmenuSub->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, GetParent(), NULL);
		}
	}

	//���j���[�I�����ɔ�������R�}���h���b�Z�[�W�͐e�E�B���h�E�ŏ�������

	*pResult = 0;
}


//�L�[�������ꂽ
void CTmpFavTreeView::OnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	TV_KEYDOWN *pTVKeyDown = (TV_KEYDOWN *) pNMHDR;

	//���^�[���L�[�ŊJ��
	if (pTVKeyDown->wVKey == VK_RETURN) {
		HTREEITEM hItem = GetSelectedItem();
		if (hItem) {
			if ( GetItemData(hItem) ) {
				OpenGroupItem(hItem);
			} else {
				OpenFavGroup(hItem);
			}
		}
	}

	*pResult = 0;
}


//���x���ҏW���I������
void CTmpFavTreeView::OnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	TV_DISPINFO *pTVDispInfo   = (TV_DISPINFO *) pNMHDR;

	*pResult = 0;

	CString 	 strBadChar    = _T("*\\|<>:/?\"");
	CString 	 strText;
	if (pTVDispInfo->item.pszText == NULL)
		return;

	//���O�Ɏg���Ȃ������������ĂȂ������ʂ���
	strText  = pTVDispInfo->item.pszText;

	if (strText.FindOneOf(strBadChar) != -1) {
		CString strErr;
		strErr.Format(_T("%s �͖��O�Ɏg�p���邱�Ƃ͂ł�����B"), strBadChar);
		MessageBox( strErr, _T("�G���[") );
		return;
	} else if ( strText.IsEmpty() ) {
		return;
	}

	CString 	 strFilePath   = m_strDataFolder + strText;

	if (strFilePath.Find( _T(".dfg") ) == -1)
		strFilePath += _T(".dfg");

	CString 	 strBeforeName = m_strDataFolder + GetItemText(pTVDispInfo->item.hItem) + _T(".dfg");
	FileOperation(strBeforeName, strFilePath, FO_RENAME, FOF_ALLOWUNDO);

	//���j���[���炵�����x���ҏW�ł��Ȃ��悤�ɂ���
	LONG		 lStyle 	   = GetWindowLong(m_hWnd, GWL_STYLE);
	lStyle	&= ~TVS_EDITLABELS;
	SetWindowLong(m_hWnd, GWL_STYLE, lStyle);

	*pResult = 1;

	//+++ .dfg����M������A�q���̏����X�V����K�v������...�Ƃ肠���������[�h�őΏ�.
	if (m_renameEndFunc)
		m_renameEndFunc();

}
