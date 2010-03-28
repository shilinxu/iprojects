// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog


COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	DDX_Control(pDX, IDC_OPTION_UPBTN, m_UpBtn);
	DDX_Control(pDX, IDC_OPTION_DOWNBTN, m_DownBtn);
	DDX_Control(pDX, IDC_OPTION_CODEC, m_CodecBtn);
	DDX_Control(pDX, IDC_OPTION_CAMERA, m_CameraBtn);
	DDX_Control(pDX, IDC_OPTION_TEXTBTN, m_TextBtn);
	DDX_Control(pDX, IDC_OPTION_SOUNDBTN, m_SoundBtn);
	DDX_Control(pDX, IDC_OPTION_SNAPSHOTBTN, m_SnapshotBtn);
	DDX_Control(pDX, IDC_OPTION_LOGOBTN, m_LogoBtn);
	DDX_Control(pDX, IDC_OPTION_FOLDERBTN, m_FolderBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDC_OPTIONCLOSEBTN, m_CloseBtn);
	DDX_Control(pDX, IDC_OPTION_TOOLBARBTN, m_ToolbarBtn);
	DDX_Control(pDX, IDC_OPTION_RECORDBTN, m_RecordBtn);
	DDX_Control(pDX, IDC_OPTION_PENBTN, m_PenBtn);
	DDX_Control(pDX, IDC_OPTION_HOTKEYBTN, m_HotkeyBtn);
	DDX_Control(pDX, IDC_OPTION_ABOUTBTN, m_AboutBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_OPTION_TOOLBARBTN, OnOptionToolbarbtn)
	ON_BN_CLICKED(IDC_OPTION_RECORDBTN, OnOptionRecordbtn)
	ON_BN_CLICKED(IDC_OPTION_PENBTN, OnOptionPenbtn)
	ON_BN_CLICKED(IDC_OPTION_HOTKEYBTN, OnOptionHotkeybtn)
	ON_BN_CLICKED(IDC_OPTION_ABOUTBTN, OnOptionAboutbtn)
	ON_BN_CLICKED(IDC_OPTIONCLOSEBTN, OnOptionclosebtn)
	ON_BN_CLICKED(IDC_OPTION_FOLDERBTN, OnOptionFolderbtn)
	ON_BN_CLICKED(IDC_OPTION_LOGOBTN, OnOptionLogobtn)
	ON_BN_CLICKED(IDC_OPTION_SNAPSHOTBTN, OnOptionSnapshotbtn)
	ON_BN_CLICKED(IDC_OPTION_SOUNDBTN, OnOptionSoundbtn)
	ON_BN_CLICKED(IDC_OPTION_TEXTBTN, OnOptionTextbtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPTION_CAMERA, OnOptionCamera)
	ON_BN_CLICKED(IDC_OPTION_CODEC, OnOptionCodec)
	ON_BN_CLICKED(IDC_OPTION_UPBTN, OnOptionUpbtn)
	ON_BN_CLICKED(IDC_OPTION_DOWNBTN, OnOptionDownbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

int COptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	VERIFY(m_AboutDlg.Create( IDD_OPT_ABOUTDLG, this));
	m_AboutDlg.ShowWindow(SW_HIDE);

	VERIFY(m_HotkeyDlg.Create( IDD_OPT_HOTKEYDLG, this));
	m_HotkeyDlg.ShowWindow(SW_HIDE);

	VERIFY(m_PenFolderDlg.Create( IDD_OPT_PENFOLDERDLG, this));
	m_PenFolderDlg.ShowWindow(SW_HIDE);

	VERIFY(m_RECDlg.Create( IDD_OPT_RECDLG, this));
	m_RECDlg.ShowWindow(SW_HIDE);

	VERIFY(m_LogoDlg.Create( IDD_OPT_LOGODLG, this));
	m_LogoDlg.ShowWindow(SW_HIDE);

	VERIFY(m_FolderDlg.Create( IDD_OPT_FOLDERDLG, this));
	m_FolderDlg.ShowWindow(SW_HIDE);

	VERIFY(m_TextDlg.Create( IDD_OPT_TEXTDLG, this));
	m_TextDlg.ShowWindow(SW_HIDE);

	VERIFY(m_SoundDlg.Create( IDD_OPT_SOUNDDLG, this));
	m_SoundDlg.ShowWindow(SW_HIDE);	

	VERIFY(m_SnapshotDlg.Create( IDD_OPT_SNAPSHOTDLG, this));
	m_SnapshotDlg.ShowWindow(SW_HIDE);		

	VERIFY(m_CameraDlg.Create( IDD_OPT_CAMERADLG, this));
	m_CameraDlg.ShowWindow(SW_HIDE);
	
	VERIFY(m_CodecDlg.Create( IDD_OPT_CODECDLG, this));
	m_CodecDlg.ShowWindow(SW_HIDE);	

	VERIFY(m_ToolbarDlg.Create( IDD_OPT_TOOLBARDLG, this));	
	
	m_State = 0;

	return 0;
}


