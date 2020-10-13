/**
	@file	adsClient.h
	@class	CadsClient
	@date	2016/01/09
	@author Dong-hyun Lee(phenom8305@gmail.com)
	@brief	- This is a Basic class to communicate the TwinCAT system

	Copyright (C) 2015. Dong-hyun Lee all rights reserved.
*/
#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsDef.h"
#include "C:\TwinCAT\AdsApi\TcAdsDll\Include\TcAdsApi.h"

using namespace std;

#define IO1_ADS_PORT_NUM	350
#define IO2_ADS_PORT_NUM	351
#define IO3_ADS_PORT_NUM	352
#define NC_ADS_PORT_NUM		500
#define PLC1_ADS_PORT_NUM	851
#define PLC2_ADS_PORT_NUM	852
#define PLC3_ADS_PORT_NUM	853
#define PLC4_ADS_PORT_NUM	854

typedef enum AdsDataTypeId
{
	ADST_VOID = VT_EMPTY,
	ADST_INT8 = VT_I1,
	ADST_UINT8 = VT_UI1,
	ADST_INT16 = VT_I2,
	ADST_UINT16 = VT_UI2,
	ADST_INT32 = VT_I4,
	ADST_UINT32 = VT_UI4,
	ADST_INT64 = VT_I8,
	ADST_UINT64 = VT_UI8,
	ADST_REAL32 = VT_R4,    
	ADST_REAL64 = VT_R8,
	ADST_STRING = VT_LPSTR,
	ADST_WSTRING = VT_LPWSTR,
	ADST_REAL80 = VT_LPWSTR+1,
	ADST_BIT = VT_LPWSTR+2,
	ADST_BIGTYPE = VT_BLOB,
	ADST_MAXTYPES = VT_STORAGE,
} ADS_DATATYPE;

class CadsClient
{
private:


	AdsSymbolUploadInfo tAdsSymbolInfo;
	char *pTotalSymbols;
	PAdsSymbolEntry* pEachSymbols;

	ULONG* hNotification;
	ULONG* hSymbols;

	AdsVersion* pDLLVersion;
	
	WORD idleData;

	void readAdsDllVer();

public:

	AmsAddr SrvAddr, ModuleAddr;
	PAmsAddr pSrvAddr, pModuleAddr;

	CadsClient();
	~CadsClient();
	long Connect(int SrvPort=IO1_ADS_PORT_NUM);
	long setModulePort();
	long Close();
	long loadSymbolList();
	void sortSymbolListByOffset();
	long getAllSymbolHandle();
	long delAllSymbolHandle();
	long readByHandle(int index, PVOID pData);
	long readByHandle(int index, PVOID pData, ULONG DataSize);
	long writeByHandle(int index, PVOID pData);
	long readBySymbol(int index, PVOID pData);
	long writeBySymbol(int index, PVOID pData);
	long readModule(ADS_UINT32 iGroup, ADS_UINT32 iOffset, ULONG rLength, PVOID pData);
	long writeModule(ADS_UINT32 iGroup, ADS_UINT32 iOffset, ULONG wLength, PVOID pData);
	long addNotification(int i, PAdsNotificationFuncEx pFunc, ULONG cycleTime=1, USHORT TransMode=ADSTRANS_SERVERONCHA);
	long delNotification(int i);

	//Output Part
	void printSymbols_console();
	string printSymbols(long index);
	int AdsDllVer()				{return (int)pDLLVersion->version;}
	int AdsDllreVis()			{return (int)pDLLVersion->revision;}
	USHORT AdsDllbuild()		{return (USHORT)pDLLVersion->build;}
	const ULONG SymbolHandle(int i) const {return hSymbols[i];}
	long SymbolNum()			{return (long)tAdsSymbolInfo.nSymbols;}
};