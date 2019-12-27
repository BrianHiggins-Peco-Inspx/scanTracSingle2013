//ScanTrac Side View Source File
// ACPowerMoniterLogFile.cpp: implementation of the CACPowerMoniterLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "ACPowerMoniterLogFile.h"
#include "LogFile.h"
#include "DisplayDialogThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const DWORD cMaximumLinesInFile = 200000;

extern CDisplayDialogThread *vGlobalDisplayDialogThread;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CACPowerMoniterLogFile::CACPowerMoniterLogFile()
{
	vLogDirectory = cLogDirectory;
//	vShowingSerialDataOnScreen = false;
	//vLastDataReceived = "None";
	vLastDataSent = "None";
	vLogACPowerMoniterFormat = 0;
	vTodaysDate = "";
	vTime = NULL;
	vLogFileName = "";
	vTimeOffMessage = "";
	vDataFromLogPointer = NULL;
	vOrigionalNumberOfValues = 0;
	vNumberOfValues = 0;
	vDateIndex = 0;
	vFoundFiles = false;
	vStartUp = true;
	vNumOfErrors = 0;
	vShowAllErrorMessages = false;
	vPreviousTime = 0;
	vCurrentDayFileTimeRange = "";
	vCurrentFileSize = 0;
	vDoResolveErrors = false;
	vValues = NULL;
	vTimes = NULL;
	vCurrentGraphTimes = NULL;
	vZoomScale = 1;
	vMaxGroupSize = 1;
	vPanIndex = 0;
	vShowZeros = false;
	vMaxZoomedOut = true;
	vGaveInspxMessageOverflowWarning = false;
	vPreviousDataLogMessageTime = "";
}

CACPowerMoniterLogFile::~CACPowerMoniterLogFile()
{
;
}

