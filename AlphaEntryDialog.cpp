//ScanTrac Side View Source File
// AlphaEntryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "AlphaEntryDialog.h"
#include "NoticeDialog.h"
#include "SerialPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFont * vGlobalCurrentFonts;
extern BYTE vGlobalShiftKeyDown;
//extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern unsigned long vGlobalDialogTitleColor;
extern CScanTracSystemRunningData *vGlobalRunningData;
/////////////////////////////////////////////////////////////////////////////
// CAlphaEntryDialog dialog


CAlphaEntryDialog::CAlphaEntryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAlphaEntryDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlphaEntryDialog)
	m_DialogTitleStaticText2 = _T("");
	m_DialogTitleStaticText = _T("");
	m_TextEditingString = _T("");
	m_DialogTitleStaticText3 = _T("");
	//}}AFX_DATA_INIT
	vNumbersOnly = false;
	AfxInitRichEdit( );
	vCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE | CFM_SIZE;
	vCharacterFormat.crTextColor = cBlue;
	vCharacterFormat.dwEffects = 0;
	vCharacterFormat.yHeight = ResizeYCoor(187);

	vRedCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE | CFM_SIZE;
	vRedCharacterFormat.crTextColor = cRed;
	vRedCharacterFormat.dwEffects = 0;
	vRedCharacterFormat.yHeight = vCharacterFormat.yHeight;


	vSelectCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE | CFM_SIZE;
	vSelectCharacterFormat.crTextColor = cGreen;
	vSelectCharacterFormat.dwEffects = CFM_UNDERLINE;
	vSelectCharacterFormat.yHeight = vCharacterFormat.yHeight;
	
	vColor = NULL;

	if (ThereIsEnoughMemory(sizeof(int) * 10000, "Color array in Alpha Entry dialog"))
		vColor = (int *)malloc(sizeof(int) * 10000);
	else
	{
		ReportErrorMessage("Not Enough Memory for Color Array Alloc in Alpha Entry Dialog",cEMailInspx, 32000);
		#ifdef _DEBUG
		afxDump << "Not Enough Memory for Alpha Entry Dialog Alloc";
		#endif
	}
	if (vColor)
	for (WORD TempLoop = 0; TempLoop < 10000; TempLoop++)
		vColor[TempLoop] = 0;
	vOnScreenKeyBoardPress = false;

	vLocalSystemData = NULL;
	vFlashLightsTimerHandle = 31320;

	vFlashingLights = false;
	vNoMainMenu = false;
	vChatMode = false;
	vFindTextMode = false;
	vAcceptAllKeys = true; 
	vAllowDashes = false;
	vAllowPeriod = false;
	vAllowSlashes = false;
	vAllowApproximate = false;
	vAllowParenthesis = false;
	vAllowComma = false;
	vAllowIPComma = false;
	vBlankNameOK = false;
	vAllowMoreThan32Characters = false;
	vMinimumNumberOfCharacters = 0;
	vMaximumNumberOfCharacters = 0;

	vUpperCase = true;
	vCapLock = false;

	vShowingTouchKeyboard=false;

	for (WORD TempLoop = 0;TempLoop < cNumberOfKeys; TempLoop++)
		vLocalKey[TempLoop] = cLocalAlphaKey[TempLoop];

	vEditString = "";
	vDeleteAll = true;
	vLastKey = 0;
	vLastKeyTimes = 0;
	vCursorPosition = 0;
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vLightBlueBrush = ::CreateSolidBrush(cLightBlue);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vRedBrush = ::CreateSolidBrush(cRed);
	vInAutoSetup = false;
	vFlashAllLampStatus = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText3Control);
	vLocalCWndCollection.Add(&m_Function1Button				 );
	vLocalCWndCollection.Add(&m_SubFunction5Button			 );
	vLocalCWndCollection.Add(&m_SubFunction6Button			 );
	vLocalCWndCollection.Add(&m_DialogTitleStaticText2Control);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText1Control);
	vLocalCWndCollection.Add(&m_DialogTitleStaticTextControl );
	vLocalCWndCollection.Add(&m_SubFunction1Button			 );
	vLocalCWndCollection.Add(&m_SubFunction2Button			 );
	vLocalCWndCollection.Add(&m_SubFunction7Button			 );
	vLocalCWndCollection.Add(&m_SubFunction3Button			 );
	vLocalCWndCollection.Add(&m_SubFunction4Button			 );
	vLocalCWndCollection.Add(&m_SubFunction8Button			 );
	vLocalCWndCollection.Add(&m_Function5Button				 );
	vLocalCWndCollection.Add(&m_Function3Button				 );
	vLocalCWndCollection.Add(&m_Function2Button				 );
	vLocalCWndCollection.Add(&m_KeyHelp						 );
	vLocalCWndCollection.Add(&m_KeyClear					 );
	vLocalCWndCollection.Add(&m_Key9						 );
	vLocalCWndCollection.Add(&m_Key8						 );
	vLocalCWndCollection.Add(&m_Key7						 );
	vLocalCWndCollection.Add(&m_Key6						 );
	vLocalCWndCollection.Add(&m_Key5						 );
	vLocalCWndCollection.Add(&m_Key4						 );
	vLocalCWndCollection.Add(&m_Key3						 );
	vLocalCWndCollection.Add(&m_Key2						 );
	vLocalCWndCollection.Add(&m_Key1						 );
	vLocalCWndCollection.Add(&m_Key0						 );
	vLocalCWndCollection.Add(&m_Background					 );
	vLocalCWndCollection.Add(&m_Function4Button				 );
	//vLocalCWndCollection.Add(&m_Title						 );
	vLocalCWndCollection.Add(&m_TextEditing					 );
}

CAlphaEntryDialog::~CAlphaEntryDialog()
{
	if(vColor != NULL)
	{
		free(vColor);
		vColor = NULL;
	}
	DeleteObject(vWhiteBrush);
	DeleteObject(vLightBlueBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vGreenBrush);
	DeleteObject(vRedBrush);
}

void CAlphaEntryDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	vDeleteAll = true;
	vLastKey = 0;
	vLastKeyTimes = 0;
	
	if (vLocalSystemData)
	if (vLocalSystemData->vTouchScreen)
		vShowingTouchKeyboard = true;

	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,//106
		cMainBackgroundTop,cMainBackgroundLength, //41, 423
		cMainBackgroundHeight,SWP_NOZORDER); //367

	m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction5Button.ShowWindow(SW_SHOW);

	vDoNotClickUpperY = (WORD) ResizeYCoor(42);
	if (vChatMode)
	{
		vDeleteAll = false;
		if (vShowingTouchKeyboard)
			vDoNotClickLowerY = (WORD) ResizeYCoor(200);
		else
			vDoNotClickLowerY = (WORD) ResizeYCoor(412);
	}
	else
		vDoNotClickLowerY = (WORD) ResizeXCoor(150);

	vDoNotClickLeftX = (WORD) ResizeXCoor(105);
	vDoNotClickRightX = (WORD) ResizeXCoor(533);


	if (vAllowParenthesis)
	if (vAllowSlashes)
		vAllowParenthesis = false;

	if (((vAllowParenthesis) || (vAllowSlashes)) && (vAllowIPComma))
		vAllowIPComma = false;

	if (vAllowComma)
	if (vAllowPeriod)
	if (!vOnScreenKeyBoardPress)
		vAllowComma = false;

	if (vChatMode)
		vAllowSlashes = true;

	if (vAllowComma)
		SetDlgItemText(IDC_Function3Button, _T(","));

	if (vAllowParenthesis)
	{
		SetDlgItemText(IDC_SubFunction1Button, _T("("));
		SetDlgItemText(IDC_SubFunction2Button, _T(")"));
	}

	if (vAllowIPComma)
	{
		SetDlgItemText(IDC_SubFunction2Button, _T(","));
	}

	if (vChatMode)
	{
		SetDlgItemText(IDC_DialogTitleStaticText1, _T("Chat"));
		vAcceptAllKeys = true;
		SetDlgItemText(IDC_Function1Button, _T("Exit Chat"));
		m_TextEditing.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		vNoMainMenu = true;

		m_DialogTitleStaticTextControl.ShowWindow(SW_HIDE);
		m_DialogTitleStaticText2Control.ShowWindow(SW_HIDE);
		m_DialogTitleStaticText3Control.ShowWindow(SW_HIDE);
		if (!vShowingTouchKeyboard)
			m_TextEditing.SetWindowPos(NULL,cMainBackgroundLeft + 20, cMainBackgroundTop + 20,cMainBackgroundLength - 40, cMainBackgroundHeight - 40,SWP_NOZORDER);
		this->SetWindowText(_T("Chat"));

		if ((vLocalSystemData) && (vLocalSystemData->vLastChatWindowText.GetLength() > 2))
		{
			vEditString = vLocalSystemData->vLastChatWindowText + "\n";
			SetDlgItemText(IDC_TextEditing, vEditString);
		}
		else
			ProcessAlphaNumericKey(' ');
	}
	else
	if (vFindTextMode)
	{
		this->SetWindowText(_T("Find"));
		m_DialogTitleStaticText3Control.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_DialogTitleStaticText1, _T("Text to Find:"));
		vEditString = "Text to Find";
	}
	else
	{
		this->SetWindowText(_T("Name"));
		m_DialogTitleStaticText3Control.ShowWindow(SW_SHOW);
		CString TempText = " ";
		TempText.LoadString(IDS_EnterNameFor);
		SetDlgItemText(IDC_DialogTitleStaticText1, TempText);
	}

	//if (PasswordOK(cTemporaryInspxPassword,false))
	//	vAcceptAllKeys = true;

	if (vNoMainMenu)
	{
		m_Function1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function1Button.GetRectToInvalidateArrow());
	//	m_ArrowDown1.ShowWindow(SW_HIDE);
	}
	CString TempText("");
	if (vInAutoSetup)
	{
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
//		m_ArrowDown2.ShowWindow(SW_HIDE);
		m_Function5Button.ShowWindow(SW_SHOW);
//		m_ArrowDown5.ShowWindow(SW_SHOW);
		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempText.LoadString(IDS_NextAutoSetupStep);
		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	{
		TempText.LoadString(IDS_SaveAndExit);
		SetDlgItemText(IDC_Function2Button,TempText);
	}
	TempText.LoadString(IDS_ChangeToLowerCase);
	SetDlgItemText(IDC_SubFunction6Button,TempText);
	TempText.LoadString(IDS_MainMenu);
	if (!vChatMode)
		SetDlgItemText(IDC_Function1Button,TempText);
	TempText.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempText);

	vCursorPosition = vEditString.GetLength();
	SetCursor();
	//if ((vGlobalShowMouseCursor) && (!vChatMode))
	if (!vChatMode)
	{
		m_Key0.ShowWindow(SW_SHOW);
		m_Key1.ShowWindow(SW_SHOW);
		m_Key2.ShowWindow(SW_SHOW);
		m_Key3.ShowWindow(SW_SHOW);
		m_Key4.ShowWindow(SW_SHOW);
		m_Key5.ShowWindow(SW_SHOW);
		m_Key6.ShowWindow(SW_SHOW);
		m_Key7.ShowWindow(SW_SHOW);
		m_Key8.ShowWindow(SW_SHOW);
		m_Key9.ShowWindow(SW_SHOW);
		m_KeyClear.ShowWindow(SW_SHOW);
		m_KeyHelp.ShowWindow(SW_SHOW);
	}
	if (vAllowDashes)
	{
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
	}
	if ((vAllowPeriod) || (vAllowComma))
	{
		m_Function3Button.ShowWindow(SW_SHOW);
	}
	
	if (vAllowApproximate)
	{
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction2Button, _T("~"));
	}
	if ((vAllowSlashes) || (vAllowParenthesis))
	{
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction1Button.ShowWindow(SW_SHOW);
	}
	if (vAllowIPComma)
	{
		m_SubFunction2Button.ShowWindow(SW_SHOW);
	}

	
	SetNumLock(true);
	if (vChatMode)
	if (!vShowingTouchKeyboard)
	{
		m_TextEditing.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 20),
				ResizeYCoor(cMainBackgroundTop + 20),ResizeXCoor(cMainBackgroundLength - 40),
				ResizeYCoor(cMainBackgroundHeight - 40),SWP_NOZORDER);
	}
		
	if (vLocalSystemData)
	if (vChatMode)
	//if (PasswordOK(cTemporaryInspxPassword,false))
	{
		m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction1Button, _T("Flash\nAll\nLights"));
		m_SubFunction1Button.ShowWindow(SW_SHOW);
