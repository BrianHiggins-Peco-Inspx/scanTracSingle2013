//ScanTrac Side View Source File
#if !defined(AFX_CONTAINER_H__B7B33751_D9C3_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_CONTAINER_H__B7B33751_D9C3_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Container.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CContainer command target

class CContainer : public CObject
{
// Attributes
public:

// Operations
public:
	CImageData *vTheImage;
	//BYTE vRejectedForThisInspection[cMaximumNumberOfInspections];
	bool vRealImage;
	DWORD vContainerNumber;
	WORD vLastBodyTriggerGapLength;
	BYTE vNumberOfLinesShort;
	WORD vBodyTriggerPosition;
	WORD vStartImagePosition;
	WORD vEndImagePosition;
	WORD vStartExternalDetectorPosition[5];
	WORD vEndExternalDetectorPosition[5];
	WORD vTimeOutPosition;
	WORD vEjectPosition[cNumberOfEjectors];
	DWORD vEjectorNumber;  //is a one based number because uController is one based

			//Eject Reason = 0 to cNumberOfExternalDetectors - 1
			//	 are Auxiliary detectors
			//Eject Reason = cNumberOfExternalDetectors to cNumberOfExternalDetectors + 
			//	cMaximumNumberOfInspections - 1 are major inspections
	BYTE vDataComplete;
	bool vImageReceived;
	bool vImageAnalyzed;
	bool vExternalDetectorsDone;
	bool vGotStartImageNotice;
	bool vGotEndImageNotice;
	bool vGotExternalDetectorNotice[5];
	bool vToBeEjected;
	bool vToBeEjectedButDisabled;
	bool vGotEjectConfirmedNotice;
	BYTE vGotEjectedNotice;
	BYTE vNumberOfEjectors;
	BYTE vImageBrightness;
	BYTE vHeadNumber[3]; //for filler monitoring and Filler and Seamer Sampling
	double vFillLevel;

	//Constructor
	CContainer();
	DECLARE_DYNCREATE(CContainer)
	virtual ~CContainer();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContainer)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CContainer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTAINER_H__B7B33751_D9C3_11D3_ABCE_00500466E305__INCLUDED_)