//return of 1 indicates graph has changed, a 0 indicates that it hasn't changed
bool CACPowerMoniterLogFile::DataLogChangeZoomOrZeros(BYTE TempType)
{

	bool TempZoomPanSuccess = false;

	if (vNumberOfValues != 0)
	{
		int TempNumOfThrowAwaysPerGroup = 0;
		float TempMin = 0;
		float TempMax = 0;
		int TempNewArrayIndex = 0;
		int j = 0;
		
		BYTE *TempValues;
		unsigned 
			int TempPreviousNumberOfValues = vNumberOfValues;

		if (vCurrentGraphTimes != NULL)
		{
			free(vCurrentGraphTimes);
			vCurrentGraphTimes = NULL;
		}
		if (ThereIsEnoughMemory(sizeof(DWORD) * cMaximumLinesInFile, "Check for AC Power Monitor Log Graph"))
			vCurrentGraphTimes = (DWORD *)malloc(sizeof(DWORD) * cMaximumLinesInFile);

		if (ThereIsEnoughMemory(sizeof(BYTE) * cMaximumLinesInFile, "Check for AC Power Monitor Log Graph"))
			TempValues = (BYTE *)malloc(sizeof(BYTE) * cMaximumLinesInFile);

		switch (TempType)
		{
			case cZoomIn:
			{
				if (vZoomScale != 1)
				{
					vMaxZoomedOut = false;

					//set zoom scale
					int TempNextZoomScale=1;	//zoom scale is number of points per pixel to graph

					int TempZoomNumberOfPointsPerPixelDifference = vZoomScale - TempNextZoomScale;
					
					while (TempZoomNumberOfPointsPerPixelDifference > TempNextZoomScale) 
					{
						TempNextZoomScale = TempNextZoomScale * 2;
						TempZoomNumberOfPointsPerPixelDifference = vZoomScale - TempNextZoomScale;
					}
					
					vZoomScale = TempNextZoomScale;

					//set groupsize of data
					vNumberOfValues = cWidth*vZoomScale;

					//set start point in data
						//leave same as zooming in you will not hit any boundries if you were not hitting any before

					TempZoomPanSuccess = true;
				}
				else 
				if ( vNumberOfValues > cWidth )
				{
					vMaxZoomedOut = false;
					vNumberOfValues = cWidth;
					TempZoomPanSuccess = true;
				}
				break;
			}
			case cZoomOut:
				{
				if(vMaxZoomedOut == false)
				{
					TempZoomPanSuccess = true;
					//set scale
					vZoomScale = vZoomScale * 2; 
					
					if(vZoomScale >= vMaxGroupSize)
					{
						vZoomScale = vMaxGroupSize;
						vMaxZoomedOut = true;
						vPanIndex = 0;
						vNumberOfValues = vOrigionalNumberOfValues;
						break;
					}

					//set starting point in data "Pan Index"
					if( (vPanIndex + cWidth*vZoomScale) > (int)vOrigionalNumberOfValues )
					{
						vPanIndex = vOrigionalNumberOfValues - cWidth*vZoomScale;
						if(vPanIndex < 0)
							vPanIndex = 0;
					}

					//set number of data points to plot
					if( (vPanIndex + cWidth*vZoomScale) < (int)vOrigionalNumberOfValues ) 
						vNumberOfValues = cWidth*vZoomScale;
					else
						vNumberOfValues = vOrigionalNumberOfValues - vPanIndex; //if this point is reached vPanIndex should be 0
				} 
				else
					vNumberOfValues = vOrigionalNumberOfValues;
				
				break;
			}
			case cFullZoomIn:
			{
				if(vNumberOfValues > cWidth )
				{
					TempZoomPanSuccess = true;
					vZoomScale = 1;
					vNumberOfValues = cWidth;
					//leave start point of data the same, no boundries will be hit by zooming in
					vMaxZoomedOut = false;
				}
				else if (vOrigionalNumberOfValues > cWidth)
					vNumberOfValues = cWidth;
				break;
			}
			case cFullZoomOut:
			{
				if( vMaxZoomedOut == false )
				{
					vZoomScale = vMaxGroupSize;
					vPanIndex = 0;
					vNumberOfValues = vOrigionalNumberOfValues;
					vMaxZoomedOut = true;
					TempZoomPanSuccess = true;
				}
				else
					vNumberOfValues = vOrigionalNumberOfValues;
				break;
			}
			case cPanRight:		
			{//reset number of values
				vNumberOfValues = cWidth*vZoomScale;
				if ((int)vOrigionalNumberOfValues > (vPanIndex + vNumberOfValues))
				{
					vPanIndex = vPanIndex + vNumberOfValues;
					if ((vPanIndex + vNumberOfValues) > (int)vOrigionalNumberOfValues)
						vPanIndex = vOrigionalNumberOfValues - vNumberOfValues;
					TempZoomPanSuccess = true;
				}
				break;
			}
			case cPanLeft:
			{
				vNumberOfValues = cWidth*vZoomScale;
				if (vPanIndex != 0)
				{
					vPanIndex = vPanIndex - vNumberOfValues;
					if (vPanIndex < 0)
						vPanIndex = 0;
					TempZoomPanSuccess = true;
				}
				break;
			}
			case cShowZeros:
			{
				if( vMaxZoomedOut == true )
					vNumberOfValues = vOrigionalNumberOfValues;
				else
					vNumberOfValues = cWidth*vZoomScale;

				//don't change any data paramters
				vShowZeros = true;
				TempZoomPanSuccess = true;
				break;
			}
			case cHideZeros:
			{
				if( vMaxZoomedOut == true )
					vNumberOfValues = vOrigionalNumberOfValues;
				else
					vNumberOfValues = cWidth*vZoomScale;

				vShowZeros = false;
				TempZoomPanSuccess = true;
				break;
			}
			default:
			;	//unkown case
		}

		int TempGroupSize = vZoomScale;

	//	vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + TempGraphTime;

		if (!vCurrentGraphTimes)
			ReportErrorMessage("Not Enough Memory for vCurrentGraphTimes Alloc",cEMailInspx, 32000);
		else
		if (!TempValues)
			ReportErrorMessage("Not Enough Memory for TempValues Alloc",cEMailInspx, 32000);
		else
		if (vTimes)
		if (vValues)
		for (int i = vPanIndex; i < (int)(vNumberOfValues + vPanIndex); i = i + TempGroupSize)
		{
	//			if(i> int(vNumberOfValues + vPanIndex - TempGroupSize)) //for debug 
	//				int x=0;
			
			TempMin = vValues[i];
			TempMax = vValues[i];

			for (j = 1; j < TempGroupSize; j++)
			{
				if ((i+j) >= (int)vNumberOfValues + vPanIndex)
					break;
				if (vValues[i+j] < TempMin )
					TempMin = vValues[i+j];
				if (vValues[i+j] > TempMax )
					TempMax = vValues[i+j];
			}
			

			if (vShowZeros == true)
			{
				if (vNumberOfValues != 0)
				{
					if (TempNewArrayIndex > 99980)
					{
						ReportErrorMessage("AC Power Monitor Voltage Data Log Overflow", cEMailInspx, 32000);
						break;
					}
					if (TempNewArrayIndex < 0)
					{
						ReportErrorMessage("AC Power Monitor Voltage Data Log Index negitive", cEMailInspx, 32000);
						break;
					}
					TempValues[TempNewArrayIndex] = (BYTE)TempMin;
					vCurrentGraphTimes[TempNewArrayIndex] = vTimes[i];
					TempNewArrayIndex++;
				
					if( (TempGroupSize != 1) && ( j != 1 ) )
					{
						TempValues[TempNewArrayIndex] = (BYTE)TempMax;
						vCurrentGraphTimes[TempNewArrayIndex] = vTimes[i];
						TempNewArrayIndex++;
					};
					if (j < TempGroupSize)
						break;
					if (i <= int(vNumberOfValues + vPanIndex - TempGroupSize))//if this isn't the last point graphed, then add zeros
					if (int(vTimes[i + TempGroupSize] - vTimes[i] ) > 2 + TempGroupSize)
					{
						if (vZoomScale > 1)
						for (int k = (vTimes[i + TempGroupSize] - vTimes[i]) / TempGroupSize; k > 1 ; k--) //need 2 zeros per group of values that don't exist
						{
							TempValues[TempNewArrayIndex] = 0;
							vCurrentGraphTimes[TempNewArrayIndex] = vCurrentGraphTimes[TempNewArrayIndex-1]+TempGroupSize/2;
							TempNewArrayIndex++;
							TempValues[TempNewArrayIndex] = 0;
							vCurrentGraphTimes[TempNewArrayIndex] = vCurrentGraphTimes[TempNewArrayIndex-1]+TempGroupSize/2;
							TempNewArrayIndex++;
						}
						else
						for (int k = ( vTimes[i + 1] - vTimes[i] ); k > 1 ; k--) 
						{
							TempValues[TempNewArrayIndex] = 0;
							vCurrentGraphTimes[TempNewArrayIndex] = vCurrentGraphTimes[TempNewArrayIndex-1]+1;
							TempNewArrayIndex++;
						}
					}
				}
			}
			else
			{
				TempValues[TempNewArrayIndex] = (BYTE)TempMin;
				vCurrentGraphTimes[TempNewArrayIndex] = vTimes[i];
				TempNewArrayIndex++;

	//			if( TempValues[TempNewArrayIndex-1] == 0 )	//debug only
	//				int x = 0;
			
				if ((TempGroupSize > 1) && (j!=1))
				{
					TempValues[TempNewArrayIndex] = (BYTE)TempMax;
					vCurrentGraphTimes[TempNewArrayIndex] = vTimes[i];
					TempNewArrayIndex++;
				}

	//			if( TempValues[TempNewArrayIndex-1] == 0 )// debug only
	//				int x = 0;
			}
		}
		
		vNumberOfValues = TempNewArrayIndex;

		if ((vCurrentGraphTimes) && (TempValues))
		{
			if (ThereIsEnoughMemory(sizeof(DWORD) * vNumberOfValues, "Check for AC Power Monitor Log Graph"))
			{

				DWORD *TempPointer = (DWORD *)malloc(sizeof(DWORD) * vNumberOfValues);

				if (TempPointer)
				{
					for (int i = 0; (unsigned int)i < vNumberOfValues; i++)
					{
						TempPointer[i] = vCurrentGraphTimes[i];
					}

					if (vCurrentGraphTimes != NULL)
					{
						free(vCurrentGraphTimes);
						vCurrentGraphTimes = NULL;
					}

					vCurrentGraphTimes = TempPointer;
				}
				else
				{
					ReportErrorMessage("Not Enough Memory for LogFile Alloc",cEMailInspx, 32000);
					vCurrentGraphTimes = NULL;
				}
			}
			else
			{
				ReportErrorMessage("Not Enough Memory for LogFile Alloc",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for LogFile Alloc";
				#endif
			}
			
			
			//put data into global pointer Warning - make sure to free memory
			if (ThereIsEnoughMemory(sizeof(double) * vNumberOfValues, "AC Power Monitor vDataFromLogPointer"))
			{
				if (vDataFromLogPointer != NULL)
				{
					free(vDataFromLogPointer);
					vDataFromLogPointer = NULL;
				}
				vDataFromLogPointer = (double *)malloc(sizeof(double) * vNumberOfValues);
			
				if (vDataFromLogPointer)
				for (int i = 0; (unsigned int)i < vNumberOfValues; i++)
				{
					vDataFromLogPointer[i] = TempValues[i];
				}
			}

			//Title String Time and Date Calculations
			//calculating time -start hour
			if (vZoomScale != vOrigionalNumberOfValues / cWidth) //if not showing the full day on the screen, then figure out what the time span showing is
			{
				CString TempNumberString = "";
				vCurrentDayFileTimeRange = " from " + dtoa(vCurrentGraphTimes[0] / 3600) + ":";

				//start minute
				TempNumberString = dtoa((vCurrentGraphTimes[0] % 3600) / 60);
				if (TempNumberString.GetLength() == 1)
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + "0" + TempNumberString+ ":";
				else
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + TempNumberString+ ":";

				//start second and finish hour
				TempNumberString = dtoa(vCurrentGraphTimes[0] % 60);
				if (TempNumberString.GetLength() == 1)
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + "0" + TempNumberString+ " to " + dtoa( vCurrentGraphTimes[vNumberOfValues - 1] / 3600 );
				else
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + TempNumberString+ " to " + dtoa(vCurrentGraphTimes[vNumberOfValues - 1] / 3600);

				//finish minute
				TempNumberString = dtoa((vCurrentGraphTimes[vNumberOfValues - 1] % 3600) / 60);
				if (TempNumberString.GetLength() == 1)
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + ":" + "0" + TempNumberString+ ":";
				else
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + ":" + TempNumberString+ ":";

				//finish second
				TempNumberString = dtoa(vCurrentGraphTimes[vNumberOfValues - 1] %60);
				if (TempNumberString.GetLength() == 1)
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + "0" + TempNumberString;
				else
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + TempNumberString;

				//+ spans hr:min:sec
				//spans hours
				vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + "-Spans " + dtoa((vCurrentGraphTimes[vNumberOfValues - 1] - vCurrentGraphTimes[0]) / 3600 ) + ":";

					//spans minutes
				TempNumberString = dtoa(((vCurrentGraphTimes[vNumberOfValues - 1] - vCurrentGraphTimes[0]) % 3600) / 60);
				if (TempNumberString.GetLength() == 1)
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + "0" + TempNumberString+ ":";
				else
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + TempNumberString+ ":";

				//spans seconds
				TempNumberString = dtoa((vCurrentGraphTimes[vNumberOfValues - 1] - vCurrentGraphTimes[0]) % 60);
				if (TempNumberString.GetLength() == 1)
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + "0" + TempNumberString;
				else
					vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + TempNumberString;
			}

			if (((TempType == cZoomIn) || (TempType == cFullZoomIn)) && (vShowZeros == true)  && (vZoomScale == 1))
				if (TempPreviousNumberOfValues == vNumberOfValues)
					TempZoomPanSuccess = false;
		}

		if (TempValues != NULL)
			free(TempValues);
	}
	return TempZoomPanSuccess;
}

void CACPowerMoniterLogFile::GetLastDateTimeStamp(CString *TempReturnStrings)
{
	int TempTodaysDateIndex = 0, TempTodaysTimeIndex = 0;
	int TempCurrentFileDateIndex = 0, TempCurrentFileTimeIndex = 0;
	int TempLastFileDateIndex = 0, TempLastFileTimeIndex = 0;
	CString TempFileType = "*.*", TempTimeString = "";
	CString TempFileNameString = "", TempFileTypeString = "";
	CString TempTodaysDateString = "", TempTodaysTimeString = "";
	CString TempCurrentFilePathString = "", TempCurrentFileDateString = "", TempCurrentFileTimeString = "";
	CString TempLastFilePathString = "", TempLastFileDateString = "", TempLastFileTimeString = "";
	CString TempLastFileDateTimeStamp = ""; 
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString TempString = cACPowerMoniterLogDirectory + TempFileType;
	LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) ;// "\n\n\nNo files in image directory to delete"; 
	else 
	{	
		TempTodaysDateString = vTodaysDate;
		TempTodaysDateString.Remove('-');
		TempTodaysDateIndex = _wtoi(TempTodaysDateString);

		TempTodaysTimeString = vTime.Format("%H%M%S");
		TempTodaysTimeIndex = _wtoi(TempTodaysTimeString);
		
		do
		{
			TempFileNameString = "";
			TempFileNameString = FindFileData.cFileName; 
			TempCurrentFilePathString = cACPowerMoniterLogDirectory + TempFileNameString;

			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				TempFileTypeString = TempFileNameString.Mid(14,7);
				if (TempFileTypeString == "DataLog")
				{
					LPCTSTR TempFileName = TempCurrentFilePathString;

					TempCurrentFileDateString = "";
					TempTimeString = TempCurrentFileDateString = TempFileNameString.Mid(21,10); 
					TempTimeString.Remove('-');
					TempCurrentFileDateIndex = _wtoi(TempTimeString);
					
					TempCurrentFileTimeString = "";
					TempTimeString = TempCurrentFileTimeString = TempFileNameString.Mid(32,8);
					TempTimeString.Remove('-');
					TempCurrentFileTimeIndex = _wtoi(TempTimeString);
					
					if ((TempCurrentFileDateIndex == TempTodaysDateIndex) && (TempCurrentFileTimeIndex > TempTodaysTimeIndex - 5))
						continue;
					else
					if (TempLastFilePathString == "")
					{
						TempLastFilePathString = TempCurrentFilePathString;
						TempLastFileDateIndex = TempCurrentFileDateIndex;
						TempLastFileDateString = TempCurrentFileDateString;
						TempLastFileTimeIndex = TempCurrentFileTimeIndex;
						TempLastFileTimeString = TempCurrentFileTimeString;
					}
					else
					if (TempCurrentFileDateIndex > TempLastFileDateIndex)
					{
						TempLastFilePathString = TempCurrentFilePathString;
						TempLastFileDateIndex = TempCurrentFileDateIndex;
						TempLastFileDateString = TempCurrentFileDateString;
						TempLastFileTimeIndex = TempCurrentFileTimeIndex;
						TempLastFileTimeString = TempCurrentFileTimeString;
					}
					else 
					if (TempCurrentFileDateIndex == TempLastFileDateIndex)
					{
						if (TempCurrentFileTimeIndex > TempLastFileTimeIndex)
						{
							TempLastFilePathString = TempCurrentFilePathString;
							TempLastFileDateIndex = TempCurrentFileDateIndex;
							TempLastFileDateString = TempCurrentFileDateString;
							TempLastFileTimeIndex = TempCurrentFileTimeIndex;
							TempLastFileTimeString = TempCurrentFileTimeString;
						}
					}
				}
			}
		} 
		
		while (FindNextFile(hFind,&FindFileData));

		FindClose(hFind);
		
		//if you have found a file
		if (TempLastFilePathString != "")
		{
			//put date in date time stamp
			TempLastFileDateTimeStamp = TempLastFileDateString + " ";
			
			//get the last time stamp of last message
			CFileStatus TempFileStatus;
			CFileException TempFileException;
			LPCTSTR TempFileName = TempLastFilePathString;
			BYTE i;

			int TempTimeFileWasLastRead = 0;
			if (CFile::GetStatus(TempFileName, TempFileStatus))
				TempTimeFileWasLastRead = _wtoi((CString)TempFileStatus.m_atime.Format("%H%M%S"));
			if (TempTimeFileWasLastRead != 0) //if file was never read, the system shutdown durring file creation, so there are errors in the file
			if (vSerialDataLogFile.Open(TempFileName, 
				CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone , &TempFileException ))
			{
				TRY
				{
					vSerialDataLogFile.SeekToBegin();
				}
				CATCH_ALL(TempFileException)
				{
					ReportErrorMessage("Failed SeekToBegin of AC Power Monitor Log", cEMailInspx, 32000);
				}
				END_CATCH_ALL

				char TempChars[20];
				UINT TempLength;
				while(true)
				{
					TempLength = vSerialDataLogFile.Read(TempChars,13);
					
					if (8 > TempLength)
					{ //characters 8 is the time stamp on each line
						TempLastFileDateTimeStamp = TempLastFileDateTimeStamp + TempLastFileTimeString;
						break;
					}
					
					TempLastFileTimeString = "";
					for (i = 0; i < 8; i++)
					{
						TempLastFileTimeString = TempLastFileTimeString + TempChars[i];
					}
				}

				TRY
				{
					vSerialDataLogFile.Close();
				}
				CATCH_ALL(TempFileException)
				{
				 #ifdef _DEBUG
					afxDump << "Could Not Close Serial Log File after Writing"  << "\n";
				 #endif
				}
				END_CATCH_ALL
			}
			else
			{
				//could not open serial log file
				#ifdef _DEBUG
					afxDump << "Could Not Open Serial Log File"  << "\n";
				#endif
		//		ReportErrorMessage("Could Not Open AC Log File to write error message",cEMailInspx, 32000);
			}
		}
	}
	//clean up message
	if (TempLastFileDateTimeStamp == "")
	{
		TempReturnStrings[0] = " First ScanTrac Turn On with AC Power Log On";
		TempReturnStrings[2] = dtoa(201);
	}
	else 
	{
		TempReturnStrings[0] = " ScanTrac On.  Previous Turn Off - " + TempLastFileDateTimeStamp;	
		
		if (TempLastFileDateIndex == TempTodaysDateIndex)
		{
			CString TempHoursOff = dtoa(_wtoi(TempTodaysTimeString.Mid(0,2)) - _wtoi(TempLastFileTimeString.Mid(0,2)));
			CString TempMinutesOff = dtoa(_wtoi(TempTodaysTimeString.Mid(2,2)) - _wtoi(TempLastFileTimeString.Mid(3,2)));
			CString TempSecondsOff = dtoa(_wtoi(TempTodaysTimeString.Mid(4,2)) - _wtoi(TempLastFileTimeString.Mid(6,2)));
			if (_wtoi(TempSecondsOff) < 0)
			{
				TempMinutesOff = dtoa(_wtoi(TempMinutesOff) - 1);
				TempSecondsOff = dtoa( _wtoi(TempSecondsOff) + 60);
			}
			if(_wtoi(TempMinutesOff) < 0)
			{
				TempHoursOff = dtoa(_wtoi(TempHoursOff) - 1);
				TempMinutesOff = dtoa(_wtoi(TempMinutesOff) + 60);
			}
			if (TempHoursOff.GetLength()<2)
				TempHoursOff = "0" + TempHoursOff;
			if (TempMinutesOff.GetLength() < 2)
				TempMinutesOff = "0" + TempMinutesOff;
			if (TempSecondsOff.GetLength() <2 )
				TempSecondsOff = "0" + TempSecondsOff;

			TempReturnStrings[1] =  " ScanTrac off for " + TempHoursOff + " hours, " + TempMinutesOff + " minutes, and " + TempSecondsOff + " seconds.";

			TempReturnStrings[2] = TempHoursOff+TempMinutesOff+TempSecondsOff;
		}
		else
			TempReturnStrings[2] = dtoa(201);
	}
}

