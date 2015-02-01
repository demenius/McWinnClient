// CustomerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "McWinnClient.h"
#include "CustomerDlg.h"
#include "afxdialogex.h"

#include "NotesDlg.h"
#include "FilterDlg.h"


// CCustomerDlg dialog

IMPLEMENT_DYNAMIC(CCustomerDlg, CDialogEx)

CCustomerDlg::CCustomerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCustomerDlg::IDD, pParent)
	, m_CustomerName(_T(""))
	, m_ContactNumber(_T(""))
	, m_FaxNumber(_T(""))
	, m_EMail(_T(""))
	, m_StreetAddress(_T(""))
	, m_City(_T(""))
	, m_Province(_T(""))
	, m_ZIPCode(_T(""))
	, m_FilterList(_T("\r\n"))
	, m_Notes(_T(""))
	, m_ExtraInfo(_T(""))
	, m_InitialNotes(_T(""))
	, m_OriginalName(_T(""))
{

}

CCustomerDlg::~CCustomerDlg()
{
}

void CCustomerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_CUSTOMER_NAME, m_CustomerNameCtrl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_FILTER_LIST, m_FilterControl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_CONTACT_NUMBER, m_ContactNumberCtrl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_FAX_NUMBER, m_FaxNumberCtrl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_EMAIL, m_EmailCtrl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_ADDRESS, m_StreetAddressCtrl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_CITY, m_CityCtrl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_PROVINCE, m_ProvinceCtrl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_ZIPCODE, m_ZipCodeCtrl);
	DDX_Control(pDX, IDOK, m_AddCustomerCtrl);
	DDX_Control(pDX, IDCANCEL, m_CancelCtrl);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_CUSTOMER_NAME, m_CustomerName);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_CONTACT_NUMBER, m_ContactNumber);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_FAX_NUMBER, m_FaxNumber);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_EMAIL, m_EMail);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_ADDRESS, m_StreetAddress);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_CITY, m_City);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_PROVINCE, m_Province);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_ZIPCODE, m_ZIPCode);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_FILTER_SHOW_ALL, m_ShowAllFilters);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_FILTER_SEARCH, m_FilterSearchCtrl);
	DDX_Control(pDX, IDC_CUSTOMER_DIALOG_EXTRA_INFO, m_ExtraInfoCtrl);
	DDX_Text(pDX, IDC_CUSTOMER_DIALOG_EXTRA_INFO, m_ExtraInfo);
	DDX_Control(pDX, IDC_VERIFY, m_Verify);
}


BEGIN_MESSAGE_MAP(CCustomerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CUSTOMER_DIALOG_NOTES, &CCustomerDlg::OnBnClickedCustomerDialogNotes)
	ON_BN_CLICKED(IDOK, &CCustomerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CUSTOMER_DIALOG_FILTER_SHOW_ALL, &CCustomerDlg::OnBnClickedCustomerDialogFilterShowAll)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CUSTOMER_DIALOG_FILTER_LIST, &CCustomerDlg::OnLvnItemchangedCustomerDialogFilterList)
	ON_EN_CHANGE(IDC_CUSTOMER_DIALOG_FILTER_SEARCH, &CCustomerDlg::OnEnChangeCustomerDialogFilterSearch)
	ON_NOTIFY(NM_RCLICK, IDC_CUSTOMER_DIALOG_FILTER_LIST, &CCustomerDlg::OnNMRClickCustomerDialogFilterList)
	ON_NOTIFY(NM_DBLCLK, IDC_CUSTOMER_DIALOG_FILTER_LIST, &CCustomerDlg::OnNMDblclkCustomerDialogFilterList)
	ON_BN_CLICKED(IDCANCEL, &CCustomerDlg::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_CUSTOMER_DIALOG_CONTACT_NUMBER, &CCustomerDlg::OnEnKillfocusCustomerDialogContactNumber)
	ON_EN_KILLFOCUS(IDC_CUSTOMER_DIALOG_FAX_NUMBER, &CCustomerDlg::OnEnKillfocusCustomerDialogFaxNumber)
	ON_BN_CLICKED(IDC_VERIFY, &CCustomerDlg::OnBnClickedVerify)
	ON_EN_CHANGE(IDC_CUSTOMER_DIALOG_CUSTOMER_NAME, &CCustomerDlg::OnEnChangeCustomerDialogCustomerName)
