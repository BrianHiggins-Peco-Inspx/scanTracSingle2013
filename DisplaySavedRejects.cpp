// DisplaySavedRejects.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "DisplaySavedRejects.h"
#include "YesNoDialog.h"
#include "SelectItemDialog.h"
#include "NumericEntryDialog.h"
#include <string.h>
#include "windowsx.h"  //for GlobalAllocPtr

#ifdef InScanTrac
//#include "SystemConfigurationData.h"
#else
#include "NoticeDialog.h"
#include "Imgutil.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef InScanTrac
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
#endif
const WORD cWindowY = 338;
const WORD cWindowX = 416;
const WORD cWindowOffsetX = 108;
const WORD cWindowOffsetY = 43;

/////////////////////////////////////////////////////////////////////////////
// CDisplaySavedRejects dialog


CDisplaySavedRejects::CDisplaySavedRejects(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplaySavedRejects::IDD, pParent)
{
	vShowBox = true;
	vDateOfImage = 0;  //digits 1,2 are date, digits 3,4 are month, digits 5-8 are year
	vTimeOfImage = 0;  //digits 1,2 are seconds, digits 3,4 are minutes, digits 5,6 are hours
	vImageTimeString = " ";
	vImageDateString = " ";
	vCurrentImageFileName = "";
	vRejectImage = NULL;
	vRejectImageX = 0;
	vRejectImageY = 0;
	vProductName = "";
	vImageHasBox = false;
	vScanTracRejectedImagesDirectory = "";

#ifdef InScanTrac
	vDisplayTimerHandle = 31341;
	vGrabFocusTimerHandle = 51344;
	vGrabFocusAgainTimerHandle = 51345;
	vGreyPreviousImageButton = true;
	vGreyNextImageButton = true;
#else
	vButtonColorBrush = ::CreateSolidBrush(cButtonColor);
	vFoundDateFolders = false;
#endif
	//{{AFX_DATA_INIT(CDisplaySavedRejects)
	//}}AFX_DATA_INIT
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_EjectorDisabledDisplay);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Clock);
	vLocalCWndCollection.Add(&m_DateDisplay);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_FileNameLine);
	vLocalCWndCollection.Add(&m_InspectionNameDisplay);
	vLocalCWndCollection.Add(&m_ProductNameDisplay);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_TimeDisplay);
}

CDisplaySavedRejects::~CDisplaySavedRejects()
{
}

void CDisplaySavedRejects::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplaySavedRejects)
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_EjectorDisabledDisplay, m_EjectorDisabledDisplay);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Clock, m_Clock);
	DDX_Control(pDX, IDC_DateDisplay, m_DateDisplay);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_FileNameLine, m_FileNameLine);
	DDX_Control(pDX, IDC_InspectionNameDisplay, m_InspectionNameDisplay);
	DDX_Control(pDX, IDC_ProductNameDisplay, m_ProductNameDisplay);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_TimeDisplay, m_TimeDisplay);
}


BEGIN_MESSAGE_MAP(CDisplaySavedRejects, CDialog)
	//{{AFX_MSG_MAP(CDisplaySavedRejects)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
#ifdef InScanTrac
	ON_WM_TIMER()
#endif
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CDisplaySavedRejects::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplaySavedRejects message handlers

void CDisplaySavedRejects::OnFunction1Button() 
{
	// Exit Button was pressed - Send to main menu
	CDialog::EndDialog(10);	
}

void CDisplaySavedRejects::OnFunction2Button() 
{
	// Choose directory to get images from
#ifdef InScanTrac
	if (!vLocalConfigurationData)
#endif
	{
		bool TempHaveDirectory = false;
		CString TempDateString = " ";

	//	LPCTSTR lpszDefExt = ".BMP";
		LPCTSTR lpszFileName = NULL;
		//LPCTSTR lpszFilter = NULL;
		//CWnd* pParentWnd = NULL;
		LPCTSTR szFilter = _T("Reject Image Files (*.BMP)|*.BMP||");//|All Files (*.*)|*.*||";

			
		CFileDialog TempGetDirectory(true, NULL, lpszFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter,  NULL );

		int TempOK = TempGetDirectory.DoModal();
		if (TempOK == IDOK)
		{
			vCurrentImageFileName = vScanTracRejectedImagesDirectory = TempGetDirectory.GetPathName();
			int TempDirectoryLength = vScanTracRejectedImagesDirectory.ReverseFind('\\');

			int TempLength = vScanTracRejectedImagesDirectory.GetLength() - TempDirectoryLength - 1;
			if (vScanTracRejectedImagesDirectory.GetLength() > TempLength)
				vImageTimeString = vScanTracRejectedImagesDirectory.Right(vScanTracRejectedImagesDirectory.GetLength() - TempDirectoryLength - 1);

			CString TempString = vImageTimeString;
			vProductName = TempString.Left(TempString.Find('-'));
			TempString.Delete( 0,TempString.Find('-') - 1 );
			if (TempString.GetLength() > 21)
			{
				vImageDateString = TempString.Mid(2, 10);
				TempDateString = TempString.Mid(2,4) + TempString.Mid(7,2) + TempString.Mid(10,2);
				TempString = TempString.Mid(13,2) + TempString.Mid(16,2) + TempString.Mid(19,2) + TempString.Mid(22, (TempString.ReverseFind('-') - 22));
			}
			if (TempString.GetLength() == 7)
				TempString.Insert(6,'0');
			vTimeOfImage = _wtoi(TempString);
			TempString.Insert(2, ':');
			TempString.Insert(5, ':');
			TempString.Insert(8, '.');
			vImageTimeString = TempString;

			vDateOfImage = _wtoi(TempDateString);

			vScanTracRejectedImagesDirectory = vScanTracRejectedImagesDirectory.Left(TempDirectoryLength + 1);
			
			if (vScanTracRejectedImagesDirectory.GetLength() > 12)
				TempDateString = vScanTracRejectedImagesDirectory.Right(12);
			TempDateString.Remove('\\');
			TempDateString.Remove('-');
			
			if (vDateOfImage == _wtoi(TempDateString))
				vScanTracRejectedImagesDirectory = vScanTracRejectedImagesDirectory.Left(TempDirectoryLength-10);
			
			FindPreviousImage();
			DisplayImage();		//puts image in byte buffer vRejectImage, ready to paint
			
			if (vScanTracRejectedImagesDirectory.GetLength() > 2)
			if(vScanTracRejectedImagesDirectory.GetLength() < 500)
			if (vRejectImage || PreviousImageExists() || NextImageExists())//don't save path to folder if did not have a good image
			{
				TempHaveDirectory = true;
				DWORD TempCount = 0;
				LPTSTR TempRegistryValue = _T("");
				CRegKey TempRegistryKey;
				CString TempText("");

				LONG TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("Software"), KEY_ALL_ACCESS);

				if (TempErrorCode == ERROR_SUCCESS)
				{
					TempErrorCode = TempRegistryKey.Create(HKEY_LOCAL_MACHINE, _T("Software\\Inspx\\ScanTrac RejectDisplay"));

					if (TempErrorCode == ERROR_SUCCESS)
					{
						TempErrorCode = TempRegistryKey.SetKeyValue(_T(""), vScanTracRejectedImagesDirectory, _T("LastDirectoryName"));
					}
					
					TempRegistryKey.Close();
					
				}
			}
		}

		if (TempHaveDirectory)
		{	//un-grey out navigation buttons
			
			vFunction3ButtonEnable = true;	//m_Function3Button.EnableWindow(true);
			vFunction4ButtonEnable = true;	//m_Function4Button.EnableWindow(true);
			vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
			vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
			vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
			DisplayImage();
		}
		else
		{
			vFunction3ButtonEnable = false;	//m_Function3Button.EnableWindow(false);
			vFunction4ButtonEnable = false;	//m_Function4Button.EnableWindow(false);
			vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
			vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
			vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
			
		}
		ReDrawScreen();
	}		
}

void CDisplaySavedRejects::OnFunction3Button() 
{
	if ((vRejectImage == NULL) && (!NextImageExists() && !PreviousImageExists()))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	if (vFoundDateFolders == false)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nFile not in proper date-folder format\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

		// Goto Date and Time

		//set time
	if ( !NextImageExists() && !PreviousImageExists() )
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Previous or Next Images";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 2000;
		TempNoticeDialog.DoModal();
		return;
	}

	if (vFunction3ButtonEnable == true)
	{
	bool TempOK = true;
	WORD TempMonth = (vDateOfImage%10000)/100;
	WORD TempDay = vDateOfImage%100;
	WORD TempYear = vDateOfImage/10000;
	WORD TempHour = vTimeOfImage/1000000;
	WORD TempMinute = (vTimeOfImage%1000000)/10000;
	WORD TempSecond = (vTimeOfImage%10000)/100;


	CTime TempTime;
	TempTime = CTime::GetCurrentTime();
	CString TempTimeString = TempTime.Format("%#I:%M:%S%p");
	
	CString TempDateString = TempTime.Format("%B %#d, %Y");

	CNumericEntryDialog INumericEntryDialog;  


		
	if (TempOK)		//YEAR
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(TempYear, 0); 
	
		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Enter In YEAR of Image Search";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Today's Date - Time: " + TempDateString + " - " + TempTimeString;
		INumericEntryDialog.m_UnitsString = "Current Image Date - Time: " + vImageDateString + " - " + vImageTimeString;

		INumericEntryDialog.vMaxValue = 2098;
		INumericEntryDialog.vMinValue = 2000;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			TempYear = _wtoi(INumericEntryDialog.vEditString);
			if (TempYear < 2001)
				TempYear = 2001;
		}
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	
	if (TempOK)		//MONTH
	{
		CSelectItemDialog ISelectItemDialog;  
		ISelectItemDialog.vNumberOfItems = 12;
		ISelectItemDialog.vItems[0] = "January";
		ISelectItemDialog.vItems[1] = "February";
		ISelectItemDialog.vItems[2] = "March";
		ISelectItemDialog.vItems[3] = "April";
		ISelectItemDialog.vItems[4] = "May";
		ISelectItemDialog.vItems[5] = "June";
		ISelectItemDialog.vItems[6] = "July";
		ISelectItemDialog.vItems[7] = "August";
		ISelectItemDialog.vItems[8] = "September";
		ISelectItemDialog.vItems[9] = "October";
		ISelectItemDialog.vItems[10] = "November";
		ISelectItemDialog.vItems[11] = "December";
		ISelectItemDialog.vOkayMessage = "Default: " + ISelectItemDialog.vItems[TempMonth-1];

		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Today : " + TempDateString + " - " + TempTimeString;
		ISelectItemDialog.vBackgroundColor = cGreen;
		ISelectItemDialog.vShowExit = true;
		ISelectItemDialog.m_DialogTitleStaticText2 = "Enter MONTH of Image Search";
		ISelectItemDialog.m_DialogTitleStaticText3 = "Current Image Date - Time: " + vImageDateString + " - " + vImageTimeString;

		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
			TempMonth = ISelectItemDialog.vItemNumberSelected;
		else
		if (nResponse != 11)
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	if (TempOK)		//DAY
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(TempDay, 0); 

		INumericEntryDialog.m_DialogTitleStaticText3 = "Enter In DAY of Image Search";

		INumericEntryDialog.vMaxValue = 31;
		INumericEntryDialog.vMinValue = 1;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
			TempDay = _wtoi(INumericEntryDialog.vEditString);
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}

	if (TempOK)		//HOUR
	{
		INumericEntryDialog.vEditString = dtoa(TempHour, 0); 
		INumericEntryDialog.m_DialogTitleStaticText3 = "Enter In HOUR of Image Search";

		INumericEntryDialog.vMaxValue = 23;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;

		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
			TempHour = _wtoi(INumericEntryDialog.vEditString);
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	if (TempOK)		//MINUTE
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(TempMinute, 0); 
		INumericEntryDialog.m_DialogTitleStaticText3 = "Enter In MINUTE of Image Search";

		INumericEntryDialog.vMaxValue = 59;
		INumericEntryDialog.vIntegerOnly = true;

		int nResponse = INumericEntryDialog.DoModal();
		if (nResponse == IDOK)
			TempMinute = _wtoi(INumericEntryDialog.vEditString);
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	if (TempOK)
	{
		INumericEntryDialog.vEditString = dtoa(TempSecond, 0); 

		INumericEntryDialog.m_DialogTitleStaticText3 = "Enter In SECOND of Image Search";

		INumericEntryDialog.vMaxValue = 59;
		INumericEntryDialog.vIntegerOnly = true;

		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
			TempSecond = _wtoi(INumericEntryDialog.vEditString);
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	if (TempOK)
	{
		vImageDateString = dtoa(TempYear) + " - " + dtoa(TempMonth) + " - " + dtoa(TempDay);
		vImageTimeString = dtoa(TempHour) + " - " + dtoa(TempMinute) + " - " + dtoa(TempSecond);

		vDateOfImage = TempYear * 10000 + TempMonth * 100 + TempDay;  //digits 1,2 are date, digits 3,4 are month, digits 5-8 are year
		vTimeOfImage = TempHour * 1000000 + TempMinute * 10000 + TempSecond * 100 + 01;	//1,2 hour; 3,4 minute; 5,6 second; 7,8 sequence in second number

		FindPreviousImage();
		DisplayImage();		//puts image in byte buffer vRejectImage, ready to paint
		ReDrawScreen();
	}
	}
	else
	{
		FunctionNotAvailable();
	}
}

