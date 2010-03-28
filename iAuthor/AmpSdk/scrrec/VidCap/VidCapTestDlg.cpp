// VidCapTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VidCapTest.h"
#include "VidCapTestDlg.h"

#include <windowsx.h>

#include "VSCap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVidCapTestDlg dialog

CVidCapTestDlg::CVidCapTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVidCapTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVidCapTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVidCapTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVidCapTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVidCapTestDlg, CDialog)
	//{{AFX_MSG_MAP(CVidCapTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_HIDE_CURSOR, OnRadioHideCursor)
	ON_BN_CLICKED(IDC_RADIO_SHOW_CURSOR, OnRadioShowCursor)
	ON_BN_CLICKED(ID_ABOUT, OnAbout)
	ON_BN_CLICKED(IDRECORD_START, OnRecStart)
	ON_BN_CLICKED(IDRECORD_STOP, OnRecStop)
	ON_BN_CLICKED(IDRECORD_PAUSE, OnRecPause)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVidCapTestDlg message handlers

BOOL CVidCapTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	HDC hScreenDC = ::GetDC(NULL);	
	m_ScreenBits = ::GetDeviceCaps(hScreenDC, BITSPIXEL );	
	//nColors = bits;
	::ReleaseDC(NULL,hScreenDC);		

	VSCap::Initialize();

	InitVideoCompressorList();
	OnRadioShowCursor();
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVidCapTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVidCapTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVidCapTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVidCapTestDlg::OnRadioHideCursor() 
{
	m_bRecordCursor = false;

	((CButton *) GetDlgItem(IDC_RADIO_HIDE_CURSOR))->SetCheck(TRUE);
	((CButton *) GetDlgItem(IDC_RADIO_SHOW_CURSOR))->SetCheck(FALSE);
}

void CVidCapTestDlg::OnRadioShowCursor() 
{
	m_bRecordCursor = true;

	((CButton *) GetDlgItem(IDC_RADIO_HIDE_CURSOR))->SetCheck(FALSE);
	((CButton *) GetDlgItem(IDC_RADIO_SHOW_CURSOR))->SetCheck(TRUE);	
}

void CVidCapTestDlg::RefreshCompressorButtons() 
{

	int sel = ((CComboBox *) GetDlgItem(IDC_COMPRESSORS))->GetCurSel();

	if (sel != CB_ERR)  {
		HIC hic = ICOpen(m_pCompressorInfo[sel].fccType, m_pCompressorInfo[sel].fccHandler, ICMODE_QUERY);
		if (hic) {
			
			if (ICQueryAbout(hic)) 
				((CButton *) GetDlgItem(ID_ABOUT))->EnableWindow(TRUE);
			else
				((CButton *) GetDlgItem(ID_ABOUT))->EnableWindow(FALSE);

			//if (ICQueryConfigure(hic)) 
			//	((CButton *) GetDlgItem(ID_CONFIGURE))->EnableWindow(TRUE);
			//else
			//	((CButton *) GetDlgItem(ID_CONFIGURE))->EnableWindow(FALSE);	
			
			ICClose(hic);
		}
	}
}

void CVidCapTestDlg::InitVideoCompressorList()
{
	m_CompFCCHandler = mmioFOURCC('M', 'S', 'V', 'C');;
	m_pCompressorInfo = NULL;
	m_nSelectedCompressor = -1;

	//Capture a frame and use it to determine compatitble compressors for user to select
	
	// retrieve the compressor list from the system 
	//

	LPBITMAPINFOHEADER first_alpbi = NULL;

	COMPVARS compVars;
	compVars.cbSize = sizeof(COMPVARS);     // validate it
    compVars.dwFlags = 0;	

	int top=0;
	int left=0;
	int width=320;
	int height=200;

    first_alpbi=VSCap::CaptureScreenFrame(left,top,width, height,1);		

	m_nCompressorNumber =0;
	if (m_pCompressorInfo)	free(m_pCompressorInfo);
	m_pCompressorInfo = (ICINFO *) calloc(30,sizeof(ICINFO));

	for(int i=0; ICInfo(ICTYPE_VIDEO, i, &m_pCompressorInfo[m_nCompressorNumber]); i++) {


		if (m_nCompressorNumber>=30) break; //maximum allows 30 compressors		
		
		HIC hic;	

		hic = ICOpen(m_pCompressorInfo[m_nCompressorNumber].fccType, m_pCompressorInfo[m_nCompressorNumber].fccHandler, ICMODE_QUERY);
		if (hic) {
			
			if   (ICERR_OK==ICCompressQuery(hic, first_alpbi, NULL)) {
						
				ICGetInfo(hic, &m_pCompressorInfo[m_nCompressorNumber], sizeof(ICINFO));				
				m_nCompressorNumber++ ;				
				
			}
			ICClose(hic);
		}
	}
	
	
	SAFE_RELEASE_GLOBAL(first_alpbi);

	// populate the compressor list
	//
	if (m_nCompressorNumber > 0)
	{

		int sel = -1;
		for (int i=0; i<m_nCompressorNumber; i++)
		{

			CString cname(m_pCompressorInfo[i].szDescription);
			((CComboBox *) GetDlgItem(IDC_COMPRESSORS))->AddString(cname);
			
			if (m_CompFCCHandler == m_pCompressorInfo[i].fccHandler)
				sel = i;
		}


		if (sel == -1)
		{
			sel = 0;
			m_CompFCCHandler = m_pCompressorInfo[sel].fccHandler;
		}

		
		((CComboBox *) GetDlgItem(IDC_COMPRESSORS))->SetCurSel(sel);

		RefreshCompressorButtons(); 
		
	}	
}


void CVidCapTestDlg::OnAbout() 
{
	int sel = ((CComboBox *) GetDlgItem(IDC_COMPRESSORS))->GetCurSel();
	if (sel != CB_ERR)  {
		
	
		HIC hic = ICOpen(m_pCompressorInfo[sel].fccType, m_pCompressorInfo[sel].fccHandler, ICMODE_QUERY);
		if (hic) {			
				
			ICAbout(hic,m_hWnd);			
			ICClose(hic);
		}
	}
}

void CVidCapTestDlg::OnRecStart() 
{
	// TODO: Add your control notification handler code here
	if (VSCap::GetRecordState() == VSCap::Paused)
	{
		VSCap::RecordResume();
	}
	else
	{
		HDC hScreenDC = ::GetDC(NULL);
		int nScreenWidth  = ::GetDeviceCaps(hScreenDC, HORZRES);
		int nScreenHeight = ::GetDeviceCaps(hScreenDC, VERTRES);
		::ReleaseDC(NULL, hScreenDC);


		int	sel = ((CComboBox *) GetDlgItem(IDC_COMPRESSORS))->GetCurSel();
		m_CompFCCHandler = m_pCompressorInfo[sel].fccHandler;
		
		VSCap::SetVidComp(m_CompFCCHandler);
		VSCap::SetAudComp(VSC_ACM_GSM610);	// VSC_ACM_PCM , VSC_ACM_GSM610
		
		VSCap::SetSourceRect(0,0, nScreenWidth, nScreenHeight);
		VSCap::SetTargetSize(nScreenWidth, nScreenHeight);
		VSCap::RecordStart(_T("c:\\aa.avi"), 15, 5, m_bRecordCursor);
	}
}

void CVidCapTestDlg::OnRecPause() 
{
	VSCap::RecordPause();
}

void CVidCapTestDlg::OnRecStop() 
{
	VSCap::RecordStop();
}


void CVidCapTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	free(m_pCompressorInfo);
	
	VSCap::UnInitialize();
}
