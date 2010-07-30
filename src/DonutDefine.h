/**
 *	@file	DonutDefine.h
 *	@brief	Donut�ŗL��Window���b�Z�[�W�̒�`.
 */

#ifndef __DONUTDEFINE_H__
#define __DONUTDEFINE_H__

#pragma once

#define MYWM_NOTIFYICON 	(WM_APP + 100)		// UDT DGSTR
#define TM_TRAY 			0					// UDT DGSTR

// Don't change for saving rebar state
#define IDC_ADDRESSBAR					101
#define IDC_GOBUTTON					102 	//+++ ���g�p�ۂ�?
#define IDC_MDITAB						103
#define IDC_LINKBAR 					104
#define IDC_SEARCHBAR					105
#define IDC_PLUGIN_TOOLBAR				151
#define IDC_PLUGIN_IETOOLBAR			180		//+++ �e�X�g.
#define IDC_PLUGIN_EXPLORERBAR			181 	//minit

#define IDC_PROGRESS					201
#define IDC_COMBBOX 					202

// #define DONUT_HAS_CMDBAR


#define D_OPENFILE_ACTIVATE 	0x00000001
#define D_OPENFILE_NOCREATE 	0x00000002
#define D_OPENFILE_NOSETFOCUS	0x00000004

// Dev studio doesn't know BEGIN_MSG_MAP_EX
#undef	BEGIN_MSG_MAP
#define BEGIN_MSG_MAP			BEGIN_MSG_MAP_EX


#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL			0x020A
#endif

enum EDvs_Ex {
	DVS_EX_OPENNEWWIN		= 0x00000001L,
  #ifdef USE_ORG_UNDONUT_INI				//+++ unDonut�I���W�i���̒l
	DVS_EX_MESSAGE_FILTER	= 0x00000002L,
	DVS_EX_MOUSE_GESTURE	= 0x00000004L,
	DVS_EX_BLOCK_MAILTO 	= 0x00000008L,
	DVS_EX_FLATVIEW 		= 0x00000010L,	//+++ �I���W�i���ɂ͂Ȃ��̂œK���ɐݒ�.
  #else										//+++ unDonut+ �ł̒l (+mod��)
	DVS_EX_FLATVIEW 		= 0x00000002L,
	DVS_EX_MESSAGE_FILTER	= 0x00000004L,
	DVS_EX_MOUSE_GESTURE	= 0x00000008L,
	DVS_EX_BLOCK_MAILTO 	= 0x00000010L,
  #endif
};


#define DONUT_THREADWAIT		5000					//+++ �X���b�h�̏I����5�b�҂�
#define DONUT_BACKUP_THREADWAIT 90000					//+++ �����X�V�X���b�h�̏I����90�b�҂�


// UH
__declspec(selectany) BOOL	   	g_bSwapProxy 		 = FALSE;
__declspec(selectany) DWORD    	g_dwProgressPainWidth = 0;			//*+++ �蔲���ŃO���[�o���ϐ��B���ƂŁA�Ȃ�Ƃ�����...
extern	const LPCTSTR			g_lpszLight[];						//*+++ �w�b�_��
extern	class CMainFrame *		g_pMainWnd;


// to emulate DDE.
#define WM_NEWINSTANCE			(WM_USER + 16)
#define USER_MSG_WM_NEWINSTANCE(func) \
	if (uMsg == WM_NEWINSTANCE) {	  \
		SetMsgHandled(TRUE);		  \
		func( (ATOM) wParam );		  \
		lResult = 0;				  \
		if ( IsMsgHandled() )		  \
			return TRUE;			  \
	}

// to save child window options
#define WM_SAVE_OPTION	(WM_USER + 20)
#define USER_MSG_WM_SAVE_OPTION(func)			\
	if (uMsg == WM_SAVE_OPTION) {				\
		SetMsgHandled(TRUE);					\
		func( (LPCTSTR) wParam, (int) lParam ); \
		lResult = 0;							\
		if ( IsMsgHandled() )					\
			return TRUE;						\
	}

// UDT DGSTR
#define WM_UPDATE_TITLEBAR	(WM_USER + 24)
#define USER_MSG_WM_UPDATE_TITLEBAR(func)							  \
	if (uMsg == WM_UPDATE_TITLEBAR) {								  \
		SetMsgHandled(TRUE);										  \
		lResult = (LRESULT) func( (LPCTSTR) wParam, (DWORD) lParam ); \
		if ( IsMsgHandled() )										  \
			return TRUE;											  \
	}
// ENDE

// UDT DGSTR
#define WM_UPDATE_EXPBAR	(WM_USER + 28)
#define USER_MSG_WM_UPDATE_EXPBAR(func) 							  \
	if (uMsg == WM_UPDATE_EXPBAR) { 								  \
		SetMsgHandled(TRUE);										  \
		lResult = (LRESULT) func( (LPCTSTR) wParam, (DWORD) lParam ); \
		if ( IsMsgHandled() )										  \
			return TRUE;											  \
	}
