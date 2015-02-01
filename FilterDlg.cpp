// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "McWinnClient.h"
#include "FilterDlg.h"
#include "afxdialogex.h"

#include "NotesDlg.h"

// CFilterDlg dialog

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterDlg::IDD, pParent)
	, m_PartNum(_T(""))
	, m_Size(_T(""))
	, m_New(_T(""))
	, m_Reman(_T(""))
	, m_Clean(_T(""))
	, m_EquivalentFilters(_T(""))
	, m_CustomerList(_T(""))
	, m_Search(_T(""))
	, m_Notes(_T(""))
	, m_Cost(_T(""))
{

}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILTER_DIALOG_PART_NUMBER, m_PartNumCtrl);
	DDX_Text(pDX, IDC_FILTER_DIALOG_PART_NUMBER, m_PartNum);
	DDX_Control(pDX, IDC_FILTER_DIALOG_SIZE, m_SizeCtrl);
	DDX_CBString(pDX, IDC_FILTER_DIALOG_SIZE, m_Size);
	DDX_Control(pDX, IDC_FILTER_DIALOG_NEW, m_NewCtrl);
	DDX_Text(pDX, IDC_FILTER_DIALOG_NEW, m_New);
	DDX_Control(pDX, IDC_FILTER_DIALOG_REMAN, m_RemanCtrl);
	DDX_Text(pDX, IDC_FILTER_DIALOG_REMAN, m_Reman);
	DDX_Control(pDX, IDC_FILTER_DIALOG_CLEAN, m_CleanCtrl);
	DDX_Text(pDX, IDC_FILTER_DIALOG_CLEAN, m_Clean);
	DDX_Control(pDX, IDC_FILTER_DIALOG_COST, m_CostCtrl);
	DDX_Text(pDX, IDC_FILTER_DIALOG_COST, m_Cost);
	DDX_Control(pDX, IDC_FILTER_DIALOG_EQUIVALENT_FILTERS, m_EquivalentFiltersCtrl);
	DDX_Text(pDX, IDC_FILTER_DIALOG_EQUIVALENT_FILTERS, m_EquivalentFilters);
	DDX_Control(pDX, IDC_FILTER_DIALOG_CUSTOMER_LIST, m_CustomerCtrl);
	DDX_Control(pDX, IDC_FILTER_DIALOG_CUSTOMER_SEARCH, m_SearchCtrl);
	DDX_Text(pDX, IDC_FILTER_DIALOG_CUSTOMER_SEARCH, m_Search);
	DDX_Control(pDX, IDCANCEL, m_CancelCtrl);
	DDX_Control(pDX, IDOK, m_AddCtrl);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FILTER_DIALOG_NOTES, &CFilterDlg::OnBnClickedFilterDialogNotes)
	ON_BN_CLICKED(IDOK, &CFilterDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILTER_DIALOG_CUSTOMER_LIST, &CFilterDlg::OnLvnItemchangedFilterDialogCustomerList)
	ON_EN_CHANGE(IDC_FILTER_DIALOG_CUSTOMER_SEARCH, &CFilterDlg::OnEnChangeFilterDialogCustomerSearch)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CFilterDlg::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_FILTER_DIALOG_COST, &CFilterDlg::OnEnKillfocusFilterDialogCost)
	ON_EN_KILLFOCUS(IDC_FILTER_DIALOG_NEW, &CFilterDlg::OnEnKillfocusFilterDialogNew)
	ON_EN_KILLFOCUS(IDC_FILTER_DIALOG_CLEAN, &CFilterDlg::OnEnKillfocusFilterDialogClean)
	ON_EN_KILLFOCUS(IDC_FILTER_DIALOG_REMAN, &CFilterDlg::OnEnKillfocusFilterDialogReman)
END_MESSAGE_MAP()


