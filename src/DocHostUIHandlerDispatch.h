/**
 *	@file	DocHostUIHandlerDispatch.h
 *	@brief	��ɉE�N���b�N���j���[�̃J�X�^���������s����悤�ɂ��邽�߂̃N���X
 */
#pragma once

#include <atlbase.h>
#include <atlapp.h>
#include <atlcom.h>
#include <mshtmdid.h>

#include "MtlDragDrop.h"
#include "CustomContextMenu.h"



/*
	CDocHostUIHandlerDispatch
	��ɉE�N���b�N���j���[�̃J�X�^���������s����悤�ɂ��邽�߂̃N���X
	IE�R���|�[�l���g��UI������u��������IDocHostUIHandlerDispatch����������
	�C���X�^���X���쐬���܂��B�z�X�g�E�B���h�E����N�G���[����IAxWinHostWindow��
	SetDefaultUIHandler�Őݒ肵�܂��B�����č��x�͋t�ɃC���X�^���X�̃|�C���^��
	IAxWinHostWindow::SetExternalUIHandler�Őݒ肵�܂��B

	���Ƃ��Ƃ�atlhost.h�𒼐ڏ���������atlhostEx.h���g���悤�ɂȂ��Ă����̂ł����A
	ATL7�ɂȂ��Ă���w�b�_�̈ˑ��֌W���ς�����̂��A���̕��@���g���ɂ����Ȃ������߁A
	���������Ă����������O���Ɉڂ�IDocHostUIHandlerDispatch�̎����Ƃ��܂����B

	���̃N���X�͖{���Ɋ����̏�����u�������Ă��܂��悤�ɒ�`����Ă��܂�(atlhost.h�Q��)
	���������ەK�v�Ȋ֐���ShowContextMenu�����ŁA����ȊO�̓f�t�H���g���삷��悤��
	�Ȃ��Ă��ė~�����̂ł����A�Ȃ��Ă��܂���B
	��̂̃��\�b�h�͓K���Ȗ߂�l��Ԃ��΃f�t�H���g�Ɠ�����������܂����A
	�����łȂ�����̃��\�b�h�ɑΉ����邽�߂Ƀf�t�H���g��UI�n���h�����������A
	�ꎞ�I�ɓ��N���X���������ăf�t�H���g������s�킹�܂�(GetHostInfo,GetOptionKeyPath���Q��)
 */
class CDocHostUIHandlerDispatch : public IDocHostUIHandlerDispatch {
private:
	bool					   m_bNoIECustom;
	CComPtr<IDocHostUIHandler> m_pDefaultHandler;
	_IDropTarget *			   m_pExternalDropTarget;

public: //CDocHostUIHandlerDispatch
	CDocHostUIHandlerDispatch()
		: m_bNoIECustom(false)
		, m_pExternalDropTarget(NULL)
	{
	}


	void SetIEContextMenuCustom(bool bCustom)
	{
		m_bNoIECustom = !bCustom;
	}


	bool GetIEContextMenuCustom()
	{
		return !m_bNoIECustom;
	}


	void SetDefaultUIHandler(const CComPtr<IDocHostUIHandler> &pDefaultHandler)
	{
		m_pDefaultHandler = pDefaultHandler;
	}


	bool InstallExternalUIHandler(IAxWinHostWindow *pAxWinHostWindow)
	{
		HRESULT hr = pAxWinHostWindow->SetExternalUIHandler(this);

		if ( FAILED(hr) )
			return false;

		return true;
	}


	bool UninstallExternalUIHandler(IAxWinHostWindow *pAxWinHostWindow)
	{
		HRESULT hr = pAxWinHostWindow->SetExternalUIHandler(NULL);

		if ( FAILED(hr) )
			return false;

		return true;
	}


	void SetExternalDropTarget(_IDropTarget *pExternalDropTarget)
	{
		m_pExternalDropTarget = pExternalDropTarget;
	}


public: 	//IUnknown atlhost.h���������K�v�ł͂Ȃ�

	STDMETHOD					(QueryInterface) (REFIID riid, void **ppvObject) { return E_NOINTERFACE; }
	ULONG STDMETHODCALLTYPE 	AddRef()	{ return 1; }
	ULONG STDMETHODCALLTYPE 	Release()	{ return 1; }


public: 	// IDispatch atlhost.h���������K�v�ł͂Ȃ�

	STDMETHOD	(GetTypeInfoCount) (UINT * pctinfo) { return E_NOTIMPL; }
	STDMETHOD	(GetTypeInfo) (UINT itinfo, LCID lcid, ITypeInfo * *pptinfo) { return E_NOTIMPL; }
	STDMETHOD	(GetIDsOfNames) (REFIID riid, LPOLESTR * rgszNames, UINT cNames, LCID lcid, DISPID * rgdispid)	{ return E_NOTIMPL; }

	STDMETHOD	(Invoke) (
			DISPID			dispidMember,
			REFIID			riid,
			LCID			lcid,
			WORD			/*wFlags*/,
			DISPPARAMS *	pdispparams,
			VARIANT *		pvarResult,
			EXCEPINFO * 	/*pexcepinfo*/,
			UINT *			/*puArgErr*/)
	{
		return E_NOTIMPL;
	}


public: 	// IDocHostUIHandlerDispatch
			//���̃C���^�[�t�F�C�X�͊����̓����u��������

	//�J�X�^���R���e�L�X�g���j���[ �{���ɕK�v�Ȃ̂͂��̊֐�����
	STDMETHOD(ShowContextMenu) (
		/* [in] */ DWORD				dwID,
		/* [in] */ DWORD				x,
		/* [in] */ DWORD				y,
		/* [in] */ IUnknown *			pcmdtReserved,
		/* [in] */ IDispatch *			pdispReserved,
		/* [retval][out] */ HRESULT *	dwRetVal)
	{
		if ( m_bNoIECustom && (GetKeyState(VK_LBUTTON) >= 0) )								//�J�X�^��&���N���b�N����Ă��邩�ǂ���
			return S_FALSE;
		*dwRetVal = CCustomContextMenu().Show(dwID, x, y, pcmdtReserved);					//�ۓ���
		return S_OK;
	}


	STDMETHOD(GetHostInfo) (
		/* [out][in] */ DWORD  * pdwFlags,
		/* [out][in] */ DWORD  * pdwDoubleClick)
	{
		//�ꎞ�I�ɊO��IDispatch�𖳌��ɂ��f�t�H���g���삳����
		CComQIPtr<IAxWinHostWindow> pHostWindow = m_pDefaultHandler;
		pHostWindow->SetExternalUIHandler(NULL);

		DOCHOSTUIINFO	info= { sizeof (DOCHOSTUIINFO) };
		info.dwFlags		= *pdwFlags;
		info.dwDoubleClick	= *pdwDoubleClick;
		HRESULT 		hr	= m_pDefaultHandler->GetHostInfo(&info);	//�f�t�H���g����

		//�O��IDispatch��L���ɂ���
		pHostWindow->SetExternalUIHandler(this);

		*pdwFlags		   = info.dwFlags;
		*pdwDoubleClick    = info.dwDoubleClick;
		return hr;
	}


	STDMETHOD(ShowUI) (
		/* [in] */ DWORD			 dwID,
		/* [in] */ IUnknown  *		 pActiveObject,
		/* [in] */ IUnknown  *		 pCommandTarget,
		/* [in] */ IUnknown  *		 pFrame,
		/* [in] */ IUnknown  *		 pDoc,
		/* [retval][out] */ HRESULT* dwRetVal)
	{
		return S_OK;
	}

	STDMETHOD(HideUI) (void) { return S_OK; }
	STDMETHOD(UpdateUI) (void) { return S_OK; }
	STDMETHOD(EnableModeless) (/* [in] */ VARIANT_BOOL fEnable) { return S_OK; }
	STDMETHOD(OnDocWindowActivate) ( /* [in] */ VARIANT_BOOL fActivate) { return S_OK; }
	STDMETHOD(OnFrameWindowActivate) ( /* [in] */ VARIANT_BOOL fActivate) { return S_OK; }


	STDMETHOD(ResizeBorder) (
		/* [in] */ long 			left,
		/* [in] */ long 			top,
		/* [in] */ long 			right,
		/* [in] */ long 			bottom,
		/* [in] */ IUnknown  *		pUIWindow,
		/* [in] */ VARIANT_BOOL 	fFrameWindow)
	{
		return S_OK;
	}


	STDMETHOD(TranslateAccelerator) (
		/* [in] */ DWORD_PTR		hWnd,
		/* [in] */ DWORD			nMessage,
		/* [in] */ DWORD_PTR		wParam,
		/* [in] */ DWORD_PTR		lParam,
		/* [in] */ BSTR 			bstrGuidCmdGroup,
		/* [in] */ DWORD			nCmdID,
		/* [retval][out] */HRESULT* dwRetVal)
	{
		return S_FALSE;
	}


	STDMETHOD(GetOptionKeyPath) (/* [out] */ BSTR  * pbstrKey, /* [in] */ DWORD dw)
	{
		CComQIPtr<IAxWinHostWindow> 	pHostWindow = m_pDefaultHandler;
		pHostWindow->SetExternalUIHandler(NULL);

		HRESULT hr	= m_pDefaultHandler->GetOptionKeyPath(pbstrKey, dw);

		pHostWindow->SetExternalUIHandler(this);

		return hr;
	}


	STDMETHOD(GetDropTarget) (/* [in] */ IUnknown  * pDropTarget, /* [out] */ IUnknown	* *ppDropTarget)
	{
		CComQIPtr<IAxWinHostWindow> pHostWindow = m_pDefaultHandler;
		pHostWindow->SetExternalUIHandler(NULL);

		if (m_pExternalDropTarget) {
			*ppDropTarget = (IUnknown *) m_pExternalDropTarget;
			pHostWindow->SetExternalUIHandler(this);
			return S_OK;
		}

		CComQIPtr<IDropTarget>		pTarget 	= pDropTarget;
		HRESULT 					hr			= m_pDefaultHandler->GetDropTarget( pTarget, reinterpret_cast<IDropTarget **>(ppDropTarget) );
		pHostWindow->SetExternalUIHandler(this);
		return hr;
	}


	STDMETHOD(GetExternal) ( /* [out] */ IDispatch	* *ppDispatch)
	{
		CComQIPtr<IAxWinHostWindow> pHostWindow = m_pDefaultHandler;
		pHostWindow->SetExternalUIHandler(NULL);

		HRESULT 	hr			= m_pDefaultHandler->GetExternal(ppDispatch);
		pHostWindow->SetExternalUIHandler(this);
		return hr;
	}


	STDMETHOD(TranslateUrl) (
		/* [in] */ DWORD		dwTranslate,
		/* [in] */ BSTR 		bstrURLIn,
		/* [out] */ BSTR  * 	pbstrURLOut)
	{
		return E_NOTIMPL;
	}


	STDMETHOD(FilterDataObject)(
		/* [in] */ IUnknown  *		pDO,
		/* [out] */ IUnknown  * *	ppDORet)
	{
		return E_NOTIMPL;
	}

};