//		m_ArrowLeft1.ShowWindow(SW_SHOW);
	}

	DWORD TempResult;
	if (vCursorPosition)
	{
		vSelectCharacterFormat.yHeight = cExtraHugeFont;
		TempResult = m_TextEditing.GetDefaultCharFormat(vSelectCharacterFormat);
		m_TextEditing.SetSelectionCharFormat(vSelectCharacterFormat);
		m_TextEditing.SetSel(0,vCursorPosition);
	}

	SetupMenu(vLocalCWndCollection);

	m_SubFunction4Button.ShowNavigationArrow(cLeftArrowBig);
	m_SubFunction8Button.ShowNavigationArrow(cRightArrowBig);

	m_TextEditing.SetSelectionCharFormat(vCharacterFormat);

	if (vChatMode)
		m_TextEditing.SetFont(&vGlobalCurrentFonts[4],true);
	else
		m_TextEditing.SetFont(&vGlobalCurrentFonts[5],true);

	ShowTouchScreenKeyboard(vShowingTouchKeyboard);
	//if (vShowingTouchKeyboard == true)
	//{
	//	m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	//	SetDlgItemText(IDC_SubFunction5Button, "Hide Touch Keyboard");
	//	m_SubFunction5Button.ShowWindow(SW_SHOW);
	//	ShowTouchScreenKeyboard(true);
	//	ProcessAlphaNumericKey(' ');
	//}
//#endif
}

void CAlphaEntryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlphaEntryDialog)
	DDX_Control(pDX, IDC_DialogTitleStaticText3, m_DialogTitleStaticText3Control);
	DDX_Control(pDX, IDC_TextEditing, m_TextEditing);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticText2Control);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1Control);
	DDX_Control(pDX, IDC_DialogTitleStaticText, m_DialogTitleStaticTextControl);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_KeyHelp, m_KeyHelp);
	DDX_Control(pDX, IDC_KeyClear, m_KeyClear);
	DDX_Control(pDX, IDC_Key9, m_Key9);
	DDX_Control(pDX, IDC_Key8, m_Key8);
	DDX_Control(pDX, IDC_Key7, m_Key7);
	DDX_Control(pDX, IDC_Key6, m_Key6);
	DDX_Control(pDX, IDC_Key5, m_Key5);
	DDX_Control(pDX, IDC_Key4, m_Key4);
	DDX_Control(pDX, IDC_Key3, m_Key3);
	DDX_Control(pDX, IDC_Key2, m_Key2);
	DDX_Control(pDX, IDC_Key1, m_Key1);
	DDX_Control(pDX, IDC_Key0, m_Key0);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Text(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticText2);
	DDX_Text(pDX, IDC_DialogTitleStaticText, m_DialogTitleStaticText);
	DDX_Text(pDX, IDC_TextEditing, m_TextEditingString);
	DDX_Text(pDX, IDC_DialogTitleStaticText3, m_DialogTitleStaticText3);
	DDV_MaxChars(pDX, m_DialogTitleStaticText3, 200);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_KeyBoard1, m_KeyBoard1);
	DDX_Control(pDX, IDC_KeyBoard10, m_KeyBoard10);
	DDX_Control(pDX, IDC_KeyBoard11, m_KeyBoard11);
	DDX_Control(pDX, IDC_KeyBoard12, m_KeyBoard12);
	DDX_Control(pDX, IDC_KeyBoard13, m_KeyBoard13);
	DDX_Control(pDX, IDC_KeyBoard14, m_KeyBoard14);
	DDX_Control(pDX, IDC_KeyBoard15, m_KeyBoard15);
	DDX_Control(pDX, IDC_KeyBoard16, m_KeyBoard16);
	DDX_Control(pDX, IDC_KeyBoard17, m_KeyBoard17);
	DDX_Control(pDX, IDC_KeyBoard18, m_KeyBoard18);
	DDX_Control(pDX, IDC_KeyBoard19, m_KeyBoard19);
	DDX_Control(pDX, IDC_KeyBoard2, m_KeyBoard2);
	DDX_Control(pDX, IDC_KeyBoard20, m_KeyBoard20);
	DDX_Control(pDX, IDC_KeyBoard21, m_KeyBoard21);
	DDX_Control(pDX, IDC_KeyBoard22, m_KeyBoard22);
	DDX_Control(pDX, IDC_KeyBoard23, m_KeyBoard23);
	DDX_Control(pDX, IDC_KeyBoard24, m_KeyBoard24);
	DDX_Control(pDX, IDC_KeyBoard25, m_KeyBoard25);
	DDX_Control(pDX, IDC_KeyBoard26, m_KeyBoard26);
	DDX_Control(pDX, IDC_KeyBoard27, m_KeyBoard27);
	DDX_Control(pDX, IDC_KeyBoard3, m_KeyBoard3);
	DDX_Control(pDX, IDC_KeyBoard4, m_KeyBoard4);
	DDX_Control(pDX, IDC_KeyBoard5, m_KeyBoard5);
	DDX_Control(pDX, IDC_KeyBoard6, m_KeyBoard6);
	DDX_Control(pDX, IDC_KeyBoard7, m_KeyBoard7);
	DDX_Control(pDX, IDC_KeyBoard8, m_KeyBoard8);
	DDX_Control(pDX, IDC_KeyBoard9, m_KeyBoard9);
	DDX_Control(pDX, IDC_KeyBoard28, m_KeyBoard28);
	DDX_Control(pDX, IDC_KeyBoard29, m_KeyBoard29);
	DDX_Control(pDX, IDC_KeyBoard30, m_KeyBoard30);
	DDX_Control(pDX, IDC_KeyBoard31, m_KeyBoard31);
	DDX_Control(pDX, IDC_KeyBoard32, m_KeyBoard32);
	DDX_Control(pDX, IDC_KeyBoard33, m_KeyBoard33);
	DDX_Control(pDX, IDC_KeyBoard34, m_KeyBoard34);
	DDX_Control(pDX, IDC_KeyBoard35, m_KeyBoard35);
	DDX_Control(pDX, IDC_KeyBoard36, m_KeyBoard36);
	DDX_Control(pDX, IDC_KeyBoard37, m_KeyBoard37);
	DDX_Control(pDX, IDC_KeyBoard38, m_KeyBoard38);
	DDX_Control(pDX, IDC_KeyBoard39, m_KeyBoard39);
	DDX_Control(pDX, IDC_KeyBoard40, m_KeyBoard40);
	//  DDX_Control(pDX, IDC_KeyBoard41, m_KeyBoard47);
	//  DDX_Control(pDX, IDC_KeyBoard42, m_KeyBoard48);
	//  DDX_Control(pDX, IDC_KeyBoard43, m_KeyBoard49);
	DDX_Control(pDX, IDC_KeyBoard41, m_KeyBoard41);
	DDX_Control(pDX, IDC_KeyBoard42, m_KeyBoard42);
	DDX_Control(pDX, IDC_KeyBoard43, m_KeyBoard43);
	DDX_Control(pDX, IDC_KeyBoard44, m_KeyBoard44);
	DDX_Control(pDX, IDC_KeyBoard45, m_KeyBoard45);
	DDX_Control(pDX, IDC_KeyBoard46, m_KeyBoard46);
	DDX_Control(pDX, IDC_KeyBoard47, m_KeyBoard47);
}


BEGIN_MESSAGE_MAP(CAlphaEntryDialog, CDialog)
	//{{AFX_MSG_MAP(CAlphaEntryDialog)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_Key0, OnKey0)
	ON_BN_CLICKED(IDC_Key1, OnKey1)
	ON_BN_CLICKED(IDC_Key2, OnKey2)
	ON_BN_CLICKED(IDC_Key3, OnKey3)
	ON_BN_CLICKED(IDC_Key4, OnKey4)
	ON_BN_CLICKED(IDC_Key5, OnKey5)
	ON_BN_CLICKED(IDC_Key6, OnKey6)
	ON_BN_CLICKED(IDC_Key7, OnKey7)
	ON_BN_CLICKED(IDC_Key8, OnKey8)
	ON_BN_CLICKED(IDC_Key9, OnKey9)
	ON_BN_CLICKED(IDC_KeyClear, OnKeyClear)
	ON_BN_CLICKED(IDC_KeyHelp, OnKeyHelp)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CAlphaEntryDialog::OnDialogtitlestatictext1)

	ON_STN_CLICKED(IDC_KeyBoard1, &CAlphaEntryDialog::OnKeyboard1)
	ON_STN_CLICKED(IDC_KeyBoard2, &CAlphaEntryDialog::OnKeyboard2)
	ON_STN_CLICKED(IDC_KeyBoard3, &CAlphaEntryDialog::OnKeyboard3)
	ON_STN_CLICKED(IDC_KeyBoard4, &CAlphaEntryDialog::OnKeyboard4)
	ON_STN_CLICKED(IDC_KeyBoard5, &CAlphaEntryDialog::OnKeyboard5)
	ON_STN_CLICKED(IDC_KeyBoard6, &CAlphaEntryDialog::OnKeyboard6)
	ON_STN_CLICKED(IDC_KeyBoard7, &CAlphaEntryDialog::OnKeyboard7)
	ON_STN_CLICKED(IDC_KeyBoard8, &CAlphaEntryDialog::OnKeyboard8)
	ON_STN_CLICKED(IDC_KeyBoard9, &CAlphaEntryDialog::OnKeyboard9)
	ON_STN_CLICKED(IDC_KeyBoard10, &CAlphaEntryDialog::OnKeyboard10)
	ON_STN_CLICKED(IDC_KeyBoard11, &CAlphaEntryDialog::OnKeyboard11)
	ON_STN_CLICKED(IDC_KeyBoard13, &CAlphaEntryDialog::OnKeyboard13)
	ON_STN_CLICKED(IDC_KeyBoard12, &CAlphaEntryDialog::OnKeyboard12)
	ON_STN_CLICKED(IDC_KeyBoard14, &CAlphaEntryDialog::OnKeyboard14)
	ON_STN_CLICKED(IDC_KeyBoard15, &CAlphaEntryDialog::OnKeyboard15)
	ON_STN_CLICKED(IDC_KeyBoard16, &CAlphaEntryDialog::OnKeyboard16)
	ON_STN_CLICKED(IDC_KeyBoard17, &CAlphaEntryDialog::OnKeyboard17)
	ON_STN_CLICKED(IDC_KeyBoard18, &CAlphaEntryDialog::OnKeyboard18)
	ON_STN_CLICKED(IDC_KeyBoard19, &CAlphaEntryDialog::OnKeyboard19)
	ON_STN_CLICKED(IDC_KeyBoard20, &CAlphaEntryDialog::OnKeyboard20)
	ON_STN_CLICKED(IDC_KeyBoard21, &CAlphaEntryDialog::OnKeyboard21)
	ON_STN_CLICKED(IDC_KeyBoard22, &CAlphaEntryDialog::OnKeyboard22)
	ON_STN_CLICKED(IDC_KeyBoard23, &CAlphaEntryDialog::OnKeyboard23)
	ON_STN_CLICKED(IDC_KeyBoard24, &CAlphaEntryDialog::OnKeyboard24)
	ON_STN_CLICKED(IDC_KeyBoard25, &CAlphaEntryDialog::OnKeyboard25)
	ON_STN_CLICKED(IDC_KeyBoard26, &CAlphaEntryDialog::OnKeyboard26)
	ON_STN_CLICKED(IDC_KeyBoard27, &CAlphaEntryDialog::OnKeyboard27)
	ON_STN_CLICKED(IDC_KeyBoard28, &CAlphaEntryDialog::OnKeyboard28)
	ON_STN_CLICKED(IDC_KeyBoard29, &CAlphaEntryDialog::OnKeyboard29)
	ON_STN_CLICKED(IDC_KeyBoard30, &CAlphaEntryDialog::OnKeyboard30)
	ON_STN_CLICKED(IDC_KeyBoard31, &CAlphaEntryDialog::OnKeyboard31)
	ON_STN_CLICKED(IDC_KeyBoard32, &CAlphaEntryDialog::OnKeyboard32)
	ON_STN_CLICKED(IDC_KeyBoard33, &CAlphaEntryDialog::OnKeyboard33)
	ON_STN_CLICKED(IDC_KeyBoard34, &CAlphaEntryDialog::OnKeyboard34)
	ON_STN_CLICKED(IDC_KeyBoard35, &CAlphaEntryDialog::OnKeyboard35)
	ON_STN_CLICKED(IDC_KeyBoard36, &CAlphaEntryDialog::OnKeyboard36)
	ON_STN_CLICKED(IDC_KeyBoard37, &CAlphaEntryDialog::OnKeyboard37)
	ON_STN_CLICKED(IDC_KeyBoard38, &CAlphaEntryDialog::OnKeyboard38)
	ON_STN_CLICKED(IDC_KeyBoard39, &CAlphaEntryDialog::OnKeyboard39)
	ON_STN_CLICKED(IDC_KeyBoard40, &CAlphaEntryDialog::OnKeyboard40)
	ON_STN_CLICKED(IDC_KeyBoard41, &CAlphaEntryDialog::OnKeyboard41)
	ON_STN_CLICKED(IDC_KeyBoard42, &CAlphaEntryDialog::OnKeyboard42)
	ON_STN_CLICKED(IDC_KeyBoard43, &CAlphaEntryDialog::OnKeyboard43)
	ON_STN_CLICKED(IDC_KeyBoard44, &CAlphaEntryDialog::OnKeyboard44)
	ON_STN_CLICKED(IDC_KeyBoard45, &CAlphaEntryDialog::OnKeyboard45)
	ON_STN_CLICKED(IDC_KeyBoard46, &CAlphaEntryDialog::OnKeyboard46)
	ON_STN_CLICKED(IDC_KeyBoard47, &CAlphaEntryDialog::OnKeyboard47)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlphaEntryDialog message handlers

void CAlphaEntryDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	if (!vNoMainMenu)
	if ((vChatMode) || (AbortSetupChanges()))
	{
		PrepareToExit();
		CDialog::EndDialog(10);
	}
}

void CAlphaEntryDialog::OnFunction2Button() 
{
	if (!vInAutoSetup)
	{
		PrepareToExit();
		SaveAndExit();
	}
}

void CAlphaEntryDialog::SaveAndExit() 
{
	WORD TempLength = vEditString.GetLength();
	if ((TempLength > 1) || ((TempLength == 1) && (vEditString != "&")) || (vBlankNameOK) || (vChatMode))
  {
		if ((!vMinimumNumberOfCharacters) || (TempLength > vMinimumNumberOfCharacters))
		{
			WORD TempOldCursorPosition = vEditString.Find(_T("&"));
			if (TempOldCursorPosition >= 0)
			{
				vEditString.Delete(TempOldCursorPosition,1);
			}
			while ((vEditString.GetLength()) && (vEditString.GetAt(0) == 32))
				vEditString.Delete(0,1);
			TempLength = vEditString.GetLength();
			if ((TempLength) || (vBlankNameOK))
			{
				PrepareToExit();
				CDialog::EndDialog(true);
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				CString TempText("");
				TempText.LoadString(IDS_NameCanNotBeBlank);
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\nName too short\nmust have at least ");
			TempText = TempText + dtoa(vMinimumNumberOfCharacters,0) + " characters";
			//TempText.LoadString(IDS_NameCanNotBeBlank);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
  }
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText.LoadString(IDS_NameCanNotBeBlank);
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CAlphaEntryDialog::OnFunction3Button() 
{
	vOnScreenKeyBoardPress = false;
	if ((vAllowPeriod) || (vAllowComma))
		ProcessKey(cF3);
	if (vChatMode)
	if (vColor)
	{ //key board or mouse entry
		if (vCursorPosition < 10000)
			vColor[vCursorPosition] = cBlue;

		DisplayColor();
	}
}

void CAlphaEntryDialog::OnFunction4Button() 
{
	//Cancel was pressed
	PrepareToExit();
	CDialog::OnCancel();
}

void CAlphaEntryDialog::OnFunction5Button() 
{
	if (vInAutoSetup)
	{
		PrepareToExit();
		SaveAndExit();
	}
}

void CAlphaEntryDialog::OnSubFunction1Button() 
{
	vOnScreenKeyBoardPress = false;
	//SubFunction 1 button pressed
	if ((vLocalSystemData) && (vChatMode))
	//if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (vFlashingLights)
		{
			PrepareToExit();
			SetDlgItemText(IDC_SubFunction1Button,_T("Flash\nAll\nLights"));
		}
		else
		{
			vOriginalLampStatus = vLocalSystemData->vCurrentLampStatus;
			vFlashingLights = true;
			vGlobaluCSerialPort->vFlashingLights = true; //stops other commands from changing the lights, but they update state
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Test Lamps Original State: " +
				dtoa(vOriginalLampStatus,0),cDebugMessage);
			int TimerResult = SetTimer(vFlashLightsTimerHandle,500,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-EndCalibrate Timer Failed",cEMailInspx,32000);
			SetDlgItemText(IDC_SubFunction1Button, _T("Stop Lights Flashing"));
			ReportErrorMessage("Operator Pressed Flash All Lights", cUserAction,0);
		}
	}
	else
	if ((vAllowSlashes) || (vAllowParenthesis))
		ProcessKey(cSF1);
}

void CAlphaEntryDialog::OnSubFunction2Button() //SubFunction 2 button pressed
{
	vOnScreenKeyBoardPress = false;
	//if ((vShowingTouchKeyboard) || (vChatMode))
	{
		vDeleteAll = true;
		ProcessAlphaNumericKey(' ');
		vUpperCase = true;
		SetKeyBoardToUpperCase();
	}
	//else
	//if ((vAllowSlashes) || (vAllowParenthesis) || (vAllowIPComma))
	//	ProcessKey(cSF2);
	//else
	//if (vAllowApproximate)
	//	ProcessKey(9); //126
}

void CAlphaEntryDialog::OnSubFunction3Button() 
{
	vOnScreenKeyBoardPress = false;
	if (vAllowDashes)
	  ProcessKey(cSF3);
	if (vCursorPosition < 10000)
		vColor[vCursorPosition] = cBlue;
	if (vChatMode)
		DisplayColor();
}

void CAlphaEntryDialog::OnSubFunction4Button() 
{
	vOnScreenKeyBoardPress = false;
	//Left arrow key pressed
	ProcessKey(cSF4);
}

void CAlphaEntryDialog::OnSubFunction5Button() 
{
	vOnScreenKeyBoardPress = false;
	//SubFunction 5 button pressed
	//if (vChatMode)
	//{
		if (!vShowingTouchKeyboard)
		{
			vShowingTouchKeyboard = true;
			ShowTouchScreenKeyboard(true);
		}
		else //if (vShowingTouchKeyboard == true)
		{
			vShowingTouchKeyboard = false;
			ShowTouchScreenKeyboard(false);
		}
	//}
	//else
	//if (vAllowApproximate)
	//	ProcessKey(cSF5);
}

void CAlphaEntryDialog::OnSubFunction6Button() 
{
	vOnScreenKeyBoardPress = false;
	// User pressed caps lower case button
	if (vShowingTouchKeyboard)
		OnKeyClear();
	else
	{
		vLastKey = 0;
		vLastKeyTimes = 0;
		CString TempText("");
		m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (vUpperCase)
		{
			m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText.LoadString(IDS_ChangeToUpperCase);
			SetDlgItemText(IDC_SubFunction6Button,TempText);
			vUpperCase = false;

			WORD TempLoop = 0;
			for (;TempLoop < cNumberOfKeys; TempLoop++)
				vLocalKey[TempLoop] = cLocalLowerAlphaKey[TempLoop];
		}
		else
		{
			m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText.LoadString(IDS_ChangeToLowerCase);
			SetDlgItemText(IDC_SubFunction6Button,TempText);
			vUpperCase = true;

			WORD TempLoop = 0;
			for (;TempLoop < cNumberOfKeys; TempLoop++)
				vLocalKey[TempLoop] = cLocalAlphaKey[TempLoop];
		}
	}
	//SetCursor();
	//SubFunction 6 button pressed
	//ProcessKey(cSF6);
}

void CAlphaEntryDialog::OnSubFunction7Button() 
{
	vOnScreenKeyBoardPress = false;
	//SubFunction 7 button pressed
	if (vAllowDashes)
		ProcessKey(cSF7);
	if (vCursorPosition < 10000)
		vColor[vCursorPosition] = cBlue;
	if (vChatMode)
		DisplayColor();
}

void CAlphaEntryDialog::OnSubFunction8Button() 
{
	vOnScreenKeyBoardPress = false;
	//Right Arrow Button pressed
	ProcessKey(cSF8);
}

BOOL CAlphaEntryDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->message == WM_KEYDOWN) && (!vShowingTouchKeyboard) && (pMsg->wParam == 96))
	{
		pMsg->wParam = 48;  //make zero key if from keypad
		pMsg->message = WM_CHAR;
	}

	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if ((pMsg->message == 513) || (pMsg->message == 514) || (pMsg->message == 515)) //mouse click
	{
		//system locks up if click mouse in edit box, so don't let system
		//process mouse clicks in the edit box
			if ((pMsg->pt.x > vDoNotClickLeftX) && (pMsg->pt.x < vDoNotClickRightX) && 
				(pMsg->pt.y > vDoNotClickUpperY) && (pMsg->pt.y < vDoNotClickLowerY))
				return true;
/*			if (vChatMode)
			{
				if (vShowingTouchKeyboard)
				{
					if ((pMsg->pt.x > 105) && (pMsg->pt.x < 533) && 
						(pMsg->pt.y > 42) && (pMsg->pt.y < 200))
						return true;
				} 
				else
					if ((pMsg->pt.x > 105) && (pMsg->pt.x < 533) && 
						(pMsg->pt.y > 42) && (pMsg->pt.y < 412))
						return true;
			}
			else
			{
				if ((pMsg->pt.x > 105) && (pMsg->pt.x < 533) && 
					(pMsg->pt.y > 42) && (pMsg->pt.y < 150))
					return true;
			}*/
	}
	else
	if ((pMsg->message == WM_CHAR) && (((pMsg->wParam >= 'A') && (pMsg->wParam <= 'Z') && (!vNumbersOnly)) || ((pMsg->wParam >= 'a') && (pMsg->wParam <= 'z') && (!vNumbersOnly)) ||  
		((pMsg->wParam == ' ') && (!vNumbersOnly))))
	{
		ProcessAlphaNumericKey(pMsg->wParam);
		return true;
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalShiftKeyDown)
		{
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
			CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
			if (TempWindow)
				TempWindow->Invalidate(false);
		}
		else
		if (pMsg->wParam == 113)
				OnSubFunction5Button();
		else
		if (pMsg->wParam == 114)
				OnSubFunction7Button();
		else
		if (pMsg->wParam == 115)
				OnSubFunction6Button();
		else
		if (pMsg->wParam == 116)
				OnSubFunction1Button();
		else
		if (pMsg->wParam == 117)
				OnSubFunction4Button();
		else
		if (pMsg->wParam == 118)
				OnFunction1Button();
		else
		if (pMsg->wParam == 119)
				OnSubFunction8Button();
		else
		if (pMsg->wParam == 120)
				OnSubFunction2Button();
		else
		if (pMsg->wParam == 122)
				OnSubFunction3Button();
		else
		if (pMsg->wParam == 111)
				OnFunction2Button();
		else
		if (pMsg->wParam == 106)
				OnFunction4Button();
		else
		if (pMsg->wParam == 109)
				OnFunction3Button();
		else
		if (pMsg->wParam == 107)
				OnFunction5Button();
		else
		if ((pMsg->wParam >= c0) && (pMsg->wParam <= c9))
		{
		//brh try this
				if (vShowingTouchKeyboard)
					OnSubFunction5Button();
				ProcessKey(pMsg->wParam);
		}
		else
		if ((pMsg->wParam == 16) || (pMsg->wParam == 16))
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
		else
		if (pMsg->wParam == 36)
		{
			vGlobalShiftKeyDown = true;
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
			if (TempWindow)
				TempWindow->Invalidate(false);
		}
		else
		if (pMsg->wParam == cPageDown)
		{
			PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
			{
				CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
		}
		else
		if (pMsg->wParam == 27)
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
		else
		if (!(((pMsg->wParam >= 'A') && (pMsg->wParam <= 'Z') && (!vNumbersOnly)) || ((pMsg->wParam >= 'a') && (pMsg->wParam <= 'z') && (!vNumbersOnly)) ||  
			((pMsg->wParam == ' ') && (!vNumbersOnly))))
		{
			//if (!vGlobalShiftKeyDown)
			if ((pMsg->wParam == 8) && (!vChatMode))// change back space to clear
				pMsg->wParam = 46;

			if ((pMsg->wParam == 110) && (!vChatMode))// . key on side keypad
				OnKeyboard47();
			else
			if ((pMsg->wParam == 190) && (!vChatMode))// . key on keyboard
				OnKeyboard47();
			else
			if ((pMsg->wParam == 192) && (!vChatMode))// ~ key on keyboard
				OnKeyboard43();
			else
			{
				if ((pMsg->wParam == 16) && (!vChatMode))// Shift key, but don't use in chat mode
				{
					vGlobalShiftKeyDown = true;
					return true;
				}
				else
				if (pMsg->wParam == 27) // Escape
				{
					if (!vChatMode)
							::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
					else
						return true;
				}
				if ((pMsg->wParam == 13) && (!vChatMode))
				{
					if (vInAutoSetup)
						OnFunction5Button();
					else
						OnFunction2Button();
					return true;
				}
				else
				if (pMsg->wParam == 36)  //Home key, Screen Capture
				{
					vGlobalShiftKeyDown = true;
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
				}
				else
				if ((!vChatMode) && (pMsg->wParam == 49) && (vGlobalShiftKeyDown)) //don't accept an ! for names
					return true;
				else
				if ((pMsg->wParam >= 48) && (pMsg->wParam <= 57)) //accept 0 to 9 keys
				{
					ProcessAlphaNumericKey(pMsg->wParam);
					return true;
				}
				else
				if ((!vChatMode) && (pMsg->wParam == 192) && (vGlobalShiftKeyDown) && (!vShowingTouchKeyboard)) //accept a ~ for names
				{
					if (PasswordOK(cTemporaryInspxPassword, false))
						ProcessKey(126);
				}
				else
				if ((vChatMode) && (pMsg->wParam == 113))
					OnSubFunction5Button();
				else
				if ((vChatMode) && (pMsg->wParam == 116))
					OnSubFunction1Button();
				else
				if ((vChatMode) && (pMsg->wParam == 120))
					OnSubFunction2Button();
				else
				if ((pMsg->wParam == 16) && (vChatMode))// Shift key, but don't use in chat mode
				{
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
					return true;
				}
				else
				if (((!vAcceptAllKeys) || ((pMsg->wParam >= 96) && (pMsg->wParam <= 105)) || (pMsg->wParam == 117) || (pMsg->wParam == 119) || (pMsg->wParam == 109)
						|| (pMsg->wParam == 106) || (pMsg->wParam == 107) || (pMsg->wParam == 115) || (pMsg->wParam == 118) || (pMsg->wParam == 113) || (pMsg->wParam == 114) 
						|| (pMsg->wParam == 111)	|| (pMsg->wParam == 33)	 || (pMsg->wParam == 34) || (pMsg->wParam == 0x74)|| (pMsg->wParam == 0x78) || (pMsg->wParam == 116) ||
						((vAllowDashes) && ((pMsg->wParam == cSF3) || (pMsg->wParam == 189)))) &&
					((vAllowSlashes) || ((pMsg->wParam != 0x2F) && (pMsg->wParam != 0x5C) && (pMsg->wParam != 0x22) && (pMsg->wParam != 0x23) && (pMsg->wParam != 0x24) && 
					(pMsg->wParam != 0x25) && (pMsg->wParam != 0x26) && (pMsg->wParam != 0x27) && (pMsg->wParam != 0x28) && (pMsg->wParam != 0x29) && (pMsg->wParam != 0x2A) && 
					(pMsg->wParam != 0x31) && (pMsg->wParam != 0x3C) && (pMsg->wParam != 0x3D) && (pMsg->wParam != 0x3E) && (pMsg->wParam != 0x3F) && (pMsg->wParam != 0x40) && 
					(pMsg->wParam != 0x5B) && (pMsg->wParam != 0x5D) && (pMsg->wParam != 0x5E) && (pMsg->wParam != 0x7B) && (pMsg->wParam != 0x7C) && (pMsg->wParam != 0x7D) && 
					(pMsg->wParam != 0x7E) && (pMsg->wParam != 0x7F) && (pMsg->wParam != 0x7B) && (pMsg->wParam != 0x7C) && (pMsg->wParam != 0x7D) && 
					(pMsg->wParam != 0x2B) && (pMsg->wParam != 0x2C) && (pMsg->wParam != 0x2E) && (pMsg->wParam != 0x3A) && (pMsg->wParam != 0x3B) && (pMsg->wParam != 0x14)))) //0X14 is cap locks
				{
					if ((pMsg->wParam == 189) && (vGlobalShiftKeyDown))
					{//Underscore Key
						pMsg->wParam = cSF3;
						vGlobalShiftKeyDown = false;
					}
					if (pMsg->wParam == 189) //dash key
						pMsg->wParam = cSF7;

					//ProcessKey(pMsg->wParam);
					ProcessAlphaNumericKey(pMsg->wParam);

					if (vChatMode)
					if (pMsg->wParam != 33) //clear key
					if (pMsg->wParam != 119)//right arrow button
					if (vColor)
					{  //key pad entry
						if (vCursorPosition < 10000)
							vColor[vCursorPosition] = cBlue;
						DisplayColor();
					}
					return true;
				}
				else
				{
					if (((pMsg->wParam == 13) && (vChatMode)) || 
						(pMsg->wParam == 8) || (pMsg->wParam == 35) || (pMsg->wParam == 36) || 
						(pMsg->wParam == 37) || (pMsg->wParam == 39) || 
						(pMsg->wParam == 38) || (pMsg->wParam == 40) ||
						(pMsg->wParam == 46))
					{
						if ((pMsg->wParam == 40) || (pMsg->wParam == 35))  //35 is end
						{ //Down Arrow
							vDeleteAll = false;
							vCursorPosition = vEditString.GetLength();
							SetCursor();
							if (vChatMode)
								DisplayColor();
							return true;
						}
						else
						if ((pMsg->wParam == 38) || (pMsg->wParam == 36))  //36 is home
						{ //Up Arrow
							vDeleteAll = false;
							vCursorPosition = 0;
							SetCursor();
							if (vChatMode)
								DisplayColor();
							return true;
						}
						else
						if (pMsg->wParam == 39)
						{ //Right Arrow
							vDeleteAll = false;
							if (vCursorPosition < vEditString.GetLength())
								vCursorPosition++;
							SetCursor();
							if (vChatMode)
								DisplayColor();
							return true;
						}
						else
						if (pMsg->wParam == 37)
						{ //Left Arrow
							if (vDeleteAll)
							{
								vDeleteAll = false;
								vCursorPosition = 0;
							}
							if (vCursorPosition > 0)
								vCursorPosition--;
							SetCursor();
							if (vChatMode)
								DisplayColor();
							return true;
						}

						if (vDeleteAll)
						{
							vEditString = "";
							vDeleteAll = false;
							SetCursor();
							return true;
						}

						if (pMsg->wParam == 13)
						{ //Enter key
							WORD TempLength = vEditString.GetLength();
							vEditString.Insert(vCursorPosition,(CHAR)10);

							if (vCursorPosition < TempLength)
							if (vColor)
							for (WORD TempLoop = TempLength; TempLoop > vCursorPosition + 1; TempLoop--)
							if (TempLoop + 1 < 10000)
								vColor[TempLoop + 1] = vColor[TempLoop];

							vCursorPosition++;
							SetCursor();
							if (vChatMode)
								DisplayColor();
							return true;
						}
						if (pMsg->wParam == 46) //Delete key
						{
							OnKeyClear();
							SetCursor();
							if (vChatMode)
								DisplayColor();
							return true;
						}
						else
						if (pMsg->wParam == 8) //back space key
						{
							if (vCursorPosition > 0)
							{
								if (vCursorPosition < vEditString.GetLength())
								{
									vCursorPosition--;
									SetCursor();
								}
								OnKeyClear();
								if (vChatMode)
									DisplayColor();
							}
							return true;
						}
					}
				}
			}
		}
	}
	else
	if (pMsg->message == WM_CHAR)
	//if (!vGlobalShiftKeyDown)
	if (vShowingTouchKeyboard)	
	{
		//get an alpha numeric key press
		if (vAcceptAllKeys)  
		{
			if (((pMsg->wParam >= 32) && (pMsg->wParam <= 126)) || 
				((pMsg->wParam == 13) && (vChatMode)) || 
				(pMsg->wParam == 8))
			{
				if (vDeleteAll)
				{
					vEditString = "";
					vDeleteAll = false;
				}

				//if ((!vShowingTouchKeyboard) && ((pMsg->wParam >= 48) && (pMsg->wParam <= 57)))  //overlay support with pressing 0 to 9 multiple times to get alpha entry
				//	ProcessKey(pMsg->wParam);
				//else
				if (pMsg->wParam == 8) //back space key
				{
					if (vCursorPosition > 0)
					{
						if (vCursorPosition < vEditString.GetLength())
						{
							vCursorPosition--;
							SetCursor();
						}
						OnKeyClear();
						if (vChatMode)
							DisplayColor();
					}
				}
				else
				{ //alpha numeric key
					WORD TempLength = vEditString.GetLength();
					if (((TempLength > 32) && (!vAllowMoreThan32Characters) && (!vChatMode)) ||
						(TempLength > 9998))
					{
						CNoticeDialog TempNoticeDialog;
						CString TempText("");
						TempText.LoadString(IDS_32CharactersMaximum);
						TempNoticeDialog.vNoticeText = TempText;
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
					else
					if ((vMaximumNumberOfCharacters) && (TempLength >= vMaximumNumberOfCharacters))
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nOnly " + dtoa(vMaximumNumberOfCharacters,0) + " characters allowed.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
					else
					if ((vAllowSlashes) || (!vOnScreenKeyBoardPress) || ((pMsg->wParam != 0x2F) && (pMsg->wParam != 0x5C) && (pMsg->wParam != 0x22) && (pMsg->wParam != 0x23) && (pMsg->wParam != 0x24) && 
						(pMsg->wParam != 0x25) && (pMsg->wParam != 0x26) && (pMsg->wParam != 0x27) && (pMsg->wParam != 0x28) && (pMsg->wParam != 0x29) && (pMsg->wParam != 0x2A) && 
						(pMsg->wParam != 0x3C) && (pMsg->wParam != 0x3D) && (pMsg->wParam != 0x3E) && (pMsg->wParam != 0x3F) && (pMsg->wParam != 0x40) && (pMsg->wParam != 0x5B) && 
						(pMsg->wParam != 0x5D) && (pMsg->wParam != 0x5E) && (pMsg->wParam != 0x60) && (pMsg->wParam != 0x7B) && (pMsg->wParam != 0x7C) && (pMsg->wParam != 0x7D) && 
						(pMsg->wParam != 0x7E) && (pMsg->wParam != 0x7F) && (pMsg->wParam != 0x60) && (pMsg->wParam != 0x7B) && (pMsg->wParam != 0x7C) && (pMsg->wParam != 0x7D) && 
						(pMsg->wParam != 0x2B) && (pMsg->wParam != 0x2C) && (pMsg->wParam != 0x2D) && (pMsg->wParam != 0x2E) && (pMsg->wParam != 0x3A) && (pMsg->wParam != 0x3B)))
					{
						WORD TempLength = vEditString.GetLength();
						vEditString.Insert(vCursorPosition,(CHAR)pMsg->wParam);

						if (vCursorPosition < TempLength)
						if (vColor)
						for (WORD TempLoop = TempLength; TempLoop > vCursorPosition + 1; TempLoop--)
						if (TempLoop + 1 < 10000)
							vColor[TempLoop + 1] = vColor[TempLoop];

						vCursorPosition++;
						vLastKeyTimes = 0;
						vLastKey = 0;
						SetCursor();

						if (vChatMode)
						if (vColor)
						{ //key board entry
							if (vCursorPosition < 10000)
								vColor[vCursorPosition] = cRed;

							DisplayColor();
						}
					}
				}
			}
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

bool CAlphaEntryDialog::ProcessKey(BYTE TempKey)
{
	bool TempReturn = false;
	if ((!vShowingTouchKeyboard) && ((TempKey >= 48) && (TempKey <= 57)))  //overlay support with pressing 0 to 9 multiple times to get alpha entry
	{
		//if hitting same key as before, back up one character
		if (vDeleteAll)
		{
			vEditString = "";
			vDeleteAll = false;
		}

		if ((vLastKey == TempKey) && (!vNumbersOnly))
		{
			WORD TempLength = vEditString.GetLength();
			if (vCursorPosition > 0)
			{
				vEditString.Delete(vCursorPosition - 1,1);
				vCursorPosition--;
			}
			vLastKeyTimes++;
			//the zero key only has 3 items on it!!! so skip an item
			if ((TempKey == 48) && (vLastKeyTimes ==2))
							vLastKeyTimes++;

			if (vLastKeyTimes > 3)
				vLastKeyTimes = 0;
		}
		//hitting new key, so reset key hit counters
		else
		{
			//make sure string plus & is not over 33 characters
			// so have room to add one more
			WORD TempLength = vEditString.GetLength();
			if ((TempLength < 33) || (vAllowMoreThan32Characters) || (vAcceptAllKeys))
			if ((!vMaximumNumberOfCharacters) || ((TempLength < vMaximumNumberOfCharacters)))
			{
				vLastKeyTimes = 0;
				vLastKey = TempKey;
			}
		}
		if ((TempKey == cF3) || (TempKey == cSF1) || (TempKey == cSF2) || (TempKey == cSF3) || 
			(TempKey == cSF5) || (TempKey == cSF6) || (TempKey == cSF7))
		{
			vLastKeyTimes = 0;
			vLastKey = '*';
		}
		WORD TempLength = vEditString.GetLength();
		//make sure string 32 characters
		// so have room to add one more
		if (((TempLength > 32) && (!vAllowMoreThan32Characters) && (!vChatMode)) ||
			(TempLength > 9998))
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			TempText.LoadString(IDS_32CharactersMaximum);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if ((vMaximumNumberOfCharacters) && (TempLength >= vMaximumNumberOfCharacters))
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nOnly " + dtoa(vMaximumNumberOfCharacters,0) + " characters allowed.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			WORD TempIndex = 0;

			if ((TempKey >= 48) && (TempKey <= 57))
			{
				if (vNumbersOnly)
					vLastKeyTimes = 3; //only can enter 0 to 9

				TempIndex = TempKey - 48 + c0Key;
			}
			else
			{
				switch (TempKey)
				{
					case cF3:
						TempIndex = cF3Key;
					break;
					case cSF1:
						TempIndex = cSF1Key;
					break;
					case cSF2:
						TempIndex = cSF2Key;
					break;
					case cSF3:
						TempIndex = cSF3Key;
					break;
					case cSF5:
						TempIndex = cSF5Key;
					break;
					case cSF6:
						TempIndex = cSF6Key;
					break;
					case cSF7:
						TempIndex = cSF7Key;
					break;
				}
			}
//				vEditString = vEditString + (CHAR)vLocalKey[TempIndex + (vLastKeyTimes * cMaxKeys)];
			WORD TempLength = vEditString.GetLength();
			if ((TempKey == 9) && (vAllowApproximate))
				vEditString.Insert(vCursorPosition,(CHAR)'~');
			else
			if ((TempIndex == cSF1Key) && (vAllowParenthesis))
				vEditString.Insert(vCursorPosition,(CHAR)'(');
			else
			if ((TempIndex == cSF2Key) && (vAllowParenthesis))
				vEditString.Insert(vCursorPosition,(CHAR)')');
			else
			if ((TempIndex == cSF2Key) && (vAllowIPComma))
				vEditString.Insert(vCursorPosition,(CHAR)',');
			else
			if ((TempIndex == cF3Key) && (vAllowComma))
				vEditString.Insert(vCursorPosition,(CHAR)',');
			else
				vEditString.Insert(vCursorPosition,(CHAR)vLocalKey[TempIndex + (vLastKeyTimes * cMaxKeys)]);

			if (vCursorPosition < TempLength)
			if (vColor)
			for (WORD TempLoop = TempLength; TempLoop > vCursorPosition + 1; TempLoop--)
			if (TempLoop + 1 < 10000)
				vColor[TempLoop + 1] = vColor[TempLoop];

			vCursorPosition++;
			SetCursor();
			TempReturn = true;
			//return true if handled message an no further processing needed
		}
	}
	else
	{  //touch screen
		if (((TempKey >= c0) && (TempKey <= c9)) || 
			((TempKey == cF3) && ((vAllowPeriod) || (vAllowComma))) ||
			(((vAllowSlashes) ||(vAllowParenthesis)|| (vAllowIPComma)) && (TempKey == cSF2)) ||
		
			((vAllowApproximate) && (TempKey == 9)) ||
			(((vAllowSlashes) || (vAllowParenthesis)) && (TempKey == cSF1)) ||
			((vAllowDashes) && ((TempKey == cSF3) || (TempKey == cSF7) || (TempKey == 189))))
		{
			//if hitting same key as before, back up one character
			if (vDeleteAll)
			{
				vEditString = "";
				vDeleteAll = false;
			}

			if ((vLastKey == TempKey) && (!vNumbersOnly))
			{
				WORD TempLength = vEditString.GetLength();
				if (vCursorPosition > 0)
				{
					vEditString.Delete(vCursorPosition - 1,1);
					vCursorPosition--;
				}
				vLastKeyTimes++;
				//the zero key only has 3 items on it!!! so skip an item
				if ((TempKey == c0) && (vLastKeyTimes ==2))
								vLastKeyTimes++;

				if (vLastKeyTimes > 3)
					vLastKeyTimes = 0;
			}
			//hitting new key, so reset key hit counters
			else
			{
				//make sure string plus & is not over 33 characters
				// so have room to add one more
				WORD TempLength = vEditString.GetLength();
				if ((TempLength < 33) || (vAllowMoreThan32Characters) || (vAcceptAllKeys))
				if ((!vMaximumNumberOfCharacters) || ((TempLength < vMaximumNumberOfCharacters)))
				{
					vLastKeyTimes = 0;
					vLastKey = TempKey;
				}
			}
			if ((TempKey == cF3) || (TempKey == cSF1) || (TempKey == cSF2) || (TempKey == cSF3) || 
				(TempKey == cSF5) || (TempKey == cSF6) || (TempKey == cSF7))
			{
				vLastKeyTimes = 0;
				vLastKey = '*';
			}
			WORD TempLength = vEditString.GetLength();
			//make sure string 32 characters
			// so have room to add one more
			if (((TempLength > 32) && (!vAllowMoreThan32Characters) && (!vChatMode)) ||
				(TempLength > 9998))
			{
				CNoticeDialog TempNoticeDialog;
				CString TempText("");
				TempText.LoadString(IDS_32CharactersMaximum);
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			if ((vMaximumNumberOfCharacters) && (TempLength >= vMaximumNumberOfCharacters))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nOnly " + dtoa(vMaximumNumberOfCharacters,0) + " characters allowed.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				WORD TempIndex = 0;

				if ((TempKey >= c0) && (TempKey <= c9))
				{
					if (vNumbersOnly)
						vLastKeyTimes = 3; //only can enter 0 to 9

					TempIndex = TempKey - c0 + c0Key;
				}
				else
				{
					switch (TempKey)
					{
						case cF3:
							TempIndex = cF3Key;
						break;
						case cSF1:
							TempIndex = cSF1Key;
						break;
						case cSF2:
							TempIndex = cSF2Key;
						break;
						case cSF3:
							TempIndex = cSF3Key;
						break;
						case cSF5:
							TempIndex = cSF5Key;
						break;
						case cSF6:
							TempIndex = cSF6Key;
						break;
						case cSF7:
							TempIndex = cSF7Key;
						break;
					}
				}
	//				vEditString = vEditString + (CHAR)vLocalKey[TempIndex + (vLastKeyTimes * cMaxKeys)];
				WORD TempLength = vEditString.GetLength();
				if ((TempKey == 9) && (vAllowApproximate))
					vEditString.Insert(vCursorPosition,(CHAR)'~');
				else
				if ((TempIndex == cSF1Key) && (vAllowParenthesis))
					vEditString.Insert(vCursorPosition,(CHAR)'(');
				else
				if ((TempIndex == cSF2Key) && (vAllowParenthesis))
					vEditString.Insert(vCursorPosition,(CHAR)')');
				else
				if ((TempIndex == cSF2Key) && (vAllowIPComma))
					vEditString.Insert(vCursorPosition,(CHAR)',');
				else
				if ((TempIndex == cF3Key) && (vAllowComma))
					vEditString.Insert(vCursorPosition,(CHAR)',');
				else
					vEditString.Insert(vCursorPosition,(CHAR)vLocalKey[TempIndex + (vLastKeyTimes * cMaxKeys)]);

				if (vCursorPosition < TempLength)
				if (vColor)
				for (WORD TempLoop = TempLength; TempLoop > vCursorPosition + 1; TempLoop--)
				if (TempLoop + 1 < 10000)
					vColor[TempLoop + 1] = vColor[TempLoop];

				vCursorPosition++;
				SetCursor();
				TempReturn = true;
				//return true if handled message an no further processing needed
			}
		}
		else
		{
			//didn't get alphanumeric key press, so reset key press counter
			vLastKeyTimes = 0;
			vLastKey = TempKey;
			//check for function keys
			if ((vGlobalShiftKeyDown) && (TempKey != 126))
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, TempKey);
			else
			switch (TempKey)
			{
				case 113: // SF5:
					OnSubFunction5Button();
					TempReturn = true;
				break;
				case 114: // SF7:
					OnSubFunction7Button();
					TempReturn = true;
				break;
				case 115: // SF6:
					OnSubFunction6Button();
					TempReturn = true;
				break;
				case 116: // SF1:
					OnSubFunction1Button();
					TempReturn = true;
				break;
				case 117: // F6 Key - Sub Function 4:
					if (vDeleteAll)
						vCursorPosition = 0;
					if (vCursorPosition > 0)
						vCursorPosition--;
					vDeleteAll = false;
					SetCursor();
					if (vChatMode)
						DisplayColor();
					TempReturn = true;
				break;
				case 118: // F7 Key - Sub Function 1:
					OnFunction1Button();
					TempReturn = true;
				break;
				case 119: // F8 Key - Sub Function 8:
					if (vCursorPosition < vEditString.GetLength())
						vCursorPosition++;
					vDeleteAll = false;
					SetCursor();
					if (vChatMode)
						DisplayColor();
					TempReturn = true;
				break;
				case 120: // F9 Key - Sub Function 2:
					OnSubFunction2Button();
					TempReturn = true;
				break;
				case 122: // F11 Key - Sub Function 4:
					OnSubFunction3Button();
					TempReturn = true;
				break;
				case 111: // / Key - Sub Function 5:
					OnFunction2Button();
					TempReturn = true;
				break;
				case 106: // * Key - Sub Function 6:
					OnFunction4Button();
					TempReturn = true;
				break;
				case 109: // - Key - Sub Function 7  change case:
					OnFunction3Button();
					TempReturn = true;
				break;
				case 107: // + Key - Sub Function 8:
					OnFunction5Button();
					TempReturn = true;
				break;
				case cPageUp: // PageUp:
					OnKeyClear();
					TempReturn = true;
				break;
				case 36:  //Home key, Screen Capture
					vGlobalShiftKeyDown = true;
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
				break;
				case cPageDown: // PageDown:
					OnKeyHelp();
					TempReturn = true;
				break;
			}
		}
	}
	vOnScreenKeyBoardPress = false;
	return TempReturn;
}

HBRUSH CAlphaEntryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize = 8;
	
	pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetBkMode(TRANSPARENT);
	if (vLocalSystemData == NULL)
		vLocalSystemData = vGlobalRunningData;

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1Control, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (vShowingTouchKeyboard)
	{
		if (pWnd->GetDlgCtrlID() == IDC_KeyBoard30)
		{
			TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyBoard30, 5);  //5 is medium large
		}
		if (pWnd->GetDlgCtrlID() == IDC_KeyBoard41)
		{
			TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_KeyBoard41, 5);  //not a function button but is two lines
		}
		if (pWnd->GetDlgCtrlID() == IDC_KeyBoard44)
		{
			TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyBoard44, 5);  //5 is medium large
		}
		if (pWnd->GetDlgCtrlID() == IDC_KeyBoard42)
		{
			TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyBoard42, 5);  //5 is medium large
		}
		if ((pWnd->GetDlgCtrlID() == IDC_KeyBoard1) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard2) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard3)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard4) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard5) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard6)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard7) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard8) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard9)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard10) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard11) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard12)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard13) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard14) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard15)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard16) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard17) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard18)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard19) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard20) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard21)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard22) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard23) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard24)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard25) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard26) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard27)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard28) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard29) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard31)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard32) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard33) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard34)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard35) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard36) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard37)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard38) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard39) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard40)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard43) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard45)
		|| (pWnd->GetDlgCtrlID() == IDC_KeyBoard46) || (pWnd->GetDlgCtrlID() == IDC_KeyBoard47))
		{
			TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyBoard1, 5);  //5 is medium large
		}
	}
	else
	{	
		if (pWnd->GetDlgCtrlID() == IDC_KeyHelp)
			TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyHelp, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == IDC_KeyClear)
			TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyClear, 5);  //5 is medium large
		if ((pWnd->GetDlgCtrlID() == IDC_Key1) || (pWnd->GetDlgCtrlID() == IDC_Key2) || (pWnd->GetDlgCtrlID() == IDC_Key3)
		|| (pWnd->GetDlgCtrlID() == IDC_Key4) || (pWnd->GetDlgCtrlID() == IDC_Key5) || (pWnd->GetDlgCtrlID() == IDC_Key6)
		|| (pWnd->GetDlgCtrlID() == IDC_Key7) || (pWnd->GetDlgCtrlID() == IDC_Key8) || (pWnd->GetDlgCtrlID() == IDC_Key9)
		|| (pWnd->GetDlgCtrlID() == IDC_Key0))
		{
			TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Key1, 5);  //key buttons not function buttons but is two lines
		}
		//if (pWnd->GetDlgCtrlID() == IDC_Key0)
		//	TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Key1, 6);  //key buttons not function buttons but is two lines
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		if (vFlashingLights)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	//if (vInAutoSetup)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		if (vInAutoSetup)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vGreenBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_TextEditing) 
	{
		if (vDeleteAll)
		{
			pDC->SetBkColor(cBlue);
			pDC->SetTextColor(cWhite);
		}
		//if text is highlighted, then show blue background, else
		//make transparent so shows white background
		else
			pDC->SetBkMode(TRANSPARENT);
		return vWhiteBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextControl, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText2Control, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText3Control, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAlphaEntryDialog::SetCursor()
{
	if (vCursorPosition > vEditString.GetLength())
	{
		vCursorPosition = vEditString.GetLength();
	}
	SetDlgItemText(IDC_TextEditing,vEditString);

	if (vDeleteAll)
		m_TextEditing.SetSel(0, vCursorPosition);
	else
		m_TextEditing.SetSel(vCursorPosition, vCursorPosition);
}

void CAlphaEntryDialog::OnKey0() 
{
	ProcessKey(c0);
}

void CAlphaEntryDialog::OnKey1() 
{
	ProcessKey(c1);
}

void CAlphaEntryDialog::OnKey2() 
{
	ProcessKey(c2);
}

void CAlphaEntryDialog::OnKey3() 
{
	ProcessKey(c3);
}

void CAlphaEntryDialog::OnKey4() 
{
	ProcessKey(c4);
}

void CAlphaEntryDialog::OnKey5() 
{
	ProcessKey(c5);
}

void CAlphaEntryDialog::OnKey6() 
{
	ProcessKey(c6);
}

void CAlphaEntryDialog::OnKey7() 
{
	ProcessKey(c7);
}

void CAlphaEntryDialog::OnKey8() 
{
	ProcessKey(c8);
}

void CAlphaEntryDialog::OnKey9() 
{
	ProcessKey(c9);
}

void CAlphaEntryDialog::OnKeyClear() 
{
	//if clear key pressed
	WORD TempLength = vEditString.GetLength();
	if (vDeleteAll)
		vEditString = "";
	else
	if (vCursorPosition >= TempLength - 1)  // - 1 for the &
	{
		if (TempLength > 0)
		{
			vEditString.Delete(TempLength - 1,1);
			if (vCursorPosition > TempLength)
				vCursorPosition--;

			if (vCursorPosition < TempLength)
			if (vColor)
			for (WORD TempLoop = vCursorPosition + 1; TempLoop < TempLength - 1; TempLoop++)
			if (TempLoop + 1 < 10000)
				vColor[TempLoop] = vColor[TempLoop + 1];
		}
	}
	else
	{
		vEditString.Delete(vCursorPosition,1); //vCursorPosition is at the &
		if (vCursorPosition == TempLength)
			vCursorPosition--;

		if (vCursorPosition < TempLength)
		if (vColor)
		for (WORD TempLoop = vCursorPosition + 1; TempLoop < TempLength; TempLoop++)
		if (TempLoop + 1 < 10000)
			vColor[TempLoop] = vColor[TempLoop + 1];
	}
	//SetDlgItemText(IDC_EditStaticText,vEditString);
	SetDlgItemText(IDC_TextEditing,vEditString);
	vDeleteAll = false;
	SetCursor();
	if (vChatMode)
		DisplayColor();
	vLastKey = 0;
	vLastKeyTimes = 0;

	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CAlphaEntryDialog::OnKeyHelp() 
{
	// Help key pressed
	PostThreadMessage(vGlobalCallingThreadID,cShowHelp, 23, 0);

	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CAlphaEntryDialog::PrepareToExit()
{
	if (vFlashingLights)
	if (vLocalSystemData)
	if (vGlobaluCSerialPort)
	{
		vGlobaluCSerialPort->vFlashingLights = false;//makes other commands update lights
		KillTimer(vFlashLightsTimerHandle);
		//if ((vOriginalLampStatus & 1) != (vLocalSystemData->vCurrentLampStatus & 1))
			vGlobaluCSerialPort->TurnOnOffRedLamp(vLocalSystemData->vCurrentLampStatus & 1);
		//if ((vOriginalLampStatus & 2) != (vLocalSystemData->vCurrentLampStatus & 2))
			vGlobaluCSerialPort->TurnOnOffYellowLamp(vLocalSystemData->vCurrentLampStatus & 2);
		//if ((vOriginalLampStatus & 4) != (vLocalSystemData->vCurrentLampStatus & 4))
			vGlobaluCSerialPort->TurnOnOffBlueLamp(vLocalSystemData->vCurrentLampStatus & 4);
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("End Test Lamps Current State: " + dtoa(vLocalSystemData->vCurrentLampStatus,0),cDebugMessage);
		vFlashingLights = false; 
		ReportErrorMessage("Operator Pressed Stop Flashing Lights", cUserAction,0);
	}
}


void CAlphaEntryDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vFlashLightsTimerHandle)
	if (vGlobaluCSerialPort)
  {
		if (vFlashingLights)
		if (vLocalSystemData)
		{
			if (!vFlashAllLampStatus)
				vFlashAllLampStatus = 0x07;
			else
				vFlashAllLampStatus = 0;

			tSerialCommand TempCommand;
			TempCommand[0] = 0x23;  //write to FPGA Address
			TempCommand[1] = 0xC8;  //FPGA Address 0xC008
			TempCommand[2] = 0;
			TempCommand[3] = vFlashAllLampStatus;
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CAlphaEntryDialog::DisplayColor()
{
	if (vChatMode)
	if (vColor)
	{
		SetDlgItemText(IDC_TextEditing,vEditString);
		WORD TempLength = vEditString.GetLength();
		if (TempLength > 9999)
			TempLength = 9999;

		WORD TempLast = 1;
		if (vColor)
		{
			WORD TempLoop = 2;
			for (; TempLoop <= TempLength; TempLoop++)
			if (vColor[TempLoop] != vColor[TempLoop - 1])
			{
				m_TextEditing.SetSel(TempLast - 1,TempLoop - 1);
				if (vColor[TempLoop - 1] == cRed)
					m_TextEditing.SetSelectionCharFormat(vRedCharacterFormat);
				else
				if (vColor[TempLoop - 1] == cBlue)
					m_TextEditing.SetSelectionCharFormat(vCharacterFormat);
				TempLast = TempLoop;
			}
			m_TextEditing.SetSel(TempLast - 1,TempLength);
			if (vColor[TempLoop - 1] == cRed)
				m_TextEditing.SetSelectionCharFormat(vRedCharacterFormat);
			else
			if (vColor[TempLoop - 1] == cBlue)
				m_TextEditing.SetSelectionCharFormat(vCharacterFormat);
		}
		m_TextEditing.SetSel(vCursorPosition,vCursorPosition);

		long TempCaratPosition = m_TextEditing.LineFromChar(vCursorPosition);

		int TempNumberOfLines = m_TextEditing.GetLineCount();
		int TempFirstLine = m_TextEditing.GetFirstVisibleLine();

		if (vShowingTouchKeyboard)
		{
			if (TempFirstLine + 8 <= TempCaratPosition)
				m_TextEditing.LineScroll(TempCaratPosition - 8 - TempFirstLine + 1);
			else
			if (TempFirstLine > TempCaratPosition)
				m_TextEditing.LineScroll(TempCaratPosition - TempFirstLine);
		}
		else
		{
			if (TempFirstLine + 18 <= TempCaratPosition)
				m_TextEditing.LineScroll(TempCaratPosition - 18 - TempFirstLine + 1);
			else
			if (TempFirstLine > TempCaratPosition)
				m_TextEditing.LineScroll(TempCaratPosition - TempFirstLine);
			//if (TempFirstLine + 18 < TempNumberOfLines)
			//	m_TextEditing.LineScroll(1);
		}
	}
}

void CAlphaEntryDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 0;
	if (vChatMode)
	{
		TempHiddenKeys = 0;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			TempHiddenKeys = 2;
		PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
	}
	else
	{
		TempHiddenKeys = 8;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			TempHiddenKeys = 10;
		PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
	}
}

void CAlphaEntryDialog::ShowTouchScreenKeyboard(bool vShowKeyboard)
{
	if (vShowKeyboard)
	{
		//half screen text box
		if (vChatMode)
		{
			m_TextEditing.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 20),
				ResizeYCoor(cMainBackgroundTop + 15),ResizeXCoor(cMainBackgroundLength - 40),
				ResizeYCoor(cMainBackgroundHeight - 220),SWP_NOZORDER);

			vDoNotClickLowerY = (WORD) ResizeYCoor(210);
				
			//move background 
			m_Background.SetWindowPos(NULL, ResizeXCoor(cMainBackgroundLeft),//106
				ResizeYCoor(cMainBackgroundTop),ResizeXCoor(cMainBackgroundLength), //41, 423
				ResizeYCoor(cMainBackgroundHeight + 10),SWP_NOZORDER); //367

			m_DialogTitleStaticText2Control.ShowWindow(SW_HIDE);
			m_DialogTitleStaticText3Control.ShowWindow(SW_HIDE);
			m_DialogTitleStaticTextControl.ShowWindow(SW_HIDE);

		}
		m_Key0.ShowWindow(SW_HIDE);
		m_Key1.ShowWindow(SW_HIDE);
		m_Key2.ShowWindow(SW_HIDE);
		m_Key3.ShowWindow(SW_HIDE);
		m_Key4.ShowWindow(SW_HIDE);
		m_Key5.ShowWindow(SW_HIDE);
		m_Key6.ShowWindow(SW_HIDE);
		m_Key7.ShowWindow(SW_HIDE);
		m_Key8.ShowWindow(SW_HIDE);
		m_Key9.ShowWindow(SW_HIDE);
		m_KeyClear.ShowWindow(SW_HIDE);
		m_KeyHelp.ShowWindow(SW_HIDE);
	
		m_TextEditing.ShowWindow(SW_SHOW);
				
		m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction6Button, _T("Clear"));
		m_SubFunction6Button.ShowWindow(SW_SHOW);

		m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction2Button, _T("\nClear\nAll"));
		m_SubFunction2Button.ShowWindow(SW_SHOW);

		SetDlgItemText(IDC_SubFunction5Button, _T("Hide\nTouch Keyboard"));

		int TempKeyboardStartPosX, TempKeyboardStartPosY, TempKeyWidth, TempKeyHeight, TempCurrX, TempCurrY;
		TempKeyboardStartPosX = ResizeXCoor(118);
		TempKeyboardStartPosY = ResizeYCoor(210);
		TempKeyWidth = ResizeXCoor(40);
		TempKeyHeight = ResizeYCoor(40);
		
		TempCurrX = TempKeyboardStartPosX;
		TempCurrY = TempKeyboardStartPosY;
		m_KeyBoard1.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard2.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard3.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard4.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard5.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard6.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard7.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard8.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard9.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard10.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempKeyboardStartPosX;
		TempCurrY = TempCurrY + TempKeyHeight;
		m_KeyBoard11.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;

		m_KeyBoard12.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard13.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard14.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard15.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard16.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard17.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard18.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard19.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard20.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempKeyboardStartPosX;
		TempCurrY = TempCurrY + TempKeyHeight;
		m_KeyBoard21.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard22.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard23.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard24.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard25.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard26.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard27.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard28.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard29.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard30.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempKeyboardStartPosX;
		TempCurrY = TempCurrY + TempKeyHeight;
		m_KeyBoard31.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard32.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard33.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard34.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard35.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard36.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard37.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard38.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard39.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard40.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempKeyboardStartPosX;
		TempCurrY = TempCurrY + TempKeyHeight;
		m_KeyBoard41.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard42.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard43.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard44.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth * 4, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth * 4;
		m_KeyBoard45.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard46.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		TempCurrX = TempCurrX + TempKeyWidth;
		m_KeyBoard47.SetWindowPos(NULL, TempCurrX, TempCurrY, TempKeyWidth, TempKeyHeight, SWP_NOZORDER);
		//end set position

		//set text
		SetDlgItemText(IDC_KeyBoard1, _T("1"));
		SetDlgItemText(IDC_KeyBoard2, _T("2"));
		SetDlgItemText(IDC_KeyBoard3, _T("3"));
		SetDlgItemText(IDC_KeyBoard4, _T("4"));
		SetDlgItemText(IDC_KeyBoard5, _T("5"));
		SetDlgItemText(IDC_KeyBoard6, _T("6"));
		SetDlgItemText(IDC_KeyBoard7, _T("7"));
		SetDlgItemText(IDC_KeyBoard8, _T("8"));
		SetDlgItemText(IDC_KeyBoard9, _T("9"));
		SetDlgItemText(IDC_KeyBoard10, _T("0"));
		//SetDlgItemText(IDC_KeyBoard11, "Q");
		//SetDlgItemText(IDC_KeyBoard12, "W");
		//SetDlgItemText(IDC_KeyBoard13, "E");
		//SetDlgItemText(IDC_KeyBoard14, "R");
		//SetDlgItemText(IDC_KeyBoard15, "T");
		//SetDlgItemText(IDC_KeyBoard16, "Y");
		//SetDlgItemText(IDC_KeyBoard17, "U");
		//SetDlgItemText(IDC_KeyBoard18, "I");
		//SetDlgItemText(IDC_KeyBoard19, "O");
		//SetDlgItemText(IDC_KeyBoard20, "P");
		//SetDlgItemText(IDC_KeyBoard21, "A");
		//SetDlgItemText(IDC_KeyBoard22, "S");
		//SetDlgItemText(IDC_KeyBoard23, "D");
		//SetDlgItemText(IDC_KeyBoard24, "F");
		//SetDlgItemText(IDC_KeyBoard25, "G");
		//SetDlgItemText(IDC_KeyBoard26, "H");
		//SetDlgItemText(IDC_KeyBoard27, "J");
		//SetDlgItemText(IDC_KeyBoard28, "K");
		//SetDlgItemText(IDC_KeyBoard29, "L");
		//SetDlgItemText(IDC_KeyBoard31, "Z");
		//SetDlgItemText(IDC_KeyBoard32, "X");
		//SetDlgItemText(IDC_KeyBoard33, "C");
		//SetDlgItemText(IDC_KeyBoard34, "V");
		//SetDlgItemText(IDC_KeyBoard35, "B");
		//SetDlgItemText(IDC_KeyBoard36, "N");
		//SetDlgItemText(IDC_KeyBoard37, "M");
		SetDlgItemText(IDC_KeyBoard30, _T("Enter"));
		SetDlgItemText(IDC_KeyBoard38, _T("?"));
		SetDlgItemText(IDC_KeyBoard39, _T("!"));
		SetDlgItemText(IDC_KeyBoard40, _T(":"));
		m_KeyBoard41.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_KeyBoard41, _T("Cap\nLock"));
		SetDlgItemText(IDC_KeyBoard42, _T("Shift"));
		SetDlgItemText(IDC_KeyBoard43, _T("~"));
		SetDlgItemText(IDC_KeyBoard44, _T("Space"));
		SetDlgItemText(IDC_KeyBoard45, _T("\\"));
		SetDlgItemText(IDC_KeyBoard46, _T(","));
		SetDlgItemText(IDC_KeyBoard47, _T("."));
		//end set text

		//show keys
		m_KeyBoard1.ShowWindow(SW_SHOW);
		m_KeyBoard2.ShowWindow(SW_SHOW);
		m_KeyBoard3.ShowWindow(SW_SHOW);
		m_KeyBoard4.ShowWindow(SW_SHOW);
		m_KeyBoard5.ShowWindow(SW_SHOW);
		m_KeyBoard6.ShowWindow(SW_SHOW);
		m_KeyBoard7.ShowWindow(SW_SHOW);
		m_KeyBoard8.ShowWindow(SW_SHOW);
		m_KeyBoard9.ShowWindow(SW_SHOW);
		m_KeyBoard10.ShowWindow(SW_SHOW);
		m_KeyBoard11.ShowWindow(SW_SHOW);
		m_KeyBoard12.ShowWindow(SW_SHOW);
		m_KeyBoard13.ShowWindow(SW_SHOW);
		m_KeyBoard14.ShowWindow(SW_SHOW);
		m_KeyBoard15.ShowWindow(SW_SHOW);
		m_KeyBoard16.ShowWindow(SW_SHOW);
		m_KeyBoard17.ShowWindow(SW_SHOW);
		m_KeyBoard18.ShowWindow(SW_SHOW);
		m_KeyBoard19.ShowWindow(SW_SHOW);
		m_KeyBoard20.ShowWindow(SW_SHOW);
		m_KeyBoard21.ShowWindow(SW_SHOW);
		m_KeyBoard22.ShowWindow(SW_SHOW);
		m_KeyBoard23.ShowWindow(SW_SHOW);
		m_KeyBoard24.ShowWindow(SW_SHOW);
		m_KeyBoard25.ShowWindow(SW_SHOW);
		m_KeyBoard26.ShowWindow(SW_SHOW);
		m_KeyBoard27.ShowWindow(SW_SHOW);
		m_KeyBoard28.ShowWindow(SW_SHOW);
		m_KeyBoard29.ShowWindow(SW_SHOW);
		m_KeyBoard30.ShowWindow(SW_SHOW);
		m_KeyBoard31.ShowWindow(SW_SHOW);
		m_KeyBoard32.ShowWindow(SW_SHOW);
		m_KeyBoard33.ShowWindow(SW_SHOW);
		m_KeyBoard34.ShowWindow(SW_SHOW);
		m_KeyBoard35.ShowWindow(SW_SHOW);
		m_KeyBoard36.ShowWindow(SW_SHOW);
		m_KeyBoard37.ShowWindow(SW_SHOW);
		m_KeyBoard38.ShowWindow(SW_SHOW);
		m_KeyBoard39.ShowWindow(SW_SHOW);
		m_KeyBoard40.ShowWindow(SW_SHOW);
		m_KeyBoard41.ShowWindow(SW_SHOW);
		m_KeyBoard42.ShowWindow(SW_SHOW);
		m_KeyBoard43.ShowWindow(SW_SHOW);
		m_KeyBoard44.ShowWindow(SW_SHOW);
		m_KeyBoard45.ShowWindow(SW_SHOW);
		m_KeyBoard46.ShowWindow(SW_SHOW);
		m_KeyBoard47.ShowWindow(SW_SHOW);
		//end show
		
		if (vUpperCase)
			SetKeyBoardToUpperCase();
		else
			SetKeyBoardToLowerCase();

		SetCursor();
		DisplayColor();
		this->Invalidate();
	}
	else
	{
		if (vChatMode)
		{
			//set background to normal area
			m_Background.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft),//106
				ResizeYCoor(cMainBackgroundTop),ResizeXCoor(cMainBackgroundLength), //41, 423
				ResizeYCoor(cMainBackgroundHeight),SWP_NOZORDER); //367
			
			vDoNotClickLowerY = (WORD) ResizeYCoor(412);

			//set edit box to full screen
			m_TextEditing.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 20),
					ResizeYCoor(cMainBackgroundTop + 15),ResizeXCoor(cMainBackgroundLength - 40),
					ResizeYCoor(cMainBackgroundHeight - 30),SWP_NOZORDER); 
		}
				
		if (vChatMode)
		{
			m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction2Button, _T("\nClear\nAll"));
			m_SubFunction2Button.ShowWindow(SW_SHOW);
		}
		else
		if (vAllowParenthesis)
		{
			m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction1Button, _T("("));
			SetDlgItemText(IDC_SubFunction2Button, _T(")"));
		}
		else
		if (vAllowIPComma)
		{
			m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction2Button, _T(","));
		}

		CString TempText = " ";
		if (vAllowApproximate)
		{
			m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction2Button, _T("~"));
		}
		if ((vAllowSlashes) || (vAllowParenthesis))
		{
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			m_SubFunction1Button.ShowWindow(SW_SHOW);
		}

		m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (!vUpperCase)
		{
			m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText.LoadString(IDS_ChangeToUpperCase);
			SetDlgItemText(IDC_SubFunction6Button,TempText);

			WORD TempLoop = 0;
			for (;TempLoop < cNumberOfKeys; TempLoop++)
				vLocalKey[TempLoop] = cLocalLowerAlphaKey[TempLoop];
		}
		else
		{
			m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText.LoadString(IDS_ChangeToLowerCase);
			SetDlgItemText(IDC_SubFunction6Button,TempText);

			WORD TempLoop = 0;
			for (;TempLoop < cNumberOfKeys; TempLoop++)
				vLocalKey[TempLoop] = cLocalAlphaKey[TempLoop];
		}

		m_TextEditing.ShowWindow(SW_SHOW);

		m_SubFunction6Button.ShowWindow(SW_SHOW);

		SetDlgItemText(IDC_SubFunction5Button, _T("Show\nTouch Keyboard"));
		
		//start hide keys
		m_KeyBoard1.ShowWindow(SW_HIDE);
		m_KeyBoard2.ShowWindow(SW_HIDE);
		m_KeyBoard3.ShowWindow(SW_HIDE);
		m_KeyBoard4.ShowWindow(SW_HIDE);
		m_KeyBoard5.ShowWindow(SW_HIDE);
		m_KeyBoard6.ShowWindow(SW_HIDE);
		m_KeyBoard7.ShowWindow(SW_HIDE);
		m_KeyBoard8.ShowWindow(SW_HIDE);
		m_KeyBoard9.ShowWindow(SW_HIDE);
		m_KeyBoard10.ShowWindow(SW_HIDE);
		m_KeyBoard11.ShowWindow(SW_HIDE);
		m_KeyBoard12.ShowWindow(SW_HIDE);
		m_KeyBoard13.ShowWindow(SW_HIDE);
		m_KeyBoard14.ShowWindow(SW_HIDE);
		m_KeyBoard15.ShowWindow(SW_HIDE);
		m_KeyBoard16.ShowWindow(SW_HIDE);
		m_KeyBoard17.ShowWindow(SW_HIDE);
		m_KeyBoard18.ShowWindow(SW_HIDE);
		m_KeyBoard19.ShowWindow(SW_HIDE);
		m_KeyBoard20.ShowWindow(SW_HIDE);
		m_KeyBoard21.ShowWindow(SW_HIDE);
		m_KeyBoard22.ShowWindow(SW_HIDE);
		m_KeyBoard23.ShowWindow(SW_HIDE);
		m_KeyBoard24.ShowWindow(SW_HIDE);
		m_KeyBoard25.ShowWindow(SW_HIDE);
		m_KeyBoard26.ShowWindow(SW_HIDE);
		m_KeyBoard27.ShowWindow(SW_HIDE);
		m_KeyBoard28.ShowWindow(SW_HIDE);
		m_KeyBoard29.ShowWindow(SW_HIDE);
		m_KeyBoard30.ShowWindow(SW_HIDE);
		m_KeyBoard31.ShowWindow(SW_HIDE);
		m_KeyBoard32.ShowWindow(SW_HIDE);
		m_KeyBoard33.ShowWindow(SW_HIDE);
		m_KeyBoard34.ShowWindow(SW_HIDE);
		m_KeyBoard35.ShowWindow(SW_HIDE);
		m_KeyBoard36.ShowWindow(SW_HIDE);
		m_KeyBoard37.ShowWindow(SW_HIDE);
		m_KeyBoard38.ShowWindow(SW_HIDE);
		m_KeyBoard40.ShowWindow(SW_HIDE);
		m_KeyBoard39.ShowWindow(SW_HIDE);
		m_KeyBoard41.ShowWindow(SW_HIDE);
		m_KeyBoard42.ShowWindow(SW_HIDE);
		m_KeyBoard43.ShowWindow(SW_HIDE);
		m_KeyBoard44.ShowWindow(SW_HIDE);
		m_KeyBoard45.ShowWindow(SW_HIDE);
		m_KeyBoard46.ShowWindow(SW_HIDE);
		m_KeyBoard47.ShowWindow(SW_HIDE);
		//end hide keys
		if (!vChatMode)
		{
			m_Key0.ShowWindow(SW_SHOW);
			m_Key1.ShowWindow(SW_SHOW);
			m_Key2.ShowWindow(SW_SHOW);
			m_Key3.ShowWindow(SW_SHOW);
			m_Key4.ShowWindow(SW_SHOW);
			m_Key5.ShowWindow(SW_SHOW);
			m_Key6.ShowWindow(SW_SHOW);
			m_Key7.ShowWindow(SW_SHOW);
			m_Key8.ShowWindow(SW_SHOW);
			m_Key9.ShowWindow(SW_SHOW);
			m_KeyClear.ShowWindow(SW_SHOW);
			m_KeyHelp.ShowWindow(SW_SHOW);
		}

		SetCursor();
		if (vChatMode)
			DisplayColor();
		this->Invalidate();
	}
}

