#pragma once
#include "afxwin.h"

class CImgButton :
	public CButton
{
	DECLARE_MESSAGE_MAP()
private:
	int m_nCurImage;
	CImageList m_imgList;
	BOOL m_bIsPushButton;//按钮模式，按下后不会自动弹起，需要通过ResetButton弹起
public:
	CImgButton(void);
	~CImgButton(void);
	void SetImage(const CString & strFile);
	void SetImage(int nIndex);
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	BOOL EnableWindow(BOOL bEnable);
	void SetPushButtonMode(BOOL bPushButtonMode = TRUE);//设置为按钮模式
	void ResetButton();//恢复button到默认态
};
