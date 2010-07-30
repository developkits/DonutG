/**
 *	@file 	MyDialog.cpp
 *	@barief	LinkExtractBar�̃_�C�A���O�̏���
 *	@note
 *		OnIrvine,OnIria,OnFlashGet,OnDcSakura,OnNetTrans,OnReget
 *		�Ƃ����֐�����Ή�����ID�������邪�A
 *		�_�E�����[�_�[�̏����̓X�N���v�g|�R�}���h���������߁A
 *		�{���Ȃ�OnDownLoaderButton0�`5���ɖ��O��ς���ׂ�����
 *		�s�����Ă��̂܂܂ɂȂ��Ă���B���O�Ǝ��ۂɎ��s�����_�E�����[�_��
 *		��v���Ȃ��̂Œ��ӂ��Ă��������B
 */
// MyDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "linkextractbar.h"
#include "MyDialog.h"
#include "DataFile.h"


//#define OLD_DOWNLOADER	//+++

#ifdef _DEBUG
 # define new DEBUG_NEW
 # undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Misc.h"



/////////////////////////////////////////////////////////////////////////////
// CMyDialog �_�C�A���O


CMyDialog::CMyDialog(CWnd *pParent /*=NULL*/)
	: CDialog(CMyDialog::IDD, pParent)
  #if 1	//+++
	, m_autoDownloader(0)
	, m_nSbtnDownloader(0)
	, m_dir( Misc::GetFileNameNoExt(Misc::GetExeFileName()) )
	, m_baseName( Misc::GetFileBaseName(m_dir) )
	, m_iniFile( m_dir + _T('\\') + m_baseName + _T(".ini") )
  #endif
{
	m_dir += _T('\\');
	//{{AFX_DATA_INIT(CMyDialog)
	// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}



void CMyDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDialog)
	DDX_Control(pDX, IDC_SELECTED    , m_Selected);
  #if 1	//+++
	DDX_Control(pDX, IDC_IMGEXTRACT  , m_ImageChk);
	DDX_Control(pDX, IDC_CHK_SBTN    , m_chkSbtn);
	DDX_Control(pDX, IDC_REGEX		 , m_chkRegex);
	DDX_Control(pDX, IDC_CHK_SAVE	 , m_chkSave);
	DDX_Control(pDX, IDC_CMB_SBTN_SEL, m_cmbSbtnSel);
  #endif
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	//{{AFX_MSG_MAP(CMyDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1		, OnCloseDlg		)
	ON_BN_CLICKED(IDC_FILTER		, OnFilter			)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1	, OnListRClick		)
	ON_COMMAND(ID_ALLSELECT			, OnAllSelect		)
	ON_COMMAND(ID_SELECTRELEASE		, OnSelectRelease	)
	ON_COMMAND(ID_SELECTREVERSE		, OnSelectReverse	)
	ON_COMMAND(ID_EXCLUDENOTSELECT	, OnExcludeNotSelect)
	ON_COMMAND(ID_EXCLUDESELECT		, OnExcludeSelect	)
	ON_BN_CLICKED(IDC_SAVE			, OnSave			)
	ON_BN_CLICKED(IDC_COPY			, OnCopy			)
  #if 1	//+++ ���łɖ��O�͉R��ԁB�{���Ȃ�OnDownloader0�`5�݂����ɂ��ׂ��Ƃ��B
	ON_BN_CLICKED(IDC_IRVINE		, OnIrvine			)
	ON_BN_CLICKED(IDC_IRIA			, OnIria			)
	ON_BN_CLICKED(IDC_FLASHGET		, OnFlashGet		)
	ON_BN_CLICKED(IDC_DCSAKURA		, OnDcSakura		)
	ON_BN_CLICKED(IDC_NETTRANS		, OnNetTrans		)
	ON_BN_CLICKED(IDC_REGET			, OnReget			)	//+++
  #endif
	ON_BN_CLICKED(IDC_OPEN			, OnOpen			)
	ON_NOTIFY(HDN_ITEMCLICK, 0		, OnHdrClickList	)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO1		, OnSelChangeFrameCmb)
	ON_BN_CLICKED(IDC_SELECTED		, OnSelected		)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST1, OnSetFocusList1	)
	ON_NOTIFY(NM_CLICK, IDC_LIST1	, OnClickList1		)
	ON_BN_CLICKED(IDC_IMGEXTRACT	, OnImgExtract		)
	ON_COMMAND(ID_SELECTEDITEM_OPEN	, OnSelectedItemOpen)
	ON_COMMAND(ID_ALLITEM_OPEN		, OnAllItemOpen		)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CMyDialog ���b�Z�[�W �n���h��

BOOL CMyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nSbtnDownloader = ::GetPrivateProfileInt("etc", "DefaultDownloader",0, m_iniFile);		//+++
	int     bSelected = ::GetPrivateProfileInt("etc", "Selected"		, 0, m_iniFile) != 0;	//+++
	int     bImgChk   = ::GetPrivateProfileInt("etc", "ImageCheck"		, 0, m_iniFile) != 0;	//+++
	int		bSbtn	  = ::GetPrivateProfileInt("etc", "S_Button"		, 0, m_iniFile) != 0;	//+++
	int		bChkSave  = ::GetPrivateProfileInt("etc", "SaveSearchData"	, 0, m_iniFile) != 0;	//+++
	int		w		  = ::GetPrivateProfileInt("etc", "Width"			, 500, m_iniFile);		//+++
	int		h		  = ::GetPrivateProfileInt("etc", "Height"			, 400, m_iniFile);		//+++
	if (w == 0)
		w = 500;
	if (h == 0)
		h = 400;

	//�R���g���[���̏�����
	m_cmbFrame.Attach(GetDlgItem(IDC_COMBO1)->m_hWnd);
	m_cmbExt.Attach(GetDlgItem(IDC_COMBO2)->m_hWnd);
	m_cmbNExt.Attach(GetDlgItem(IDC_COMBO3)->m_hWnd);
	m_List.Attach(GetDlgItem(IDC_LIST1)->m_hWnd);
	m_editSearch.Attach(GetDlgItem(IDC_EDIT1)->m_hWnd);	//+++

	m_Selected.SetCheck(bSelected);		//+++ m_Selected.SetCheck(1);
	m_ImageChk.SetCheck(bImgChk);		//+++
	m_chkSbtn.SetCheck(bSbtn);			//+++
	m_chkSave.SetCheck(bChkSave);		//+++

	//�R���g���[���̃Z�b�e�B���O
	m_List.InsertColumn(0, "URL");
	m_List.InsertColumn(1, "�g���q");
	m_List.SetColumnWidth(0, 300);
	m_List.SetColumnWidth(1, 60);

  #if 1	//+++
	InitDownloader();
  #endif

  #if 1	//+++ dll�̂���t�H���_��ɕύX.
	CString		fname = m_dir + m_baseName + _T(".dat");
	CDataFile	dat;
	int			rc;
	if (dat.Open(fname) != 0) {
		CString	  str;
		while (dat.ReadLine(str) != 0) {
			rc = m_cmbExt.AddString( str );
			rc = m_cmbNExt.AddString( str );
		}
	}

	//+++ �����������Z�[�u���Ă����ꍇ.
	if (bChkSave) {
		int		bRegex	  = ::GetPrivateProfileInt(_T("SearchData"), _T("Regex") , 0, m_iniFile) != 0;	//+++
		m_chkRegex.SetCheck(bRegex);			//+++
		TCHAR	buf[0x1000];
		buf[0] = 0;
		::GetPrivateProfileString(_T("SearchData"), _T("Ext"), _T(""), buf, sizeof buf, m_iniFile);
		m_cmbExt.SetWindowText(buf);

		buf[0] = 0;
		::GetPrivateProfileString(_T("SearchData"), _T("NExt"), _T(""), buf, sizeof buf, m_iniFile);
		m_cmbNExt.SetWindowText(buf);

		buf[0] = 0;
		::GetPrivateProfileString(_T("SearchData"), _T("SearchStr"), _T(""), buf, sizeof buf, m_iniFile);
		m_editSearch.SetWindowText(buf);
		//m_editSearch.SetSel(0,0);
		//m_editSearch.ReplaceSel(buf);
	}
  #else
	CDataFile	dat;
	if (dat.Open(PLUGIN_DIR "LinkExtractBar.dat") != 0 || dat.Open("Plugin/LinkExtractBar.dat") != 0)
	{
		string	  str;
		while (dat.ReadLine(&str) != 0) {
			m_cmbExt.AddString( str.c_str() );
			m_cmbNExt.AddString( str.c_str() );
		}
	}
  #endif

	::CoInitialize(NULL);
	InitComboFrame();
	::CoUninitialize();

	if (m_cmbFrame.GetCount() != 0) {
		m_cmbFrame.SetCurSel(0);
		ShowItem(0);
	}

	AdjustButtonState();

	if (w < 482 || w > 0x4000 || h < 280 || h > 0x4000) {	//+++ �T�C�Y��ݒ�t�@�C���ɋL������悤�ɂ����̂ŁA���̔����ǉ�
		w = 482;
		h = 280;
	}
	MoveWindow(0, 0, w, h);

	CenterWindow( GetDesktopWindow() );

	if (m_autoDownloader) {	//+++ �_�E�����[�_�[���Ăԏꍇ
		SendToDownloader();
	}

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
				  // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}



void CMyDialog::ShowItem(int FrameNum)
{
	int 		count		= 0;
	CButton *	pImgCheck	= (CButton *) GetDlgItem(IDC_IMGEXTRACT);
	BOOL		bImg		= pImgCheck->GetCheck();

	m_List.SetRedraw(FALSE);
	m_List.DeleteAllItems();

	if (FrameNum != 0) {
		BOOL	bfound = FALSE;

		//�t���[�����̃����N��\��
		for (SviMap::iterator it = m_map.begin(); it != m_map.end(); ++it) {
			vector<int>*	ary = &it->second;
			if (bImg == FALSE) {
				for (vector<int>::iterator vt = ary->begin(); vt != ary->end(); ++vt) {
					//IMG�^�O��r��
					if (*vt == -1)
						goto Next;
				}

				for (vector<int>::iterator vt = ary->begin(); vt != ary->end(); ++vt) {
					if (*vt == FrameNum) {
						string str = it->first;
						InsertItemToList(count, &str);
						count++;
						break;
					}
				}
			} else {
				for (vector<int>::iterator vt = ary->begin(); vt != ary->end(); ++vt) {
					if (*vt == FrameNum) {
						string str = it->first;
						InsertItemToList(count, &str);
						count++;
						break;
					}
				}
			}

		  Next: //to next item
			;
		}
	} else {
		BOOL	bfound = FALSE;

		//�S�����N��\��
		for (SviMap::iterator it = m_map.begin(); it != m_map.end(); ++it) {
			if (bImg) {
				//IMG�^�O������Ȃ�ΑS�Ẵ����N��\��
				string str = it->first;
				InsertItemToList(count, &str);
				count++;
				continue;
			} else {
				//IMG�������Ȃ��Vector�ɓ���Ă�����-1���m�F
				vector<int> *	ary = &it->second;
				for (vector<int>::iterator vt = ary->begin(); vt != ary->end(); ++vt) {
					if (*vt == -1) {
						bfound = TRUE;
						break;
					}
				}

				if (bfound == FALSE) {
					string str = it->first;
					InsertItemToList(count, &str);
					count++;
				}

				bfound = FALSE;
			}
		}
	}

	if (m_List.m_hWnd)	//+++ NULL�`�F�b�N
		m_List.SetRedraw(TRUE);
}



void CMyDialog::InsertItemToList(int count, string *str)
{
	m_List.InsertItem( count, str->c_str() );
	string::size_type 	pos = str->rfind(".");
	if (pos != string::npos) {
		string	ext = str->substr(pos + 1);
		if (ext.find("/") == string::npos)
			m_List.SetItemText( count, 1, ext.c_str() );
	}
}



//m_cmbFrame�̏��������[�`��
BOOL CMyDialog::InitComboFrame()
{
	IAPI *					pDonutP = 0;

	HRESULT	hr	= ::CoCreateInstance(CLSID_API, NULL, CLSCTX_SERVER, IID_IAPI, (void **) &pDonutP);
	if ( FAILED(hr) )
		return FALSE;

	if (pDonutP->TabIndex == -1) {
		if (m_cmbFrame.m_hWnd)	//+++ NULL�`�F�b�N
			m_cmbFrame.EnableWindow(FALSE);
		return TRUE;
	}

	//�h�L�������g���擾
	IDispatch*		disp = pDonutP->GetDocumentObject(pDonutP->TabIndex);
	if (disp == NULL)
		return TRUE;

	IHTMLDocument2*	doc  = 0;
	hr		   = disp->QueryInterface(IID_IHTMLDocument2, (void **) &doc);
	if ( FAILED(hr) )
		return TRUE;

	//���C���h�L�������g���烊�X�g�ɒǉ�
	AddMainLinksToList(doc);

	//�t���[���h�L�������g���烊�X�g�ɒǉ�
	int						framecount  = 2;
	IHTMLFramesCollection2*	frameCol	= 0;
	if ( SUCCEEDED( doc->get_frames(&frameCol) ) )
		AddFrameLinksToList(frameCol, &framecount);

	if (m_cmbFrame.m_hWnd)	//+++ NULL�`�F�b�N
		m_cmbFrame.InsertString( 0, ("�i�S�t���[���j") );

	pDonutP->Release();
	return TRUE;
}



void CMyDialog::AddFrameLinksToList(IHTMLFramesCollection2 *frameCol, int *totalframe)
{
	long	count = 0;
	if ( FAILED( frameCol->get_length(&count) ) )
		return;
	if (count <= 0)
		return;

	USES_CONVERSION;

	for (int i = 0; i < count; i++) {
		VARIANT					frameRequested;
		frameRequested.vt	= VT_I4;
		frameRequested.lVal = i;

		VARIANT 				frameOut;
		if ( FAILED( frameCol->item(&frameRequested, &frameOut) ) )
			continue;

		IHTMLWindow2 *			window = 0;
		if ( FAILED( frameOut.pdispVal->QueryInterface(
						IID_IHTMLWindow2, (void **) &window) ) )
			continue;

		IHTMLDocument2 *		frmdoc = 0;
		if ( FAILED( window->get_document(&frmdoc) ) )
			continue;

		//�A���J�[�^�O�ɂ�郊���N�����X�g�ɓo�^
		IHTMLElementCollection*	linkCol = 0;
		if ( FAILED( frmdoc->get_links(&linkCol) ) )
			continue;

		AddLink_Anchor( linkCol, (*totalframe) );

		//�C���[�W�^�O�ɂ�郊���N�����X�g�ɓo�^
		IHTMLElementCollection*	imageCol = 0;
		if ( FAILED( frmdoc->get_images(&imageCol) ) )
			continue;

		AddLink_Image( imageCol, (*totalframe) );

		//cmbFrame��totalframe�Ԗڂ̍��ڂƂ��ēo�^
		BSTR					bsTitle;
		if ( FAILED( frmdoc->get_title(&bsTitle) ) )
			continue;

		m_cmbFrame.AddString( OLE2T(bsTitle) );
		(*totalframe)++;

		//��艺�ʂ̃t���[������������
		IHTMLFramesCollection2*	nestframes = 0;
		if ( FAILED( frmdoc->get_frames(&nestframes) ) )
			continue;

		AddFrameLinksToList(nestframes, totalframe);
	}
}



void CMyDialog::AddMainLinksToList(IHTMLDocument2 *mainDoc)
{
	IHTMLElementCollection *linkCol, *imageCol;
	BSTR					bsTitle;

	//�A���J�[�^�O�ɂ�郊���N�����X�g�ɓo�^
	if ( FAILED( mainDoc->get_links(&linkCol) ) )
		return;

	AddLink_Anchor(linkCol, 1);
	linkCol->Release();

	//�C���[�W�^�O�ɂ�郊���N�����X�g�ɓo�^
	if ( FAILED( mainDoc->get_images(&imageCol) ) )
		return;

	AddLink_Image(imageCol, 1);
	imageCol->Release();

	//cmbFrame��0�Ԗڂ̍��ڂƂ��ēo�^
	if ( FAILED( mainDoc->get_title(&bsTitle) ) )
		return;

	USES_CONVERSION;
	if (m_cmbFrame.m_hWnd)	//+++ NULL�`�F�b�N�ǉ�
		m_cmbFrame.AddString( OLE2T(bsTitle) );
}



//�A���J�[�̃����N��ǉ�
void CMyDialog::AddLink_Anchor(IHTMLElementCollection *elemCol, int Num)
{
	USES_CONVERSION;

	long				lnkct = 0;
	if ( FAILED( elemCol->get_length(&lnkct) ) )
		return;

	for (long i = 0; i < lnkct; i++) {
		VARIANT		varIndex;
		varIndex.vt   = VT_I4;
		varIndex.lVal = i;

		IDispatch	*		disp = 0;
		if ( FAILED( elemCol->item(varIndex, varIndex, &disp) ) )
			continue;

		IHTMLAnchorElement*	anchor	= 0;
		HRESULT 			ret		= disp->QueryInterface(IID_IHTMLAnchorElement, (void **) &anchor);

		if ( FAILED(ret) ) {
			//���x��Area�Ŏ����Ă݂�
			IHTMLAreaElement*	area = 0;
			ret = disp->QueryInterface(IID_IHTMLAreaElement, (void **) &area);

			if ( FAILED(ret) ) {
				disp->Release();
				continue;
			}

			BSTR				bstr;
			ret = area->get_href(&bstr);
			disp->Release();
			area->Release();

			if ( FAILED(ret) )
				continue;

			AddItem(OLE2T(bstr), Num);
		} else {
			BSTR				bstr;
			ret = anchor->get_href(&bstr);
			disp->Release();
			anchor->Release();

			if ( FAILED(ret) )
				continue;

			AddItem(OLE2T(bstr), Num);
		}
	}
}



//�C���[�W�̃����N��ǉ�
void CMyDialog::AddLink_Image(IHTMLElementCollection *elemCol, int Num)
{
	USES_CONVERSION;

	long			 lnkct = 0;
	if ( FAILED( elemCol->get_length(&lnkct) ) )
		return;

	for (long i = 0; i < lnkct; i++) {
		VARIANT 		 varIndex;
		varIndex.vt   = VT_I4;
		varIndex.lVal = i;

		IDispatch	*	 disp = 0;
		if ( FAILED( elemCol->item(varIndex, varIndex, &disp) ) )
			continue;

		IHTMLImgElement *image = 0;
		if ( FAILED( disp->QueryInterface(IID_IHTMLImgElement , (void **) &image) ) )
			continue;

		BSTR	bstr;
		if ( FAILED( image->get_href(&bstr) ) )
			continue;

		if (bstr == 0)
			continue;

		AddItem(OLE2T(bstr), Num, 1);
		disp->Release();
	}
}



void CMyDialog::AddItem(LPCTSTR str, int Num, int img )
{
	vector<int> 		count_frame;
	if (img == 0)
		count_frame.push_back(Num);
	else
		count_frame.push_back(-1);

	//�����N��o�^����
	string				buf = str;
	string::size_type	pos = buf.rfind("#");
	if (pos != string::npos) {
		buf.erase(pos);
	}

	//pos = buf.find("://");
	//if(pos==string::npos)
	//	return;
	pair<SviMap::iterator, bool> ret;
	ret = m_map.insert( pair<string, vector<int> >(buf, count_frame) );

	if (ret.second == false) {
		//���łɓ��������N���o�^����Ă���
		//�t���[���̔ԍ���o�^
		vector<int> *	ary = &ret.first->second;

		for (vector<int>::iterator it = ary->begin(); it != ary->end(); ++it)
			if (img == 0) {
				if (*it == Num)
					return;
			} else {
				if (*it == -1)
					return;
			}

		if (img == 0)
			ary->push_back(Num);
		else
			ary->push_back(-1);
	}
}



void CMyDialog::OnSelChangeFrameCmb()
{
	ShowItem( m_cmbFrame.GetCurSel() );
	AdjustButtonState();
}



void CMyDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	const int space = 4;
	CEdit	  *Edit  = (CEdit   *) GetDlgItem(IDC_EDIT1);
	CButton   *Btn	 = (CButton *) GetDlgItem(IDC_BUTTON1);
	CStatic   *Label = (CStatic *) GetDlgItem(IDC_STATIC2);

	if (Edit == NULL || Btn == NULL || Label == NULL)
		return;

	RECT	   cs;

	//�R���{�ƃG�f�B�b�g�̔z�u
	m_cmbFrame.GetWindowRect(&cs);
	GetRect(&cs);
	cs.right  = cx - space;
	m_cmbFrame.MoveWindow(&cs);

	m_cmbExt.GetWindowRect(&cs);
	GetRect(&cs);
	cs.right  = cx - space;
	m_cmbExt.MoveWindow(&cs);

	m_cmbNExt.GetWindowRect(&cs);
	GetRect(&cs);
	cs.right  = cx - space;
	m_cmbNExt.MoveWindow(&cs);

	Edit->GetWindowRect(&cs);
	GetRect(&cs);
	cs.right  = cx - space;
	Edit->MoveWindow(&cs);

	//���X�g�̔z�u
	m_List.GetWindowRect(&cs);
	GetRect(&cs);
	cs.right  = cx - space;
	cs.bottom = cy - 27 - space;
	m_List.MoveWindow(&cs);
	m_List.SetColumnWidth(0, cx - 90);

	//�{�^���̔z�u
	Btn->GetClientRect(&cs);
	Btn->SetWindowPos(NULL, cx - cs.right - 4, cy - cs.bottom - 6, 0, 0, SWP_NOSIZE);

	//���x���̔z�u
	Label->SetWindowPos(NULL, space, cy - cs.bottom - 6, 0, 0, SWP_NOSIZE);
}



