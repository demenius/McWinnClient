// NotesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "McWinnClient.h"
#include "NotesDlg.h"
#include "afxdialogex.h"


// CNotesDlg dialog

IMPLEMENT_DYNAMIC(CNotesDlg, CDialogEx)

CNotesDlg::CNotesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNotesDlg::IDD, pParent)
	, m_Notes(_T(""))
{

}

CNotesDlg::~CNotesDlg()
{
}

void CNotesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NOTES_DIALOG_NOTES, m_NotesCtrl);
	DDX_Control(pDX, IDOK, m_Confirm);
	DDX_Text(pDX, IDC_NOTES_DIALOG_NOTES, m_Notes);
}


BEGIN_MESSAGE_MAP(CNotesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDOK, &CNotesDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CNotesDlg message handlers


BOOL CNotesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_NotesCtrl.SetWindowTextW(m_Notes);
	if (mode == DIALOG_VIEW)
	{
		m_NotesCtrl.EnableWindow(FALSE);
		m_Confirm.SetWindowTextW(TEXT("Close"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CNotesDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			if (CheckChanges())
			{
				SaveChanges();
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CNotesDlg::CheckChanges(BOOL ask)
{
	CString new_Notes;
	m_NotesCtrl.GetWindowTextW(new_Notes);

	if (new_Notes.Compare(m_Notes) != 0)
	{
		if (ask)
			return (AfxMessageBox(TEXT("Save Changes?"), MB_YESNO) == IDYES);
		else
			return TRUE;
	}
	return FALSE;
}

void CNotesDlg::SaveChanges()
{
	CDialogEx::OnOK();
}

void CNotesDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (!CheckChanges(FALSE))
		CDialogEx::OnCancel();
	else
		SaveChanges();
}

void CNotesDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (!CheckChanges())
		CDialogEx::OnClose();
	else
		SaveChanges();
}