#pragma once


// CSoftKeyMenu dialog

class CSoftKeyMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftKeyMenu)

public:
	BYTE vMenuType;
	BYTE vResultKey;
	BYTE vDisableKeys;
	bool vOnMainMenu;
	CString vLanguageString;
	tCWndCollection vLocalCWndCollection;
	
	CSoftKeyMenu(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSoftKeyMenu();
	void SendSoftKey(BYTE TempKey);

// Dialog Data
	enum { IDD = IDD_SoftKeyMenu };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCancel();
	afx_msg void OnSoftkey1();
	afx_msg void OnSoftkey2();
	afx_msg void OnSoftkey3();
	afx_msg void OnSoftkey4();
	afx_msg void OnSoftkey5();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CStatic m_SoftKey1;
	CStatic m_SoftKey2;
	CStatic m_SoftKey3;
	CStatic m_SoftKey4;
	CStatic m_SoftKey5;
	CStatic m_Cancel;
};
