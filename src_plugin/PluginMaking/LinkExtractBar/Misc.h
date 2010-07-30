/**
 *	@file	Misc.h
 *	@biref	�G���ȃ��[�`���Q
 */
#pragma once

#include <stdarg.h>


namespace Misc {


inline bool IsExistFile(LPCTSTR fname)
{
	return ::GetFileAttributes(fname) != 0xFFFFFFFF;
}


/// .exe�̂���f�B���N�g���̃t���p�X��Ԃ�. �Ō��'\\'���܂�.
//   �J�����g�f�B���N�g���̎擾.
inline const CString GetCurDirectory()
{
	TCHAR dir[MAX_PATH+2];
	dir[0] = 0;
	::GetCurrentDirectory(MAX_PATH, dir);
	return CString(dir);
}


//+++
inline int SetCurDirectory(const CString& strDir)
{
	return ::SetCurrentDirectory(LPCTSTR(strDir));
}



//+++ �t�@�C���p�X�����A�f�B���N�g�������擾. �Ō��'\\'�͊܂܂Ȃ�.
inline const CString	GetDirName(const CString& strFileName)
{
	int 	n	= strFileName.ReverseFind( _T('\\') );
	if (n < 0)
		return _T(".");	//strFileName;
	return	strFileName.Left(n);
}


///+++ exe(dll)�̃t���p�X����Ԃ�.
CString 	GetExeFileName();


///+++ exe(dll)�̂���t�H���_��Ԃ�. �Ō��'\\'���t���Ȃ�
inline CString GetExeDirName()
{
	CString str = GetExeFileName();
	int 	n	= str.ReverseFind( _T('\\') );
	return	str.Left(n);
}


//+++ �t�@�C���p�X�����A�t�@�C�������擾
inline const CString	GetFileBaseName(const CString& strFileName)
{
	int 	n	= strFileName.ReverseFind( _T('\\') );
	if (n < 0)
		return strFileName;
	return	strFileName.Mid(n + 1);
}


///+++ �t�@�C�����̊g���q�̎擾. �� ���ʂ̕�����ɂ�'.'�͊܂܂�Ȃ�.
inline const CString	GetFileExt(const CString& strFileName)
{
	const CString strBaseName = GetFileBaseName(strFileName);
	int 	n	= strBaseName.ReverseFind( _T('.') );
	if (n < 0)
		return CString();
	return strBaseName.Mid(n + 1);
}


///+++ �g���q�����̃t�@�C�����̎擾. �� ���ʂ̕�����ɂ�'.'�͊܂܂�Ȃ�.
inline const CString	GetFileNameNoExt(const CString& strFileName)
{
	const CString strBaseName = GetFileBaseName(strFileName);
	int 	n	= strBaseName.ReverseFind( _T('.') );
	if (n < 0)
		return strFileName;

	int 	nd	= strFileName.ReverseFind( _T('\\') );
	if (nd >= 0)
		return strFileName.Left(nd + 1) + strBaseName.Left(n);
	return strBaseName.Left(n);
}



inline const CString StrFmt(const char* fmt, ...)
{
	va_list app;
	va_start(app, fmt);
	CString 	str;
	str.FormatV(fmt, app);
	va_end(app);
	return str;
}



}	// namespace Misc