CString CACPowerMoniterLogFile::GetACPowerMoniterDataLogData(CString TempLogFileName)
{
	CFileException TempFileException;
	LPCTSTR TempFileName = TempLogFileName;
	BYTE i;
	BYTE TempValue = 0;
	DWORD TempTime = 0;
	CString TempReturnLastTimeString = "";

	if ((vTimes) && (vValues))
	if (vSerialDataLogFile.Open(TempFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone , &TempFileException ))
	{
		TRY
		{
			vSerialDataLogFile.SeekToBegin();
		}
		CATCH_ALL(TempFileException)
		{
			ReportErrorMessage("Failed SeekToEnd of AC Power Monitor Log", cEMailInspx, 32000);
		}
		END_CATCH_ALL

		char TempChars[20];
		UINT TempLength;
		while (true)
		{
			TempLength = vSerialDataLogFile.Read(TempChars,13);
			
			if (TempLength < 8)
			{ //characters 8 is the time stamp on each line
				break;
			}
			
			CString TempString = TempChars;
			int TempCheck = TempString.Find(10);
			if (TempCheck != 13)
				vSerialDataLogFile.Seek(TempCheck-12, CFile::current);

			//get time
			TempTime = 0;
			TempReturnLastTimeString = "";
			int TempInvalidTime = 0;
			for (i = 0; i < 8; i++)
			{
				TempReturnLastTimeString = TempReturnLastTimeString + (char)TempChars[i];
				if (TempChars[i] == 58)
				{
					TempTime = TempTime * 6;
					continue;
				}
				TempTime = TempTime + TempChars[i] - 48;
				if ((TempChars[i+1] < 48) || (TempChars[i+1] > 57))
				{
					TempInvalidTime++;
					continue;
				}
				else
					TempTime = TempTime * 10;
			}
			if (TempInvalidTime > 5)
				break;

			//get voltage
			TempValue = 0;
			for (i = 9; i < TempLength - 1; i++)
			{
				if (TempChars[i] == 32)
					continue;
				TempValue = TempValue + TempChars[i] - 48;
				if ((TempChars[i+1] < 48) || (TempChars[i+1] > 57))
				{
			//		TempInvalidVoltage++;
					break;
				}
				else
					TempValue = TempValue * 10;
			}
			if (TempValue == 0)
				int x = 0;
			
			if (TempValue > 150)
				int x = 0;
/*
			if(vShowZeros == true)
			{
				if(vNumberOfValues != 0)
					if( TempTime - vPreviousTime > 4 )
					{
						for(int j = TempTime - vPreviousTime; j > 1 ; j--)
						{
							vValues[vNumberOfValues] = 0;
							vNumberOfValues++;
						}
					}

				vPreviousTime = TempTime;
				
			}
*/		
			if (vNumberOfValues < cMaximumLinesInFile)
			{
				vTimes[vNumberOfValues] = TempTime;
				vValues[vNumberOfValues] = TempValue;
				vNumberOfValues++;
			}
			else
				break;
		}

		TRY
		{
			vSerialDataLogFile.Close();
		}
		CATCH_ALL(TempFileException)
		{
		 #ifdef _DEBUG
			afxDump << "Could Not Close Serial Log File after Writing"  << "\n";
		 #endif
		}
		END_CATCH_ALL

	}
	else
	{
		//could not open serial log file
		#ifdef _DEBUG
			afxDump << "Could Not Open Serial Log File"  << "\n";
		#endif
		ReportErrorMessage("Could Not Open AC Monitor Data Log File",cEMailInspx, 32000);
	}
	return TempReturnLastTimeString;
}