void CDisplaySavedRejects::OnFunction4Button() 
{
	if ((vRejectImage == NULL) && (!NextImageExists() && !PreviousImageExists()))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	if (vFoundDateFolders == false)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nFile not in proper date-folder format\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	if (vFunction4ButtonEnable == true)
	{
	// Previous Image
	if (PreviousImageExists())
	{
		int TempOldImageDate = vDateOfImage;
		int TempOldImageTime = vTimeOfImage;
		CString TempOldProductName = vProductName;

		vTimeOfImage--;  
		FindPreviousImage();
		DisplayImage();
		ReDrawScreen();

		if ((TempOldImageDate == vDateOfImage) && (TempOldImageTime == vTimeOfImage))
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Previous Image";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}
		else
			ReDrawScreen();

		if	( TempOldImageDate != vDateOfImage )
		{
			if (TempOldProductName.Compare(vProductName) != 0)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nDate and Product Changed";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nDate Changed";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();
			}
		}
		else
		if (TempOldProductName.Compare(vProductName) != 0)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nProduct Changed";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Previous Image";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 2000;
		TempNoticeDialog.DoModal();
	}
	}
	else
	{
		FunctionNotAvailable();
	}
}

void CDisplaySavedRejects::OnFunction5Button() 
{
	if ((vRejectImage == NULL) && (!NextImageExists() && !PreviousImageExists()))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	if (vFoundDateFolders == false)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nFile not in proper date-folder format\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	// Next Image
	if (NextImageExists())
	{
		int TempOldImageDate = vDateOfImage;
		int TempOldImageTime = vTimeOfImage;
		CString TempOldProductName = vProductName;

		vTimeOfImage++;
		FindNextImage();
		DisplayImage();
		
		if ((TempOldImageDate == vDateOfImage) && (TempOldImageTime == vTimeOfImage))
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Next Image";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}
		else
			ReDrawScreen();

		if	( TempOldImageDate != vDateOfImage )
		{
			if (TempOldProductName.Compare(vProductName) != 0)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nDate and Product Changed";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nDate Changed";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();
			}
		}
		else
		if (TempOldProductName.Compare(vProductName) != 0)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nProduct Changed";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Next Image";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 2000;
		TempNoticeDialog.DoModal();
	}
}

void CDisplaySavedRejects::OnSubFunction5Button() 
{
	if ((vRejectImage == NULL) && (!NextImageExists() && !PreviousImageExists()))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	if (vFoundDateFolders == false)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nFile not in proper date-folder format\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	// Previous Day
	if (PreviousImageExists())
	{
		int TempOldImageDate = vDateOfImage;
		int TempOldImageTime = vTimeOfImage;
		CString TempOldProductName = vProductName;
		
		vDateOfImage--;
		vTimeOfImage = 99999999;
		FindPreviousImage();
		DisplayImage();
		
		if (TempOldImageDate == vDateOfImage)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Image in Previous Days";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
			
			if ( TempOldImageTime != vTimeOfImage )
				ReDrawScreen();
		}
		else
			ReDrawScreen();

		if (TempOldProductName.Compare(vProductName) != 0)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nProduct Changed";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Previous Image";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 2000;
		TempNoticeDialog.DoModal();
		return;
	}
}

void CDisplaySavedRejects::OnSubFunction6Button() 
{
	if ((vRejectImage == NULL) && (!NextImageExists() && !PreviousImageExists()))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	if (vFoundDateFolders == false)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nFile not in proper date-folder format\nUse Select File";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
		return;
	}

	if (NextImageExists())
	{
		// Next Day
		int TempOldImageDate = vDateOfImage;
		int TempOldImageTime = vTimeOfImage;
		CString TempOldProductName = vProductName;

		vDateOfImage++;
		vTimeOfImage = 0;
		FindNextImage();
		DisplayImage();

		if( TempOldImageDate == vDateOfImage )
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Image in Next Days";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
			if (TempOldImageTime != vTimeOfImage)
				ReDrawScreen();
			}
		else
			ReDrawScreen();

		if (TempOldProductName.Compare(vProductName) != 0)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nProduct Changed";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Next Image";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 2000;
		TempNoticeDialog.DoModal();
	}
}

void CDisplaySavedRejects::OnSubFunction7Button() 
{

}

