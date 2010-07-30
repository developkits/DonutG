/**
 *	@file	stdafx.h
 *	@brief	include file for standard system include files,
 *			 or project specific include files that are used frequently, but
 *			are changed infrequently
 */

#if !defined (AFX_STDAFX_H__19D42987_EAF8_11D3_BD32_96A992FCCD39__INCLUDED_)
#define 	  AFX_STDAFX_H__19D42987_EAF8_11D3_BD32_96A992FCCD39__INCLUDED_

#define COMMENT 0

#if 1	//+++	�f�o�b�O�p
 #define USE_ZEROFILL_NEW				//+++ �蔲���� 0 �N���A������ new ���g��. �܂��͂����Ȃ��ق����悳����...
 //#define USE_DIET
 //#define USE_DLMALLOC
 //#define USE_ORG_UNDONUT_INI			//+++ unDonut+ ����ς���Ă��܂��� .ini ��g���v���p�e�B�̒l���Ȃ�ׂ��A�I���W�i����unDonut�ɂ��킹��ꍇ�ɒ�`.
 //#define USE_MEMORYLOG 				//+++ donut�ł�new,delete���O����.
 //#define USE_ATL3_BASE_HOSTEX			//+++ about:blank����݂̃o�O�̃f�o�b�O�ŗp��. �˂��~�߂��̂�atl3�p�ȊO�Œ�`����K�v�Ȃ�.
 //x #define USE_UNDONUT_G_SRC 			//+++ gae����unDonut_g 2006-08-05 �̌��J�\�[�X���ڐA����������L���ɂ��Ă݂�.(������) ...�f�t�H���g�Ŕ��f���Ƃ��̂Ń��x���͔j��.
 #ifndef NDEBUG
  //#define _CRTDBG_MAP_ALLOC 			//+++ ��`�����VC���C�u�����ɂ��malloc�n�̃`�F�b�N����...
  //#define USE_ATLDBGMEM				//+++ atldbgmem.h ���g���ꍇ... ��include,�}�N���̈ˑ��֌W�̓s���A����Aregex�͎g�p�ł��Ȃ����.
 #endif
#endif

// Change these values to use different versions
#ifdef WIN64	//+++ 64�r�b�g��win �� winXp64�ȍ~�݂̂ɑΉ�.
#define WINVER					0x0501
#define _WIN32_WINNT			0x0501
#define _WIN32_IE				0x0600					//+++ _WIN32_IE_IE60SP2
#define _RICHEDIT_VER			0x0100					//+++ 0x200�ȏ�(3?)�ŏ\�������A�Ȃ�ƂȂ�
#define DONUT_NAME				_T("64unDonut")
#define DONUT_WND_CLASS_NAME	_T("WTL:") DONUT_NAME	//+++ ���O������ƃv���O�C���Ƃ��X�N���v�g�Ń}�Y�C?
#else		   //+++ �ꉞ�Awin9x�͈̔�
#define WINVER					0x0400
#define _WIN32_WINNT			0x0400
#define _WIN32_IE				0x0501
#define _RICHEDIT_VER			0x0100
#define DONUT_NAME				_T("DonutG")
#define DONUT_WND_CLASS_NAME	_T("WTL:Donut") 		//+++ ���O������ƃv���O�C���Ƃ��X�N���v�g�Ń}�Y�C?
#endif

#define ATL_TRACE_CATEGORY		0x00000001
#define ATL_TRACE_LEVEL 		4
#define _WTL_USE_CSTRING
#define _WTL_FORWARD_DECLARE_CSTRING
#define _CRT_NON_CONFORMING_SWPRINTFS
//#define _ATL_FREE_THREADED
//#define _ATL_SINGLE_THREADED
#define _ATL_APARTMENT_THREADED

#if _ATL_VER < 0x800
#define ATLASSUME(e)			ATLASSERT(e)
#endif


// unDonut �� unDonut+(mod) �Ƃ̔�݊������̐؂�ւ�
#ifdef USE_ORG_UNDONUT_INI								//+++ unDonut r13test�̋L�q.
#define STR_ADDRESS_BAR 		_T("AddresBar")
#define STR_ENABLE				_T("Enabel")
#else													//+++ unDonut+ ���ύX(typo�C�����ꂽ)
#define STR_ADDRESS_BAR 		_T("AddressBar")
#define STR_ENABLE				_T("Enable")
#endif

