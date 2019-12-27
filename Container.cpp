//ScanTrac Side View Source File
// Container.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "Container.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNCREATE(CContainer, CObject)

/////////////////////////////////////////////////////////////////////////////
// CContainer

CContainer::CContainer()
{
	vTheImage = NULL;
	vContainerNumber = 0;
	vLastBodyTriggerGapLength = 0;
	vRealImage = false;
	vNumberOfLinesShort = 0;
	vBodyTriggerPosition = 0;
	vStartImagePosition = 0;
	vEndImagePosition = 0;
	for(BYTE TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		vStartExternalDetectorPosition[TempLoop] = 0;
		vEndExternalDetectorPosition[TempLoop] = 0;
		vGotExternalDetectorNotice[TempLoop] = false;
	}
	//for(BYTE TempLoop = 0;TempLoop < cMaximumNumberOfInspections; TempLoop++)
	//	vRejectedForThisInspection[TempLoop] = false;

	vTimeOutPosition = 0;
	for (BYTE TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
		vEjectPosition[TempLoop] = 0;
	vEjectorNumber = 0;
	vDataComplete = 0;
	vNumberOfEjectors = 0;
	vImageReceived = false;
	vImageAnalyzed = false;
	vExternalDetectorsDone = false;
	vGotStartImageNotice = false;
	vGotEndImageNotice = false;
	vToBeEjected = false;
	vToBeEjectedButDisabled = false;
	vGotEjectedNotice = 0;
	vGotEjectConfirmedNotice = false;
	vImageBrightness = 255;  //set to all white image
	for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
		vHeadNumber[TempLoop] = 0; //for filler monitoring and Filler and Seamer Sampling
	vFillLevel = 0;
}

CContainer::~CContainer()
{
	if (vTheImage)
		delete vTheImage;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CContainer, CObject)
	//{{AFX_MSG_MAP(CContainer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CContainer member functions
