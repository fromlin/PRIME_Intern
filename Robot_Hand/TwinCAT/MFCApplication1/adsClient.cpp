#include "stdafx.h"
#include "adsClient.h"

CadsClient::CadsClient(): idleData(0)
{
	pEachSymbols = NULL;
	pTotalSymbols = NULL;
	hSymbols = NULL;
	hNotification = NULL;
}

CadsClient::~CadsClient()
{
	if (pEachSymbols) delete[] pEachSymbols;
	if (pTotalSymbols) delete(pTotalSymbols);
	if (hSymbols) delete[] hSymbols;
	if (hNotification) delete[] hNotification;
	Close();
}

void CadsClient::readAdsDllVer()
{
	long nTemp = AdsGetDllVersion();
	pDLLVersion = (AdsVersion *)&nTemp;
}

long CadsClient::Connect(int SrvPort/* =IO1_ADS_PORT_NUM */)
{
	long nErr=0;
	pSrvAddr = &SrvAddr;
	pModuleAddr = &ModuleAddr;

	// Open communication port on the ADS router (Local Address: 127.0.0.1.1.1)
	nErr = AdsPortOpen();
	nErr = AdsGetLocalAddress(pSrvAddr);
	if (nErr != 0) return nErr;
	pSrvAddr->port = SrvPort;
	pModuleAddr->netId = pSrvAddr->netId;
	readAdsDllVer();
	loadSymbolList();
	sortSymbolListByOffset();
	getAllSymbolHandle();
	setModulePort();
	return nErr;
}

long CadsClient::setModulePort()
{
	long nErr;
	string strBuffer;
	WORD port=0;
	for (ULONG i=0; i<tAdsSymbolInfo.nSymbols-1; i++)
	{
		strBuffer = PADSSYMBOLNAME(pEachSymbols[i]);
		if (strBuffer.substr(strBuffer.find('.'))==".AdsPort")
		{
			nErr = readBySymbol(i, &port);
			pModuleAddr->port = port;
			break;
		}
	}
	return 0;
}

long CadsClient::Close()
{
	delAllSymbolHandle();
	return AdsPortClose();
}

long CadsClient::loadSymbolList()
{
	long nErr;

	// Read the Size and Number of the Symbols variable declaration
	nErr = AdsSyncReadReq(pSrvAddr, ADSIGRP_SYM_UPLOADINFO, 0x0, sizeof(tAdsSymbolInfo), &tAdsSymbolInfo);
	if (nErr) return nErr;

	// Save total Symbols
	pTotalSymbols = new char[tAdsSymbolInfo.nSymSize];
	nErr = AdsSyncReadReq(pSrvAddr, ADSIGRP_SYM_UPLOAD, 0, tAdsSymbolInfo.nSymSize, pTotalSymbols); 
	if (nErr) return nErr;

	// Sorting Total Symbols and Saving each symbols
	pEachSymbols = new PAdsSymbolEntry[tAdsSymbolInfo.nSymbols];
	pEachSymbols[0] = (PAdsSymbolEntry)pTotalSymbols;
	for (ULONG i=0; i<tAdsSymbolInfo.nSymbols-1; i++)
		pEachSymbols[i+1] = PADSNEXTSYMBOLENTRY(pEachSymbols[i]);	

	return nErr;
}

void CadsClient::sortSymbolListByOffset()
{// Insertion Sort method(Algorithm)
	string strBuffer;
	PAdsSymbolEntry tempSymbolBuff;
	
	for (UINT i=1; i<tAdsSymbolInfo.nSymbols; i++)
	{
		tempSymbolBuff = pEachSymbols[i];
		for (UINT j=i-1; j>=0 && pEachSymbols[j]->iOffs>tempSymbolBuff->iOffs; j--)
		{
			pEachSymbols[j+1]=pEachSymbols[j];
			pEachSymbols[j]=tempSymbolBuff;
		}
	}
}

long CadsClient::getAllSymbolHandle()
{
	long nErr;

	hSymbols = new ULONG[tAdsSymbolInfo.nSymbols];
	hNotification = new ULONG[tAdsSymbolInfo.nSymbols];

	//Fetch handle of varName
	for(ULONG i=0; i<tAdsSymbolInfo.nSymbols; i++)
	{
		nErr = AdsSyncReadWriteReq(pSrvAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(ULONG), &hSymbols[i], pEachSymbols[i]->nameLength, PADSSYMBOLNAME(pEachSymbols[i]));
		if (nErr) return nErr;
	}

	return nErr;
}

