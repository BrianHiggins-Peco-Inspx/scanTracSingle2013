//ScanTrac Side View Source File
#if !defined(AFX_NEWIMAGEDISPLAYDIALOG_H__ABEC9731_069F_11D4_ABE4_00500466E305__INCLUDED_)
#define AFX_NEWIMAGEDISPLAYDIALOG_H__ABEC9731_069F_11D4_ABE4_00500466E305__INCLUDED_

#include "SystemConfigurationData.h"
#include "Inspection.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNewImageDisplayDialog dialog
enum {cShowTypeContaminant, cShowTypeVoid, cShowTypeDensity, cShowTypeKernel, cShowTypeContaminant2, cShowTypeContaminant2NoDilation};

class CNewImageDisplayDialog : public CDialog
{
public:
	//variables
	BYTE vTotalNumberOfInspectionsToDisplay;
	double vSensitivity;
	DWORD vSizeOfLargestPerimeter;
	DWORD vMaximumUnderfillByWeightThreshold;
	DWORD vMaximumThreshold;
	WORD vOldAddToLearnCount;
	bool vHideRedIndicator;
	BYTE *vProcessedRedImage;
	WORD vPlotLine;
	BYTE vShowLineDerivative;
	BYTE vTakeDerivative;
	BYTE vFindSpotInSubROI;
	BYTE vDoContaminant22Algorithm;
	BYTE vTakeDifferenceFromMedian;
	BYTE vTakeDifferenceFromMedianFilter;
	BYTE vBonesInspection;
	BYTE vFillBackground;
	BYTE vFindDarkSpotOnLine;
	BYTE vTrimEdgesOfJarAtROI;
	BYTE vUseVariabiltyImage;
	BYTE vMouseState;		//1 - 8 is left mouse button clicked down, 9 - 16 is left mouse button double click, 0 is idle or all functions completed
	CString vDialogTitleString;
	//WORD vOldUnderRemoteControl;
	bool vNeedToDeleteDisplayingImageData;
	MVT_BLOB_PARAMS *vBlobParameters;
	MVT_BLOB_RESULTS *vBlobResults;
	DWORD vHistogramData[256];
	//WORD vSaveTypesOfRejectsToView;
	BYTE vLastCalculateThreshold;
	bool vRecievedButtonPress;	//do not repeat click and wait for mouse button up if you used keyboard down 
	bool vShowBackground;
	bool vFromSetupWindow;
	double vOriginalROIRight;
	double vOriginalROILeft;
	double vOriginalROITop;
	double vOriginalROIBottom;
	BYTE vOriginalRemoveJarPunt;
	bool vShowingUpperThreshold;
	bool vEditROIBoundaries;
	BYTE vInspectionNumberShowing;
	bool 	vAllRejectsHidden;
	BYTE vFirstInspectionToView;
	unsigned long int vOldGoodCountTotal;
	unsigned long int vOldImproveLearnTotalContainers;
	unsigned long int vOldImproveLearnCount;
	bool vWindowInitialized;
	DWORD vOldLearnCount;
	bool vGaveNoRejectsToShowMessage;
	bool vTweakingThreshold;
	BYTE vDarkestValueInROI;
	BYTE vLightestValueInROI;
	DWORD vNumberOfContaminants;
	DWORD vSizeOfLargestContaminant;
	DWORD vSizeOfAllContaminants;

	DWORD vShowingInspectionNumberOfRedPixels;
	bool vHaveAnImageToShow;
	DWORD vOldCounter[8];
	bool vOldGlobalPasswordOK;
	double vCurrentDensity;
	double vAverageDensity;
	bool vMakeExitButtonGreen;
	bool vChangeMade;
	BYTE vShowBluredImage;
	BYTE vShowTopHatImage;
	BYTE vShowErodeCount;
	BYTE vShowErodeCountAtStart;
	BYTE vShowDilationCount;
	BYTE vShowErodeCountAtEnd;
	BYTE vShowDilationCountAtEnd;
	int vIntensityAdjust;
	bool vOldFindGoodIntensity;
	bool vOldCollectingDensityAverage;
	double vOldDisplayLastMinimumGoodMaximumBad;
	float vOldMinimumGoodStandardDeviation;
	float vOldMinimumGoodAverage;
	float vOldMinimumGoodIntensity;
	double vOldDensityMaximumValue;
	double vOldDensityAverage;
	double vOldDensityMinimumValue;