void CMyDialog::GetRect(LPRECT rec)
{
	POINT pos[2];

	pos[0].x	= rec->left;
	pos[0].y	= rec->top;
	pos[1].x	= rec->right;
	pos[1].y	= rec->bottom;
	::ScreenToClient(m_hWnd, &pos[0]);
	::ScreenToClient(m_hWnd, &pos[1]);
	rec->left	= pos[0].x;
	rec->top	= pos[0].y;
	rec->right	= pos[1].x;
	rec->bottom = pos[1].y;
}



void CMyDialog::OnCloseDlg()
{
	CMyDialog::OnClose();
	CMyDialog::OnOK();
}



void CMyDialog::DoFilter()
{
	CEdit* 		Edit   = (CEdit   *) GetDlgItem(IDC_EDIT1);
	CButton*	pCheck = (CButton *) GetDlgItem(IDC_REGEX);

	//����
	vector<string>	aExt;
	vector<string>	aNExt;
	bool		bChkExt  = GetFilterTexts(IDC_COMBO2, &aExt);
	bool		bChkNExt = GetFilterTexts(IDC_COMBO3, &aNExt);

	//char buf[1024]; //�����񂩂��ȁH
	char		buf[BUF_SIZE]; //�����񂩂��ȁH	//+++
	string		strExp;
	bool		bSearch;
	if (Edit->GetWindowText(buf, sizeof buf /*1024*/) == 0) {
		strExp	= ".";
		bSearch = false;
	} else {
		bSearch = true;
		strExp	= buf;
	}

	if (bChkExt == false && bChkNExt == false && bSearch == false) {
		OnSelChangeFrameCmb();
		return;
	}

	map<string, bool>	mURL;

	try {
		boost::match_results<const char *>  results;
		boost::basic_regex<char> 			regex;

		if (pCheck->GetCheck() == 1)
			regex = strExp.c_str();
		else if (pCheck->GetCheck() == 0) {
			string wc("*");
			strExp.insert(0, wc);
			strExp.append(wc);
		}

		int ct = m_List.GetItemCount();
		if (ct == 0)
			return;

		for (int i = 0; i < ct; i++) {
			m_List.GetItemText(i, 0, buf, BUF_SIZE);
			string	str = buf;

			//�\���g���q�`�F�b�N
			if (bChkExt == true) {
				if (CheckIncludeExt(&str, &aExt) == false)
					continue;
			}

			//��\���g���q�`�F�b�N
			if (bChkNExt == true) {
				if (CheckIncludeExt(&str, &aNExt) != false)
					continue;
			}

			//���K�\���ɂ�錟��
			if (bSearch != false) {
				if (pCheck->GetCheck() == 1) {
					const char *source = str.c_str();

					if (boost::regex_search(source, results, regex) == false)
						continue;
				} else if (pCheck->GetCheck() == 0) {
					if (StrMatch( strExp.c_str(), str.c_str() ) == 0)
						continue;
				}
			}

			//�����ēo�^
			mURL.insert( pair<string, bool>(str, true) );
		}
	} catch (boost::bad_expression) {
		MessageBox("���K�\�������񂪕s�K�؂ł�");
		return;
	}

	m_List.SetRedraw(FALSE);
	m_List.DeleteAllItems();

	int ct = 0;
	for (map<string, bool>::iterator it = mURL.begin();
		it != mURL.end();
		++it)
	{
		string tmp = it->first;
		InsertItemToList(ct, &tmp);
		++ct;
	}

	m_List.SetRedraw(TRUE);
}



