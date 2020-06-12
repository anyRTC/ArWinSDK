// AGDesktopCaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ARMeeting.h"
#include "AGDesktopCaptureDlg.h"
#include "afxdialogex.h"


// CAGDesktopCaptureDlg 对话框

IMPLEMENT_DYNAMIC(CAGDesktopCaptureDlg, CDialogEx)

CAGDesktopCaptureDlg::CAGDesktopCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAGDesktopCaptureDlg::IDD, pParent)
	, m_bMouseLDown(FALSE)
{

}

CAGDesktopCaptureDlg::~CAGDesktopCaptureDlg()
{
}

void CAGDesktopCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAGDesktopCaptureDlg, CDialogEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CAGDesktopCaptureDlg 消息处理程序
BOOL CAGDesktopCaptureDlg::SaveScreen(LPCRECT lpRect)
{
	CDC			dcDesktop;
	CRect		rcDesktop;
	CRect		rcDest;

	CBitmap		*lpOldBitmap = NULL;

	CWnd	*lpWndDesktop = GetDesktopWindow();
	CDC		*lpDCDesktop = lpWndDesktop->GetDC();

	lpWndDesktop->GetWindowRect(&rcDesktop);

	if (lpRect == NULL)
		rcDest.CopyRect(&rcDesktop);
	else {
		if (lpRect->left < rcDesktop.left || lpRect->top < rcDesktop.top || lpRect->right > rcDesktop.right || lpRect->bottom > rcDesktop.bottom)
			return FALSE;
		else
			rcDest.CopyRect(lpRect);
	}

	dcDesktop.CreateCompatibleDC(lpDCDesktop);

	if (m_bmpDesktop.GetSafeHandle() != NULL)
		m_bmpDesktop.DeleteObject();

	m_bmpDesktop.CreateCompatibleBitmap(lpDCDesktop, rcDest.Width(), rcDest.Height());
	lpOldBitmap = dcDesktop.SelectObject(&m_bmpDesktop);
	dcDesktop.BitBlt(0, 0, rcDesktop.Width(), rcDesktop.Height(), lpDCDesktop, 0, 0, SRCCOPY);
	dcDesktop.SelectObject(lpOldBitmap);

	return TRUE;
}

void CAGDesktopCaptureDlg::OnPaint()
{
	CPaintDC	dc(this);
	CDC			dcBuffer;
	CBitmap		bmpBuffer;
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CRect			rcWindow;
	BLENDFUNCTION	blend;
	GetWindowRect(&rcWindow);

	CDC dcBufferB;
	dcBufferB.CreateCompatibleDC(&dc);
	dcBufferB.SelectObject(&m_bmpDesktop);

	//CRect rcWindow;
	//GetWindowRect(&rcWindow);
	dc.BitBlt(0, 0, rcWindow.Width(), rcWindow.Height(), &dcBufferB, 0, 0, SRCCOPY);


	CBrush brBack;
	brBack.CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	dcBuffer.CreateCompatibleDC(&dc);
	bmpBuffer.CreateCompatibleBitmap(&dc, rcWindow.Width(), rcWindow.Height());

	CBitmap *lpOldBitmap = dcBuffer.SelectObject(&bmpBuffer);
	dcBuffer.FillRect(&rcWindow, &brBack);

	blend.BlendOp = AC_SRC_OVER;
	blend.AlphaFormat = 0;
	blend.SourceConstantAlpha = 0x80;
	blend.BlendFlags = 0;

	dc.AlphaBlend(0, 0, rcWindow.Width(), rcWindow.Height(), &dcBuffer, 0, 0, rcWindow.Width(), rcWindow.Height(), blend);

	if (m_bMouseLDown) {
		CDC		dcBitmap;
		CPen	penMark;
		CString	strTip;

		penMark.CreatePen(PS_SOLID, 4, RGB(0x00, 0xA0, 0xE9));

		dcBitmap.CreateCompatibleDC(&dc);
		dcBitmap.SelectObject(&m_bmpDesktop);
		//dc.BitBlt(m_rcRegionMax.left, m_rcRegionMax.top, m_rcRegionMax.Width(), m_rcRegionMax.Height(), &dcBitmap, m_rcRegionMax.left, m_rcRegionMax.top, SRCCOPY);
		dc.BitBlt(m_rcRegion.left, m_rcRegion.top, m_rcRegion.Width(), m_rcRegion.Height(), &dcBitmap, m_rcRegion.left, m_rcRegion.top, SRCCOPY);

		dc.SelectObject(&penMark);
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(&m_rcRegion);

		strTip = _T("double click to share the desktop region or ESC to abort");

		dc.SetBkColor(RGB(0x00, 0xA0, 0xE9));
		dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		dc.TextOut(m_rcRegion.left + 10, m_rcRegion.top + 10, strTip);
	}
}

void CAGDesktopCaptureDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	DESKTOP_SHARE_PARAM deskShareParam;

	deskShareParam.nX = m_rcRegion.left;
	deskShareParam.nY = m_rcRegion.top;
	deskShareParam.nWidth = m_rcRegion.Width();
	deskShareParam.nHeight = m_rcRegion.Height();
	deskShareParam.nFPS = 30;

	GetParent()->SendMessage(WM_DESKTOPSHARE, (WPARAM)&deskShareParam, NULL);

	CDialogEx::OnOK();
}

void CAGDesktopCaptureDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnOK();
}

void CAGDesktopCaptureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMouseLDown = TRUE;
	m_ptStart = point;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CAGDesktopCaptureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bMouseLDown = FALSE;
	m_ptEnd = point;


	if (m_rcRegion.right == m_rcRegion.left
		|| m_rcRegion.bottom == m_rcRegion.top){
		return;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CAGDesktopCaptureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect		rcMarkWnd;

	if (m_bMouseLDown) {
		m_ptEnd = point;

		m_rcRegion.left = m_ptStart.x < m_ptEnd.x ? m_ptStart.x : m_ptEnd.x;
		m_rcRegion.top = m_ptStart.y < m_ptEnd.y ? m_ptStart.y : m_ptEnd.y;

		m_rcRegion.right = m_ptStart.x >= m_ptEnd.x ? m_ptStart.x : m_ptEnd.x;
		m_rcRegion.bottom = m_ptStart.y >= m_ptEnd.y ? m_ptStart.y : m_ptEnd.y;

		InvalidateRect(NULL, FALSE);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CAGDesktopCaptureDlg::OnEraseBkgnd(CDC* pDC)
{
	//CDC dcBuffer;
	//dcBuffer.CreateCompatibleDC(pDC);
	//dcBuffer.SelectObject(&m_bmpDesktop);

	//CRect rcWindow;
	//GetWindowRect(&rcWindow);
	//pDC->BitBlt(0, 0, rcWindow.Width(), rcWindow.Height(), &dcBuffer, 0, 0, SRCCOPY);

	return TRUE;
}


BOOL CAGDesktopCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
