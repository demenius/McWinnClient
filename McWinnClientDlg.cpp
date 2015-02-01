
// McWinnClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "McWinnClient.h"
#include "McWinnClientDlg.h"
#include "afxdialogex.h"

#include "CustomerDlg.h"
#include "FilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMcWinnClientDlg dialog



CMcWinnClientDlg::CMcWinnClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMcWinnClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMcWinnClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOMER_LIST, m_CustomerControl);
	DDX_Control(pDX, IDC_FILTER_LIST, m_FilterControl);
	DDX_Control(pDX, IDC_FILTER_SHOW_ALL, m_ShowAllFilters);
	DDX_Control(pDX, IDC_CUSTOMER_SEARCH, m_CustomerSearchCtrl);
	DDX_Control(pDX, IDC_FILTER_SEARCH, m_FilterSearchCtrl);
	DDX_Control(pDX, IDC_FILTER_STATIC, m_Filter_Text);
	DDX_Control(pDX, IDC_FILTER_SEARCH_STATIC, m_FilterSearchText);
	DDX_Control(pDX, IDC_CUSTOMER_STATIC, m_CustomerText);
	DDX_Control(pDX, IDC_CUSTOMER_SEARCH_STATIC, m_CustomerSearchText);
}

BEGIN_MESSAGE_MAP(CMcWinnClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_CUSTOMER_LIST, &CMcWinnClientDlg::OnNMDblclkCustomerList)
	ON_BN_CLICKED(IDC_CUSTOMER_ADD_BTN, &CMcWinnClientDlg::OnBnClickedCustomerAddBtn)
	ON_BN_CLICKED(IDC_FILTER_ADD_BTN, &CMcWinnClientDlg::OnBnClickedFilterAddBtn)
	ON_BN_CLICKED(IDC_CUSTOMER_DETAILS_BTN, &CMcWinnClientDlg::OnBnClickedCustomerDetailsBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_FILTER_LIST, &CMcWinnClientDlg::OnNMDblclkFilterList)
	ON_BN_CLICKED(IDC_FILTER_SHOW_ALL, &CMcWinnClientDlg::OnBnClickedFilterShowAll)
	ON_NOTIFY(NM_RCLICK, IDC_CUSTOMER_LIST, &CMcWinnClientDlg::OnNMRClickCustomerList)
	ON_NOTIFY(NM_RCLICK, IDC_FILTER_LIST, &CMcWinnClientDlg::OnNMRClickFilterList)
	ON_EN_CHANGE(IDC_CUSTOMER_SEARCH, &CMcWinnClientDlg::OnEnChangeCustomerSearch)
	ON_EN_CHANGE(IDC_FILTER_SEARCH, &CMcWinnClientDlg::OnEnChangeFilterSearch)
	ON_BN_CLICKED(IDC_FILTER_DETAILS_BTN, &CMcWinnClientDlg::OnBnClickedFilterDetailsBtn)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOMER_LIST, &CMcWinnClientDlg::OnNMClickCustomerList)
	ON_NOTIFY(NM_CLICK, IDC_FILTER_LIST, &CMcWinnClientDlg::OnNMClickFilterList)
END_MESSAGE_MAP()


// CMcWinnClientDlg message handlers

