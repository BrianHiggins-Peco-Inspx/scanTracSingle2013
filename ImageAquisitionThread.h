//ScanTrac Side View Source File
#include "stdafx.h"
#include "SystemConfigurationData.h"
#include "ScanTrac.h"

#if !defined(AFX_IMAGEAQUISITIONTHREAD_H__71FC7601_0D8E_11D5_AD72_00500466E305__INCLUDED_)
#define AFX_IMAGEAQUISITIONTHREAD_H__71FC7601_0D8E_11D5_AD72_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageAquisitionThread.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CImageAquisitionThread thread

class CImageAquisitionThread : public CWinThread
{
	DECLARE_DYNCREATE(CImageAquisitionThread)
public:
	//variables
	bool vSavedFillerMonitorWeight;
	WORD vMultiLaneEjectorOffset;
	bool vCurrentImageBrightnessOK;
	int vLastEvaluateSecond;
	int vPreviousHardwareFrameCount;
	BYTE vNumberOfWrongSizeInARow;
	bool vProcessMinimumGood;
	WORD vCurrentDemoImage;
	double vStartFreeTime;
	//int vReferenceTries;
	bool vSizeOK;
	bool vLoadedThisImageFromFile;
	BYTE *vAFramePointer;
	BYTE *vBFramePointer;
	BYTE *vMultiLaneImage[cMaximumNumberForMultiLane];
	BYTE *vMultiLaneImageMaximum[cMaximumNumberForMultiLane];
	WORD vMultiLaneLineCounter[cMaximumNumberForMultiLane];
	WORD vMultiLaneLineCounterNotInImage[cMaximumNumberForMultiLane];
	WORD vMultiLaneLastImageStartLine[cMaximumNumberForMultiLane];
	BYTE *vMultiLaneSingleLinePointer;
	BYTE *vFinalOutPutBuffer;
	WORD vAFrameCounter;
	WORD vBFrameCounter;
	BYTE vCorrectDetectorEdges;
	bool vMyLock;
	int vEndVirtualFrame;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracDlg *vMainWindowPointer;
	BOOLEAN vShutDownNow;

protected:
	//constructor
	CImageAquisitionThread();           // protected constructor used by dynamic creation

// Attributes

// Operations
public:
	//methods
	int ProcessImage();
	void AdjustEdgeDetectorPixels(bool TempCorrectJustVisiblePicture);
	void AdjustEdgeDetectorPixels(bool TempCorrectJustVisiblePicture, BYTE TempLaneNumber);
	double AdjustImageIntensity(BYTE *TempImage, double TempImageDensity, double TempDensityTarget);
	void ReplaceDeadPixels();
	void FindContaminants(CContainer *TempContainer, 
		BYTE TempReferenceLoop, int TempTopReferenceLoop, CInspection *TempInspection,
		BYTE TempInspectionNumber);
	void FindSizeInspection(CContainer *TempContainer, 
		CInspection *TempInspection,
		BYTE TempInspectionNumber);
	void CheckForJarBottomContaminants(CContainer *TempContainer, 
		CInspection *TempInspection,
		BYTE TempInspectionNumber);
	void CheckForGlassContaminants(CContainer *TempContainer, 
		CInspection *TempInspection,
		BYTE TempInspectionNumber);
	
	void FindKernelContaminants(CContainer *TempContainer, 
		BYTE TempReferenceLoop, BYTE TempTopReferenceLoop, CInspection *TempInspection,
		BYTE TempInspectionNumber);
	void FindVoids(CContainer *TempContainer,
		BYTE TempReferenceLoop, BYTE TempTopReferenceLoop, CInspection *TempInspection,
		BYTE TempInspectionNumber);
	void CheckDensity(CContainer *TempContainer, CInspection *TempInspection, 
		int TempInspectionNumber);
	void CheckStandardDeviation(CContainer *TempContainer, CInspection *TempInspection, 
		int TempInspectionNumber);
	CContainer *GetNextContainerForImageProcessing();
	void ExitImageAquisition();
	//void FindDemoEdges();
	void KillThread();
	void AccumulateStructure(BYTE *TempPointerToImage);
	void AccumulateContaminant2Structure(BYTE *TempPointerToImage);
	//void AccumulateGlassStructure(BYTE *TempPointerToImage);
	void AccumulateVoidStructure(BYTE *TempPointerToImage);
	void AccumulateBodyTriggerReference();
	void AccumulateAverageImage(BYTE *TempPointerToImage);
	void HandleCompletedImage(BYTE *TempFrameBuffer, int TempSequenceNumber);
	void TestEjectThisContainer();
	bool SampleThisContainer(CContainer *TempContainer);
	int CalculateTopReference();
	int CalculateBottomReference();
	void AccumulateAverageStandardDeviation(CInspection *TempInspection, int TempInspectionNumber);
	void AccumulateAverageDensity(CInspection *TempInspection, int TempInspectionNumber);
	//void FindBestReferenceValue();
	//void ReportErrorMessageIAT(LPCTSTR TempErrorMessage, BYTE TempLevel,WORD TempCode);
	//bool ThereAreAnyBlobsAtValue(BYTE TempThreshold, Image *TempImage, CInspection *TempInspection, WORD TempBottom, WORD TempLeft, WORD TempHeight, WORD TempWidth);
	void ProcessCalibrateDetectorOffsetFrameDAC();
	void ProcessCalibrateDetectorOffsetFrameADC();
	bool HalfPixelsGreaterThanZero(BYTE TempDetectorNumber);
	void ProcessCalibrationImage();
	void OutputImageToCSVFile(CString TempImageFileName, BYTE *TempImage, WORD TempSizeX, WORD TempSizeY);
	void CheckUnderfillByWeight(CContainer *TempContainer, CInspection *TempInspection, int TempInspectionNumber);

	//void ClearDataInThisBuffer(BYTE *TempFrameBuffer);
	void WriteImageToHardDisk(BYTE *TempPointer, CString TempName, WORD TempSizeX, WORD TempSizeY);
	double CalculateDetectorDensity(BYTE *TempImagePointer, BYTE TempDetectorNumber, WORD TempSizeX);
	//void ProcessGlassAlgorithm(BYTE *TempImageIn, 
	//			 CInspection *TempInspection);
	void CheckForFloodedIVBag(CContainer *TempContainer, CInspection *TempInspection, BYTE TempInspectionNumber);
	void SaveDataInValveMonitor(CContainer *TempContainer, double TempWeight);
	void InspectionFoundContaminant(BYTE TempInspectionNumber);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageAquisitionThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CImageAquisitionThread();

	// Generated message map functions
	//{{AFX_MSG(CImageAquisitionThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif;// !defined(AFX_IMAGEAQUISITIONTHREAD_H__71FC7601_0D8E_11D5_AD72_00500466E305__INCLUDED_)
