/**
 *	@file	PropertySheet.h
 *	@brief	�c���[�r���[�`���ŃI�v�V�������Ǘ����邽�߂̃_�C�A���O
 *			���\�[�X�e���v���[�g�Ɉˑ��������������邽�ߔėp���͔��ɒႭ�Ȃ��Ă��܂��B
 */

#ifndef __PROPERTY_SHEET_H_
#define __PROPERTY_SHEET_H_


class CTreeViewPropertySheet : public CDialogImpl<CTreeViewPropertySheet> {
public:
	enum {
		IDD 		= IDD_PROPSHEET,
	};

private:
	enum {
		TITLELENGTH = 200,
		SPACE		= 2,
	};

	struct PAGEINFO {
		PROPSHEETPAGE	psp;
	  #if 1	//+++ UNICODE�Ή�
		TCHAR			title[TITLELENGTH];
	  #else
		char			title[TITLELENGTH];
	  #endif
		HWND			hWnd;
		int 			nIndent;
	};


	CSimpleArray<CToolTipCtrl>		m_aryToolTip;
	CSimpleArray<PAGEINFO *>		m_aryPage;
	CString 						m_Title;
	CTreeViewCtrl					m_TreeView;
	HWND							m_hActiveWnd;

	int 							m_nWidth;
	int 							m_nHeight;
	bool							m_bInitPos;

	static HHOOK					s_hHook;
	static CTreeViewPropertySheet*	s_pThis;
	static int						m_nCurSel;


public:
	CTreeViewPropertySheet(LPCTSTR title = NULL)
	{
		m_Title    = title;
		m_nCurSel  = 0;
		m_bInitPos = false;
	  #if 1 //+++
		m_nWidth	= 0;
		m_nHeight	= 0;
		m_hActiveWnd= 0;
		m_bInitPos	= 0;
	  #endif
	}


	BOOL AddPage(PROPSHEETPAGE *pPage, int nIndent = -1)
	{
		ATLASSERT(pPage != NULL);

		PAGEINFO *ppi = new PAGEINFO;
		ppi->psp	 = *pPage;
		ppi->hWnd	 = NULL;
		ppi->nIndent = nIndent;
		m_aryPage.Add(ppi);
		return TRUE;
	}


	INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL)
	{
		if (m_aryPage.GetSize() == 0)
			return 0;

		//�L�[�A�N�Z�����[�^�p�Ƀt�b�N������
		s_pThis = this;

	  #ifndef NDEBUG	//+++ �ǉ�.
		SetLastError(0);
	  #endif

		s_hHook = ::SetWindowsHookEx( WH_MSGFILTER, MsgFilterProc, _Module.GetResourceInstance(), ::GetCurrentThreadId() );

	  #ifndef NDEBUG	//+++ ����: unDonut+4 �Œǉ����ꂽ�`�F�b�N. �Ȃ�������?�Ȋ��ł��ꍇ�ɂ���ẮA
						//+++ "�w�肳�ꂽ���\�[�X�����C���[�W�t�@�C���Ɍ�����܂���B"
						//+++ "���̃V�X�e���ł͂��̊֐��̓T�|�[�g����܂���B"
						//+++ �Ƃ����G���[���o��炵��...�̂ŁA�����ł̕\���͖����ɂ���.
		// �I�v�V������\������Ɨ�����炵���̂ŃG���[��\�������Ă݂�
		DWORD dw = GetLastError();
		if (dw != ERROR_SUCCESS) {
			LPTSTR lpBuffer;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, dw, LANG_USER_DEFAULT,
				// MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
				(LPTSTR) &lpBuffer, 0, NULL );
			::MessageBox(NULL, lpBuffer, _T("error message"), MB_ICONHAND | MB_OK);
		}
	  #endif

		return CDialogImpl<CTreeViewPropertySheet>::DoModal(hWndParent, dwInitParam);
	}