void CDisplaySavedRejects::OnSubFunction8Button() 
{
	// Show Hide Color [Box]
	CString TempText = "";
	if (vImageHasBox)
	{
		if (vShowBox)
		{
			vShowBox = false;
			TempText="Show Defects In Color";
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction8Button,TempText);
			
			ReDrawScreen();
		}
		else
		{
			vShowBox = true;
			m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			TempText="Hide Color";
			SetDlgItemText(IDC_SubFunction8Button,TempText);
			
			ReDrawScreen();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Box Available for this Reject";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
	
}

void CDisplaySavedRejects::FindPreviousImage() //finds image closest older image to target, if no previous finds closest next
{
	CString TempTargetDateString = vImageDateString;
	CString TempTargetTimeString = vImageTimeString;

	int TempTargetTime = vTimeOfImage;
	int TempTargetDate = vDateOfImage;
	
	int TempClosestPreviousDate = 0;
	int TempClosestNextDate = 99999999;
	int TempClosestPreviousImageTime = 0;
	int TempClosestNextImageTime = 99999999;
	CString TempClosestPreviousImageString = "";
	CString TempClosestPreviousImageDate = "";
	CString TempClosestNextImageString = "";
	CString TempCurrentImageFileName = "";
	CString TempCurrentImageFolderName = "";
	CString TempDateString = "";
	int TempCurrentFileDate = 0;
	int TempCurrentFileTime = 0;
	int TempDashNumber = 0;

	bool TempDoneSearching = false;
	vFoundDateFolders = false;
	bool TempFoundAFile = false;
	bool TempHaveNextFile = false;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	// TempString(vScanTracRejectedImagesDirectory);

	CString TempStringTrial = vScanTracRejectedImagesDirectory + "*.*";
	
//	int TempLength = TempStringTrial.GetLength();
//	TempStringTrial.Insert(TempLength, '*');
//	TempStringTrial.Insert(TempLength, '.');
//	TempStringTrial.Insert(TempLength, '*');
	
	LPTSTR TempDirectoryName = TempStringTrial.GetBuffer(TempStringTrial.GetLength());
	CString TempFolder = vScanTracRejectedImagesDirectory;

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	bool TempLookingForOldestFolder = true;
	BYTE TempFilesDeleted = 0;

	CString TempPreviousImageFolderName = "";
	while (!TempDoneSearching)
	{
		while (true) //while looking for a folder closest to the date
		{
			//find the closest folder
			if (FindNextFile(hFind,&FindFileData))
			{
				CString TempFileNameString = TempFolder + FindFileData.cFileName;
				
				CString TempName = FindFileData.cFileName;
				TempName.Remove('-');
				int TempCurrentFolderDate = _wtoi(TempName);


				if (TempFileNameString.GetAt(TempFileNameString.GetLength() -1) != '.')
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					vFoundDateFolders = true;
					if (TempCurrentFolderDate == TempTargetDate)
					{
						TempCurrentImageFolderName = FindFileData.cFileName;
						TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
						CString TempFileName1 = TempFolder + "*.BMP";
						FindClose(hFind);
						hFind = FindFirstFile(TempFileName1, &FindFileData);
						if (hFind != INVALID_HANDLE_VALUE)
							TempHaveNextFile = true;
						else
							TempHaveNextFile = false;
						break;
					}
					else
					if (TempCurrentFolderDate < TempTargetDate)
					{
						if (TempCurrentFolderDate > TempClosestPreviousDate)
						{
							TempClosestPreviousDate = TempCurrentFolderDate;
						}
					}
					else
					if (TempCurrentFolderDate > TempTargetDate)
					{
						if (TempCurrentFolderDate < TempClosestNextDate)
						{
							TempClosestNextDate = TempCurrentFolderDate;
						}
					}
				}
			}
			else
			{	//no matching folders, look at previous, then next, folders
				if (TempClosestPreviousDate != 0)
				{
					TempCurrentImageFolderName = dtoa( TempClosestPreviousDate);
					TempCurrentImageFolderName.Insert(4,'-');
					TempCurrentImageFolderName.Insert(7,'-');
					TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
					CString TempFileName1 = TempFolder + "*.BMP";
					FindClose(hFind);
					hFind = FindFirstFile(TempFileName1, &FindFileData);
					if (hFind != INVALID_HANDLE_VALUE)
						TempHaveNextFile = true;
					else
						TempHaveNextFile = false;
					TempTargetTime = 99999999; //want the last image of the previous day

					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						TempDoneSearching = true;
					}

					TempPreviousImageFolderName = TempCurrentImageFolderName;
					break;
				}
				else if (TempClosestNextDate != 99999999 )
				{
					TempCurrentImageFolderName = dtoa( TempClosestNextDate);
					TempCurrentImageFolderName.Insert(4,'-');
					TempCurrentImageFolderName.Insert(7,'-');
					TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
					CString TempFileName1 = TempFolder + "*.BMP";
					FindClose(hFind);
					hFind = FindFirstFile(TempFileName1, &FindFileData);
					if (hFind != INVALID_HANDLE_VALUE)
						TempHaveNextFile = true;
					else
						TempHaveNextFile = false;
					TempTargetTime = 0;		//want the first image of the next day

					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						TempDoneSearching = true;
					}

					TempPreviousImageFolderName = TempCurrentImageFolderName;
					break;
				}
				else
				{
					//no files found in this folder  if same as previous then abort
					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						TempDoneSearching = true;
						break;
					}
				}
			}
			TempPreviousImageFolderName = TempCurrentImageFolderName;
		}
		if (!TempDoneSearching)
		while (true)	//search through files in folder found above
		{
			if (TempHaveNextFile)
			{
				TempCurrentImageFileName = TempFolder + FindFileData.cFileName;

				//extract time from file string
				CString TempString = FindFileData.cFileName;
				
				if (TempCurrentImageFileName.GetAt(TempCurrentImageFileName.GetLength() -1) == '.')
				{
					if (FindNextFile(hFind,&FindFileData))
						TempHaveNextFile = true;
					else
						TempHaveNextFile = false;
					continue;
				}
				int TempProductNameLength = TempString.Find('-');

				//set TempDashNumber -the digit smaller than seconds in file name
				int TempDashEndIndex = TempString.ReverseFind('-');
				if (TempProductNameLength+20 < TempDashEndIndex)
					TempDashNumber = _wtoi(TempString.Mid(TempProductNameLength+21, TempDashEndIndex - (TempProductNameLength+21)));
				
				//set TempCurrentFileTime from file name
				if ((TempProductNameLength) && (TempString.GetLength() < (20+TempProductNameLength) ) )
				{
					if (FindNextFile(hFind,&FindFileData))
						TempHaveNextFile = true;
					else
						TempHaveNextFile = false;
					continue;
				}
				
				TempString = TempString.Mid(TempProductNameLength+12, 8);//12 is the length of the date string inside the file name, 8 is the length of the time string
				TempString.Remove('-');
				TempCurrentFileTime = _wtoi(TempString);
				TempCurrentFileTime = TempCurrentFileTime*100 + TempDashNumber;

				if (hFind == INVALID_HANDLE_VALUE)
				{
					//did not find any files in this subfolder, so delete the folder and go up to the original folder level and look again
					RemoveDirectory(TempFolder);
					
					//reinitialize folder search
					TempFolder = vScanTracRejectedImagesDirectory;
					FindClose(hFind);
					hFind = FindFirstFile(TempDirectoryName, &FindFileData);
					
				}
				else
				if ( TempCurrentFileTime == TempTargetTime )
				{
					vCurrentImageFileName = TempCurrentImageFileName;
					vTimeOfImage = TempCurrentFileTime;
					vImageTimeString = dtoa(TempCurrentFileTime);
					vImageDateString = TempCurrentImageFolderName;
					TempCurrentImageFolderName.Remove('-');
					vDateOfImage = _wtoi(TempCurrentImageFolderName);
					TempDoneSearching = true;
					TempFoundAFile = true;
					break;
				}
				else
				if ( TempCurrentFileTime > TempTargetTime )
				{
					if (TempCurrentFileTime < TempClosestNextImageTime )
					{
						TempClosestNextImageTime = TempCurrentFileTime;
						TempClosestNextImageString = TempCurrentImageFileName;
					}
				}
				else
				if ( TempCurrentFileTime < TempTargetTime )
				{
					if (TempCurrentFileTime > TempClosestPreviousImageTime )
					{
						TempClosestPreviousImageTime = TempCurrentFileTime;
						TempClosestPreviousImageString = TempCurrentImageFileName;
					}
				}
				if (FindNextFile(hFind,&FindFileData))
					TempHaveNextFile = true;
				else
					TempHaveNextFile = false;
			}
			else	//no more files in the folder
			{
				if (TempClosestPreviousImageTime > 0)
				{	//save file name
					vCurrentImageFileName = TempClosestPreviousImageString;
					//time
					vTimeOfImage = TempClosestPreviousImageTime;
					vImageTimeString = dtoa(vTimeOfImage);
					if (vImageTimeString.GetLength() == 5)
						vImageTimeString.Insert(0,'0');
					vImageTimeString.Insert(2,'-');
					vImageTimeString.Insert(5,'-');
					//date
					vImageDateString = TempCurrentImageFolderName;
					TempCurrentImageFolderName.Remove('-');
					vDateOfImage = _wtoi(TempCurrentImageFolderName);
					TempFoundAFile = true;
					TempDoneSearching = true;
					break;
				}
				else
				if (TempClosestPreviousDate == 0)
				{
					if (TempClosestNextImageTime < 99999999)
					{
						vCurrentImageFileName = TempClosestNextImageString;
						
						vTimeOfImage = TempClosestNextImageTime;
						vImageTimeString = dtoa(vTimeOfImage);
							if (vImageTimeString.GetLength() == 5)
								vImageTimeString.Insert(0,'0');
							vImageTimeString.Insert(2,'-');
							vImageTimeString.Insert(5,'-');
						
						vImageDateString = TempCurrentImageFolderName;
						TempCurrentImageFolderName.Remove('-');
						vDateOfImage = _wtoi(TempCurrentImageFolderName);
						TempFoundAFile = true;
						TempDoneSearching = true;
						break;
					}
					else
						break;
				}
				else	//if there was a previous date, search for it
				{
					TempCurrentImageFolderName.Remove('-');
					TempTargetDate = _wtoi(TempCurrentImageFolderName) - 1;	//search for a folder before the folder you just looked at
					TempTargetTime = 99999999;
					TempClosestPreviousDate = 0;
					TempClosestPreviousImageTime = 0;
					TempClosestNextImageTime = 99999999;
					FindClose(hFind);
					hFind = FindFirstFile(TempDirectoryName, &FindFileData);	//setup folder search
				}
			}
		}
	}

	if (vFoundDateFolders == false)	//if found no folders in dirrectory, look in that directory for files
	{
		TempDoneSearching = false;
		TempFolder = vScanTracRejectedImagesDirectory + "\\";
		CString TempFileName1 = TempFolder + "*.BMP";
		FindClose(hFind);
		hFind = FindFirstFile(TempFileName1, &FindFileData);
		TempClosestPreviousDate = 0;
		TempClosestPreviousImageTime = 0;
		TempClosestNextImageTime = 99999999;

		do	//search through files in folder found above
		{
			TempCurrentImageFileName = TempFolder + FindFileData.cFileName;

			//extract time and date from file string
			CString TempString = FindFileData.cFileName;
			
			if (TempCurrentImageFileName.GetAt(TempCurrentImageFileName.GetLength() -1) == '.')
				continue;
			int TempProductNameLength = TempString.Find('-');

			if ((TempProductNameLength) && (TempString.GetLength() < (20+TempProductNameLength) ) )
				continue;
			//date
			TempDateString = TempString.Mid(TempProductNameLength+1,10);
			TempDateString.Remove('-');
			TempCurrentFileDate = _wtoi(TempDateString);
			//time
			TempString = TempString.Mid(TempProductNameLength+12, 8);//12 is the length of the date string inside the file name, 8 is the length of the time string
			TempString.Remove('-');
			TempCurrentFileTime = _wtoi(TempString);

			if (hFind == INVALID_HANDLE_VALUE)
			{
				//did not find any files in this subfolder, so delete the folder and go up to the original folder level and look again
				RemoveDirectory(TempFolder);
				
				//reinitialize folder search
				TempFolder = vScanTracRejectedImagesDirectory;
				FindClose(hFind);
				hFind = FindFirstFile(TempDirectoryName, &FindFileData);
				
			}
			else
			if (TempCurrentFileDate == TempTargetDate)
			{
				if (TempCurrentFileTime == TempTargetTime)//same date same time
				{
					//file name
					vCurrentImageFileName = TempCurrentImageFileName;
					//time
					vTimeOfImage = TempCurrentFileTime;
					vImageTimeString = dtoa(TempCurrentFileTime);
					vImageTimeString.Insert(2,':');
					vImageTimeString.Insert(5,':');
					//date
					vImageDateString  = TempDateString;	
					vImageDateString.Insert(4,'-');
					vImageDateString.Insert(7,'-');
					vDateOfImage = TempCurrentFileDate;
					TempDoneSearching = true;
					break;
				}
				else	//first file with same date
				if (TempCurrentFileDate > TempClosestPreviousDate)
				{
					TempClosestPreviousDate = TempCurrentFileDate;
					TempClosestPreviousImageTime = TempCurrentFileTime;
					TempClosestPreviousImageString = TempCurrentImageFileName;
				}
				else	//same date and closer time than the last file
				if (TempCurrentFileTime < TempTargetTime)
				{
					if (TempCurrentFileTime > TempClosestPreviousImageTime)
					{
						TempClosestPreviousDate = TempCurrentFileDate;
						TempClosestPreviousImageTime = TempCurrentFileTime;
						TempClosestPreviousImageString = TempCurrentImageFileName;
					}
				}
			}
			else	//before target date
			if (TempCurrentFileDate < TempTargetDate)
			{
				if (TempCurrentFileDate > TempClosestPreviousDate)	//first file with that date
				{
					TempClosestPreviousDate = TempCurrentFileDate;
					TempClosestPreviousImageTime = TempCurrentFileTime;
					TempClosestPreviousImageString = TempCurrentImageFileName;
				}
				if (TempCurrentFileTime < TempTargetTime)	//closer date and closer time
				{
					if (TempCurrentFileTime > TempClosestPreviousImageTime)
					{
						TempClosestPreviousDate = TempCurrentFileDate;
						TempClosestPreviousImageTime = TempCurrentFileTime;
						TempClosestPreviousImageString = TempCurrentImageFileName;
					}
				}
			}
		}
		while (FindNextFile(hFind,&FindFileData));
			
		if (TempDoneSearching == false)		
		{
			if (TempClosestPreviousImageTime > 0)
			{	//save file name
				vCurrentImageFileName = TempClosestPreviousImageString;
				//time
				vTimeOfImage = TempClosestPreviousImageTime;
				vImageTimeString = dtoa(vTimeOfImage);
				if (vImageTimeString.GetLength() == 5)
					vImageTimeString.Insert(0,'0');
				vImageTimeString.Insert(2,'-');
				vImageTimeString.Insert(5,'-');
				//date
				vImageDateString = dtoa(TempClosestPreviousDate);
				vImageDateString.Insert(4, '-');
				vImageDateString.Insert(7, '-');
				vDateOfImage = TempClosestPreviousDate;
			}
			else
			if (TempClosestPreviousDate == 0)
			{
				if (TempClosestNextImageTime < 99999999)
				{
					vCurrentImageFileName = TempClosestNextImageString;
					
					vTimeOfImage = TempClosestNextImageTime;
					vImageTimeString = dtoa(vTimeOfImage);
						if (vImageTimeString.GetLength() == 5)
							vImageTimeString.Insert(0,'0');
						vImageTimeString.Insert(2,'-');
						vImageTimeString.Insert(5,'-');
					
					TempDateString = vImageDateString;
					TempDateString.Remove('-');
					vDateOfImage = _wtoi(TempDateString);
				}
				
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nNo Previous Image";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.vAutoClose = 2000;
					TempNoticeDialog.DoModal();
				}
			}
		}
		int TempLengthOfDirectory = vScanTracRejectedImagesDirectory.GetLength();
		vProductName = vCurrentImageFileName.Right(vCurrentImageFileName.GetLength() - (TempLengthOfDirectory +1));
	}
	else
	{	
		if (!TempFoundAFile)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Previous Image";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}

		int TempLengthOfDirectory = vScanTracRejectedImagesDirectory.GetLength() + 11;
		if (TempLengthOfDirectory >= 0)
			vProductName = vCurrentImageFileName.Right(vCurrentImageFileName.GetLength() - TempLengthOfDirectory);
	}

	FindClose(hFind);

	int TempDateStartIndex = vProductName.Find('-');
	if ((TempDateStartIndex) && (TempDateStartIndex < vProductName.GetLength()))
		vProductName = vProductName.Left(TempDateStartIndex);
}