// ENDE

// UH
#define WM_MENU_GOBACK	(WM_USER + 50)
#define USER_MSG_WM_MENU_GOBACK(func)				\
	if (uMsg == WM_MENU_GOBACK) {					\
		SetMsgHandled(TRUE);						\
		lResult = (LRESULT) func( (HMENU) wParam ); \
		if ( IsMsgHandled() )						\
			return TRUE;							\
	}

#define WM_MENU_GOFORWARD	(WM_USER + 51)
#define USER_MSG_WM_MENU_GOFORWARD(func)			\
	if (uMsg == WM_MENU_GOFORWARD) {				\
		SetMsgHandled(TRUE);						\
		lResult = (LRESULT) func( (HMENU) wParam ); \
		if ( IsMsgHandled() )						\
			return TRUE;							\
	}

#define WM_MENU_GET_FAV 	(WM_USER + 52)
#define USER_MEG_WM_MENU_GET_FAV(func) \
	if (uMsg == WM_MENU_GET_FAV) {	   \
		SetMsgHandled(TRUE);		   \
		lResult = (LRESULT) func();    \
		if ( IsMsgHandled() )		   \
			return lResult; 		   \
	}

#define WM_MENU_GET_FAV_GROUP	(WM_USER + 53)
#define USER_MEG_WM_MENU_GET_FAV_GROUP(func) \
	if (uMsg == WM_MENU_GET_FAV_GROUP) {	 \
		SetMsgHandled(TRUE);				 \
		lResult = (LRESULT) func(); 		 \
		if ( IsMsgHandled() )				 \
			return lResult; 				 \
	}

#define WM_USER_HILIGHT 	(WM_USER + 54)
#define MSG_WM_USER_HILIGHT(func)					  \
	if (uMsg == WM_USER_HILIGHT) {					  \
		SetMsgHandled(TRUE);						  \
		lResult = (LRESULT) func( (LPCTSTR) wParam ); \
		if ( IsMsgHandled() )						  \
			return TRUE;							  \
	}

#define WM_USER_FIND_KEYWORD	(WM_USER + 55)
#define MSG_WM_USER_FIND_KEYWORD(func)								 \
	if (uMsg == WM_USER_FIND_KEYWORD) { 							 \
		SetMsgHandled(TRUE);										 \
		lResult = (LRESULT) func( (LPCTSTR) wParam, (BOOL) lParam ); \
		if ( IsMsgHandled() )										 \
			return TRUE;											 \
	}

#define WM_USER_USED_MOUSE_GESTURE		(WM_USER + 56)
#define MSG_WM_USER_USED_MOUSE_GESTURE(func)  \
	if (uMsg == WM_USER_USED_MOUSE_GESTURE) { \
		SetMsgHandled(TRUE);				  \
		lResult = (LRESULT) func(); 		  \
		if ( IsMsgHandled() )				  \
			return lResult; 				  \
	}

#define WM_USER_CHANGE_CSS		(WM_USER + 57)
#define MSG_WM_USER_CHANGE_CSS(func)				  \
	if (uMsg == WM_USER_CHANGE_CSS) {				  \
		SetMsgHandled(TRUE);						  \
		lResult = (LRESULT) func( (LPCTSTR) wParam ); \
		if ( IsMsgHandled() )						  \
			return lResult; 						  \
	}

#if 0	//+++ ���g�p?
#define WM_MENU_GET_CSS 		(WM_USER + 58)
#endif

#define WM_MENU_GET_SCRIPT		(WM_USER + 59)
#define USER_MEG_WM_MENU_GET_SCRIPT(func) \
	if (uMsg == WM_MENU_GET_SCRIPT) {	  \
		SetMsgHandled(TRUE);			  \
		lResult = (LRESULT) func(); 	  \
		if ( IsMsgHandled() )			  \
			return lResult; 			  \
	}

#define WM_STATUS_SETICON		(WM_USER + 60)
#define WM_STATUS_SETTIPTEXT	(WM_USER + 61)		// 20061119�ǉ�
//End UH


//minit
#define WM_MENU_REFRESH_FAV 	(WM_USER + 80)
#define USER_MEG_WM_MENU_REFRESH_FAV(func)		   \
	if (uMsg == WM_MENU_REFRESH_FAV) {			   \
		SetMsgHandled(TRUE);					   \
		lResult = (LRESULT) func( (BOOL) wParam ); \
		if ( IsMsgHandled() )					   \
			return lResult; 					   \
	}

