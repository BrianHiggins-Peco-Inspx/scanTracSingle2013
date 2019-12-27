//ScanTrac Side View Source File
// EasySelectProductDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "EasySelectProductDialog.h"
#include "NoticeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern double vGlobalPercentToResizeY;
extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern unsigned long vGlobalDialogTitleColor;

/////////////////////////////////////////////////////////////////////////////
// CEasySelectProductDialog dialog


CEasySelectProductDialog::CEasySelectProductDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEasySelectProductDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEasySelectProductDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vNumberOfProducts = 0;
	vProductNumberSelected = 0;
	vCurrentFirstProductNumberShown = 1;
	SelectWindowForStyle = SelectCurrentProduct;
	vLightGreenBrush = ::CreateSolidBrush(cLightGreen);
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vQuickFindCharacter = ' ';
	vMouseState = 0;		//1 is left mouse button clicked down, 2 is left mouse button transistioned up, 3 is double clicked left mouse button
	vRecievedButtonPress = false;
	vMouseDownRepeatTimerHandle = 3000;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_ProductLabel);
	
	vLocalDisplayCollection.Add(&m_SubFunction8Button);
	vLocalDisplayCollection.Add(&m_SubFunction7Button);
	vLocalDisplayCollection.Add(&m_SubFunction6Button);
	vLocalDisplayCollection.Add(&m_SubFunction5Button);
	vLocalDisplayCollection.Add(&m_SubFunction4Button);
	vLocalDisplayCollection.Add(&m_SubFunction3Button);
	vLocalDisplayCollection.Add(&m_SubFunction2Button);
	vLocalDisplayCollection.Add(&m_SubFunction1Button);
}

CEasySelectProductDialog::~CEasySelectProductDialog()
{
	DeleteObject(vLightGreenBrush);
	DeleteObject(vGreenBrush);
	DeleteObject(vBlueBrush);
}

void CEasySelectProductDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	vNumberOfProducts = vLocalProductCollection->GetSize() - 
		CalculateNumberOfHiddenProducts();

	if (vNumberOfProducts == 1)
		SelectAndExit(1);
	
	// TODO: Add your message handler code here
	::SetupMenu(vLocalCWndCollection);
	UpdateProductDisplay();
	CString TempText;
	switch (SelectWindowForStyle)
	{												
		case SelectCurrentProduct:
			TempText.LoadString(IDS_SelectProductToRun);
		break;
		case SelectProductForEdit:
			TempText.LoadString(IDS_SelectProductToEdit);
		break;
		case SelectProductForCopy:
			TempText.LoadString(IDS_SelectProductToCopy);
		break;
		case SelectProductForDelete:
			TempText.LoadString(IDS_SelectProductToDelete);
		break;
		case SelectProductForDisplay:
			TempText.LoadString(IDS_SelectProductToDisplay);
		break;
		case SelectProductForNew:
			TempText.LoadString(IDS_SelectProductToCopy);
		break;
		case SelectProductForEvaluate:
			TempText.LoadString(IDS_SelectProductToEvaluate);
		break;
	}
	SetDlgItemText(IDC_DialogTitleStaticText1,TempText);
  if (vGlobalCurrentProduct)
	{
		TempText.LoadString(IDS_CurrentProduct);
		TempText = TempText + ": ";
		CString TempString3(*vGlobalCurrentProduct->GetProductName());
		if (TempString3 == "")
			TempString3 = "Blank String 1";
		else
			TempString3 = TempText + TempString3;
		//copy product name text into Label control on screen 
		SetDlgItemText(IDC_ProductLabel,TempString3);
	}
	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);
	TempText.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempText);
	if (vGlobalCurrentProduct)
	{
		TempText.LoadString(IDS_CurrentProduct);
		SetDlgItemText(IDC_Function2Button,TempText);
		m_Function2Button.ShowWindow(SW_SHOW);
	}

	this->SetWindowText(_T("Select Product"));
}

void CEasySelectProductDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEasySelectProductDialog)
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_ProductLabel, m_ProductLabel);
}


BEGIN_MESSAGE_MAP(CEasySelectProductDialog, CDialog)
	//{{AFX_MSG_MAP(CEasySelectProductDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CEasySelectProductDialog::OnDialogtitlestatictext1)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasySelectProductDialog message handlers

void CEasySelectProductDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	CDialog::EndDialog(10);
}

void CEasySelectProductDialog::OnFunction2Button() 
{
  if (vGlobalCurrentProduct)
	{
		//figure out what product number is current product
		CString TempCurrentProductName(*vGlobalCurrentProduct->GetProductName());
		CString TempTestProductName("");

		WORD vCurrentProductNumber = 0;;
		WORD TempNumberOfProducts = vLocalProductCollection->GetSize();
		CProduct *TempProduct = NULL;
		for (WORD TempLoop = 0; TempLoop < TempNumberOfProducts;  TempLoop++)
		{
			TempProduct = vLocalProductCollection->GetAt(TempLoop);
			if (TempProduct)
			{
				TempTestProductName = *TempProduct->GetProductName();
				if (TempTestProductName == TempCurrentProductName)
					vCurrentProductNumber = TempLoop;
			}
		}
		vProductNumberSelected = vCurrentProductNumber + 1;
		CDialog::EndDialog(true);
	}
}

void CEasySelectProductDialog::OnFunction3Button() 
{
	//Previous Group button was pressed
  if (vCurrentFirstProductNumberShown > 1)
  {
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down Easy Select IN FUNCTION 3",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState == 3) || (vMouseState == 4)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 1;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		if (vCurrentFirstProductNumberShown > 8)
      vCurrentFirstProductNumberShown = vCurrentFirstProductNumberShown - 8;
		else
			vCurrentFirstProductNumberShown = 1;
		UpdateProductDisplay();
  }	
}

void CEasySelectProductDialog::OnFunction4Button() 
{
	//Cancel button pressed
	CDialog::OnCancel();
}

void CEasySelectProductDialog::OnFunction5Button() 
{
	//next button pressed to display next group of products
	if (vNumberOfProducts > vCurrentFirstProductNumberShown + 7)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down Easy Select IN FUNCTION 5",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState == 3) || (vMouseState == 4)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 2;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		//if (vCurrentFirstProductNumberShown + 15 > vNumberOfProducts)
		//	vCurrentFirstProductNumberShown = vNumberOfProducts - 7;
		//else
			vCurrentFirstProductNumberShown = vCurrentFirstProductNumberShown + 8;
		UpdateProductDisplay();
	}
}

void CEasySelectProductDialog::OnSubFunction1Button() 
{
	SelectAndExit(1);
}

void CEasySelectProductDialog::OnSubFunction2Button() 
{
	SelectAndExit(2);
}

void CEasySelectProductDialog::OnSubFunction3Button() 
{
	SelectAndExit(3);
}

void CEasySelectProductDialog::OnSubFunction4Button() 
{
	SelectAndExit(4);
}

void CEasySelectProductDialog::OnSubFunction5Button() 
{
	SelectAndExit(5);
}

void CEasySelectProductDialog::OnSubFunction6Button() 
{
	SelectAndExit(6);
}

void CEasySelectProductDialog::OnSubFunction7Button() 
{
	SelectAndExit(7);
}

void CEasySelectProductDialog::OnSubFunction8Button() 
{
	SelectAndExit(8);
}

void CEasySelectProductDialog::SelectAndExit(WORD TempProductNumber) 
{
	if ((TempProductNumber > 0) &&
		(vCurrentFirstProductNumberShown - 1 + TempProductNumber <= 
		vNumberOfProducts))
	{
		//figure out what product number is selected by the button press
		vProductNumberSelected = vCurrentFirstProductNumberShown - 1 + TempProductNumber;

		CDialog::EndDialog(true);
	}
	/*
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText.LoadString(IDS_NoProductSelected);
		TempNoticeDialog.vNoticeText = TempText;
		TempText.LoadString(IDS_Error);
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	*/
}

