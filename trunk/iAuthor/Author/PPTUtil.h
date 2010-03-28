// PPTUtil.h: interface for the CPPTUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PPTUTIL_H__B6A08FF7_BB98_4AB0_9BC8_9DEE49E6A8EB__INCLUDED_)
#define AFX_PPTUTIL_H__B6A08FF7_BB98_4AB0_9BC8_9DEE49E6A8EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPPTUtil  
{
public:
	CPPTUtil();
	virtual ~CPPTUtil();


	static bool IsPPTActive();
	static bool CPPTUtil::GoFirstSlide();
	static bool CPPTUtil::GoPrevSlide();
	static bool CPPTUtil::GoNextSlide();
	static bool CPPTUtil::GoLastSlide();

};

#endif // !defined(AFX_PPTUTIL_H__B6A08FF7_BB98_4AB0_9BC8_9DEE49E6A8EB__INCLUDED_)
