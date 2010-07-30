/**
 *	@file	appconst.cpp
 *	@brief	�A�v���P�[�V�����Ǝ��̃f�[�^���L�q����t�@�C��
 */
#include "stdafx.h"
#include "appconst.h"

namespace app {


#define VERSTR			_T("0.2.7")

#ifdef UNICODE
#define MBVER
#else
#define MBVER			_T("-mb")
#endif

#ifdef USE_DIET
#define DIETVER			_T("-DIET")
#else
#define DIETVER
#endif

/// �A�v���P�[�V������
const TCHAR *cnt_AppName = DONUT_NAME;

#if defined _WIN64
/// �A�v���P�[�V�����o�[�W����
const TCHAR *cnt_AppVersion = VERSTR DIETVER MBVER ;
#else
/// �A�v���P�[�V�����o�[�W����
const TCHAR *cnt_AppVersion = VERSTR DIETVER MBVER;
#endif



} //namespace app
