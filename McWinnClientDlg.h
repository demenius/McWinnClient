
// McWinnClientDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMcWinnClientDlg dialog
class CMcWinnClientDlg : public CDialogEx
{
// Construction
public:
	CMcWinnClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MCWINNCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG *pMsg);

private:
	CListCtrl m_CustomerControl;
	CListCtrl m_FilterControl;

	CButton m_ShowAllFilters;

	CEdit m_CustomerSearchCtrl;
	CEdit m_FilterSearchCtrl;

	BOOL m_ExtendedCustomerDetails;
	BOOL m_ExtendedFilterDetails;
	int m_SelectedCustomerIndex;
	int m_SelectedFilterIndex;

	CStatic m_Filter_Text;
	CStatic m_FilterSearchText;
	CStatic m_CustomerText;
	CStatic m_CustomerSearchText;

	CRect m_FilterRect;
	CRect m_CustomerRect;
	CRect m_DetailRect;

	void DeselectAll(CListCtrl *ctrl);

	void RefreshCustomers(CString name = TEXT(""));
	afx_msg void OnNMClickCustomerList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickCustomerList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkCustomerList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCustomerAddBtn();
	afx_msg void OnBnClickedCustomerDetailsBtn();
	afx_msg void OnEnChangeCustomerSearch();

	void RefreshFilters(CString partNum = TEXT(""));
	afx_msg void OnNMClickFilterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickFilterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkFilterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFilterAddBtn();
	afx_msg void OnBnClickedFilterDetailsBtn();
	afx_msg void OnEnChangeFilterSearch();

	afx_msg void OnBnClickedFilterShowAll();
};