BOOL COptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_CloseBtn.DrawBorder(FALSE);
	m_CloseBtn.SetBitmaps(IDB_WMVCLOSEBTN, RGB(255,0,255));	
	m_CloseBtn.DrawTransparent();
	
	m_OkBtn.DrawBorder(FALSE);
	m_OkBtn.SetBitmaps(IDB_OPTIONOKBTN, RGB(255,0,255));	
	m_OkBtn.DrawTransparent();

	m_CancelBtn.DrawBorder(FALSE);
	m_CancelBtn.SetBitmaps(IDB_OPTIONCANCELBTN, RGB(255,0,255));	
	m_CancelBtn.DrawTransparent();

	m_CloseBtn.DrawBorder(FALSE);
	m_CloseBtn.SetBitmaps(IDB_WMVCLOSEBTN, RGB(255,0,255));	
	m_CloseBtn.DrawTransparent();

	m_ToolbarBtn.DrawBorder(FALSE);
	m_ToolbarBtn.Set3StateButton();
	m_ToolbarBtn.SetBitmaps(IDB_OPTION_TOOLBARBTN, RGB(255,0,255));	
	m_ToolbarBtn.DrawTransparent();
	m_ToolbarBtn.SetSelect();

	m_RecordBtn.DrawBorder(FALSE);
	m_RecordBtn.Set3StateButton();
	m_RecordBtn.SetBitmaps(IDB_OPTION_RECBTN, RGB(255,0,255));	
	m_RecordBtn.DrawTransparent();

	m_PenBtn.DrawBorder(FALSE);
	m_PenBtn.Set3StateButton();
	m_PenBtn.SetBitmaps(IDB_OPTION_PENBTN, RGB(255,0,255));	
	m_PenBtn.DrawTransparent();

	m_HotkeyBtn.DrawBorder(FALSE);
	m_HotkeyBtn.Set3StateButton();
	m_HotkeyBtn.SetBitmaps(IDB_OPTION_HOTKEYBTN, RGB(255,0,255));	
	m_HotkeyBtn.DrawTransparent();

	m_AboutBtn.DrawBorder(FALSE);
	m_AboutBtn.Set3StateButton();
	m_AboutBtn.SetBitmaps(IDB_OPTION_ABOUTBTN, RGB(255,0,255));	
	m_AboutBtn.DrawTransparent();

	m_TextBtn.DrawBorder(FALSE);
	m_TextBtn.Set3StateButton();
	m_TextBtn.SetBitmaps(IDB_OPTION_TEXT, RGB(255,0,255));	
	m_TextBtn.DrawTransparent();	

	m_SoundBtn.DrawBorder(FALSE);
	m_SoundBtn.Set3StateButton();
	m_SoundBtn.SetBitmaps(IDB_OPTION_SOUND, RGB(255,0,255));	
	m_SoundBtn.DrawTransparent();

	m_SnapshotBtn.DrawBorder(FALSE);
	m_SnapshotBtn.Set3StateButton();
	m_SnapshotBtn.SetBitmaps(IDB_OPTION_SNAPSHOT, RGB(255,0,255));	
	m_SnapshotBtn.DrawTransparent();

	m_LogoBtn.DrawBorder(FALSE);
	m_LogoBtn.Set3StateButton();
	m_LogoBtn.SetBitmaps(IDB_OPTION_LOGO, RGB(255,0,255));	
	m_LogoBtn.DrawTransparent();

	m_FolderBtn.DrawBorder(FALSE);
	m_FolderBtn.Set3StateButton();
	m_FolderBtn.SetBitmaps(IDB_OPTION_FOLDER, RGB(255,0,255));	
	m_FolderBtn.DrawTransparent();

	m_CameraBtn.DrawBorder(FALSE);
	m_CameraBtn.Set3StateButton();
	m_CameraBtn.SetBitmaps(IDB_OPTION_CAMERA, RGB(255,0,255));	
	m_CameraBtn.DrawTransparent();

	m_UpBtn.DrawBorder(FALSE);
	m_UpBtn.Set3StateButton();
	m_UpBtn.SetBitmaps(IDB_OPTION_UPBTN, RGB(255,0,255));	
	m_UpBtn.DrawTransparent();

	m_DownBtn.DrawBorder(FALSE);
	m_DownBtn.Set3StateButton();
	m_DownBtn.SetBitmaps(IDB_OPTION_DOWNBTN, RGB(255,0,255));	
	m_DownBtn.DrawTransparent();

	m_CodecBtn.DrawBorder(FALSE);
	m_CodecBtn.Set3StateButton();
	m_CodecBtn.SetBitmaps(IDB_OPTION_VIDEO, RGB(255,0,255));	
	m_CodecBtn.DrawTransparent();


	ArrangeControl();

	m_bModifyToolbar = false;
	m_bModifyRecord = false;
	m_bModifyLogo = false;
	m_bModifySnapshot = false;
	m_bModifyText = false;
	m_bModifyCamera = false;
	m_bModifyCodec = false;
	m_bModifyPenFolder = false;

	m_bModifyRegistration = false;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptionDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBitmap	LoadBitmap, *pBitmap;
	BITMAP	bm;
	CDC	memDC;
		
	memDC.CreateCompatibleDC(&dc);
	LoadBitmap.LoadBitmap(IDB_OPTIONBG);
	LoadBitmap.GetObject(sizeof(BITMAP), &bm);
	pBitmap	= (CBitmap*)memDC.SelectObject(&LoadBitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pBitmap);
	LoadBitmap.DeleteObject();	
	// Do not call CDialog::OnPaint() for painting messages
}


