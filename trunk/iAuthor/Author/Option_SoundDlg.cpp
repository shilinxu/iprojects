// Option_SoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "author.h"
#include "Option_SoundDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COption_SoundDlg dialog


COption_SoundDlg::COption_SoundDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COption_SoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COption_SoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brush.CreateSolidBrush(RGB(203,202,206));
	
	m_bFindStereoMix = false;
	m_bFindMike = false;

	m_nCheckIndex = 0;
}


void COption_SoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COption_SoundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COption_SoundDlg, CDialog)
	//{{AFX_MSG_MAP(COption_SoundDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPT_MIKEREC, OnOptMikerec)
	ON_BN_CLICKED(IDC_OPT_MIXREC, OnOptMixrec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COption_SoundDlg message handlers

BOOL COption_SoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!IsValidMixer())
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_OPT_MIKEREC);
		pButton->EnableWindow(FALSE);

		pButton = (CButton*)GetDlgItem(IDC_OPT_MIXREC);
		pButton->EnableWindow(FALSE);

		m_bEnableMixer = false;
	}
	else
	{
		CButton* pButton;
		if(!m_bFindStereoMix && !m_bFindMike)
		{
			pButton = (CButton*)GetDlgItem(IDC_OPT_MIKEREC);
			pButton->EnableWindow(FALSE);

			pButton = (CButton*)GetDlgItem(IDC_OPT_MIXREC);
			pButton->EnableWindow(FALSE);

			m_bEnableMixer = false;

			return TRUE;
		}

		if(m_bFindStereoMix)
		{
			pButton = (CButton*)GetDlgItem(IDC_OPT_MIXREC);
			pButton->EnableWindow();
		}
		
		if(m_bFindMike)
		{
			pButton = (CButton*)GetDlgItem(IDC_OPT_MIKEREC);
			pButton->EnableWindow();
		}

		if(m_bFindStereoMix && m_bFindMike)
			m_bEnableMixer = true;

		switch(m_nCheckIndex)
		{
		case	1:
			{
				pButton = (CButton*)GetDlgItem(IDC_OPT_MIXREC);
				pButton->SetCheck(1);
			}
			break;
		case	2:
			{
				pButton = (CButton*)GetDlgItem(IDC_OPT_MIKEREC);
				pButton->SetCheck(1);
			}
			break;
		default:
			break;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COption_SoundDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetTextColor(RGB(100,100,100));
	dc.SetBkMode(TRANSPARENT);
	CFont	Font, *pOldFont;		
	Font.CreatePointFont(90,"Arial", &dc);
	pOldFont = (CFont*)dc.SelectObject(&Font);	

	dc.DrawText("마이크녹음을 체크하시면 마이크를 통한 소리만 녹음됩니다."
				"\r\n동시녹음을 체크하시면 PC에서 재생되는 음악이나 효과음 등이"
				"\r\n마이크를 통한 소리와 같이 녹음됩니다.",
				CRect(10, 130, 406, 175), DT_LEFT);

	dc.SetTextColor(RGB(0,0,255));
	if(!m_bFindStereoMix && !m_bFindMike)
	{
		dc.DrawText("(사운드 카드에서 마이크녹음과 동시 녹음을 지원하지 않습니다.)",
				CRect(10, 175, 406, 200), DT_LEFT);
	}
	else if(!m_bFindMike)
	{
		dc.DrawText("(사운드 카드에서 마이크녹음을 지원하지 않습니다.)",
				CRect(10, 175, 406, 200), DT_LEFT);
	}
	else if(!m_bFindStereoMix)
	{
		dc.DrawText("(사운드 카드에서 동시녹음을 지원하지 않습니다.)",
				CRect(10, 175, 406, 200), DT_LEFT);
	}

	if(m_nCheckIndex == 0)
	{
		dc.SetTextColor(RGB(0,0,255));
		dc.DrawText("- 현재 사운드 카드 녹음 설정이 마이크녹음이나 동시녹음 설정으로 되어"
					"\r\n  있지 않습니다.",					
					CRect(10, 200, 406, 260), DT_LEFT);
	}
	dc.SelectObject(pOldFont);
	Font.DeleteObject();
}

HBRUSH COption_SoundDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkColor(RGB(203,202,206));

	return m_brush;
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL COption_SoundDlg::PreTranslateMessage(MSG* pMsg) 
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


///////////////////////////////////////////////////////////////////////////////
//
bool COption_SoundDlg::IsValidMixer()
{
	if(::waveInGetNumDevs() < 1)
		return false;

	int nStereoMixIndex, nMikeIndex;
	
	HMIXER hmx;
    mixerOpen(&hmx, 0, 0, 0, 0);    
    
    MIXERLINE mxl;
    mxl.cbStruct = sizeof(mxl);
    mxl.dwComponentType = 
		MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
    MMRESULT MMResult = mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);	
		
	if(MMResult != MMSYSERR_NOERROR)
	{		
		mixerClose(hmx);
		return false;
	}
	
	MMResult = mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_LINEID);	
	if(MMResult != MMSYSERR_NOERROR)
	{			
		mixerClose(hmx);
		return false;
	}
	
	if (0 == mxl.cControls)
	{		
		mixerClose(hmx);
		return false;
	}

	PMIXERCONTROL       pamxctrl;
	UINT cbmxctrls = sizeof(*pamxctrl) * (UINT)mxl.cControls;
	pamxctrl = (PMIXERCONTROL)LocalAlloc(LPTR, cbmxctrls);
	if (NULL == pamxctrl)
	{		
		mixerClose(hmx);
		return false;
	}
	
    MIXERLINECONTROLS   mxlc;
	mxlc.cbStruct       = sizeof(mxlc);
	mxlc.dwLineID       = mxl.dwLineID;
	mxlc.cControls      = mxl.cControls;
	mxlc.cbmxctrl       = sizeof(*pamxctrl);
	mxlc.pamxctrl       = pamxctrl;	
	
	MMResult = mixerGetLineControls((HMIXEROBJ)hmx, &mxlc, MIXER_GETLINECONTROLSF_ALL);

	if (MMSYSERR_NOERROR != MMResult)
	{		
		LocalFree((HLOCAL)pamxctrl);
		mixerClose(hmx);

		return false;
	}    

    switch (pamxctrl->dwControlType)
    {
	case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
	case MIXERCONTROL_CONTROLTYPE_MUX:
	case MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT:
	case MIXERCONTROL_CONTROLTYPE_MIXER:
		break;		
	default:		
		mixerClose(hmx);
		return false;
    }

	PMIXERCONTROLDETAILS_LISTTEXT pmxcd_lt;
    MIXERCONTROLDETAILS         mxcd;
    UINT                        uIndex;

	BOOL fSingleSelect = (MIXERCONTROL_CT_SC_LIST_SINGLE == (MIXERCONTROL_CT_SUBCLASS_MASK & pamxctrl->dwControlType));
	UINT cChannels = (UINT)mxl.cChannels;
    if (MIXERCONTROL_CONTROLF_UNIFORM & pamxctrl->fdwControl)
        cChannels = 1;
	
    UINT cMultipleItems = 1;
    if (MIXERCONTROL_CONTROLF_MULTIPLE & pamxctrl->fdwControl)
        cMultipleItems = (UINT)pamxctrl->cMultipleItems;	
	
	int cb = cChannels * cMultipleItems * sizeof(*pmxcd_lt);
	
	pmxcd_lt = (PMIXERCONTROLDETAILS_LISTTEXT)LocalAlloc(LPTR, cb);
    if (NULL == pmxcd_lt)
    {        
		mixerClose(hmx);
        return false;
    }	
	
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pamxctrl->dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_lt);
    mxcd.paDetails      = pmxcd_lt;
	
	MMResult = mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd, MIXER_GETCONTROLDETAILSF_LISTTEXT);

    if (MMSYSERR_NOERROR != MMResult)
    {        
		mixerClose(hmx);
        return false; 
    }	
	
	for (int u = 0; u < cChannels; u++)
    {
        for (int v = 0; v < cMultipleItems; v++)
        {
            uIndex = (u * cMultipleItems) + v;
			
			if(stricmp(pmxcd_lt[uIndex].szName, "mixer")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Stereo Mixer")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "스테레오 믹서")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Mixed Output")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "믹스드 아웃풋")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Stereo Mix")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "스테레오 믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Mixed Out")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "믹스드 아웃")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Stereo Output")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "스테레오 아웃풋")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "What U hear")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "왓 유 히어")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Stereo Out")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "스테레오 아웃")==0 ||			
			   stricmp(pmxcd_lt[uIndex].szName, "Loop back")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "루프 백")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Digital Mixe")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "디지털 믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Post-Mix")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "포스트 믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Digital Mix")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Sum")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "섬")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "wave out mix")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "웨이브 아웃 믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "웨이크 출력 믹스")==0)
			{
				nStereoMixIndex = uIndex;
				m_bFindStereoMix = true;
				break;
			}
        }		
    }

	for (int u = 0; u < cChannels; u++)
    {
        for (int v = 0; v < cMultipleItems; v++)
        {
            uIndex = (u * cMultipleItems) + v;
			
			if(stricmp(pmxcd_lt[uIndex].szName, "Microphone")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "마이크로폰")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Mike")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "마이크")==0)
			{
				nMikeIndex = uIndex;
				m_bFindMike = true;
				break;
			}
        }		
    }
	
	PMIXERCONTROLDETAILS_BOOLEAN    pmxcd_f;
	pmxcd_f = (PMIXERCONTROLDETAILS_BOOLEAN) new MIXERCONTROLDETAILS_BOOLEAN[20];
			
	mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pamxctrl[0].dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = pamxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_f);
    mxcd.paDetails      = pmxcd_f;
		
    MMResult = mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);
    if (MMSYSERR_NOERROR != MMResult)
    {      
        mixerClose(hmx);      
		LocalFree((HLOCAL)pmxcd_lt);  		
		
		free(pmxcd_f);
        return false;
    }	
		
	cMultipleItems = 1;
    if (MIXERCONTROL_CONTROLF_MULTIPLE & pamxctrl->fdwControl)
        cMultipleItems = (UINT)pamxctrl->cMultipleItems;
		
	BOOL fValue;
    for (int u = 0; u < cChannels; u++)
    {
        for (int v = 0; v < cMultipleItems; v++)
        {
            uIndex = (u * cMultipleItems) + v;			
            fValue = (BOOL)pmxcd_f[uIndex].fValue;
			
			if(fValue)
			{
				if(nStereoMixIndex == uIndex)
					m_nCheckIndex = 1;
				else if(nMikeIndex == uIndex)
					m_nCheckIndex = 2;
			}
        }
    }

	mixerClose(hmx);
	delete [] pmxcd_f;	
	LocalFree((HLOCAL)pmxcd_lt);
	return true;
}