long CadsClient::delAllSymbolHandle()
{
	long nErr;

	//Release handle of variable
	for(ULONG i=0; i<tAdsSymbolInfo.nSymbols; i++)
	{
		nErr = AdsSyncWriteReq(pSrvAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(ULONG), &hSymbols[i]);
		if (nErr) return nErr;
	}

	return nErr;
}

long CadsClient::readByHandle(int i, PVOID pData)
{
	//Read value of variable by handle
	return AdsSyncReadReq(pSrvAddr, ADSIGRP_SYM_VALBYHND, hSymbols[i], pEachSymbols[i]->size, pData); 
}

long CadsClient::readByHandle(int i, PVOID pData, ULONG DataSize)
{
	//Read value of variable by handle
	return AdsSyncReadReq(pSrvAddr, ADSIGRP_SYM_VALBYHND, hSymbols[i], DataSize, pData); 
}

long CadsClient::writeByHandle(int i, PVOID pData)
{
	return AdsSyncWriteReq(pSrvAddr, ADSIGRP_SYM_VALBYHND, hSymbols[i], pEachSymbols[i]->size, pData);
}

long CadsClient::writeByHandle(int i, PVOID pData, ULONG DataSize)
{
	return AdsSyncWriteReq(pSrvAddr, ADSIGRP_SYM_VALBYHND, hSymbols[i], DataSize, pData);
}

long CadsClient::readBySymbol(int i, PVOID pData)
{
	return AdsSyncReadReq(pSrvAddr, pEachSymbols[i]->iGroup, pEachSymbols[i]->iOffs, pEachSymbols[i]->size, pData);
}

long CadsClient::writeBySymbol(int i, PVOID pData)
{
	return AdsSyncWriteReq(pSrvAddr, pEachSymbols[i]->iGroup, pEachSymbols[i]->iOffs, pEachSymbols[i]->size, pData);
}

long CadsClient::readModule(ADS_UINT32 iGroup, ADS_UINT32 iOffset, ULONG rLength, PVOID pData)
{
	return AdsSyncReadWriteReq(pModuleAddr, iGroup, iOffset, rLength, pData, sizeof(idleData), &idleData);
}

long CadsClient::writeModule(ADS_UINT32 iGroup, ADS_UINT32 iOffset, ULONG wLength, PVOID pData)
{
	return AdsSyncReadWriteReq(pModuleAddr, iGroup, iOffset, sizeof(idleData), &idleData, wLength, pData);
}

long CadsClient::addNotification(int i, PAdsNotificationFuncEx pFunc, ULONG cycleTime, USHORT TransMode)
{
	AdsNotificationAttrib adsNotificationAttrib;

	// set the attributes of the notification
	adsNotificationAttrib.cbLength = pEachSymbols[i]->size;
	adsNotificationAttrib.nTransMode = (ADSTRANSMODE)TransMode; //ADSTRANS_SERVERONCHA;
	adsNotificationAttrib.nMaxDelay = 0;
	adsNotificationAttrib.nCycleTime = cycleTime; //10000000 : 1sec

	return AdsSyncAddDeviceNotificationReq(pSrvAddr, ADSIGRP_SYM_VALBYHND, hSymbols[i], &adsNotificationAttrib, pFunc, hSymbols[i], &hNotification[i]);
}

long CadsClient::delNotification(int i)
{
	return AdsSyncDelDeviceNotificationReq(pSrvAddr, hNotification[i]);
}

void CadsClient::printSymbols_console()
{
	string strBuffer;

	strBuffer = PADSSYMBOLNAME(pEachSymbols[0]);
	cout<<"Common Name: "<<strBuffer.substr(0,strBuffer.find('.'))<<endl;
	for (ULONG i=0; i<tAdsSymbolInfo.nSymbols; i++)
	{
		cout<< i << " : ";
		strBuffer = PADSSYMBOLNAME(pEachSymbols[i]);
		cout<<strBuffer.substr(strBuffer.find('.'))<<'\t';
		cout<<hex;
		cout<<"0x"<<pEachSymbols[i]->iGroup<<'\t';
		cout<<"0x"<<pEachSymbols[i]->iOffs <<'\t';
		cout<<dec;
		cout<<pEachSymbols[i]->size<<'\t';
		cout<<PADSSYMBOLTYPE(pEachSymbols[i]) <<'\t';
		cout<<pEachSymbols[i]->dataType<<'\t';
		cout<<PADSSYMBOLCOMMENT(pEachSymbols[i])<<endl;
	}
}

string CadsClient::printSymbols(long index)
{
	string strBuffer;
	strBuffer  = "["+to_string((LONGLONG)index)+"] ";
	strBuffer += PADSSYMBOLNAME(pEachSymbols[index]);
	return strBuffer;
}