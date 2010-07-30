/**
 *	@file	SearchBar.h
 *	@brief	�����o�[
 *	@note
 *		template�Ŏ�������Ă��� SearchBar.h �𕁒ʂ�class�ɂ��� .h, .cpp ����������.
 */

#ifndef SEARCHBAR_H
#define SEARCHBAR_H
#pragma once

#include "FlatComboBox.h"
#include "FlatComboBox_SearchKeyword.h"


enum ESts {
	STS_ENG_CHANGE_GO		=	0x00000001,
	STS_DROP_GO 			=	0x00000002,
	STS_HISTORY_SAVE		=	0x00000004,
	STS_NO_REPOSITION		=	0x00000008,
	STS_KEY_CHANGE_GO		=	0x00000010,
	STS_HEIGHTCOUNT 		=	0x00000020,
	STS_TEXT_FILTER 		=	0x00000040,
	STS_LASTSEL 			=	0x00000080,
	STS_SCROLLCENTER		=	0x00000100,
	STS_ACTIVEWINDOW		=	0x00000200,
	STS_AUTOHILIGHT 		=	0x00000400,
};


enum {
	DEFAULT_HEIGHTCOUNT 	=  25,
	MAXHEIGHTCOUNT			= 100,
};



extern bool 		g_bNoReposition;


