// isoDoc.h : interface of the CIsoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISODOC_H__828DA2AA_A121_11D4_BB95_CC0761F9C633__INCLUDED_)
#define AFX_ISODOC_H__828DA2AA_A121_11D4_BB95_CC0761F9C633__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CIsoDoc : public CDocument
{
protected: // create from serialization only
	CIsoDoc();
	DECLARE_DYNCREATE(CIsoDoc)

// Attributes
public:

C_Edge m_cube;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIsoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CIsoDoc)
	afx_msg void OnOpenData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISODOC_H__828DA2AA_A121_11D4_BB95_CC0761F9C633__INCLUDED_)
