#pragma once

#ifdef GETCORETEMPINFO_EXPORTS
#define GETCORETEMPINFO_API __declspec(dllexport)
#else
#define GETCORETEMPINFO_API __declspec(dllimport)
#endif

#define UNKNOWN_EXCEPTION 0x20000000

typedef struct core_temp_shared_data
{
	unsigned int	uiLoad[256];
	unsigned int	uiTjMax[128];
	unsigned int	uiCoreCnt;
	unsigned int	uiCPUCnt;
	float			fTemp[256];
	float			fVID;
	float			fCPUSpeed;
	float			fFSBSpeed;
	float			fMultipier;	
	char			sCPUName[100];
	unsigned char	ucFahrenheit;
	unsigned char	ucDeltaToTjMax;
}CORE_TEMP_SHARED_DATA,*PCORE_TEMP_SHARED_DATA,**PPCORE_TEMP_SHARED_DATA;

bool GETCORETEMPINFO_API fnGetCoreTempInfo(CORE_TEMP_SHARED_DATA *&pData);
bool WINAPI fnGetCoreTempInfoAlt(CORE_TEMP_SHARED_DATA *pData);

class GETCORETEMPINFO_API CoreTempProxy
{
public:
	CoreTempProxy(void);
	virtual ~CoreTempProxy(void);
	
	unsigned int GetCoreLoad(int Index) const;
    unsigned int GetTjMax(int Index) const;
    unsigned int GetCoreCount() const;
    unsigned int GetCPUCount() const;
    float GetTemp(int Index) const;
    float GetVID() const;
    float GetCPUSpeed() const;
    float GetFSBSpeed() const;
    float GetMultiplier() const;
    char* GetCPUName() const;
    bool IsFahrenheit() const;
    bool IsDistanceToTjMax() const;
    const CORE_TEMP_SHARED_DATA &GetDataStruct() const;

	bool GetData();
	unsigned long GetDllError() const { return GetLastError(); }
	wchar_t* GetErrorMessage();
private:

	CORE_TEMP_SHARED_DATA m_pCoreTempData;
	wchar_t m_ErrorMessage[100];
};