#define WM_MENU_REFRESH_FAV_GROUP		(WM_USER + 81)
#define USER_MEG_WM_MENU_REFRESH_FAV_GROUP(func)   \
	if (uMsg == WM_MENU_REFRESH_FAV_GROUP) {	   \
		SetMsgHandled(TRUE);					   \
		lResult = (LRESULT) func( (BOOL) wParam ); \
		if ( IsMsgHandled() )					   \
			return lResult; 					   \
	}

#define WM_MENU_REFRESH_SCRIPT		(WM_USER + 82)
#define USER_MEG_WM_MENU_REFRESH_SCRIPT(func)	   \
	if (uMsg == WM_MENU_REFRESH_SCRIPT) {		   \
		SetMsgHandled(TRUE);					   \
		lResult = (LRESULT) func( (BOOL) wParam ); \
		if ( IsMsgHandled() )					   \
			return lResult; 					   \
	}

#define WM_MENU_RESTRICT_MESSAGE	(WM_USER + 83)
#define USER_MSG_WM_MENU_RESTRICT_MESSAGE(func)    \
	if (uMsg == WM_MENU_RESTRICT_MESSAGE) { 	   \
		SetMsgHandled(TRUE);					   \
		lResult = (LRESULT) func( (BOOL) wParam ); \
		if ( IsMsgHandled() )					   \
			return lResult; 					   \
	}

#define WM_RELEASE_CONTEXTMENU		(WM_USER + 84)
#define USER_MSG_WM_RELEASE_CONTEXTMENU(func)					  \
	if (uMsg == WM_RELEASE_CONTEXTMENU) {						  \
		SetMsgHandled(TRUE);									  \
		lResult = (LRESULT) func(uMsg, wParam, lParam, bHandled); \
		if ( IsMsgHandled() )									  \
			return lResult; 									  \
	}

#define WM_MENU_RECENTDOCUMENT		(WM_USER + 85)
#define USER_MSG_WM_MENU_RECENTDOCUMENT(func)		\
	if (uMsg == WM_MENU_RECENTDOCUMENT) {			\
		SetMsgHandled(TRUE);						\
		lResult = (LRESULT) func( (HMENU) wParam ); \
		if ( IsMsgHandled() )						\
			return lResult; 						\
	}

#define WM_GET_EXTENDED_TABSTYLE		(WM_USER + 86)
#define USER_MSG_WM_GET_EXTENDED_TABSTYLE(func) \
	if (uMsg == WM_GET_EXTENDED_TABSTYLE) { 	\
		SetMsgHandled(TRUE);					\
		lResult = (LRESULT) func(); 			\
		if ( IsMsgHandled() )					\
			return lResult; 					\
	}

#define WM_SET_EXTENDED_TABSTYLE		(WM_USER + 87)
#define USER_MSG_WM_SET_EXTENDED_TABSTYLE(func) \
	if (uMsg == WM_SET_EXTENDED_TABSTYLE) { 	\
		SetMsgHandled(TRUE);					\
		func( (DWORD) wParam ); 				\
		if ( IsMsgHandled() )					\
			return TRUE;						\
	}

#define WM_GET_CHILDFRAME		(WM_USER + 88)
#define USER_MSG_WM_SET_CHILDFRAME(func) \
	if (uMsg == WM_GET_CHILDFRAME) {	 \
		SetMsgHandled(TRUE);			 \
		lResult = (LRESULT) func(); 	 \
		if ( IsMsgHandled() )			 \
			return lResult; 			 \
	}

#define WM_CHANGE_SKIN		(WM_USER + 89)
#define USER_MSG_WM_CHANGE_SKIN(func) \
	if (uMsg == WM_CHANGE_SKIN) {	  \
		SetMsgHandled(TRUE);		  \
		lResult = (LRESULT) func();   \
		if ( IsMsgHandled() )		  \
			return lResult; 		  \
	}

#define WM_OPEN_EXPFAVMENU		(WM_USER + 90)
#define USER_MSG_WM_OPEN_EXPFAVMENU(func)						\
	if (uMsg == WM_OPEN_EXPFAVMENU) {							\
		SetMsgHandled(TRUE);									\
		lResult = (LRESULT) func( (int) wParam, (int) lParam ); \
		if ( IsMsgHandled() )									\
			return lResult; 									\
	}

#define WM_COMMAND_DIRECT		(WM_USER + 91)
#define USER_MSG_WM_COMMAND_DIRECT(func)							\
	if (uMsg == WM_COMMAND_DIRECT) {								\
		SetMsgHandled(TRUE);										\
		lResult = (LRESULT) func( (int) wParam, (LPCTSTR) lParam ); \
		if ( IsMsgHandled() )										\
			return lResult; 										\
	}

