#pragma once
#include "afxwin.h"


// CNotesDlg dialog

class CNotesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNotesDlg)

public:
	CNotesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNotesDlg();

// Dialog Data
	enum { IDD = IDD_NOTES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	int mode;
	CString m_Notes;

private:
	CEdit m_NotesCtrl;
	CButton m_Confirm;

	BOOL CheckChanges(BOOL ask=TRUE);
	void SaveChanges();

	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
};