END_MESSAGE_MAP()


// CCustomerDlg message handlers
BOOL CCustomerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 100;
	lvColumn.pszText = TEXT("Part Number");
	m_FilterControl.InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 75;
	lvColumn.pszText = TEXT("Size");
	m_FilterControl.InsertColumn(1, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 120;
	lvColumn.pszText = TEXT("Clean");
	m_FilterControl.InsertColumn(2, &lvColumn);

	m_FilterControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if (mode == DIALOG_VIEW)
	{
		CString title;
		title.Format(TEXT("View Customer - %s"), m_CustomerName);
		this->SetWindowTextW(title);
		LONG mFilterStyle = GetWindowLong(m_FilterControl.m_hWnd, GWL_STYLE);
		mFilterStyle |= LVS_SORTASCENDING;
		SetWindowLong(m_FilterControl.m_hWnd, GWL_STYLE, mFilterStyle);

		m_CustomerNameCtrl.SetReadOnly(TRUE);
		m_ContactNumberCtrl.SetReadOnly(TRUE);
		m_ExtraInfoCtrl.SetReadOnly(TRUE);
		m_FaxNumberCtrl.SetReadOnly(TRUE);
		m_EmailCtrl.SetReadOnly(TRUE);
		m_StreetAddressCtrl.SetReadOnly(TRUE);
		m_CityCtrl.SetReadOnly(TRUE);
		m_ProvinceCtrl.SetReadOnly(TRUE);
		m_ZipCodeCtrl.SetReadOnly(TRUE);
		m_AddCustomerCtrl.ShowWindow(FALSE);
		m_Verify.SetCheck(BST_CHECKED);
		m_Verify.EnableWindow(FALSE);
		m_CancelCtrl.SetWindowTextW(TEXT("Close"));
	} else
	{
		m_FilterControl.SetExtendedStyle(m_FilterControl.GetExtendedStyle() | LVS_EX_CHECKBOXES);
		if (mode == DIALOG_EDIT)
		{
			CString title;
			title.Format(TEXT("Edit Customer - %s"), m_CustomerName);
			this->SetWindowTextW(title);
			m_AddCustomerCtrl.SetWindowTextW(TEXT("Edit"));
			m_Verify.SetCheck(BST_CHECKED);
			m_Verify.EnableWindow(FALSE);
		}
	}
	RefreshFilters();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CCustomerDlg::ShowDialog()
{
	// Retrieve dataset from table
	CRecordset rs(&theApp.m_Database);

	CString search;
	search.Format(TEXT("SELECT * FROM Customers WHERE Name='%s'"), m_CustomerName);

	if (rs.Open(CRecordset::forwardOnly, search))
	{
		if (!rs.IsEOF())
		{
			rs.GetFieldValue((short)CUSTOMER_CONTACT, m_ContactNumber);
			rs.GetFieldValue((short)CUSTOMER_FILTERS, m_FilterList);
			rs.GetFieldValue((short)CUSTOMER_FAX, m_FaxNumber);
			rs.GetFieldValue((short)CUSTOMER_EMAIL, m_EMail);
			rs.GetFieldValue((short)CUSTOMER_STREETADDRESS, m_StreetAddress);
			rs.GetFieldValue((short)CUSTOMER_CITY, m_City);
			rs.GetFieldValue((short)CUSTOMER_PROVINCE, m_Province);
			rs.GetFieldValue((short)CUSTOMER_ZIPCODE, m_ZIPCode);
			rs.GetFieldValue((short)CUSTOMER_PRIMARY_NOTES, m_ExtraInfo);
			rs.GetFieldValue((short)CUSTOMER_SECONDARY_NOTES, m_Notes);
			
			m_InitialNotes = m_Notes;
			m_OriginalName = m_CustomerName;

			int index = 0;
			CString restoken = m_FilterList.Tokenize(TEXT("\r\n"), index);
			while (restoken != TEXT(""))
			{
				m_SelectedFilters.AddTail(restoken);
				m_InitialFilters.AddTail(restoken);
				restoken = m_FilterList.Tokenize(TEXT("\r\n"), index);
			}
		}
		rs.Close();
	}

	INT_PTR nRet = this->DoModal();

	CStringList *list;

	if (nRet == IDOK)
	{
		list = &m_SelectedFilters;
	}
	else
	{
		list = &m_InitialFilters;
	}

	CString cmd, partNum;


	if (list->GetCount() > 0)
	{
		search.Format(TEXT("SELECT PartNum FROM CustomerFilters WHERE Name='%s'"), m_CustomerName);

		if (rs.Open(CRecordset::forwardOnly, search))
		{
			while (!rs.IsEOF())
			{
				rs.GetFieldValue((short)0, partNum);
				if (list->Find(partNum) == NULL) // Cannot Find It In List, So Delete It
				{
					cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE Name = '%s' AND PartNum = '%s'"), m_CustomerName, partNum);
					theApp.m_Database.ExecuteSQL(cmd);
				}
				rs.MoveNext();
			}
			rs.Close();
		}
	}
	else 
	{
		cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE Name='%s'"), m_CustomerName);
		theApp.m_Database.ExecuteSQL(cmd);
	}

	/*POSITION pos = list->GetHeadPosition();

	CString saveFilters(TEXT(""));
	while (pos != NULL)
	{
		saveFilters.AppendFormat(TEXT("NOT(PartNum='%s')"), list->GetNext(pos));
		if (pos != NULL)
			saveFilters.AppendFormat(TEXT(" AND "));
	}



	CString cmd;
	if (saveFilters.IsEmpty())
		cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE Name='%s'"), m_CustomerName);
	else
		cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE Name='%s' AND %s"), m_CustomerName, saveFilters);

	theApp.m_Database.ExecuteSQL(cmd);*/

	if (nRet != IDOK)
	{
		cmd.Format(TEXT("UPDATE CustomerFilters SET Name='%s' WHERE Name='%s'"), m_OriginalName, m_CustomerName);
		theApp.m_Database.ExecuteSQL(cmd);
	}

	return nRet;
}

void CCustomerDlg::RefreshFilters()
{
	CRecordset filterRS(&theApp.m_Database);

	if (filterRS.Open(CRecordset::forwardOnly, TEXT("SELECT PartNum,Size,Clean,SecondaryPartNum FROM Filters ORDER BY PartNum")))
	{
		m_FilterControl.DeleteAllItems();

		CString s;
		CString search;
		CString primaryFilter, secondaryFilter;

		int nRow = 0;

		while (!filterRS.IsEOF())
		{
			for (int i = 0; i < filterRS.GetODBCFieldCount(); i++)
			{
				filterRS.GetFieldValue(i, s);

				if (i == FILTER_PARTNUM)
				{
					primaryFilter = s;
					if (m_SelectedFilters.Find(s) != NULL)// Filter Is Selected
					{
						m_FilterControl.InsertItem(nRow, s);
						m_FilterControl.SetCheck(nRow);
					}
					else if (mode == DIALOG_VIEW)
					{
						nRow--;
						break; // Not To Be Shown, Exit Loop
					}
					else
						m_FilterControl.InsertItem(nRow, s);
				}
				else if (i < FILTER_COST)
					m_FilterControl.SetItemText(nRow, i, s);
			}
			nRow++;
			if (s.Compare(primaryFilter) != 0 && m_ShowAllFilters.GetCheck() == BST_CHECKED)
			{
				int curPos = 0;

				CString restoken = s.Tokenize(TEXT("\r\n"), curPos);

				while (restoken != TEXT(""))
				{
					secondaryFilter.Format(TEXT("%s(%s)"), primaryFilter, restoken);
					m_FilterControl.InsertItem(nRow, secondaryFilter);

					restoken = s.Tokenize(TEXT("\r\n"), curPos);
					nRow++;
				}
			}

			filterRS.MoveNext();
		}
		filterRS.Close();
	}
	CString cmd, partNum, clean;
	cmd.Format(TEXT("SELECT PartNum,Clean FROM CustomerFilters WHERE Name = '%s'"), m_CustomerName);
	if (filterRS.Open(CRecordset::forwardOnly, cmd))
	{
		while (!filterRS.IsEOF())
		{
			filterRS.GetFieldValue((short)0, partNum);
			filterRS.GetFieldValue((short)1, clean);

			LVFINDINFO info;
			int nIndex;

			info.flags = LVFI_STRING;
			info.psz = partNum;

			if((nIndex = m_FilterControl.FindItem(&info)) != -1)
			{
				m_FilterControl.SetItemText(nIndex, 2, clean);
			}

			filterRS.MoveNext();
		}
		filterRS.Close();
	}
}

void CCustomerDlg::OnBnClickedCustomerDialogNotes()
{
	// TODO: Add your control notification handler code here
	CNotesDlg dlg;

	dlg.mode = this->mode;
	dlg.m_Notes = m_Notes;

	dlg.DoModal();


	m_Notes = dlg.m_Notes;
}

void CCustomerDlg::OnBnClickedCustomerDialogFilterShowAll()
{
	// TODO: Add your control notification handler code here
	RefreshFilters();
}

void CCustomerDlg::OnEnChangeCustomerDialogFilterSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString filterSearch;
	m_FilterSearchCtrl.GetWindowTextW(filterSearch);
	if (filterSearch.IsEmpty())
	{
		RefreshFilters();
		m_ShowAllFilters.EnableWindow(TRUE);
		return;
	}

	CRecordset rs(&theApp.m_Database);

	CString cmd, custFilters(TEXT("")), secondaryFilters;
	cmd.Format(TEXT("SELECT Filters FROM Customers WHERE Name='%s'"), m_CustomerName);
	if (rs.Open(CRecordset::forwardOnly, cmd))
	{
		if (!rs.IsEOF())
		{
			// Get A List Of Filters The Customer Has
			rs.GetFieldValue((short)0, custFilters);
		}
		rs.Close();
	}

	m_ShowAllFilters.EnableWindow(FALSE);

	cmd.Format(TEXT("SELECT PartNum,Size,SecondaryPartNum FROM Filters WHERE PartNum LIKE '%%%s%%' OR SecondaryPartNum LIKE '%%%s%%' ORDER BY PartNum"), filterSearch, filterSearch);

	if (rs.Open(CRecordset::forwardOnly, cmd))
	{
		m_FilterControl.DeleteAllItems();
		int i, nRow = 0;
		CString s, primary;
		while (!rs.IsEOF())
		{
			for (i = 0; i < rs.GetODBCFieldCount(); i++)
			{
				rs.GetFieldValue(i, s);
				if (i == FILTER_PARTNUM)
				{
					primary = s;
					secondaryFilters = TEXT("");
					if (mode == DIALOG_VIEW)
					{
						if (custFilters.Find(s) == -1)// Not A Customers Filter, Skip It
							break;
					}
					else
					{
						if (m_SelectedFilters.Find(s) != NULL)
						{
							m_FilterControl.InsertItem(nRow, s);
							m_FilterControl.SetCheck(nRow, TRUE);
						}
						else
							m_FilterControl.InsertItem(nRow, s);
					}
				}
				else if (i == FILTER_SIZE)
					m_FilterControl.SetItemText(nRow, i, s);
				else if (i == FILTER_SECONDARY)
					secondaryFilters = s;
			}
			if (!secondaryFilters.IsEmpty())
			{
				nRow++;
				CString restoken, secondary, upper;
				int curPos = 0;
				// Split Filter List Into Individual Filters
				restoken = s.Tokenize(TEXT("\r\n"), curPos);
				while (restoken != TEXT(""))
				{
					upper = restoken;
					upper.MakeUpper();
					filterSearch.MakeUpper();
					// All Secondary PartNum Are Gathered So
					// Check If This Secondary PartNum Matches Search
					if (upper.Find(filterSearch, 0) != -1)
					{
						secondary.Format(TEXT("%s(%s)"), primary, restoken);
						m_FilterControl.InsertItem(nRow, secondary);
						nRow++;
					}

					restoken = s.Tokenize(TEXT("\r\n"), curPos);
				}
			}
			rs.MoveNext();
		}
		rs.Close();
	}
}

