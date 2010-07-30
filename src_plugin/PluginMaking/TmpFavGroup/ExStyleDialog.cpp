// ExStyleDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "TmpFavGroup.h"
#include "ExStyleDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExStyleDialog �_�C�A���O


CExStyleDialog::CExStyleDialog(CApiExStyle &style, CWnd *pParent /*=NULL*/)
	: CDialog(CExStyleDialog::IDD, pParent)
	  , m_ExStyle(style)
{
	//{{AFX_DATA_INIT(CExStyleDialog)
	//}}AFX_DATA_INIT

	//�ϐ��̏�����
	DWORD dwExStyle	= m_ExStyle.m_dwApiExStyle;

	m_bDLActiveX   	= dwExStyle & FLAG_SE_DLACTIVEXCTLS ? TRUE : FALSE;
	m_bImage	   	= dwExStyle & FLAG_SE_DLIMAGES 		? TRUE : FALSE;
	m_bJava 	   	= dwExStyle & FLAG_SE_JAVA 			? TRUE : FALSE;
	m_bNaviLock    	= dwExStyle & FLAG_SE_NAVIGATELOCK 	? TRUE : FALSE;
	m_bRunActiveX  	= dwExStyle & FLAG_SE_RUNACTIVEXCTLS? TRUE : FALSE;
	m_bScript	   	= dwExStyle & FLAG_SE_SCRIPTS 		? TRUE : FALSE;
	m_bSound	   	= dwExStyle & FLAG_SE_BGSOUNDS 		? TRUE : FALSE;
	m_bVideo	   	= dwExStyle & FLAG_SE_VIDEOS 		? TRUE : FALSE;
	m_bBlockMailTo 	= dwExStyle & FLAG_SE_BLOCKMAILTO 	? TRUE : FALSE;
	m_bGesture	   	= dwExStyle & FLAG_SE_MOUSEGESTURE 	? TRUE : FALSE;
	m_bFilter	   	= dwExStyle & FLAG_SE_MSGFILTER 	? TRUE : FALSE;

	int   nFlag		= (dwExStyle & FLAGS_SE_RELOAD) / FLAG_SE_REFRESH_NONE;
	m_nReload	   	= 0;

	while ( (nFlag >>= 1) > 0 )
		m_nReload++;
}


void CExStyleDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExStyleDialog)
	DDX_Check(pDX, IDC_CHECK_DLACTIVEXCTLS	, m_bDLActiveX	);
	DDX_Check(pDX, IDC_CHECK_IMAGE			, m_bImage		);
	DDX_Check(pDX, IDC_CHECK_JAVA			, m_bJava		);
	DDX_Check(pDX, IDC_CHECK_NAVILOCK		, m_bNaviLock	);
	DDX_Check(pDX, IDC_CHECK_RUNACTIVEXCTLS	, m_bRunActiveX	);
	DDX_Check(pDX, IDC_CHECK_SCRIPTS		, m_bScript		);
	DDX_Check(pDX, IDC_CHECK_SOUND			, m_bSound		);
	DDX_Check(pDX, IDC_CHECK_VIDEO			, m_bVideo		);
	DDX_Check(pDX, IDC_CHK_BLOCK_MAILTO		, m_bBlockMailTo);
	DDX_Check(pDX, IDC_CHK_MOUSE_GESTURE	, m_bGesture	);
	DDX_Check(pDX, IDC_CHK_MSG_FILTER		, m_bFilter		);
	DDX_Radio(pDX, IDC_RADIO_RELOAD_NONE	, m_nReload		);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExStyleDialog, CDialog)
//{{AFX_MSG_MAP(CExStyleDialog)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CExStyleDialog ���b�Z�[�W �n���h��

void CExStyleDialog::OnOK()
{
	UpdateData(TRUE);

	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	DWORD dwExStyle = 0;

	if (m_bImage)			dwExStyle |= FLAG_SE_DLIMAGES;
	if (m_bVideo)			dwExStyle |= FLAG_SE_VIDEOS;
	if (m_bSound)			dwExStyle |= FLAG_SE_BGSOUNDS;
	if (m_bRunActiveX)		dwExStyle |= FLAG_SE_RUNACTIVEXCTLS;
	if (m_bDLActiveX)		dwExStyle |= FLAG_SE_DLACTIVEXCTLS;
	if (m_bScript)			dwExStyle |= FLAG_SE_SCRIPTS;
	if (m_bJava)			dwExStyle |= FLAG_SE_JAVA;
	if (m_bNaviLock)		dwExStyle |= FLAG_SE_NAVIGATELOCK;
	if (m_bFilter)			dwExStyle |= FLAG_SE_MSGFILTER;
	if (m_bGesture)			dwExStyle |= FLAG_SE_MOUSEGESTURE;
	if (m_bBlockMailTo)		dwExStyle |= FLAG_SE_BLOCKMAILTO;
	if (m_nReload != -1)	dwExStyle |= (FLAG_SE_REFRESH_NONE << m_nReload);

	m_ExStyle.m_dwApiExStyle = dwExStyle; //�ύX���ꂽ��̏��������o�ϐ��ɕۑ�

	CDialog::OnOK();
}


void CExStyleDialog::OnCancel()
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	CDialog::OnCancel();
}