void CMyDialog::ParseString(vector<string> *ary, string *str)
{
	typedef boost::tokenizer<boost::char_separator<char> >	 Tokens;
	boost::char_separator<char> sep(",; ");
	Tokens						tokens(*str, sep);

	for (Tokens::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		ary->push_back(*it);
	}
}



bool CMyDialog::GetFilterTexts(DWORD CmbID, vector<string> *ary)
{
	CComboBox *cmb = (CComboBox *) GetDlgItem(CmbID);
	// char buf[1024]; //����邩�ȁH
	char	   buf[BUF_SIZE]; //����邩�ȁH	//+++

	//idx = cmb->GetCurSel();
	//if(idx==CB_ERR) return false;
	//cmb->GetLBText(idx,buf);
	int ret = GetEditWnd(cmb)->GetWindowText(buf, sizeof buf);
	if (ret == 0)
		return false;

	string	   str = buf;
	ParseString(ary, &str);
	return true;
}



bool CMyDialog::CheckIncludeExt(string *str, vector<string> *ary)
{
	for (vector<string>::iterator it = ary->begin(); it != ary->end(); ++it) {
		string::size_type	 pos = str->find( it->c_str() );
		if (pos != string::npos)
			return true;
	}
	return false;
}



void CMyDialog::OnFilter()
{
	DoFilter();
	AdjustButtonState();
}



