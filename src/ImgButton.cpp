#include "StdAfx.h"
#include "ImgButton.h"
#include "GlobalFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int BTN_IMAGE_DEFAULT = 0;
static const int BTN_IMAGE_HOVER = 1;
static const int BTN_IMAGE_DOWN = 2;
static const int BTN_IMAGE_GREY = 3;

BEGIN_MESSAGE_MAP(CImgButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

CImgButton::CImgButton(void)
	: m_nCurImage(BTN_IMAGE_DEFAULT)
	, m_bIsPushButton(FALSE)
{
}

CImgButton::~CImgButton(void)
{
}

void CImgButton::SetImage( const CString & strFile )
{
	CRect rtWnd;
	GetWindowRect(rtWnd);
	m_imgList.DeleteImageList();
	m_imgList.Create(rtWnd.Width(), rtWnd.Height(), ILC_COLOR24|ILC_MASK, 0, 10);
	CString strName = CGlobalFunction::GetFilePath(strFile) + _T("\\") + CGlobalFunction::GetFileName(strFile);
	CString strExt = CGlobalFunction::GetFileNameExt(strFile);
	CString strFileName;
	CBitmap bitmap;

	strFileName.Empty();
	strFileName.Format(_T("%s_default%s"), strName, strExt);
	CGlobalFunction::LoadBmpFileToCbitmap(strFileName, bitmap);
	m_imgList.Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();

	strFileName.Empty();
	strFileName.Format(_T("%s_hover%s"), strName, strExt);
	CGlobalFunction::LoadBmpFileToCbitmap(strFileName, bitmap);
	m_imgList.Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();

	strFileName.Empty();
	strFileName.Format(_T("%s_down%s"), strName, strExt);
	CGlobalFunction::LoadBmpFileToCbitmap(strFileName, bitmap);
	m_imgList.Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();

	strFileName.Empty();
	strFileName.Format(_T("%s_grey%s"), strName, strExt);
	CGlobalFunction::LoadBmpFileToCbitmap(strFileName, bitmap);
	m_imgList.Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
}

void CImgButton::SetImage( int nIndex )
{
	if (IsWindowEnabled())
	{
		m_nCurImage = nIndex;
	}
	else
	{
		m_nCurImage = BTN_IMAGE_GREY;
	}
	Invalidate(FALSE);
}

void CImgButton::OnLButtonDown( UINT nFlags, CPoint point )
{
	//设置为down图片
	SetImage(BTN_IMAGE_DOWN);
	CButton::OnLButtonDown(nFlags, point);
}

void CImgButton::OnLButtonUp( UINT nFlags, CPoint point )
{
	if (m_bIsPushButton)
	{
		//按钮模式不会自动弹起
	}
	else
	{
		//普通模式会自动弹起，设置为hover图片
		SetImage(BTN_IMAGE_HOVER);
	}
	CButton::OnLButtonUp(nFlags, point);
}

void CImgButton::OnMouseMove( UINT nFlags, CPoint point )
{
	//设置为hover图片
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = 5;
	tme.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tme);
	CButton::OnMouseMove(nFlags, point);
}

void CImgButton::OnKillFocus( CWnd* pNewWnd )
{
	if (m_bIsPushButton)
	{
		//按钮模式不会自动弹起，保持原状态
	}
	else
	{
		//普通模式在丢失焦点后会自动弹起，设置为default图片
		SetImage(BTN_IMAGE_DEFAULT);
	}
	CButton::OnKillFocus(pNewWnd);
}

void CImgButton::OnMouseLeave()
{
	if (m_bIsPushButton)
	{
		//按钮模式在鼠标离开后，如果原来是Hover则恢复，否则无动作
		if (m_nCurImage == BTN_IMAGE_HOVER)
		{
			SetImage(BTN_IMAGE_DEFAULT);
		}
	}
	else
	{
		//普通模式在鼠标离开后会恢复到默认态，设置为default图片
		SetImage(BTN_IMAGE_DEFAULT);
	}
	CButton::OnMouseLeave();
}

void CImgButton::OnMouseHover(UINT nFlags, CPoint point)
{
	if (m_bIsPushButton)
	{
		//按钮模式需要区分如果是默认态则进入hover，否则无动作
		if (m_nCurImage == BTN_IMAGE_DEFAULT)
		{
			SetImage(BTN_IMAGE_HOVER);
		}
	}
	else
	{
		//普通模式鼠标hover后设置为hover图片
		SetImage(BTN_IMAGE_HOVER);
	}
	CButton::OnMouseHover(nFlags, point);
}

BOOL CImgButton::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	return CButton::OnCreate(lpCreateStruct);
}

void CImgButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	IMAGEINFO  imageInfo;
	m_imgList.GetImageInfo(m_nCurImage, &imageInfo);
	HBITMAP hImage = (HBITMAP)CopyImage(imageInfo.hbmImage, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
	if (hImage != NULL)
	{
		CImage image;
		image.Attach(hImage);

		CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

		CDC dcMem;
		CBitmap bmpMem;
		dcMem.CreateCompatibleDC(NULL);
		bmpMem.CreateCompatibleBitmap(pDC, imageInfo.rcImage.right - imageInfo.rcImage.left, imageInfo.rcImage.bottom - imageInfo.rcImage.top);
		dcMem.SelectObject(&bmpMem);
		image.BitBlt(dcMem.GetSafeHdc(), 0, 0, imageInfo.rcImage.right - imageInfo.rcImage.left, imageInfo.rcImage.bottom - imageInfo.rcImage.top, imageInfo.rcImage.left, imageInfo.rcImage.top);

		pDC->BitBlt(0, 0, imageInfo.rcImage.right - imageInfo.rcImage.left, imageInfo.rcImage.bottom - imageInfo.rcImage.top, &dcMem, 0, 0, SRCCOPY);

		dcMem.DeleteDC();
		image.Detach();
		DeleteObject(hImage);
		bmpMem.DeleteObject();
		dcMem.DeleteDC();
	}
}

BOOL CImgButton::EnableWindow( BOOL bEnable )
{
	BOOL bRet = CButton::EnableWindow(bEnable);
	if (!bEnable)
	{
		SetImage(BTN_IMAGE_GREY);
	}
	else
	{
		SetImage(BTN_IMAGE_DEFAULT);
	}
	return bRet;
}

void CImgButton::ResetButton()
{
	SetImage(BTN_IMAGE_DEFAULT);
}

void CImgButton::SetPushButtonMode( BOOL bPushButtonMode /*= TRUE*/ )
{
	m_bIsPushButton = bPushButtonMode;
}