void CCustomerDlg::OnLvnItemchangedCustomerDialogFilterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (mode == DIALOG_VIEW || pNMLV->uNewState == pNMLV->uOldState)
		return;

	CString filter = m_FilterControl.GetItemText(pNMLV->iItem, FILTER_PARTNUM);

	int primaryIndex = filter.Find(TEXT("("));//Check If It Is A Secondary Filter
	if (primaryIndex != -1)// Get Primary Filter PartNum
	{
		CString primary = filter.Left(primaryIndex);
		LVFINDINFO info;

		info.flags = LVFI_STRING;
		info.psz = primary;

		primaryIndex = m_FilterControl.FindItem(&info);
	}// primaryIndex == -1 than it is a primary filter



	if (pNMLV->uNewState == UNCHECKED)
	{
		if (primaryIndex == -1) // Remove And Unselect Primary Filter
		{
			POSITION pos = m_SelectedFilters.Find(filter);
			if (pos != NULL)
				m_SelectedFilters.RemoveAt(pos);
			m_FilterControl.SetItemState(pNMLV->iItem, ~LVIS_SELECTED, LVIS_SELECTED);
		}// Do Nothing With Secondary Filter
	}
	else if (pNMLV->uNewState == CHECKED)
	{
		if (primaryIndex != -1)// Secondary Filter Selected
		{
			// Check Primary Filter
			m_FilterControl.SetCheck(primaryIndex, TRUE);
			// Uncheck And Unselect Secondary Filter
			m_FilterControl.SetCheck(pNMLV->iItem, FALSE);
			m_FilterControl.SetItemState(pNMLV->iItem, ~LVIS_SELECTED, LVIS_SELECTED);
		}
		else// Primary Filter Checked
		{
			m_SelectedFilters.AddTail(filter);
			if ((m_FilterControl.GetItemState(pNMLV->iItem, LVIS_SELECTED) & LVIS_SELECTED) != LVIS_SELECTED)
				m_FilterControl.SetItemState(pNMLV->iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED);
		}
	}
	else
	{
		if ((pNMLV->uNewState & LVIS_SELECTED) == LVIS_SELECTED)
		{
			if (primaryIndex != -1)// Secondary Filter Selected
			{
				// Unselect Secondary Filter
				m_FilterControl.SetItemState(pNMLV->iItem, ~LVIS_SELECTED, LVIS_SELECTED);
				// Check If Primary Filter Has A Check
				if (!m_FilterControl.GetCheck(primaryIndex))
				{
					m_FilterControl.SetCheck(primaryIndex, TRUE);
				}
			}
			else// Primary Filter Selected
			{
				if (!m_FilterControl.GetCheck(pNMLV->iItem))
				{
					m_FilterControl.SetCheck(pNMLV->iItem, TRUE);
				}
			}
		}
		else if (m_FilterControl.GetCheck(pNMLV->iItem))// If Unselected Check If It Should Be Selected
		{
			m_FilterControl.SetItemState(pNMLV->iItem, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void CCustomerDlg::OnNMRClickCustomerDialogFilterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)
		return;

	if (m_Verify.GetCheck() == BST_UNCHECKED)
	{
		AfxMessageBox(TEXT("Verify Customer Name"));
		return;
	}

	CMenu mnuPopupOptions;
	mnuPopupOptions.LoadMenuW(IDR_POPUP_CUSTOMER_OPTIONS);

	CMenu *mnuPopupMenu = mnuPopupOptions.GetSubMenu(0);
	ASSERT(mnuPopupMenu);
	CString part = m_FilterControl.GetItemText(pNMItemActivate->iItem, 0);

	// GET POINT
	POINT point;
	if (GetCursorPos(&point))
	{
		BOOL option = mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
		if (option == ID_POPUP_VIEW)// View
		{
			CString part;

			part = m_FilterControl.GetItemText(pNMItemActivate->iItem, 0);

			CFilterDlg dlg;
			dlg.mode = DIALOG_VIEW;

			int curPos = 0;

			dlg.m_PartNum = part.Tokenize(TEXT("("), curPos);
			dlg.m_Search.Format(TEXT("%s"), m_CustomerName);

			dlg.ShowDialog();
		}
		else if (option == ID_POPUP_EDIT)// Edit
		{
			CString part;

			part = m_FilterControl.GetItemText(pNMItemActivate->iItem, 0);

			CFilterDlg dlg;
			dlg.mode = DIALOG_EDIT;

			int curPos = 0;

			dlg.m_PartNum = part.Tokenize(TEXT("("), curPos);
			dlg.m_Search.Format(TEXT("%s"), m_CustomerName);

			if (dlg.ShowDialog() == IDOK)
			{
				CString cmd;
				cmd.Format(TEXT("DELETE FROM CustomerFilters WHERE Name='%s' AND PartNum='%s'"), m_CustomerName, dlg.m_PartNum);
				theApp.m_Database.ExecuteSQL(cmd);

				cmd.Format(TEXT("INSERT INTO CustomerFilters (Name, PartNum, New, Reman, Clean, Notes) VALUES ('%s', '%s', '%s', '%s', '%s', '%s')"),
					m_CustomerName, dlg.m_PartNum, dlg.m_New, dlg.m_Reman, dlg.m_Clean, dlg.m_Notes);
				theApp.m_Database.ExecuteSQL(cmd);
				RefreshFilters();
			}
		}
	}
}

void CCustomerDlg::OnNMDblclkCustomerDialogFilterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)
		return;

	if (m_Verify.GetCheck() == BST_UNCHECKED)
	{
		AfxMessageBox(TEXT("Verify Customer Name"));
		return;
	}

	CString part;

	part = m_FilterControl.GetItemText(pNMItemActivate->iItem, 0);

	CFilterDlg dlg;
	dlg.mode = DIALOG_VIEW;

	int curPos = 0;

	dlg.m_PartNum = part.Tokenize(TEXT("("), curPos);


	dlg.m_Search.Format(TEXT("%s"), m_CustomerName);


	dlg.ShowDialog();
}