CWnd *CMyDialog::GetEditWnd(CComboBox *Parent)
{
	CWnd *pWnd = Parent->GetTopWindow();

	while (pWnd != NULL) {
		char  buf[BUF_SIZE];
		::GetClassName(pWnd->m_hWnd, buf, BUF_SIZE);
		if (lstrcmpi(buf, "EDIT") == 0)
			return pWnd;
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	return &CWnd();
}



bool CMyDialog::SaveURLList(LPCTSTR fname)		//+++ �����ύX
{
	FILE * fp;
	char   buf[BUF_SIZE];
	string str;
	int    ct, len;

	fp = ::fopen(fname, "w");
	if (fp == NULL)
		return false;

	ct = m_List.GetItemCount();

	for (int i = 0; i < ct; i++) {
		if (CheckSelectedUse() && IsSelected(i) == FALSE)
			continue;

		len = m_List.GetItemText(i, 0, buf, BUF_SIZE);

		if (len != 0) {
			fwrite(buf, 1, len, fp);
			fwrite("\n", 1, 1, fp);
		}
	}

	::fclose(fp);

	return true;
}



void CMyDialog::AdjustButtonState()
{
	CStatic*			Label	 = (CStatic *) GetDlgItem(IDC_STATIC);
	int					BtnCount = 4 + m_downloader.size();		//+++ �_�E�����[�_�[���O���X�N���v�g�ǂݍ��݂ɕύX
	static const DWORD	Btns[]   = {
		IDC_OPEN, IDC_COPY,   IDC_SAVE, 	IDC_FILTER ,
		IDC_IRIA, IDC_IRVINE, IDC_FLASHGET,		IDC_DCSAKURA, IDC_NETTRANS, IDC_REGET
	};

	int ct;
	if ( CheckSelectedUse() )
		ct = m_List.GetSelectedCount();
	else
		ct = m_List.GetItemCount();

	if (ct == 0) {
		for (int i = 0; i < BtnCount; i++)
			GetDlgItem(Btns[i])->EnableWindow(FALSE);
	} else {
		for (int i = 0; i < BtnCount && i < m_downloaderSize; i++) {
			GetDlgItem(Btns[i])->EnableWindow(TRUE);
		}
	}

	ShowItemCount();
}



void CMyDialog::ShowItemCount()
{
	int 	ct	= m_List.GetItemCount();
	char	 a[20];
	_itoa(ct, a, 10);
	string	 str(a);
	str += "�̃I�u�W�F�N�g";

	CStatic *Label = (CStatic *) GetDlgItem(IDC_STATIC2);
	Label->SetWindowText( str.c_str() );
}



void CMyDialog::OnListRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	AdjustButtonState();

	if (m_List.GetItemCount() == 0)
		return;

	CMenu 	menu;
	menu.LoadMenu(IDR_POPMENU);
	CMenu*	mpopup = menu.GetSubMenu(0);

	if (mpopup == NULL)
		return;

	POINT 	pos;
	GetCursorPos(&pos);
	mpopup->TrackPopupMenu(TPM_CENTERALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pos.x , pos.y , this);

	*pResult = 0;
}



///+++ �����F�S�I��
void CMyDialog::OnAllSelect()
{
	CListCtrl*	List = (CListCtrl *) GetDlgItem(IDC_LIST1);
	int			ct	 = List->GetItemCount();
	List->SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
	AdjustButtonState();
}



///+++ ����:�I������
void CMyDialog::OnSelectRelease()
{
	CListCtrl*	List = (CListCtrl *) GetDlgItem(IDC_LIST1);
	int 	   	ct 	 = List->GetItemCount();
	List->SetItemState(-1, 0, LVIS_SELECTED);
	AdjustButtonState();
}



