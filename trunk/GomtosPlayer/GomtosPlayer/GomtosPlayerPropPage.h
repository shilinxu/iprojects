#pragma once

// GomtosPlayerPropPage.h : CGomtosPlayerPropPage �Ӽ� ������ Ŭ������ �����Դϴ�.


// CGomtosPlayerPropPage : ������ ������ GomtosPlayerPropPage.cpp��(��) �����Ͻʽÿ�.

class CGomtosPlayerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CGomtosPlayerPropPage)
	DECLARE_OLECREATE_EX(CGomtosPlayerPropPage)

// �������Դϴ�.
public:
	CGomtosPlayerPropPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PROPPAGE_GOMTOSPLAYER };

// �����Դϴ�.
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �޽��� ���Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