void CDisplaySavedRejects::FindNextImage()
{
	CString TempTargetDateString = vImageDateString;
	CString TempTargetTimeString = vImageTimeString;

	int TempTargetTime = vTimeOfImage;
	int TempTargetDate = vDateOfImage;
	
	int TempClosestPreviousDate = 0;
	int TempClosestNextDate = 99999999;
	int TempClosestPreviousImageTime = 0;
	int TempClosestNextImageTime = 99999999;
	CString TempClosestPreviousImageString = "";
	CString TempClosestNextImageString = "";
	CString TempCurrentImageFileName = "";
	CString TempCurrentImageFolderName = "";
	int TempCurrentFileDate = 0;
	int TempCurrentFileTime =0;

	bool TempDoneSearching = false;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString TempString(vScanTracRejectedImagesDirectory);
	TempString = TempString + "*.*";
	LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());
	CString TempFolder = vScanTracRejectedImagesDirectory;

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	CString TempPreviousImageFolderName = "";
	bool TempLookingForOldestFolder = true;
	BYTE TempFilesDeleted = 0;
	bool TempFoundNextFile = false;

	while (!TempDoneSearching)
	{
		while (true) //while looking for a folder closest to the date
		{		
			//find the closest folder
			if (FindNextFile(hFind,&FindFileData))
			{
				CString TempFileNameString = TempFolder + FindFileData.cFileName;
				
				CString TempString = FindFileData.cFileName;
				TempString.Remove('-');
				int TempCurrentFolderDate = _wtoi(TempString);


				if (TempFileNameString.GetAt(TempFileNameString.GetLength() -1) != '.')
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (TempCurrentFolderDate == TempTargetDate)
					{
						TempCurrentImageFolderName = FindFileData.cFileName;
						TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
						CString TempFileName1 = TempFolder + "*.BMP";

						if (FindNextFile(hFind,&FindFileData))	//is there a folder after this one?
						{
							CString TempString = FindFileData.cFileName;
							TempString.Remove('-');
							TempClosestNextDate = _wtoi(TempString);
						}
						FindClose(hFind);
						hFind = FindFirstFile(TempFileName1, &FindFileData);
						if (hFind != INVALID_HANDLE_VALUE)
							TempFoundNextFile = true;
						break;
					}
					else
					if (TempCurrentFolderDate > TempTargetDate)
					{
						if (TempCurrentFolderDate < TempClosestNextDate)
						{
							TempClosestNextDate = TempCurrentFolderDate;
						}
					}
					else
					if (TempCurrentFolderDate < TempTargetDate)
					{
						if (TempCurrentFolderDate > TempClosestPreviousDate)
						{
							TempClosestPreviousDate = TempCurrentFolderDate;
						}
					}
				}
			}
			else
			{	//no matching folders, look at previous, then next, folders
				if (TempClosestNextDate != 99999999 )
				{
					TempCurrentImageFolderName = dtoa( TempClosestNextDate);
					TempCurrentImageFolderName.Insert(4,'-');
					TempCurrentImageFolderName.Insert(7,'-');
					TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
					CString TempFileName1 = TempFolder + "*.BMP";
					FindClose(hFind);
					hFind = FindFirstFile(TempFileName1, &FindFileData);
					if (hFind != INVALID_HANDLE_VALUE)
						TempFoundNextFile = true;
					TempTargetTime = 0;		//want the first image of the next day

					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						TempDoneSearching = true;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nNo Next Image";
						TempNoticeDialog.vType = cInformationMessage;
						TempNoticeDialog.vAutoClose = 2000;
						TempNoticeDialog.DoModal();
					}

					TempPreviousImageFolderName = TempCurrentImageFolderName;
					break;
				}
				else 
				if (TempClosestPreviousDate != 0)
				{
					TempCurrentImageFolderName = dtoa( TempClosestPreviousDate);
					TempCurrentImageFolderName.Insert(4,'-');
					TempCurrentImageFolderName.Insert(7,'-');
					TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
					CString TempFileName1 = TempFolder + "*.BMP";
					FindClose(hFind);
					hFind = FindFirstFile(TempFileName1, &FindFileData);
					if (hFind != INVALID_HANDLE_VALUE)
						TempFoundNextFile = true;
					TempTargetTime = 99999999; //want the last image of the previous day

					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						TempDoneSearching = true;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nNo Next Image";
						TempNoticeDialog.vType = cInformationMessage;
						TempNoticeDialog.vAutoClose = 2000;
						TempNoticeDialog.DoModal();
					}

					TempPreviousImageFolderName = TempCurrentImageFolderName;
					break;
				}
				else
				{
					//no files found in this folder  if same as previous then abort
					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						TempDoneSearching = true;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nNo Next Image";
						TempNoticeDialog.vType = cInformationMessage;
						TempNoticeDialog.vAutoClose = 2000;
						TempNoticeDialog.DoModal();
						break;
					}
				}
			}
			TempPreviousImageFolderName = TempCurrentImageFolderName;
		}

		//found folder, now find file in folder
		while (true)
		{
			if (TempFoundNextFile)
			{
				int TempDashNumber = 0;
				TempCurrentImageFileName = TempFolder + FindFileData.cFileName;
				TempFoundNextFile = false;

				//extract time from file string
				CString TempString = FindFileData.cFileName;
				
				if (TempCurrentImageFileName.GetAt(TempCurrentImageFileName.GetLength() -1) == '.')
					continue;
				int TempProductNameLength = TempString.Find('-');

				//find dashed number
				int TempDashEndIndex = TempString.ReverseFind('-');
				if (TempProductNameLength+20 < TempDashEndIndex)
					TempDashNumber = _wtoi(TempString.Mid(TempProductNameLength+21, TempDashEndIndex - (TempProductNameLength+21)));

				//find time
				if (TempString.GetLength() < (20 + TempProductNameLength))
					continue;
				TempString = TempString.Mid(TempProductNameLength+12, 8);//12 is the length of the date string inside the file name, 8 is the length of the time string
				TempString.Remove('-');
				TempCurrentFileTime = _wtoi(TempString);
				TempCurrentFileTime = TempCurrentFileTime*100 + TempDashNumber;

				if (hFind == INVALID_HANDLE_VALUE)
				{
					//did not find any files in this subfolder, so delete the folder and go up to the original folder level and look again
					RemoveDirectory(TempFolder);
					
					//reinitialize folder search
					TempFolder = vScanTracRejectedImagesDirectory;
					FindClose(hFind);
					hFind = FindFirstFile(TempDirectoryName, &FindFileData);
					
				}
				else
				{	//found file, set file name
					if ( TempCurrentFileTime == TempTargetTime )
					{
						vCurrentImageFileName = TempCurrentImageFileName;
						
						vTimeOfImage = TempCurrentFileTime;
						vImageTimeString = dtoa(TempCurrentFileTime);
						
						vImageDateString = TempCurrentImageFolderName;
						TempCurrentImageFolderName.Remove('-');
						vDateOfImage = _wtoi(TempCurrentImageFolderName);
						
						TempDoneSearching = true;
						break;
					}
					else
					if ( TempCurrentFileTime > TempTargetTime )
					{
						if (TempCurrentFileTime < TempClosestNextImageTime )
						{
							TempClosestNextImageTime = TempCurrentFileTime;
							TempClosestNextImageString = TempCurrentImageFileName;
						}
					}
					else
					if ( TempCurrentFileTime < TempTargetTime )
					{
						if (TempCurrentFileTime > TempClosestPreviousImageTime )
						{
							TempClosestPreviousImageTime = TempCurrentFileTime;
							TempClosestPreviousImageString = TempCurrentImageFileName;
						}
					}
				}
				if (FindNextFile(hFind,&FindFileData))
					TempFoundNextFile = true;
			}
			else	//no more files in the folder
			{
				if (TempClosestNextImageTime < 99999999)
				{
					vCurrentImageFileName = TempClosestNextImageString;
					
					vTimeOfImage = TempClosestNextImageTime;
					vImageTimeString = dtoa(vTimeOfImage);
						if (vImageTimeString.GetLength() == 5)
							vImageTimeString.Insert(0, '0');
						vImageTimeString.Insert(2,'-');
						vImageTimeString.Insert(5,'-');
					
					vImageDateString = TempCurrentImageFolderName;
					TempCurrentImageFolderName.Remove('-');
					vDateOfImage = _wtoi(TempCurrentImageFolderName);

					TempDoneSearching = true;
					break;
				}
				else 
				if (TempClosestNextDate == 99999999 )	//if there is no next date
				{
					if (TempClosestPreviousImageTime > 0)	//search for previous
					{
						vCurrentImageFileName = TempClosestPreviousImageString;
						
						vTimeOfImage = TempClosestPreviousImageTime;
						vImageTimeString = dtoa(vTimeOfImage);
							if(vImageTimeString.GetLength() == 5)
								vImageTimeString.Insert(0, '0');
							vImageTimeString.Insert(2,'-');
							vImageTimeString.Insert(5,'-');
						
						vImageDateString = TempCurrentImageFolderName;
						TempCurrentImageFolderName.Remove('-');
						vDateOfImage = _wtoi(TempCurrentImageFolderName);

						TempDoneSearching = true;
						break;
					}
					else
						break; //error
				}
				else	//if is a next date, search for the next date
				{
					TempCurrentImageFolderName.Remove('-');
					TempTargetDate = _wtoi(TempCurrentImageFolderName) + 1;
					TempTargetTime = 0;
					TempClosestNextDate = 99999999;
					TempClosestPreviousImageTime = 0;
					TempClosestNextImageTime = 99999999;
					FindClose(hFind);
					hFind = FindFirstFile(TempDirectoryName, &FindFileData);
					break;
				}

			}
		}
	}
	FindClose(hFind);

	//set product name, it is different if path has date folder in it
	if (vFoundDateFolders == false)
	{
		int TempLengthOfDirectory = vScanTracRejectedImagesDirectory.GetLength();
		vProductName = vCurrentImageFileName.Right(vCurrentImageFileName.GetLength() - (TempLengthOfDirectory +1));
	}
	else
	{
		int TempLengthOfDirectory = vScanTracRejectedImagesDirectory.GetLength() + 11;
		if (TempLengthOfDirectory >= 0)
			vProductName = vCurrentImageFileName.Right(vCurrentImageFileName.GetLength() - TempLengthOfDirectory);
	}
	int TempDateStartIndex = vProductName.Find('-');
	if ((TempDateStartIndex) && (TempDateStartIndex < vProductName.GetLength()))
		vProductName = vProductName.Left(TempDateStartIndex);
}

int CDisplaySavedRejects::PreviousImageExists() //finds image closest older image to target, if no previous finds closest next
{
	CString TempTargetDateString = vImageDateString;
	CString TempTargetTimeString = vImageTimeString;

	int TempTargetTime = vTimeOfImage - 1;
	int TempTargetDate = vDateOfImage;
	
	int TempClosestPreviousDate = 0;
	int TempClosestNextDate = 99999999;
	int TempClosestPreviousImageTime = 0;
	int TempClosestNextImageTime = 99999999;
	CString TempClosestPreviousImageString = "";
	CString TempClosestNextImageString = "";
	CString TempCurrentImageFileName = "";
	CString TempCurrentImageFolderName = "";
	int TempCurrentFileDate = 0;
	int TempCurrentFileTime =0;

	bool TempDoneSearching = false;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	// TempString(vScanTracRejectedImagesDirectory);

	CString TempStringTrial = vScanTracRejectedImagesDirectory + "*.*";
	
//	int TempLength = TempStringTrial.GetLength();
//	TempStringTrial.Insert(TempLength, '*');
//	TempStringTrial.Insert(TempLength, '.');
//	TempStringTrial.Insert(TempLength, '*');
	
	LPTSTR TempDirectoryName = TempStringTrial.GetBuffer(TempStringTrial.GetLength());
	CString TempFolder = vScanTracRejectedImagesDirectory;

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	bool TempLookingForOldestFolder = true;
	BYTE TempFilesDeleted = 0;

	CString TempPreviousImageFolderName = "";
	while (!TempDoneSearching)
	{
		while (true) //while looking for a folder closest to the date
		{
			//find the closest folder
			if (FindNextFile(hFind,&FindFileData))
			{
				CString TempFileNameString = TempFolder + FindFileData.cFileName;
				
				CString TempName = FindFileData.cFileName;
				TempName.Remove('-');
				int TempCurrentFolderDate = _wtoi(TempName);


				if (TempFileNameString.GetAt(TempFileNameString.GetLength() -1) != '.')
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (TempCurrentFolderDate == TempTargetDate)
					{
						TempCurrentImageFolderName = FindFileData.cFileName;
						TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
						CString TempFileName1 = TempFolder + "*.BMP";
						FindClose(hFind);
						hFind = FindFirstFile(TempFileName1, &FindFileData);
						break;
					}
					else
					if (TempCurrentFolderDate < TempTargetDate)
					{
						if (TempCurrentFolderDate > TempClosestPreviousDate)
						{
							TempClosestPreviousDate = TempCurrentFolderDate;
						}
					}
					else
					if (TempCurrentFolderDate > TempTargetDate)
					{
						if (TempCurrentFolderDate < TempClosestNextDate)
						{
							TempClosestNextDate = TempCurrentFolderDate;
						}
					}
				}
			}
			else
			{	//no matching folders, look at previous, then next, folders
				if (TempClosestPreviousDate != 0)
				{
					TempCurrentImageFolderName = dtoa( TempClosestPreviousDate);
					TempCurrentImageFolderName.Insert(4,'-');
					TempCurrentImageFolderName.Insert(7,'-');
					TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
					CString TempFileName1 = TempFolder + "*.BMP";
					FindClose(hFind);
					hFind = FindFirstFile(TempFileName1, &FindFileData);
					TempTargetTime = 99999999; //want the last image of the previous day

					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						FindClose(hFind);
						return false;
					}

					TempPreviousImageFolderName = TempCurrentImageFolderName;
					break;
				}
				else if (TempClosestNextDate != 99999999 )
				{
					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{	
						FindClose(hFind);
						return false;
					}

					TempCurrentImageFolderName = dtoa( TempClosestNextDate);
					TempCurrentImageFolderName.Insert(4,'-');
					TempCurrentImageFolderName.Insert(7,'-');
					TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
					CString TempFileName1 = TempFolder + "*.BMP";
					FindClose(hFind);
					hFind = FindFirstFile(TempFileName1, &FindFileData);
					TempTargetTime = 0;		//want the first image of the next day

					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{	
						FindClose(hFind);
						return false;
					}

					TempPreviousImageFolderName = TempCurrentImageFolderName;
					break;
				}
				else
				{
					//no files found in this folder  if same as previous then abort
					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						FindClose(hFind);
						return false;
					}
				}
			}
			TempPreviousImageFolderName = TempCurrentImageFolderName;
		}
		if (!TempDoneSearching)
		do	//search through files in folder found above
		{
			int TempDashNumber = 0;
			TempCurrentImageFileName = TempFolder + FindFileData.cFileName;

			//extract time from file string
			CString TempString = FindFileData.cFileName;
			
			if (TempCurrentImageFileName.GetAt(TempCurrentImageFileName.GetLength() -1) == '.')
				continue;
			int TempProductNameLength = TempString.Find('-');

			//find dashed number
			int TempDashEndIndex = TempString.ReverseFind('-');
			if (TempProductNameLength+20 < TempDashEndIndex)
				TempDashNumber = _wtoi(TempString.Mid(TempProductNameLength+21, TempDashEndIndex - (TempProductNameLength+21)));
			//set time
			if ((TempProductNameLength) && (TempString.GetLength() < (20+TempProductNameLength) ) )
				continue;
			TempString = TempString.Mid(TempProductNameLength+12, 8);//12 is the length of the date string inside the file name, 8 is the length of the time string
			TempString.Remove('-');
			TempCurrentFileTime = _wtoi(TempString);
			TempCurrentFileTime = TempCurrentFileTime * 100 + TempDashNumber;

			if (hFind == INVALID_HANDLE_VALUE)
			{
				//did not find any files in this subfolder, so delete the folder and go up to the original folder level and look again
				RemoveDirectory(TempFolder);
				
				//reinitialize folder search
				TempFolder = vScanTracRejectedImagesDirectory;
				FindClose(hFind);
				hFind = FindFirstFile(TempDirectoryName, &FindFileData);
				
			}
			else
			if ( TempCurrentFileTime == TempTargetTime )
			{
				FindClose(hFind);
				return true;
			}
			else
			if ( TempCurrentFileTime > TempTargetTime )
			{
				if (TempCurrentFileTime < TempClosestNextImageTime )
				{
					TempClosestNextImageTime = TempCurrentFileTime;
					TempClosestNextImageString = TempCurrentImageFileName;
				}
			}
			else
			if ( TempCurrentFileTime < TempTargetTime )
			{
				if (TempCurrentFileTime > TempClosestPreviousImageTime )
				{
					TempClosestPreviousImageTime = TempCurrentFileTime;
					TempClosestPreviousImageString = TempCurrentImageFileName;
				}
			}
		}
		while (FindNextFile(hFind,&FindFileData));
			
		if (TempClosestPreviousImageTime > 0)
		{	//save file name
			FindClose(hFind);
			return true;
		}
		else
		if (TempClosestPreviousDate == 0)
		{
			if (TempClosestNextImageTime < 99999999)
			{
				FindClose(hFind);
				return false;
			}
			else
				break;
		}
		else	//if there was a previous date, search for it
		{
			TempCurrentImageFolderName.Remove('-');
			TempTargetDate = _wtoi(TempCurrentImageFolderName) - 1;	//search for a folder before the folder you just looked at
			TempTargetTime = 99999999;
			TempClosestPreviousDate = 0;
			TempClosestPreviousImageTime = 0;
			TempClosestNextImageTime = 99999999;
			FindClose(hFind);
			hFind = FindFirstFile(TempDirectoryName, &FindFileData);	//setup folder search
			//break;	//break out of file search
		}
	}
	FindClose(hFind);
	return true;

}