void CAlphaEntryDialog::ProcessAlphaNumericKey(char TempKey)
{
	WORD TempLength = vEditString.GetLength();

	if (vDeleteAll)
	{
		vEditString = "";
		vDeleteAll = false;
		TempLength = vEditString.GetLength();
	}

	if (((TempLength > 32) && (!vAllowMoreThan32Characters) && (!vChatMode)) ||
		(TempLength > 9998))
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText.LoadString(IDS_32CharactersMaximum);
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((vMaximumNumberOfCharacters) && (TempLength >= vMaximumNumberOfCharacters))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nOnly " + dtoa(vMaximumNumberOfCharacters,0) + " characters allowed.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		//handle vUpperCase vCapLocks
		if (vOnScreenKeyBoardPress) //if key press came from onscreen keyboard
		if (((!vUpperCase) && (!vCapLock)) && (TempKey > 64) && (TempKey < 91)) //if key is lower case
		{
			TempKey = TempKey + 32; //set lower case
		}
		
		if (vUpperCase)	//if is upper case and cap lock is not on, change keyboard to lower case
		if (!vCapLock)
		{
			vUpperCase = false;
			SetKeyBoardToLowerCase();
		}

		if ((vChatMode) || (vFindTextMode) || (vOnScreenKeyBoardPress) || ((TempKey != 92) && ((TempKey != 126) || (PasswordOK(cTemporaryInspxPassword, false))) && 
			(TempKey != 44) && (TempKey != 46) && (TempKey != 58) && (TempKey != 10) && (TempKey != 33) && (TempKey != 63)))
		{
			WORD TempLength = vEditString.GetLength();
			vEditString.Insert(vCursorPosition, (CHAR)TempKey);

			if (vCursorPosition < TempLength)
				if (vColor)
					for (WORD TempLoop = TempLength; TempLoop > vCursorPosition + 1; TempLoop--)
						if (TempLoop + 1 < 10000)
							vColor[TempLoop + 1] = vColor[TempLoop];

			vCursorPosition++;
			vLastKeyTimes = 0;
			vLastKey = 0;
			SetCursor();

			if (vChatMode)
			if (vColor)
			{ //key board entry
				if (vCursorPosition < 10000)
				{
					if (vOnScreenKeyBoardPress)
					{
						vColor[vCursorPosition] = cBlue;
						vOnScreenKeyBoardPress = false;
					}
					else
						vColor[vCursorPosition] = cRed;
				}

				DisplayColor();
			}
		}
	}
	vOnScreenKeyBoardPress = false;
}
//ProcessAlphaNumericKey((char)pMsg->wParam);