/////////////////////////////////////////////////////////////////////////////
//
void COptionDlg::ArrangeControl()
{	
	CRect rc;
	GetWindowRect(&rc);
	int nWidth, nHeight, cx, cy;
	nWidth = rc.Width();
	nHeight = rc.Height();
	cx = ::GetSystemMetrics(SM_CXSCREEN);
	cy = ::GetSystemMetrics(SM_CYSCREEN);

	rc.left = (cx-nWidth)/2;
	rc.right = rc.left + nWidth;
	rc.top = (cy - nHeight -160)/2;
	rc.bottom = rc.top + nHeight;

	MoveWindow(&rc);

	rc = CRect(137,43,551,311);

	m_AboutDlg.MoveWindow(&rc);
	m_HotkeyDlg.MoveWindow(&rc);
	m_PenFolderDlg.MoveWindow(&rc);
	m_RECDlg.MoveWindow(&rc);
	m_ToolbarDlg.MoveWindow(&rc);
	m_LogoDlg.MoveWindow(&rc);
	m_FolderDlg.MoveWindow(&rc);
	m_TextDlg.MoveWindow(&rc);
	m_SoundDlg.MoveWindow(&rc);
	m_SnapshotDlg.MoveWindow(&rc);
	m_CameraDlg.MoveWindow(&rc);
	m_CodecDlg.MoveWindow(&rc);

	m_CloseBtn.MoveWindow(CRect(540,6, 555, 21));
	m_OkBtn.MoveWindow(CRect(341,332,430,357));
	m_CancelBtn.MoveWindow(CRect(450,332, 539, 357));

	State0();	
}

void COptionDlg::OnOptionToolbarbtn() 
{
	// TODO: Add your control notification handler code here		
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_SHOW);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}

void COptionDlg::OnOptionRecordbtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);	
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_SHOW);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}


void COptionDlg::OnOptionLogobtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);	
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_SHOW);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}

void COptionDlg::OnOptionPenbtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);	
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_SHOW);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}

void COptionDlg::OnOptionFolderbtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);
	m_LogoBtn.SetSelect(FALSE);	
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_SHOW);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}