// CFilterDlg message handlers
BOOL CFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 175;
	lvColumn.pszText = TEXT("Name");
	m_CustomerCtrl.InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 125;
	lvColumn.pszText = TEXT("Contact");
	m_CustomerCtrl.InsertColumn(1, &lvColumn);

	m_CustomerCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if (mode == DIALOG_VIEW)
	{
		CString title;
		title.Format(TEXT("View Filter - %s"), m_PartNum);
		this->SetWindowTextW(title);

		m_PartNumCtrl.SetReadOnly(TRUE);
		m_SizeCtrl.EnableWindow(FALSE);
		m_CostCtrl.SetReadOnly(TRUE);
		m_NewCtrl.SetReadOnly(TRUE);
		m_RemanCtrl.SetReadOnly(TRUE);
		m_CleanCtrl.SetReadOnly(TRUE);
		m_EquivalentFiltersCtrl.SetReadOnly(TRUE);
		m_AddCtrl.ShowWindow(FALSE);
		m_CancelCtrl.SetWindowTextW(TEXT("Close"));
	} else
	{
		if (m_Search.IsEmpty())
			m_CustomerCtrl.SetExtendedStyle(m_CustomerCtrl.GetExtendedStyle() | LVS_EX_CHECKBOXES);

		if (mode == DIALOG_EDIT)
		{
			m_AddCtrl.SetWindowTextW(TEXT("Edit"));
			CString title;
			title.Format(TEXT("Edit Filter - %s"), m_PartNum);
			this->SetWindowTextW(title);

			if (!m_Search.IsEmpty())
			{
				m_PartNumCtrl.SetReadOnly(TRUE);
				m_SizeCtrl.EnableWindow(FALSE);
				m_CostCtrl.SetReadOnly(TRUE);
				m_EquivalentFiltersCtrl.SetReadOnly(TRUE);
				m_CustomerCtrl.EnableWindow(FALSE);
				m_SearchCtrl.SetReadOnly(TRUE);
			}
		}
	}

	RefreshCustomers();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CFilterDlg::ShowDialog()
{
	// Retrieve dataset from table
	CRecordset rs(&theApp.m_Database);

	CString search;
	search.Format(TEXT("SELECT * FROM Filters WHERE PartNum='%s'"), m_PartNum);

	if (rs.Open(CRecordset::forwardOnly, search))
	{
		if (!rs.IsEOF())
		{
			rs.GetFieldValue((short)FILTER_SIZE, m_Size);
			rs.GetFieldValue((short)FILTER_SECONDARY, m_EquivalentFilters);
			rs.GetFieldValue((short)FILTER_COST, m_Cost);
			rs.GetFieldValue((short)FILTER_REPLACE, m_New);
			rs.GetFieldValue((short)FILTER_CLEAN, m_Clean);
			rs.GetFieldValue((short)FILTER_REMAN, m_Reman);
			rs.GetFieldValue((short)FILTER_NOTES, m_Notes);
			m_InitialNotes = m_Notes;
		}
		rs.Close();

		if (!m_Search.IsEmpty())
		{
			search.Format(TEXT("SELECT * FROM CustomerFilters WHERE Name ='%s' AND PartNum='%s'"), m_Search, m_PartNum);

			if (rs.Open(CRecordset::forwardOnly, search))
			{
				if (!rs.IsEOF())
				{
					rs.GetFieldValue((short)2, m_New);
					rs.GetFieldValue((short)3, m_Reman);
					rs.GetFieldValue((short)4, m_Clean);
					rs.GetFieldValue((short)5, m_Notes);
					m_InitialNotes = m_Notes;
				}
				rs.Close();
			}
		}
	}
	if (m_Search.IsEmpty())
	{
		CString temp;
		temp.Format(TEXT("\r\n%s\r\n"), m_PartNum);
		search.Format(TEXT("SELECT Name FROM Customers WHERE Filters LIKE '%%%s%%' ORDER BY Name"), temp);

		if (rs.Open(CRecordset::forwardOnly, search))
		{
			while (!rs.IsEOF())
			{
				rs.GetFieldValue((short)CUSTOMER_NAME, temp);
				m_SelectedCustomers.AddTail(temp);
				m_InitialCustomers.AddTail(temp);
				rs.MoveNext();
			}
			rs.Close();
		}
	}
	else
	{
		m_SelectedCustomers.AddTail(m_Search);
		m_InitialCustomers.AddTail(m_Search);
	}
	

	return (this->DoModal());
}

