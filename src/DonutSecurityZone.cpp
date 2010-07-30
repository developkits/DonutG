/**
 *	@file	DonutSecurityZone.cpp
 *	@brief	�N�b�L�[�̈����Ɋւ���R�}���h���b�Z�[�W���n���h�����O����N���X
 */
#include "stdafx.h"
#include "DonutSecurityZone.h"


#if defined USE_ATLDBGMEM
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDonutSecurityZone::OnCookieAllowAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	SetGlobalCookiePref(0);
}

void CDonutSecurityZone::OnCookieAllowSameOrigin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	SetGlobalCookiePref(1);
}

void CDonutSecurityZone::OnCookieDenyAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	SetGlobalCookiePref(2);
}

void CDonutSecurityZone::OnCookieCustom(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
{
	SetGlobalCookiePref(3);
}