void COptionDlg::OnOptionHotkeybtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);	
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_SHOW);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}

void COptionDlg::OnOptionAboutbtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_SHOW);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}

void COptionDlg::OnOptionTextbtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);	
	m_SoundBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_SHOW);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}

void COptionDlg::OnOptionSoundbtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);	
	m_SnapshotBtn.SetSelect(FALSE);
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_SHOW);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}

void COptionDlg::OnOptionSnapshotbtn() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);	
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_CameraBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_SHOW);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_HIDE);
}


void COptionDlg::OnOptionCamera() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);	
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);
	m_CodecBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_SHOW);
	m_CodecDlg.ShowWindow(SW_HIDE);
}


void COptionDlg::OnOptionCodec() 
{
	// TODO: Add your control notification handler code here
	m_ToolbarBtn.SetSelect(FALSE);
	m_RecordBtn.SetSelect(FALSE);
	m_PenBtn.SetSelect(FALSE);
	m_HotkeyBtn.SetSelect(FALSE);
	m_AboutBtn.SetSelect(FALSE);
	m_TextBtn.SetSelect(FALSE);
	m_SoundBtn.SetSelect(FALSE);	
	m_LogoBtn.SetSelect(FALSE);
	m_FolderBtn.SetSelect(FALSE);
	m_SnapshotBtn.SetSelect(FALSE);

	m_AboutDlg.ShowWindow(SW_HIDE);
	m_HotkeyDlg.ShowWindow(SW_HIDE);
	m_PenFolderDlg.ShowWindow(SW_HIDE);
	m_RECDlg.ShowWindow(SW_HIDE);
	m_ToolbarDlg.ShowWindow(SW_HIDE);
	m_LogoDlg.ShowWindow(SW_HIDE);
	m_FolderDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
	m_SoundDlg.ShowWindow(SW_HIDE);
	m_SnapshotDlg.ShowWindow(SW_HIDE);
	m_CameraDlg.ShowWindow(SW_HIDE);
	m_CodecDlg.ShowWindow(SW_SHOW);
}


void COptionDlg::OnOptionclosebtn()
{
	// TODO: Add your control notification handler code here
	
	OnCancel();
}

void COptionDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_bModifyToolbar = m_ToolbarDlg.WriteOption();
	m_bModifyRecord = m_RECDlg.WriteOption();
	m_bModifyPenFolder = m_PenFolderDlg.WriteOption();
	m_bModifyLogo = m_LogoDlg.WriteOption();
	m_bModifySnapshot = m_SnapshotDlg.WriteOption();
	m_bModifyText = m_TextDlg.WriteOption();
	m_bModifyCamera = m_CameraDlg.WriteOption();
	m_bModifyCodec = m_CodecDlg.WriteOption();
	
	m_bModifyRegistration = m_AboutDlg.m_bModifyRegistration;

	CDialog::OnOK();
}

void COptionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_bModifyRegistration = m_AboutDlg.m_bModifyRegistration;
	
	CDialog::OnCancel();
}

BOOL COptionDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_SYSKEYDOWN )
	{
        if(pMsg->wParam == VK_F4)
			return TRUE;
	}

	if(pMsg->message == WM_KEYDOWN)
	{			
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE)		// Esc, Enter key down
			return	TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void COptionDlg::SetClose()
{
	SetTimer(1, 500, NULL);
}

void COptionDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		KillTimer(1);
		OnOK();
	}

	CDialog::OnTimer(nIDEvent);
}


void COptionDlg::OnOptionUpbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_State == 0)
		return;

	m_State--;

	switch(m_State)
	{
	case	0:
		State0();
		break;		
	case	1:
		State1();
		break;
	case	2:
		State2();
		break;
	case	3:
		State3();
		break;
	}
}

void COptionDlg::OnOptionDownbtn() 
{
	// TODO: Add your control notification handler code here
	if(m_State == 3)
		return;

	m_State++;

	switch(m_State)
	{
	case	0:
		State0();
		break;		
	case	1:
		State1();
		break;
	case	2:
		State2();
		break;
	case	3:
		State3();
		break;
	}
}