///+++ ����:�I�𔽓]
void CMyDialog::OnSelectReverse()
{
	CListCtrl*	List = (CListCtrl *) GetDlgItem(IDC_LIST1);
	int 	   	ct	 = List->GetItemCount();
	List->SetRedraw(FALSE);

	for (int i = 0; i < ct; ++i) {
		if (List->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			List->SetItemState(i, 0, LVIS_SELECTED);
		else
			List->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}

	List->SetRedraw(TRUE);
	AdjustButtonState();
}



void CMyDialog::OnExcludeNotSelect()
{
	CListCtrl *List = (CListCtrl *) GetDlgItem(IDC_LIST1);
	int 	   idx	= 0;

	List->SetRedraw(FALSE);

	while ( idx < List->GetItemCount() ) {
		if (List->GetItemState(idx, LVIS_SELECTED) == 0)
			List->DeleteItem(idx);
		else
			idx++;
	}

	List->SetRedraw(TRUE);
	ShowItemCount();
	AdjustButtonState();
}



void CMyDialog::OnExcludeSelect()
{
	CListCtrl*	List = (CListCtrl *) GetDlgItem(IDC_LIST1);
	int 		idx	 = 0;

	List->SetRedraw(FALSE);

	while ( idx < List->GetItemCount() ) {
		if (List->GetItemState(idx, LVIS_SELECTED) == LVIS_SELECTED)
			List->DeleteItem(idx);
		else
			idx++;
	}

	List->SetRedraw(TRUE);
	ShowItemCount();
	AdjustButtonState();
}



void CMyDialog::OnSave()
{
	char			buf[MAX_PATH];
	::memset(buf, 0, MAX_PATH);
	char			title[MAX_PATH];
	::memset(title, 0, MAX_PATH);

	OPENFILENAME 	ofn;
	::memset( &ofn, 0, sizeof (OPENFILENAME) );
	ofn.lStructSize		= sizeof (OPENFILENAME);
	ofn.hwndOwner		= m_hWnd;
	ofn.lpstrFile		= buf;
	ofn.nMaxFile		= MAX_PATH;
	ofn.lpstrFilter		= "�e�L�X�g�t�@�C��(*.txt)\0*.txt\0���[�U�[��`(*.*)\0*.*";
	ofn.lpstrFileTitle	= title;
	ofn.nMaxFileTitle	= 64;
	ofn.lpstrTitle		= "URL���X�g�̕ۑ�";
	ofn.nFilterIndex	= 1;
	ofn.lpstrDefExt		= "txt";
	ofn.Flags			= OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ;

	if (GetSaveFileName(&ofn)) {
		//string 	str = ofn.lpstrFile;
		//SaveURLList(str.c_str());
		SaveURLList(ofn.lpstrFile);
	}
}


void CMyDialog::OnCopy()
{
	CString		str;
	int			ct = m_List.GetItemCount();
	for (int i = 0; i < ct; i++) {
		if (CheckSelectedUse() && IsSelected(i) == FALSE)
			continue;

		CString strbuf = m_List.GetItemText(i, 0);
		str   += strbuf;
		str   += "\r\n";
	}

	HGLOBAL hGlobal = 0;
	try {
		unsigned	l   = str.GetLength();
		if (l == 0)
			throw 0;

		if ( !OpenClipboard() )
			throw 0;

		if ( !EmptyClipboard() )
			throw 1;

		hGlobal			= (HGLOBAL) GlobalAlloc(GPTR, l + 1);
		char *	p		= (char *) GlobalLock(hGlobal);
		if (p == NULL)
			throw 2;

		::memcpy( (void *) p, (const void *) (LPCTSTR) str, l );
		//p[str.GetLength() + 1] = '\0';		//+++ �@+1���Ɣ͈̓I�[�o�[
		p[l]			= '\0';
		GlobalUnlock(hGlobal);

		if (SetClipboardData(CF_TEXT, hGlobal) == NULL)
			throw 2;

		//GlobalFree(hGlobal); //���L�����N���b�v�{�[�h�Ɉڂ����̂ŊJ�����Ă͂����Ȃ�
		//9x�n�ł��݂��������藎�����肷��
		CloseClipboard();
		return;

	} catch (int stage) {
		MessageBox("�R�s�[�Ɏ��s���܂����B");

		if (stage >= 1)
			CloseClipboard();

		if (stage >= 2)
			GlobalFree(hGlobal);

		return;
	}
}


void CMyDialog::OnOpen()
{
	OnOpen_Sub(FALSE, FALSE);
}



void CMyDialog::OnOpen_Sub(BOOL bForceFlag = FALSE, BOOL bSelected = FALSE)
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	const int Upper = 10;

	BOOL	  checkflag;
	if (bForceFlag)
		checkflag = bSelected;
	else
		checkflag = CheckSelectedUse();

	int 	  ct;
	if (checkflag)
		ct = m_List.GetSelectedCount();
	else
		ct = m_List.GetItemCount();

	if (ct >= Upper) {
		int IDRet = MessageBox("���ڂ������ł����{���Ɏ��s���܂����H", "�m�F", MB_YESNO);
		if (IDRet == IDNO)
			return;
	}

	ct = m_List.GetItemCount();

	IAPI *	  pDonutP;
	HRESULT   hr;

	hr = ::CoInitialize(0);

	if ( FAILED(hr) )
		return;

	hr = ::CoCreateInstance(CLSID_API, NULL, CLSCTX_SERVER, IID_IAPI, (void **) &pDonutP);

	if ( FAILED(hr) )
		return;

	USES_CONVERSION;

	char	  buf[BUF_SIZE];
	if (checkflag) {
		for (int i = 0; i < ct; i++) {
			if ( IsSelected(i) ) {
				m_List.GetItemText(i, 0, buf, BUF_SIZE);
				pDonutP->NewWindow( T2OLE(buf), pDonutP->GetTabCount() );
			}
		}
	} else {
		for (int i = 0; i < ct; i++) {
			m_List.GetItemText(i, 0, buf, BUF_SIZE);
			pDonutP->NewWindow( T2OLE(buf), pDonutP->GetTabCount() );
		}
	}

	::CoUninitialize();
}



int CALLBACK CMyDialog::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	pair<CMyDialog *, BOOL> *getData = (pair<CMyDialog *, BOOL> *)lParamSort;
	CListCtrl * 			 pList	 = &(getData->first->m_List);
	CString 				 str1	 = pList->GetItemText(lParam1, 0);
	CString 				 str2	 = pList->GetItemText(lParam2, 0);

	int 					 iReturn;
	if (getData->second == FALSE)
		iReturn = ::strcmp( str1, str2 );	// ����
	else
		iReturn = ::strcmp( str2, str1 );	// �~��
	return iReturn;
}


int CALLBACK CMyDialog::CompareFunc2(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	pair<CMyDialog *, BOOL> *getData = (pair<CMyDialog *, BOOL> *)lParamSort;
	CListCtrl * 			 pList	 = &(getData->first->m_List);
	CString 				 str1	 = pList->GetItemText(lParam1, 1);
	CString 				 str2	 = pList->GetItemText(lParam2, 1);

	// strcmp���g���Ȃ�A�~���̏ꍇ��str1��str2���t�ɂ��Ȃ���΂Ȃ�Ȃ�
	int 					 iReturn;
	if (getData->second == FALSE)
		iReturn = ::strcmp( str1, str2 );	// ����
	else
		iReturn = ::strcmp( str2, str1 );	// �~��
	return iReturn;
}


void CMyDialog::RenumberItem()
{
	LV_ITEM 	lvItem;
	for (int i = 0; i < m_List.GetItemCount(); i++) {
		lvItem.iItem	= i;
		lvItem.iSubItem = 0;
		lvItem.mask 	= LVIF_PARAM;
		lvItem.lParam	= i;				// �����Ŕԍ����A�C�e���Ɏw�肷��
		m_List.SetItem(&lvItem);
	}
}


void CMyDialog::OnHdrClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	HD_NOTIFY * 			phdn = (HD_NOTIFY *) pNMHDR;
	static BOOL 			d1	 = FALSE;
	static BOOL 			d2	 = FALSE;

	pair<CMyDialog *, BOOL> sendData;

	RenumberItem();

	if (phdn->iItem == 0) {
		d1				= !d1;
		sendData.first	= this;
		sendData.second = d1;
		m_List.SortItems( CompareFunc ,  (LPARAM) &sendData );
	} else if (phdn->iItem == 1) {
		d2				= !d2;
		sendData.first	= this;
		sendData.second = d2;
		m_List.SortItems( CompareFunc2 ,  (LPARAM) &sendData );
	}

	*pResult = 0;
}



// ��P�����Ƀ��C���h�J�[�h���܂ޕ�������w�肷��B
// �Q�̕����񂪈�v����ΐ^�A�����Ȃ��΋U��Ԃ��B
int CMyDialog::StrMatch( const char *ptn, const char *str )
{
	switch (*ptn) {
	case '\0':	return '\0' == *str;
	case '*':	return StrMatch( ptn + 1, str ) || ( ('\0' != *str) && StrMatch( ptn, str + 1 ) );
	case '?':	return ('\0' != *str) && StrMatch( ptn + 1, str + 1 );
	default:	return ( (unsigned char) *ptn == (unsigned char) *str ) && StrMatch( ptn + 1, str + 1 );
	}
}



void CMyDialog::OnClose()
{
	WriteProfile();			//+++ ini�t�@�C���ւ̏����o��

	// OnInitDialog��Attach�������̂�Detach����
	if (m_cmbFrame.m_hWnd)
		m_cmbFrame.Detach();
	if (m_cmbExt.m_hWnd)
		m_cmbExt.Detach();
	if (m_cmbNExt.m_hWnd)
		m_cmbNExt.Detach();
	if (m_List.m_hWnd)
		m_List.Detach();
	CDialog::OnClose();
}