void CACPowerMoniterLogFile::FreeGlobalDataPointer()
{
	if (vDataFromLogPointer != NULL)
		free(vDataFromLogPointer);

	if (vValues != NULL)
		free(vValues);

	if (vTimes != NULL)
		free(vTimes);

	if (vCurrentGraphTimes != NULL)
		free(vCurrentGraphTimes);

	vDataFromLogPointer = NULL;
	vValues = NULL;
	vTimes = NULL;
	vCurrentGraphTimes = NULL;
}

CString CACPowerMoniterLogFile::GetACPowerMoniterErrorDataFromLogFile(CString TempLogFile)
{
	CString TempErrorString = "", TempString = "", TempReturnErrorString = "", TempPreviousErrorString = "";
	CFileException TempFileException;
	LPCTSTR TempFileName = TempLogFile;
	bool TempShow = true;

	TempErrorString = "";
	TempString = "";
	TempReturnErrorString = "";
	TempPreviousErrorString = "";

	{
		DWORD TempLineCount = 0;
		if (vSerialDataLogFile.Open(TempFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone , &TempFileException ))
		{
			TRY
			{
				vSerialDataLogFile.SeekToBegin();
			}
			CATCH_ALL(TempFileException)
			{
				ReportErrorMessage("Failed SeekToEnd of AC Power Monitor Log", cEMailInspx, 32000);
			}
			END_CATCH_ALL

			char TempChars[200];
			char TempLine[255];
			WORD TempLength;
			WORD TempLineIndex = 0;
			DWORD TempTime = 0;

			for (WORD TempLoop = 0; TempLoop < 200; TempLoop++)
				TempLine[TempLoop] = ' ';

			while(true)
			{
				TRY
				{
					for (WORD TempLoop = 0; TempLoop < 100; TempLoop++)
						TempChars[TempLoop] = ' ';

					TempLength = vSerialDataLogFile.Read(TempChars,100);
				}
				CATCH_ALL(TempFileException)
				{
					#ifdef _DEBUG
						afxDump << "Error reading AC Error Log, File may be corrupt"  << "\n";
					#endif
					break;
				}
				END_CATCH_ALL

				WORD TempTimeCharacterCount = 0;
				TempLineCount++;
				if (!(TempLineCount % 50))
					Sleep(1);
				if ((TempLength > 0) && (TempLength <= 100))
				for (WORD i = 0; i < TempLength; i++)
				{
					if (((TempChars[i] >= 32) && (TempChars[i] < 126)) || (TempChars[i] == 10))
					{
						if ((TempChars[i] != '*') && (TempChars[i] != '+'))
						{
							TempLine[TempLineIndex] = TempChars[i];
							TempLineIndex++;
						}

						if ((vShowAllErrorMessages == false) && (TempChars[i] == '/' ))
							TempShow = false;
						else 
						if (TempShow)
							TempErrorString = TempErrorString + TempChars[i];
					}
					
					if (TempChars[i] == '\n')
					{
						if (TempLineIndex > 10) //must have 8 characters in line for a valid time
						{
							TempShow = true;
					
							//get time
							if ((TempLine[0] != '/') || (vShowAllErrorMessages))
							{
								TempTime = 0;
								for (WORD TempLineIndexLoop = 0; TempLineIndexLoop < 9; TempLineIndexLoop++)
								{
									if (TempLine[TempLineIndexLoop] == 58)
									{
										TempTime = TempTime * 6;
										continue;
									}
									if ((TempLine[TempLineIndexLoop] < 48) || (TempLine[TempLineIndexLoop] > 57))
										continue;

									TempTime = TempTime + TempLine[TempLineIndexLoop] - 48;
								
									if ((TempLine[TempLineIndexLoop + 1] < 48) || (TempLine[TempLineIndexLoop + 1] > 57))
										continue;

									TempTimeCharacterCount++;
									TempTime = TempTime * 10;
								}

								if (TempErrorString.GetLength())
								if (TempTimeCharacterCount >= 6)  //did not get a valid line with a time
								{
									if (vPreviousTime != (DWORD)-1)
									{ //if there is a prevoius time to compare it to
										if (TempTime - vPreviousTime > cTimePerLineFeedIncrimentInLog) //see if it the difference is greater than 5 seconds
											TempErrorString = (char)10 + TempErrorString; //add another line if so.
									}
									vPreviousTime = TempTime;
									TempReturnErrorString = TempReturnErrorString + TempErrorString;
									TempPreviousErrorString = TempErrorString;
								}
							}
						}
					}
				}
				for (WORD TempLoop = 0; TempLoop < TempLineIndex; TempLoop++)
					TempLine[TempLoop] = ' ';

				TempLineIndex = 0;
				TempReturnErrorString = TempReturnErrorString + TempErrorString;
				TempErrorString = "";

				if (100 > TempLength)  //read less than 100 characters, so must be end of file so exit
					break;
			}
			TRY
			{
				vSerialDataLogFile.Close();
			}
			CATCH_ALL(TempFileException)
			{
			 #ifdef _DEBUG
				afxDump << "Could Not Close Serial Log File after Writing"  << "\n";
			 #endif
			}
			END_CATCH_ALL
		}
		else
		{
			//could not open serial log file
			#ifdef _DEBUG
				afxDump << "Could Not Open Serial Log File"  << "\n";
			#endif
			ReportErrorMessage("Could Not Open AC Monitor Error Log File",cEMailInspx, 32000);
		}
	}

	return TempReturnErrorString;
}

void CACPowerMoniterLogFile::SetACPowerMoniterFormat(bool Set, BYTE TempLogType){
	if(Set)
		vLogACPowerMoniterFormat = TempLogType;
}