/// �����o�[�̏���
class CDonutSearchBar
	: public CDialogImpl< CDonutSearchBar >
	, public IDropTargetImpl< CDonutSearchBar >
	, public IDropSourceImpl< CDonutSearchBar >
{
public:
	enum { IDD = IDD_SEARCHBAR_FORM };


	CDonutSearchBar();
	~CDonutSearchBar() { s_pThis_ = NULL; } 	//+++ �蔲���̐K�@��.

	static CDonutSearchBar* GetInstance() { return s_pThis_; }	//+++ ���܂��C�����悭�Ȃ���...�蔲���ŗp��.
	//static CDonutSearchBar& instance() { return *s_pThis_; }	//+++

	static CString	GetSearchIniPath();

	const CString& 	GetSearchStr();       	// { return MtlGetWindowText(GetEditCtrl()); }		//+++
	const CString& 	GetSearchEngineStr();	// { return MtlGetWindowText(m_cmbEngine); }		//+++
	//HMENU 	GetSearchEngineMenu();		//+++
	CMenuHandle GetSearchEngineMenuHandle();

	HWND		GetHWndToolBar() { return m_wndToolBar; }
	CEdit		GetEditCtrl();
	bool		GetHilightSw() { return m_bShowToolBar && GetHilightBtnFlag(); }

	void		RefreshEngine() { _InitialEngine(m_cmbEngine.m_hWnd); SetCmbKeywordEmptyStr();} //+++
	void		SetCmbKeywordEmptyStr();	//+++ �L�[���[�h���͗�����̎��ɃG���W������\�����邽�߂̐ݒ�

	void		OpenSearch(CString strWord, CString strSearchEng, int &nLoopCnt, BOOL &bFirst);

  #if 1 	//+++ ".url"���ɋL�q�����g�����ł̌����ɑΉ����邽�߁A�ǉ�
	bool		GetOpenURLstr(CString& strOpenURL, const CString& strWord0, CIniFileI& pr, const CString& frontURL0);
  #endif

	BYTE		PreTranslateMessage(MSG *pMsg);
	void		SetFocusToEngine();

	DROPEFFECT	OnDragEnter(IDataObject *pDataObject, DWORD dwKeyState, CPoint /*point*/);
	DROPEFFECT	OnDragOver(IDataObject *pDataObject, DWORD dwKeyState, CPoint /*point*/, DROPEFFECT dropOkEffect);
	void		OnDragLeave();

	DROPEFFECT	OnDrop(IDataObject *pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint /*point*/);

	void		SetFont(HFONT hFont, BOOL bRedraw = TRUE);
	void		ShowToolBarIcon(BOOL flag);

	bool		ForceSetHilightBtnOn(bool sw);

	void		SearchWeb();
	void		SearchHilight();
	void		SearchPage(BOOL bForward);

	BOOL		GetToolIconState();
	void		FilterString(CString &str) { str.Replace( _T("�@"), _T(" ") ); }

	void		ReloadSkin(int nCmbStyle);
	void		SetComboStyle(int nCmbStyle);

	void 		InitComboBox_for_AddressBarPropertyPage(CComboBox& rCmbCtrl, CComboBox& rCmbShift);

	const CString	RemoveShortcutWord(const CString &str);

	//+++ ���Ƃ�private�ɖ߂����Ƃ肠�����e�X�g.
	void		_AddToSearchBoxUnique(const CString &strURL);

public:
	// Message map and handlers
	BEGIN_MSG_MAP(CDonutSearchBarEx)
		MSG_WM_INITDIALOG	(OnInitDialog	)
		MSG_WM_DESTROY		(OnDestroy		)
		MSG_WM_MOUSEMOVE	(OnMouseMove	)
		MSG_WM_LBUTTONDOWN	(OnLButtonDown	)
		MSG_WM_LBUTTONUP	(OnLButtonUp	)
		MSG_WM_COMMAND		(OnCommand		)
		NOTIFY_CODE_HANDLER (TTN_GETDISPINFO, OnToolTipText)

		MESSAGE_HANDLER 	(WM_SIZE		, OnSize	   )
		MESSAGE_HANDLER 	(WM_ERASEBKGND	, OnEraseBkgnd )
		//COMMAND_HANDLER_EX	(IDC_CMB_ENGIN	, CBN_DBLCLK   , OnSelDblClkForEngine)
		COMMAND_HANDLER_EX	(IDC_CMB_ENGIN	, CBN_SELCHANGE, OnSelChangeForEngine)
		COMMAND_HANDLER_EX	(IDC_CMB_KEYWORD, CBN_SELCHANGE, OnSelChange)

		COMMAND_RANGE_HANDLER_EX(ID_INSERTPOINT_SEARCHENGINE, ID_INSERTPOINT_SEARCHENGINE_END, OnSearchWeb_engineId)	//+++
		COMMAND_ID_HANDLER( ID_SEARCHENGINE_MENU, OnSearchEngineMenu )			//+++

		//checkEngineNameEnable();	//* +++ �e�X�g
		//MSG_WM_RBUTTONUP	(OnToolBarRButtonUp	)			//+++

	  #if SEARCH_JIKKEN == 1	//+++
		NOTIFY_CODE_HANDLER(RBN_CHEVRONPUSHED, OnChevronPushed) //+++
	  #elif SEARCH_JIKKEN == 2	//+++
		CHAIN_MSG_MAP(CChevronHandler<CDonutSearchBar >)		//*+++
	  #endif
	ALT_MSG_MAP(1)
		MSG_WM_CHAR 		(OnChar 		 )
		MSG_WM_KEYDOWN		(OnKeywordKeyDown)
		NOTIFY_CODE_HANDLER_EX(CBN_DROPDOWN , OnKeywordCbnDropDown)
	ALT_MSG_MAP(2)
		MSG_WM_KEYDOWN		(OnEngineKeyDown  )
		MSG_WM_RBUTTONUP	(OnEngineRButtonUp)
		MSG_WM_SETFOCUS 	(OnEngineSetFocus )
		MSG_WM_KILLFOCUS	(OnEngineKillFocus)
		MESSAGE_HANDLER 	(WM_CTLCOLORLISTBOX, OnCtlColor 	  )
	ALT_MSG_MAP(3)
		MESSAGE_HANDLER 	(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	END_MSG_MAP()


private:
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnToolTipText(int idCtrl, LPNMHDR pnmh, BOOL & /*bHandled*/);

	void	OnCommand(UINT uFlag, int nID, HWND hWndCtrl);
	void	_OnCommand_SearchWeb(CString &str);
	void	_OnCommand_SearchHilight(CString &str);
	void	_OnCommand_SearchPage(BOOL bForward, int no=-1);
	void	SearchWeb_str_engine(CString &str, const CString&  strSearchEng);
	void	OnToolBarRButtonUp(UINT nFlags, const CPoint& point);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	void	InitToolBar(/*+++ UINT nImageBmpID, UINT nHotImageBmpID,*/ int cx, int cy, COLORREF clrMask, UINT nFlags = ILC_COLOR24);

	CString GetSkinSeachBarPath(BOOL bHot);
	LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);

	//�X���b�h�𗘗p���ăR���{�{�b�N�X�Ƀf�[�^��o�^(INI����̓ǂݍ��݂Ɏ��Ԃ������邽��)
	static UINT WINAPI _SearchThread(void* lpParam);
	bool	IsAliveSearchThread() const;

	void	checkToolBarWords();							//+++
	bool	toolBarAddButtons(bool chk=0);					//+++
	int		addDefaultToolBarIcon(unsigned n);				//+++
	int		addDefaultToolBarIcon_tinyWordButton(unsigned n);	//+++
	void	toolBarAddWordTbl(std::vector<CString>& tbl);	//+++

	int		btnWidth() const;								//+++ �������͑��̉E�̃{�^���̉���.

	void	OnDestroy();
	void	OnEngineKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	LRESULT OnCtlColorListBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	void	OnKeywordKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	LRESULT OnKeywordCbnDropDown(LPNMHDR pnmh);
	BOOL	DeleteKeywordHistory();

	struct	_Function_MakeSearchFileListMenu;

	void	OnEngineRButtonUp(UINT nFlags, CPoint point);
	void	OnEngineSetFocus(HWND hWndBefore);
	void	OnEngineKillFocus(HWND hWndNew);
	void	OnMouseMove(UINT nFlags, const CPoint& pt);

  #if 0
	void	OnSelDblClkForEngine(UINT code, int id, HWND hWnd); 	///+++ �G���W���I���̉�������... ���son_.
  #endif

	///+++ �G���W���I���̉�������... ���son_...�����ǁA������Ƃ����L�p.
	void	OnSelChangeForEngine(UINT code, int id, HWND hWnd);

	void	OnSelChange(UINT code, int id, HWND hWnd);
	void	OnLButtonDown(UINT nFlags, const CPoint& pt);
	void	OnLButtonUp(UINT nFlags, const CPoint& pt);

	void	UpdateLayout(const CPoint& pt);
	void	UpdateLayout2(const CPoint& pt);

	void	_OnEnterKeyDown(int flag);

	LRESULT OnCtlColor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	//x void	OnItemSelected(const CString& str, const CString& strEngine);		//+++ strEngine �ǉ�

	void		OpenSearchGroup(const CString& strWord, const CString& strSearchEng, int &nLoopCnt, BOOL &bFirst);
	void		EncodeString(CString &str, int dwEncode);	//minit
	void		ShortcutSearch(CString &strWord, CString &strSearchEng);
	CString 	GetShortcutWord(const CString& strSearchEng);

	static void _InitialEngine(LPVOID lpV);
	static void _InitialKeyword(LPVOID lpV);

	int 		ResizeBar(int difA, int difB);						//+++

	void		SaveHistory();

	void		_DrawDragEffect(bool bRemove);

	void		_SetVerticalItemCount(CComboBox &cmb, int count);
	void		_InitCombo();						//minit

	bool		checkEngineNameEnable();					//+++
	bool 		MakeSearchEngineMenu(CMenu& menu0);			//+++
	bool 		OnSearchEngineMenu(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/);	//+++
	void 		OnSearchWeb_engineId(UINT code, int id, HWND hWnd);		//+++
	bool		GetHilightBtnFlag();						//+++

#if SEARCH_JIKKEN == 1	//+++
private:
	LRESULT 	OnChevronPushed(int /*idCtrl*/, LPNMHDR pnmh, BOOL &bHandled);
	CMenuHandle PrepareChevronMenu();
	void		CleanupChevronMenu(CMenuHandle menu, LPNMREBARCHEVRON lpnm);
	void		DisplayChevronMenu(CMenuHandle menu, LPNMREBARCHEVRON lpnm);

#elif SEARCH_JIKKEN == 2	//+++
public:
	LRESULT 	OnChevronPushed(int /*idCtrl*/, LPNMHDR pnmh, BOOL & bHandled); 	//+++
	HMENU		ChevronHandler_OnGetChevronMenu(int nCmdID, HMENU &hMenuDestroy);	//+++
	void		Chevronhandler_OnCleanupChevronMenu();		//+++

private:
	HMENU		_GetDropDownMenu(int nCmdID, bool &bDestroy, bool &bSubMenu);
#endif

private:
	typedef CDonutSearchBar 	thisClass;

	struct _ThreadParam {
		thisClass *  m_pSearchBar;
		HANDLE		 m_hExitEvent;
	};

	enum {
		STB_SHOWTOOLBARICON 	= 0x00000001,
	};

	enum EEnt_Read {
		ENT_READ_ENG			=	1,
		ENT_READ_HIST			=	2,
	};


	enum EEnter_KeyDown {
		ENTER_KEYDOWN_RETURN	=	0,
		ENTER_KEYDOWN_SELCHANGE =	1,
	};


	enum ECharEncode {
		ENCODE_NONE 			=	0,
		ENCODE_SHIFT_JIS		=	1,
		ENCODE_EUC				=	2,
		ENCODE_UTF8 			=	3,
	};

	//enum { BTN_W    = 21 };	//+++

	enum {
		m_nBarCX	=  2,
		m_nBmpCX	= 16,
		m_nBmpCY	= 16,
		s_kcxGap	=  2
	};

	enum {
		m_nDefEditT =  1,
		//+++ m_nDefDlgH	= 22
	};


private:
	CFlatComboBox_SearchKeyword		m_cmbKeyword;	//+++ �G���W�����\���̂��߁A��p�łɕύX.
	// CFlatComboBox m_cmbKeyword;

	CFlatComboBox	 m_cmbEngine;
	CToolBarCtrl	 m_wndToolBar;

	HCURSOR 		 m_hCursor;
	CPoint			 m_ptDragHist;
	CPoint			 m_ptDragStart;
	CPoint			 m_ptDragEnd;
	CContainedWindow m_wndKeyword;
	CContainedWindow m_wndEngine;
	CContainedWindow m_wndKeywordCombo;
	HWND			 m_hWndKeywordList;
	HANDLE			 m_hThread;
	_ThreadParam	 m_ThreadParams;

	CString			 m_strKeyword;					//+++ �L�[���[�h�̃R�s�[	 �����[�g�I�ɍŐV�̒l�łȂ��ꍇ����
	CString			 m_strEngine;					//+++ �T�[�`�G���W�����R�s�[ �����[�g�I�ɍŐV�̒l�łȂ��ꍇ����
	CString 		 m_toolBarAddWordStr;			//+++ �����P�ꂪ�V���ɕύX���ꂽ���̃`�F�b�N�p

	int 			 m_nDefDlgH;					///+++ �ϐ���
	int 			 m_nItemFontH;					///< +++ �t�H���g�̍������狁�߂��A�C�e���̍���
	int 			 m_cxBtnsBtn;					///< �� ���������p
	int 			 m_clrMask;
	int 			 m_has; 						//+++
	int 			 m_nKeywordWidth;				//+++
	int 			 m_nEngineWidth;				//+++

	DWORD			 m_dwNoWordButton;				//+++ �P��{�^�������Ȃ�true(��������L���ɂ���)
	DWORD			 m_dwUsePageButton;				//+++ �P��{�^���g�p���ɂ��P�Ƃ̃y�[�W�������{�^�����g��
	DWORD			 m_dwTinyWordButton;			//+++ �P��{�^���łȂ�5�̐����{�^�����g���ꍇ

	BYTE/*bool*/	 m_bDragAccept;
	BYTE/*bool*/	 m_bDragFromItself;
	BYTE/*BOOL*/	 m_bShowToolBar;
	BYTE/*BOOL*/	 m_bLoadedToolBar;
	BYTE/*BOOL*/	 m_bFiltering;					///< +++ �S�p�X�y�[�X�𔼊p�ɕϊ�����ꍇtrue.
	BYTE/*BOOL*/	 m_bActiveWindow;
	BYTE/*BOOL*/	 m_bUseShortcut;				//option

	BYTE/*BOOL*/	 m_bExistManifest;				//+++ .exe.manifest �t�@�C���̗L��(XP Theme�g�p�̗L��)
	BYTE/*BOOL*/	 m_bHilightSw;					//+++ �n�C���C�g�{�^����on/off���
	//int			 m_nHilightBtnNo;				//+++

	//+++ 1�����C���X�^���X������Ȃ��A���낤�Ƃ��āAParseInternetShortcutFile()�p�Ɏ蔲���Ȏ������w���|�C���^.
	static CDonutSearchBar* s_pThis_;
};



#endif	// DONUTSEARCHBAR_H
