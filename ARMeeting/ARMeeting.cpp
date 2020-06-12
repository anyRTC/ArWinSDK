
// AgoraVideoCall.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ARMeeting.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAgoraVideoCallApp

BEGIN_MESSAGE_MAP(CARMeetingApp, CWinApp)
END_MESSAGE_MAP()


// CAgoraVideoCallApp construction

CARMeetingApp::CARMeetingApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	m_pShellManager=NULL;
	m_pDlgMain = NULL;
}


// The one and only CAgoraVideoCallApp object

CARMeetingApp theApp;


// CAgoraVideoCallApp initialization

BOOL CARMeetingApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	m_pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	DlgMain*pDlgMain = new DlgMain();
	pDlgMain->Create(DlgMain::IDD);
	pDlgMain->ShowWindow(SW_SHOW);
	m_pMainWnd = pDlgMain;
	m_pDlgMain = pDlgMain;

	return TRUE;
}



int CARMeetingApp::ExitInstance()
{
	if (m_pDlgMain) {
		delete m_pDlgMain;
		m_pDlgMain = NULL;
	}

	if (m_pShellManager) {
		delete m_pShellManager; 
		m_pShellManager = NULL;
	}


	return CWinApp::ExitInstance();
}