void CFilterDlg::RefreshCustomers()
{
	CString search;

	if (mode == DIALOG_VIEW)
	{
		CRecordset customerRS(&theApp.m_Database);
		CRecordset filterRS(&theApp.m_Database);
		CString filters, restoken, output;
		m_CustomerCtrl.DeleteAllItems();

		CString command;

		CString extra = TEXT("");

		m_SearchCtrl.GetWindowTextW(search);
		if (!search.IsEmpty())
		{
			extra.Format(TEXT("AND Name LIKE '%%%s%%'"), search);
		}

		command.Format(TEXT("SELECT Name,Contact FROM Customers WHERE Filters LIKE '%%\r\n%s\r\n%%' %s ORDER BY Name"), m_PartNum, extra);


		if (customerRS.Open(CRecordset::forwardOnly, command))
		{
			int nRow = 0;
			int index = 0;

			while (!customerRS.IsEOF())
			{
				for (int i = 0; i < customerRS.GetODBCFieldCount(); i++)
				{
					customerRS.GetFieldValue(i, output);
					if (i == 0)
						m_CustomerCtrl.InsertItem(nRow, output);
					else
						m_CustomerCtrl.SetItemText(nRow, i, output);
				}

				customerRS.MoveNext();
				nRow++;
			}

			customerRS.Close();
		}
	}
	else // Adding Or Editing Filter
	{
		CRecordset rs(&theApp.m_Database);
		CString s, search;
		m_CustomerCtrl.DeleteAllItems();

		CString command;

		CString extra = TEXT("");

		m_SearchCtrl.GetWindowTextW(search);
		if (!search.IsEmpty())
		{
			extra.Format(TEXT("WHERE Name LIKE '%%%s%%'"), search);
		}


		command.Format(TEXT("SELECT Name,Contact FROM Customers %s ORDER BY Name"), extra);

		if (rs.Open(CRecordset::forwardOnly, command))
		{
			int nRow = 0;

			while (!rs.IsEOF())
			{
				rs.GetFieldValue((short)CUSTOMER_NAME, s);
				if (m_SelectedCustomers.Find(s) != NULL)
				{
					m_CustomerCtrl.InsertItem(nRow, s);
					m_CustomerCtrl.SetCheck(nRow, TRUE);
				}
				else
					m_CustomerCtrl.InsertItem(nRow, s);
				rs.GetFieldValue((short)CUSTOMER_CONTACT, s);
				m_CustomerCtrl.SetItemText(nRow, CUSTOMER_CONTACT, s);

				rs.MoveNext();
				nRow++;
			}
			rs.Close();
		}
	}
}

void CFilterDlg::OnBnClickedFilterDialogNotes()
{
	// TODO: Add your control notification handler code here
	CNotesDlg dlg;
	dlg.mode = this->mode;
	dlg.m_Notes = m_Notes;
	if(dlg.DoModal() == IDOK)
		m_Notes = dlg.m_Notes;
}

void CFilterDlg::OnEnChangeFilterDialogCustomerSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	RefreshCustomers();
	/*CRecordset rs(&theApp.m_Database);
	CString search, cmd;
	m_SearchCtrl.GetWindowTextW(search);
	if (search.IsEmpty())
	{
		RefreshCustomers();
		return;
	}

	cmd.Format(TEXT("SELECT * FROM Customers WHERE Name LIKE '%%%s%%' ORDER BY Name"), search);

	if (rs.Open(CRecordset::forwardOnly, cmd))
	{
		m_CustomerCtrl.DeleteAllItems();
		int nRow = 0;
		while (!rs.IsEOF())
		{
			for (int i = 0; i < rs.GetODBCFieldCount(); i++)
			{
				rs.GetFieldValue(i, search);
				if (i == 0)
					m_CustomerCtrl.InsertItem(nRow, search);
				else
					m_CustomerCtrl.SetItemText(nRow, i, search);
			}
			nRow++;
			rs.MoveNext();
		}
		rs.Close();
	}*/
}

