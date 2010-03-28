//////////////////////////////////////////////////////////////////////
//
// By Raguru (2003. 8. 12)
//
// The Product Code Generator for BCLSoft Corp.
//
// Product Code = Product number + Registration number
//
//////////////////////////////////////////////////////////////////////

//#ifndef __AFXWIN_H__
#include "stdafx.h"
//#endif

#include <stdlib.h>
#include <math.h>
#include <tchar.h>
#include <strsafe.h>
#include <rpc.h>
#include <assert.h>

#include "ProductCode.h"
#include "Crc16.h"

#define MAX_PREFIX_LEN			(16 + 1)
#define MAX_UUID_LEN			(36 + 1)
#define MAX_FRAME_LEN			(MAX_PREFIX_LEN + 34 + 1 + 1)
#define MAX_PROD_CODE_LEN		(MAX_FRAME_LEN + 1 + 4)
#define CRC_REV_INDEX			30

//
// code : PREFIX_CODE(Product Name + Serial Number) + '-' +
//        uuid(8 chars) + CRC(4 chars) + '-' +
//        uuid(4+4+4 chars) + '-' +
//        uuid(12 chars)
//
//   ex : "BBEDU00031-D91205D674AE-66D64810A942-8BE6B39B228C"
//         ----------         ----
//         PREFIX_CODE        CRC16
//                    --------     -------------------------
//                         UUID
//


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProductCode::CProductCode()
{

}

CProductCode::~CProductCode()
{

}

#ifdef __AFX_H__
bool CProductCode::GenerateCodeList(LPCTSTR pszPrefix, int nStart, int nCount, CList<CString,CString&> *pCodeList)
{
	TCHAR szCode[MAX_PROD_CODE_LEN];
	CString strFinalCode;

	ASSERT(nCount <= 99999);

	for (int i=0; i<nCount; i++)
	{
		CString strPrefix;
		strPrefix.Format(_T("%s%05d"), pszPrefix, nStart+i);
	
		memset((void *) szCode, 0, MAX_PROD_CODE_LEN);
		if (!GenerateCode(strPrefix, szCode)) return false;

		strFinalCode = szCode;
		pCodeList->AddTail(strFinalCode);
	}

	return true;
}
#endif

bool CProductCode::GenerateCode(LPCTSTR pszPrefix, LPTSTR pszCode)
{
	UUID uuid;
	unsigned char *pszUuid;
	TCHAR szNewUuid[MAX_UUID_LEN];
	TCHAR szFrame[MAX_FRAME_LEN];
	int idx = 0;

	// uuid(36 chars) : 8 - 4 - 4 - 4 - 12
	if (UuidCreate(&uuid) == RPC_S_OK)
	{
		if (UuidToString(&uuid, &pszUuid) != RPC_S_OK)
			return false;

		// to upper case
		_tcsupr((TCHAR *) pszUuid);
		_tcsupr((TCHAR *) pszPrefix);

		// reformat uuid string
		memset((void *)szNewUuid, 0, MAX_UUID_LEN);
		_tcsncpy(&szNewUuid[0], (LPCTSTR) &pszUuid[0], 8);
		szNewUuid[8] = '-';
		_tcsncpy(&szNewUuid[8+1], (LPCTSTR) &pszUuid[8+1], 4);
		_tcsncpy(&szNewUuid[8+1+4], (LPCTSTR) &pszUuid[8+1+5], 4);
		_tcsncpy(&szNewUuid[8+1+4+4], (LPCTSTR) &pszUuid[8+1+5+5], 4);
		szNewUuid[8+1+4+4+4] = '-';
		_tcsncpy(&szNewUuid[8+1+4+4+4+1], (LPCTSTR) &pszUuid[8+1+5+5+5], 12);

		// make frame string
		int nPrefixLen = _tcslen((TCHAR *) pszPrefix);
		int nFrameLen = nPrefixLen+34+1;
		memset((void *)szFrame, 0, MAX_FRAME_LEN);
		_tcsncpy(szFrame, pszPrefix, nPrefixLen);
		szFrame[nPrefixLen] = '-';
		_tcsncpy(&szFrame[nPrefixLen+1], szNewUuid, 34);


		// crc
		WORD crc = CalcCRC(szFrame, nFrameLen);

		// make final product code string
		// 
		_tcscpy(pszCode, pszPrefix);
		pszCode[nPrefixLen] = '-';

		// 8 + crc
		TCHAR tmp[10]; tmp[2] = 0;
		_tcsncpy(&pszCode[nPrefixLen+1], szNewUuid, 8);

		_itoa(HIBYTE(crc), tmp, 16); if (_tcslen(tmp) == 1) {tmp[1] = tmp[0]; tmp[0] = '0';}
		_tcsupr((TCHAR *) tmp);
		_tcsncpy(&pszCode[nPrefixLen+1+8], tmp, 2);
		_itoa(LOBYTE(crc), tmp, 16); if (_tcslen(tmp) == 1) {tmp[1] = tmp[0]; tmp[0] = '0';}
		_tcsupr((TCHAR *) tmp);
		_tcsncpy(&pszCode[nPrefixLen+1+8+2], tmp, 2);

		idx = nPrefixLen+1+8+2+2;
		_tcscpy(&pszCode[idx], &szNewUuid[8]);

		RpcStringFree(&pszUuid);
	}

	return true;
}