BOOL CMcWinnClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 185;
	lvColumn.pszText = TEXT("Customer Name");
	m_CustomerControl.InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 125;
	lvColumn.pszText = TEXT("Contact Number");
	m_CustomerControl.InsertColumn(1, &lvColumn);

	m_CustomerControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 150;
	lvColumn.pszText = TEXT("Part Number");
	m_FilterControl.InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 50;
	lvColumn.pszText = TEXT("Size");
	m_FilterControl.InsertColumn(1, &lvColumn);

	m_FilterControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	CWnd *pFWnd = GetDlgItem(IDC_FILTER_LIST);
	pFWnd->GetWindowRect(&m_FilterRect);
	ScreenToClient(&m_FilterRect);

	CWnd *pCWnd = GetDlgItem(IDC_CUSTOMER_LIST);
	pCWnd->GetWindowRect(&m_CustomerRect);
	ScreenToClient(&m_CustomerRect);

	
	CWnd *pDWnd = GetDlgItem(IDC_DETAIL_LIST);
	pDWnd->GetWindowRect(&m_DetailRect);
	ScreenToClient(&m_DetailRect);

	// Retrieve dataset from table
	m_SelectedCustomerIndex = -1;
	m_SelectedFilterIndex = -1;

	RefreshCustomers();
	RefreshFilters();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMcWinnClientDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMcWinnClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMcWinnClientDlg::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
			return TRUE;
		if ((pMsg->wParam == VK_DELETE) || (pMsg->wParam == VK_BACK))
		{

			if (this->GetFocus()->GetSafeHwnd() == m_CustomerControl.GetSafeHwnd())
			{
				POSITION pos = m_CustomerControl.GetFirstSelectedItemPosition();
				BOOL refresh = FALSE;
				while (pos != NULL)
				{
					int item = m_CustomerControl.GetNextSelectedItem(pos);
					CString name = m_CustomerControl.GetItemText(item, CUSTOMER_NAME);
					CString cmd;
					cmd.Format(DELETE_CUSTOMER_MSG, name);
					if (AfxMessageBox(cmd, MB_YESNO) == IDYES)
					{
						refresh = TRUE;
						cmd.Format(TEXT("DELETE FROM Customers WHERE Name='%s'"), name);
						theApp.m_Database.ExecuteSQL(cmd);
						cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE Name='%s'"), name);
						theApp.m_Database.ExecuteSQL(cmd);
					}
				}
				if (refresh)
					RefreshCustomers();
			}
			else if (this->GetFocus()->GetSafeHwnd() == m_FilterControl.GetSafeHwnd())
			{
				POSITION pos = m_FilterControl.GetFirstSelectedItemPosition();
				BOOL refresh = FALSE;
				while (pos != NULL)
				{
					int item = m_FilterControl.GetNextSelectedItem(pos);
					CString partNum = m_FilterControl.GetItemText(item, CUSTOMER_NAME);
					CString cmd;
					cmd.Format(DELETE_FILTER_MSG, partNum);
					if (AfxMessageBox(cmd, MB_YESNO) == IDYES)
					{
						refresh = TRUE;
						cmd.Format(TEXT("DELETE FROM Filters WHERE PartNum='%s'"), partNum);
						theApp.m_Database.ExecuteSQL(cmd);
						cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE PartNum='%s'"), partNum);
						theApp.m_Database.ExecuteSQL(cmd);
					}
				}
				if (refresh)
					RefreshFilters();
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMcWinnClientDlg::DeselectAll(CListCtrl *ctrl)
{
	int count = ctrl->GetItemCount();

	for (int i = 0; i < count; i++)
	{
		ctrl->SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CMcWinnClientDlg::RefreshCustomers(CString sel_name)
{
	CRecordset rs(&theApp.m_Database);
	CString s;
	m_CustomerControl.DeleteAllItems();

	CString command;

	CString extra = TEXT("");

	CString search;
	m_CustomerSearchCtrl.GetWindowTextW(search);
	if (!search.IsEmpty())
	{
		extra.Format(TEXT("WHERE Name LIKE '%%%s%%'"), search);
	}

	command.Format(TEXT("SELECT Name FROM Customers %s ORDER BY Name"), extra);

	if (rs.Open(CRecordset::forwardOnly, command))
	{
		int nRow = 0;
		CString address;

		while (!rs.IsEOF())
		{
			rs.GetFieldValue((short)CUSTOMER_NAME, s);
			m_CustomerControl.InsertItem(nRow, s);

			rs.MoveNext();
			nRow++;
		}
		rs.Close();
	}

	command.Format(TEXT("SELECT * FROM Customers %s ORDER BY Name"), extra);

	if (rs.Open(CRecordset::forwardOnly, command))
	{
		int nRow = 0;
		CString address;
		BOOL input = sel_name.IsEmpty();
		while (!rs.IsEOF())
		{
			address = TEXT("");
			for (int i = 0; i < rs.GetODBCFieldCount(); i++)
			{
				rs.GetFieldValue(i, s);
				if (s.IsEmpty() && i != 7)
				{
					continue;
				}
				if (i == 0)
				{
					if (!sel_name.IsEmpty())
						input = sel_name.Compare(s) == 0;
				}
				else if (sel_name.IsEmpty() || input)
				{
					if (i == CUSTOMER_FILTERS)
					{
						if (sel_name.IsEmpty())
						{
							m_CustomerControl.SetItemText(nRow, i, TEXT("..."));
						}
						else
						{
							CString restoken;
							int start = 0;

							restoken = s.Tokenize(TEXT("\r\n"), start);
							int nnRow = nRow;

							while (restoken != TEXT(""))
							{
								if (m_CustomerControl.GetItemCount() <= nnRow)
									m_CustomerControl.InsertItem(nnRow, TEXT(""));
								m_CustomerControl.SetItemText(nnRow, i, restoken);

								restoken = s.Tokenize(TEXT("\r\n"), start);
								nnRow++;
							}
						}
					} 
					else
					{
						if (i == CUSTOMER_STREETADDRESS)
							address.Format(TEXT("%s"), s);
						else if (i == CUSTOMER_CITY)
							address.AppendFormat(TEXT(" %s"), s);
						else if (i == CUSTOMER_PROVINCE)
							address.AppendFormat(TEXT(", %s"), s);
						else if (i == CUSTOMER_ZIPCODE)
						{
							address.AppendFormat(TEXT(" %s"), s);
							m_CustomerControl.SetItemText(nRow, i - 3, address);
						}
						else
						{
							m_CustomerControl.SetItemText(nRow, i, s);
						}
					}
				}
			}

			rs.MoveNext();
			nRow++;
		}
		rs.Close();
	}
}

void CMcWinnClientDlg::OnNMClickCustomerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_ExtendedCustomerDetails)
		return;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iSubItem != 0)
		return;

	int selected = pNMItemActivate->iItem;

	if (selected != m_SelectedCustomerIndex && selected != -1)
	{
		CString name = m_CustomerControl.GetItemText(selected, 0);
		RefreshCustomers(name);

		m_SelectedCustomerIndex = selected;
		m_CustomerControl.SetItemState(selected, LVIS_SELECTED, LVIS_SELECTED);


	}
	else if (m_SelectedCustomerIndex != -1)
	{
		m_SelectedCustomerIndex = -1;
		RefreshCustomers();
	}

}

void CMcWinnClientDlg::OnNMRClickCustomerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)
		return;

	DeselectAll(&m_CustomerControl);

	m_CustomerControl.SetItemState(pNMItemActivate->iItem, LVIS_SELECTED, LVIS_SELECTED);

	CString name = m_CustomerControl.GetItemText(pNMItemActivate->iItem, 0);

	CMenu mnuPopupOptions;
	mnuPopupOptions.LoadMenuW(IDR_POPUP_OPTIONS);

	CMenu *mnuPopupMenu = mnuPopupOptions.GetSubMenu(0);
	ASSERT(mnuPopupMenu);

	// GET POINT
	POINT point;
	if (GetCursorPos(&point))
	{
		BOOL option = mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
		if (option == ID_POPUP_DELETE)// Delete
		{
			CString cmd;
			cmd.Format(DELETE_CUSTOMER_MSG, name);
			if (AfxMessageBox(cmd, MB_YESNO) == IDYES)
			{
				cmd.Format(TEXT("DELETE FROM Customers WHERE Name='%s'"), name);
				theApp.m_Database.ExecuteSQL(cmd);
				cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE Name='%s'"), name);
				theApp.m_Database.ExecuteSQL(cmd);
				RefreshCustomers();
			}
		}
		else
		{
			CCustomerDlg dlg;
			dlg.m_CustomerName = name;

			if (option == ID_POPUP_VIEW)// View
			{
				dlg.mode = DIALOG_VIEW;

				dlg.ShowDialog();
			}
			else if (option == ID_POPUP_EDIT)// Edit
			{
				dlg.mode = DIALOG_EDIT;

				CString cmd;
				if (dlg.ShowDialog() == IDOK)
				{
					cmd.Format(TEXT("UPDATE Customers SET Name='%s', Contact='%s', Filters='%s', Fax='%s', Email='%s', StreetAddress='%s', City='%s', Province='%s', ZIPCode='%s', PrimaryNotes='%s', SecondaryNotes='%s' WHERE Name='%s'"), dlg.m_CustomerName, dlg.m_ContactNumber, dlg.m_FilterList, dlg.m_FaxNumber, dlg.m_EMail, dlg.m_StreetAddress, dlg.m_City, dlg.m_Province, dlg.m_ZIPCode, dlg.m_ExtraInfo, dlg.m_Notes, name);
					theApp.m_Database.ExecuteSQL(cmd);
					cmd.Format(TEXT("UPDATE CustomerFilters SET Name='%s' WHERE Name='%s'"), dlg.m_CustomerName, name);
					theApp.m_Database.ExecuteSQL(cmd);
				}
				RefreshCustomers();
			}
		}
	}
}