WORD CEasySelectProductDialog::CalculateNumberOfHiddenProducts()
{
	WORD TempNumberOfHiddenProducts = 0;
	if (!PasswordOK(cTemporaryInspxPassword,false))
	{
		vNumberOfProducts = vLocalProductCollection->GetSize();
		CProduct *TempProduct;
		CString *TempProductName;
		for (WORD TempLoop = 0; TempLoop < vNumberOfProducts; TempLoop++)
		{
			TempProduct = vLocalProductCollection->GetAt(TempLoop);
			TempProductName = TempProduct->GetProductName();
			if (TempProductName->GetAt(0) == '~')
				TempNumberOfHiddenProducts++;
		}
	}
	return TempNumberOfHiddenProducts;
}
/*
void CEasySelectProductDialog::AddProduct(CProduct *TempProduct)
{
	if (vLocalProductCollection->GetSize() > 0)
	{
		int TempIndex = 0;
		CString *NewProductName = TempProduct->GetProductName();
		CString *CompareProductName = vLocalProductCollection->GetAt(TempIndex)->GetProductName();
		while ((*NewProductName > *CompareProductName) &&
					(TempIndex < vLocalProductCollection->GetSize()))
		{
			TempIndex++;
			if (TempIndex <= vLocalProductCollection->GetUpperBound())
		  CompareProductName = vLocalProductCollection->GetAt(TempIndex)->GetProductName();
		}
	  vLocalProductCollection->InsertAt(TempIndex, TempProduct);
	}
	else
	  vLocalProductCollection->Add(TempProduct);
}
*/
void CEasySelectProductDialog::UpdateProductDisplay()
{
	CWnd* pWnd;
	CDC* pDC;
	HWND TempDialogHwnd = this->m_hWnd;

	CProduct *TempProduct;
	CString *TempProductName;
	CString TempProductNameString("");
	CString TempBlankName(" ");
	CString TempText("");
	WORD TempLength = 0;
	for (WORD TempLoop = 1; TempLoop < 9;  TempLoop++)
	{
		if (vCurrentFirstProductNumberShown + TempLoop - 2 < vNumberOfProducts)
		{
			TempProduct = vLocalProductCollection->GetAt(vCurrentFirstProductNumberShown + TempLoop - 2);
			TempProductName = TempProduct->GetProductName();
			/*
			if (((*TempProductName == "~Default") || (*TempProductName == "~Calibrate")) && 
				(!PasswordOK(cTemporaryInspxPassword,false)))
			{
				TempLength = 20;  //blank this box as it is ~calibrate or ~default
			}
			else
			*/
			{
				if (TempProductName->IsEmpty())
					TempProductNameString = "Blank Name 2";
				else
				{
					if (TempLoop > 4)
						TempProductNameString = *TempProductName + TempBlankName;
					else
						TempProductNameString =  TempBlankName + *TempProductName;
				}
			}
			TempLength = TempProductNameString.GetLength();
		}
		else
			TempLength = 0;

		switch (TempLoop)
		{
			case 1:
				if (!TempLength)
				{
					m_SubFunction1Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction1Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction1Button,TempProductNameString);

					pDC = m_SubFunction1Button.GetDC();
					pWnd = &m_SubFunction1Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5) + 30;  //5 is medium large
					m_SubFunction1Button.SetWindowPos(NULL, 11, 52, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 2:
				if (!TempLength)
				{
					m_SubFunction2Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction2Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction2Button,TempProductNameString);

					pDC = m_SubFunction2Button.GetDC();
					pWnd = &m_SubFunction2Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5) + 30;  //5 is medium large
					m_SubFunction2Button.SetWindowPos(NULL, 11, 142, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 3:
				if (!TempLength)
				{
					m_SubFunction3Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction3Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction3Button,TempProductNameString);

					pDC = m_SubFunction3Button.GetDC();
					pWnd = &m_SubFunction3Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5) + 30;  //5 is medium large
					m_SubFunction3Button.SetWindowPos(NULL, 11, 232, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 4:
				if (!TempLength)
				{
					m_SubFunction4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction4Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction4Button,TempProductNameString);

					pDC = m_SubFunction4Button.GetDC();
					pWnd = &m_SubFunction4Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5) + 30;  //5 is medium large
					m_SubFunction4Button.SetWindowPos(NULL, 11, 327, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 5:
				if (!TempLength)
				{
					m_SubFunction5Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction5Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction5Button,TempProductNameString);

					pDC = m_SubFunction5Button.GetDC();
					pWnd = &m_SubFunction5Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5) + 30;  //5 is medium large
					m_SubFunction5Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 79, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 6:
				if (!TempLength)
				{
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction6Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction6Button,TempProductNameString);

					pDC = m_SubFunction6Button.GetDC();
					pWnd = &m_SubFunction6Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5) + 30;  //5 is medium large
					m_SubFunction6Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 169, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 7:
				if (!TempLength)
				{
					m_SubFunction7Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction7Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction7Button,TempProductNameString);

					pDC = m_SubFunction7Button.GetDC();
					pWnd = &m_SubFunction7Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5) + 30;  //5 is medium large
					m_SubFunction7Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 259, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 8:
				if (!TempLength)
				{
					m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction8Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction8Button,TempProductNameString);

					pDC = m_SubFunction8Button.GetDC();
					pWnd = &m_SubFunction8Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5) + 30;  //5 is medium large
					m_SubFunction8Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 354, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
		}
	}
	if (vNumberOfProducts > vCurrentFirstProductNumberShown + 7)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Next);
		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_Function5Button, _T(""));
	}

	if (vCurrentFirstProductNumberShown > 1)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Previous);
		SetDlgItemText(IDC_Function3Button,TempText);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_Function3Button, _T(""));
	}
	::SetupMenu(vLocalDisplayCollection);
}