WORD CProductCode::CalcCRC(LPCTSTR pszFrame, int nFrameLen)
{
	assert(nFrameLen < MAX_FRAME_LEN);

	WORD crc = 0;
	BYTE *pByte = (BYTE *) pszFrame;

	crc = CCrc16::MakeCRC16((BYTE *) pszFrame, 0, nFrameLen);

	return crc;
}

WORD CProductCode::ExtractCRC(LPCTSTR pszCode, int nCodeLen)
{
	assert(nCodeLen < MAX_PROD_CODE_LEN);

	TCHAR szTmp[10];

	memset((void *) szTmp, 0, sizeof szTmp);
	_tcsncpy(szTmp, &pszCode[nCodeLen - CRC_REV_INDEX], 4);
	int dz[4];
	WORD crc = 0;

	if (szTmp[0] == '0' || szTmp[3] == '0')
	{
		int i;
		i = 1;
	}
	for (int i=3; i>=0; i--)
	{
		if (_istdigit(szTmp[i])) dz[i] = szTmp[i] - '0';
		else					 dz[i] = szTmp[i] - 'A' + 10;

		dz[i] *= int( (double) pow((double) 16, 3-i) );
		crc += dz[i];
	}

	return crc;
}


//
// pszCode : SRECSDK00001-147AA447D127-86AE49659587-924D3BEFF365
//           <----------> - nPrefexLen
//           <-------------------------------------------------> - nCodeLen
//
bool CProductCode::CheckCode(LPCTSTR pszCode, int nPrefixLen, int nCodeLen)
{
	assert(nCodeLen < MAX_PROD_CODE_LEN);
	assert(nPrefixLen < MAX_PREFIX_LEN);

	TCHAR szNewCode[MAX_PROD_CODE_LEN];
	int idx = 0;

	TCHAR * pszCodeTmp = new TCHAR[nCodeLen+1];
	_tcsncpy(pszCodeTmp, pszCode, nCodeLen+1);	// prefix + '-' + uuid(8)
	_tcsupr((TCHAR *) pszCodeTmp);

	// reformat product code string to excludeing CRC code
	memset((void *)szNewCode, 0, MAX_PROD_CODE_LEN);
	_tcsncpy(&szNewCode[0], (LPCTSTR) &pszCodeTmp[0], nPrefixLen+1+8);	// prefix + '-' + uuid(8)
	_tcsncpy(&szNewCode[nPrefixLen+1+8], (LPCTSTR) &pszCodeTmp[nPrefixLen+1+8+4], 26);	// remaining string

	WORD oldCrc = ExtractCRC(pszCodeTmp, nCodeLen);
	WORD newCrc = CalcCRC(szNewCode, nCodeLen-4);
	if (oldCrc != newCrc)
		return false;

	return true;
}
