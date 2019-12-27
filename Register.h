/******************************************************************************
 *
 * MODULE   
 *		Register.h
 *
 * REVISION INFORMATION  
 *		$Logfile: /ifc/include/Register.h $
 *		$Revision: 14 $
 *		$Modtime: 10/14/99 2:39p $
 *
 * ABSTRACT  
 *		IFC interface of the CRegister class.
 *
 * TECHNICAL NOTES 
 *
 *
 * Copyright 1998-1999 (c) Imaging Technology, Inc.  All rights reserved.
 *
 ******************************************************************************/

#if !defined(AFX_REGISTER_H__E554E635_93FD_11D1_AE72_00A0C959E757__INCLUDED_)
#define AFX_REGISTER_H__E554E635_93FD_11D1_AE72_00A0C959E757__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <ifcstd.h>
#include <ifcclass.h>

typedef struct ifcmodreg IFC_MODREG;
typedef struct ifcmodreg * pIFC_MODREG;
typedef DWORD REGID; 

typedef CRegister *PCRegister;

#define IFC_ALL_REGISTERS 0x40000000

#include <IFCBus.h>
#include <module.h>


#define REGNAME(name) #name,name


/**************************************************************************
**
** MODREG -- Module Register Structure.
*/

typedef WORD    (*IFC_RHANDLER)(pCModule,short);
typedef void    (*IFC_WHANDLER)(pCModule,short,WORD );

typedef BYTE (*IFC_RBYTEH)(pCModule,WORD);
typedef void (*IFC_WBYTEH)(pCModule,WORD,BYTE);
typedef WORD (*IFC_RWORDH)(pCModule,WORD);
typedef void (*IFC_WWORDH)(pCModule,WORD,WORD);
typedef DWORD (*IFC_RDWORDH)(pCModule,WORD);
typedef void  (*IFC_WDWORDH)(pCModule, WORD, DWORD);

/**********************************************************************
**             Register symbol table (BYTE) flags bit definitions
**********************************************************************/

/* Bit 0 */
#define         IFC_GEN_ENTRY   0x00    /* llfuncs generic type register */
#define         IFC_SPEC_ENTRY  0x01    /* special handler type register */

/* Bit 1 */
#define         IFC_NOBASE      0x00    /* leave adr offset as is */
#define         IFC_ADDBASE     0x02    /* add mapped base adr to adr offset */

/* Bit 2 and Bit 3 */
#define                 IFC_DATASZ_BITS 0x0C                    /* used as a mask   */
#define         IFC_DATASZ_16     0x00
#define         IFC_DATASZ_32     0x04
#define         IFC_DATASZ_8      0x08
#define         IFC_DATASZ_64     0x0C

/* Bit 4 */
/* Bit 5 */     /* future expansion */
/* Bit 6 */
/* Bit 7 */

#define DGHT_ADRSPACE 0xff
#define PCI_CONFIG_ADRSPACE 0xfe

// struct ifcmodreg - this type is used to define static register definitions in the module register table
struct ifcmodreg {
	pSTRING			regname;		/* Name of the register */
	DWORD			regID;
	IFC_RHANDLER    rhandler;       /* read register function handler */
	IFC_WHANDLER    whandler;       /* write register function handler */
	DWORD           adr;            /* decoded address to register */
	DWORD           mask;           /* mask which indicates relevant bits */
	BYTE            shift;          /* shift to first valid bit in mask */
	BYTE            flags;          /* bit flags: see above */
	BYTE			adrSpace;		/* 0 - 5 for parent mod reg, 0xff for all daughter mod reg */
};


// DIFCMODREG - this type defines the dynamic register structure formed during register install
// WriteReg/ReadReg funtions deal with structures of this type
typedef struct  {
	pSTRING			regname;		/* Name of the register */
	IFC_RHANDLER	rhandler;       /* read register function handler */
	IFC_WHANDLER	whandler;       /* write register function handler */
	DWORD           adr;            /* decoded address to register */
	DWORD           mask;           /* mask which indicates relevant bits */
	BYTE            shift;          /* shift to first valid bit in mask */
	BYTE            flags;          /* bit flags: see above */
	BYTE			adrSpace;		/* 0 - 5 for parent mod reg, 0xff for all daughter mod reg */
	DWORD			physicalAddress;
} DIFCMODREG,*PDIFCMODREG;


#ifdef __cplusplus
class IFC_EXT_API CRegister  
{
public:
	pSTRING RegName(REGID reg);
	virtual BOOL RegInstall(DWORD regID,pIFC_MODREG stat_mr,DWORD base,DWORD physBase,CIFCBus *bus);
	DWORD ReadReg(REGID reg);
	void WriteReg(REGID reg,DWORD val);
	void DisplayReg(BOOL verbose=false,REGID reg=IFC_ALL_REGISTERS);
	CRegister(pCModule mod,DWORD numRegs);
	virtual ~CRegister();
	PDIFCMODREG m_regTable;
	DWORD m_numRegs;
	pCModule m_mod;
};
#endif

#endif // !defined(AFX_REGISTER_H__E554E635_93FD_11D1_AE72_00A0C959E757__INCLUDED_)