void CMcWinnClientDlg::OnNMDblclkCustomerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)
		return;

	CCustomerDlg dlg;
	dlg.mode = DIALOG_VIEW;
	dlg.m_CustomerName = m_CustomerControl.GetItemText(pNMItemActivate->iItem, CUSTOMER_NAME);

	dlg.ShowDialog();
}

void CMcWinnClientDlg::OnBnClickedCustomerAddBtn()
{
	// TODO: Add your control notification handler code here
	CCustomerDlg dlg;
	dlg.mode = DIALOG_ADD;
	if (dlg.ShowDialog() == IDOK)
	{
		CString s;
		s.Format(TEXT("INSERT INTO Customers (Name, Contact, Filters, Fax, Email, StreetAddress, City, Province, ZIPCode, PrimaryNotes, SecondaryNotes) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')"),
			dlg.m_CustomerName, dlg.m_ContactNumber, dlg.m_FilterList, dlg.m_FaxNumber, dlg.m_EMail, dlg.m_StreetAddress, dlg.m_City, dlg.m_Province, dlg.m_ZIPCode, dlg.m_ExtraInfo, dlg.m_Notes);

		theApp.m_Database.ExecuteSQL(s);
		RefreshCustomers();
	}
}

void CMcWinnClientDlg::OnBnClickedCustomerDetailsBtn()
{
	// TODO: Add your control notification handler code here

	if (!m_ExtendedCustomerDetails)
	{
		if (m_ExtendedFilterDetails)
		{
			OnBnClickedFilterDetailsBtn();
		}

		m_ExtendedFilterDetails = FALSE;
		m_FilterControl.ShowWindow(FALSE);
		m_FilterSearchCtrl.ShowWindow(FALSE);
		m_Filter_Text.ShowWindow(FALSE);
		m_FilterSearchText.ShowWindow(FALSE);
		m_ShowAllFilters.ShowWindow(FALSE);

		LVCOLUMN lvColumn;

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 185;
		lvColumn.pszText = TEXT("Filters");
		m_CustomerControl.InsertColumn(CUSTOMER_FILTERS, &lvColumn);

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 125;
		lvColumn.pszText = TEXT("Fax Number");
		m_CustomerControl.InsertColumn(CUSTOMER_FAX, &lvColumn);

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 185;
		lvColumn.pszText = TEXT("Email");
		m_CustomerControl.InsertColumn(CUSTOMER_EMAIL, &lvColumn);

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 300;
		lvColumn.pszText = TEXT("Address");
		m_CustomerControl.InsertColumn(CUSTOMER_STREETADDRESS, &lvColumn);

		RefreshCustomers();
		m_CustomerControl.MoveWindow(m_DetailRect);
	}
	else
	{
		for (int i = m_CustomerControl.GetHeaderCtrl()->GetItemCount() - 1; i > 1; i--)
		{
			m_CustomerControl.DeleteColumn(i);
		}
		m_CustomerControl.MoveWindow(m_CustomerRect);

		m_ExtendedFilterDetails = FALSE;
		m_FilterControl.ShowWindow(TRUE);
		m_FilterSearchCtrl.ShowWindow(TRUE);
		m_Filter_Text.ShowWindow(TRUE);
		m_FilterSearchText.ShowWindow(TRUE);
		m_ShowAllFilters.ShowWindow(TRUE);
	}
	m_SelectedCustomerIndex = -1;
	m_ExtendedCustomerDetails = !m_ExtendedCustomerDetails;
}

