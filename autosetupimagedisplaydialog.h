//ScanTrac Side View Source File
#if !defined(AFX_AUTOSETUPIMAGEDISPLAYDIALOG_H__19E8EBE1_F162_11D4_AD38_00500466E305__INCLUDED_)
#define AFX_AUTOSETUPIMAGEDISPLAYDIALOG_H__19E8EBE1_F162_11D4_AD38_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoSetupImageDisplayDialog.h : header file
//

#include "Inspection.h"

enum {cCenterExposureMenu, //cEdgeExposureMenu, 
cSetImageSize, cLearnMenu, cSetROI, cSelectEjectors,
	cSetContaminantThreshold, cSetVoidThreshold, cSetDensityThreshold,cSetInspections,
	cSetBackground,cSetSizeSettings, cInitializing, CMultiLanePositionAdjustment};
enum {cNewAutoSetup,cEditIPAutoSetup,cProductAutoSetup,cEvaluateAutoSetup,cEditSizeAutoSetup,
	cEditExposureAutoSetup,cEditInspectionAutoSetup,cCopyAutoSetup};
/////////////////////////////////////////////////////////////////////////////
// CAutoSetupImageDisplayDialog dialog

class CAutoSetupImageDisplayDialog : public CDialog
{
public:
	//variables
	//bool vAskedIfShouldRemoveBackground;
	BYTE vMultiImageLaneEditing;
	WORD vMaximumLaneWidth;
	bool vGaveSendAContainerNotice;
	WORD vMouseClickCount;
	bool vEditOnlyThisInspection;
	WORD vOldXRayState;
	BYTE vOldCalibrationImageNumber;
	WORD vOldImageNumberToDisplay;
	bool vAdjustingBottomEdgeFinding;
	BYTE vBlink;
	//bool vShowingPipeEmpty;
	WORD vOldActualEncoderRate;
	BYTE vOldPipeIsEmpty;
	BYTE vOldAirBubblesInPipe;
	DWORD vMaximumUnderfillByWeightThreshold;
	DWORD vMaximumThreshold;
	bool vAutosizedContainer;
	bool vChangedEdgeFindingMode;
	bool vChangedPurpleLine;
	bool vToggleSign;
	bool vOldAdjustingBrightnessBeforeLearning;
	bool vOldGlobalPasswordOK;
	bool vInitialized;
	bool vMakeStatusLineYellowRed;
	bool vNeedToAutoSizeNextContainer;
	CString vDialogTitleString;
	//WORD vOldUnderRemoteControl;
	BYTE vProcessCount;
	bool vJustFinishedProcessingQuickLearn;
	bool vHaveAnAutoThresholdValue;
	bool vWaitingForBackgroundImage;
	BYTE vShowingBackground;
	bool vPickOneToEvaluate;
	BYTE vInspectionNumberEditing;
	//bool vUseLowExposureModeInAutoSize;
	bool vHaveScaledImageToPaint;
	bool vNeedToCalibrateDetectors;
	bool vFreezeImageOnScreen;
	bool vOldDriftCompensationLocked;
	bool vShowReturnToEditMenu;
	bool vNeedToConfigureInspections;
	//used to save last value written to know if should be yellow or red
	double vCurrentDensity;
	double vAverageDensity;  
	bool vReturnToSetSource;
	int vOldDriftCompensationADCOffset;
	int vOldDriftCompensationDACOffset;
	bool vOldTakingQuickLearnImages;
	DWORD vTotalToLearn;
	bool vLearningReference;
	BYTE vNumberEjectorsConfigured;
	BYTE vOnlyEjectorNumberConfigured;
	bool vDrawPurpleLine;
	bool vCollectingBadDensityAverage;
	bool vTookNewDensityData;
	CInspection *vInspectionEditing;
	bool vOldFindGoodIntensity;
	bool vOldVoidFindGoodIntensity;
	bool vOldFindBadIntensity;
	bool vOldVoidFindBadIntensity;
	BYTE vExitCode;
	bool vAnyChangeMade;
	CString vOldErrorMessage;
	tProductCollection *vLocalProductCollection;
	CString vCurrentStatusLineText;
	bool vOldButton2Yellow;
	bool vOldCollectingDensityAverage;
	double vOldDensityAverageValue;
	BYTE vAutoSetupType;
	bool vShowAlternateImage;
	bool vStartingUpSource;
	bool vRestartingXRaysWithNewSettings;
	bool vSavedImageProcessingData;
	double vOldMaximumBadAverage ;
	double vOldMaximumBadStandardDeviation;
	double vOldMinimumGoodStandardDeviation;
	double vOldMinimumGoodAverage;
	double vOldQualityFactor;
	double vPercentBeltSpeed;
	bool vStartedSource;
	double vOldMaximumBadIntensity;
	double vOldMinimumGoodIntensity;
	DWORD vOldMaximumBadCount;
	DWORD vOldMinimumGoodCount;
	bool vShowImageInColor;
	float vXScale;
	float vYScale;
	WORD vXOffset;
	WORD vYOffset;
	//bool vNeedToSetSize;
	BYTE vNeedToSetContainerThresholdBounds;
	bool vHaveNotSetSize;
	double vImageMargin;
	double vMaxDetectorHeight;
	bool vHaveAnImage;
	BYTE vMode;
	bool vOldCalibrateBodyTriggerReferenceEnabled;
	bool vOldAverageEnabled;
	bool vOldStructureEnabled;
	double vAutomaticDensityThresholdUpper;
	double vAutomaticDensityThresholdLower;
	CString vProductEditingName;
	BYTE vOldSystemRunMode;