int CDisplaySavedRejects::NextImageExists()
{
	CString TempTargetDateString = vImageDateString;
	CString TempTargetTimeString = vImageTimeString;

	int TempTargetTime = vTimeOfImage + 1;
	int TempTargetDate = vDateOfImage;
	
	int TempClosestPreviousDate = 0;
	int TempClosestNextDate = 99999999;
	int TempClosestPreviousImageTime = 0;
	int TempClosestNextImageTime = 99999999;
	CString TempClosestPreviousImageString = "";
	CString TempClosestNextImageString = "";
	CString TempCurrentImageFileName = "";
	CString TempCurrentImageFolderName = "";
	int TempCurrentFileDate = 0;
	int TempCurrentFileTime =0;

	bool TempDoneSearching = false;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString TempString(vScanTracRejectedImagesDirectory);
	TempString = TempString + "*.*";
	LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());
	CString TempFolder = vScanTracRejectedImagesDirectory;

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	CString TempPreviousImageFolderName = "";
	bool TempLookingForOldestFolder = true;
	BYTE TempFilesDeleted = 0;

	while (!TempDoneSearching)
	{
		while (true) //while looking for a folder closest to the date
		{		
			//find the closest folder
			if (FindNextFile(hFind,&FindFileData))
			{
				CString TempFileNameString = TempFolder + FindFileData.cFileName;
				
				CString TempString = FindFileData.cFileName;
				TempString.Remove('-');
				int TempCurrentFolderDate = _wtoi(TempString);


				if (TempFileNameString.GetAt(TempFileNameString.GetLength() -1) != '.')
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (TempCurrentFolderDate == TempTargetDate)
					{
						TempCurrentImageFolderName = FindFileData.cFileName;
						TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
						CString TempFileName1 = TempFolder + "*.BMP";

						if (FindNextFile(hFind,&FindFileData))	//is there a folder after this one?
						{
							CString TempString = FindFileData.cFileName;
							TempString.Remove('-');
							TempClosestNextDate = _wtoi(TempString);
						}
						FindClose(hFind);
						hFind = FindFirstFile(TempFileName1, &FindFileData);
						break;
					}
					else
					if (TempCurrentFolderDate > TempTargetDate)
					{
						if (TempCurrentFolderDate < TempClosestNextDate)
						{
							TempClosestNextDate = TempCurrentFolderDate;
						}
					}
					else
					if (TempCurrentFolderDate < TempTargetDate)
					{
						if (TempCurrentFolderDate > TempClosestPreviousDate)
						{
							TempClosestPreviousDate = TempCurrentFolderDate;
						}
					}
				}
			}
			else
			{	//no matching folders, look at previous, then next, folders
				if (TempClosestNextDate != 99999999 )
				{
					TempCurrentImageFolderName = dtoa( TempClosestNextDate);
					TempCurrentImageFolderName.Insert(4,'-');
					TempCurrentImageFolderName.Insert(7,'-');
					TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
					CString TempFileName1 = TempFolder + "*.BMP";
					FindClose(hFind);
					hFind = FindFirstFile(TempFileName1, &FindFileData);
					TempTargetTime = 0;		//want the first image of the next day

					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						FindClose(hFind);
						return false;
					}

					TempPreviousImageFolderName = TempCurrentImageFolderName;
					break;
				}
				else 
				if (TempClosestPreviousDate != 0)
				{
					TempCurrentImageFolderName = dtoa( TempClosestPreviousDate);
					TempCurrentImageFolderName.Insert(4,'-');
					TempCurrentImageFolderName.Insert(7,'-');
					TempFolder = vScanTracRejectedImagesDirectory + TempCurrentImageFolderName + "\\";
					CString TempFileName1 = TempFolder + "*.BMP";
					FindClose(hFind);
					hFind = FindFirstFile(TempFileName1, &FindFileData);
					TempTargetTime = 99999999; //want the last image of the previous day

					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						FindClose(hFind);
						return false;
					}

					TempPreviousImageFolderName = TempCurrentImageFolderName;
					break;
				}
				else
				{
					//no files found in this folder  if same as previous then abort
					if (TempPreviousImageFolderName == TempCurrentImageFolderName)
					{
						FindClose(hFind);
						return false;
					}
				}
			}
			TempPreviousImageFolderName = TempCurrentImageFolderName;
//			FindClose(hFind);
//			return false;
		}

		//found folder, now find file in folder
		while (true)
		{
			if (FindNextFile(hFind,&FindFileData))
			{
				int TempDashNumber = 0;
				TempCurrentImageFileName = TempFolder + FindFileData.cFileName;

				//extract time from file string
				CString TempString = FindFileData.cFileName;
				
				if (TempCurrentImageFileName.GetAt(TempCurrentImageFileName.GetLength() -1) == '.')
					continue;
				int TempProductNameLength = TempString.Find('-');

				//find dashed number
				int TempDashEndIndex = TempString.ReverseFind('-');
				if (TempProductNameLength+20 < TempDashEndIndex)
					TempDashNumber = _wtoi(TempString.Mid(TempProductNameLength+21, TempDashEndIndex - (TempProductNameLength+21)));
				//set file time -from file name
				if (TempString.GetLength() < (20+TempProductNameLength) )
					continue;
				TempString = TempString.Mid(TempProductNameLength+12, 8);//12 is the length of the date string inside the file name, 8 is the length of the time string
				TempString.Remove('-');
				TempCurrentFileTime = _wtoi(TempString);
				TempCurrentFileTime = TempCurrentFileTime * 100 + TempDashNumber;

				if (hFind == INVALID_HANDLE_VALUE)
				{
					//did not find any files in this subfolder, so delete the folder and go up to the original folder level and look again
					RemoveDirectory(TempFolder);
					
					//reinitialize folder search
					TempFolder = vScanTracRejectedImagesDirectory;
					FindClose(hFind);
					hFind = FindFirstFile(TempDirectoryName, &FindFileData);
					
				}
				else
				{	//found file, set file name
					if ( TempCurrentFileTime == TempTargetTime )
					{
						
						FindClose(hFind);
						return true;
					}
					else
					if ( TempCurrentFileTime > TempTargetTime )
					{
						if (TempCurrentFileTime < TempClosestNextImageTime )
						{
							TempClosestNextImageTime = TempCurrentFileTime;
							TempClosestNextImageString = TempCurrentImageFileName;
						}
					}
					else
					if ( TempCurrentFileTime < TempTargetTime )
					{
						if (TempCurrentFileTime > TempClosestPreviousImageTime )
						{
							TempClosestPreviousImageTime = TempCurrentFileTime;
							TempClosestPreviousImageString = TempCurrentImageFileName;
						}
					}
				}
			}
			else	//no more files in the folder
			{
				if (TempClosestNextImageTime < 99999999)
				{
					
					FindClose(hFind);
					return true;
				}
				else 
				if (TempClosestNextDate == 99999999 )	//if there is no next date
				{
					if (TempClosestPreviousImageTime > 0)	//search for previous
					{
					
						FindClose(hFind);
						return false;
					}
					else
						break; //error
				}
				else	//if is a next date, search for the next date
				{
					TempCurrentImageFolderName.Remove('-');
					TempTargetDate = _wtoi(TempCurrentImageFolderName) + 1;
					TempTargetTime = 0;
					TempClosestNextDate = 99999999;
					TempClosestPreviousImageTime = 0;
					TempClosestNextImageTime = 99999999;
					FindClose(hFind);
					hFind = FindFirstFile(TempDirectoryName, &FindFileData);
					break;
				}
			}
		}
	}
	FindClose(hFind);
	return true;
}