void CMcWinnClientDlg::OnEnChangeCustomerSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	RefreshCustomers();
}

void CMcWinnClientDlg::RefreshFilters(CString sel_partNum)
{
	CRecordset rs(&theApp.m_Database);
	CString s, secondaryFilters;
	m_FilterControl.DeleteAllItems();

	CString command;

	CString extra = TEXT("");

	CString search;
	m_FilterSearchCtrl.GetWindowTextW(search);
	if (!search.IsEmpty())
	{
		extra.Format(TEXT("WHERE PartNum LIKE '%%%s%%'"), search);
	}

	command.Format(TEXT("SELECT PartNum FROM Filters %s ORDER BY PartNum"), extra);


	if (rs.Open(CRecordset::forwardOnly, command))
	{
		int nRow = 0;

		while (!rs.IsEOF())
		{
			rs.GetFieldValue((short)0, s);
			m_FilterControl.InsertItem(nRow, s);

			rs.MoveNext();
			nRow++;
		}
		rs.Close();
	}
	CString primaryFilter, secondaryFilter;

		command.Format(TEXT("SELECT * FROM Filters %s ORDER BY PartNum"), extra);

	if (rs.Open(CRecordset::forwardOnly, command))
	{
		int nRow = 0;
		BOOL input = sel_partNum.IsEmpty();
		while (!rs.IsEOF())
		{
			for (int i = 0; i < rs.GetODBCFieldCount() - 1; i++)
			{
				rs.GetFieldValue(i, s);
				if (s.IsEmpty())
				{
					if (i == FILTER_SECONDARY)
						secondaryFilters = TEXT("");
					continue;
				}
				if (i == FILTER_PARTNUM)
				{
					primaryFilter.Format(TEXT("%s"), s);
					if (!sel_partNum.IsEmpty())
						input = sel_partNum.Compare(s) == 0;
				}
				else if (sel_partNum.IsEmpty() || input)
				{
					if (i == FILTER_SECONDARY)
					{
						secondaryFilters = s;
						if (sel_partNum.IsEmpty())
						{
							m_FilterControl.SetItemText(nRow, i, TEXT("..."));
						}
						else
						{
							CString restoken;
							int start = 0;

							restoken = s.Tokenize(TEXT("\r\n"), start);
							int nnRow = nRow;

							while (restoken != TEXT(""))
							{
								if (m_FilterControl.GetItemCount() <= nnRow)
									m_FilterControl.InsertItem(nnRow, TEXT(""));
								m_FilterControl.SetItemText(nnRow, i, restoken);

								restoken = s.Tokenize(TEXT("\r\n"), start);
								nnRow++;
							}
						}
					} 
					else
					{
						m_FilterControl.SetItemText(nRow, i, s);
					}
				}
			}

			nRow++;
			if (!m_ExtendedFilterDetails && m_ShowAllFilters.GetCheck() == BST_CHECKED)
			{
				int curPos = 0;

				CString restoken = secondaryFilters.Tokenize(TEXT("\r\n"), curPos);

				while (restoken != TEXT(""))
				{
					secondaryFilter.Format(TEXT("%s(%s)"), primaryFilter, restoken);
					m_FilterControl.InsertItem(nRow, secondaryFilter);

					restoken = secondaryFilters.Tokenize(TEXT("\r\n"), curPos);
					nRow++;
				}
			}

			rs.MoveNext();
		}
		rs.Close();
	}
}