	bool vShowAverage;
	bool vShowStructure;
	bool vShowVoidStructure;
	//bool vShowGlassStructure;
	bool vShowContaminant2Structure;
	bool vShowHistogram;
	//bool vShowGlassHistogram;
	double vSensitivity;
	HBRUSH vLightGrayBrush;
	CBitmap vGraphicBitmap;

	bool vOldXRayOnNotice;
	bool vChangeMade;
	unsigned long int vUpdateDisplayTimerHandle;
  unsigned long int vOneSecondTimerHandle;
	unsigned long int vStartTimerHandle;
	unsigned long int vSetupTimerHandle;
	unsigned long int vStartSetupTimerHandle;
	unsigned long int vCheckKeySwitchTimerHandle;
	unsigned long int vWaitToUpdateScreenTimerHandle;
	unsigned long int vWaitToUpdateScreenLocationEdgeTimerHandle;
	unsigned long int vWaitUntilDoneRunningForExitTimer;
	unsigned long int vWaitUntilDoneRunningForReloadTimer;
	unsigned long int vGrabFocusTimerHandle;
	unsigned long int vGetBackgroundImageTimerHandle;
	unsigned long int vRestoreStatusLineTimerHandle;
	unsigned long int vClearStartingIndicatorIfXRaysOnTimerHandle;
	unsigned long int vMouseDownRepeatTimerHandle;
	WORD vAutoSetupSensitivityLeft;
	WORD vAutoSetupSensitivityBottom;
	WORD vAutoSetupSensitivityHeight;
	WORD vAutoSetupTop;
	WORD vAutoSetupRight;
	WORD vAutoSetupSensitivityWidth;
	WORD vAutoSetupSensitivityRight;
	WORD vAutoSetupLearnSensitivityBottom;
	WORD vAutoSetupSensitivityTop;

	bool vWaitingToUpdateScreen;
	bool vRecievedButtonPress;	//do not repeat click and wait for mouse button up if you used keyboard down 
	BYTE vHoldImageDisplayOnlyRejects;
	BYTE vMouseState;		//1 - 8 is left mouse button clicked down, 9 - 16 is left mouse button double click, 0 is idle or all functions completed
	CProduct *vHoldCurrentProduct;
	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	bool vFunction1ButtonEnable;
	bool vFunction2ButtonEnable;
	bool vFunction3ButtonEnable;
	bool vFunction4ButtonEnable;
	bool vFunction5ButtonEnable;

	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;
	bool vSubFunction8ButtonEnable;

// Construction
	CAutoSetupImageDisplayDialog(CWnd* pParent = NULL);   // standard constructor

