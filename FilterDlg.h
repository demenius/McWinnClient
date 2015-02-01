#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CFilterDlg dialog

class CFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterDlg();

// Dialog Data
	enum { IDD = IDD_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int ShowDialog();

	int mode;
	CString m_PartNum;
	CString m_Size;
	CString m_New;
	CString m_Reman;
	CString m_Clean;
	CString m_Cost;
	CString m_EquivalentFilters;
	CString m_CustomerList;
	CString m_Notes;
	CString m_Search;

private:
	CString m_InitialCustomerList;
	CString m_InitialNotes;

	CEdit m_PartNumCtrl;
	CComboBox m_SizeCtrl;
	CEdit m_NewCtrl;
	CEdit m_RemanCtrl;
	CEdit m_CleanCtrl;
	CEdit m_CostCtrl;
	CEdit m_EquivalentFiltersCtrl;

	CStringList m_SelectedCustomers;
	CStringList m_InitialCustomers;

	CListCtrl m_CustomerCtrl;
	CEdit m_SearchCtrl;

	CButton m_CancelCtrl;
	CButton m_AddCtrl;

	void RefreshCustomers();

	afx_msg void OnBnClickedFilterDialogNotes();
	afx_msg void OnEnChangeFilterDialogCustomerSearch();

	afx_msg void OnLvnItemchangedFilterDialogCustomerList(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusFilterDialogCost();
	afx_msg void OnEnKillfocusFilterDialogNew();
	afx_msg void OnEnKillfocusFilterDialogClean();
	afx_msg void OnEnKillfocusFilterDialogReman();

	BOOL CheckChanges(BOOL ask = TRUE);
	BOOL Verify(CEdit *edit, CString value);
	void SaveChanges();
	void FormatCurrency(CString*);
	void FormatNumber(CEdit*);
};