void CAlphaEntryDialog::OnKeyboard1()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('1');
}


void CAlphaEntryDialog::OnKeyboard2()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('2');
}


void CAlphaEntryDialog::OnKeyboard3()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('3');
}


void CAlphaEntryDialog::OnKeyboard4()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('4');
}


void CAlphaEntryDialog::OnKeyboard5()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('5');
}


void CAlphaEntryDialog::OnKeyboard6()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('6');
}


void CAlphaEntryDialog::OnKeyboard7()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('7');
}


void CAlphaEntryDialog::OnKeyboard8()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('8');
}


void CAlphaEntryDialog::OnKeyboard9()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('9');
}


void CAlphaEntryDialog::OnKeyboard10()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey('0');
}


void CAlphaEntryDialog::OnKeyboard11()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('Q');
	}
}

void CAlphaEntryDialog::OnKeyboard12()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('W');
	}
}


void CAlphaEntryDialog::OnKeyboard13()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('E');
	}
}

void CAlphaEntryDialog::OnKeyboard14()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('R');
	}
}


void CAlphaEntryDialog::OnKeyboard15()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('T');
	}
}


void CAlphaEntryDialog::OnKeyboard16()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('Y');
	}
}


void CAlphaEntryDialog::OnKeyboard17()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('U');
	}
}