BOOL CEasySelectProductDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * TempWindow = NULL;
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (vMouseState > 0)
		{
			//ReportErrorMessage("Screen Clicked, repeat ended",cWriteToLog, 0);
			KillTimer(vMouseDownRepeatTimerHandle);
			vMouseState = 0;
		}
	}
	else
	if (pMsg->message == WM_LBUTTONUP)
	{
		//ReportErrorMessage("Left Button Up",cAction, 32000);
		if (vMouseState < 3)
		{
			if (vMouseState > 0)
			{
				KillTimer(vMouseDownRepeatTimerHandle);
				vMouseState = 0;
			}
		}
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
		vRecievedButtonPress = true;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		if (TempKey != 16)
		if (((TempKey >= c0) && (TempKey <= c9)) || (TempKey == 192))
		{
			switch (TempKey)
			{
				case 192: 
						vQuickFindCharacter = '~'; 
				break;
				case c0: 
					if (vQuickFindCharacter == 'Y')
						vQuickFindCharacter = 'Z'; 
					else
						vQuickFindCharacter = 'Y'; 
				break;
				case c1: 
					if (vQuickFindCharacter == '9')
						vQuickFindCharacter = '0'; 
					else
					if ((vQuickFindCharacter >= '0') && (vQuickFindCharacter < '9'))
						vQuickFindCharacter++; 
					else
						vQuickFindCharacter = '0'; 
				break;
				case c2: 					
					if (vQuickFindCharacter == 'A')
						vQuickFindCharacter = 'B'; 
					else
					if (vQuickFindCharacter == 'B')
						vQuickFindCharacter = 'C'; 
					else
						vQuickFindCharacter = 'A'; 
				break;
				case c3: 					
					if (vQuickFindCharacter == 'D')
						vQuickFindCharacter = 'E'; 
					else
					if (vQuickFindCharacter == 'E')
						vQuickFindCharacter = 'F'; 
					else
						vQuickFindCharacter = 'D';
				break;
				case c4:
					if (vQuickFindCharacter == 'G')
						vQuickFindCharacter = 'H'; 
					else
					if (vQuickFindCharacter == 'H')
						vQuickFindCharacter = 'I'; 
					else
						vQuickFindCharacter = 'G';
				break;
				case c5:
					if (vQuickFindCharacter == 'J')
						vQuickFindCharacter = 'K'; 
					else
					if (vQuickFindCharacter == 'K')
						vQuickFindCharacter = 'L'; 
					else
						vQuickFindCharacter = 'J';
				break;
				case c6:
					if (vQuickFindCharacter == 'M')
						vQuickFindCharacter = 'N'; 
					else
					if (vQuickFindCharacter == 'N')
						vQuickFindCharacter = 'O'; 
					else
						vQuickFindCharacter = 'M';
				break;
				case c7:
					if (vQuickFindCharacter == 'P')
						vQuickFindCharacter = 'Q'; 
					else
					if (vQuickFindCharacter == 'Q')
						vQuickFindCharacter = 'R'; 
					else
						vQuickFindCharacter = 'P';
				break;
				case c8:
					if (vQuickFindCharacter == 'S')
						vQuickFindCharacter = 'T'; 
					else
					if (vQuickFindCharacter == 'T')
						vQuickFindCharacter = 'U'; 
					else
						vQuickFindCharacter = 'S';
				break;
				case c9:
					if (vQuickFindCharacter == 'V')
						vQuickFindCharacter = 'W'; 
					else
					if (vQuickFindCharacter == 'W')
						vQuickFindCharacter = 'X'; 
					else
						vQuickFindCharacter = 'V';
				break;
			}

			vCurrentFirstProductNumberShown = 0;
			if (vNumberOfProducts > 8)
			{
				CProduct *TempProduct = vLocalProductCollection->GetAt(0);
				if (TempProduct)
				{
					CString TempProductName = *TempProduct->GetProductName();
					while ((vCurrentFirstProductNumberShown + 7 < vNumberOfProducts) &&
						(vQuickFindCharacter > TempProductName.GetAt(0)))
					{
						vCurrentFirstProductNumberShown++;
						if (vCurrentFirstProductNumberShown + 7 < vNumberOfProducts)
						{
							TempProduct = vLocalProductCollection->GetAt(vCurrentFirstProductNumberShown);
							TempProductName = *TempProduct->GetProductName();
						}
					}
				}
				if (vCurrentFirstProductNumberShown + 7 < vNumberOfProducts)
					vCurrentFirstProductNumberShown++;
				UpdateProductDisplay();
			}
		}
		else
		switch (TempKey)
		{
			case 113: // F2:
				OnSubFunction5Button();
			break;
			case 114: // F3:
				OnSubFunction7Button();
			break;
			case 115: // F4:
				OnSubFunction6Button();
			break;
			case 116: // F5:
				OnSubFunction1Button();
			break;
			case 117: // F6 Key - Function 5:
				OnSubFunction4Button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnFunction1Button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnSubFunction8Button();
			break;
			case 120: // F9 Key - Sub Function 3:
				OnSubFunction2Button();
			break;
			case 122: // F11 Key - Sub Function 4:
				OnSubFunction3Button();
			break;
			case 111: // / Key - Sub Function 5:
				OnFunction2Button();
			break;
			case 106: // * Key - Sub Function 6:
				OnFunction4Button();
			break;
			case 109: // - Key - Sub Function 7  Decimal Point:
				OnFunction3Button();
			break;
			case 107: // + Key - Sub Function 8:
				OnFunction5Button();
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 2, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		vRecievedButtonPress = false;
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CEasySelectProductDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	
  if (pWnd->GetDlgCtrlID() == IDC_ProductLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
		//return vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
  if (pWnd->GetDlgCtrlID() == IDC_Background) 
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CEasySelectProductDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
void CEasySelectProductDialog::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
}

