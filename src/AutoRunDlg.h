#include "TooltipsDialog.h"
#include <vector>
#include "ImgButton.h"
// AutoRunDlg.h : 头文件
//

#pragma once

struct StuButtonInfo
{
	CImgButton * pButton;
	CString strImg;
	CString strTips;
	int	x;
	int y;
	int width;
	int height;
	CString strCommand;
	StuButtonInfo()
		: pButton(nullptr)
		, x(0)
		, y(0)
		, width(10)
		, height(10)
	{
		strImg.Empty();
		strTips.Empty();
		strCommand.Empty();
	}
};

// CAutoRunDlg 对话框
class CAutoRunDlg : public CTooltipsDialog
{
// 构造
public:
	CAutoRunDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTORUN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CString m_strBkImg;
	int m_nWndWidth;
	int m_nWndHeight;
	std::vector<StuButtonInfo> m_vecButtonInfo;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void LoadConfig();
	void InitializeUI();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnRangeButtonCmds(UINT nID);
	DECLARE_MESSAGE_MAP()
};