///+++ ini�t�@�C���ւ̏����o��
void CMyDialog::WriteProfile()
{
	m_nSbtnDownloader = m_cmbSbtnSel.GetCurSel();
	TCHAR numb[16];
	::wsprintf(numb, _T("%d"), m_nSbtnDownloader);
	::WritePrivateProfileString(_T("etc"), _T("DefaultDownloader"), numb, m_iniFile);
	::wsprintf(numb, _T("%d"), m_Selected.GetCheck() != 0);
	::WritePrivateProfileString(_T("etc"), _T("Selected"), numb, m_iniFile);
	::wsprintf(numb, _T("%d"), m_ImageChk.GetCheck() != 0);
	::WritePrivateProfileString(_T("etc"), _T("ImageCheck"), numb, m_iniFile);
	::wsprintf(numb, _T("%d"), m_chkSbtn.GetCheck() != 0);
	::WritePrivateProfileString(_T("etc"), _T("S_Button"), numb, m_iniFile);
	bool bChkSave = m_chkSave.GetCheck() != 0;
	::wsprintf(numb, _T("%d"), bChkSave);
	::WritePrivateProfileString(_T("etc"), _T("SaveSearchData"), numb, m_iniFile);

	CRect rect;
	GetWindowRect(&rect);
	::wsprintf(numb, _T("%d"), rect.Width());
	::WritePrivateProfileString(_T("etc"), _T("Width"), numb, m_iniFile);
	::wsprintf(numb, _T("%d"), rect.Height());
	::WritePrivateProfileString(_T("etc"), _T("Height"), numb, m_iniFile);

	//+++ �����������Z�[�u���Ă����ꍇ.
	bool bRegex = (bChkSave) ? m_chkRegex.GetCheck() != 0 : 0;
	::wsprintf(numb, _T("%d"), bRegex);
	::WritePrivateProfileString(_T("SearchData"), _T("Regex"), numb, m_iniFile);
	CString str;
	if (bChkSave)
		m_cmbExt.GetWindowText(str);
	::WritePrivateProfileString(_T("SearchData"), _T("Ext"), str, m_iniFile);

	if (bChkSave)
		m_cmbNExt.GetWindowText(str);
	::WritePrivateProfileString(_T("SearchData"), _T("NExt"), str, m_iniFile);

	if (bChkSave)
		m_editSearch.GetWindowText(str);
	::WritePrivateProfileString(_T("SearchData"), _T("SearchStr"), str, m_iniFile);
}



BOOL CMyDialog::CheckSelectedUse()
{
	return m_Selected.GetCheck() != 0;
}



BOOL CMyDialog::IsSelected(int index)
{
	return m_List.GetItemState(index, LVIS_SELECTED);
}



void CMyDialog::OnSelected()
{
	AdjustButtonState();
}



void CMyDialog::OnSetFocusList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	AdjustButtonState();
	*pResult = 0;
}



void CMyDialog::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	AdjustButtonState();
	*pResult = 0;
}



void CMyDialog::OnImgExtract()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnSelChangeFrameCmb();
}



void CMyDialog::OnSelectedItemOpen()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	OnOpen_Sub(TRUE, TRUE);
}



void CMyDialog::OnAllItemOpen()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	OnOpen_Sub(TRUE, FALSE);
}



// ===========================================================================

#ifndef OLD_DOWNLOADER

