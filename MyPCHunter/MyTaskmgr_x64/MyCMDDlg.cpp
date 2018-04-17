// MyCMDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "MyCMDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyCMDDlg dialog

CMyCMDDlg::CMyCMDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyCMDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyCMDDlg)
	m_csEnter = _T("");
	m_csShowCmd = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    hCmdReadPipe = NULL;
    hCmdWritePipe = NULL;
    hMyReadPipe = NULL;
    hMyWritePipe = NULL;

    memset(&m_pi, 0, sizeof(PROCESS_INFORMATION));
}

void CMyCMDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyCMDDlg)
	DDX_Text(pDX, EDT_ENTER, m_csEnter);
	DDX_Text(pDX, EDT_CMDSHOW, m_csShowCmd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyCMDDlg, CDialog)
	//{{AFX_MSG_MAP(CMyCMDDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(BTN_RUN, OnRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCMDDlg message handlers

BOOL CMyCMDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//…Ë÷√sa
    SECURITY_ATTRIBUTES sa;
    memset(&sa, 0, sizeof(SECURITY_ATTRIBUTES));
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    
    //create pipe
    BOOL bRet = FALSE;
//     hMyWritePipe <-> hCmdReadPipe  
//     
//     hMyReadPipe <-> hCmdWritePipe
//     
    bRet = CreatePipe(&hMyReadPipe, &hCmdWritePipe, &sa, 0);
    bRet = CreatePipe(&hCmdReadPipe, &hMyWritePipe, &sa, 0);


    //startup cmd
    STARTUPINFO si;
    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hCmdReadPipe;
    si.hStdOutput = hCmdWritePipe;
    si.hStdError = hCmdWritePipe;

    char szBuf[MAXBYTE] = {0};
    GetSystemDirectory(szBuf, MAXBYTE);

    strcat(szBuf, "\\cmd.exe");
    bRet = CreateProcess(NULL,
                        szBuf,
                        NULL,
                        NULL,
                        TRUE,
                        CREATE_NO_WINDOW,
                        NULL,
                        NULL,
                        &si,
                        &m_pi);

    if (bRet == FALSE)
    {
        AfxMessageBox("CreateProcess bRet == FALSE");
        return FALSE;
    }


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.

void CMyCMDDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//close cmd
    DWORD dwWritedBytes = 0;
    BOOL bRet = FALSE;
    bRet = WriteFile(hMyWritePipe,
                    "exit\r\n",
                    strlen("exit\r\n"),
                    &dwWritedBytes,
                    NULL
                    );

    if (hCmdReadPipe != NULL)
    {
        CloseHandle(hCmdReadPipe);
    }

    if (hCmdWritePipe != NULL)
    {
        CloseHandle(hCmdWritePipe);
    }

    if (hMyReadPipe != NULL)
    {
        CloseHandle(hMyReadPipe);
    }

    if (hMyWritePipe != NULL)
    {
        CloseHandle(hMyWritePipe);
    }
}

DWORD WINAPI GetCMDWritePipe(LPVOID lpParameter)
{
    ASSERT(lpParameter);

    PTHREADPARAM pParam = (PTHREADPARAM)lpParameter;    
    HANDLE hMyReadPipe = pParam->m_hPipe;

    char *pszBuf = NULL;
    DWORD dwTotalBytesAvail = 0;
    DWORD dwReadedBytes = 0;
    BOOL bRet = FALSE;
    DWORD dwZeroFlag = 0;

    while (TRUE)
    {
        bRet = PeekNamedPipe(hMyReadPipe,
            NULL, 
            0,
            NULL, 
            &dwTotalBytesAvail,
            NULL);        

        if (bRet == FALSE)
        {
            AfxMessageBox("PeekNamedPipe bRet == FALSE");
            return 0;
        }        
        
        if (dwTotalBytesAvail == 0 && dwZeroFlag != 0)
        {
            break;
        }
        
        if (dwTotalBytesAvail == 0 && dwZeroFlag == 0)
        {
            dwZeroFlag++;
            Sleep(200);
            continue;
        }
        
        char *pszBuf = new char[dwTotalBytesAvail];
        if (pszBuf == NULL)
        {
            AfxMessageBox("new char[dwTotalBytesAvail] pszBuf == NULL");
            return 0;
        }
        memset(pszBuf, 0, dwTotalBytesAvail);
        
        bRet = ReadFile(hMyReadPipe,
            pszBuf,
            dwTotalBytesAvail,
            &dwReadedBytes,
            NULL);
        
        if (bRet == FALSE)
        {
            AfxMessageBox("ReadFile bRet == FALSE");
            return 0;
        }

        if (dwTotalBytesAvail != 0)
        {
            *(pParam->m_pcsStr) += pszBuf;
        }              
        
        if (pszBuf != NULL)
        {
            delete pszBuf;
            pszBuf = NULL;
        }

    }

    return 0;
}

void CMyCMDDlg::OnRun() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    m_csEnter += "\r\n";
    m_csShowCmd = "";

    BOOL bRet = FALSE;
    DWORD dwReadedBytes = 0;
    DWORD dwWritedBytes = 0;

    bRet = WriteFile(hMyWritePipe,
                    m_csEnter.GetBuffer(0),
                    m_csEnter.GetLength(),
                    &dwWritedBytes,
                    NULL
                    );

    //create thread run command line
    DWORD dwTid = 0;

    PTHREADPARAM pThParam = NULL;
    pThParam = new THREADPARAM;
    if (pThParam == NULL)
    {
        AfxMessageBox("pThParam == NULL");
        return;
    }
    memset(pThParam, 0, sizeof(THREADPARAM)); 
    
    pThParam->m_hPipe = hMyReadPipe;
    pThParam->m_pcsStr = &m_csShowCmd;
    
    HANDLE hThread = NULL;
    hThread = CreateThread(NULL, 0, GetCMDWritePipe, (LPVOID)pThParam, 0, &dwTid);
    if (hThread == NULL)
    {
        AfxMessageBox("hThread == NULL");
        return;
    }

    WaitForSingleObject(hThread, INFINITE);

    UpdateData(FALSE);

    if (pThParam != NULL)
    {
        delete pThParam;
        pThParam = NULL;
    }
    
    if (hThread != NULL)
    {
        CloseHandle(hThread);
    }    
}