// FALSE == No Change, TRUE == Changes Made
BOOL CCustomerDlg::CheckChanges(BOOL ask)
{
	if (mode == DIALOG_VIEW)
		return FALSE;

	BOOL changes = FALSE;
	if (changes
		|| !Verify(&m_CustomerNameCtrl, m_OriginalName)
		|| !Verify(&m_ContactNumberCtrl, m_ContactNumber)
		|| !Verify(&m_FaxNumberCtrl, m_FaxNumber)
		|| !Verify(&m_ExtraInfoCtrl, m_ExtraInfo)
		|| !Verify(&m_EmailCtrl, m_EMail)
		|| !Verify(&m_StreetAddressCtrl, m_StreetAddress)
		|| !Verify(&m_CityCtrl, m_City)
		|| !Verify(&m_ProvinceCtrl, m_Province)
		|| !Verify(&m_ZipCodeCtrl, m_ZIPCode)
		|| m_InitialNotes.Compare(m_Notes) != 0)
		changes = TRUE;

	// Check Filter List If No Other Changes Found

	if (!changes)
	{
		if (m_SelectedFilters.GetCount() == m_InitialFilters.GetCount())
		{
			// Same Length. Check Content
			POSITION pos = m_InitialFilters.GetHeadPosition();

			while (pos != NULL)
			{
				if (m_SelectedFilters.Find(m_InitialFilters.GetAt(pos)) == NULL)
				{
					changes = TRUE;
					pos = NULL;
				}
				else
					m_InitialFilters.GetNext(pos);
			}
		}
		else
			changes = TRUE;
	}
	

	if (changes)
		if (ask)
			return (AfxMessageBox(TEXT("Save Changes?"), MB_YESNO) == IDYES);
		else
			return TRUE;

	return FALSE;
}

