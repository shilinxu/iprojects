// ProfilesFileWrite.h: interface for the CProfilesFileWrite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILESFILEWRITE_H__0104BBC6_5D8B_4D8B_B87C_2E41AB7171E5__INCLUDED_)
#define AFX_PROFILESFILEWRITE_H__0104BBC6_5D8B_4D8B_B87C_2E41AB7171E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProfilesFileWrite  
{
public:	
	CProfilesFileWrite();
	virtual ~CProfilesFileWrite();

public:
	bool WriteCBRProfile(CString strPath, int nIndex);
	bool WriteVBRProfile(CString strPath, int nWidth, int nHeight);
	bool WritePDAProfile(CString strPath);

private:
	bool Write600kStreaming(CString strPath);
	bool Write500kStreaming(CString strPath);
	bool Write400kStreaming(CString strPath);
	bool Write300kStreaming(CString strPath);
	void WriteUnicode(CFile* pFile, CString strTemp);
	bool Write100kStreaming(CString strPath);
	bool Write200kStreaming(CString strPath);
};

#endif // !defined(AFX_PROFILESFILEWRITE_H__0104BBC6_5D8B_4D8B_B87C_2E41AB7171E5__INCLUDED_)