CString CACPowerMoniterLogFile::GetACPowerMoniterErrorLogData(int TempDateOffset)			//gets all files for today and compiles the strings from all files
{
	CString TempFileType = "*.*";
	CString TempDataString = "";
	CString TempFileDateString = "";
	CString TempFileString = "";
	CString TempFileTypeString = "";
	CString TempFileNameString = "";
	CString TempFileTimeString = "";
	CString TempInspectionString = "";
	CString *TempDaysFiles = NULL;//can be increased later, don't think people will shut off machine more than 50x per day, only 20 error logs will be looked at per day
	int TempNumOfDaysFiles = 0, i=0;
	int TempNextFileIndex = 0;
	int TempSizeOfDaysFilesArray = 250;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString TempString = cACPowerMoniterLogDirectory + TempFileType;
	LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	if (ThereIsEnoughMemory(sizeof(CString) * 1000, "TempDaysFiles"))
	{
		TempDaysFiles = new CString[1000];
		if (TempDaysFiles)
		for (int i = 0; i < 1000; i++)
			TempDaysFiles[i] = "";
	}
	else
	{
		ReportErrorMessage("Not Enough Memory for AC Power Monitor Log File Data -TempDaysFiles- Alloc", cEMailInspx, 32000);
		#ifdef _DEBUG
			afxDump << "Not Enough Memory for LogFile Alloc";
		#endif
	}
	
	if (TempDaysFiles)
	if (hFind != INVALID_HANDLE_VALUE)//If have some files in image directory to delete"; 
	{
		do
		{
			TempFileNameString = "", TempFileTimeString = "";
			TempFileString = FindFileData.cFileName; 

			TempFileNameString = TempFileNameString + cACPowerMoniterLogDirectory + TempFileString;

			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				TempFileTypeString = TempFileString.Mid(14,8);
				if (TempFileTypeString == "ErrorLog")
				{
					TempFileDateString = TempFileString.Mid(22,10); 
					TempFileTimeString = TempFileString.Mid(33,8);
					TempFileTimeString.Remove('-');
					
					int TempCurrentFileDateIndex = _wtoi(TempFileDateString.Mid(0,4)) * 10000//years
							+ _wtoi(TempFileDateString.Mid(5,2)) * 100 //months
							+ _wtoi(TempFileDateString.Mid(8,2)); //days

					if (TempDateOffset == 0)
					{
						if (vDateIndex == 0)
						{
							if (vTodaysDate == "")
							{
								vTime = CTime::GetCurrentTime();
								vTodaysDate = vTime.Format("%Y-%m-%d");
							}
							vDateIndex = _wtoi(vTodaysDate.Mid(0,4)) * 10000//years
								+ _wtoi(vTodaysDate.Mid(5,2)) * 100 //months
								+ _wtoi(vTodaysDate.Mid(8,2)); //days
							vCurrentFileDateString = vTodaysDate;
						}
						if (TempCurrentFileDateIndex == vDateIndex)
						{
							if (TempNumOfDaysFiles >= TempSizeOfDaysFilesArray - 1)
							{
								ReportErrorMessage("Too Many AC Power Monitor Log Files.  There are >250.  Delete some.",cEMailInspx, 32000); 
								continue;
							}
							TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
							TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
							TempNumOfDaysFiles++;
						}
					}
					else
					if (TempDateOffset < 0) //negative offset, need to go back in time, smaller number
					{
					
						if (TempCurrentFileDateIndex < vDateIndex)
						{
							if ((TempNextFileIndex == 0) || (TempNextFileIndex < TempCurrentFileDateIndex))
							{
								TempNextFileIndex = TempCurrentFileDateIndex;
								TempNumOfDaysFiles = 0;
								vCurrentFileDateString = TempFileDateString;
								TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
								TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
								TempNumOfDaysFiles++;
							}
							else
							if (TempNextFileIndex == TempCurrentFileDateIndex)
							{
								if (TempNumOfDaysFiles >= TempSizeOfDaysFilesArray - 1)
								{
									ReportErrorMessage("Too Many AC Power Monitor Log Files.  There are >250.  Delete some.", cEMailInspx, 32000); 
									continue;
								}
								TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
								TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
								TempNumOfDaysFiles++;
							}
						}
					}
					else 
					if (TempDateOffset > 0)
					{
						if (TempCurrentFileDateIndex > vDateIndex)
						{
							if ((TempNextFileIndex == 0) || (TempNextFileIndex > TempCurrentFileDateIndex))
							{
								TempNextFileIndex = TempCurrentFileDateIndex;
								TempNumOfDaysFiles = 0;
								vCurrentFileDateString = TempFileDateString;
								TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
								TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
								TempNumOfDaysFiles++;
							}
							else
							if (TempNextFileIndex == TempCurrentFileDateIndex)
							{
								if (TempNumOfDaysFiles >= TempSizeOfDaysFilesArray - 1)
								{
									ReportErrorMessage("Too Many AC Power Monitor Log Files.  There are > 250.  Delete some.", cEMailInspx, 32000); 
									continue;
								}
								TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
								TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
								TempNumOfDaysFiles++;
							}
						}
					}
				}
			}
		} 
		
		while (FindNextFile(hFind,&FindFileData));  //go through while you find files

		if (TempNumOfDaysFiles == 0)
			vFoundFiles = false;
		else
			vFoundFiles = true;

		if ((TempNextFileIndex == 0) && (TempDateOffset != 0))
		{
			if (TempDateOffset > 0)
				TempDataString = "No Previous Files Found";
			else 
			if (TempDateOffset < 0)
				TempDataString = "No Next Files Found";
		}
		else
		{
			if (TempDateOffset!=0)
			{
				vDateIndex = TempNextFileIndex;
			}
				
/*
			CString TempIndex="", TempFile="";
			for ( i = 0 ; i < TempNumOfDaysFiles - 1 ; i++) //make sure files are sorted by time
			{
				//bubble sort compares in n time if it is in the correct order, which it should be
				//compares time stamp of one file to the next one and makes sure the first one is smaller
				if( _wtoi(TempDaysFiles[TempNumOfDaysFiles*2+1]) > _wtoi(TempDaysFiles[(TempNumOfDaysFiles+1)*2+1]) )
				{
					//switch the two files
					TempIndex = TempDaysFiles[TempNumOfDaysFiles*2+1];
					TempFile = TempDaysFiles[TempNumOfDaysFiles*2];
					TempDaysFiles[TempNumOfDaysFiles*2+1] = TempDaysFiles[(TempNumOfDaysFiles+1)*2+1];
					TempDaysFiles[TempNumOfDaysFiles*2] = TempDaysFiles[(TempNumOfDaysFiles+1)*2];
					TempDaysFiles[(TempNumOfDaysFiles+1)*2+1] = TempIndex;
					TempDaysFiles[(TempNumOfDaysFiles+1)*2] = TempFile;

					//see if that file needs to be switched again
					i=i-2;
				}
				//else they are in the correct order
			}
*/			

			bool TempLargeFile = false;
			CFileStatus TempFileStatus;
			if ((CFile::GetStatus(TempDaysFiles[0], TempFileStatus)))
			{
				if (TempFileStatus.m_size > 2000)
					TempLargeFile = true;
			}

			//2009/11/12
			if (TempLargeFile)
				DisplayReadingInErrorLog();

			vPreviousTime = (DWORD)-1;

			for (i=0; i<TempNumOfDaysFiles; i++)
				TempDataString = TempDataString + GetACPowerMoniterErrorDataFromLogFile(TempDaysFiles[i*2]);

			//2009/11/12
			CloseReadingInErrorLog();
		}
	}
	FindClose(hFind);	

	if (TempDaysFiles)
		delete[] (TempDaysFiles);
	TempDaysFiles = NULL;

	if (TempDataString.GetLength() < 5)
	{
		TempDataString = "No AC Power Errors or Events This Day: " + vCurrentFileDateString;
	}
	else
	{
		//invert the order of the lines.
		DWORD TempLength = TempDataString.GetLength();
		CString TempInvertedString("");
		DWORD TempPosition = 0;
		DWORD TempIndex = 0;
		DWORD TempLineCount = 0;
		while (TempPosition < TempLength)
		{
			TempIndex = TempDataString.Find((char)0x0A,TempPosition);
			if (TempIndex <= TempLength)
			{
				DWORD TempLineLength = TempIndex - TempPosition + 1;
				CString TempLineString = TempDataString.Mid(TempPosition, TempLineLength);
				TempInvertedString = TempLineString + TempInvertedString;
				TempPosition = TempIndex + 1;

				TempLineCount++;
				if (!(TempLineCount % 25))
					Sleep(1);
			}
			else
			{
				DWORD TempLineLength = TempLength - TempPosition + 1;
				CString TempLineString = TempDataString.Mid(TempPosition, TempLineLength);
				TempInvertedString = TempLineString + TempInvertedString;
				TempPosition = TempLength;
			}
		}
		TempDataString = TempInvertedString;
	}
	return TempDataString;
}