// TRUE == Same, FALSE == Different
BOOL CCustomerDlg::Verify(CEdit *edit, CString value)
{
	CString temp;
	edit->GetWindowTextW(temp);
	return temp.Compare(value) == 0;
}

void CCustomerDlg::SaveChanges()
{
	if (m_Verify.GetCheck() == BST_UNCHECKED)
	{
		AfxMessageBox(TEXT("Verify Customer Name"));
		return;
	}

	m_FilterList = TEXT("\r\n");
	POSITION pos = m_SelectedFilters.GetHeadPosition();

	while (pos != NULL)
	{
		m_FilterList.AppendFormat(TEXT("%s\r\n"), m_SelectedFilters.GetNext(pos));
	}

	CDialogEx::OnOK();
}

void CCustomerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (!CheckChanges(FALSE))
	{
		if (mode == DIALOG_ADD && Verify(&m_CustomerNameCtrl, TEXT("")))
			SaveChanges();
		else
			CDialogEx::OnCancel();
	}
	else
		SaveChanges();
}

void CCustomerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (CheckChanges())
		SaveChanges();
	else
		CDialogEx::OnCancel();
}

void CCustomerDlg::FormatPhoneNumber(CString *output)
{
	if (output->GetLength() > 10)
	{
		output->Insert(7, TEXT("-"));
		output->Insert(4, TEXT("-"));
		output->Insert(1, TEXT("-"));
	}
	else if (output->GetLength() > 7)
	{
		output->Insert(6, TEXT("-"));
		output->Insert(3, TEXT("-"));
	}
	else if (output->GetLength() == 7)
	{
		output->Insert(3, TEXT("-"));
	}
}