void CMcWinnClientDlg::OnNMClickFilterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_ExtendedFilterDetails)
		return;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iSubItem != 0)
		return;

	int selected = pNMItemActivate->iItem;

	if (selected != m_SelectedFilterIndex && selected != -1)
	{
		CString partNum = m_FilterControl.GetItemText(selected, 0);
		RefreshFilters(partNum);

		m_SelectedFilterIndex = selected;
		m_FilterControl.SetItemState(selected, LVIS_SELECTED, LVIS_SELECTED);

	}
	else if (m_SelectedFilterIndex != -1)
	{
		m_SelectedFilterIndex = -1;
		RefreshFilters();
	}
}

void CMcWinnClientDlg::OnNMRClickFilterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)
		return;

	DeselectAll(&m_FilterControl);
	m_FilterControl.SetItemState(pNMItemActivate->iItem, LVIS_SELECTED, LVIS_SELECTED);

	CString temp = m_FilterControl.GetItemText(pNMItemActivate->iItem, FILTER_PARTNUM);
	int curPos = 0;
	CString partNum = temp.Tokenize(TEXT("("), curPos);

	CMenu mnuPopupOptions;
	mnuPopupOptions.LoadMenuW(IDR_POPUP_OPTIONS);

	CMenu *mnuPopupMenu = mnuPopupOptions.GetSubMenu(0);
	ASSERT(mnuPopupMenu);

	// GET POINT
	POINT point;
	if (GetCursorPos(&point))
	{
		BOOL option = mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
		if (option == ID_POPUP_DELETE)// Delete
		{
			CString cmd;
			cmd.Format(DELETE_FILTER_MSG, partNum);
			if (AfxMessageBox(cmd, MB_YESNO) == IDYES)
			{
				cmd.Format(TEXT("DELETE FROM Filters WHERE PartNum='%s'"), partNum);
				theApp.m_Database.ExecuteSQL(cmd);
				cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE PartNum='%s'"), partNum);
				theApp.m_Database.ExecuteSQL(cmd);
				RefreshFilters();
			}
		}
		else
		{
			CFilterDlg dlg;
			dlg.m_PartNum = partNum;

			if (option == ID_POPUP_VIEW)// View
			{
				dlg.mode = DIALOG_VIEW;
				dlg.ShowDialog();
			}
			else if (option == ID_POPUP_EDIT)// Edit
			{
				dlg.mode = DIALOG_EDIT;

				if (dlg.ShowDialog() == IDOK)
				{
					CString cmd, ptsearch, name, oldFilters, newFilters, search;
					cmd.Format(TEXT("UPDATE Filters SET PartNum='%s', Size='%s', SecondaryPartNum='%s', Cost='%s', Replace='%s', Clean='%s', Reman='%s', Notes='%s' WHERE PartNum='%s'"), dlg.m_PartNum, dlg.m_Size, dlg.m_EquivalentFilters, dlg.m_Cost, dlg.m_New, dlg.m_Clean, dlg.m_Reman, dlg.m_Notes, partNum);
					theApp.m_Database.ExecuteSQL(cmd);

					ptsearch.Format(TEXT("\r\n%s\r\n"), partNum);

					search.Format(TEXT("SELECT Name,Filters FROM Customers WHERE Filters LIKE '%%%s%%'"), ptsearch);

					CRecordset rs(&theApp.m_Database);

					// Remove Filter From Customer Lists
					if (rs.Open(CRecordset::forwardOnly, search))
					{
						while (!rs.IsEOF())
						{
							rs.GetFieldValue((short)0, name);
							rs.GetFieldValue((short)1, oldFilters);

							curPos = 0;

							int index = oldFilters.Find(ptsearch);
							int leftSize = index + 2;
							int rightSize = oldFilters.GetLength() - (ptsearch.GetLength() - 2) - leftSize;

							CString left = oldFilters.Left(leftSize), right = oldFilters.Right(rightSize);

							newFilters = left + right;

							cmd.Format(TEXT("UPDATE Customers SET Filters='%s' WHERE Name='%s'"), newFilters, name);
							theApp.m_Database.ExecuteSQL(cmd);

							rs.MoveNext();
						}
						rs.Close();
					}

					CString restoken; // IE Customer Name
					int curPos = 0;

					restoken = dlg.m_CustomerList.Tokenize(TEXT("\r\n"), curPos);
					// For All Customers Append Filter
					while (restoken != TEXT(""))
					{
						cmd.Format(TEXT("UPDATE Customers SET Filters=Filters+'%s\r\n' WHERE Name='%s'"), dlg.m_PartNum, restoken);
						theApp.m_Database.ExecuteSQL(cmd);
						restoken = dlg.m_CustomerList.Tokenize(TEXT("\r\n"), curPos);
					}

					RefreshFilters();
				}
			}
		}
	}
}