void CACPowerMoniterLogFile::GetACPowerMoniterLogData(int TempDateOffset)			//gets all files for today and compiles the strings from all files
{
	CString TempFileType = "*.*";
	CString TempFileDateString = "";
	CString TempFileString = "";
	CString TempFileTypeString = "";
	CString TempFileNameString = "";
	CString TempFileTimeString = "";
	CString *TempDaysFiles;//can be increased later, don't think people will shut off machine more than 50x per day, only 20 error logs will be looked at per day
	int TempNumOfDaysFiles = 0, i=0;
	int TempNextFileIndex = 0;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString TempString = cACPowerMoniterLogDirectory + TempFileType;
	LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	if (ThereIsEnoughMemory(sizeof(CString) * 1000, "TempDaysFiles"))
	{
		TempDaysFiles = new CString[1000];
		if (TempDaysFiles)
		for (int i = 0; i < 1000; i++)
			TempDaysFiles[i] = "";
	}
	else
	{
		ReportErrorMessage("Not Enough Memory for AC Power Monitor Log File Data -TempDaysFiles- Alloc",cEMailInspx, 32000);
		#ifdef _DEBUG
		afxDump << "Not Enough Memory for LogFile Alloc";
		#endif
	}

	if (TempDaysFiles)
	if (hFind != INVALID_HANDLE_VALUE) // Have files in image directory to read data from"; 
	{
		do
		{
			TempFileNameString = "", TempFileTimeString = "";
			TempFileString = FindFileData.cFileName; 
			

			TempFileNameString = TempFileNameString + cACPowerMoniterLogDirectory + TempFileString;

			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				TempFileTypeString = TempFileString.Mid(14,7);
				if (TempFileTypeString == "DataLog")
				{
					TempFileDateString = TempFileString.Mid(21,10); 
					TempFileTimeString = TempFileString.Mid(32,8);
					TempFileTimeString.Remove('-');
					
					int TempCurrentFileDateIndex = _wtoi(TempFileDateString.Mid(0,4)) * 10000//years
							+ _wtoi(TempFileDateString.Mid(5,2)) * 100 //months
							+ _wtoi(TempFileDateString.Mid(8,2)); //days

					if (TempDateOffset == 0)
					{
						if (vDateIndex == 0)
						{
							if (vTodaysDate == "")
							{
								vTime = CTime::GetCurrentTime();
								vTodaysDate = vTime.Format("%Y-%m-%d");
							}
							vDateIndex = _wtoi(vTodaysDate.Mid(0,4))* 10000//years
								+ _wtoi(vTodaysDate.Mid(5,2))*100 //months
								+ _wtoi(vTodaysDate.Mid(8,2)); //days
							vCurrentFileDateString = vTodaysDate;
						}
						if (TempCurrentFileDateIndex == vDateIndex)
						{
							TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
							TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
							TempNumOfDaysFiles++;
						}
					}
					else
					if (TempDateOffset < 0) //negative offset, need to go back in time, smaller number
					{
						if (TempCurrentFileDateIndex < vDateIndex)
						{
							if ((TempNextFileIndex == 0) || (TempNextFileIndex < TempCurrentFileDateIndex))
							{
								TempNextFileIndex = TempCurrentFileDateIndex;
								TempNumOfDaysFiles = 0;
								vCurrentFileDateString = TempFileDateString;
								TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
								TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
								TempNumOfDaysFiles++;
							}
							else
							if (TempNextFileIndex == TempCurrentFileDateIndex)
							{
								TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
								TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
								TempNumOfDaysFiles++;
							}
						}
					}
					else 
					if (TempDateOffset > 0)
					{
						if (TempCurrentFileDateIndex > vDateIndex)
						{
							if ((TempNextFileIndex == 0) || (TempNextFileIndex > TempCurrentFileDateIndex))
							{
								TempNextFileIndex = TempCurrentFileDateIndex;
								TempNumOfDaysFiles = 0;
								vCurrentFileDateString = TempFileDateString;
								TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
								TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
								TempNumOfDaysFiles++;
							}
							else
							if (TempNextFileIndex == TempCurrentFileDateIndex)
							{
								TempDaysFiles[TempNumOfDaysFiles*2] = TempFileNameString;
								TempDaysFiles[TempNumOfDaysFiles*2+1] = TempFileTimeString;
								TempNumOfDaysFiles++;
							}
						}
					}
				}
			}
		} 
		
		while (FindNextFile(hFind,&FindFileData));
		
		if (TempNumOfDaysFiles == 0)
			vFoundFiles = false;
		else
		{
			vFoundFiles = true;
			vCurrentDayFileTimeRange = "";
		}

		if ((TempNextFileIndex != 0) || (TempDateOffset == 0))
		{
			if(TempDateOffset!=0)
			{
				vDateIndex = TempNextFileIndex;
			}
/*
			CString TempIndex="", TempFile="";

			 //make sure files are sorted by time
			for ( i = 0 ; i < TempNumOfDaysFiles - 1 ; i++)
			{
				//bubble sort compares in n time if it is in the correct order, which it should be
				//compares time stamp of one file to the next one and makes sure the first one is smaller
				if( _wtoi(TempDaysFiles[TempNumOfDaysFiles*2+1]) > _wtoi(TempDaysFiles[(TempNumOfDaysFiles+1)*2+1]) )
				{
					//switch the two files
					TempIndex = TempDaysFiles[TempNumOfDaysFiles*2+1];
					TempFile = TempDaysFiles[TempNumOfDaysFiles*2];
					TempDaysFiles[TempNumOfDaysFiles*2+1] = TempDaysFiles[(TempNumOfDaysFiles+1)*2+1];
					TempDaysFiles[TempNumOfDaysFiles*2] = TempDaysFiles[(TempNumOfDaysFiles+1)*2];
					TempDaysFiles[(TempNumOfDaysFiles+1)*2+1] = TempIndex;
					TempDaysFiles[(TempNumOfDaysFiles+1)*2] = TempFile;

					//see if that file needs to be switched again
					i=i-2;
				}
				//else they are in the correct order
			}
*/
			//get data
			if (TempNumOfDaysFiles > 0)
			{
				BYTE *TempValues;
				
				CString TempGraphTimeStart = "", TempGraphTimeEnd = "";
				vNumberOfValues = 0;
				vOrigionalNumberOfValues = 0;
				
				if (vTimes != NULL)
				{
					free(vTimes);
					vTimes = NULL;
				}
				if (vValues != NULL)
				{
					free(vValues);
					vValues = NULL;
				}
				
				if (ThereIsEnoughMemory(sizeof(BYTE) * cMaximumLinesInFile, "AC Power Monitor vValues"))
					vValues = (BYTE *)malloc(sizeof(BYTE) * cMaximumLinesInFile);
				
				if (ThereIsEnoughMemory(sizeof(BYTE) * cMaximumLinesInFile, "AC Power Monitor TempValues"))
					TempValues = (BYTE *)malloc(sizeof(BYTE) * cMaximumLinesInFile);

				if (ThereIsEnoughMemory(sizeof(DWORD) * cMaximumLinesInFile, "AC Power Monitor vTimes"))
					vTimes = (DWORD *)malloc(sizeof(DWORD) * cMaximumLinesInFile);

				if ((TempValues) && (vTimes) && (vValues))
				{
					vPanIndex = 0;

					TempGraphTimeStart = TempDaysFiles[1];
					TempGraphTimeStart.Insert(2, ':');
					TempGraphTimeStart.Insert(5, ':');

					CString TempGraphTimeStringToVerifyExistance = "";
					for (i=0; i<TempNumOfDaysFiles; i++)
					{
						TempGraphTimeStringToVerifyExistance = GetACPowerMoniterDataLogData(TempDaysFiles[i*2]);
						if (TempGraphTimeStringToVerifyExistance != "")
						{
							TempGraphTimeEnd = "";
							TempGraphTimeEnd = TempGraphTimeStringToVerifyExistance;
						}
					}

					vCurrentDayFileTimeRange = " from " + TempGraphTimeStart + " to " + TempGraphTimeEnd;

					if (TempGraphTimeEnd != "")
					{
					//calculate time between start of file and end of file
						CString TempHours = dtoa(_wtoi(TempGraphTimeEnd.Mid(0,2)) - _wtoi(TempGraphTimeStart.Mid(0,2)));
						CString TempMinutes = dtoa(_wtoi(TempGraphTimeEnd.Mid(3,2)) - _wtoi(TempGraphTimeStart.Mid(3,2)));
						CString TempSeconds = dtoa(_wtoi(TempGraphTimeEnd.Mid(6,2)) - _wtoi(TempGraphTimeStart.Mid(6,2)));
						if (_wtoi(TempSeconds) < 0)
						{
							TempMinutes = dtoa(_wtoi(TempMinutes) - 1);
							TempSeconds = dtoa(_wtoi(TempSeconds) + 60);
						}
						if (_wtoi(TempMinutes) < 0)
						{
							TempHours = dtoa(_wtoi(TempHours) - 1);
							TempMinutes = dtoa(_wtoi(TempMinutes) + 60);
						}
						if (TempHours.GetLength() < 2)
							TempHours = "0" + TempHours;
						if (TempMinutes.GetLength() < 2)
							TempMinutes = "0" + TempMinutes;
						if (TempSeconds.GetLength() < 2)
							TempSeconds = "0" + TempSeconds;

						vCurrentDayFileTimeRange = vCurrentDayFileTimeRange + "-Spans " + TempHours + ":" + TempMinutes + ":" + TempSeconds;
					}

					vOrigionalNumberOfValues = vNumberOfValues;

					int TempNumOfThrowAwaysPerGroup = 0;
					int TempMin = 0;
					int TempMax = 0;
					int TempNewArrayIndex = 0;
					int j = 0;
					
					vZoomScale = vMaxGroupSize = vNumberOfValues / cWidth;
					if (vZoomScale == 0)
						vZoomScale = vMaxGroupSize = 1;
					if (vNumberOfValues > (WORD)(cWidth * 3))
					{
						TempNumOfThrowAwaysPerGroup = vMaxGroupSize - 2;
						
						vMaxZoomedOut = true;

						for (i = 0; i < (int)vNumberOfValues; i = i + vMaxGroupSize)
						{
							if (TempNewArrayIndex < 0)
								break;
							if (TempNewArrayIndex >= 3000)
								break;
				//			if(i> int(vNumberOfValues - vMaxGroupSize))
					//			int x=0;
							TempMin = 1000;
							TempMax = -1;
							for (j = 0; j < vMaxGroupSize; j++)
							{
								if ((i+j) >= (int)vNumberOfValues)
									break;
								if (vValues[i+j] < TempMin)
									TempMin = (int)vValues[i+j];
								if (vValues[i+j] > TempMax)
									TempMax = vValues[i+j];
							}
							TempValues[TempNewArrayIndex] = (BYTE)TempMin;
							TempNewArrayIndex++;
							TempValues[TempNewArrayIndex] = (BYTE)TempMax;
							TempNewArrayIndex++;

			/*				if(vShowZeros == true)
							if(vNumberOfValues != 0)
							if( int(vTimes[i+j] - vTimes[i+j-vMaxGroupSize]) > 3+vMaxGroupSize  )
							{
								if(vZoomScale > 1)
									for(int k = (vTimes[i+j] - vTimes[i+j-vMaxGroupSize])/vMaxGroupSize; k > 1 ; k--) //need 2 zeros per group of values that don't exist
									{
										TempValues[vNumberOfValues] = 0;
										vNumberOfValues++;
										TempValues[vNumberOfValues] = 0;
										vNumberOfValues++;
									}
								else
									for(int k = (vTimes[i+j] - vTimes[i+j-vMaxGroupSize])/vMaxGroupSize; k > 1 ; k--) //need 2 zeros per group of values that don't exist
									{
										TempValues[vNumberOfValues] = 0;
										vNumberOfValues++;
									}
							}
			*/			}
						
						if (j == 1)
							TempNewArrayIndex--;

						vNumberOfValues = TempNewArrayIndex;
					}
					else
					{
						for (int i = 0; i < (int)vNumberOfValues; i++)
							TempValues[i] = vValues[i];
					}

					//set vDataFromLogPointer				//put data into global pointer Warning - make sure to free memory

					if (vDataFromLogPointer != NULL)
					{
						free(vDataFromLogPointer);
						vDataFromLogPointer = NULL;
					}

					DWORD TempSize = sizeof(double) * vNumberOfValues;
					if (ThereIsEnoughMemory(TempSize, "Check for AC Power Monitor Log Graph"))
					{
						vDataFromLogPointer = (double *)malloc(TempSize);

				//		if (vDataFromLogPointer == NULL)				//for debug
				//			DWORD TempErrorCode = GetLastError();

						if (vDataFromLogPointer)
						for (i = 0; (unsigned int)i < vNumberOfValues; i++)
						{
							double TempFloatForDebug = TempValues[i];
							vDataFromLogPointer[i] = TempFloatForDebug;
						}
					}
					else
					{
						ReportErrorMessage("Not Enough Memory for AC Power Monitor Log File Data Alloc",cEMailInspx, 32000);
						#ifdef _DEBUG
						afxDump << "Not Enough Memory for LogFile Alloc";
						#endif
					}

					//Set vTimes
					if (ThereIsEnoughMemory(sizeof(DWORD) * vOrigionalNumberOfValues, "AC Power Monitor vTimes"))
					{
						DWORD *TempPointer = (DWORD *)malloc(sizeof(DWORD) * vOrigionalNumberOfValues);

						if (TempPointer)
						{
							for (i = 0; (unsigned int)i < vOrigionalNumberOfValues; i++)
							{
								TempPointer[i] = vTimes[i];
							}
							if (vTimes != NULL)
							{
								free(vTimes);
								vTimes = NULL;
							}
							vTimes = TempPointer;
						}
					}
	/*				//set vCurrentGraphTimes
					if (ThereIsEnoughMemory(sizeof(DWORD) * vNumberOfValues, "Check for AC Power Monitor Log Graph"))
					{

						DWORD *TempPointer = (DWORD *)malloc( sizeof(DWORD)*vNumberOfValues);

						DWORD TempIndex = 0;

						for (i = 0; (unsigned int)i < vNumberOfValues; i++)
						{
							if(vDataFromLogPointer[i] != 0)
							{
								TempIndex++;
								TempPointer[i] = vTimes[TempIndex];
							}
							else
								TempPointer[i] = TempPointer[i-1]+vMaxGroupSize / 2;
						}
						vCurrentGraphTimes = TempPointer;
					}
					else
					{
						ReportErrorMessage("Not Enough Memory for LogFile Alloc",cEMailInspx, 32000);
						#ifdef _DEBUG
						afxDump << "Not Enough Memory for LogFile Alloc";
						#endif
					}
	*/
					//set vValues
					if (ThereIsEnoughMemory(sizeof(BYTE) * vOrigionalNumberOfValues, "Check for AC Power Monitor Log Graph"))
					{
						
						BYTE *TempPointer = (BYTE *)malloc(sizeof(BYTE)*vOrigionalNumberOfValues);

						if (TempPointer)
						{
							for (i = 0; (unsigned int)i < vOrigionalNumberOfValues; i++)
							{
								TempPointer[i] = vValues[i];
							}
							if (vValues != NULL)
							{
								free(vValues);
								vValues = NULL;
							}
							vValues = TempPointer;
						}
					}
				}
				if (TempValues)
					free(TempValues);
			}
		}
	}
	if (TempDaysFiles != NULL)
		delete[](TempDaysFiles);
	TempDaysFiles = NULL;

	FindClose(hFind);	
}