void CMyDialog::InitDownloader()
{
	if (m_cmbSbtnSel.m_hWnd == 0) {
		HWND	hWnd   = GetDlgItem(IDC_CMB_SBTN_SEL)->m_hWnd;
		if (hWnd)
			m_cmbSbtnSel.Attach(hWnd);
	}

	m_downloader.clear();
	const int			BtnCount = 6;
	for (unsigned i = 0; i < BtnCount; ++i) {
		TCHAR	numb[16];
		sprintf(numb, "%d", i);
		TCHAR	buf[0x1000];
		buf[0] = 0;
		::GetPrivateProfileString(_T("Downloader"), numb, _T(""), buf, sizeof buf, m_iniFile);
		CString str = buf;
		CString fname = m_dir + str;
		//if (Misc::IsExistFile(fname))
		{
			m_downloader.push_back( str );
			m_cmbSbtnSel.AddString( Misc::GetFileNameNoExt(str) );
		}
	}

	int	n 			 = (int)m_downloader.size();
	m_downloaderSize = n;
	if (n == 0) {
		//x m_downloader.push_back( CString("�\�\") );
		m_cmbSbtnSel.AddString( CString(_T("�\�\")) );
	}

	if (m_nSbtnDownloader >= n) {
		m_nSbtnDownloader = n - (n > 0);
	}
	m_cmbSbtnSel.SetCurSel(m_nSbtnDownloader);

	static const DWORD	Btns[]   = {
		IDC_IRVINE, IDC_IRIA, IDC_FLASHGET, IDC_DCSAKURA, IDC_NETTRANS, IDC_REGET,
	};
	for (unsigned i = 0; i < n; ++i) {
		GetDlgItem(Btns[i])->SetWindowText( Misc::GetFileNameNoExt(m_downloader[i]) );
	}
	for (unsigned i = n; i < BtnCount; ++i) {
		m_downloader.push_back( CString(_T("�\�\")) );
		GetDlgItem(Btns[i])->SetWindowText( CString(_T("�\�\")) );
		GetDlgItem(Btns[i])->EnableWindow( FALSE );
	}
}



///+++
void CMyDialog::SendToDownloader()
{
	if (m_autoDownloader && m_nSbtnDownloader < m_downloaderSize) {
		DoFilter();
		DoDownloader( m_downloader[ m_nSbtnDownloader ] );
		m_autoDownloader = 0;
		SendMessage(WM_CLOSE, 0, 0);	//	OnClose();
	}
}



void CMyDialog::OnIrvine()   { if (0 < m_downloaderSize) DoDownloader( m_downloader[0] ); }
void CMyDialog::OnIria()     { if (1 < m_downloaderSize) DoDownloader( m_downloader[1] ); }
void CMyDialog::OnFlashGet() { if (2 < m_downloaderSize) DoDownloader( m_downloader[2] ); }
void CMyDialog::OnDcSakura() { if (3 < m_downloaderSize) DoDownloader( m_downloader[3] ); }
void CMyDialog::OnNetTrans() { if (4 < m_downloaderSize) DoDownloader( m_downloader[4] ); }
void CMyDialog::OnReget()    { if (5 < m_downloaderSize) DoDownloader( m_downloader[5] ); }



void	CMyDialog::DoDownloader( const CString& name )
{
	if (name.IsEmpty() || name == _T("�\�\"))
		return;

	if (name.CompareNoCase(_T("Irvine")) == 0) {
		DoIrvine();
		return;
	} else if (name.CompareNoCase(_T("Iria")) == 0) {
		DoIria();
		return;
	} else if (name.CompareNoCase(_T("DcSakura")) == 0) {
		DoDcSakura();
		return;
	}

	if ( FAILED( CoInitialize(0) ) ) {
		MessageBox("�������Ɏ��s���܂����B");
		return;
	}

	int 			 ct;
	if ( CheckSelectedUse() )
		ct = m_List.GetSelectedCount();
	else
		ct = m_List.GetItemCount();

	CString	flist = m_dir + _T("tmpfile.tmp");
	if (SaveURLList(flist) == false) {
		MessageBox("�����ɕK�v�Ȉꎞ�t�@�C���̐����Ɏ��s���܂����B");
		CoUninitialize();
		return;
	}

	CString		fscript	= m_dir + name;
	CString		strCt   = Misc::StrFmt(_T("%d"), ct);
	CString		tmp		= flist + _T(" ") + strCt;

	SHELLEXECUTEINFO 	sinfo;
	sinfo.cbSize	   = sizeof (SHELLEXECUTEINFO);
	sinfo.fMask 	   = SEE_MASK_NOCLOSEPROCESS;
	sinfo.hwnd		   = m_hWnd;
	sinfo.lpVerb	   = _T("open");
	sinfo.lpFile	   = fscript;
	sinfo.lpParameters = tmp;
	sinfo.lpDirectory  = NULL;
	sinfo.nShow 	   = SW_SHOWNORMAL;
	ShellExecuteEx(&sinfo);

	WaitForSingleObject(sinfo.hProcess, INFINITE);

	if (DeleteFile( flist ) == 0) {
		tmp = flist + "\n�ꎞ�t�@�C���̍폜�Ɏ��s���܂����B";
		MessageBox( tmp );
	}

	CoUninitialize();
}


void CMyDialog::DoIrvine()
{
  #ifdef USE_IRVINE
	if ( FAILED( CoInitialize(0) ) ) {
		MessageBox("�������Ɏ��s���܂����B");
		return;
	}

	IApi * irvine = 0;
	if ( FAILED( ::CoCreateInstance(CLSID_Api, NULL, CLSCTX_SERVER, IID_IApi, (void **) &irvine) ) ) {
		MessageBox("Irvine�̃C���^�[�t�F�[�X�擾�Ɏ��s���܂����B");
		CoUninitialize();
		return;
	}

	USES_CONVERSION;

	string str;
	int    ct = m_List.GetItemCount();

	for (int i = 0; i < ct; i++) {
		if (CheckSelectedUse() && IsSelected(i) == FALSE)
			continue;

		char   buf[BUF_SIZE];
		m_List.GetItemText(i, 0, buf, BUF_SIZE);
		str += buf;
		str += "\r\n";
	}

	irvine->AddUrl (T2OLE( str.c_str() ), 1);
	irvine->Release();

	CoUninitialize();
  #endif
}



void CMyDialog::DoIria()
{
  #ifdef USE_IRIA
	if ( FAILED( CoInitialize(0) ) ) {
		MessageBox("�������Ɏ��s���܂����B");
		return;
	}

	IIriaApi*	iria = 0;
	if ( FAILED( ::CoCreateInstance(CLSID_IriaApi, NULL, CLSCTX_SERVER, IID_IIriaApi, (void **) &iria) ) ) {
		MessageBox("Iria�̃C���^�[�t�F�[�X�擾�Ɏ��s���܂����B");
		CoUninitialize();
		return;
	}

	USES_CONVERSION;

	string	  str;
	int 	  ct = m_List.GetItemCount();

	for (int i = 0; i < ct; i++) {
		if (CheckSelectedUse() && IsSelected(i) == FALSE)
			continue;

		char	  buf[BUF_SIZE];
		m_List.GetItemText(i, 0, buf, BUF_SIZE);
		str += buf;
		str += "\r\n";
	}

	iria->AddUrl (T2OLE( str.c_str() ), 1);
	iria->Release();

	CoUninitialize();
  #endif
}



void CMyDialog::DoDcSakura()
{
  #ifdef USE_DCSAKURA
	if ( FAILED( CoInitialize(0) ) ) {
		MessageBox("�������Ɏ��s���܂����B");
		return;
	}

	IDCSakuraAPI *sakura = 0;
	if ( FAILED( ::CoCreateInstance(CLSID_DCSakuraAPI, NULL, CLSCTX_SERVER, IID_IDCSakuraAPI, (void **) &sakura) ) ) {
		MessageBox("DCSakura�̃C���^�[�t�F�[�X�擾�Ɏ��s���܂����B");
		CoUninitialize();
		return;
	}

	USES_CONVERSION;

	string		  str;
	int 		  ct  = m_List.GetItemCount();
	for (int i = 0; i < ct; i++) {
		if (CheckSelectedUse() && IsSelected(i) == FALSE)
			continue;

		char	buf[BUF_SIZE];
		m_List.GetItemText(i, 0, buf, BUF_SIZE);
		str += buf;
		str += "\r\n";
	}

	sakura->GetUrlsToImport( T2OLE( str.c_str() ) );
	sakura->Release();

	CoUninitialize();
  #endif
}



#else


void CMyDialog::InitDownloader()
{
	static const TCHAR *dwld[] = {_T("Irvine"),_T("Iria"),_T("FlashGet"),_T("DcSakura"),_T("NetTran")};
	if (m_cmbSbtnSel.m_hWnd == 0) {
		HWND	hWnd   = GetDlgItem(IDC_CMB_SBTN_SEL)->m_hWnd;
		if (hWnd)
			m_cmbSbtnSel.Attach(hWnd);
	}
   #ifndef WIN64
	enum { N = 5 };
   #else
	enum { N = 2 };
   #endif
	for (unsigned i = 0; i < N; ++i) {
		m_cmbSbtnSel.AddString(dwld[i]);
	}
	m_cmbSbtnSel.SetCurSel(m_nSbtnDownloader);
	//m_cmbExt.SetItemHeight(0, 1000);
}


///+++
void CMyDialog::SendToDownloader()
{
	if (m_autoDownloader) {
		DoFilter();
		switch (m_nSbtnDownloader) {
		case 0: OnIrvine();		break;
		case 1: OnIria();		break;
		case 2: OnFlashGet();	break;
		case 3: OnDcSakura();	break;
		case 4: OnNetTrans();	break;
		default: assert(0);
		}

		m_autoDownloader = 0;
		SendMessage(WM_CLOSE, 0, 0);	//	OnClose();
	}
}



//�_�E�����[�_�ւ�URL�n��
//���̋@�\��L���ɂ���ɂ�MyDialog.h�Ń^�C�v���C�u�����̃C���|�[�g��ݒ肵
//�ȉ��̃R�����g���������Ă�������

void CMyDialog::OnIria()
{
  #ifdef USE_IRIA
	if ( FAILED( CoInitialize(0) ) ) {
		MessageBox("�������Ɏ��s���܂����B");
		return;
	}

	IIriaApi*	iria = 0;
	if ( FAILED( ::CoCreateInstance(CLSID_IriaApi, NULL, CLSCTX_SERVER, IID_IIriaApi, (void **) &iria) ) ) {
		MessageBox("Iria�̃C���^�[�t�F�[�X�擾�Ɏ��s���܂����B");
		CoUninitialize();
		return;
	}

	USES_CONVERSION;

	string	  str;
	int 	  ct = m_List.GetItemCount();

	for (int i = 0; i < ct; i++) {
		if (CheckSelectedUse() && IsSelected(i) == FALSE)
			continue;

		char	  buf[BUF_SIZE];
		m_List.GetItemText(i, 0, buf, BUF_SIZE);
		str += buf;
		str += "\r\n";
	}

	iria->AddUrl (T2OLE( str.c_str() ), 1);
	iria->Release();

	CoUninitialize();
  #endif
}



void CMyDialog::OnIrvine()
{
  #ifdef USE_IRVINE
	if ( FAILED( CoInitialize(0) ) ) {
		MessageBox("�������Ɏ��s���܂����B");
		return;
	}

	IApi * irvine = 0;
	if ( FAILED( ::CoCreateInstance(CLSID_Api, NULL, CLSCTX_SERVER, IID_IApi, (void **) &irvine) ) ) {
		MessageBox("Irvine�̃C���^�[�t�F�[�X�擾�Ɏ��s���܂����B");
		CoUninitialize();
		return;
	}

	USES_CONVERSION;

	string str;
	int    ct = m_List.GetItemCount();

	for (int i = 0; i < ct; i++) {
		if (CheckSelectedUse() && IsSelected(i) == FALSE)
			continue;

		char   buf[BUF_SIZE];
		m_List.GetItemText(i, 0, buf, BUF_SIZE);
		str += buf;
		str += "\r\n";
	}

	irvine->AddUrl (T2OLE( str.c_str() ), 1);
	irvine->Release();

	CoUninitialize();
  #endif
}



void CMyDialog::OnDcSakura()
{
  #ifdef USE_DCSAKURA

	if ( FAILED( CoInitialize(0) ) ) {
		MessageBox("�������Ɏ��s���܂����B");
		return;
	}

	IDCSakuraAPI *sakura = 0;
	if ( FAILED( ::CoCreateInstance(CLSID_DCSakuraAPI, NULL, CLSCTX_SERVER, IID_IDCSakuraAPI, (void **) &sakura) ) ) {
		MessageBox("DCSakura�̃C���^�[�t�F�[�X�擾�Ɏ��s���܂����B");
		CoUninitialize();
		return;
	}

	USES_CONVERSION;

	string		  str;
	int 		  ct  = m_List.GetItemCount();
	for (int i = 0; i < ct; i++) {
		if (CheckSelectedUse() && IsSelected(i) == FALSE)
			continue;

		char	buf[BUF_SIZE];
		m_List.GetItemText(i, 0, buf, BUF_SIZE);
		str += buf;
		str += "\r\n";
	}

	sakura->GetUrlsToImport( T2OLE( str.c_str() ) );
	sakura->Release();

	CoUninitialize();
  #endif
}



void CMyDialog::OnFlashGet()
{
  #ifdef USE_FLASHGET
	if ( FAILED( CoInitialize(0) ) ) {
		MessageBox("�������Ɏ��s���܂����B");
		return;
	}

	IJetCarNetscape*	flash = 0;
	if ( FAILED( ::CoCreateInstance(CLSID_JetCarNetscape, NULL, CLSCTX_SERVER, IID_IJetCarNetscape, (void **) &flash) ) ) {
		MessageBox("FlashGet�̃C���^�[�t�F�[�X�擾�Ɏ��s���܂����B");
		CoUninitialize();
		return;
	}

	int 			 ct;
	if ( CheckSelectedUse() )
		ct = m_List.GetSelectedCount();
	else
		ct = m_List.GetItemCount();

	string		flist;
	GetTempName(&flist, "txt");
	if (SaveURLList(flist) == false) {
		MessageBox("�����ɕK�v�Ȉꎞ�t�@�C���̐����Ɏ��s���܂����B");
		return;
	}

	string		fscript;
	GetTempName(&fscript, "vbs");
	FILE *		fp = std::fopen(fscript.c_str(), "w");
	if (fp == NULL) {
		MessageBox("�����ɕK�v�Ȉꎞ�t�@�C���̐����Ɏ��s���܂����B");
		DeleteFile( flist.c_str() );
		return;
	}

	static const char script[14][70] = {
		"On Error Resume Next",
		"Set objArg = WScript.Arguments",
		"set JetCarCatch = CreateObject(\"JetCar.Netscape\")",
		"If err=0 And objArg.Count >= 2 Then ",
		"	Set objFileSystem = CreateObject(\"Scripting.FileSystemObject\")", //70�������肬��
		"	Set objFile 	  = objFileSystem.OpenTextFile(objArg(0))",
		"	Redim ary(objArg(1)*2)",
		"	ary(0) = \"\"",
		"	For i=0 To objArg(1)-1",
		"		ary(i*2+1) = objFile.ReadLine",
		"		ary(i*2+2) = CStr(i)",
		"	Next",
		"	JetCarCatch.AddUrlList ary",
		"End If",
	};

	for (int i = 0; i < 14; i++) {
		fputs(script[i], fp);
		fwrite("\n", 1, 1, fp);
	}
	std::fclose(fp);

	char		aa[16];
	std::sprintf(aa, "%d", ct);
	string		tmp		= flist + " " + aa;

	SHELLEXECUTEINFO 	sinfo;
	sinfo.cbSize	   = sizeof (SHELLEXECUTEINFO);
	sinfo.fMask 	   = SEE_MASK_NOCLOSEPROCESS;
	sinfo.hwnd		   = m_hWnd;
	sinfo.lpVerb	   = "open";
	sinfo.lpFile	   = fscript.c_str();
	sinfo.lpParameters = tmp.c_str();
	sinfo.lpDirectory  = NULL;
	sinfo.nShow 	   = SW_SHOWNORMAL;
	ShellExecuteEx(&sinfo);

	WaitForSingleObject(sinfo.hProcess, INFINITE);

	if (DeleteFile( flist.c_str() ) == 0) {
		tmp = flist + "\n�ꎞ�t�@�C���̍폜�Ɏ��s���܂����B";
		MessageBox( tmp.c_str() );
	}

	if (DeleteFile( fscript.c_str() ) == 0) {
		tmp = fscript + "\n�ꎞ�t�@�C���̍폜�Ɏ��s���܂����B";
		MessageBox( tmp.c_str() );
	}

	CoUninitialize();
  #endif
}



void CMyDialog::OnNetTrans()
{
  #ifdef USE_NETTRANS
	int 			 ct = m_List.GetItemCount();

	string			flist;
	GetTempName(&flist, "txt");
	if (SaveURLList(flist) == false) {
		MessageBox("�����ɕK�v�Ȉꎞ�t�@�C���̐����Ɏ��s���܂����B");
		return;
	}

	string			fscript;
	GetTempName(&fscript, "vbs");
	FILE *			 fp	= std::fopen(fscript.c_str(), "w");
	if (fp == NULL) {
		MessageBox("�����ɕK�v�Ȉꎞ�t�@�C���̐����Ɏ��s���܂����B");
		DeleteFile( flist.c_str() );
		return;
	}

	static const char	script[14][70] = {
		"On Error Resume Next",
		"Set objArg = WScript.Arguments",
		"set NTIECatcher = CreateObject(\"NTIEHelper.NTIEAddUrl\")",
		"If err=0 And objArg.Count >= 2 Then ",
		"	Set objFileSystem = CreateObject(\"Scripting.FileSystemObject\")", //70�������肬��
		"	Set objFile 	  = objFileSystem.OpenTextFile(objArg(0))",
		"	Redim ary(objArg(1))",
		"	Redim mark(objArg(1))",
		"	For i=0 To objArg(1)-1",
		"		ary(i) = objFile.ReadLine",
		"		mark(i) = CStr(i)",
		"	Next",
		"	call NTIECatcher.AddList(\"\", ary(0), mark(0) )",
		"End If",
	};
	for (int i = 0; i < 14; i++) {
		fputs(script[i], fp);
		fwrite("\n", 1, 1, fp);
	}

	std::fclose(fp);

	char		aa[16];
	std::sprintf(aa, "%d", ct);
	string		tmp		= flist + " " + aa;

	SHELLEXECUTEINFO sinfo;
	sinfo.cbSize	   = sizeof (SHELLEXECUTEINFO);
	sinfo.fMask 	   = SEE_MASK_NOCLOSEPROCESS;
	sinfo.hwnd		   = m_hWnd;
	sinfo.lpVerb	   = "open";
	sinfo.lpFile	   = fscript.c_str();
	sinfo.lpParameters = tmp.c_str();
	sinfo.lpDirectory  = NULL;
	sinfo.nShow 	   = SW_SHOWNORMAL;
	ShellExecuteEx(&sinfo);

	WaitForSingleObject(sinfo.hProcess, INFINITE);

	if (DeleteFile( flist.c_str() ) == 0) {
		tmp = flist + "\n�ꎞ�t�@�C���̍폜�Ɏ��s���܂����B";
		MessageBox( tmp.c_str() );
	}

	if (DeleteFile( fscript.c_str() ) == 0) {
		tmp = fscript + "\n�ꎞ�t�@�C���̍폜�Ɏ��s���܂����B";
		MessageBox( tmp.c_str() );
	}

	CoUninitialize();
  #endif
}



bool CMyDialog::GetTempName(string *str, const char *ext)
{
	char	buf[MAX_PATH];
	if (GetTempPath(MAX_PATH, buf) == 0)
		return false;

  #if 0
	char   	fn[MAX_PATH];
	GetTempFileName(buf, "url", 1, fn);
  #endif

	for (int i = 0; i < 1000; i++) {
		char   	aa[16];
		::sprintf(aa, "%d.", i);
		string 	tmp  = buf;		//+++ buf�łȂ� fn ����Ȃ��񂾂낤��...
		tmp += aa;
		tmp += ext;

		if (GetFileAttributes( tmp.c_str() ) == 0xFFFFFFFF) {
			*str = tmp;
			return true;
		}
	}

	return false;
}



#endif