void CDisplaySavedRejects::DisplayImage()
{
	if (vRejectImage)
	{
		free(vRejectImage);
		vRejectImage = NULL;
	}

	int TempBytesPerPixel = 0;
	int TempType = 0;

	LPTSTR TempString = vCurrentImageFileName.GetBuffer(vCurrentImageFileName.GetLength());

	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	{
		CW2A TempFileNameString(TempString);
		Erflag TempError = im_file_get_size(TempFileNameString, IM_BMP_FILE, &vRejectImageX, &vRejectImageY, &TempBytesPerPixel, &TempType);
		if (TempError == IM_OK)
		{
			if ((vRejectImageX < 10) || (vRejectImageY < 10))
			{
#ifdef InScanTrac
				if (vLocalSystemData != NULL)
					ReportErrorMessage("Error-Saved Reject Image Very Small, ignored", cWriteToLog,32000);
#endif
			}
			else
			if ((vRejectImageX > 10000) || (vRejectImageY > 10000))
			{
#ifdef InScanTrac
				if (vLocalSystemData != NULL)
					ReportErrorMessage("Error-Saved Reject Image Very Large, ignored", cWriteToLog,32000);
#endif
			}
			else
#ifdef InScanTrac
			if (ThereIsEnoughMemory(sizeof(BYTE) * vRejectImageX * vRejectImageY, "Temp Image In"))
#endif
			{
				vRejectImage = (BYTE *)malloc(vRejectImageX * vRejectImageY);
				
				if (vRejectImage)
				{
					CFile TempImageFile;
					CFileException TempFileException;
					
					Image *TempImageIn = NULL;
					TempImageIn = im_create(IM_BYTE, vRejectImageX, vRejectImageY);
					if ((TempImageIn) && (TempImageIn->pix) && (vRejectImage))
					{
						CW2A TempFileNameString(TempString);
						TempError = im_file_read(TempImageIn, TempFileNameString, IM_AUTO_FILE);
						if (TempError == IM_OK)
						{
							CopyMemory(vRejectImage, TempImageIn->pix, vRejectImageX * vRejectImageY);
						}
					}
					if (TempImageIn)
						im_delete(TempImageIn);
					
					int TempLength = vCurrentImageFileName.GetLength() - (vCurrentImageFileName.ReverseFind('\\') + 1);	//11 is the length of the subfolder that has the date on it
					CString TempImageFileName;
					if (vCurrentImageFileName.GetLength() > TempLength)
						TempImageFileName = vCurrentImageFileName.Right(TempLength);

					//file name
					TempImageFileName = TempImageFileName.Left(TempImageFileName.GetLength() - 4); //four cuts off .BMP from end of the file name
					TempImageFileName = "File: " + TempImageFileName;
					SetDlgItemText(IDC_FileNameLine, TempImageFileName);
		
					//date
					CString TempString1 = "";

					if (vImageDateString.GetLength() > 9)
						TempString1 = vImageDateString.Mid(5,2) + "/" + vImageDateString.Right(2) + "/" + vImageDateString.Left(4);
					else if (vImageDateString.GetLength() > 7)
						TempString1 = vImageDateString.Mid(4,2) + "/" + vImageDateString.Right(2) + "/" + vImageDateString.Left(4);	//bug fix

					TempString1 = "Date:\n" + TempString1;
					SetDlgItemText(IDC_DateDisplay, TempString1);

					//Product Name
	//				WORD TempStartDatePosition = TempImageFileName.Find('-');
	//				if ( (TempStartDatePosition > 6) && (TempImageFileName.GetLength() > 8) )
	//					TempString1 = TempImageFileName.Mid(6,TempStartDatePosition - 6);
					TempString1 = "Product:\n" + vProductName;
					SetDlgItemText(IDC_ProductNameDisplay, TempString1);

					//inspection name and ejectors disabled
					WORD TempStartOfInspection = TempImageFileName.ReverseFind('-') + 1;
					int TempStringLength = TempImageFileName.GetLength() - TempImageFileName.ReverseFind('-') - 1;
					if (TempImageFileName.GetLength() > TempStringLength)
						TempString1 = TempImageFileName.Right(TempStringLength);
					if (TempString1.Compare(_T("Disabled")) == 0)
					{
						m_EjectorDisabledDisplay.ShowWindow(SW_SHOW);
						TempImageFileName.Delete(TempImageFileName.GetLength() - 9,9); //remove 'disabled' from name
						TempStartOfInspection = TempImageFileName.ReverseFind('-') + 1;
					}
					else
						m_EjectorDisabledDisplay.ShowWindow(SW_HIDE);

					
					if(TempImageFileName.GetLength() > TempStartOfInspection)
						TempString1 = TempImageFileName.Right(TempImageFileName.GetLength() - TempStartOfInspection);

					TempString1 = "Inspection:\n" + TempString1;
					SetDlgItemText(IDC_InspectionNameDisplay, TempString1);
					
					//remove inspeciton name and dash before
					TempStartOfInspection--;
					if (TempStartOfInspection < TempImageFileName.GetLength())
						TempImageFileName.Delete(TempStartOfInspection, TempImageFileName.GetLength() - TempStartOfInspection);  

					WORD TempDashPosition = TempImageFileName.ReverseFind('-') + 1;
					WORD TempDashLength = TempImageFileName.GetLength() - TempDashPosition + 1;
					CString TempDashNumber = " ";
					if (TempImageFileName.GetLength() - TempDashPosition > 0) //non negative value
						TempDashNumber = TempImageFileName.Right(TempImageFileName.GetLength() - TempDashPosition);

					//delete the dash number
					if ((2 - TempDashLength) >= 0)
						TempImageFileName.Delete(TempImageFileName.GetLength() - 2, TempDashLength);

					TempString1 = "";

					if (TempImageFileName.GetLength() > 12)
						vImageTimeString = TempImageFileName.Right(11);

					if (vImageTimeString.GetLength() > 7)
						vImageTimeString = vImageTimeString.Left(2) + ":" + vImageTimeString.Mid(3,2) + ":" + vImageTimeString.Mid(6,2) + "." + 
						TempDashNumber;  //the frame number in the second

					TempString1 = "Time:\n" + vImageTimeString;
					SetDlgItemText(IDC_TimeDisplay, TempString1);

					vImageHasBox = false;
					
					for (int i = 0; i < vRejectImageX * vRejectImageY;)
					{
						if (vRejectImage[i] != vRejectImage[i + vRejectImageX/2])
						{
							vImageHasBox = true;
							break;
						}

						i++;
						if ((i % (vRejectImageX/2)) == 0)
							i = i + vRejectImageX/2;
					}
				}
#ifdef InScanTrac
				else
				if (vLocalSystemData != NULL)
					ReportErrorMessage("Error-Not enough memory to display Reject Image", cWriteToLog,32000);
#endif
			}
#ifdef InScanTrac
			else
			{
				if (vLocalSystemData != NULL)
					ReportErrorMessage("Error-Not enough memory to display Reject Image", cWriteToLog,32000);
				CString TempImageFileName = "Memory unavailable to display:" + TempImageFileName;
				if (vRejectImage)
				{
					free(vRejectImage);
					vRejectImage = NULL;
				}
				vRejectImageX = 1;
				vRejectImageY = 1;
				//vRejectImage = (BYTE *)malloc(1);
				//vRejectImage[0] = 196;	//set the pixel to light gray
				
				SetDlgItemText(IDC_FileNameLine, TempImageFileName);

				vImageHasBox = false;
			}
#endif
		}
		else
		{
#ifdef InScanTrac
			if (vLocalSystemData != NULL)
				ReportErrorMessage("Error-Corrupt Reject Image", cWriteToLog,32000);
#endif

			CString TempImageFileName = vCurrentImageFileName;
			int TempLength = vCurrentImageFileName.GetLength() - vScanTracRejectedImagesDirectory.GetLength() - 11;	//11 is the length of the subfolder that has the date on it
			if (vCurrentImageFileName.GetLength() > TempLength)
				TempImageFileName = vCurrentImageFileName.Right(TempLength);

			TempImageFileName = TempImageFileName.Left(TempImageFileName.GetLength() - 4); //four cuts off .BMP from end of the file name
			TempImageFileName = "Corrupt File: " + TempImageFileName;
			SetDlgItemText(IDC_FileNameLine, TempImageFileName);
			if (vRejectImage)
			{
				free(vRejectImage);
				vRejectImage = NULL;
			}
			vRejectImageX = 1;
			vRejectImageY = 1;
			//vRejectImage = (BYTE *)malloc(1);
			//vRejectImage[0] = 196;	//set the pixel to light gray

			vImageHasBox = false;

			//date
			CString TempString1 = "";

			if (vImageDateString.GetLength() > 9)
				TempString1 = vImageDateString.Mid(5,2) + "/" + vImageDateString.Right(2) + "/" + vImageDateString.Left(4);
			else if (vImageDateString.GetLength() > 7)
				TempString1 = vImageDateString.Mid(4,2) + "/" + vImageDateString.Right(2) + "/" + vImageDateString.Left(4);	//bug fix

			TempString1 = "Date:\n" + TempString1;
			SetDlgItemText(IDC_DateDisplay, TempString1);

			//Product Name
//				WORD TempStartDatePosition = TempImageFileName.Find('-');
//				if ( (TempStartDatePosition > 6) && (TempImageFileName.GetLength() > 8) )
//					TempString1 = TempImageFileName.Mid(6,TempStartDatePosition - 6);
			TempString1 = "Product:\n" + vProductName;
			SetDlgItemText(IDC_ProductNameDisplay, TempString1);

			//inspection name and ejectors disabled
			WORD TempStartOfInspection = TempImageFileName.ReverseFind('-') + 1;
			int TempStringLength = TempImageFileName.GetLength() - TempImageFileName.ReverseFind('-') - 1;
			if (TempImageFileName.GetLength() > TempStringLength)
				TempString1 = TempImageFileName.Right(TempStringLength);
			if (TempString1.Compare(_T("Disabled")) == 0)
			{
				m_EjectorDisabledDisplay.ShowWindow(SW_SHOW);
				TempImageFileName.Delete(TempImageFileName.GetLength() - 9,9); //remove 'disabled' from name
				TempStartOfInspection = TempImageFileName.ReverseFind('-') + 1;
			}
			else
				m_EjectorDisabledDisplay.ShowWindow(SW_HIDE);
			
			if(TempImageFileName.GetLength() > TempStartOfInspection)
				TempString1 = TempImageFileName.Right(TempImageFileName.GetLength() - TempStartOfInspection);

			TempString1 = "Inspection:\n" + TempString1;
			SetDlgItemText(IDC_InspectionNameDisplay, TempString1);
			
			//remove inspeciton name and dash before
			TempStartOfInspection--;
			if (TempStartOfInspection < TempImageFileName.GetLength())
				TempImageFileName.Delete(TempStartOfInspection, TempImageFileName.GetLength() - TempStartOfInspection);  

			WORD TempDashPosition = TempImageFileName.ReverseFind('-') + 1;
			WORD TempDashLength = TempImageFileName.GetLength() - TempDashPosition + 1;
			CString TempDashNumber = " ";
			if (TempImageFileName.GetLength() - TempDashPosition > 0) //non negative value
				TempDashNumber = TempImageFileName.Right(TempImageFileName.GetLength() - TempDashPosition);

			//delete the dash number
			if ((2 - TempDashLength) >= 0)
				TempImageFileName.Delete(TempImageFileName.GetLength() - 2, TempDashLength);

			TempString1 = "";

			if (TempImageFileName.GetLength() > 12)
				vImageTimeString = TempImageFileName.Right(11);

			if (vImageTimeString.GetLength() > 7)
				vImageTimeString = vImageTimeString.Left(2) + ":" + vImageTimeString.Mid(3,2) + ":" + vImageTimeString.Mid(6,2) + "." + 
				TempDashNumber;  //the frame number in the second

			TempString1 = "Time:\n" + vImageTimeString;
			SetDlgItemText(IDC_TimeDisplay, TempString1);
		}
	}


	if (vImageHasBox)
		vSubFunction8ButtonEnable = true;	//m_SubFunction8Button.EnableWindow(true);
	else
		vSubFunction8ButtonEnable = false;	//m_SubFunction8Button.EnableWindow(false);

	if (PreviousImageExists())
	{
		vFunction4ButtonEnable = true;	//m_Function4Button.EnableWindow(true);
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		vFunction3ButtonEnable = true;	//m_Function3Button.EnableWindow(true);

		if (NextImageExists())
		{
			vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
			vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		}
		else
		{
			vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
			vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		}
	}
	else
	{
		vFunction4ButtonEnable = false;	//m_Function4Button.EnableWindow(false);
		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		if (NextImageExists())
		{
			vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
			vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
			vFunction3ButtonEnable = true;	//m_Function3Button.EnableWindow(true);
		}
		else
		{
			vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
			vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
			vFunction3ButtonEnable = false;	//m_Function3Button.EnableWindow(false);
		}
	}
}

void CDisplaySavedRejects::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (vRejectImage)
	{
		free(vRejectImage);
		vRejectImage = NULL;
	}
	// TODO: Add your message handler code here
	
}

void CDisplaySavedRejects::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	CTime TempTime = CTime::GetCurrentTime();
	vImageTimeString = TempTime.Format("%H-%M-%S.00");
	vImageDateString = TempTime.Format("%Y-%m-%d");
	vDateOfImage = _wtoi(TempTime.Format("%Y%m%d"));
	vTimeOfImage = _wtoi(TempTime.Format("%H%M%S00"));
	vCurrentImageFileName = "";

	vFunction3ButtonEnable = true;
	vFunction4ButtonEnable = true;
	vFunction5ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction8ButtonEnable = true;

#ifdef InScanTrac
	if (vLocalConfigurationData)
	{
// kjh change 9/2012		vScanTracRejectedImagesDirectory = vLocalConfigurationData->vScanTracRejectedImagesDirectory;
		if ((vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetAt(0) != 'C') && (vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetAt(0) != 'D') &&
			(vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetAt(0) != 'c') && (vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetAt(0) != 'd'))
		{
			if  (CheckDiskSpaceFree("D:\\") == 0)
			{
				vScanTracRejectedImagesDirectory = cRejectedImagesDirectoryCDrive; 
			}
			else
			{
				vScanTracRejectedImagesDirectory = cRejectedImagesDirectoryDDrive;
			}
		}
		else
		{
			vScanTracRejectedImagesDirectory = vLocalConfigurationData->vScanTracRejectedImagesDirectory;
		}		
	}
#endif
	

	//call functions, do things
#ifdef InScanTrac
	if (vLocalConfigurationData)
	{
		FindPreviousImage(); //finds image previous to current time
							//saves image file name to vCurrentImageFileName
		DisplayImage();		//puts image in byte buffer vRejectImage, ready to paint
	}
	else //if (registryKey exists