	WORD vNumberOfRejectsInLocalBuffer;
	bool vHaveRejects;
	bool vShowClipboard;
	bool vImageDisplayOptions;
	bool vRejectsImageDisplay;
	bool vAnalysisImageDisplay;
	bool vEditWhileRunning;
	bool vImproveLearnMenu;
	bool vShowThresholdMenu;
	bool vOldAverageEnabled;
	bool vOldStructureEnabled;
	bool vOldCalibrateBodyTriggerReferenceEnabled;
	bool vOldTakingQuickLearnImages;
	//CInspection *vROIInspection;
	CInspection *vInspectionShowing;
	bool vProductPasswordOK;
	bool vProcessNextImage;
	bool vShowDetectorEdgeLines;
	WORD vSaveImagesToFiles;
	CString vSaveImagesToFilesName;
	unsigned int vWaitForScreenUpdateTimerHandle;
	unsigned int vFinishLoadingFileTimerHandle;
	unsigned int vCursorUpdateTimerHandle;
	bool vWaitForScreenUpdate;
	CImageData *vLocalLastNRejectImages[(cMaxNumberOfLastRejectImages + 1) * 
		cMaximumNumberOfCountsDisplayed];
	CImageData *vProcessData;
	CImageData *vImageBufferForDisplay;
	BYTE vShowType;
	bool vDisplayChanges;
	bool vShowAlternateImage;
	BYTE vPreSmoothImage;
	BYTE vRemoveJarPunt;
	//BYTE vPreBlurImage;
	BYTE vRequire2InARow;
	//BYTE vPreSelectDarkSpotsInImage;
	BYTE vShowingProcess;
	BYTE vShowThreshold;
	CImageData *vDisplayingImageData;
	CImageData *vOnScreenImageData;
	//BYTE vShowImageFactors;
	double vShowOriginalImageFactor;
	double vShowEnhancedImageFactor;
	BYTE vOldSystemRunMode;
	//BYTE vShowGlassHistogram;
	BYTE vShowHistogram;
	BYTE vShowStructure;
	//BYTE vShowGlassStructure;
	BYTE vShowAverage;
	BYTE vShowSubtractAverage;
	BYTE vLookForDarkSpots;
	
	BYTE vShowImproveLearnPreview;
	bool vShowOriginalStructure;
	BYTE vShowDeltaOriginalToCurrentStructure;
	BYTE vShowDeltaCurrentPreviewStructure;

	BYTE vFillInLightSpots;
	BYTE vShowAddStructure;
	BYTE vShowAddHistogram;
	BYTE vShowReferenceAdjustedImage;
	bool vOldXRayOnNotice;
	BYTE MinimumEnhanceROIIntensity;
	BYTE MaximumEnhanceROIIntensity;
	BYTE MinimumROIIntensity;
	BYTE MaximumROIIntensity;
	CScanTracDlg *vMainWindowPointer;
	BYTE vDisplayEnhancedImage;
	BYTE vWhiteOutAnythingTouchingTheTop;
	BYTE vIPContaminant;
	double vThreshold;
	bool vShowNextFrame;
	unsigned long int vUpdateDisplayTimerHandle;
	unsigned long int vOneSecondTimerHandle;
	unsigned long int vMouseDownRepeatTimerHandle;
	DWORD m_maxTime;
	bool vShowingLiveImages;

	WORD vSensitivityLeft;
	WORD vSensitivityBottom;
	WORD vSensitivityHeight;
	WORD vTop;
	WORD vRight;
	WORD vSensitivityWidth;
	WORD vSensitivityRight;
	WORD vLearnSensitivityBottom;
	WORD vSensitivityTop;

	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tProductCollection *vLocalProductCollection;
	tCWndCollection vLocalCWndCollection;
	bool vFunction2ButtonEnable;
	bool vFunction3ButtonEnable;
	bool vFunction4ButtonEnable;
	CDC vCDC;
	HBITMAP vHBitmap;
	BITMAPINFO vBitmapInfo;
	CBitmap *vBitmapPrevious;
	BYTE* vBitmapPixelArray;
	bool vFunction5ButtonEnable;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;
	bool vSubFunction8ButtonEnable;

