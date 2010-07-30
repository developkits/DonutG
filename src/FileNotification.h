/**
 *	@file FileNotification.h
 *	@brief	�t�@�C���̍X�V�̊Ď�. (�����N�o�[��p?)
 */

#pragma once



class CFileNotification {
public:
	// Declarations
	DECLARE_REGISTERED_MESSAGE( Mtl_FileNotification )

private:
	struct _ThreadParam {
		HWND	_hWnd;
		HANDLE	_hExitEvent;
		HANDLE	_hNotification;

		_ThreadParam() : _hWnd(0), _hExitEvent(0), _hNotification(0) {} 	//+++
	};

public:
	// Ctor/Dtor
	CFileNotification()
		: m_hNotificationThread( NULL )
		, m_dwNotificationThreadID( 0 )
		, m_ThreadParams()					//+++ �����`�F�b�N�΍�.
	{
	}


	~CFileNotification()
	{
		if (m_hNotificationThread != NULL)
			_CleanUpNotification();
	}


private:
	// Data members
	HANDLE			m_hNotificationThread;
	DWORD			m_dwNotificationThreadID;
	_ThreadParam	m_ThreadParams;

public:
	bool SetUpFileNotificationThread(HWND hWnd, const CString &strDirPath, bool bWatchSubTree = false)
	{
		m_ThreadParams._hExitEvent	  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		ATLASSERT(m_ThreadParams._hExitEvent != INVALID_HANDLE_VALUE);

		ATLASSERT( ::IsWindow(hWnd) );
		m_ThreadParams._hWnd		  = hWnd;

		CString 		strPath(strDirPath);
		MtlRemoveTrailingBackSlash(strPath);

		HANDLE	hWait = ::FindFirstChangeNotification(  strPath,
														bWatchSubTree,													// flag for monitoring
														FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);	// fixed by INUYA, thank you.

		// ���C�ɓ���̃����N�t�H���_��������Ȃ�������false��Ԃ�
		if (hWait == INVALID_HANDLE_VALUE || hWait == NULL) 				// can't find the Link directory
			return false;

		m_ThreadParams._hNotification = hWait;
		m_hNotificationThread		  = ::CreateThread(NULL, 0, _FileNotificationThread, (LPVOID) &m_ThreadParams, 0, &m_dwNotificationThreadID);

		ATLASSERT(m_hNotificationThread != INVALID_HANDLE_VALUE);
		return true;
	}


	void _CleanUpNotification()
	{
		MTLVERIFY( ::SetEvent(m_ThreadParams._hExitEvent) );

		DWORD dwResult = ::WaitForSingleObject(m_hNotificationThread, DONUT_THREADWAIT);

		if (dwResult == WAIT_OBJECT_0) {
			// wait the thread over
		}

		::FindCloseChangeNotification(m_ThreadParams._hNotification);
		::CloseHandle(m_ThreadParams._hExitEvent);
		::CloseHandle(m_hNotificationThread);								// fixed by DGSTR, thanks!
	}


	static DWORD WINAPI _FileNotificationThread(LPVOID lpParam)
	{
		_ThreadParam*	pParam	= (_ThreadParam *) lpParam;

		ATLASSERT( ::IsWindow(pParam->_hWnd) );

		HANDLE			handles[] = { pParam->_hExitEvent, pParam->_hNotification };

		for (;;) {
			DWORD dwResult = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);

			if (dwResult == WAIT_OBJECT_0) {								// killevent
				break;														// thread must be ended
			} else if (dwResult == WAIT_OBJECT_0 + 1) { 					//notification
				::PostMessage(pParam->_hWnd, GET_REGISTERED_MESSAGE(Mtl_FileNotification), 0, 0);
				::FindNextChangeNotification(pParam->_hNotification);
			} else if (dwResult == WAIT_FAILED) {
				ATLASSERT(FALSE);
				break;
			} else {
				ATLASSERT(FALSE);
				break;
			}
		}

		return 0;
	}
};
