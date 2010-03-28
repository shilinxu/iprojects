// AuthorDlg.h : header file
//

#if !defined(AFX_AUTHORDLG_H__B54EED83_E097_4E50_9D05_E82D573F1FE3__INCLUDED_)
#define AFX_AUTHORDLG_H__B54EED83_E097_4E50_9D05_E82D573F1FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../AmpSdk/DBHook/DBHook.h"
#include "../AmpSdk/include/BclScrRec.h"
//#include "../../DBCodecLib/DBCodec.h"

#include "BtnST.h"
#include "BoardSelectDlg.h"
#include "ColorSelctDlg.h"
#include "DiagramSelectDlg.h"
#include "EraserSelectDlg.h"
#include "LineWeightSelectDlg.h"
#include "PenSelectDlg.h"
#include "SlideMenuDlg.h"
#include "BoardDlg.h"
#include "MagnifierDlg.h"
#include "RecSizeDlg.h"

#include "TextDlg.h"

#define		PENTOOL			1
#define		ERASERTOOL		2
#define		TEXTTOOL		3
#define		DIAGRAMTOOL		4

#define		NONE_POPUP					0
#define		BOARDSELECT_POPUP			1
#define		COLORSELECT_POPUP			2
#define		DIAGRAMSELECT_POPUP			3
#define		ERASERSELECT_POPUP			4
#define		LINEWEIGHTSELECT_POPUP		5
#define		PENSELECT_POPUP				6

#define		WM_MESSAGE_TEXT_DELETE		(WM_USER+200)

/////////////////////////////////////////////////////////////////////////////
// CAuthorDlg dialog

