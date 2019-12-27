//ScanTrac Side View Source File
// Win32SerialPortThread.cpp: implementation of the CWin32SerialPortThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "Win32SerialPortThread.h"
#include "NoticeDialog.h"
#include "SerialPort.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "ACPowerMoniter.h"
#include "XRaySource.h"
#include "SerialPortGuardMaster.h"
#include "SerialPortPowerFlex40.h"
#include "SerialPortRFIdeasBadgeReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSerialPortRFIDEASBadgeReader *vGlobalRFIDEASBadgeReaderSerialPort;
extern CSerialPortPowerFlex40 *vGlobalPowerFlex40;
extern CSerialPortGuardMaster *vGlobalGuardMaster;
extern CXRaySource *vGlobalMonoBlockXRayPort;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern CACPowerMoniter *vGlobalACPowerMoniter;
extern CSerialPort *vGlobaluCSerialPort;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWin32SerialPortThread::CWin32SerialPortThread(HANDLE TempComPortHandle, 
	WORD *TempBufferHead, WORD *TempBufferTail, BYTE *TempDataBuffer, 
	DWORD TempCallingThreadID, OVERLAPPED *TempOverlappedStructure,
	bool *TempReadError)
{
	vSerialPortNumber = 0;
	//vMySerialPortOwner = NULL;
	DWORD TempReturnThreadIdentifier;
	vComPortHandle = TempComPortHandle;
	vPointerToThreadOverlappedStructure = TempOverlappedStructure;

	vMyComPortReceiveDataBufferPointer = TempDataBuffer;
	vMyComPortReceiveTailPointer = TempBufferTail;
	vMyComPortReceiveHeadPointer = TempBufferHead;
	for (WORD TempLoop = 0; TempLoop < cReceiveBufferSize; TempLoop++)
		vMyComPortReadDataBuffer[TempLoop] = 0;
	vMyComPortReadError = TempReadError;

	//vGlobalReadError = TempReadError;
	//vGlobalReceiveDataBufferPointer = TempDataBuffer;
	//vGlobalReceiveTailPointer = TempBufferTail;
	//vGlobalReceiveHeadPointer = TempBufferHead;

	vOverFlowError = false;
	vShutDown = false;
	vReadCount = 0;
	vComPortThreadHandle = CreateThread(
			NULL,             // address of thread security attributes
			(DWORD)0x10000,   // initial thread stack size, in bytes
			ReadComPortThread,// address of thread function
			(LPVOID)this,     // argument for new thread
			0x0,              // creation flags
			&TempReturnThreadIdentifier);// address of returned thread identifier
	if (!SetThreadPriority(vComPortThreadHandle,THREAD_PRIORITY_HIGHEST))
	//if (!SetThreadPriority(vComPortThreadHandle,THREAD_PRIORITY_TIME_CRITICAL))//run time critical does not seem to work
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSerial Port Thread Set Priority Failed";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

CWin32SerialPortThread::~CWin32SerialPortThread()
{
	// Tell the trigger thread to shutdown
	//vShutDown = true;
	// Wait for the thread to terminate
	//WaitForSingleObject (vComPortThreadHandle,INFINITE);
	//CloseHandle(vComPortThreadHandle);
}

void CWin32SerialPortThread::CloseSerialPortReadThread()
{
	// Tell the trigger thread to shutdown
	vShutDown = true;
}


// This is the serial port read thread
DWORD WINAPI ReadComPortThread(LPVOID owner)
{
	CWin32SerialPortThread *PointerToThreadObject = (CWin32SerialPortThread *)owner;
	DWORD TempBytesRead = 0;
	DWORD TempEventTypeMask;
	TempEventTypeMask = EV_RXCHAR | EV_ERR;
	int TempResult = 0;
	//PointerToThreadObject->vPointerToThreadOverlappedStructure->
	//	hEvent = CreateEvent(
	//		NULL,   // no security attributes 
	//		false,  // auto reset event 
	//		false,  // not signaled 
	//		NULL    // no name 
//			);
	DWORD TempSleepResult = 0;
  DWORD TempEventMask = 0;
	DWORD TempLength = 0;
	//BYTE TempByte = 0;
	BYTE *TempDataBufferPointer;

	while (!PointerToThreadObject->vShutDown)
	{
    TempEventMask = 0 ;

    WaitCommEvent(PointerToThreadObject->vComPortHandle, &TempEventMask, NULL );
    if ((TempEventMask & EV_RXCHAR) == EV_RXCHAR)
    {
       do
       {
          if (TempLength = ReadCommBlock(PointerToThreadObject->vComPortHandle,
						(LPSTR)&PointerToThreadObject->vMyComPortReadDataBuffer, cReceiveBufferSize, PointerToThreadObject->vPointerToThreadOverlappedStructure))
          {
						//Copy Data To Circular Buffer
						WORD TempBufferSpaceFree = 0;
						if (*PointerToThreadObject->vMyComPortReceiveTailPointer == *PointerToThreadObject->vMyComPortReceiveHeadPointer)
							TempBufferSpaceFree = cReceiveBufferSize;
						else
						if (*PointerToThreadObject->vMyComPortReceiveTailPointer < *PointerToThreadObject->vMyComPortReceiveHeadPointer)
							TempBufferSpaceFree = cReceiveBufferSize - *PointerToThreadObject->vMyComPortReceiveHeadPointer + *PointerToThreadObject->vMyComPortReceiveTailPointer;
						else
							TempBufferSpaceFree = *PointerToThreadObject->vMyComPortReceiveTailPointer - *PointerToThreadObject->vMyComPortReceiveHeadPointer;

						if (TempLength < TempBufferSpaceFree)
						{
							//TempReadDataBufferPointer = PointerToThreadObject->vMyComPortReadDataBuffer;
							for (WORD TempLoop = 0; TempLoop < TempLength; TempLoop++)
							{
								TempDataBufferPointer = PointerToThreadObject->vMyComPortReceiveDataBufferPointer + *(PointerToThreadObject->vMyComPortReceiveHeadPointer);

								*TempDataBufferPointer = PointerToThreadObject->vMyComPortReadDataBuffer[TempLoop];
								if (*PointerToThreadObject->vMyComPortReceiveHeadPointer < cReceiveBufferSize - 1)
									*PointerToThreadObject->vMyComPortReceiveHeadPointer = *PointerToThreadObject->vMyComPortReceiveHeadPointer + 1;
								else
									*PointerToThreadObject->vMyComPortReceiveHeadPointer = 0;
							}
						}
						else
						{
							*PointerToThreadObject->vMyComPortReadError = true;
							::PostThreadMessage(PointerToThreadObject->vSerialPortThreadID, cSerialInputOverflowMessage, TempLength, PointerToThreadObject->vSerialPortNumber);
						}


						if ((vGlobaluCSerialPort) && (PointerToThreadObject->vSerialPortNumber == vGlobaluCSerialPort->vComPortNumber)) //is port 1
							vGlobaluCSerialPort->ProcessSerialMessage();
						else //port 2 is for UPS
						if ((vGlobalDXMXRayPort) && (PointerToThreadObject->vSerialPortNumber == vGlobalDXMXRayPort->vComPortNumber)) //is port 3
						{
							if (vGlobalDXMXRayPort)
								vGlobalDXMXRayPort->ProcessSerialMessage();
						}	
						else
						if ((vGlobalRFIDEASBadgeReaderSerialPort) && (PointerToThreadObject->vSerialPortNumber == vGlobalRFIDEASBadgeReaderSerialPort->vComPortNumber)) //is port number configured by inspx person
						{
							if (vGlobalRFIDEASBadgeReaderSerialPort)
								vGlobalRFIDEASBadgeReaderSerialPort->ProcessSerialMessage();
						}	
						else
						if ((vGlobalMonoBlockXRayPort) && (PointerToThreadObject->vSerialPortNumber == vGlobalMonoBlockXRayPort->vComPortNumber)) //is port 3
						{
							if (vGlobalMonoBlockXRayPort)
								vGlobalMonoBlockXRayPort->ProcessSerialMessage();
						}	
						else
						if ((vGlobalACPowerMoniter) && (PointerToThreadObject->vSerialPortNumber == vGlobalACPowerMoniter->vComPortNumber)) //port 4
						{
							if (vGlobalACPowerMoniter)
								vGlobalACPowerMoniter->ProcessSerialMessage();
						}
						else
						if ((vGlobalGuardMaster) && (PointerToThreadObject->vSerialPortNumber == vGlobalGuardMaster->vComPortNumber)) //is port 5
							vGlobalGuardMaster->ProcessSerialMessage();
						else
						//if ((vGlobalPowerFlex40) && (PointerToThreadObject->vSerialPortNumber == vGlobalPowerFlex40->vComPortNumber)) //is port 2 on top downs
						//	vGlobalPowerFlex40->ProcessSerialMessage();
						//else
						{
							::PostThreadMessage(PointerToThreadObject->vSerialPortThreadID,cSerialReceivedDataMessage, TempLength, 0);
						}
          }
       }
       while (TempLength > 0);
    }
	}
	return 0;
}

int NEAR ReadCommBlock( HANDLE TempCommHandle, 
	LPSTR TempBuffer, int TempMaxLength, 
	OVERLAPPED *TempOverlappedStructure )
{
   BOOL       TempReadStatus ;
   COMSTAT    TempComStatus ;
   DWORD      TempErrorFlags;
   DWORD      TempLength;
   DWORD      TempError;




   // only try to read number of bytes in queue
   ClearCommError( TempCommHandle, &TempErrorFlags, &TempComStatus ) ;
   TempLength = min( (DWORD) TempMaxLength, TempComStatus.cbInQue ) ;

   if (TempLength > 0)
   {
      TempReadStatus = ReadFile(TempCommHandle, TempBuffer,
		    TempLength, &TempLength, TempOverlappedStructure);
			
      if (!TempReadStatus)
      {
         if (GetLastError() == ERROR_IO_PENDING)
         {
            //OutputDebugString("\n\rIO Pending");
            // We have to wait for read to complete.
            // This function will timeout according to the
            // CommTimeOuts.ReadTotalTimeoutConstant variable
            // Every time it times out, check for port errors
            while(!GetOverlappedResult( TempCommHandle,
               TempOverlappedStructure, &TempLength, true ))
            {
               TempError = GetLastError();
               if(TempError == ERROR_IO_INCOMPLETE)
                  // normal result if not finished
                  continue;
               else
               {
								 TempLength = 0;
                  // an error occurred, try to recover
                  //wsprintf( szError, "<CE-%u>", dwError ) ;
                  //WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
                  //ClearCommError( COMDEV( npTTYInfo ), &dwErrorFlags, &ComStat ) ;
                  //if ((dwErrorFlags > 0) && DISPLAYERRORS( npTTYInfo ))
                  //{
	                //  wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
	                //  WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
                  //}
                 // break;
               }
            }
	      }
         else
         {
            // some other error occurred
            TempLength = 0 ;
            ClearCommError( TempCommHandle, &TempErrorFlags, &TempComStatus ) ;

         }
      }
   }
   return ( TempLength ) ;
} // end of ReadCommBlock()


