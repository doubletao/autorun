#include "StdAfx.h"
#include "TooltipsDialog.h"
#include <assert.h>
#include "GlobalFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CTooltipsDialog, CDialog)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CTooltipsDialog::OnToolTipText)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CTooltipsDialog::CTooltipsDialog(UINT nIDTemplate, CWnd* pParent/* = NULL*/)
	: CDialog(nIDTemplate, pParent)
{
}

CTooltipsDialog::~CTooltipsDialog(void)
{
}

void CTooltipsDialog::SetBackgroundImg(CString strBkImg)
{
	HRESULT hr = m_imgBk.Load(strBkImg);
	if (hr)
	{
		int i = 0;
		i++;
	}
}

BOOL CTooltipsDialog::OnToolTipText( UINT, NMHDR* pNMHDR, LRESULT* pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	UINT nID = pNMHDR->idFrom;
	if (pTTT->uFlags & TTF_IDISHWND)
	{
		nID = ::GetDlgCtrlID((HWND)nID);
		if(nID)
		{
			CString strTips(_T(""));
			if(GetToolTips(nID,strTips))
			{
				lstrcpyn(pTTT->szText, strTips, sizeof(pTTT->szText));
				return TRUE;
			}
		}
	}
	return(FALSE);
}

BOOL CTooltipsDialog::GetToolTips( UINT nID,CString &strTips )
{
	if (m_mapIDTips.count(nID) > 0)
	{
		strTips = m_mapIDTips[nID];
		return TRUE;
	}
	return FALSE;
}

void CTooltipsDialog::OnPaint()
{
	CDialog::OnPaint();
}

BOOL CTooltipsDialog::PreTranslateMessage( MSG* pMsg )
{
	if (::IsWindow(GetSafeHwnd()))
	{
		m_tt.RelayEvent(pMsg);
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTooltipsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	EnableToolTips(TRUE);
	m_tt.Create(this);
	m_tt.Activate(TRUE);
	for (map<UINT, CString>::iterator it = m_mapIDTips.begin(); it != m_mapIDTips.end(); it++)
	{
		assert(GetDlgItem(it->first));
		m_tt.AddTool(GetDlgItem(it->first), LPSTR_TEXTCALLBACK);//Ìí¼Ó
	}
	return TRUE;
}

BOOL CTooltipsDialog::OnEraseBkgnd(CDC * pDC)
{
	CDialog::OnEraseBkgnd(pDC);

	CRect rtClient;
	GetClientRect(rtClient);
	CDC dcMem;
	CBitmap bmpMem;
	dcMem.CreateCompatibleDC(NULL);
	bmpMem.CreateCompatibleBitmap(pDC, rtClient.Width(), rtClient.Height());
	dcMem.SelectObject(&bmpMem);
	if (!m_imgBk.IsNull())
	{
		m_imgBk.StretchBlt(dcMem.GetSafeHdc(), 0, 0, rtClient.Width(), rtClient.Height(), 0, 0, m_imgBk.GetWidth(), m_imgBk.GetHeight());
	}

	pDC->BitBlt(0, 0, rtClient.Width(), rtClient.Height(), &dcMem, 0, 0, SRCCOPY);

	return TRUE;
}

void CTooltipsDialog::ModifyTooltips( UINT nID, const CString &strTips )
{
	m_mapIDTips[nID] = strTips;
}

void CTooltipsDialog::RemoveTooltips( UINT nID )
{
	if(m_mapIDTips.count(nID) > 0)
	{
		m_mapIDTips.erase(nID);
	}
}