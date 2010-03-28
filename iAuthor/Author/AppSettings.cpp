// AppSettings.cpp: implementation of the CAppSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Author.h"
#include "AppSettings.h"

#include "ProductCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PRODUCT_REG_KEY		_T("Software\\MMCorp")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool	CAppSettings::m_bRegistration = false;
bool	CAppSettings::m_bLayeredWndEnable = false;
CString CAppSettings::m_strUserName;
CString CAppSettings::m_strPCode;
CString CAppSettings::m_strRCode;
CString CAppSettings::m_strAppFilePath;
CString CAppSettings::m_strAppPath;
CString CAppSettings::m_strSchemePath;
CString CAppSettings::m_strWorkPath;
CString CAppSettings::m_strTempPath;
CString CAppSettings::m_strContentsPath;
CString CAppSettings::m_strBoardPath;
CString CAppSettings::m_strSlidePath;
CString CAppSettings::m_strHelpPath;

CString CAppSettings::m_strOptionToolbar;
CString CAppSettings::m_strOptionRecord;
CString CAppSettings::m_strOptionPen;
CString CAppSettings::m_strOptionFolder;
CString CAppSettings::m_strOptionSnapshot;
CString CAppSettings::m_strOptionText;
CString CAppSettings::m_strOptionCamera;
CString CAppSettings::m_strOptionCodec;
bool	CAppSettings::m_bChangeSlideFolder;

CAppSettings::CAppSettings()
{

}

CAppSettings::~CAppSettings()
{

}

void CAppSettings::Init()
{
	char szTemp[MAX_PATH];
	CString strPath;
	GetModuleFileName(AfxGetInstanceHandle(), szTemp, MAX_PATH);
	strPath = szTemp;
	
	m_strAppFilePath = m_strAppPath = strPath;
	int nLength = m_strAppPath.GetLength();
	int nCount = m_strAppPath.ReverseFind('\\');
	m_strAppPath.Delete(nCount, nLength-nCount);
		
	m_strWorkPath.Format("%s\\Work", m_strAppPath);	
	m_strTempPath.Format("%s\\Temp", m_strAppPath);	
	m_strContentsPath.Format("%s\\Contents", m_strAppPath);	
	m_strBoardPath.Format("%s\\Board", m_strAppPath);
	m_strSlidePath.Format("%s\\Slide", m_strAppPath);
	m_strHelpPath.Format("%s\\Help\\%s", m_strAppPath, "Help.htm");		
	m_strSchemePath.Format("%s\\%s", m_strWorkPath, "Scheme.txt");	
		
	m_strOptionToolbar = "iAuthor\\Option\\Toolbar";
	m_strOptionRecord = "iAuthor\\Option\\Record";
	m_strOptionPen = "iAuthor\\Option\\Pen";
	m_strOptionFolder = "iAuthor\\Option\\Folder";
	m_strOptionSnapshot = "iAuthor\\Option\\Snapshot";
	m_strOptionText = "iAuthor\\Option\\Text";
	m_strOptionCamera = "iAuthor\\Option\\Camera";
	m_strOptionCodec = "iAuthor\\Option\\Codec";

	m_bChangeSlideFolder = true;

	OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;
    
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
    {
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		{
            m_bLayeredWndEnable = false;
			return;
		}
    }
    
	if((osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) ||	// Microsoft Windows 95
        (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) ||	// Microsoft Windows 98
        (osvi.dwMajorVersion <= 4 && osvi.dwMinorVersion == 90))	// Microsoft Windows Me
		m_bLayeredWndEnable = false;
	else
		m_bLayeredWndEnable = true;

	m_strUserName = "";
	m_strPCode = "";
	m_strRCode = "";

	HKEY hKey;
	char szUserName[MAX_PATH];
	char szProductCode[MAX_PATH];
	char szRegistryCode[MAX_PATH];	

    DWORD dwBufLen = MAX_PATH;
    if(ERROR_SUCCESS == RegOpenKeyEx( HKEY_CLASSES_ROOT, PRODUCT_REG_KEY, 0, KEY_QUERY_VALUE, &hKey))
	{	
		RegQueryValueEx( hKey, "UName", NULL, NULL, (LPBYTE)szUserName, &dwBufLen);
		m_strUserName.Format("%s", szUserName);	

		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "PCode", NULL, NULL, (LPBYTE)szProductCode, &dwBufLen);
		m_strPCode.Format("%s", szProductCode);

		dwBufLen = MAX_PATH;
		RegQueryValueEx( hKey, "RCode", NULL, NULL, (LPBYTE)szRegistryCode, &dwBufLen);
		m_strRCode.Format("%s", szRegistryCode);
		
        RegCloseKey( hKey );
	
		m_strPCode.MakeUpper();		
		if (m_strPCode.Find(_T("APREG"), 0) == -1)
		{
			m_bRegistration = false;		
			return;
		}
		else
		{
			CProductCode pc;
			CString strSerialCode;
			strSerialCode.Format("%s-%s", m_strPCode, m_strRCode);

			bool ret = pc.CheckCode((LPCTSTR) strSerialCode, 10, strSerialCode.GetLength());	
			if (ret)			
				m_bRegistration = true;
			else				
				m_bRegistration = false;
		}
	}
	else
		m_bRegistration = false;
}