#endif
	{
		CString TempRegistryValue;
		CRegKey TempRegistryKey;
		unsigned long int TempCount = 500;
		bool TempNoRegKey = true;

		LONG TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("Software"), KEY_ALL_ACCESS);

		if (TempErrorCode == ERROR_SUCCESS)
		{
			TempErrorCode = TempRegistryKey.Create(HKEY_LOCAL_MACHINE, _T("Software\\Inspx\\ScanTrac RejectDisplay"));

			if (TempErrorCode == ERROR_SUCCESS)
			{
				TempRegistryValue.Empty();
				LPCTSTR TempBuffer = _T("");
				TempErrorCode = TempRegistryKey.QueryStringValue(TempBuffer, _T("LastDirectoryName"), &TempCount);
				if (TempErrorCode == ERROR_SUCCESS)
				{
					vScanTracRejectedImagesDirectory = "";
					if (TempCount > 500)
						TempCount = 500;
					for (unsigned int i = 0; i < TempCount; i++)
					{
						wchar_t a = TempBuffer[i];
						if (a == 0)
							break;
						vScanTracRejectedImagesDirectory = vScanTracRejectedImagesDirectory + (CString)a;
					}
						
					//vRegistryReadString = "D:\\ScanTracRejectedImages\\";
/*
				TempRegistryKeyString = "Previous Server " + dtoa(TempLoop + 1,0);
				TempCount = 50;
				TempRegistryValue.Empty();
				TempErrorCode = TempRegistryKey.QueryValue(
					TempRegistryValue.GetBuffer(50),TempRegistryKeyString,&TempCount);
				if (TempErrorCode == ERROR_SUCCESS)
*/					//CString TempTest = vScanTracRejectedImagesDirectory;
								
					if (PreviousImageExists())
					{
						FindPreviousImage(); //finds image previous to current time
											//saves image file name to vCurrentImageFileName
						DisplayImage();		//puts image in byte buffer vRejectImage, ready to paint

						ReDrawScreen();

						TempNoRegKey = false;
					}
				}
			}
			TempRegistryKey.Close();
		}

		if (TempNoRegKey == true)
		{		//grey out navigation buttons
			m_Function2Button.ShowWindow(SW_SHOW);
			vSubFunction8ButtonEnable = false;	//m_SubFunction8Button.EnableWindow(false);
			vFunction3ButtonEnable = false;	//m_Function3Button.EnableWindow(false);
			vFunction4ButtonEnable = false;	//m_Function4Button.EnableWindow(false);
			vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
			vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
			vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		}
		;//vScanTracRejectedImagesDirectory;
		m_Function2Button.ShowWindow(SW_SHOW);
	}	

#ifdef InScanTrac
	int TempTimerResult = SetTimer(vDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);

	int TimerResult = SetTimer(vGrabFocusTimerHandle,800,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-GrabFocus Timer Failed",cEMailInspx, 32000);
	if (vLocalSystemData)
	{
		if (vLocalConfigurationData->vSaveAllImagesToRejectsFolder)
		{
			this->SetWindowText(_T("Saved Images"));
			SetDlgItemText(IDC_DialogTitleStaticText1, _T("Saved Images"));
		}
		else
			this->SetWindowText(_T("Saved Rejects"));
	}
	else
#endif
		this->SetWindowText(_T("Reject Viewer"));

	if (PreviousImageExists())
	{
		vFunction4ButtonEnable = true;	//m_Function4Button.EnableWindow(true);
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		vFunction3ButtonEnable = true;	//m_Function3Button.EnableWindow(true);

		if (NextImageExists())
		{
			vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
			vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		}
		else
		{
			vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
			vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		}
	}
	else
	{
		vFunction4ButtonEnable = false;	//m_Function4Button.EnableWindow(false);
		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		if (NextImageExists())
		{
			vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
			vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
			vFunction3ButtonEnable = true;	//m_Function3Button.EnableWindow(true);
		}
		else
		{
			vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
			vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
			vFunction3ButtonEnable = false;	//m_Function3Button.EnableWindow(false);
		}
	}
	::SetupMenu(vLocalCWndCollection);
}

HBRUSH CDisplaySavedRejects::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	int TempControlID = pWnd->GetDlgCtrlID();

	if (TempControlID == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		if (vSubFunction8ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		pDC->SetBkMode(TRANSPARENT);
		if (vShowBox)
#ifdef InScanTrac
			return vGlobalButtonColorBrush;
#else
			return vButtonColorBrush;
#endif
		else
			return vLocalSystemData->vGreenBrush;
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
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction1Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction2Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction3Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction4Button) ||
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
#ifdef InScanTrac
			return vGlobalButtonColorBrush;
#else
			return vButtonColorBrush;
#endif
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
#ifdef InScanTrac
			return vGlobalButtonColorBrush;
#else
			return vButtonColorBrush;
#endif
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
#ifdef InScanTrac
			return vGlobalButtonColorBrush;
#else
			return vButtonColorBrush;
#endif
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
#ifdef InScanTrac
			return vGlobalButtonColorBrush;
#else
			return vButtonColorBrush;
#endif
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
#ifdef InScanTrac
			return vGlobalButtonColorBrush;
#else
			return vButtonColorBrush;
#endif
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction6ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
#ifdef InScanTrac
			return vGlobalButtonColorBrush;
#else
			return vButtonColorBrush;
#endif
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction8ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
#ifdef InScanTrac
			return vGlobalButtonColorBrush;
#else
			return vButtonColorBrush;
#endif
	}
	if (pWnd->GetDlgCtrlID() == IDC_Clock)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Clock, 5);
	if (pWnd->GetDlgCtrlID() == IDC_FileNameLine)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FileNameLine, 5);

	if (pWnd->GetDlgCtrlID() == IDC_DateDisplay)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DateDisplay, 5);	//not button but is two lines
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_TimeDisplay)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_TimeDisplay, 5);	//not button but is two lines
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_ProductNameDisplay)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductNameDisplay, 5);	
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_InspectionNameDisplay)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_InspectionNameDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
	}

	if (TempControlID == IDC_EjectorDisabledDisplay)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_EjectorDisabledDisplay, 5);	//not button but is two lines
		pDC->SetTextColor(cButtonTextColor);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
	}

	if (TempControlID == IDC_FileNameLine)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FileNameLine, 5);
		if (!vRejectImage)
		{
			pDC->SetTextColor(cButtonTextColor);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
#ifdef InScanTrac
		pDC->SetTextColor(vGlobalDialogTitleColor);
	//	if (PasswordOK(cTemporaryInspxPassword,false))
	//		pDC->SetTextColor(cBrilliantBlue);
	//	else
#endif
			pDC->SetTextColor(cDarkBlue);
	}
	
	return hbr;
}

BOOL CDisplaySavedRejects::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
	
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
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
			case 118: // F7 Key - Sub Function 1:
				OnFunction1Button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnSubFunction8Button();
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
	//		case c1:
	//			if (PasswordOK(cTemporaryInspxPassword,false))
	//				this->Invalidate(true);
	//		break;
#ifdef InScanTrac
			case 36:  //Home key, Screen Capture			
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
					PostThreadMessage(vGlobalCallingThreadID,cShowHelp, 0, 0);
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
			break;
			case 0x60:  //0 key on KeyPad
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
#endif
		}
		return true;  //true indicates it has been handled, so do not process
	}
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void CDisplaySavedRejects::OnPaint()	//image starts from top left corner
{
	CPaintDC dc(this); // device context for painting
	
	int TempIncrementYLoop = 1;
	int TempIncrementXLoop = 1;
	WORD TempOffsetX = (WORD) ResizeXCoor(cWindowOffsetX)+10;
	WORD TempOffsetY = (WORD) ResizeYCoor(cWindowOffsetY)+10;
	int TempWindowWidthX = ResizeXCoor(cWindowX);
	int TempWindowHeightY = ResizeYCoor(cWindowY);

	if (vRejectImage != NULL)
	{
		BYTE *TempDataPoint = vRejectImage;
		BYTE *TempFirstDataPoint = vRejectImage;
		BYTE *TempFirstDataPointInRow = 0;
		int TempRejectImageX = vRejectImageX;
		int TempRejectImageY = vRejectImageY;
		double TempScaleX = (TempRejectImageX / 2) / double(TempWindowWidthX);  //width
		double TempScaleY = TempRejectImageY / double(TempWindowHeightY);  //height
		double TempTrackXDouble = 0;
		double TempTrackYDouble = 0;
		int TempLastPixel = vRejectImageX*vRejectImageY;

		CDC TempCompatableDeviceContext;
		TempCompatableDeviceContext.CreateCompatibleDC(&dc);
		HBITMAP TempBitmapOfReject = CreateCompatibleBitmap ( dc, vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY );
		BITMAPINFO TempBitmapInfo = {0};		//http://stackoverflow.com/questions/17137100/c-changing-the-values-in-a-bitmap
		TempBitmapInfo.bmiHeader.biSize = sizeof(TempBitmapInfo.bmiHeader);
		int TempBitmapColorArraySize = vLocalSystemData->vITIPCDig->vDisplayImageSizeX * vLocalSystemData->vITIPCDig->vDisplayImageSizeY * 4;
		int TempIterator = TempBitmapColorArraySize;

		if(0 == GetDIBits(dc, TempBitmapOfReject, 0, 0, NULL, &TempBitmapInfo, DIB_RGB_COLORS))  //get just bitmap info header
		{
			int r = 0;
		}
		TempBitmapInfo.bmiHeader.biBitCount = 32;  
		TempBitmapInfo.bmiHeader.biCompression = BI_RGB;  

		BYTE* TempBitmapColorArray = new BYTE[TempBitmapColorArraySize];
		GetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);

		TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
		
		if (!vShowBox) //show right half of the image
		{
			for (WORD TempLoopY = 0; TempLoopY < TempWindowHeightY; TempLoopY++)
			{
			//	TempEndOfRowPoint = TempDataPoint + TempRejectImageX;
				TempFirstDataPointInRow = TempDataPoint + TempRejectImageX / 2; //scroll to halfway across image
				TempTrackXDouble = 0;
				for (WORD TempLoopX = 0; TempLoopX < TempWindowWidthX; TempLoopX++)
				{
//				if ((cWindowOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
//					(cWindowOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
//					(cWindowOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
//					(cWindowOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
//				{
						if (*TempDataPoint == 0)
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, cRed); //draw pixel in intensity
						}
						else
						{
							TempBitmapColorArray[TempIterator] = *TempDataPoint;//blUE
							TempBitmapColorArray[TempIterator + 1] = *TempDataPoint;//green
							TempBitmapColorArray[TempIterator + 2] = *TempDataPoint;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, 
								//RGB(*TempDataPoint,*TempDataPoint,*TempDataPoint)); //draw pixel in intensity
						}
						
						TempTrackXDouble = TempTrackXDouble + TempScaleX;
						TempDataPoint = TempFirstDataPointInRow + int(TempTrackXDouble);
						TempIterator = TempIterator + 4;
//					}
				}
				
				//TempDataPoint = TempEndOfRowPoint;	//Goto next row start
				TempTrackYDouble = TempTrackYDouble + TempScaleY;
				TempDataPoint = TempFirstDataPoint + int(TempTrackYDouble) * TempRejectImageX;
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;  //multiply by 8, 4 points per pixel, go back two rows and forward one
			}
			
		}
		else //!vShowBox... hide box show left half of image
		{
			for (WORD TempLoopY = 0; TempLoopY < TempWindowHeightY; TempLoopY++)
			{
				TempFirstDataPointInRow = TempDataPoint; //scroll to halfway across image
				TempTrackXDouble = 0;
				for (WORD TempLoopX = 0; TempLoopX < TempWindowWidthX; TempLoopX++)
				{
				//	if ((cWindowOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
				//		(cWindowOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
				//		(cWindowOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
				//		(cWindowOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
				//	{
						if (*TempDataPoint == 0)
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, cRed); //draw pixel in Red
						}
						else
						{
							TempBitmapColorArray[TempIterator] = *TempDataPoint;//blUE
							TempBitmapColorArray[TempIterator + 1] = *TempDataPoint;//green
							TempBitmapColorArray[TempIterator + 2] = *TempDataPoint;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, RGB(*TempDataPoint,*TempDataPoint,*TempDataPoint)); //draw pixel in intensity
						}
						TempTrackXDouble = TempTrackXDouble + TempScaleX;
						TempDataPoint = TempFirstDataPointInRow + int(TempTrackXDouble);
						TempIterator = TempIterator + 4;
				//	}
				}
				TempTrackYDouble = TempTrackYDouble + TempScaleY;
				TempDataPoint = TempFirstDataPoint + int(TempTrackYDouble) * TempRejectImageX;
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;  //multiply by 8, 4 points per pixel, go back two rows and forward one
			}
		}

		SetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);
		free(TempBitmapColorArray);

		TempCompatableDeviceContext.SelectObject(TempBitmapOfReject);
		dc.BitBlt(TempOffsetX, TempOffsetY, TempBitmapInfo.bmiHeader.biWidth, TempBitmapInfo.bmiHeader.biHeight, &TempCompatableDeviceContext, 0, 0, SRCCOPY);
		
		DeleteObject(TempCompatableDeviceContext);
		DeleteObject(TempBitmapOfReject);
	}
	else
	{ //no reject file
		for (WORD TempLoopY = 0; TempLoopY < TempWindowHeightY; TempLoopY++)
		{
			for (WORD TempLoopX = 0; TempLoopX < TempWindowWidthX; TempLoopX++)
			{
				dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, RGB(193,193,193)); //draw pixel in Gray
			}
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}