void CACPowerMoniterLogFile::WriteToLogFile(CString TempStringToWrite, BYTE TempMessageType) 
{
	int i = 0;

	CFileException TempFileException;
	CMemoryException TempMemoryException;
	TRY
	{
		BYTE TempSyncMessage = false;

		vTime = CTime::GetCurrentTime();
		CString TempTimeString = vTime.Format("%H:%M:%S");

		if ((vLogACPowerMoniterFormat != cERRORLOG) && (TempMessageType == cInputMessage))	//if it is a data log voltage message	
		{
			CString TempReturnStrings[10];	//should only return with 3 strings
			if (vPreviousDataLogMessageTime == "")	//when not set, when the computer shuts down, another fucntion takes care of time stamping it
			{
				vPreviousDataLogMessageTime = TempTimeString;
			}
			else
			{
				// to TempTimeString and write error message if > 3 second gap in log

				CString TempHoursOff = dtoa(_wtoi(TempTimeString.Mid(0,2)) - _wtoi(vPreviousDataLogMessageTime.Mid(0,2)));
				CString TempMinutesOff = dtoa(_wtoi(TempTimeString.Mid(3,2)) - _wtoi(vPreviousDataLogMessageTime.Mid(3,2)));
				
				CString TempSecondsOff = dtoa(_wtoi(TempTimeString.Mid(6,2)) - _wtoi(vPreviousDataLogMessageTime.Mid(6,2)) - 2); //minus two for wait algorithm to collect all errors in a row.
				if(_wtoi(TempSecondsOff) < 0)
				{
					TempMinutesOff = dtoa(_wtoi(TempMinutesOff) - 1);
					TempSecondsOff = dtoa(_wtoi(TempSecondsOff) + 60);
				}
				if (_wtoi(TempMinutesOff) < 0)
				{
					TempHoursOff = dtoa(_wtoi(TempHoursOff)-1);
					TempMinutesOff = dtoa(_wtoi(TempMinutesOff) + 60);
				}
				if (TempHoursOff.GetLength() < 2)
					TempHoursOff = "0" + TempHoursOff;
				if (TempMinutesOff.GetLength() < 2)
					TempMinutesOff = "0" + TempMinutesOff;
				if (TempSecondsOff.GetLength() < 2)
					TempSecondsOff = "0" + TempSecondsOff;

				vPreviousDataLogMessageTime = TempTimeString;
				int TempTimeOff = _wtoi(TempHoursOff + TempMinutesOff + TempSecondsOff);	//temp time off is the result of the calculation [currentTime]-[messageTimeStamp]
				if (TempTimeOff > 3)
				{
					//write error message	
//					IErrorACPowerMoniterLogFile.WriteToLogFile("Power was off for " + TempHoursOff + " hours, "+ TempMinutesOff 
//						+ " minutes, " + TempSecondsOff + " seconds.", cInputMessage);	//can't jump from object like that, use wrapper

					vTimeOffMessage = " Power was off for " + TempHoursOff + " hours, "+ TempMinutesOff 
						+ " minutes, " + TempSecondsOff + " seconds.";
					vDoResolveErrors = true;
				}
			}
		}
		
		if (vLogACPowerMoniterFormat == cERRORLOG)
		{
			if (TempMessageType == cPowerOnMessage)
				TempTimeString = "+" + TempTimeString;
			if (TempMessageType == cErrorPowerOnMessage)
				TempTimeString = "*" + TempTimeString;
		}

		CString TempString = TempTimeString + TempStringToWrite + "\n";

		//if ((vNumOfErrors) && (TempMessageType == cResolveErrorMessages)) || (vLogACPowerMoniterFormat == cERRORLOG)
		{
			TRY
			{
				CStringBufferObject *TempStringToBuffer = NULL;
				TempStringToBuffer = new CStringBufferObject;
				if (TempStringToBuffer)
				{
					TempStringToBuffer->vTheString = TempString;
					if (vACPowerMoniterStringCollectionLock.Lock())
					{
						vACPowerMoniterOutputStringCollection.AddTail(TempStringToBuffer);
						vACPowerMoniterStringCollectionLock.Unlock();
					}
				}
			}
			CATCH(CMemoryException,TempMemoryException)
			{
				#ifdef _DEBUG
				afxDump << "Could Not Write AC Monitor Log File, Out of memory"  << "\n";
				#endif
			}
			END_CATCH
		} 
	}
	CATCH_ALL(TempFileException)
	{
	 #ifdef _DEBUG
			afxDump << "Could Not Write AC Monitor Log File 1"  << "\n";
	 #endif
	}
	END_CATCH_ALL
}


