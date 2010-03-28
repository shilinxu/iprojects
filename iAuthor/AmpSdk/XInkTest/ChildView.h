// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__5F0D869A_ADC4_413A_A3B5_3D7C7117080B__INCLUDED_)
#define AFX_CHILDVIEW_H__5F0D869A_ADC4_413A_A3B5_3D7C7117080B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XInkTest.h"

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	int  GetVideoHWAccelerationLevel();
	bool SetVideoHWAccelerationLevel(const int level, const bool bForever=false);
	virtual ~CChildView();

protected:
	XInk::xCanvas*	m_pCanvas;
	XInk::xPen		m_pen;
	bool			m_bDisableHardwareAcceleration;

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditUndo();
	afx_msg void OnColorBlack();
	afx_msg void OnColorGreen();
	afx_msg void OnColorBlue();
	afx_msg void OnColorRed();
	afx_msg void OnColorWhite();
	afx_msg void OnWidth01();
	afx_msg void OnWidth02();
	afx_msg void OnWidth03();
	afx_msg void OnWidth05();
	afx_msg void OnWidth07();
	afx_msg void OnWidth09();
	afx_msg void OnWidth20();
	afx_msg void OnWidth30();
	afx_msg void OnDtoolChpen();
	afx_msg void OnDtoolEraser();
	afx_msg void OnDtoolHlighter();
	afx_msg void OnDtoolPen();
	afx_msg void OnDtoolEraserAll();
	afx_msg void OnViewRefresh();
	afx_msg void OnDtoolEraserRect();
	afx_msg void OnDtoolShapeLine();
	afx_msg void OnDtoolShapeLineArrow();
	afx_msg void OnDtoolShapeRect();
	afx_msg void OnDtoolShapeEllipse();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDtoolHlighterEl();
	afx_msg void OnRecordStart();
	afx_msg void OnRecordStop();
	afx_msg void OnWidth15();
	afx_msg void OnUpdateAppHaccel(CCmdUI* pCmdUI);
	afx_msg void OnAppHaccel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__5F0D869A_ADC4_413A_A3B5_3D7C7117080B__INCLUDED_)