void CAlphaEntryDialog::OnKeyboard18()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('I');
	}
}


void CAlphaEntryDialog::OnKeyboard19()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('O');
	}
}


void CAlphaEntryDialog::OnKeyboard20()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('P');
	}
}


void CAlphaEntryDialog::OnKeyboard21()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('A');
	}
}


void CAlphaEntryDialog::OnKeyboard22()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('S');
	}
}


void CAlphaEntryDialog::OnKeyboard23()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('D');
	}
}


void CAlphaEntryDialog::OnKeyboard24()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('F');
	}
}


void CAlphaEntryDialog::OnKeyboard25()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('G');
	}
}


void CAlphaEntryDialog::OnKeyboard26()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('H');
	}
}


void CAlphaEntryDialog::OnKeyboard27()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('J');
	}
}


void CAlphaEntryDialog::OnKeyboard28()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('K');
	}
}


void CAlphaEntryDialog::OnKeyboard29()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('L');
	}
}


void CAlphaEntryDialog::OnKeyboard30()
{
	if (vAcceptAllKeys)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('\n');
	}
}


void CAlphaEntryDialog::OnKeyboard31()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('Z');
	}
}


void CAlphaEntryDialog::OnKeyboard32()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('X');
	}
}


void CAlphaEntryDialog::OnKeyboard33()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('C');
	}
}