  //methods
	void DrawImageOnScreen();
	void OneSecondUpdate();
	void SetChangeMade();
	bool AskIfAbortAutoSetupDialog();
	void ExitAutoSetupDialog(int TempExitCode);
	void UpdateButtonText();
	//void ShowReferenceData();
	void ProcessTheImage();
	void ChangedProductSettingsSoReload();
	bool FinishLearnMenu(bool TempWarn);
	void StartContaminantThreshold();
	bool FinishContaminantThreshold(bool TempWarn);
	void StartVoidThreshold();
	void StartThreshold();
	void InitializeThresholdValues();
	bool FinishSizeMenu(bool TempWarn, bool TempExiting);
	void FinishThreshold();
	void StartCenterExposureMenu();
	//void StartEdgeExposureMenu();
	void StartLearnMenu();
	bool FinishCenterExposureMenu(bool TempWarn);
	//bool FinishEdgeExposureMenu(bool TempWarn);
	void FinishBackground();
	void StartBackground();
	void StartSizeMenu();
	bool FinishVoidThreshold(bool TempWarn);
	void SetROILines();
	void StartROI();
	void FinishROI(bool TempExitingSetup);
	void StartDensityThreshold();
	bool FinishDensityThreshold(bool TempWarn);
	void StartAccumulatingStructure();
	void StartAccumulatingAverage();
	//void SaveHistogramDataToFile();
	bool HaveAVoidStructure(); 
	bool HaveAContaminantStructure();
	void ShowMaximumConveyorSpeed();
	void DisplayWithPassword();
	//void DisplayScale();
	void WaitThenDrawImageOnScreen();
	void ClearYellowWarningButton();
	void UpdateFunction3Button();
	void SetStatusLine(CString TempStatusLineText);
	bool MyAbortSetupChanges();
	void WaitDisplayLocationEdge();
	void DisplayLocationEdge();
	void DoneRunningSoExit();
	void DoneRunningSoReload();
	void WriteCSVFiles();
	void ConfigureInspections(bool TempSelectForEvaluate, bool TempEditROI);
	void SelectInspectionEjectors(bool TempGoingForward);
	//void SetInspectionROIToThisImageThreshold(
	//			CImageData *TempHistogramImage, CInspection *TempInspection);
	void EditNotes();
	bool NoSetupOrOKToChange();
	bool NoSetupOrOKToMinorChange();
	//double ConvertMaxConveyorSpeedToIntegrationTime(double TempMaxConveyorSpeed);
	double ConvertIntegrationTimeToMaxConveyorSpeed(WORD TempIntegrationCode);
	void YouCantChangeSettingBecauseInspecting();
	void StartingSystemPleaseTryAgain();
	void RestoreOriginalProductSettings();
	void AutoSizeImage();
	void DisplayCurrentDensityValue();
	void UpdateAverageDensityDisplay();
	void CheckDensityCompensationInspection();
	void CheckIfShouldLockDensityFeedbackLoop();
	void ClearDensityFeedBackLoop();
	void TakeBackgroundImageSample();
	//void LoadBackGroundIfNeeded();
	void CalculateContainerThresholdBounds();
	void StartSetSizeSettings();
	void FinishSetSizeSettings();
	void CheckPurpleLineHeight();
	//void SetDefaultCheckPurpleLineHeight();
	void SetInspectionROI(CInspection *TempInspection);
	void DisplayDensityThresholdLower();
	void DisplayDensityThresholdUpper();
	void DisplayAutoDensityThresholdUpper();
	void DisplayAutoDensityThresholdLower();
	void DisplayMinimumGoodMaximumBadDataValues(bool TempAlreadyUpdatedStatusLine);
	void StartLearning();
	//void DisplayEdgeExposureTimePercent();
	void GiveMessageAlreadyAtThisValue(BYTE TempWhichOne);
	void ManuallySetWidth(double TempNewWidth);
	//void ShowPipeEmptyIndicator();
	void ShowSensitivity();
	void ShowLearnSensitivity();
	void ClearLearnSensitivity();
	void ClearSensitivity();
	void EnterNominalWeight();
	void GiveLinkedInspectionNotice();
	void AskForWeightTrendingInspectionToLinkThisOneTo(BYTE TempThisInspection);
	bool DeleteCheckWeighCalibrations(bool TempChangingROI);
	void ClearEvaluate();
	void GiveSendGoodContainersNotice();
	void DisplayANewImage();
	void CalculateLearnedReferenceValues();
	void ShowNoMouseClickDialogMessage();
	void StartMultiLanePositionMenu();
	void FinishMultiLanePositionMenu();
	void ClearReferenceLocationData();
// Dialog Data
	//{{AFX_DATA(CAutoSetupImageDisplayDialog)
	enum { IDD = IDD_AutoSetupImageDisplayDialog };
	CStatic	m_DialogTitleStaticText;
	CStatic	m_RejectReason;
	CStatic	m_DensityNotLockedIndicator;
	CDownFunctionButton	m_Function1Button;
	CStatic	m_SubFunction4Display;
	CStatic	m_CurrentDensity;
	CStatic	m_AverageDensity;
	CStatic	m_SizeY;
	CStatic	m_SizeX;
	CStatic	m_SizeLabelY;
	CStatic	m_SizeLabelX;
	CStatic	m_XScale2;
	CStatic	m_XScaleLabel;
	CStatic	m_YScale2;
	CStatic	m_YScaleLabel;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_NextArrow;
	CStatic	m_PreviousArrowGreenBack;
	CStatic	m_NextArrowGreenBack;
	CStatic	m_PreviousArrow;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_SubFunction8Button3;
	CStatic	m_SubFunction8Button2;
	CStatic	m_SubFunction8Button1;
	CStatic	m_SubFunction6Button3;
	CStatic	m_SubFunction6Button2;
	CStatic	m_SubFunction6Button1;
	CStatic	m_Calibrating;
	CStatic	m_SubFunction2Display;
	CStatic	m_Reference;
	CStatic	m_SubFunction3Display;
	CStatic	m_SubFunction1Display;
	CStatic	m_IntegrateTime2;
	CStatic	m_IntegrateTimeLabel2;
	CStatic	m_ThresholdForContainerBounds2;
	CStatic	m_Width2;
	CStatic	m_IntegrateTimeLabel;
	CStatic	m_CurrentLabel;
	CStatic	m_Background;
	CDownFunctionButton	m_Function5Button;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CStatic	m_SubFunction8Display;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CStatic	m_SubFunction4DisplayA;
	CDownFunctionButton	m_Function4Button;
	CStatic	m_SubFunction5Display;
	CStatic	m_SubFunction7Display;
	CStatic	m_SubFunction6Display;
	CStatic	m_XRaysOn;
	CStatic m_PleaseWait;
	CStatic m_Simulating;
	CStatic m_StatusLine;
	CStatic m_StatusLine2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoSetupImageDisplayDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoSetupImageDisplayDialog)
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSubFunction8Button3();
	afx_msg void OnSubFunction8Button2();
	afx_msg void OnSubFunction8Button1();
	afx_msg void OnSubFunction6Button3();
	afx_msg void OnSubFunction6Button2();
	afx_msg void OnSubFunction6Button1();
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDialogTitleStaticText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnSubFunction2ButtonDoubleClicked();
	afx_msg void OnSubFunction1ButtonDoubleClicked();
	afx_msg void OnSubFunction3ButtonDoubleClicked();
	afx_msg void OnSubFunction4ButtonDoubleClicked();
	afx_msg void OnSubFunction5ButtonDoubleClicked();
	afx_msg void OnSubFunction6ButtonDoubleClicked();
	afx_msg void OnSubFunction7ButtonDoubleClicked();
	afx_msg void OnSubFunction8ButtonDoubleClicked();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOSETUPIMAGEDISPLAYDIALOG_H__19E8EBE1_F162_11D4_AD38_00500466E305__INCLUDED_)
