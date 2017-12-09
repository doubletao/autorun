#pragma once
#include <map>
using namespace std;

class CTooltipsDialog :
	public CDialog
{
public:
	CTooltipsDialog(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CTooltipsDialog(void);

protected:
	CToolTipCtrl m_tt;
	map<UINT, CString> m_mapIDTips;
	CImage m_imgBk;
	void SetBackBmp(CString strBkImg);
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	BOOL GetToolTips(UINT nID,CString &strTips);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void ModifyTooltips(UINT nID, const CString &strTips);
	void RemoveTooltips(UINT nID);
};
