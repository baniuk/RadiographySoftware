// RadioGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RadioGUI.h"
#include "RadioGUIDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define NO_REGISTRY		// jesli zdefiniowane to addip nie uzywa rejestru - powinno byc zdefiniowane do prob pod VS

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRadioGUIDlg dialog




CRadioGUIDlg::CRadioGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRadioGUIDlg::IDD, pParent)
{
	HKEY hKey;
	LONG lResult;
	DWORD dwSize = sizeof(dirp) - 1;
//Sleep(5000);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	interpreter = new C_InterpBase(this);
//	strcpy_s(dirp,1024,"c:\\addip");
	lResult = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\PSZ\\ADDIP"), 0, KEY_READ, &hKey);
#ifndef NO_REGISTRY
	if (lResult != ERROR_SUCCESS) 
    {
        MessageBox("Registry key not found. Install ADDIP again");
		strcpy_s(dirp,1024,"c:\\");
	} else	{
//		lResult = RegGetValue(hKey,NULL,"InstallDir",RRF_RT_REG_SZ,NULL,dirp,&dwSize);
		lResult = RegQueryValueEx(hKey,"InstallDir",NULL,NULL,(LPBYTE)dirp,&dwSize);
		if (lResult != ERROR_SUCCESS) 
		{
			MessageBox("Registry value not found. Install ADDIP again");
			strcpy_s(dirp,1024,"c:\\");
		}
	}
#endif
	RegCloseKey(hKey);

/*#ifdef PRZEMEK_VERSION	
	isparameter = TRUE;
	parameter = new char[14];
	parameter = "c:\\input.fmf";
	actline = 0;
	licznik = 0;
	test = NULL;*/
//#else
	licznik = 0;
	actline = 0;
	test = NULL;
	CString szArglist;
	CString param;
	int poz;
	int pointexe;
	int options;
	int nArgs=2;
	int delta=0;
	szArglist = GetCommandLine();

//		FILE *stream;
//		fopen_s( &stream, "c:\\cc.txt", "w" );
//		fprintf_s(stream,"%s\n",szArglist);
//		fclose(stream);

	szArglist.MakeLower();
	poz = szArglist.Find("addip_d.exe");
	delta = 7+4;
	if(poz<0)	{
		poz = szArglist.Find("addip.exe");
		delta = 9;
	}
//	pointexe = szArglist.Find(".exe");
//	if(pointexe>=0)
//		delta+=4;
	options = szArglist.Find("-debug");
	param = szArglist.Mid(poz+delta,szArglist.GetLength()-poz);
	param.TrimLeft();
	param.TrimLeft('"');
	param.TrimLeft();

	param.TrimRight();
	param.TrimRight('"');
	param.TrimRight();

	if(param.GetLength()>0)	{
		isparameter = TRUE;
		parameter = new char[param.GetLength()+1];
		strcpy_s(parameter,param.GetLength()+1,param.GetBuffer());



	} else {
		isparameter = FALSE;
		parameter = NULL;
	}


//	if( poz>=0 && options>=0)	{
//		isparameter = TRUE;
//		poz+=delta;		// !!
		
/*		size_t dl = strlen(point);
		if(dl>0)	{
			parameter = new char[dl+1];
			strcpy_s(parameter,dl+1,point);
			while(parameter[0]==' ')
				parameter++;
			if(parameter[0]=='"') {
				parameter++;
				dl = strlen(parameter);
				parameter[dl-1] = '\0';
			}
		}
		else	{
			isparameter = FALSE;
			parameter = NULL;
		}*/
//	}
//	else	{
//		isparameter = FALSE;
//		parameter = NULL;
//	}
	if(options>0)	{
		isdebug = TRUE;
		debugparam = new char[256];
//		strcpy_s(debugparam,256,options);
		_getcwd(dirp,1024);	// jesli opcja -debug to nie uzywa rejestru do okreœlenia lokalizacji programu
	} else	{
		isdebug = FALSE;
		debugparam = NULL;
	}
#ifdef NO_REGISTRY		// zdefiniowane dla trybu debug zeby nie gryz³o sie z podawaniem paramtreów
_getcwd(dirp,1024);	// jesli opcja -debug to nie uzywa rejestru do okreœlenia lokalizacji programu
#endif

	interpreter->SetDLLDir(dirp);	// ustawianie katalogu z programem
	GetNumberDLL(dirp);
//#endif
}
CRadioGUIDlg::~CRadioGUIDlg(void)
{
	delete interpreter;
	SAFE_DELETE(test);
	SAFE_DELETE(parameter);
	SAFE_DELETE(debugparam);
}
void CRadioGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, listcontrol);
	DDX_Control(pDX, IDC_CLEAR, m_clear);
	DDX_Control(pDX, IDC_RUN, m_run);
	DDX_Control(pDX, IDC_LOAD, m_load);
	DDX_Control(pDX, IDC_STEP, m_step);
	DDX_Control(pDX, IDC_OPENWINDOW, m_openwindow);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_Rerun, m_rerun);
}