void CACPowerMoniterLogFile::LogFileObjectBackground() 
{
	if (vACPowerMoniterOutputStringCollection.GetCount() > 0)  //if have string to write in file
	{
		vTime = CTime::GetCurrentTime();
		if (vTime.Format("%Y-%m-%d") != vTodaysDate)  //if the date changed, open the file for the new day
			SetupLogFileName();

		CFileException TempFileException;
		LPCTSTR TempFileName = vLogFileName;
		if (vSerialDataLogFileToWrite.Open(TempFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone , &TempFileException ))
		{
			TRY
			{
				vSerialDataLogFileToWrite.SeekToEnd();
			}
			CATCH_ALL(TempFileException)
			{
				ReportErrorMessage("Failed SeekToEnd of AC Power Monitor Log", cEMailInspx, 32000);
			}
			END_CATCH_ALL

			CFileException TempFileException;
			TRY
			{
				if (vACPowerMoniterStringCollectionLock.Lock())
				{
					OutputStringsToFile();
					vACPowerMoniterStringCollectionLock.Unlock();
				}
			}
			CATCH_ALL(TempFileException)
			{
			 #ifdef _DEBUG
					afxDump << "Could Not Output AC Strings to File"  << "\n";
			 #endif
			}
			END_CATCH_ALL

			vSerialDataLogFileToWrite.Close();
		}
		else
		{
			//could not open serial log file
			#ifdef _DEBUG
				afxDump << "Could Not Open Serial Log File"  << "\n";
			#endif
			ReportErrorMessage("Could Not Write AC Monitor Event Log 2",cEMailInspx, 32000);
		}
	}
}

void CACPowerMoniterLogFile::OutputStringsToFile() 
{  //must lock collection and validate log file open before calling
	CStringBufferObject *TempLastStringObject = NULL;

	while (vACPowerMoniterOutputStringCollection.GetCount() > 0)
	{
		CStringBufferObject *TempStringObject = vACPowerMoniterOutputStringCollection.GetHead();
		if (TempStringObject)
		{
			if (TempLastStringObject != TempStringObject)
			{
				if (TempStringObject->vTheString)
				{
					WORD TempLength = TempStringObject->vTheString.GetLength();
					if (TempLength)
						//10/26/2009
						if (TempStringObject->vTheString.GetAt(0) != '/')
						{
							CW2A TempStringToWrite(TempStringObject->vTheString);
							vSerialDataLogFileToWrite.Write(TempStringToWrite, TempLength);
						}
				}
				TempLastStringObject = TempStringObject;
				vACPowerMoniterOutputStringCollection.RemoveHead();
				delete TempStringObject;
			}
			else
			{
				if (TempStringObject->vTheString)
					vSerialDataLogFile.Write("Log Error - Pointer",19);
			}
		}
	}
	vSerialDataLogFile.Flush();
}

BYTE CACPowerMoniterLogFile::SetupLogFileName() 
{
	//Open serial data log file if configured
	bool TempDidIt = false;

	double TempTotalBytesAvailable = CheckDiskSpaceFree("C:\\");
	if (TempTotalBytesAvailable > 200000000)  //200 megabytes free
	{
		CString TempTimeString = "";
		vTime = CTime::GetCurrentTime();
		vTodaysDate = vTime.Format("%Y-%m-%d");

		TempTimeString = vTime.Format("%Y-%m-%d-%H-%M-%S");
		CString TempDirectoryString(vLogDirectory);
		CString TempACPowerMoniterDirectoryString(cACPowerMoniterLogDirectory);
		LPCTSTR TempFileName = _T("");
		if (vLogACPowerMoniterFormat == cERRORLOG)
		{
			TempTimeString = TempACPowerMoniterDirectoryString + "ACPowerMoniterErrorLog" +TempTimeString + ".txt";
			 
			TempFileName = vLogFileName = TempTimeString;
			CFileException TempFileException;
			vSendCount = 0;
			vReceiveCount = 0;
			if (vSerialDataLogFile.Open(TempFileName, CFile::modeCreate | CFile::modeNoTruncate  | CFile::modeReadWrite | CFile::shareDenyNone, &TempFileException))
			{
				vSerialDataLogFile.Close();
				TempDidIt = true;
				if (vStartUp)
				{
					vStartUp = false;
					ReportErrorMessage("Started AC Power Monitor Error Logging",cAction,0);
					CString TempString[3];
					GetLastDateTimeStamp(TempString);
					if (_wtoi(TempString[2]) > 200)
					{
						WriteToLogFile(TempString[0], cPowerOnMessage);
						if (TempString[1] != "")
							WriteToLogFile(TempString[1], cPowerOnMessage);
					}
					else
					if (_wtoi(TempString[2]) > 0)
					{
						WriteToLogFile(TempString[0], cErrorPowerOnMessage);
						if (TempString[1] != "")
							WriteToLogFile(TempString[1], cErrorPowerOnMessage);
					}
				}
			}
			else
			{
				//could not open serial log file
				#ifdef _DEBUG
					afxDump << "Could Not Open Serial Log File"  << "\n";
				#endif
				ReportErrorMessage("Could Not Open AC Monitor Log 3",cEMailInspx, 32000);
			}
			CFileStatus TempFileStatus;
			if (!(CFile::GetStatus(TempFileName, TempFileStatus)))
			{
				TempDidIt = false;
				ReportErrorMessage("Could Not Open AC Monitor Log 4",cEMailInspx, 32000);
			} 
			else
				vCurrentFileSize = TempFileStatus.m_size;
		}
		else
		{
			vLogFileName = TempACPowerMoniterDirectoryString + "ACPowerMonitorDataLog" +TempTimeString + ".txt";
			TempDidIt = true;
		}
	}
	//else
	//	ReportErrorMessage("Low Hard Disk Space, Event Log Not Recorded",cError,31999);
	return TempDidIt;
}

void CACPowerMoniterLogFile::DisplayReadingInErrorLog()
{
	if (!vGlobalDisplayDialogThread)
	{
		vGlobalDisplayDialogThread = (CDisplayDialogThread *)AfxBeginThread(
			RUNTIME_CLASS(CDisplayDialogThread),
			THREAD_PRIORITY_NORMAL);//,0,CREATE_SUSPENDED);
			//THREAD_PRIORITY_ABOVE_NORMAL);//,0,CREATE_SUSPENDED);
			//THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED);
		if (!vGlobalDisplayDialogThread)
		{
			ReportErrorMessage("Error-Could Not Create Display Dialog Thread", cEMailInspx,32000); 
		}
		else
		{
			vGlobalDisplayDialogThread->vMessageToDisplay = "\n\nReading in A/C Power Monitor Error Log\nPlease Wait...";
			vGlobalDisplayDialogThread->ResumeThread();
		}
	}
}

void CACPowerMoniterLogFile::CloseReadingInErrorLog()
{
	if (vGlobalDisplayDialogThread)
	{
		//2009/11/12
		Sleep(10);
		vGlobalDisplayDialogThread->ExitDisplayThread();
		vGlobalDisplayDialogThread = NULL;
	}
}