void CMcWinnClientDlg::OnNMDblclkFilterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)
		return;

	CFilterDlg dlg;
	dlg.mode = DIALOG_VIEW;

	CString part;
	part = m_FilterControl.GetItemText(pNMItemActivate->iItem, 0);
	int curPos = 0;

	dlg.m_PartNum = part.Tokenize(TEXT("("), curPos);

	dlg.ShowDialog();
}

void CMcWinnClientDlg::OnBnClickedFilterAddBtn()
{
	// TODO: Add your control notification handler code here

	CFilterDlg dlg;
	dlg.mode = DIALOG_ADD;
	if (dlg.ShowDialog() == IDOK)
	{
		CString s;
		s.Format(TEXT("INSERT INTO Filters (PartNum, Size, SecondaryPartNum, Cost, Replace, Clean, Reman, Notes) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')"),
			dlg.m_PartNum, dlg.m_Size, dlg.m_EquivalentFilters, dlg.m_Cost, dlg.m_New, dlg.m_Clean, dlg.m_Reman, dlg.m_Notes);

		theApp.m_Database.ExecuteSQL(s);

		CString search, filters, restoken; // IE Customer Name
		int curPos = 0;

		restoken = dlg.m_CustomerList.Tokenize(TEXT("\r\n"), curPos);

		while (restoken != TEXT(""))
		{
			s.Format(TEXT("UPDATE Customers SET Filters=Filters+'%s\r\n' WHERE Name='%s'"), dlg.m_PartNum, restoken);
			theApp.m_Database.ExecuteSQL(s);
			restoken = dlg.m_CustomerList.Tokenize(TEXT("\r\n"), curPos);
		}
		RefreshFilters();
	}
}

