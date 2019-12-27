#pragma once
#include "afxwin.h"
//ScanTrac Side View Source File

enum {cSizeMenu, cExposureMenu, cEvaluateMenu, cInitializingDisplay, cFindingBelowStaturatedImage, cFindingjustStaturatedImage, cTestEjectorsMenu};

// CTDAutoSetupImageDisplayDialog dialog

class CTDAutoSetupImageDisplayDialog : public CDialog
{
	DECLARE_DYNAMIC(CTDAutoSetupImageDisplayDialog)

public:
	//variables
	tProductCollection *vLocalProductCollection;
	CString vProductEditingName;
	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	WORD vOldImageNumberToDisplay;
	WORD vOldXRayState;
	BYTE vOldCalibrationImageNumber;
	BYTE vGaveTooDarkWarning;
	bool vStartedResize;
	bool vMakeStatusLineGreen;
	bool vExposureIsCorrect;
	byte vSanityCheckFailureCount;
	bool vIntegrationTimeSet;
	bool vSamplingConveyorSpeed;
	BYTE vAdjustedExposureForAutoSize;
	bool vAdjustingImageBrightnessBelowSaturation;
	bool vAdjustingImageBrightnessJustAboveSaturation;
	bool vGotImageToAutoSize;
	int vAdjustBrightnessFrameCount;
	int vAdjustBrightnessDotCount;
	CString vCurrentStatusLineText;
	BYTE vMode;
	bool vHaveAnImage;
  unsigned long int vOneSecondTimerHandle;
  unsigned long int vCheckKeySwitchTimerHandle;
	unsigned long int vStartSetupTimerHandle;
	unsigned long int vGrabFocusTimerHandle;
	unsigned long int vWaitUntilDoneRunningForReloadTimer;
	unsigned long int vWaitUntilDoneRunningForExitTimer;
	unsigned long int vSetupTimerHandle;
	unsigned long int vWaitToUpdateScreenTimerHandle;
	unsigned long int vStartTimerHandle;
	unsigned long int vShowStatusLineTimerHandle;
	unsigned long int vCalculateIntegrationTimerHandle;
	unsigned long int vSimulateContainerAfterXRaysAreOnTimerHandle;
	bool vStartingUpSource;
	bool vRestartingXRaysWithNewSettings;
	float vXScale;
	float vYScale;
	bool vStartedSource;
	bool vInitialized;
	CString vDialogTitleString;
	BYTE vHoldImageDisplayOnlyRejects;
	BYTE vExitCode;
	bool vMakeStatusLineYellowRed;
	bool vNeedToAutoSizeNextContainer;
	double vMaxDetectorHeight;
	bool vHaveScaledImageToPaint;
	bool vWaitingToUpdateScreen;
	//WORD vOldUnderRemoteControl;
	bool vOldButton2Yellow;
	CString vOldErrorMessage;
	double vPercentBeltSpeed;
	bool vOldXRayOnNotice;
	double vImageMargin;
	WORD vXOffset;
	WORD vYOffset;
	WORD vOldQuickLearnImageIndex;
	BYTE vProcessingEvaluateImagesStep;
	bool vEvaluationComplete;
	bool vEjectorUsed[cNumberOfEjectors];
	BYTE vOldTestEjectNextContainer;
	BYTE vGaveTurnOnConveyorMessage;
		//constructor
	CTDAutoSetupImageDisplayDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTDAutoSetupImageDisplayDialog();

	//methods
	void SetStatusLine(CString TempStatusLineText, WORD TempBlink, bool TempMakeGreen);
	void DrawImageOnScreen();
	void UpdateSubFunction4Button();
	void ExitAutoSetupDialog(int TempExitCode);
	void ChangedProductSettingsSoReload();
	void DoneRunningSoReload();
	void AutoSizeImage();
	void StartSizeMenu();
	bool FinishSizeMenu(bool TempWarn, bool TempExiting);
	void DoneRunningSoExit();
	//void WaitThenDrawImageOnScreen();
	void StartExposureMenu();
	bool FinishExposureMenu();
	void StartEvaluateMenu();
	void FinishEvaluateMenu();
	void ClearYellowWarningButton();
	void OneSecondUpdate();
	void AutoAdjustExposure();
	void CalculateBestIntegrationTime();
	void StartTestEjectorsMenu();
	void UpdateButtons();
	void TestEjectNextContainer(BYTE TempWaitCount, BYTE TempEjectorNumber);


// Dialog Data
	enum {IDD = IDD_TDAutoSetupImageDisplayDialog};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	// Generated message map functions
	//{{AFX_MSG(CTDAutoSetupImageDisplayDialog)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_Calibrating;
	CStatic m_XRaysOn;
	CStatic m_Background;
	CRightFunctionButton m_SubFunction8Button;
	CDownFunctionButton m_Function3Button;
	CLeftFunctionButton m_SubFunction1Button;
	CStatic m_Width2;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CDownFunctionButton m_Function5Button;
	CStatic m_SubFunction4Display;
	CStatic m_SubFunction8Display;
	CStatic m_SubFunction7Display;
	CRightFunctionButton m_SubFunction5Button;
	CStatic m_StatusLine;
	CLeftFunctionButton m_SubFunction4Button;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function4Button;
	CStatic m_Reference;
	CStatic m_RejectReason;
	CStatic m_Simulating;
	CStatic m_StatusLine2;
	CStatic m_SubFunction1Display;
	CLeftFunctionButton m_SubFunction2Button;
	CStatic m_SubFunction2Display;
	CLeftFunctionButton m_SubFunction3Button;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction5Display;
	CStatic m_ThresholdForContainerBounds2;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_SubFunction6Display;
	CRightFunctionButton m_SubFunction6Button;
	afx_msg void OnDialogtitlestatictext1();
};