private:
	static LRESULT CALLBACK MsgFilterProc(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code >= 0) {
			MSG *pMsg = (MSG *) lParam;

			if ( (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
			   || (WM_MOUSEFIRST <= pMsg->message && pMsg->message <= WM_MOUSELAST) )
			{
				HWND	  hWnd = s_pThis->_GetActivePage();

				if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB) {
					HWND hWndFocus = ::GetFocus();
					HWND hWndDlg   = s_pThis->m_hActiveWnd;
					BOOL bPrevious = (::GetAsyncKeyState(VK_SHIFT) & 0x80000000) != 0;

					HWND hWndNext  = ::GetNextDlgTabItem(hWndDlg, hWndFocus, bPrevious);
					if (!hWndNext)
						hWndNext = ::GetNextDlgTabItem(hWndDlg, NULL, bPrevious);

				  #if 1 //+++	������ςȂ�SHIFT+TAB�ňړ����悤�Ƃ���ƁA���C���E�B���h�E�Ƀt�H�[�J�X���ڂ��Ă��܂����������Ώ�.
						//+++	��	...�Ƃ������Adonut�̃I�v�V�������Ă񂾂Ƃ��̃J�[�\���ʒu��"�S��"�̈ʒu�ɂȂ��Ă��Ȃ����Ƃ���肩��..
					if (!hWndNext && bPrevious) {
						hWndNext  = ::GetNextDlgTabItem(hWndDlg, NULL, 0);				//+++ �܂��擪��
						if (hWndNext) {
							::SetFocus(hWndNext);										//+++ �t�H�[�J�X���ڂ��Ă���
							if (hWndNext) {
								hWndNext  = ::GetNextDlgTabItem(hWndDlg, hWndNext, 1);	//+++ ��߂�
							}
						}
					}
					if (hWndNext) {
						::SetFocus(hWndNext);
						return 1;
					}
				  #else
					::SetFocus(hWndNext);
					return 1;
				  #endif
				}

				//*+++ hWnd�̎擾�Ɏ��s�����Ԃ�����̂ŁA�b��Ώ��Ń`�F�b�N
				if (hWnd) {
					PSHNOTIFY	pn	= {0};
					pn.hdr.code 	= PSN_TRANSLATEACCELERATOR;
					pn.hdr.hwndFrom = s_pThis->m_hWnd;
					pn.hdr.idFrom	= 0;
					pn.lParam		= lParam;
					LRESULT   nRet = ::SendMessage(hWnd, WM_NOTIFY, (WPARAM) 0, (LPARAM) &pn);
					if (nRet)
						return nRet;
				}
			}
		}

		return ::CallNextHookEx(s_hHook, code, wParam, lParam);
	}


public:
	BEGIN_MSG_MAP(CTreeViewPropertySheet)
		MESSAGE_HANDLER( WM_INITDIALOG , OnInitDialog)
		MESSAGE_HANDLER( WM_DESTROY    , OnDestroy	 )
		MESSAGE_HANDLER( WM_SIZE	   , OnSize 	 )
		COMMAND_ID_HANDLER( IDOK	   , OnOk			)
		COMMAND_ID_HANDLER( IDCANCEL   , OnCancel		)
		COMMAND_ID_HANDLER( IDC_APPLY  , OnApply		)
	  #ifndef USE_DIET
		COMMAND_ID_HANDLER( IDC_TOOLTIP, OnCheckToolTip )
	  #endif
		//NOTIFY_HANDLER(IDC_TREEVIEW,TVN_SELCHANGED,OnSelChanged)
		NOTIFY_HANDLER(IDC_TREEVIEW, TVN_SELCHANGING, OnSelChanging)
		//+++ ������...���s... CHAIN_MSG_MAP(CDialogImpl<CTreeViewPropertySheet>)
	END_MSG_MAP()


private:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		//�_�C�A���O�e���v���[�g��������������
		_ReadyDialogResources();

		//�c���[�r���[����������
		_InitTreeView();

	  #ifndef USE_DIET
		//�c�[���`�b�v�\���̉ۂ��擾
		CIniFileI	pr( g_szIniFileName, _T("Option" ) );
		DWORD		dwVal = pr.GetValue(_T("ToolTip"), 1/*0*/ );	//+++ �f�t�H���g��on�ɕύX.
		pr.Close();
		CheckDlgButton(IDC_TOOLTIP, dwVal);
	  #endif

		//�ŏ��̃y�[�W��\��
		m_hActiveWnd = _LoadDialog(0);
		_BeChild(m_hActiveWnd, m_hWnd);
		::ShowWindow(m_hActiveWnd, SW_SHOW);
		NotifyToDialog(m_hActiveWnd, PSN_SETACTIVE);
		m_TreeView.Select(m_TreeView.GetFirstVisibleItem(), TVGN_CARET);

		MoveWindow(0, 0, m_nWidth + 180, m_nHeight + 70, TRUE);
		_SetTitle(0);

		::SetFocus(m_hActiveWnd);				//+++	�J�[�\���ʒu�𖳗����ݒ肷��...

		return 0;
	}


	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
	  #ifndef USE_DIET
		//OK�L�����Z���Ɋւ�炸�`�F�b�N��Ԃ�ۑ�
		DWORD		dwVal = (IsDlgButtonChecked(IDC_TOOLTIP) == BST_CHECKED);
		CIniFileO	pr( g_szIniFileName, _T("Option") );
		pr.SetValue( dwVal, _T("ToolTip") );
		pr.Close();
	  #endif
		::UnhookWindowsHookEx(s_hHook); //�t�b�N����
		_ReleasePages();
		return 0;
	}


	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if (!m_bInitPos) {
			CenterWindow( GetParent() );
			m_bInitPos = true;
		}

		CRect rc, rcb;
		GetClientRect(&rc);

		rc.right  -= SPACE;
		rc.left    = rc.right - m_nWidth;
		rc.top	   = SPACE * 5;
		rc.bottom  = m_nHeight + rc.top;
		CWindow(m_hActiveWnd).MoveWindow(&rc , TRUE);

		rc.right   = rc.left - SPACE;
		rc.left    = SPACE * 4;
		rc.bottom += 20;
		m_TreeView.MoveWindow(&rc, TRUE);

		GetClientRect(&rc);
		CWindow( GetDlgItem(IDC_APPLY) ).GetClientRect(&rcb);
		rc.right  -= SPACE * 10;
		rc.left    = rc.right - rcb.Width();
		rc.bottom -= SPACE * 3;
		rc.top	   = rc.bottom - rcb.Height();
		CWindow( GetDlgItem(IDC_APPLY) ).MoveWindow(&rc);
		rc.right   = rc.left - SPACE * 3;
		rc.left   -= rcb.Width();
		CWindow( GetDlgItem(IDCANCEL) ).MoveWindow(&rc);
		rc.right   = rc.left - SPACE * 3;
		rc.left   -= rcb.Width();
		CWindow( GetDlgItem(IDOK) ).MoveWindow(&rc);

		CWindow tooltip = GetDlgItem(IDC_TOOLTIP);
		CRect	rcc;
		tooltip.GetClientRect(rcc);
		rc.right   = rc.left - SPACE * 3;
		rc.left   -= rcc.Width() + 24;
		tooltip.MoveWindow(&rc);

		return S_OK;
	}


	LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
	{
		if ( !_ApplyToDialogs() )
			return 0;

		EndDialog(wID);
		return 0;
	}


	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}


	LRESULT OnApply(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
	{
		_ApplyToDialogs();
		return 0;
	}


  #ifndef USE_DIET
	LRESULT OnCheckToolTip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL & /*bHandled*/)
	{
		_ShowToolTip();
		return 0;
	}
  #endif

	BOOL _ApplyToDialogs()
	{
		int 		count	= m_aryPage.GetSize();
		PAGEINFO*	ppi 	= m_aryPage[m_nCurSel];

		//�܂��J�����g�^�u�ɕύX��ʒm����DDX�ȂǂɈ���������Ȃ����m�F
		//�J�����g�^�u�ȊO��DDX_INT_RANGE�ȂǂɈ��������邱�Ƃ͂Ȃ��̂ł܂Ƃ߂�for�ŏ���
		if ( ::IsWindow(ppi->hWnd) )
			if (NotifyToDialog(ppi->hWnd, PSN_APPLY) != FALSE)
				return FALSE;

		for (int i = 0; i < count; i++) {
			if (m_nCurSel == i) continue;

			PAGEINFO *ppi = m_aryPage[i];

			if ( ::IsWindow(ppi->hWnd) )
				if (NotifyToDialog(ppi->hWnd, PSN_APPLY) != FALSE)
					return FALSE;

		}

		return TRUE;
	}


	LRESULT OnSelChanging(int idCtrl, LPNMHDR pnmh, BOOL &bHandled)
	{
		NMTREEVIEW *ptv 	 = (NMTREEVIEW *) pnmh;
		int 		newindex = (int) ptv->itemNew.lParam;
		int 		oldindex = (int) ptv->itemOld.lParam;

		if (newindex == oldindex) return 0;

		HWND		hOldWnd  = m_aryPage[oldindex]->hWnd;
		HWND		hNewWnd  = m_aryPage[newindex]->hWnd;

		if ( ::IsWindow(hOldWnd) ) {
			//�Â��͉̂B��
			BOOL	bRet = NotifyToDialog(hOldWnd, PSN_KILLACTIVE);
			if (bRet != FALSE) {
				return 1; //�L�����Z�����ꂽ
			}

			::ShowWindow(hOldWnd, SW_HIDE);
		}

		if (hNewWnd == NULL) {
			//�V�����͕̂K�v�Ȃ�쐬����
			m_aryPage[newindex]->hWnd = _LoadDialog(newindex);
			_BeChild(m_aryPage[newindex]->hWnd, m_hWnd);
		}

		m_hActiveWnd = m_aryPage[newindex]->hWnd;
		_MoveChild(m_hActiveWnd);
		::ShowWindow(m_hActiveWnd, SW_SHOW);

		NotifyToDialog(m_hActiveWnd, PSN_SETACTIVE);
		_SetTitle(newindex);

		::SetFocus(m_hActiveWnd);
		m_nCurSel	 = newindex;

		return 0;
	}


	void _ReadyDialogResources()
	{
		int   max_w, max_h;
		int   count = m_aryPage.GetSize();

		max_w = max_h = 0;

		for (int i = 0; i < count; i++) {
			PAGEINFO *ppi  = m_aryPage[i];

			HRSRC	  hRes =  ::FindResource(_Module.GetResourceInstance(), ppi->psp.pszTemplate, RT_DIALOG);
			if (hRes) {
				HGLOBAL hMem = ::LoadResource(_Module.GetResourceInstance(), hRes);
				if (hMem) {
					LPBYTE pMem = (LPBYTE) ::LockResource(hMem);
					if (pMem) {
						//�_�C�A���O�̃T�C�Y���擾
						WCHAR *wpstr = (WCHAR *) pMem;
						if ( (*wpstr == 1) && (*(wpstr + 1) == 0xFFFF) ) {
							//�g���e���v���[�g
							DLGTEMPLATEEX_HALF *dpe = (DLGTEMPLATEEX_HALF *) pMem;

							if (max_w < dpe->cx)  max_w = dpe->cx;
							if (max_h < dpe->cy)  max_h = dpe->cy;

							wpstr += 13;

						} else {
							//���ʂ̃e���v���[�g
							DLGTEMPLATE *dp   = (DLGTEMPLATE *) pMem;

							if (max_w < dp->cx)  max_w = dp->cx;
							if (max_h < dp->cy)  max_h = dp->cy;

							char *		 pstr = (char *) ( pMem + sizeof (DLGTEMPLATE) );
							wpstr = (WCHAR *) pstr;
						}

						for (int j = 0; j < 2; j++) { //���j���[��N���X�Ȃǂ̏���ǂݔ�΂�
							if (*wpstr == 0)			wpstr++;
							else if (*wpstr == 0xFFFF)	wpstr += 2;
							else						_SeekUnicode(&wpstr);
						}

						//�L���v�V�������擾
					  #ifdef UNICODE	//+++
						::lstrcpyn(ppi->title, wpstr, TITLELENGTH);
					  #else
						::WideCharToMultiByte(CP_ACP, 0, wpstr, -1, ppi->title, TITLELENGTH, NULL, NULL);
					  #endif
					}
				}
			}
		}

		//�_�C�A���O�P�ʂ���s�N�Z���P�ʂɕϊ�
		CRect rc(0, 0, max_w, max_h);
		MapDialogRect(&rc);
		m_nWidth   = rc.Width();
		m_nHeight  = rc.Height();
	}


	void _SeekUnicode(WCHAR **wppstr)
	{
		WCHAR *wpstr = *wppstr;

		while (*wpstr != 0)
			wpstr++;

		*wppstr = ++wpstr;
	}


	void _InitTreeView()
	{
		m_TreeView = GetDlgItem(IDC_TREEVIEW);

		//�o�^
		HTREEITEM hParent = TVI_ROOT;
		HTREEITEM hItem   = TVI_ROOT;
		int 	  count   = m_aryPage.GetSize();

		for (int i = 0; i < count; i++) {
			PAGEINFO *ppi = m_aryPage[i];
			m_TreeView.Expand(hParent, TVE_EXPAND);

			if (ppi->nIndent == -1) {
				hItem	= m_TreeView.InsertItem(ppi->title, TVI_ROOT, TVI_LAST);
				hParent = TVI_ROOT;
			} else if (ppi->nIndent == 0) {
				hItem = m_TreeView.InsertItem(ppi->title, hParent, TVI_LAST);
			} else {
				hParent = hItem;
				hItem	= m_TreeView.InsertItem(ppi->title, hItem, TVI_LAST);
			}

			m_TreeView.SetItemData(hItem, i);
		}

		//�ݒ�
		m_TreeView.SetItemHeight(16);
		int 	  style   = m_TreeView.GetWindowLong(GWL_STYLE);
		style &= ~(WS_HSCROLL | WS_TABSTOP);
		m_TreeView.SetWindowLong(GWL_STYLE, style);
	}


	void _ReleasePages()
	{
		ShowWindow(SW_HIDE);

		int count = m_aryPage.GetSize();

		for (int i = 0; i < count; i++) {
			PAGEINFO *ppi = m_aryPage[i];

			if ( ::IsWindow(ppi->hWnd) )
				::DestroyWindow(ppi->hWnd);

			delete ppi;
		}

		m_aryPage.RemoveAll();
	}


	void _BeChild(HWND hWnd, HWND hParent)
	{
		CWindow Wnd   = hWnd;
		int 	style = Wnd.GetWindowLong(GWL_STYLE);

		style |= WS_CHILD | WS_GROUP | WS_TABSTOP;
		style &= ~(WS_POPUP | WS_CAPTION | WS_DISABLED | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
		Wnd.SetWindowLong(GWL_STYLE, style);
		Wnd.SetParent(hParent);
	}


	HWND _LoadDialog(int nIndex)
	{
		PAGEINFO *ppi = m_aryPage[nIndex];

		ATLASSERT(ppi->hWnd == NULL);

		//�R�[���o�b�N�𑗂�
		ppi->psp.pfnCallback( NULL, PSPCB_CREATE, &(ppi->psp) );

		//�_�C�A���O���쐬
		ppi->hWnd = ::CreateDialog(_Module.GetModuleInstance(), ppi->psp.pszTemplate, GetParent(), ppi->psp.pfnDlgProc);

	  #ifndef USE_DIET
		//�c�[���`�b�v��ݒ�
		_SetToolTip(nIndex);
	  #endif

		return ppi->hWnd;
	}


	void _SetTitle(int index)
	{
		PAGEINFO *ppi = m_aryPage[index];
		CString   strTitle;

		strTitle.Format(_T("%s - %s"), m_Title, ppi->title);
		SetWindowText(strTitle);
	}


	BOOL NotifyToDialog(HWND hWnd, int code)
	{
		//�_�C�A���O�Ɋe��ʒm���b�Z�[�W�𑗂�
		NMHDR nmh;

		nmh.code	 = code;
		nmh.hwndFrom = m_hWnd;
		nmh.idFrom	 = 0;
		ATLASSERT(hWnd != NULL);
		return ::SendMessage(hWnd, WM_NOTIFY, 0, (LPARAM) &nmh) != 0;
	}


	void _MoveChild(HWND hWnd)
	{
		CWindow Wnd = hWnd;
		CRect	rc;

		GetClientRect(&rc);
		rc.right -= SPACE;
		rc.left   = rc.right - m_nWidth;
		rc.top	  = SPACE;
		rc.bottom = m_nHeight + rc.top;
		Wnd.MoveWindow(&rc, TRUE);
	}


	typedef struct	DLGTEMPLATEEX_HALF {
		WORD	dlgVer;
		WORD	signature;
		DWORD	helpID;
		DWORD	exStyle;
		DWORD	style;
		WORD	cDlgItems;
		short	x;
		short	y;
		short	cx;
		short	cy;
	};	//DLGTEMPLATEEX�̑O����


  #ifndef USE_DIET
	void _SetToolTip(int nIndex)
	{
	 #if _ATL_VER >= 0x700
		//Option.xml�����[�h
		CComPtr<MSXML2::IXMLDOMDocument2> Doc;
		Doc.CoCreateInstance(CLSID_DOMDocument30);				//+++ xp,vista�ɓ����Ă����
		if (Doc.p == NULL) {
			Doc.CoCreateInstance(CLSID_DOMDocument40);			//+++ unDonut+�Ŏg���Ă�����
			if (Doc.p == NULL) {
				//Doc.CoCreateInstance(CLSID_DOMDocument60);	//+++ ���͂����Ƃ͋@�\���Ă��Ȃ��悤�Ȃ̂ŁA�~��.
				//if (Doc.p == NULL)
				//	Doc.CoCreateInstance(CLSID_DOMDocument20);	//+++ xp(32)�œ����Ă�?
			}
		}
		//+++	�w���v�͂łȂ����ǁA�n���O�͂��Ȃ��悤�ɂ���.
		if (Doc.p == NULL)
			return;

		Doc->put_validateOnParse(VARIANT_FALSE);
		Doc->put_async(VARIANT_FALSE);

		CString 	strPath    = _GetFilePath( _T("help\\Option.xml") );
		if (Doc->load( _variant_t(strPath) ) == VARIANT_FALSE)		//+++ ����msxml6���Ƃ����̃`�F�b�N�Ɉ����������ďI����Ă�...
			return;

		//���[�g<option>���擾�A<option>�̎q�m�[�h���擾
		CComPtr<MSXML2::IXMLDOMElement>   root		   = Doc->GetdocumentElement();
		CComPtr<MSXML2::IXMLDOMNodeList>  rootChildren = root->GetchildNodes();
		PAGEINFO *						  ppi		   = m_aryPage[nIndex];
		int 							  nRootChildLen= rootChildren->Getlength(); 	//+++ �f�o�b�K�Ō��₷���悤�Ɉ�U�ϐ��ɓ����
		for (int i = 0; i < nRootChildLen; ++i) {
			// xml����^�C�g�����擾
			CComQIPtr<MSXML2::IXMLDOMElement> elPage   = rootChildren->Getitem(i);
			_variant_t						  v 	   = elPage->getAttribute( _bstr_t( _T("description") ) );
			CString 						  xmlTitle = (LPCSTR) _bstr_t(v);		//+++ //*���� UNICODE�΍􂢂�?����Ȃ�?

			//xml����擾�����^�C�g���ƃy�[�W�̃^�C�g�����r
			if ( !xmlTitle.Compare(ppi->title) ) {
m_aryToolTip.RemoveAll();	//+++ �����Ă����Ȃ��Ƃ܂���?	//*
				//��v������xml����ID�ƃ`�b�v�e�L�X�g���擾���ăR���g���[���Ƀ`�b�v��ǉ�����
				CComPtr<MSXML2::IXMLDOMNodeList> nlPageItem = elPage->GetchildNodes();
				int 	len = nlPageItem->Getlength();		//+++ �f�o�b�K�Ō��₷���悤�Ɉ�U�ϐ��ɓ����
				for (int k = 0; k < len; ++k) {
					//xml����R���g���[����ID�ƃ`�b�v�e�L�X�g���擾
					CComQIPtr<MSXML2::IXMLDOMElement> elItem	  = nlPageItem->Getitem(k);
					int 							  xmlID 	  = (int) (long) elItem->getAttribute( _bstr_t( _T("id") ) );
					CString 						  strItem	  = (LPCSTR) elItem->Gettext();		//+++ //*���� UNICODE�΍􂢂�?����Ȃ�?

					//�Y��ID�̃R���g���[�����Ȃ�������߂�
					HWND	hWndControl = ::GetDlgItem(ppi->hWnd, xmlID);
					if (!hWndControl)
						continue;

					//�^�u�������폜 - �폜���Ȃ��ƃ`�b�v�������ƕ\������Ȃ�
					while (strItem.Find('\t') != -1)  strItem.Delete( strItem.Find('\t') );

					//�`�b�v�e�L�X�g��t�^
					CToolInfo		ti( TTF_SUBCLASS, hWndControl, 0, NULL, const_cast<LPTSTR>( LPCTSTR(strItem) ) /*+++ strItem.GetBuffer(0)*/ );
					CToolTipCtrl	tip;
					tip.Create(m_hWnd);
					tip.SetMaxTipWidth(500);
					tip.AddTool(ti);

					//�z��ɒǉ�
					m_aryToolTip.Add(tip);
				}

				_ShowToolTip();
				return;
			}
		}
	 #endif
	}


	void _ShowToolTip()
	{
		//+++ �f�o�b�K�Ō��₷���悤�Ɉ�U�ϐ��ɓ����
		BOOL  bButtonChecked = IsDlgButtonChecked(IDC_TOOLTIP);
		UINT  size = m_aryToolTip.GetSize();
		for (UINT i = 0; i < size; ++i) {
			m_aryToolTip[i].Activate(bButtonChecked);
		}
	}
  #endif


	HWND _GetActivePage()
	{
		int 	  nIndex = 0;
		if (m_TreeView.m_hWnd) {
			HTREEITEM hItem  = m_TreeView.GetSelectedItem();
			if (hItem) {
				ATLASSERT(hItem != NULL);
				nIndex = (int) m_TreeView.GetItemData(hItem);
			}
		}
		PAGEINFO *ppi = m_aryPage[nIndex];
		if (ppi == 0)
			return m_TreeView.m_hWnd/*NULL*/;
		return ppi->hWnd;
	}

};



__declspec(selectany) HHOOK 					CTreeViewPropertySheet::s_hHook		= NULL;
__declspec(selectany) CTreeViewPropertySheet*	CTreeViewPropertySheet::s_pThis		= NULL;
__declspec(selectany) int						CTreeViewPropertySheet::m_nCurSel	= 0;


#endif //__PROPERTY_SHEET_H_