class CAuthorDlg : public CDialog
{
// Construction
public:
	CAuthorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAuthorDlg)
	enum { IDD = IDD_AUTHOR_DIALOG };
	CButtonST	m_CameraBtn;
	CButtonST	m_OptionBtn;
	CButtonST	m_ExitBtn;
	CButtonST	m_SlideMenuBtn;	
	CButtonST	m_PrintBtn;	
	CButtonST	m_WmvBtn;
	CButtonST	m_SnapshotBtn;
	CButtonST	m_ZoomBtn;
	CButtonST	m_BoardOpenBtn;	
	CButtonST	m_HelpBtn;
	CButtonST	m_MicBtn;	
	CButtonST	m_PPTNextBtn;
	CButtonST	m_PPTPrevBtn;
	CButtonST	m_RecordStopBtn;
	CButtonST	m_RecordPauseBtn;
	CButtonST	m_RecordBtn;	
	CButtonST	m_DiagramBtn;
	CButtonST	m_LineWeightBtn;
	CButtonST	m_ColorChangeBtn;	
	CButtonST	m_EraserBtn;
	CButtonST	m_TextBtn;	
	CButtonST	m_PenBtn;
	CButtonST	m_ModeBtn;
	CButtonST	m_MoveBottomBtn;
	CButtonST	m_MoveTopBtn;
	CButtonST	m_ShowhideBtn;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAuthorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	void OnNotifyTextInputComplete();
	void OnNotifyZoomEnd(bool bZoom);
	void SetEnableLayeredWnd();
	LRESULT OnTextDelete(WPARAM wp, LPARAM lp);


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAuthorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnShowhidetoolbtn();
	afx_msg void OnModebtn();
	afx_msg void OnTextBtn();
	afx_msg void OnEraserbtn();
	afx_msg void OnColorchangebtn();
	afx_msg void OnLineweightchangebtn();
	afx_msg void OnPptprevbtn();
	afx_msg void OnPptnextbtn();
	afx_msg void OnRecordbtn();
	afx_msg void OnRecordpausebtn();
	afx_msg void OnRecordstopbtn();
	afx_msg void OnWMVbtn();
	afx_msg void OnZoombtn();
	virtual void OnOK();
	afx_msg void OnBoardOpenbtn();
	afx_msg void OnDiagrambtn();
	afx_msg void OnMicbtn();
	afx_msg void OnMovetopbtn();
	afx_msg void OnPenbtn();
	afx_msg void OnPrintbtn();
	afx_msg void OnSlidemenubtn();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMovebottombtn();
	afx_msg void OnHelp();	
	afx_msg void OnOptionbtn();
	afx_msg void OnCameraOnOff();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:	
	LRESULT OnSnapshot(WPARAM wParam, LPARAM lParam);
	LRESULT OnCtrlMouseOut(WPARAM wParam, LPARAM lParam);
	LRESULT OnCtrlMouseOver(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotifyPenSelect(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotifyEraserSelect(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotifyColorSelect(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotifyLineWeightSelect(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotifyDiagramSelect(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotifyBoardSelect(WPARAM wParam, LPARAM lParam);
	LRESULT OnNotifySlideMenuSelect(WPARAM wParam, LPARAM lParam);

private:	
	void CreateCanvas(HWND hwnd, XInk::xRect rc);
	void ReleaseCanvas();

	void InitVolume();
	void ArrangeControl();
	void ArrangePopupMenu();
	void UnFoldDialog();
	void FoldDialog();

	void HidePopupMenu();
	void ShowPopupMenu();
	void OpenBoard(int nBoardType);

	void SetPenType(int nType);

	void SetImageCursor();
	BOOL CaptureDesktop(CString strPath, bool bStrech = false);

	BOOL OnReadyRecord();
	BOOL SetRecordTerminate();
	void DisplayRecTime();
	
	void OnZoomShot();

	void ReadPenFolderOption();
	void ReadRecordOption();
	void ReadToolbarOption();

	void InitScheme();
	void WriteSchemeIndex(DWORD dwEndTime);	
	void SetSchemeTerminate(CString strFilename, DWORD dwDuration);
	void SetPPTSlideShowHide(bool bHide = true);

	void OnRegstrationDlg();
			
	void EndSelectArea();
	void InitRecord();
	void SelectRecordRange();
	void DisplaySelectWindow(bool bSeletFlag = false);
	void SetEnableRecLayeredWnd();
	void CalFoldControl();

private:
	void ReadCodecOption();
	void ReadTextOption();
	void EndTextRange();
	void ReMoveAllTextClp();
	void ReadCameraOption();
	void EndSnapshotRange();
	void ReadSnapshotOption();
	void MarkingRecArea();
	void ArrangeControlSmallMode();
	void SmallModeFold();
	CBoardSelectDlg			m_BoardSelectDlg;
	CColorSelctDlg			m_ColorSelctDlg;
	CDiagramSelectDlg		m_DiagramSelectDlg;
	CEraserSelectDlg		m_EraserSelectDlg;
	CLineWeightSelectDlg	m_LineWeightSelectDlg;
	CPenSelectDlg			m_PenSelectDlg;
	CSlideMenuDlg			m_SlideMenuDlg;
	CBoardDlg				m_BoardDlg;	

	CMagnifierDlg			m_MagnifierDlg;
	CRecSizeDlg				m_RecSizeDlg;

	CRect	m_rcShow[23];
	CRect	m_rcHide[23];
	bool	m_bShowControl[13];	

	bool	m_bFold;
	int		m_nFoldWidth;	
	int		m_nMargine;
	int		m_nControlStart;
	int		m_nHideControlStart;
	int		m_nHideSlideStart;
	int		m_nControlMargine;
	int		m_nControlWidth;
	int		m_nSlideBGWidth;	
	int		m_nShowControlCount;
	CRect	m_rcHideRecState;
	CRect	m_rcHideRecTime;
	CRect	m_rcHideRecProgress;
	
	bool	m_bRegistration;
	bool	m_bLayeredWndEnable;
	bool	m_bRecToolbar;
	bool	m_bMoveTop;

	int		m_nSubMenuPopup;
	int		m_nSelMouseOverCtrlID;
	CRect	m_rcEnable;
	CRect   m_rcEnableMenu;
	
	int			m_nHLPenPattern;		//0: "I", 1: "O"
	int			m_nLinePattern;			//0: "-" , 1: "->"
	bool		m_bSnapshotFullMode;
	int			m_nCamSize;
	bool		m_bSnapshotRange;
	bool		m_bTextRange;

	int			m_nPenToolType;
	int			m_nPenType;
	int			m_nEraserType;
	COLORREF	m_Color;
	int			m_PenWidth;
	int			m_nDiagramType;
	
	bool		m_bBoardOpen;
	int			m_nSlideIndex;
	CString		m_strSlidePath;

	bool		m_bZoomMode;
	CRect		m_rcZoom;

	bool		m_bEnableVolume;
	UINT		m_uMicLineIndex;
	DWORD		m_dwMicCurVol;	

	HCURSOR		m_hCursor;

	CString		m_strRecState;
	CString		m_strRecTime;	
	CRect		m_rcRecState;
	CRect		m_rcRecTime;
	CRect		m_rcRecProgress;
	int			m_nRecProgress;


	SRECHANDLE  m_hCapture;	
	bool		m_bRecordFlag;
	bool		m_bInitRecord;
	BOOL		m_bRecCursor;
	DWORD		m_dwFps;
	DWORD		m_dwQuality;
	DWORD		m_dwKeyFrameRate;
	bool		m_bRecLogo;
	HBITMAP		m_hBitmapLogo;

	XInk::xCanvas*	m_pCanvas;
	XInk::xPen		m_pen;
	bool			m_bDrawCanvas;

	bool		m_bMakeIndex;
	DWORD		m_nIndexStartTime;
	int			m_nSchemeIndex;

	bool		m_bPPTSlideShowMinize;	

	int			m_nRecordRangeState;
	CRect		m_rcPrevRecordRangeWindow;
	CRect		m_rcCurRecordRangeWindow;
	bool		m_bSelectRecordWindow;
	bool		m_bSelectRecordArea;	

	LOGBRUSH	m_LogBrush;


	/////////////////////////////////////////////////////////////////////////
	// 040727 - Update

	bool		m_bSmallMode;
	CRect		m_rcSmallModeShow[7];
	CRect		m_rcSmallModeHide[12];
	CRect		m_rcSmallModeSlide;
	CRect		m_rcSmallModeCamera;
	CRect		m_rcSmallModePrint;
	CRect		m_rcSmallModeExit;

	int			m_WidthScreen;
	int			m_HeightScreen;

	bool		m_bReName;

	bool		m_bMarkingRecArea;

	COLORREF	m_bgTextColor;
	CObList		m_TextList;

	bool		m_bSelCodec;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTHORDLG_H__B54EED83_E097_4E50_9D05_E82D573F1FE3__INCLUDED_)
