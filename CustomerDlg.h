#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CCustomerDlg dialog

class CCustomerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomerDlg)

public:
	CCustomerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomerDlg();

// Dialog Data
	enum { IDD = IDD_CUSTOMER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int ShowDialog();

	int mode;
	CString m_CustomerName;
	CString m_OriginalName;
	CString m_ContactNumber;
	CString m_FilterList;
	CString m_ExtraInfo;
	CString m_FaxNumber;
	CString m_EMail;
	CString m_StreetAddress;
	CString m_City;
	CString m_Province;
	CString m_ZIPCode;
	CString m_Notes;

	CStringList m_SelectedFilters;
	CStringList m_InitialFilters;

private:
	CString m_InitialFilterList;
	CString m_InitialNotes;

	CEdit m_CustomerNameCtrl;
	CEdit m_ContactNumberCtrl;
	CListCtrl m_FilterControl;
	CEdit m_ExtraInfoCtrl;
	CEdit m_FaxNumberCtrl;
	CEdit m_EmailCtrl;
	CEdit m_StreetAddressCtrl;
	CEdit m_CityCtrl;
	CEdit m_ProvinceCtrl;
	CEdit m_ZipCodeCtrl;

	CButton m_AddCustomerCtrl;
	CButton m_CancelCtrl;
	CButton m_ShowAllFilters;
	CEdit m_FilterSearchCtrl;
	CButton m_Verify;

	void RefreshFilters();

	afx_msg void OnBnClickedCustomerDialogNotes();
	afx_msg void OnBnClickedCustomerDialogFilterShowAll();
	afx_msg void OnEnChangeCustomerDialogFilterSearch();

	afx_msg void OnLvnItemchangedCustomerDialogFilterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickCustomerDialogFilterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkCustomerDialogFilterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusCustomerDialogContactNumber();
	afx_msg void OnEnKillfocusCustomerDialogFaxNumber();
	afx_msg void OnEnChangeCustomerDialogCustomerName();

	afx_msg void OnBnClickedVerify();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	BOOL CheckChanges(BOOL ask = TRUE);
	BOOL Verify(CEdit *edit, CString value);
	void SaveChanges();
	void FormatPhoneNumber(CString*);
	void FormatNumber(CEdit*);
};