BEGIN_MESSAGE_MAP(CRadioGUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CRadioGUIDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_LOAD, &CRadioGUIDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_RUN, &CRadioGUIDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_CLEAR, &CRadioGUIDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_STEP, &CRadioGUIDlg::OnBnClickedStep)
	ON_BN_CLICKED(IDC_OPENWINDOW, &CRadioGUIDlg::OnBnClickedOpenwindow)
	ON_BN_CLICKED(IDC_INFO, &CRadioGUIDlg::OnBnClickedInfo)
	ON_BN_CLICKED(IDC_CLS, &CRadioGUIDlg::OnBnClickedCls)
	ON_BN_CLICKED(IDC_Rerun, &CRadioGUIDlg::OnBnClickedRerun)
	ON_LBN_SELCHANGE(IDC_LIST1, &CRadioGUIDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CRadioGUIDlg message handlers

BOOL CRadioGUIDlg::OnInitDialog()
{
	int numoflib;
	CDialog::OnInitDialog();
	CString tmpstr;
	COLORREF kolor;
	kolor = 0x00000000;

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
	char buf[10];
	char buf1[255];
	_ltoa_s(BUILD_NUMBER,buf,10,10);
#ifdef DEBUG
	sprintf_s(buf1,255,"ADDIP_D ver. %s, build %s",VERSION,buf);
#else
	sprintf_s(buf1,255,"ADDIP ver. %s, build %s",VERSION,buf);
#endif
	SetWindowText(buf1);

	m_clear.EnableWindow(0);
	m_run.EnableWindow(0);
	m_step.EnableWindow(0);
	m_rerun.EnableWindow(0);
	listcontrol.SetHorizontalExtent(545);
	m_list2.AddString("IDLE");
	numoflib = interpreter->DLL_Container.lastdllindex;
	for(int a=0;a<numoflib;a++)	{
		tmpstr.Format(_T(" - %s loaded (%s)"),interpreter->DLL_Container.dllnameholder[a].FunctionName,interpreter->DLL_Container.dllnameholder[a].DLLName);
		listcontrol.InsertString(0,tmpstr,kolor);
	}
	tmpstr.Format(_T("Number of external functions: %d"),numoflib);
	listcontrol.InsertString(0,tmpstr,kolor);
	tmpstr.Format(_T("Working direcory: %s"),dirp);
	listcontrol.InsertString(0,tmpstr,RGB(200,200,200));
	if(isparameter && !isdebug)	{
		if(Load(parameter))
			OnBnClickedRun();
	}

/*	if(isdebug)	{
		CRect rect(0,0,100,100);
		EditDialog.Create("test",WS_CAPTION|WS_VISIBLE|WS_SYSMENU|CS_OWNDC|WS_THICKFRAME|WS_MINIMIZEBOX,this,0);
		EditDialog.mydialog.Create(WS_CAPTION|WS_VISIBLE,rect,0,0);
	}*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRadioGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRadioGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRadioGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRadioGUIDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnBnClickedClear();
	OnCancel();
//	FreeConsole();
}

void CRadioGUIDlg::OnBnClickedLoad()
{
	
	int k=0;
	CString title("Open Script");
	char szFilters[]={"Script file (*.sr)|*.sr|All Files (*.*)|*.*||"};
	char roz[] = "sr";


	char nazwa[256];

	CFileDialog dlg(TRUE, _T("*.fmf"), _T(""), OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY, _T("Batch File (*.fmf) |*.fmf|All Files (*.*)|*.*||"),NULL);
	dlg.m_ofn.lpstrTitle= title;
	if (dlg.DoModal() == IDOK) {
		CString path= dlg.GetPathName(); 
		int zm1 = path.GetLength();
		const char *fil=path.GetString();
		
		for (int j=0;j<zm1;j++)
		{
			nazwa[j]=*(fil++);
		}
		
		for (int j=0;j<zm1;j++)
		{
			nazwa1[k++]=nazwa[j];
			if (nazwa[j]=='\\')
			{
				nazwa1[k++]='\\';
				
			}
		}
	}
	else
		return;
	nazwa1[k]=NULL;
//	(char*)nazwa=(char*)nazwa1;
	Load(nazwa1);
}

BOOL CRadioGUIDlg::LoadOneLine(FILE *pFile, char* bufor)
{
	BOOL status=TRUE;
	BOOL ret = TRUE;
	int licznik = 0;
	char cc;
	size_t siz;
	do	{
		siz=fread((void*)&cc,sizeof(char),1,pFile);
		if(siz==0)	{
			ret = FALSE;
			break;
		}
		bufor[licznik++] = cc;
		ASSERT(licznik-1<maxline);
		ASSERT(licznik-1>-1);
		if(cc<31 && cc!=13)
			licznik--;
		if(cc==13)
			status = FALSE;
	} while(status && licznik<maxline);


	return ret;



}
BOOL CRadioGUIDlg::Load(char* nazwa1)
{
	FILE *pFile;
	CString tmpstr,tmpstr1;
	COLORREF kolor;
	char buffer[maxline];
//	fpos_t pos;
	licznik = 0;
//	char cc;
	int a;
	BOOL status;
	kolor = RGB(0,255,0);
//	_set_fmode(_O_BINARY);
	if ((fopen_s(&pFile,nazwa1,"rb") != NULL))	{
		tmpstr.Format(_T("%s"),"No file loaded or file corrupted");
		listcontrol.InsertString(0,tmpstr,RGB(255,0,0));
//		fclose(pFile);
		return FALSE;
	}
	else	{
		tmpstr.Format(_T("File: %s"),nazwa1);
		listcontrol.InsertString(0,tmpstr,kolor);
	}
	do {		
		for(a=0;a<maxline;a++) buffer[a] = 0;
		status = LoadOneLine(pFile, buffer);
//		if(fgets(buffer,255,pFile)==NULL)
//			break;
		if(interpreter->AddLine(buffer,err))	{// czyli linia ok
			if(err.status)
				break;
			interpreter->CheckLine(licznik,err);
			if(err.status)
				break;
			interpreter->BuildLine(licznik,err);
			if(err.status)
				break;
			licznik++;
		}
//		fgetpos( pFile, &pos ); pos--;
//		fsetpos( pFile, &pos );
	}
	while ( status);
	
	if(err.status)	{
		tmpstr.Format(_T("%s in line: %d"),err.error,err.numline+1);
		if(err.data)	{
			tmpstr1.Format(_T("___ %s"),err.e_line);
			listcontrol.InsertString(0,tmpstr,RGB(255,0,0));
			listcontrol.InsertString(1,tmpstr1,RGB(200,0,0));
		}
		else
			listcontrol.InsertString(0,tmpstr,RGB(255,0,0));
		OnBnClickedClear();
		fclose(pFile);
		return FALSE;
	}
	else	{
		m_run.EnableWindow(1);
		m_step.EnableWindow(1);
		m_clear.EnableWindow(1);
		m_load.EnableWindow(0);
		m_rerun.EnableWindow(1);
		tmpstr.Format(_T("%d Lines"),licznik);
		listcontrol.InsertString(0,tmpstr,RGB(0,0,0));
	}
	fclose(pFile);
	return TRUE;

/*	FILE *pFile;
	CString tmpstr;
	char buffer[maxline];
	fpos_t pos;
	licznik = 0;
	int a;
	_set_fmode(_O_BINARY);
	if ((fopen_s(&pFile,nazwa1,"r") != NULL))	{
		tmpstr.Format(_T("%s"),"No file loaded or file corrupted");
		listcontrol.InsertString(0,tmpstr);
		fclose(pFile);
		return FALSE;
	}
	else	{
		tmpstr.Format(_T("File: %s"),nazwa1);
		listcontrol.InsertString(0,tmpstr);
	}
	do {		
		for(a=0;a<maxline;a++) buffer[a] = 0;
		if(_fgetts(buffer,255,pFile)==NULL)
			break;
		if(interpreter->AddLine(buffer,err))	{// czyli linia ok
			if(err.status)
				break;
			interpreter->CheckLine(licznik,err);
			if(err.status)
				break;
			interpreter->BuildLine(licznik,err);
			if(err.status)
				break;
			licznik++;
		}
		fgetpos( pFile, &pos ); pos--;
		fsetpos( pFile, &pos );
	}
	while ( fgetc(pFile) != EOF);
	
	if(err.status)	{
		tmpstr.Format(_T("%s in line: %d"),err.error,err.numline+1);
		listcontrol.InsertString(0,tmpstr);
		OnBnClickedClear();
		fclose(pFile);
		return FALSE;
	}
	else	{
		m_run.EnableWindow(1);
		m_step.EnableWindow(1);
		m_clear.EnableWindow(1);
		m_load.EnableWindow(0);
		m_rerun.EnableWindow(1);
		tmpstr.Format(_T("%d Lines"),licznik);
		listcontrol.InsertString(0,tmpstr);
	}
	fclose(pFile);
	return TRUE;*/
}
void CRadioGUIDlg::OnBnClickedRun()
{
	int a,ileznakow;
	char tab[256];
	CString tekst;
//	CTime sta;
//	CTime sto;
//	CTimeSpan dt;
	clock_t start, finish;
	C_Error err;
	COLORREF kolor = 0x00000000;
	C_CommandStack cs;
	m_load.EnableWindow(0);
	m_list2.ResetContent(); m_list2.SetCurSel(0); m_list2.AddString("BUSY"); this->RedrawWindow();
//	sta = CTime::GetCurrentTime();
	start = clock();
		interpreter->Start(err,cs,actline);
//	sto = CTime::GetCurrentTime();
	finish = clock();
	m_list2.ResetContent(); m_list2.SetCurSel(0); m_list2.AddString("IDLE"); this->RedrawWindow();
//	dt = sto-sta;
//	tekst.Format(_T("Elapsed time: %u"),(unsigned int)dt.GetTotalSeconds());
	tekst.Format(_T("Elapsed time: %f"),(double)(finish - start) / CLOCKS_PER_SEC);
	listcontrol.InsertString(0,tekst,RGB(255,0,255));
	if(err.status)	{
		tekst.Format("%s",err.error);
		listcontrol.InsertString(0,tekst,RGB(255,0,0));
		OnBnClickedClear();
	}
	else	{
		for(a=licznik-1;a>=0;a--)	{	// wyswietlanie zmiennych
			if(interpreter->line[a]->LHPF!=NULL)	{	// jeœli nie ma lewego parametru
				for(int n=0;n<interpreter->line[a]->numofleftparams;n++)	{
					ileznakow = interpreter->GetVariable(interpreter->line[a]->LHPF[n],tab,err);
					tab[ileznakow-1] = 0;		// bo jakieœ smieci zostaj¹
					tekst.Format(_T("L%d. %s = %s"),a,interpreter->line[a]->LHPF[n],tab);
					listcontrol.InsertString(0,tekst,RGB(199,164,24));
				}
			}
		}
	}
	if(cs.GetNumStrings()>0)	{
		tekst.Format(_T("===== USER OUTPUT STACK ====="));
		listcontrol.InsertString(0,tekst,RGB(128,128,128));
		for(int n=0;n<cs.GetNumStrings();n++)	{
			tekst.Format(_T("%s"),cs.GetString(n));
			listcontrol.InsertString(0,tekst,RGB(128,128,128));
		}
		tekst.Format(_T("===== END OF USER OUTPUT STACK ====="));
		listcontrol.InsertString(0,tekst,RGB(128,128,128));
	}
	actline = 0;
}

void CRadioGUIDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CRadioGUIDlg::OnBnClickedClear()
{
	delete interpreter;
	COLORREF kolor = RGB(0,0,255);
//	_CrtDumpMemoryLeaks();
	CString tmpstr;
	err.status = FALSE;
	actline = 0;
	licznik = 0;
	m_run.EnableWindow(0);
	m_step.EnableWindow(0);
	m_clear.EnableWindow(0);
	m_load.EnableWindow(1);
	m_rerun.EnableWindow(0);
	interpreter = new C_InterpBase(this);
	interpreter->SetDLLDir(dirp);	// ustawianie katalogu z programem
	GetNumberDLL(dirp);
	tmpstr.Format(_T("%s"),"--------- WORKSPACE CLEARED ---------");
	listcontrol.InsertString(0,tmpstr,kolor);
}
void CRadioGUIDlg::OnBnClickedStep()
{
	CString tmpstr;
	int ileznakow;
	char tab[256];
	CString tekst;
	C_Error err;
	C_CommandStack cs;
	COLORREF kolor = RGB(255,0,0);
	if(actline>licznik-1)	{
		tmpstr.Format(_T("%s"),"End of file");
		listcontrol.InsertString(0,tmpstr,RGB(0,0,0));
		return;
	}
	m_list2.ResetContent(); m_list2.SetCurSel(0); m_list2.AddString("BUSY"); this->RedrawWindow();
	interpreter->Step(actline,err,cs);
	m_list2.ResetContent(); m_list2.SetCurSel(0); m_list2.AddString("IDLE"); this->RedrawWindow();
	if(err.status)	{
		tekst.Format("%s",err.error);
		listcontrol.InsertString(0,tekst,RGB(255,0,0));
		OnBnClickedClear();
	}
	else {
		if(interpreter->line[actline]->LHPF!=NULL)	{	// jeœli nie ma lewego parametru
			ileznakow = interpreter->GetVariable(interpreter->line[actline]->LHPF[0],tab,err);
			tab[ileznakow-1] = 0;		// bo jakieœ smieci zostaj¹
			tekst.Format(_T(" %s = %s"),interpreter->line[actline]->LHPF[0],tab);
			listcontrol.InsertString(0,tekst,RGB(199,164,24));
		}
		tmpstr.Format(_T("Line: %d: %s"),actline,interpreter->line[actline]->linetmp);
		listcontrol.InsertString(0,tmpstr,RGB(0,0,0));
	}
	if(cs.GetNumStrings()>0)	{
//		tekst.Format(_T("===== USER OUTPUT STACK ====="));
//		listcontrol.InsertString(0,tekst,RGB(128,128,128));
		for(int n=0;n<1;n++)	{
			tekst.Format(_T("STACK:   %s"),cs.GetString(n));// tylko dla 0
			listcontrol.InsertString(cs.GetNumStrings()-n,tekst,RGB(128,128,128));
		}
//		tekst.Format(_T("===== END OF USER OUTPUT STACK ====="));
//		listcontrol.InsertString(cs.GetNumStrings()+1,tekst,RGB(128,128,128));
	}
	actline++;
}

void CRadioGUIDlg::OnBnClickedOpenwindow()
{
//	C_OpenGL test;
//	test = new C_OpenGL;
//	test->Create("OPENGL",WS_CAPTION|WS_POPUP|WS_VISIBLE|WS_SYSMENU|CS_OWNDC|WS_THICKFRAME|WS_MINIMIZEBOX,this,0);
}

void CRadioGUIDlg::OnBnClickedInfo()
{
	int numofcommand;
	int licz = 0,a;
	C_Commands** tab;
	CString tmpstr;
	char* dllname;
	FILE* out;
	COLORREF kolor = 0x00000000;
	if(!fopen_s( &out, "c:\\dump.txt", "w" ) == 0 )	{
		tmpstr.Format("Cant open dump file");
		listcontrol.InsertString(0,tmpstr,kolor);
		return;
	}
	numofcommand = 38+7+interpreter->DLL_Container.lastdllindex;
	tab = new C_Commands*[numofcommand];
	tab[licz++] = new C_Command_Add;
	tab[licz++] = new C_Command_Sin;
	tab[licz++] = new C_Command_Imread;
	tab[licz++] = new C_Command_Imshow;
	tab[licz++] = new C_Command_Imclose;
	tab[licz++] = new C_Command_Dilate;
	tab[licz++] = new C_Command_Erode;
	tab[licz++] = new C_Command_SaveImageM;
	tab[licz++] = new C_Command_Filter2d;
	tab[licz++] = new C_Command_Imadd;
	tab[licz++] = new C_Command_Imsub;
	tab[licz++] = new C_Command_Immulti;
	tab[licz++] = new C_Command_GLshow;
	tab[licz++] = new C_Command_Hist;
	tab[licz++] = new C_Command_Th;
	tab[licz++] = new C_Command_Outline;
	tab[licz++] = new C_Command_Berode;
	tab[licz++] = new C_Command_Bdilate;
	tab[licz++] = new C_Command_Gth;
	tab[licz++] = new C_Command_Ith;
	tab[licz++] = new C_Command_Particals;
	tab[licz++] = new C_Command_Median;
	tab[licz++] = new C_Command_Oth;
	tab[licz++] = new C_Command_Fftfilt;
	tab[licz++] = new C_Command_Eth;
	tab[licz++] = new C_Command_Fillholes;
	tab[licz++] = new C_Command_Chdir;
	tab[licz++] = new C_Command_Nth;
	tab[licz++] = new C_Command_Sth;
	tab[licz++] = new C_Command_Imwrite;
	tab[licz++] = new C_Command_Neural;
	tab[licz++] = new C_Command_Matrixread;
	tab[licz++] = new C_Command_Matrixwrite;
	tab[licz++] = new C_Command_Cutroi;
	tab[licz++] = new C_Command_FastMedianqs;
	tab[licz++] = new C_Command_Features;
	tab[licz++] = new C_Command_Savefeatures;
	tab[licz++] = new C_Command_Showfeatures;
	tab[licz++] = new C_Command_Prewitt;
	tab[licz++] = new C_Command_Sobel;
	tab[licz++] = new C_Command_Roberts;
	tab[licz++] = new C_Command_Annclasifier;
	tab[licz++] = new C_Command_Normalize;
	tab[licz++] = new C_Command_Exit;
//	tab[licz++] = new C_Command_IQI;
//	tab[licz++] = new C_Command_Crackdetection;
	tab[licz++] = new C_Command_Dm;

	for(a=0;a<interpreter->DLL_Container.lastdllindex;a++)	{
		dllname = interpreter->DLL_Container.dllnameholder[a].DLLName;
		tab[licz+a] = new C_Command_DLL(dllname,dirp,err);
	}
	
	char buf[10];
	_ltoa_s(BUILD_NUMBER,buf,10,10);
	fprintf_s(out,"ADDIP ver. %s, build %s\n\n",VERSION,buf);
	for(a=0;a<numofcommand;a++)	{
		SaveLine(out,tab[a]->name,tab[a]->syntax,tab[a]->fun_ver,tab[a]->description,tab[a]->fun_date,tab[a]->fun_time);
//		SaveLine(out,tab[a]->name,tab[a]->syntax,tab[a]->fun_ver,tab[a]->description);
		delete tab[a];
	}
	tmpstr.Format("File list dumped to c:\\dump.txt file");
	listcontrol.InsertString(0,tmpstr,kolor);
	fclose(out);
	delete[] tab;
}

void CRadioGUIDlg::SaveLine(FILE* out, char* name, char* syntax, char* ver, char* description,char* date, char* time)
{
	fprintf_s(out,"Function name: %s",name);
	fprintf_s(out,"			Syntax: %s ",syntax);
	fprintf_s(out,"Version: %s ",ver);
	if(description!=NULL)
		fprintf_s(out," Desc: %s",description);
	if(date!=NULL)
		fprintf_s(out," Date: %s",date);
	if(time!=NULL)
		fprintf_s(out," Time: %s",time);
	fprintf_s(out,"\n");

}

void CRadioGUIDlg::OnBnClickedCls()
{
	listcontrol.ResetContent();
}

void CRadioGUIDlg::OnBnClickedRerun()
{
	OnBnClickedClear();
	if(Load(nazwa1))
		OnBnClickedRun();
}

void CRadioGUIDlg::GetNumberDLL(char* programpatch)
{
	char tmpdir[1024];
	CString filename;
	CString filename1;
	BOOL bWorking;
	int ret;
	int licznik = 0;
	_getcwd(tmpdir,1024);
	ret = _chdir(programpatch);
	ret = _chdir("dll");
	if(ret!=0)	{
		_chdir(tmpdir);
		return;
	}
	CFileFind finder;
	CFileFind finder1;
	bWorking = finder.FindFile("*.dll");
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		filename = finder.GetFileName();
		licznik++;
	}
	interpreter->DLL_Container.AllocateDLLSpace(licznik);

	bWorking = finder1.FindFile("*.dll");
	while (bWorking)
	{
		bWorking = finder1.FindNextFile();
		filename1 = finder1.GetFileName();
		interpreter->DLL_Container.AddDLL(filename1.GetBuffer(),programpatch);
	}


	_chdir(tmpdir);
}
void CRadioGUIDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}