void COptionDlg::State0()
{
	m_UpBtn.MoveWindow(CRect(25, 46, 112, 65));
	m_ToolbarBtn.MoveWindow(CRect(25, 67, 112, 91));
	m_RecordBtn.MoveWindow(CRect(25, 92, 112, 116));
	m_LogoBtn.MoveWindow(CRect(25, 117, 112, 141));	
	m_PenBtn.MoveWindow(CRect(25, 142, 112, 166));
	m_FolderBtn.MoveWindow(CRect(25, 167, 112, 191));	
	m_SnapshotBtn.MoveWindow(CRect(25, 192, 112, 216));
	m_TextBtn.MoveWindow(CRect(25, 217, 112, 241));
	m_CameraBtn.MoveWindow(CRect(25, 242, 112, 266));
	m_SoundBtn.MoveWindow(CRect(25, 267, 112, 291));	
	m_DownBtn.MoveWindow(CRect(25, 293, 112, 312));

	m_ToolbarBtn.ShowWindow(SW_SHOW);
	m_CodecBtn.ShowWindow(SW_HIDE);
	m_HotkeyBtn.ShowWindow(SW_HIDE);
	m_AboutBtn.ShowWindow(SW_HIDE);	
}

void COptionDlg::State1()
{
	m_ToolbarBtn.ShowWindow(SW_HIDE);

	m_RecordBtn.MoveWindow(CRect(25, 67, 112, 91));
	m_LogoBtn.MoveWindow(CRect(25, 92, 112, 116));
	m_PenBtn.MoveWindow(CRect(25, 117, 112, 141));	
	m_FolderBtn.MoveWindow(CRect(25, 142, 112, 166));
	m_SnapshotBtn.MoveWindow(CRect(25, 167, 112, 191));	
	m_TextBtn.MoveWindow(CRect(25, 192, 112, 216));
	m_CameraBtn.MoveWindow(CRect(25, 217, 112, 241));
	m_SoundBtn.MoveWindow(CRect(25, 242, 112, 266));
	m_CodecBtn.MoveWindow(CRect(25, 267, 112, 291));		
	
	m_RecordBtn.ShowWindow(SW_SHOW);
	m_CodecBtn.ShowWindow(SW_SHOW);
	m_HotkeyBtn.ShowWindow(SW_HIDE);
	m_AboutBtn.ShowWindow(SW_HIDE);	
}

void COptionDlg::State2()
{
	m_RecordBtn.ShowWindow(SW_HIDE);

	m_LogoBtn.MoveWindow(CRect(25, 67, 112, 91));
	m_PenBtn.MoveWindow(CRect(25, 92, 112, 116));
	m_FolderBtn.MoveWindow(CRect(25, 117, 112, 141));	
	m_SnapshotBtn.MoveWindow(CRect(25, 142, 112, 166));
	m_TextBtn.MoveWindow(CRect(25, 167, 112, 191));	
	m_CameraBtn.MoveWindow(CRect(25, 192, 112, 216));
	m_SoundBtn.MoveWindow(CRect(25, 217, 112, 241));
	m_CodecBtn.MoveWindow(CRect(25, 242, 112, 266));
	m_HotkeyBtn.MoveWindow(CRect(25, 267, 112, 291));	
	
	m_LogoBtn.ShowWindow(SW_SHOW);
	m_HotkeyBtn.ShowWindow(SW_SHOW);
	m_AboutBtn.ShowWindow(SW_HIDE);	
}

void COptionDlg::State3()
{
	m_LogoBtn.ShowWindow(SW_HIDE);

	m_PenBtn.MoveWindow(CRect(25, 67, 112, 91));
	m_FolderBtn.MoveWindow(CRect(25, 92, 112, 116));
	m_SnapshotBtn.MoveWindow(CRect(25, 117, 112, 141));	
	m_TextBtn.MoveWindow(CRect(25, 142, 112, 166));
	m_CameraBtn.MoveWindow(CRect(25, 167, 112, 191));	
	m_SoundBtn.MoveWindow(CRect(25, 192, 112, 216));
	m_CodecBtn.MoveWindow(CRect(25, 217, 112, 241));
	m_HotkeyBtn.MoveWindow(CRect(25, 242, 112, 266));
	m_AboutBtn.MoveWindow(CRect(25, 267, 112, 291));		

	m_AboutBtn.ShowWindow(SW_SHOW);	
}
