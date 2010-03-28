// AppSettings.h: interface for the CAppSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPSETTINGS_H__74045652_2082_4D8A_A9A7_DA0C19BAD61F__INCLUDED_)
#define AFX_APPSETTINGS_H__74045652_2082_4D8A_A9A7_DA0C19BAD61F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAppSettings  
{
public:	
	CAppSettings();
	virtual ~CAppSettings();	

	static void Init();
	static bool IsRegistration();	
	static bool IsLayeredWndEnable();
	static CString& GetUserName();
	static CString& GetPCode();
	static CString& GetRCode();

	static CString& GetAppPath();
	static CString& GetSchemePath();
	static CString& GetHelpPath();
	static CString& GetSlidePath();
	static CString& GetBoardPath();
	static CString& GetContentsPath();
	static CString& GetWorkPath();
	static CString& GetTempPath();

	static CString& GetOptionToolbarPath();
	static CString& GetOptionRecordPath();
	static CString& GetOptionPenPath();
	static CString& GetOptionFolderPath();
	static CString& GetOptionSnapshotPath();
	static CString& GetOptionTextPath();
	static CString& GetOptionCameraPath();
	static CString& GetOptionCodecPath();
	
	static bool IsEnableSliderFolderChange();

	static bool CheckVistaDWM4Restart();
	static void SetSlidePath(CString strPath);
	static void SetContentsPath(CString strPath);
	static bool SetRegistration(LPCTSTR pszUserName, LPCTSTR pszPCode, LPCTSTR pszRCode);
	static void SetSlideFolderLock();

private:
	static bool m_bRegistration;	
	static bool m_bLayeredWndEnable;
	static	CString m_strUserName;
	static	CString m_strPCode;
	static	CString	m_strRCode;

	static CString m_strAppFilePath;	
	static CString m_strAppPath;	
	static CString m_strSchemePath;
	static CString m_strWorkPath;
	static CString m_strTempPath;
	static CString m_strContentsPath;	
	static CString m_strBoardPath;
	static CString m_strSlidePath;
	static CString m_strHelpPath;

	static CString m_strOptionToolbar;
	static CString m_strOptionRecord;
	static CString m_strOptionPen;
	static CString m_strOptionFolder;
	static CString m_strOptionSnapshot;
	static CString m_strOptionText;
	static CString m_strOptionCamera;
	static CString m_strOptionCodec;

	static bool	   m_bChangeSlideFolder;
};

#endif // !defined(AFX_APPSETTINGS_H__74045652_2082_4D8A_A9A7_DA0C19BAD61F__INCLUDED_)