#ifdef USE_DIET	// �_�C�G�b�g��
#define NO_STYLESHEET
#endif

//Win32API
#include <windows.h>

#ifdef USE_DLMALLOC	//+++ new,delete��dlmalloc��p���Ă݂�. 
 #undef 	_CRTDBG_MAP_ALLOC	// c�����^�C���ȃf�o�b�O�֌W�͎g���Ȃ�
 #undef 	USE_ATLDBGMEM		// atl�̃������f�o�b�O�֌W�͎g���Ȃ�...
 #define 	USE_DL_PREFIX		// �{����malloc,free�̒u���͑�ςȂ̂ŁAdlmalloc���̂܂܎g��.
 #include 	"dlmalloc.h"
  #if 0 //def USE_DLMALLOC
   #define 	malloc 		dlmalloc
   #define 	calloc 		dlcalloc
   #define 	realloc 	dlrealloc
   #define 	free 		dlfree
 #endif
#endif

#if 1	//+++ �������[�{�f�o�b�O�̒��날�킹��
#include <new>
#if defined NDEBUG == 0 && defined USE_ATL3_BASE_HOSTEX == 0
#if defined USE_ATLDBGMEM
//#define _ATL_NO_TRACK_HEAP
#include <atldbgmem.h>
#endif
#ifdef _CRTDBG_MAP_ALLOC
#include <malloc.h>
#include <crtdbg.h>
#endif

#pragma push_macro("new")
#undef  new
#include <xdebug>
#include <xmemory>
#include <xlocale>
#pragma pop_macro("new")
#endif
#endif


//C Standard Library
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#include <string.h>
#include <locale.h>
#include <process.h>
#include <ctype.h>
#include <stdarg.h>
#include <tchar.h>
#include <time.h>
//#include <io.h>

//STL(C++ Standard Library)
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
#include <utility>
#include <iterator>
#include <map>


#define DBG_M() 		ErrorLogPrintf(_T("%s (%d)\n"), __FILE__, __LINE__)

#ifdef USE_DLMALLOC
#define _recalloc		recalloc
inline void* recalloc(void* p, size_t a, size_t b) { void* q = realloc(p,a*b); if (q) memset(q, 0, a*b); return q; }
#endif

//ATL/WTL
#include <atlbase.h>
#include <atlapp.h>

extern CServerAppModule _Module;						//�A�v���P�[�V�����C���X�^���X
extern TCHAR			g_szIniFileName[MAX_PATH];		//�ݒ�t�@�C��

#include <atlcom.h>


#if _ATL_VER < 0x700 && defined(USE_ATL3_BASE_HOSTEX) == 0
#define USE_ATL3_BASE_HOSTEX
#endif

#if defined USE_ATL3_BASE_HOSTEX == 0	/*_ATL_VER >= 0x700*/	//+++
//#include "AtlifaceEx.h"	//+++ ���łɕs�v�̂悤?.
//#include "AtlifaceEx_i.c" //+++ ���łɕs�v�̂悤?.
//#include <AtlHost.h>
#include "AtlHostEx.h"
#else
#endif

#include <atlwin.h>
#include <atlctl.h>
#include <atlmisc.h>
#include <atlframe.h>

#include "WtlFixed/atlsplit.h"
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>

#include <atldlgs.h>
#include <atlctrlx.h>
#include <atlcrack.h>
#include <atlddx.h>
#include <atldef.h>
//#include <atlsync.h>

// etc
#include <winerror.h>
#include <winnls32.h>
#include <comdef.h>
//#include <exdisp.h>
//#include <guiddef.h>
//#include <olectl.h>
//#include <rpc.h>
//#include <rpcndr.h>
//#include <rpcproxy.h>
//#include <urlmon.h>

#ifndef  WM_THEMECHANGED
 #define WM_THEMECHANGED	0x031A
#endif
//��XP�ł����삷��悤�ɓ��I�����N����悤�ɂ��������Ńw�b�_
#if _MSC_VER >= 1500	//+++ ����:undonut�Ŏg��WTL80�������������̂ł�������g���Ă�ok.
						//+++ �����A�w�b�_��������ĂȂ��ƃ_���Ȃ悤�Ȃ�ŁA�蔲���ŃR���p�C���o�[�W�����Ő؂�ւ�
#include <atltheme.h>
#else					//+++ �Â��R���p�C���p...�����A�������̂ق����T�C�Y�������Ȃ邩��...
#include "WtlFixed/atltheme_d.h"
#endif

//+++ Aero ���g���Ă݂�e�X�g.
#ifdef USE_AERO
#include <atldwm.h>
#endif

//IE�R���|�[�l���g�Ŏg����`
#include <shlobj.h>
#include <wininet.h>
#include <shlwapi.h>
//#include <shlguid.h>
#include <intshcut.h>
#include <MsHTML.h>
#include <mshtmdid.h>
#include <mshtmcid.h>
#include <MsHtmHst.h>
#include <mshtml.h>
#include <tlogstg.h>
#include <urlhist.h>

//Gecko
#include "nsStringAPI.h"
#include "nsEmbedString.h"

#include "nsCOMPtr.h"
#include "nsComponentManagerUtils.h"
#include "nsServiceManagerUtils.h"
#include "nsIInterfaceRequestor.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsWeakReference.h"

#include "nsIWebBrowser.h"
#include "nsIWebNavigation.h"
#include "nsIDocShell.h"
#include "nsIDOMWindow.h"
#include "nsIDOMDocument.h"
#include "nsIDOMElement.h"
#include "nsIDOMNode.h"
#include "nsIDOMEvent.h"
#include "nsIDOMMouseEvent.h"
#include "nsIDOMKeyEvent.h"
#include "nsIDOMEventTarget.h"
#include "nsIDOMEventListener.h"
#include "nsISelection.h"
#include "nsIDOMRange.h"
#include "nsIURI.h"
#include "nsNetUtil.h"
#include "nsIPrefBranch.h"
#include "nsISupportsPrimitives.h"
#include "nsIProperties.h"
#include "nsIWebBrowserPersist.h"
#include "nsIIOService.h"
#include "nsIDocShell.h"
#include "nsIInputStream.h"
#include "nsIStringStream.h"
#include "nsIWebBrowserFocus.h"

#ifndef USE_DIET	//XML�p
//+++ �g���̂�MSXML2�͈̔�?�̂悤�����Axp64,vista64 �ɂ� msxml3.dll���Ńf�t�H�œ����Ă���悤�Ȃ̂ŁA3�ɂ��Ă݂�.
//#import "msxml4.dll" named_guids	//raw_interfaces_only
#import "msxml3.dll" named_guids	//raw_interfaces_only
using namespace MSXML2;
#endif

//+++ �蔲���� 0 �N���A������ new ��p��.
//���������[�N�m�F�p��new/delete�I�[�o�[���[�h
#if defined USE_ATLDBGMEM
#define DEBUG_NEW	new(__FILE__, __LINE__)
#elif (defined USE_ZEROFILL_NEW) || (defined USE_MEMORYLOG && defined _DEBUG)
void *operator	new(size_t t);
void *operator	new[] (size_t t);
void operator	delete(void *p);
void operator	delete[] (void *p);
//#undef USE_MEMORYLOG
#endif

#if 0 //defined _DEBUG && defined _CRTDBG_MAP_ALLOC
void* operator	new(size_t sz, const char* fname, unsigned line);
void* operator	new[](size_t sz, const char* fname, unsigned line);
void  operator	delete(void* p, const char* fname, unsigned line);
void  operator	delete[](void* p, const char* fname, unsigned line);
#endif

#include "dbg_wm.h"


#include "DonutDefine.h"
#include "Misc.h"
#include "DonutPFunc.h"


#ifdef USE_ATL3_BASE_HOSTEX/*_ATL_VER < 0x700*/ //+++
#include "for_atl3/AtlifaceEx.h"
#include "for_atl3/AtlHostEx_for_atl3.h"
#endif


#undef min
#undef max

#if _ATL_VER < 0x700
namespace std {
template<typename T> inline const T min(const T& a, const T& b) { return a < b; }
template<typename T> inline const T max(const T& a, const T& b) { return b < a; }
}
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
#endif	// !defined(AFX_STDAFX_H__19D42987_EAF8_11D3_BD32_96A992FCCD39__INCLUDED_)
