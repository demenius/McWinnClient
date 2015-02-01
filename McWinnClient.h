
// McWinnClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#include <vector>

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "afxdb.h"

#define DIALOG_ADD 0
#define DIALOG_EDIT 1
#define DIALOG_VIEW 2

#define FILTER_PARTNUM 0
#define FILTER_SIZE 1
#define FILTER_SECONDARY 2
#define FILTER_COST 3
#define FILTER_REPLACE 4
#define FILTER_CLEAN 5
#define FILTER_REMAN 6
#define FILTER_NOTES 7

#define CUSTOMER_NAME 0
#define CUSTOMER_CONTACT 1
#define CUSTOMER_FILTERS 2
#define CUSTOMER_FAX 3
#define CUSTOMER_EMAIL 4
#define CUSTOMER_STREETADDRESS 5
#define CUSTOMER_CITY 6
#define CUSTOMER_PROVINCE 7
#define CUSTOMER_ZIPCODE 8
#define CUSTOMER_PRIMARY_NOTES 9
#define CUSTOMER_SECONDARY_NOTES 10

#define UNCHECKED 4096
#define CHECKED 8192

#define DELETE_FILTER_MSG TEXT("Delete Filter: %s?")
#define DELETE_CUSTOMER_MSG TEXT("Delete Customer: %s?")

// CMcWinnClientApp:
// See McWinnClient.cpp for the implementation of this class
//

class CMcWinnClientApp : public CWinApp
{ 
public:
	CMcWinnClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	CDatabase m_Database;
};

extern CMcWinnClientApp theApp;