void CAlphaEntryDialog::OnKeyboard34()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('V');
	}
}


void CAlphaEntryDialog::OnKeyboard35()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('B');
	}
}


void CAlphaEntryDialog::OnKeyboard36()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('N');
	}
}


void CAlphaEntryDialog::OnKeyboard37()
{
	if (!vNumbersOnly)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('M');
	}
}


void CAlphaEntryDialog::OnKeyboard38()
{
	if (vAcceptAllKeys)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('?');
	}
}


void CAlphaEntryDialog::OnKeyboard39()
{
	if (vAcceptAllKeys)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('!');
	}
}

/*
SetDlgItemText(IDC_KeyBoard38, "?");
		SetDlgItemText(IDC_KeyBoard39, "!");
		SetDlgItemText(IDC_KeyBoard40, ":");
		SetDlgItemText(IDC_KeyBoard41, "CAP");
		SetDlgItemText(IDC_KeyBoard42, "SHFT");
		SetDlgItemText(IDC_KeyBoard43, "~");
		SetDlgItemText(IDC_KeyBoard44, "SPACE");
		SetDlgItemText(IDC_KeyBoard45, "\\");
		SetDlgItemText(IDC_KeyBoard46, ",");
		SetDlgItemText(IDC_KeyBoard47, ".");
		*/