	//constructor
	CNewImageDisplayDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	//bool NotRunningLive();
	void UpdateDisplay(CImageData *TempDisplayImage);
	void UpdateButtons();
	void DrawImageOnScreen();
	void SaveImageToFile(bool TempSaveImageOnScreen, bool TempShowNameDialog);
	void LoadImageFromFile();
	void ShowImageDisplayInformation(CImageData *TempImage);
	void CalculateMinimumAndMaximumIntensity();
	void OneSecondUpdate();
	void UpdateDisplayIndicators();
	void ShowNextProcessData(); 
	void ShowNextProcess(bool TempForward);
	void StopShowingLiveImages();
	void StartShowingLiveImages();
	void ShowNewImageOnDisplay();
	void WaitThenDrawImageOnScreen();
	void FinishLoadingFile();
	void SetInspectionShowing(CInspection *TempInspection);
	void OperatorSelectCurrentInspection();
	void OperatorSetInspectionSize();
	void ShowNextReject(bool TempGiveNoticeIfNone, bool TempShowPrevious);
	//void SelectRejectFilter();
	void DisplayLocationEdge();
	void EditInspectionSettings();
	void ShowNextDataButton(int TempControlID, CFunctionButton *TempButton);
	void ToggleFilter(BYTE TempWhichFilter);
	bool HaveAnInspection(BYTE TempWhichInspection);
	CString GetInspectionName(BYTE TempWhichInspection);
	void InitializeLocalLastRejectsBuffer(bool TempJustCreatedVariables);
	void DisplayThreshold();
	void PrepareAndExit(BYTE TempExitCode);
	void ShowOrHideDisplays();
	void SetROILines();
	void RestoreOriginalROIValues();
	void SaveOriginalROIValues();
	//void ProcessGlassContaminants(Image *TempImage,
	//	CInspection *TempInspection,BYTE TempInspectionNumber);
	void GetBackgroundImage(BYTE *TempBufferPointer);
	void UpdateThresholdDisplay();
	void UpdateRejectCounts();
	void ShowNeedImageDialogMessage();
	void ClearAllDisplayOptions(bool TempClearSteps);
	void PlotLine(WORD TempCode);
	//void OutputImageROIToCSVFile(CString TempFileName);
	void ReadInOriginalImageFile();
	void CalculateImageDataInformation();
	//void ProcessGlassAlgorithm(BYTE *TempImageIn, 
	//		 CInspection *TempInspection);
	void AddRedSpotsToBuffer(BYTE *TempDestinationBuffer,
		CImageData *TempFromImage, BYTE TempShowReferenceAdjust, CInspection *TempInspection);
	void AddRedBoxToBuffer(BYTE *TempDestinationBuffer,
		CImageData *TempFromImage, BYTE TempShowReferenceAdjust, CInspection *TempInspection);
	void AddRedBoxesToBuffer(BYTE *TempWriteBuffer,
		CImageData *TempFromImage, BYTE TempShowReferenceAdjust, CInspection *TempInspection);
	void CorrectPipeEdgePixels();
	void ShowSensitivity();
	void ClearSensitivity();
	void StartSensitivity();
	void ShowLearnSensitivity();
	void ShowNoMouseClickDialogMessage(); 
	
// Dialog Data
	//{{AFX_DATA(CNewImageDisplayDialog)
	enum { IDD = IDD_NewImageDisplayDialog };
	CStatic	m_CurrentInspection;
	CStatic	m_SubFunction8Display;
	CStatic	m_SubFunction7Display;
	CStatic	m_SubFunction6Display;
	CStatic	m_SubFunction4Display;
	CStatic	m_SubFunction3Display;
	CStatic	m_SubFunction2Display;
	CStatic	m_SubFunction1Display;
	CStatic	m_SubFunction5Display;
	CStatic	m_InspectionThreshold;
	CStatic	m_XRaysOn;
	CStatic	m_ImageStatus;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function1Button;
	CStatic	m_DialogTitleStaticText;
	CStatic	m_ErrorMessage;
	CStatic	m_Background;
	CStatic m_Sensitivity;
	CStatic m_RejectReason;
	CStatic m_MainStatusLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewImageDisplayDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewImageDisplayDialog)
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDialogTitleStaticText();
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnSubFunction1ButtonDoubleClicked();
	afx_msg void OnSubFunction2ButtonDoubleClicked();
	afx_msg void OnSubFunction3ButtonDoubleClicked();
	afx_msg void OnSubFunction4ButtonDoubleClicked();
	afx_msg void OnSubFunction5ButtonDoubleClicked();
	afx_msg void OnSubFunction6ButtonDoubleClicked();
	afx_msg void OnSubFunction7ButtonDoubleClicked();
	afx_msg void OnSubFunction8ButtonDoubleClicked();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWIMAGEDISPLAYDIALOG_H__ABEC9731_069F_11D4_ABE4_00500466E305__INCLUDED_)
