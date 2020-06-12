#pragma once
#include "AGButton.h"
#include "AGEdit.h"
#include "VideoDlg.h"


// DlgMain 对话框

class DlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMain)

public:
	DlgMain(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DlgMain();
protected:
	CAGButton		m_btnMin;
	CAGButton		m_btnClose;
	CFont			m_ftTitle;
	CAGEdit			m_ctrChannel;
	CAGButton		m_btnJoin;
	CFont			m_ftBtn;
	CPen            m_penFrame;

	CVideoDlg		m_dlgVideo;

protected:
	void DrawClient(CDC *lpDC);
	CString GetChannelName();
public:
	HICON m_hIcon;

// 对话框数据
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnclose();
	afx_msg void OnBnClickedBtnmin();
	afx_msg void OnBnClickedBtnjoinChannel();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);

	
};