void CFilterDlg::OnLvnItemchangedFilterDialogCustomerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (mode == DIALOG_VIEW || pNMLV->uNewState == pNMLV->uOldState)
		return;

	CString name = m_CustomerCtrl.GetItemText(pNMLV->iItem, CUSTOMER_NAME);

	if (pNMLV->uNewState == UNCHECKED)
	{
		POSITION pos = m_SelectedCustomers.Find(name);
		if (pos != NULL)
			m_SelectedCustomers.RemoveAt(pos);
		m_CustomerCtrl.SetItemState(pNMLV->iItem, ~LVIS_SELECTED, LVIS_SELECTED);
	}
	else if (pNMLV->uNewState == CHECKED)
	{
		if (m_SelectedCustomers.Find(name) == NULL)
			m_SelectedCustomers.AddTail(name);
		if ((m_CustomerCtrl.GetItemState(pNMLV->iItem, LVIS_SELECTED) & LVIS_SELECTED) != LVIS_SELECTED)
			m_CustomerCtrl.SetItemState(pNMLV->iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		if ((pNMLV->uNewState & LVIS_SELECTED) == LVIS_SELECTED)
		{
			if (!m_CustomerCtrl.GetCheck(pNMLV->iItem))
			{
				m_CustomerCtrl.SetCheck(pNMLV->iItem, TRUE);
			}
		}
		else if (m_CustomerCtrl.GetCheck(pNMLV->iItem))
		{
			m_CustomerCtrl.SetItemState(pNMLV->iItem, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

}

// FALSE == No Change, TRUE == Changes Made
BOOL CFilterDlg::CheckChanges(BOOL ask)
{
	if (mode == DIALOG_VIEW)
		return FALSE;

	CString temp;
	m_SizeCtrl.GetWindowTextW(temp);
	BOOL changes = FALSE;
	if (!Verify(&m_PartNumCtrl, m_PartNum) 
		|| (temp.Compare(m_Size) != 0)
		|| !Verify(&m_CostCtrl, m_Cost)
		|| !Verify(&m_NewCtrl, m_New)
		|| !Verify(&m_CleanCtrl, m_Clean)
		|| !Verify(&m_RemanCtrl, m_Reman) 
		|| !Verify(&m_EquivalentFiltersCtrl, m_EquivalentFilters)
		|| m_InitialNotes.Compare(m_Notes) != 0)
		changes = TRUE;
	
	// Check Customer List If No Other Changes Found
	if (!changes)
	{
		if (m_InitialCustomers.GetCount() == m_SelectedCustomers.GetCount())
		{
			// Same Length. Check Content
			POSITION pos = m_InitialCustomers.GetHeadPosition();

			while (pos != NULL)
			{
				if (m_SelectedCustomers.Find(m_InitialCustomers.GetAt(pos)) == NULL)
				{
					changes = TRUE;
					pos = NULL;
				}
				else
					m_InitialCustomers.GetNext(pos);
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
BOOL CFilterDlg::Verify(CEdit *edit, CString value)
{
	CString temp;
	edit->GetWindowTextW(temp);
	return temp.Compare(value) == 0;
}

void CFilterDlg::SaveChanges()
{
	CString partNum;

	m_PartNumCtrl.GetWindowTextW(partNum);

	if (partNum.IsEmpty())
	{
		AfxMessageBox(TEXT("No Part Number Entered"));
		return;
	}

	CRecordset rs(&theApp.m_Database);
	CString s;

	if (m_PartNum.Compare(partNum) != 0)
	{
		s.Format(TEXT("SELECT PartNum FROM Filters WHERE PartNum ='%s'"), partNum);

		if (rs.Open(CRecordset::forwardOnly, s))
		{
			int nRow = 0;

			if (!rs.IsEOF())
			{
				AfxMessageBox(TEXT("Filter Already Exists"));
				return;
			}
		}
	}

	m_CustomerList = TEXT("\r\n");
	POSITION pos = m_SelectedCustomers.GetHeadPosition();
	while (pos != NULL)
	{
		m_CustomerList.AppendFormat(TEXT("%s\r\n"), m_SelectedCustomers.GetNext(pos));
	}

	CDialogEx::OnOK();
}

void CFilterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (!CheckChanges(FALSE))
	{
		if (mode == DIALOG_ADD && Verify(&m_PartNumCtrl, TEXT("")))
			SaveChanges();
		else
			CDialogEx::OnCancel();
	}
	else
		SaveChanges();
}

void CFilterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (CheckChanges())
		SaveChanges();
	else
		CDialogEx::OnCancel();
	
}

void CFilterDlg::FormatCurrency(CString *output)
{
	CString temp;
	double num = _tstof(*output);
	temp.Format(TEXT("%.2f"), num);

	for (int i = temp.GetLength() - 6; i > 0; i -= 3)
	{
		temp.Insert(i, TEXT(","));
	}
	output->Format(TEXT("$%s"), temp);
}

void CFilterDlg::FormatNumber(CEdit* edit)
{
	CString cost;
	edit->GetWindowTextW(cost);

	if (cost.SpanIncluding(TEXT("0123456789.,$")) == cost)
	{
		int index = cost.Find(TEXT("$"));
		if (index == 0 || index == -1)
		{
			index = cost.Find(TEXT("."));
			index = cost.Find(TEXT("."), index + 1);
			if (index == -1)
			{
				cost.Replace(TEXT("$"), TEXT(""));
				cost.Replace(TEXT(","), TEXT(""));

				FormatCurrency(&cost);
				edit->SetWindowTextW(cost);
			}
		}
	}
}

void CFilterDlg::OnEnKillfocusFilterDialogCost()
{
	// TODO: Add your control notification handler code here
	FormatNumber(&m_CostCtrl);
}

void CFilterDlg::OnEnKillfocusFilterDialogNew()
{
	// TODO: Add your control notification handler code here
	FormatNumber(&m_NewCtrl);
}

void CFilterDlg::OnEnKillfocusFilterDialogClean()
{
	// TODO: Add your control notification handler code here
	FormatNumber(&m_CleanCtrl);
}

void CFilterDlg::OnEnKillfocusFilterDialogReman()
{
	// TODO: Add your control notification handler code here
	FormatNumber(&m_RemanCtrl);
}