void COption_SoundDlg::OnOptMikerec() 
{
	// TODO: Add your control notification handler code here
	if(::waveInGetNumDevs() < 1)
		return;
	
	int iFinalIndex;
	HMIXER hmx;
    mixerOpen(&hmx, 0, 0, 0, 0);
  
    MIXERLINE mxl;
    mxl.cbStruct = sizeof(mxl);
    mxl.dwComponentType = 
		MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
    MMRESULT MMResult = mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);	
		
	if(MMResult != MMSYSERR_NOERROR)
	{		
		mixerClose(hmx);
		return;
	}
	
	MMResult = mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_LINEID);	
	if(MMResult != MMSYSERR_NOERROR)
	{			
		mixerClose(hmx);
		return;
	}
	
	if (0 == mxl.cControls)
	{		
		mixerClose(hmx);
		return;
	}

	PMIXERCONTROL       pamxctrl;
	UINT cbmxctrls = sizeof(*pamxctrl) * (UINT)mxl.cControls;
	pamxctrl = (PMIXERCONTROL)LocalAlloc(LPTR, cbmxctrls);
	if (NULL == pamxctrl)
	{		
		mixerClose(hmx);
		return;
	}
	
    MIXERLINECONTROLS   mxlc;
	mxlc.cbStruct       = sizeof(mxlc);
	mxlc.dwLineID       = mxl.dwLineID;
	mxlc.cControls      = mxl.cControls;
	mxlc.cbmxctrl       = sizeof(*pamxctrl);
	mxlc.pamxctrl       = pamxctrl;	
	
	MMResult = mixerGetLineControls((HMIXEROBJ)hmx, &mxlc, MIXER_GETLINECONTROLSF_ALL);

	if (MMSYSERR_NOERROR != MMResult)
	{		
		LocalFree((HLOCAL)pamxctrl);
		mixerClose(hmx);

		return;
	}    

    switch (pamxctrl->dwControlType)
    {
	case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
	case MIXERCONTROL_CONTROLTYPE_MUX:
	case MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT:
	case MIXERCONTROL_CONTROLTYPE_MIXER:
		break;		
	default:		
		mixerClose(hmx);
		return;
    }

	PMIXERCONTROLDETAILS_LISTTEXT pmxcd_lt;
    MIXERCONTROLDETAILS         mxcd;
    UINT                        uIndex;

	BOOL fSingleSelect = (MIXERCONTROL_CT_SC_LIST_SINGLE == (MIXERCONTROL_CT_SUBCLASS_MASK & pamxctrl->dwControlType));
	UINT cChannels = (UINT)mxl.cChannels;
    if (MIXERCONTROL_CONTROLF_UNIFORM & pamxctrl->fdwControl)
        cChannels = 1;
	
    UINT cMultipleItems = 1;
    if (MIXERCONTROL_CONTROLF_MULTIPLE & pamxctrl->fdwControl)
        cMultipleItems = (UINT)pamxctrl->cMultipleItems;	
	
	int cb = cChannels * cMultipleItems * sizeof(*pmxcd_lt);
	
	pmxcd_lt = (PMIXERCONTROLDETAILS_LISTTEXT)LocalAlloc(LPTR, cb);
    if (NULL == pmxcd_lt)
    {        
		mixerClose(hmx);
        return;
    }	
	
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pamxctrl->dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_lt);
    mxcd.paDetails      = pmxcd_lt;
	
	MMResult = mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd, MIXER_GETCONTROLDETAILSF_LISTTEXT);

    if (MMSYSERR_NOERROR != MMResult)
    {        
		mixerClose(hmx);
        return; 
    }	
	
	for (int u = 0; u < cChannels; u++)
    {
        for (int v = 0; v < cMultipleItems; v++)
        {
            uIndex = (u * cMultipleItems) + v;
			
			if(stricmp(pmxcd_lt[uIndex].szName, "Microphone")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "마이크로폰")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Mike")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "마이크")==0)
			{
				iFinalIndex = uIndex;				
				break;
			}
        }		
    }
	
	PMIXERCONTROLDETAILS_BOOLEAN    pmxcd_f;
	pmxcd_f = (PMIXERCONTROLDETAILS_BOOLEAN) new MIXERCONTROLDETAILS_BOOLEAN[20];
			
	mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pamxctrl[0].dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = pamxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_f);
    mxcd.paDetails      = pmxcd_f;
		
    MMResult = mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);
    if (MMSYSERR_NOERROR != MMResult)
    {      
        mixerClose(hmx);      
		LocalFree((HLOCAL)pmxcd_lt);  		
		
		free(pmxcd_f);
        return;
    }	
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pamxctrl->dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = pamxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_f);
    mxcd.paDetails      = pmxcd_f;
	
    MMResult = mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd, 0L);
    if (MMSYSERR_NOERROR != MMResult)
    {
		mixerClose(hmx);		
        return; 
    }
	
	if (fSingleSelect)
    {
        UINT        u;
        UINT        v;
        UINT        cMultipleItems;

        cMultipleItems = (UINT)pamxctrl->cMultipleItems;		
        u = uIndex / (cChannels * cMultipleItems);
		
        for (v = 0; v < cMultipleItems; v++)
        {
            pmxcd_f[(u * cMultipleItems) + v].fValue = FALSE;
        }
    }
	
    pmxcd_f[iFinalIndex].fValue = TRUE;
		
    MMResult = mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd, 0L);
    if (MMSYSERR_NOERROR != MMResult)
    {
        mixerClose(hmx);
		delete [] pmxcd_f;
		
		LocalFree((HLOCAL)pmxcd_lt);		
		return;
    }

	mixerClose(hmx);
	delete [] pmxcd_f;	
	LocalFree((HLOCAL)pmxcd_lt);

	m_nCheckIndex = 2;
	InvalidateRect(CRect(10, 200, 406, 260));

	return;
}

