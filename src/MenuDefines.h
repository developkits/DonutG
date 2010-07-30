/**
 *	@file	MenuDefines.h
 *	@brief	WINVER ��0x500�����iOS2000�����j�ł̒�`����Ȃ����j���[�n��API���`
 */
#pragma once



//WINVER ��0x500�����iOS2000�����j�ł͒�`����Ȃ����j���[�n��API���`
#if (WINVER < 0x0500)

#define MNS_NOCHECK 			0x80000000
#define MNS_MODELESS			0x40000000
#define MNS_DRAGDROP			0x20000000
#define MNS_AUTODISMISS 		0x10000000
#define MNS_NOTIFYBYPOS 		0x08000000
#define MNS_CHECKORBMP			0x04000000

#define MIM_MAXHEIGHT			0x00000001
#define MIM_BACKGROUND			0x00000002
#define MIM_HELPID				0x00000004
#define MIM_MENUDATA			0x00000008
#define MIM_STYLE				0x00000010
#define MIM_APPLYTOSUBMENUS 	0x80000000



typedef struct tagMENUINFO {
	DWORD		cbSize;
	DWORD		fMask;
	DWORD		dwStyle;
	UINT		cyMax;
	HBRUSH		hbrBack;
	DWORD		dwContextHelpID;
	ULONG_PTR	dwMenuData;
}	MENUINFO, FAR *LPMENUINFO;


typedef MENUINFO CONST FAR	 *LPCMENUINFO;


/*
	WINUSERAPI
	BOOL
	WINAPI
	GetMenuInfo(
	IN HMENU,
	OUT LPMENUINFO);

	WINUSERAPI
	BOOL
	WINAPI
	SetMenuInfo(
	IN HMENU,
	IN LPCMENUINFO);

	WINUSERAPI
	BOOL
	WINAPI
	EndMenu(
		VOID);
 */



/*
 * WM_MENUDRAG return values.
 */
#define MND_CONTINUE	0
#define MND_ENDMENU 	1

typedef struct tagMENUGETOBJECTINFO {
	DWORD	dwFlags;
	UINT	uPos;
	HMENU	hmenu;
	PVOID	riid;
	PVOID	pvObj;
} MENUGETOBJECTINFO, *PMENUGETOBJECTINFO;



/*
 * MENUGETOBJECTINFO dwFlags values
 */
#define MNGOF_TOPGAP		   0x00000001
#define MNGOF_BOTTOMGAP 		0x00000002



/*
 * WM_MENUGETOBJECT return values
 */
#define MNGO_NOINTERFACE		0x00000000
#define MNGO_NOERROR			0x00000001

#define WM_MENUDRAG 			0x0123
#define WM_MENUGETOBJECT		0x0124



#endif


typedef BOOL (WINAPI* LPFNSETMENUINFO)(HMENU, LPCMENUINFO);