void CAlphaEntryDialog::OnKeyboard40()
{
	if (vAcceptAllKeys)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey(':');
	}
}


void CAlphaEntryDialog::OnKeyboard41()
{
	if ((!vCapLock) && (!vUpperCase))
	{
		vUpperCase = true;
		vCapLock = true;
		SetKeyBoardToUpperCase();
	}
	else
	{
		vUpperCase = false;
		vCapLock = false;
		SetKeyBoardToLowerCase();
	}
	CString TempText = " ";
	if (!vUpperCase)
	{
		WORD TempLoop = 0;
		for (;TempLoop < cNumberOfKeys; TempLoop++)
			vLocalKey[TempLoop] = cLocalLowerAlphaKey[TempLoop];
	}
	else
	{
		WORD TempLoop = 0;
		for (;TempLoop < cNumberOfKeys; TempLoop++)
			vLocalKey[TempLoop] = cLocalAlphaKey[TempLoop];
	}
}


void CAlphaEntryDialog::OnKeyboard42()
{
	if (!vUpperCase)
	{
		vUpperCase = true;
		SetKeyBoardToUpperCase();
	}
	else
	{
		vUpperCase = false;
		vCapLock = false;
		SetKeyBoardToLowerCase();
	}
}


void CAlphaEntryDialog::OnKeyboard43()
{
	if (vAllowApproximate)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('~');
	}
}


void CAlphaEntryDialog::OnKeyboard44()
{
	vOnScreenKeyBoardPress = true;
	ProcessAlphaNumericKey(' ');
}


void CAlphaEntryDialog::OnKeyboard45()
{
	if (vAcceptAllKeys)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('\\');
	}
}


void CAlphaEntryDialog::OnKeyboard46()
{
	if (vAllowComma)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey(',');
	}
}


void CAlphaEntryDialog::OnKeyboard47()
{
	if (vAllowPeriod)
	{
		vOnScreenKeyBoardPress = true;
		ProcessAlphaNumericKey('.');
	}
}



void CAlphaEntryDialog::SetKeyBoardToUpperCase()
{
		SetDlgItemText(IDC_KeyBoard11, _T("Q"));
		SetDlgItemText(IDC_KeyBoard12, _T("W"));
		SetDlgItemText(IDC_KeyBoard13, _T("E"));
		SetDlgItemText(IDC_KeyBoard14, _T("R"));
		SetDlgItemText(IDC_KeyBoard15, _T("T"));
		SetDlgItemText(IDC_KeyBoard16, _T("Y"));
		SetDlgItemText(IDC_KeyBoard17, _T("U"));
		SetDlgItemText(IDC_KeyBoard18, _T("I"));
		SetDlgItemText(IDC_KeyBoard19, _T("O"));
		SetDlgItemText(IDC_KeyBoard20, _T("P"));
		SetDlgItemText(IDC_KeyBoard21, _T("A"));
		SetDlgItemText(IDC_KeyBoard22, _T("S"));
		SetDlgItemText(IDC_KeyBoard23, _T("D"));
		SetDlgItemText(IDC_KeyBoard24, _T("F"));
		SetDlgItemText(IDC_KeyBoard25, _T("G"));
		SetDlgItemText(IDC_KeyBoard26, _T("H"));
		SetDlgItemText(IDC_KeyBoard27, _T("J"));
		SetDlgItemText(IDC_KeyBoard28, _T("K"));
		SetDlgItemText(IDC_KeyBoard29, _T("L"));
		SetDlgItemText(IDC_KeyBoard31, _T("Z"));
		SetDlgItemText(IDC_KeyBoard32, _T("X"));
		SetDlgItemText(IDC_KeyBoard33, _T("C"));
		SetDlgItemText(IDC_KeyBoard34, _T("V"));
		SetDlgItemText(IDC_KeyBoard35, _T("B"));
		SetDlgItemText(IDC_KeyBoard36, _T("N"));
		SetDlgItemText(IDC_KeyBoard37, _T("M"));
}

void CAlphaEntryDialog::SetKeyBoardToLowerCase()
{

		SetDlgItemText(IDC_KeyBoard11, _T("q"));
		SetDlgItemText(IDC_KeyBoard12, _T("w"));
		SetDlgItemText(IDC_KeyBoard13, _T("e"));
		SetDlgItemText(IDC_KeyBoard14, _T("r"));
		SetDlgItemText(IDC_KeyBoard15, _T("t"));
		SetDlgItemText(IDC_KeyBoard16, _T("y"));
		SetDlgItemText(IDC_KeyBoard17, _T("u"));
		SetDlgItemText(IDC_KeyBoard18, _T("i"));
		SetDlgItemText(IDC_KeyBoard19, _T("o"));
		SetDlgItemText(IDC_KeyBoard20, _T("p"));
		SetDlgItemText(IDC_KeyBoard21, _T("a"));
		SetDlgItemText(IDC_KeyBoard22, _T("s"));
		SetDlgItemText(IDC_KeyBoard23, _T("d"));
		SetDlgItemText(IDC_KeyBoard24, _T("f"));
		SetDlgItemText(IDC_KeyBoard25, _T("g"));
		SetDlgItemText(IDC_KeyBoard26, _T("h"));
		SetDlgItemText(IDC_KeyBoard27, _T("j"));
		SetDlgItemText(IDC_KeyBoard28, _T("k"));
		SetDlgItemText(IDC_KeyBoard29, _T("l"));
		SetDlgItemText(IDC_KeyBoard31, _T("z"));
		SetDlgItemText(IDC_KeyBoard32, _T("x"));
		SetDlgItemText(IDC_KeyBoard33, _T("c"));
		SetDlgItemText(IDC_KeyBoard34, _T("v"));
		SetDlgItemText(IDC_KeyBoard35, _T("b"));
		SetDlgItemText(IDC_KeyBoard36, _T("n"));
		SetDlgItemText(IDC_KeyBoard37, _T("m"));
}