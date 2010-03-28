// ProdCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProdCode.h"
#include "ProdCodeDlg.h"
#include "ProductCode.h"

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
// CProdCodeDlg dialog

CProdCodeDlg::CProdCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProdCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProdCodeDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProdCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProdCodeDlg)
	DDX_Control(pDX, IDC_PROD_ENO, m_wndProdEno);
	DDX_Control(pDX, IDC_PROD_SNO, m_wndProdSno);
	DDX_Control(pDX, IDC_PROD_PREFIX, m_wndProdPrefix);
	DDX_Control(pDX, IDC_LIST_CODE, m_wndListCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProdCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CProdCodeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_CHECK_CRC, OnCheckCRC)
	ON_BN_CLICKED(IDC_CHECK_DUPLICATE, OnCheckDuplicate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProdCodeDlg message handlers

BOOL CProdCodeDlg::OnInitDialog()
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
	m_wndProdPrefix.SetWindowText("MBOEM");
	m_wndProdSno.SetWindowText("1");
	m_wndProdEno.SetWindowText("256");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProdCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProdCodeDlg::OnPaint() 
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
HCURSOR CProdCodeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CProdCodeDlg::OnOK() 
{
	CWaitCursor wait;
	CProductCode pc;
	TCHAR szPrefix[60];
	TCHAR szSno[60];
	TCHAR szEno[60];

	m_wndProdPrefix.GetWindowText(szPrefix, 50);
	m_wndProdSno.GetWindowText(szSno, 50);
	m_wndProdEno.GetWindowText(szEno, 50);

	int sno = atoi(szSno);
	int cnt = atoi(szEno) - sno + 1;

	m_CodeList.RemoveAll();
	pc.GenerateCodeList(szPrefix, sno, cnt, &m_CodeList);


	int nItemCount = m_CodeList.GetCount();
	if (nItemCount <= 0)
		return;

	//
	// Build code list
	//

	POSITION pos = m_CodeList.GetHeadPosition();

	CString strPCode = m_CodeList.GetAt(pos);
	int nPCodeLen = strPCode.GetLength()+2;
	int nBuffLen = nItemCount * nPCodeLen + 1;
	TCHAR *pszCodeList = new TCHAR[nItemCount * nPCodeLen + 1];
	memset((void *) pszCodeList, 0, nBuffLen);

	int idx = 0;
	for (int i = 0; i < nItemCount; i++)
	{
		strPCode = m_CodeList.GetNext(pos); strPCode += _T("\r\n");
		_tcsncpy(&pszCodeList[i * nPCodeLen], (LPCTSTR) strPCode, strPCode.GetLength());
		
	}
	m_wndListCode.SetWindowText(pszCodeList);
}

void CProdCodeDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CProdCodeDlg::OnCopy() 
{
	// Copy all of the text to the clipboard.
	m_wndListCode.SetSel(0, -1);
	m_wndListCode.Copy();
}

void CProdCodeDlg::OnCheckCRC() 
{
	CWaitCursor wait;
	CProductCode pc;

	CString strPrefix;
	m_wndProdPrefix.GetWindowText(strPrefix);
	int nPrefixLen = strPrefix.GetLength();

	CString msg;
	int nLineCount = m_wndListCode.GetLineCount()-1;
  
	if (nLineCount <= 0)
		return;

	TCHAR *pszLineBuff = new TCHAR[100];
	for (int i = 0; i < nLineCount-1; i++)
	{
		memset((void *) pszLineBuff, 0, 100);
		m_wndListCode.GetLine(i, pszLineBuff, 100);

		if (!pc.CheckCode(pszLineBuff, nPrefixLen+5, _tcslen(pszLineBuff)))
		{
			msg.Format(_T("%s\n pcode: %s"), _T("Check CRC error!\n"), pszLineBuff);
			MessageBox(msg, _T("Error"), MB_OK);

			delete pszLineBuff;
			return;
		}
	}
	delete pszLineBuff;

	MessageBox(_T("Check CRC OK!\n"));
}

void CProdCodeDlg::OnCheckDuplicate() 
{
	CWaitCursor wait;
	CString msg;
	CString strItem, strLine;
	int nLineCount = m_wndListCode.GetLineCount()-1;
  
	for (int i = 0; i < nLineCount-1; i++)
	{
		m_wndListCode.GetLine(i, strLine.GetBuffer(m_wndListCode.LineLength(i)));
		strLine.ReleaseBuffer();

		for (int j = i+1; j < nLineCount; j++)
		{
			m_wndListCode.GetLine(j, strItem.GetBuffer(m_wndListCode.LineLength(i)));
			strItem.ReleaseBuffer();

			if (strItem == strLine)
			{
				CString msg;
				msg.Format(_T("%s\n pcode: %s"), _T("Check Duplication error!\n"), strLine);
				MessageBox(msg, _T("Error"), MB_OK);
				return;
			}
		}
	}

	MessageBox(_T("Check Duplication OK!\n"));
}