void CMcWinnClientDlg::OnBnClickedFilterDetailsBtn()
{
	// TODO: Add your control notification handler code here

	if (!m_ExtendedFilterDetails)
	{
		if (m_ExtendedCustomerDetails)
		{
			OnBnClickedCustomerDetailsBtn();
		}

		m_ExtendedCustomerDetails = FALSE;
		m_CustomerControl.ShowWindow(FALSE);
		m_CustomerSearchCtrl.ShowWindow(FALSE);
		m_CustomerText.ShowWindow(FALSE);
		m_CustomerSearchText.ShowWindow(FALSE);

		m_ShowAllFilters.EnableWindow(FALSE);

		LVCOLUMN lvColumn;

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 150;
		lvColumn.pszText = TEXT("Secondary Part Number");
		m_FilterControl.InsertColumn(FILTER_SECONDARY, &lvColumn);

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 100;
		lvColumn.pszText = TEXT("Cost");
		m_FilterControl.InsertColumn(FILTER_COST, &lvColumn);

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 100;
		lvColumn.pszText = TEXT("New");
		m_FilterControl.InsertColumn(FILTER_REPLACE, &lvColumn);

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 100;
		lvColumn.pszText = TEXT("Clean");
		m_FilterControl.InsertColumn(FILTER_CLEAN, &lvColumn);

		lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 100;
		lvColumn.pszText = TEXT("Reman");
		m_FilterControl.InsertColumn(FILTER_REMAN, &lvColumn);

		m_FilterControl.MoveWindow(m_DetailRect);
	}
	else
	{
		for (int i = m_FilterControl.GetHeaderCtrl()->GetItemCount() - 1; i > 1; i--)
		{
			m_FilterControl.DeleteColumn(i);
		}

		m_FilterControl.MoveWindow(m_FilterRect);

		m_CustomerControl.ShowWindow(TRUE);
		m_CustomerSearchCtrl.ShowWindow(TRUE);
		m_CustomerText.ShowWindow(TRUE);
		m_CustomerSearchText.ShowWindow(TRUE);

		m_ShowAllFilters.EnableWindow(TRUE);
	}
	m_SelectedFilterIndex = -1;
	m_ExtendedFilterDetails = !m_ExtendedFilterDetails;
	RefreshFilters();
}