#define WM_SEARCH_WEB_SELTEXT		(WM_USER + 92)
#define USER_MSG_WM_SEARCH_WEB_SELTEXT(func)							\
	if (uMsg == WM_SEARCH_WEB_SELTEXT) {								\
		SetMsgHandled(TRUE);											\
		lResult = (LRESULT) func( (LPCTSTR) wParam, (LPCTSTR) lParam ); \
		if ( IsMsgHandled() )											\
			return lResult; 											\
	}

#define WM_REFRESH_EXPBAR		(WM_USER + 93)
#define USER_MSG_WM_REFRESH_EXPBAR(func)		  \
	if (uMsg == WM_REFRESH_EXPBAR) {			  \
		SetMsgHandled(TRUE);					  \
		lResult = (LRESULT) func( (int) wParam ); \
		if ( IsMsgHandled() )					  \
			return TRUE;						  \
	}

#define WM_SET_EXPROPERTY		(WM_USER + 94)
#define USER_MSG_WM_SET_EXPROPERTY(func)			  \
	if (uMsg == WM_SET_EXPROPERTY) {				  \
		SetMsgHandled(TRUE);						  \
		lResult = (LRESULT) func( (LPCTSTR) wParam ); \
		if ( IsMsgHandled() )						  \
			return TRUE;							  \
	}

#define WM_GET_OWNERDRAWMODE		(WM_USER + 95)
#define USER_MSG_WM_GET_OWNERDRAWMODE(func) \
	if (uMsg ==  WM_GET_OWNERDRAWMODE) {	\
		SetMsgHandled(TRUE);				\
		lResult = (LRESULT) func(); 		\
		if ( IsMsgHandled() )				\
			return TRUE;					\
	}




/// �g���v���p�e�B�̈����p
struct _EXPROP_ARGS {
	CString 	strUrl;
	DWORD		dwOpenFlag;
	CString 	strIniFile;
	CString 	strSection;
	CString 	strSearchWord;
};

#define WM_OPEN_WITHEXPROP		(WM_USER + 96)
#define USER_MSG_WM_OPEN_WITHEXPROP(func)					 \
	if (uMsg == WM_OPEN_WITHEXPROP) {						 \
		SetMsgHandled(TRUE);								 \
		lResult = (LRESULT) func( (_EXPROP_ARGS *) wParam ); \
		if ( IsMsgHandled() )								 \
			return TRUE;									 \
	}

#define WM_GET_SEARCHBAR		(WM_USER + 97)
#define USER_MSG_WM_GET_SEARCHBAR(func) \
	if (uMsg == WM_GET_SEARCHBAR) { 	\
		SetMsgHandled(TRUE);			\
		lResult = (LRESULT) func(); 	\
		if ( IsMsgHandled() )			\
			return TRUE;				\
	}

#define WM_SHOW_TOOLBARMENU 	(WM_USER + 98)
#define USER_MSG_WM_SHOW_TOOLBARMENU(func) \
	if (uMsg == WM_SHOW_TOOLBARMENU) {	   \
		SetMsgHandled(TRUE);			   \
		lResult = (LRESULT) func(); 	   \
		if ( IsMsgHandled() )			   \
			return TRUE;				   \
	}




//----------------------------------------------------------------------------
#define BEGIN_MSG_MAP_EX_decl(theClass) 				\
public:													\
	BOOL m_bMsgHandled; 								\
	/* "handled" management for cracked handlers */ 	\
	__inline BOOL IsMsgHandled() const {				\
		return m_bMsgHandled; 							\
	} 													\
	__inline void SetMsgHandled(BOOL bHandled) {		\
		m_bMsgHandled = bHandled; 						\
	} 													\
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0); \
	BOOL _ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID);



#define BEGIN_MSG_MAP_EX_impl(theClass) 				\
	BOOL theClass::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID) \
	{ 													\
		BOOL bOldMsgHandled = m_bMsgHandled; 			\
		BOOL bRet 			= _ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
		m_bMsgHandled 		= bOldMsgHandled;			\
		return bRet;									\
	} 													\
	BOOL theClass::_ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID) \
	{ 													\
		BOOL bHandled = TRUE; 							\
		hWnd; 											\
		uMsg; 											\
		wParam; 										\
		lParam; 										\
		lResult; 										\
		bHandled; 										\
		switch(dwMsgMapID) 								\
		{ 												\
		case 0:


#define BEGIN_DDX_MAP_decl(theClass)
#define END_DDX_MAP_decl(theClass)

#define GECKO_DLCTL_IMAGES  0x00000001
#define GECKO_DLCTL_FRAMES  0x00000002
#define GECKO_DLCTL_PLUGINS 0x00000004
#define GECKO_DLCTL_SCRIPTS 0x00000008
#define GECKO_DLCTL_REDIR   0x00000010
#define GECKO_DLCTL_DNS     0x00000020

#endif // __DONUTDEFINE_H__