void CCustomerDlg::FormatNumber(CEdit* edit)
{
	CString number;
	edit->GetWindowTextW(number);

	if (number.SpanIncluding(TEXT("0123456789()-")) == number)
	{
		number.Replace(TEXT("("), TEXT(""));
		number.Replace(TEXT(")"), TEXT(""));
		number.Replace(TEXT("-"), TEXT(""));

		FormatPhoneNumber(&number);
		edit->SetWindowTextW(number);
	}
}

void CCustomerDlg::OnEnKillfocusCustomerDialogContactNumber()
{
	// TODO: Add your control notification handler code here
	FormatNumber(&m_ContactNumberCtrl);
}

void CCustomerDlg::OnEnKillfocusCustomerDialogFaxNumber()
{
	// TODO: Add your control notification handler code here
	FormatNumber(&m_FaxNumberCtrl);
}

void CCustomerDlg::OnBnClickedVerify()
{
	// TODO: Add your control notification handler code here
	m_Verify.EnableWindow(FALSE);
	CString name;

	m_CustomerNameCtrl.GetWindowTextW(name);

	if (name.IsEmpty())
	{
		AfxMessageBox(TEXT("Customer name is empty"));
		m_Verify.SetCheck(BST_UNCHECKED);
		m_Verify.EnableWindow(TRUE);
		m_CustomerNameCtrl.SetFocus();
		return;
	}
	// Name is not empty
	if (mode == DIALOG_ADD || m_CustomerName.Compare(name) != 0)
	{
		CRecordset rs(&theApp.m_Database);
		CString s;

		s.Format(TEXT("SELECT Name FROM Customers WHERE Name ='%s'"), name);

		if (rs.Open(CRecordset::forwardOnly, s))
		{
			if (!rs.IsEOF())
			{
				AfxMessageBox(TEXT("Customer Already Exists"));
				m_Verify.SetCheck(BST_UNCHECKED);
				m_Verify.EnableWindow(TRUE);
				m_CustomerNameCtrl.SetFocus();
				return;
			}
		}
	}

	CString cmd;
	cmd.Format(TEXT("UPDATE CustomerFilters SET Name='%s' WHERE Name='%s'"), name, m_CustomerName);
	theApp.m_Database.ExecuteSQL(cmd);

	m_CustomerName = name;
}

void CCustomerDlg::OnEnChangeCustomerDialogCustomerName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	m_Verify.SetCheck(BST_UNCHECKED);
	m_Verify.EnableWindow(TRUE);
}
