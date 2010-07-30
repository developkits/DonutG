// NemuBarForDonutP.cpp : DLL �G�N�X�|�[�g�̃C���v�������e�[�V����


// ����: Proxy/Stub ���
//  �ʁX�� proxy/stub DLL ���r���h���邽�߂ɂ́A�v���W�F�N�g�̃f�B���N�g���� 
//      nmake -f NemuBarForDonutPps.mak �����s���Ă��������B

#include "stdafx.h"
#include "resource.h"
#include "NemuBarForDonutP.h"
#include "NemuChecker.h"
#include "PluginInfo.h"

CComModule _Module;
CNemuToolBar g_NemuBar;

IUpdateChecker* CNemuToolBar::m_pChecker = NULL;
bool CNemuToolBar::m_bUpdate = false;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL �G���g�� �|�C���g
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();

 	CNemuToolBar::UninitNemuBar();
	return TRUE;    // ok
}

HWND WINAPI CreateToolBar(HWND hWndParent, UINT nID)
{
	CNemuToolBar::InitNemuBar();
    return g_NemuBar.Create(hWndParent, nID);
}

void WINAPI ShowToolBarMenu(UINT nID)
{
	g_NemuBar.ShowMenu( nID );
}

void WINAPI PluginSetting()
{
	//�ݒ��ʕ\��
	g_NemuBar.ShowSettingDlg();
}

void WINAPI GetPluginInfo(PLUGININFO* pstPluginInfo)
{
	memset( pstPluginInfo, 0, sizeof(PLUGININFO) );
	pstPluginInfo->type = PLT_TOOLBAR;

	strcpy( pstPluginInfo->name, "NemuBarForDonutP" );
	strcpy( pstPluginInfo->version, "2.0.0" );
	strcpy( pstPluginInfo->versionDate, "2004.05.15" );
	strcpy( pstPluginInfo->authorName, "�����݂�" );
	strcpy( pstPluginInfo->authorUrl, "http://homepage2.nifty.com/egamin/donutnemu.html#DonutP" );
	strcpy( pstPluginInfo->authorEmail, "---" );
	strcpy( pstPluginInfo->comment, "NemuBar�͉E�L������ http://www.aa.alpha-net.ne.jp/bosuke/" );
}