void COption_SoundDlg::OnOptMixrec() 
{	
	if(::waveInGetNumDevs() < 1)
		return;

	int iFinalIndex;
	HMIXER hmx;
    mixerOpen(&hmx, 0, 0, 0, 0);
       
    MIXERLINE mxl;
    mxl.cbStruct = sizeof(mxl);
    mxl.dwComponentType = 
		MIXERLINE_COMPONENTTYPE_DST_WAVEIN; 
    MMRESULT MMResult = mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);	
		
	if(MMResult != MMSYSERR_NOERROR)
	{		
		mixerClose(hmx);
		return;
	}
	
	MMResult = mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_LINEID);	
	if(MMResult != MMSYSERR_NOERROR)
	{			
		mixerClose(hmx);
		return;
	}
	
	if (0 == mxl.cControls)
	{		
		mixerClose(hmx);
		return;
	}

	PMIXERCONTROL       pamxctrl;
	UINT cbmxctrls = sizeof(*pamxctrl) * (UINT)mxl.cControls;
	pamxctrl = (PMIXERCONTROL)LocalAlloc(LPTR, cbmxctrls);
	if (NULL == pamxctrl)
	{		
		mixerClose(hmx);
		return;
	}
	
    MIXERLINECONTROLS   mxlc;
	mxlc.cbStruct       = sizeof(mxlc);
	mxlc.dwLineID       = mxl.dwLineID;
	mxlc.cControls      = mxl.cControls;
	mxlc.cbmxctrl       = sizeof(*pamxctrl);
	mxlc.pamxctrl       = pamxctrl;	
	
	MMResult = mixerGetLineControls((HMIXEROBJ)hmx, &mxlc, MIXER_GETLINECONTROLSF_ALL);

	if (MMSYSERR_NOERROR != MMResult)
	{		
		LocalFree((HLOCAL)pamxctrl);
		mixerClose(hmx);

		return;
	}    

    switch (pamxctrl->dwControlType)
    {
	case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
	case MIXERCONTROL_CONTROLTYPE_MUX:
	case MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT:
	case MIXERCONTROL_CONTROLTYPE_MIXER:
		break;		
	default:		
		mixerClose(hmx);
		return;
    }

	PMIXERCONTROLDETAILS_LISTTEXT pmxcd_lt;
    MIXERCONTROLDETAILS         mxcd;
    UINT                        uIndex;

	BOOL fSingleSelect = (MIXERCONTROL_CT_SC_LIST_SINGLE == (MIXERCONTROL_CT_SUBCLASS_MASK & pamxctrl->dwControlType));
	UINT cChannels = (UINT)mxl.cChannels;
    if (MIXERCONTROL_CONTROLF_UNIFORM & pamxctrl->fdwControl)
        cChannels = 1;
	
    UINT cMultipleItems = 1;
    if (MIXERCONTROL_CONTROLF_MULTIPLE & pamxctrl->fdwControl)
        cMultipleItems = (UINT)pamxctrl->cMultipleItems;	
	
	int cb = cChannels * cMultipleItems * sizeof(*pmxcd_lt);
	
	pmxcd_lt = (PMIXERCONTROLDETAILS_LISTTEXT)LocalAlloc(LPTR, cb);
    if (NULL == pmxcd_lt)
    {        
		mixerClose(hmx);
        return;
    }	
	
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pamxctrl->dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_lt);
    mxcd.paDetails      = pmxcd_lt;
	
	MMResult = mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd, MIXER_GETCONTROLDETAILSF_LISTTEXT);

    if (MMSYSERR_NOERROR != MMResult)
    {        
		mixerClose(hmx);
        return; 
    }	
	
	for (int u = 0; u < cChannels; u++)
    {
        for (int v = 0; v < cMultipleItems; v++)
        {
            uIndex = (u * cMultipleItems) + v;
			
			if(stricmp(pmxcd_lt[uIndex].szName, "mixer")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Stereo Mixer")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "스테레오 믹서")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Mixed Output")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "믹스드 아웃풋")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Stereo Mix")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "스테레오 믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Mixed Out")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "믹스드 아웃")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Stereo Output")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "스테레오 아웃풋")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "What U hear")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "왓 유 히어")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Stereo Out")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "스테레오 아웃")==0 ||			
			   stricmp(pmxcd_lt[uIndex].szName, "Loop back")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "루프 백")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Digital Mixe")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "디지털 믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Post-Mix")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "포스트 믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Digital Mix")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "Sum")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "섬")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "wave out mix")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "웨이브 아웃 믹스")==0 ||
			   stricmp(pmxcd_lt[uIndex].szName, "웨이크 출력 믹스")==0)
			{
				iFinalIndex = uIndex;				
				break;
			}
        }		
    }

	
	PMIXERCONTROLDETAILS_BOOLEAN    pmxcd_f;
	pmxcd_f = (PMIXERCONTROLDETAILS_BOOLEAN) new MIXERCONTROLDETAILS_BOOLEAN[20];
			
	mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pamxctrl[0].dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = pamxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_f);
    mxcd.paDetails      = pmxcd_f;
		
    MMResult = mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);
    if (MMSYSERR_NOERROR != MMResult)
    {      
        mixerClose(hmx);      
		LocalFree((HLOCAL)pmxcd_lt);  		
		
		free(pmxcd_f);
        return;
    }	

	///////////////////////////////////////////////////////////////////////////////////////////////
	mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pamxctrl->dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = pamxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_f);
    mxcd.paDetails      = pmxcd_f;
	
    MMResult = mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd, 0L);
    if (MMSYSERR_NOERROR != MMResult)
    {
		mixerClose(hmx);		
        return; 
    }
	
	if (fSingleSelect)
    {
        UINT        u;
        UINT        v;
        UINT        cMultipleItems;

        cMultipleItems = (UINT)pamxctrl->cMultipleItems;		
        u = uIndex / (cChannels * cMultipleItems);
		
        for (v = 0; v < cMultipleItems; v++)
        {
            pmxcd_f[(u * cMultipleItems) + v].fValue = FALSE;
        }
    }
	
    pmxcd_f[iFinalIndex].fValue = TRUE;		

    MMResult = mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd, 0L);
    if (MMSYSERR_NOERROR != MMResult)
    {
        mixerClose(hmx);
		delete [] pmxcd_f;
		
		LocalFree((HLOCAL)pmxcd_lt);		
		return;
    }

	mixerClose(hmx);
	delete [] pmxcd_f;	
	LocalFree((HLOCAL)pmxcd_lt);

	m_nCheckIndex = 1;
	InvalidateRect(CRect(10, 200, 406, 260));

	return;
}