// disable DWM
// HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers
// full_path DISABLEDWM WINXPSP2 DISABLETHEMES HIGHDPIAWARE
bool CAppSettings::CheckVistaDWM4Restart()
{
	HKEY hKey;
	char szElement[MAX_PATH] = {0};;
	CString strElement;
	CString strValue = "WINXPSP2 DISABLEDWM";	//"WINXPSP2 DISABLEDWM";
	CString strRegKey = "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers";

	OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
    {
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (!GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return false;
    }
	if(osvi.dwMajorVersion != 6)			// Microsoft Windows Vista
		return false;

	bool bNeedRestart = false;
    DWORD dwBufLen = MAX_PATH;
    if (ERROR_SUCCESS != RegOpenKeyEx( HKEY_CURRENT_USER, strRegKey, 0, KEY_ALL_ACCESS, &hKey))
	{	
		if(ERROR_SUCCESS != RegCreateKeyEx( HKEY_CURRENT_USER, strRegKey, 0, "", 0, KEY_ALL_ACCESS, NULL, &hKey, &dwBufLen))
		{
			AfxMessageBox("ERROR! : CheckVistaDWM4Restart");
			return false;
		}
	}

	if (ERROR_SUCCESS == RegQueryValueEx( hKey, m_strAppFilePath, NULL, NULL, (LPBYTE)szElement, &dwBufLen))
		strElement.Format("%s", szElement);	

	if (strElement.IsEmpty())
	{
		if (ERROR_SUCCESS == RegSetValueEx(hKey, m_strAppFilePath, 0L, REG_SZ, (CONST BYTE *)(LPCTSTR)strValue, strValue.GetLength() + 1))	
			bNeedRestart = true;
	}

    RegCloseKey( hKey );

	return bNeedRestart;
}

bool CAppSettings::SetRegistration(LPCTSTR pszUserName, LPCTSTR pszPCode, LPCTSTR pszRCode)
{	
	CProductCode pc;
	CString strSerialCode;
	strSerialCode.Format("%s-%s", pszPCode, pszRCode);

	bool ret = pc.CheckCode((LPCTSTR) strSerialCode, 10, strSerialCode.GetLength());	
	if (!ret)
		return false;
	
	m_bRegistration = true;

	HKEY hKey;		
	DWORD dw;

	if(ERROR_SUCCESS == RegCreateKeyEx(HKEY_CLASSES_ROOT, PRODUCT_REG_KEY, 0, "", 0, KEY_READ | KEY_WRITE, NULL, &hKey, &dw))
	{
		RegSetValueEx(hKey, "UName", 0L, REG_BINARY, (CONST BYTE *) pszUserName, _tcslen(pszUserName) + 1);
		RegSetValueEx(hKey, "PCode", 0L, REG_BINARY, (CONST BYTE *) pszPCode, _tcslen(pszPCode) + 1);
		RegSetValueEx(hKey, "RCode", 0L, REG_BINARY, (CONST BYTE *) pszRCode, _tcslen(pszRCode) + 1);

		RegCloseKey( hKey );

		m_strUserName = pszUserName;
		m_strPCode = pszPCode;
		m_strRCode = pszRCode;

		return true;
	}

	return false;
}


void CAppSettings::SetSlidePath(CString strPath)
{
	m_strSlidePath = strPath;	
}

void CAppSettings::SetContentsPath(CString strPath)
{
	m_strContentsPath = strPath;
}

void CAppSettings::SetSlideFolderLock()
{
	m_bChangeSlideFolder = false;
}

bool CAppSettings::IsEnableSliderFolderChange()
{
	return	m_bChangeSlideFolder;
}
/////////////////////////////////////////////////////////////////////////////
//
bool CAppSettings::IsRegistration()
{
	return m_bRegistration;
}

bool CAppSettings::IsLayeredWndEnable()
{
	return m_bLayeredWndEnable;
}

CString& CAppSettings::GetUserName()
{
	return	m_strUserName;
}


CString& CAppSettings::GetPCode()
{
	return	m_strPCode;
}


CString& CAppSettings::GetRCode()
{
	return	m_strRCode;
}


CString& CAppSettings::GetSchemePath()
{
	return m_strSchemePath;
}


CString& CAppSettings::GetAppPath()
{
	return m_strAppPath;
}


CString& CAppSettings::GetWorkPath()
{
	return m_strWorkPath;
}


CString& CAppSettings::GetTempPath()
{
	return m_strTempPath;
}


CString& CAppSettings::GetContentsPath()
{
	return m_strContentsPath;
}


CString& CAppSettings::GetSlidePath()
{
	return	m_strSlidePath;
}

CString& CAppSettings::GetBoardPath()
{
	return m_strBoardPath;
}

CString& CAppSettings::GetHelpPath()
{
	return	m_strHelpPath;
}

CString& CAppSettings::GetOptionToolbarPath()
{
	return m_strOptionToolbar;	
}

CString& CAppSettings::GetOptionRecordPath()
{
	return m_strOptionRecord;
}

CString& CAppSettings::GetOptionPenPath()
{	
	return m_strOptionPen;
}

CString& CAppSettings::GetOptionFolderPath()
{	
	return m_strOptionFolder;
}

CString& CAppSettings::GetOptionSnapshotPath()
{
	return m_strOptionSnapshot;
}

CString& CAppSettings::GetOptionTextPath()
{
	return m_strOptionText;
}

CString& CAppSettings::GetOptionCameraPath()
{
	return m_strOptionCamera;
}


CString& CAppSettings::GetOptionCodecPath()
{
	return m_strOptionCodec;
}