void CDisplaySavedRejects::ReDrawScreen()
{
	RECT TempRectangle;		//refresh screen
	//TempRectangle.left = cWindowOffsetX;
	//TempRectangle.top = cWindowOffsetY;
	//TempRectangle.right = cWindowOffsetX + cWindowX;
	//TempRectangle.bottom = cWindowOffsetY + cWindowY;
	TempRectangle.left = 0;
	TempRectangle.top = 0;
	TempRectangle.right = ResizeXCoor(639);
	TempRectangle.bottom = ResizeYCoor(479);
	this->InvalidateRect(&TempRectangle, false);	

	int TempTimerResult = SetTimer(vGrabFocusAgainTimerHandle,50,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	
	//SetForegroundWindow();
	//this->SetFocus();
}

#ifdef InScanTrac
void CDisplaySavedRejects::UpdateDisplay()
{
	CTime TempTime;
	TempTime = CTime::GetCurrentTime();
	CString TempTimeString;
	if ((vLocalConfigurationData) && (vLocalConfigurationData->vShow24HourTime))
		TempTimeString = TempTime.Format("%H:%M:%S");//24 hour time format
	else
		TempTimeString = TempTime.Format("%#I:%M:%S%p");//12 hour time format with am pm

	SetDlgItemText(IDC_Clock,TempTimeString);
}

void CDisplaySavedRejects::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vDisplayTimerHandle)
	{
		UpdateDisplay();
	}
	else
  if (nIDEvent == vGrabFocusTimerHandle)  
  {
		KillTimer(vGrabFocusTimerHandle);
 		this->ShowWindow(SW_SHOW);
		this->SetForegroundWindow();
		this->BringWindowToTop();
		this->SetFocus();
		this->SetActiveWindow();
	}
	else
  if (nIDEvent == vGrabFocusAgainTimerHandle)  
  {
		KillTimer(vGrabFocusAgainTimerHandle);
 		this->ShowWindow(SW_SHOW);
		this->SetForegroundWindow();
		this->BringWindowToTop();
		this->SetFocus();
		this->SetActiveWindow();
	}
	
	CDialog::OnTimer(nIDEvent);
}
#endif

void CDisplaySavedRejects::FunctionNotAvailable(void)
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Function Not Currently Available");
	TempNoticeDialog.vNoticeText = "\n\n\nFunction Not Currently Available";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}



/*
CString CDisplaySavedRejects::GetProductName(CString TempFileName)
{
	CString TempProductName = "";

	int TempLength = TempFileName.GetLength();
	if (TempLength > 20)
	{
		int TempPosition = TempFileName.Find("-", 0);
		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempProductName = TempFileName.Left(TempPosition);
	}
	return TempProductName;
}

//26oz. Ball Jar 8 11 08-2009-04-01#07-55-11-1-Glass in Body-Disabled.BMP
bool CDisplaySavedRejects::WasRejectDisabled(CString TempFileName)
{
	bool TempRejectDisabled = false;

	int TempLength = TempFileName.GetLength();
	if (TempLength > 20)
	{
		int TempPosition = TempFileName.Find("-Disabled");
		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempRejectDisabled = true;
	}
	return TempRejectDisabled;
}

CString CDisplaySavedRejects::GetInspectionName(CString TempFileName)
{
	CString TempInspectioName = "";
	CString TempName = TempFileName;
	int TempStartOfInspection = 0;
	int TempLength = TempName.GetLength();
	if (TempLength > 20)
	{
		int TempPosition = TempName.Find("-Disabled");
		if ((TempPosition > 0) && (TempPosition < TempLength))
		{
			TempName.Delete(TempLength, TempLength - TempPosition); //remove 'disabled' and what is after it from name
			TempLength = TempName.GetLength();
		}

		TempPosition = TempName.Find(".BMP");
		if ((TempPosition > 0) && (TempPosition < TempLength))
		{
			TempName.Delete(TempLength, TempLength - TempPosition); //remove '.BMP' and what is after it from name
			TempLength = TempName.GetLength();
		}

		TempStartOfInspection = TempName.ReverseFind('-') + 1;
		if ((TempStartOfInspection > 0) && (TempStartOfInspection < TempLength))
		{
			TempInspectioName = TempName.Right(TempStartOfInspection);
		}
	}
	return TempInspectioName;
}
//26oz. Ball Jar 8 11 08-2009-04-01#07-55-11-1-Glass in Body-Disabled.BMP

CString CDisplaySavedRejects::GetDateFromFileName(CString TempFileName)
{
	CString TempDate = "";
	CString TempString = TempFileName;

	int TempLength = TempFileName.GetLength();
	if (TempLength > 20)
	{
		int TempPosition = TempFileName.Find("-", 0);
		if ((TempPosition > 0) && (TempPosition < TempLength))
		{
			TempFileName.Delete(0, TempPosition);
			TempLength = TempFileName.GetLength();

			if (TempLength > 10)
				TempDate = TempFileName.Left(9);

		}
	}
	return TempDate;
}
//26oz. Ball Jar 8 11 08-2009-04-01#07-55-11-1-Glass in Body-Disabled.BMP

CString CDisplaySavedRejects::GetTimeFromFileName(CString TempFileName)
{
	CString TempTime = "";
	CString TempString = TempFileName;

	int TempLength = TempString.GetLength();
	if (TempLength > 20)
	{
		int TempPosition = TempString.Find("-", 0);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
		{
			TempString.Delete(0, TempPosition);
			TempLength = TempString.GetLength();

			if (TempLength > 9)
				TempTime = TempString.Left(8);

		}
	}
	return TempTime;
}
//26oz. Ball Jar 8 11 08-2009-04-01#07-55-11-1-Glass in Body-Disabled.BMP

CString CDisplaySavedRejects::GetDashNumberFromFileName(CString TempFileName)
{
	CString TempDashNumber = "";
	CString TempString = TempFileName;

	int TempLength = TempString.GetLength();
	if (TempLength > 20)
	{
		int TempPosition = TempString.Find("-", 0);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
			TempPosition = TempString.Find("-", TempPosition);

		if ((TempPosition > 0) && (TempPosition < TempLength))
		{
			TempString.Delete(0, TempPosition);
			TempLength = TempString.GetLength();

			if ((TempPosition > 0) && (TempPosition < TempLength))
				TempPosition = TempString.Find("-", TempPosition);

			if ((TempPosition > 0) && (TempPosition < TempLength))
			if (TempLength > 2)
				TempDashNumber = TempString.Left(TempPosition);
		}
	}
	return TempDashNumber;
}

CString CDisplaySavedRejects::FindPreviousFolder() //finds folder closest to current, if no previous finds closest next
{
	//vCurrentDate
	CString TempFolderFound = "";

	int TempTargetDate = _wtoi(vImageDateString) - 1;
	
	int TempClosestPreviousDate = 0;
	int TempClosestNextDate = 99999999;

	bool TempDoneSearching = false;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString TempStringTrial = vScanTracRejectedImagesDirectory + "*.*";
	
	LPTSTR TempDirectoryName = TempStringTrial.GetBuffer(TempStringTrial.GetLength());
	CString TempFolder = vScanTracRejectedImagesDirectory;

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	bool TempLookingForOldestFolder = true;

	while (!TempDoneSearching)
	{
		//find the closest folder
		if (FindNextFile(hFind,&FindFileData))
		{
			CString TempFileNameString = TempFolder + FindFileData.cFileName;
			
			CString TempName = FindFileData.cFileName;
			TempName.Remove('-');
			int TempCurrentFolderDate = _wtoi(TempName);

			if (TempFileNameString.GetAt(TempFileNameString.GetLength() -1) != '.')
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (TempCurrentFolderDate == TempTargetDate)
				{
					TempFolderFound = FindFileData.cFileName;
					TempDoneSearching = true;
				}
				else
				if (TempCurrentFolderDate < TempTargetDate)
				{
					if (TempCurrentFolderDate > TempClosestPreviousDate)
					{
						TempClosestPreviousDate = TempCurrentFolderDate;
					}
				}
				else
				if (TempCurrentFolderDate > TempTargetDate)
				{
					if (TempCurrentFolderDate < TempClosestNextDate)
					{
						TempClosestNextDate = TempCurrentFolderDate;
					}
				}
			}
		}
		else
		{	//no matching folders, look at previous, then next, folders
			TempDoneSearching = true;
			if (TempClosestPreviousDate != 0)
			{
				TempFolderFound = dtoa(TempClosestPreviousDate);
				TempFolderFound.Insert(4,'-');
				TempFolderFound.Insert(7,'-');
			}
			else 
			if (TempClosestNextDate != 99999999)
			{
				TempFolderFound = dtoa(TempClosestNextDate);
				TempFolderFound.Insert(4,'-');
				TempFolderFound.Insert(7,'-');
			}
			else
				;//error - no dates found
				TempDoneSearching = true;
		}
	}
	FindClose(hFind);
	return TempFolderFound;
}

CString CDisplaySavedRejects::FindNextFolder() //finds folder closest to current, if no previous finds closest next
{
	//vCurrentDate
	CString TempFolderFound = "";

	int TempTargetDate = _wtoi(vImageDateString) + 1;
	
	int TempClosestPreviousDate = 0;
	int TempClosestNextDate = 99999999;

	bool TempDoneSearching = false;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString TempStringTrial = vScanTracRejectedImagesDirectory + "*.*";
	
	LPTSTR TempDirectoryName = TempStringTrial.GetBuffer(TempStringTrial.GetLength());
	CString TempFolder = vScanTracRejectedImagesDirectory;

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	bool TempLookingForOldestFolder = true;

	while (!TempDoneSearching)
	{
		//find the closest folder
		if (FindNextFile(hFind,&FindFileData))
		{
			CString TempFileNameString = TempFolder + FindFileData.cFileName;
			
			CString TempName = FindFileData.cFileName;
			TempName.Remove('-');
			int TempCurrentFolderDate = _wtoi(TempName);

			if (TempFileNameString.GetAt(TempFileNameString.GetLength() -1) != '.')
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (TempCurrentFolderDate == TempTargetDate)
				{
					TempFolderFound = FindFileData.cFileName;
					TempDoneSearching = true;
				}
				else
				if (TempCurrentFolderDate > TempTargetDate)
				{
					if (TempCurrentFolderDate < TempClosestNextDate)
					{
						TempClosestNextDate = TempCurrentFolderDate;
					}
				}
				else
				if (TempCurrentFolderDate < TempTargetDate)
				{
					if (TempCurrentFolderDate > TempClosestPreviousDate)
					{
						TempClosestPreviousDate = TempCurrentFolderDate;
					}
				}
			}
		}
		else
		{	//no matching folders, look at previous, then next, folders
			TempDoneSearching = true;
			if (TempClosestNextDate != 99999999)
			{
				TempFolderFound = dtoa(TempClosestNextDate);
				TempFolderFound.Insert(4,'-');
				TempFolderFound.Insert(7,'-');
			}
			else 
			if (TempClosestPreviousDate != 0)
			{
				TempFolderFound = dtoa(TempClosestPreviousDate);
				TempFolderFound.Insert(4,'-');
				TempFolderFound.Insert(7,'-');
			}
			else
				;//error - no dates found
				TempDoneSearching = true;
		}
	}
	FindClose(hFind);
	return TempFolderFound;
}
*/


	void CDisplaySavedRejects::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
