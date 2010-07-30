/**
	@file	AmbientDispatch.h
	@brief	DLControl �𐧌䂷�邽�߂̊O�� AmbientDispatch�B
	@note

	IAmbientDispatch & CAmbientDispatch

	DLControl �𐧌䂷�邽�߂̊O�� AmbientDispatch�B
	�ȑO�� atlhost.h �𒼐ڏ��������Ď������Ă��܂������AATL�̃o�[�W�����A�b�v�ɂ��
	IAxWinAmbientDispatchEx ���p�ӂ��ꂽ�̂ł��̂悤�ȕ��@���Ƃ�Ȃ��Ă��ǂ��Ȃ�܂����B
	����͕W���ł͐��䂳��Ȃ� DLControl ���̃A���r�G���g�v���p�e�B�ƃ��\�b�h��
	��������N���X�ł��B

	�C���X�^���X���쐬������ SetHostWindow() �Ńz�X�g�E�B���h�E�̃|�C���^��
	�w�肵�Ă��������B���̌�z�X�g�E�B���h�E���� IAxWinAmbientDispatchEx ���N�G���[��
	SetAmbientDispatch() ���Ă�ŃC���X�^���X�̃|�C���^��ݒ肵�܂��B
	DLControl �̐ݒ�E�擾�� put/get_DLControlFlags ���Ăяo���Ă��������B
 */

#pragma once

#include <atlbase.h>
#include <atlapp.h>
#include <atlcom.h>
#include <mshtmdid.h>



class IAmbientDispatch : public IDispatch {
public:
	virtual HRESULT STDMETHODCALLTYPE	put_DLControlFlags(DWORD dwControlFlags)   = 0;
	virtual HRESULT STDMETHODCALLTYPE	get_DLControlFlags(DWORD *pdwControlFlags) = 0;
};



class CAmbientDispatch : public IAmbientDispatch {
private:	//protected:
	//�����o�ϐ�
	DWORD					  m_dwDLControlFlags;
	CComPtr<IAxWinHostWindow> m_pHost;


public: //CAmbientDispatch
	CAmbientDispatch()
		: m_dwDLControlFlags(DLCTL_DLIMAGES | DLCTL_VIDEOS | DLCTL_BGSOUNDS)	//+++
	{
	}


	void SetHostWindow(const CComPtr<IAxWinHostWindow> &pHost)
	{
		m_pHost = pHost;
	}


	void RefreshAmbient(DISPID dispid)
	{
		//CAxHostWindow��FireAmbientPropertyChange���Ăяo���������A
		//�|�C���^������ꂻ���ɂȂ��̂ŁAInvoke����ČĂяo���B
		//CAxHostWindow::Invoke������ΖړI���B������Ă���̂�������͂��B
		CComPtr<IDispatch> pDisp;
		HRESULT 		   hr = m_pHost->QueryInterface(&pDisp);
		if ( FAILED(hr) )
			return;
		VARIANT 		   varResult;
		DISPPARAMS		   params = { 0 };
		hr = pDisp->Invoke(dispid, IID_NULL, 1041 /*JP*/, DISPATCH_PROPERTYPUT, &params, &varResult, NULL, NULL);
	}


public: //IUnknown
	STDMETHOD					(QueryInterface)(REFIID riid, void **ppvObject) { return E_NOINTERFACE; }
	ULONG STDMETHODCALLTYPE 	AddRef()  { return 1; }
	ULONG STDMETHODCALLTYPE 	Release() { return 1; }


public: //IDispatch
	STDMETHOD	(GetTypeInfoCount)(UINT * pctinfo) { return E_NOTIMPL; }
	STDMETHOD	(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo * *pptinfo) { return E_NOTIMPL; }
	STDMETHOD	(GetIDsOfNames)(REFIID riid, LPOLESTR * rgszNames, UINT cNames, LCID lcid, DISPID * rgdispid) { return E_NOTIMPL; }

	STDMETHOD	(Invoke)(
			DISPID			dispidMember,
			REFIID			riid,
			LCID			lcid,
			WORD			wFlags,
			DISPPARAMS *	pdispparams,
			VARIANT *		pvarResult,
			EXCEPINFO * 	/*pexcepinfo*/,
			UINT *			/*puArgErr*/)
	{
		if (!pvarResult) {
			return E_INVALIDARG;
		}

		HRESULT hr = E_NOTIMPL;

		if (dispidMember == DISPID_AMBIENT_DLCONTROL) {
			pvarResult->vt	 = VT_I4;
			pvarResult->lVal = m_dwDLControlFlags;
			hr				 = S_OK;
		}

		return hr;
	}


	STDMETHOD	(put_DLControlFlags)(DWORD dwControlFlags)
	{
		m_dwDLControlFlags = dwControlFlags;
		RefreshAmbient(DISPID_AMBIENT_DLCONTROL);
		return S_OK;
	}


	STDMETHOD	(get_DLControlFlags)(DWORD * pdwControlFlags)
	{
		*pdwControlFlags = m_dwDLControlFlags;
		return S_OK;
	}

};
