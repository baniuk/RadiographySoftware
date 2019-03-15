// RadioGUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "_OpenGL.h"
#include "version.h"
#include "mylistbox.h"
#include "_MyEditDialog.h"


// CRadioGUIDlg dialog
class CRadioGUIDlg : public CDialog
{
// Construction
public:
	CRadioGUIDlg(CWnd* pParent = NULL);	// standard constructor
	C_InterpBase* interpreter;
	C_Error err;
	void GetNumberDLL(char* programpatch);				// zwraca iloœæ DLL w aktualnym katalogu oraz wype³nia strukture z nazwami
	int licznik;
	int actline;
	char dirp[1024];		// katalog z programem
	BOOL isparameter; // parametry z linii komend
	BOOL isdebug;
	char *parameter;	//parametr z linii komend
	char *debugparam;
	C_OpenGL* test;
	BOOL LoadOneLine(FILE *pFile, char* bufor);
	virtual ~CRadioGUIDlg(void);
// Dialog Data
	enum { IDD = IDD_RADIOGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedLoad();
public:
	afx_msg void OnBnClickedRun();
public:
	afx_msg void OnEnChangeEdit1();
public:
	MyListBox listcontrol;
	C_MyEditDialog EditDialog;
	afx_msg void OnBnClickedClear();
	CButton m_clear;
	CButton m_run;
	CButton m_load;
	CButton m_step;
	afx_msg void OnBnClickedStep();
	CButton m_openwindow;
	afx_msg void OnBnClickedOpenwindow();
	CListBox m_list2;
private:
	BOOL Load(char* nazwa1);
	char nazwa1[256];
	void SaveLine(FILE* out, char* name, char* syntax, char* ver, char* description,char* date, char* time);
public:
	afx_msg void OnBnClickedInfo();
	afx_msg void OnBnClickedCls();
	afx_msg void OnBnClickedRerun();
	CButton m_rerun;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
};