void CMcWinnClientDlg::OnEnChangeFilterSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CRecordset rs(&theApp.m_Database);
	CString cmd, filterSearch, secondaryFilters;
	m_FilterSearchCtrl.GetWindowTextW(filterSearch);
	if (filterSearch.IsEmpty() || m_ExtendedFilterDetails)
	{
		RefreshFilters();
		m_ShowAllFilters.EnableWindow(TRUE);
		return;
	}

	m_ShowAllFilters.EnableWindow(FALSE);

	cmd.Format(TEXT("SELECT * FROM Filters WHERE PartNum LIKE '%%%s%%' OR SecondaryPartNum LIKE '%%%s%%' ORDER BY PartNum"), filterSearch, filterSearch);

	if (rs.Open(CRecordset::forwardOnly, cmd))
	{
		m_FilterControl.DeleteAllItems();
		int nRow = 0;
		CString s, primary;
		while (!rs.IsEOF())
		{
			for (int i = 0; i < rs.GetODBCFieldCount()-1; i++)
			{
				rs.GetFieldValue(i, s);
				if (i == FILTER_PARTNUM)
				{
					m_FilterControl.InsertItem(nRow, s);
					primary = s;
				}
				else if (i == FILTER_SECONDARY)
				{
					secondaryFilters = s;
				}
				else
					m_FilterControl.SetItemText(nRow, i, s);
			}
			nRow++;
			if (!secondaryFilters.IsEmpty())
			{
				CString restoken, secondary, upper;
				int curPos = 0;

				restoken = secondaryFilters.Tokenize(TEXT("\r\n"), curPos);
				while (restoken != TEXT(""))
				{
					upper = restoken;
					upper.MakeUpper();
					filterSearch.MakeUpper();
					if (upper.Find(filterSearch, 0) != -1)
					{
						secondary.Format(TEXT("%s(%s)"), primary, restoken);
						m_FilterControl.InsertItem(nRow, secondary);
						nRow++;
					}
					
					restoken = secondaryFilters.Tokenize(TEXT("\r\n"), curPos);
				}
			}
			rs.MoveNext();
		}
		rs.Close();
	}
}

void CMcWinnClientDlg::OnBnClickedFilterShowAll()
{
	// TODO: Add your control notification handler code here
	RefreshFilters();
}
