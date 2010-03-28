//////////////////////////////////////////////////////////////////////
//
// By Raguru (2003. 8. 12)
//
// The Product Code Generator for BCLSoft Corp.
//
// Product Code = Product number + Registration number
//
//////////////////////////////////////////////////////////////////////

#ifndef _ProductCode_H_
#define _ProductCode_H_

#ifdef __AFX_H__
#include "Afxtempl.h"
#endif

#pragma once

class CProductCode  
{
public:
#ifdef __AFX_H__
	bool GenerateCodeList(LPCTSTR pszPrefix, int nStart, int nCount, CList<CString,CString&> *pCodeList);
#endif

	bool GenerateCode(LPCTSTR pszPrefix, LPTSTR pszCode);

	bool CheckCode(LPCTSTR pszCode, int nPrefixLen, int nCodeLen);

	CProductCode();
	virtual ~CProductCode();

private:
	WORD CalcCRC(LPCTSTR pszFrame, int nFrameLen);
	WORD ExtractCRC(LPCTSTR pszCode, int nCodeLen);

};

#endif // !_ProductCode_H_
