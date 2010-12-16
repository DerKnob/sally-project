////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Ping.cpp
///
/// \brief	Implements the ping class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#define _UNICODE
#include "Ping.h" 

using namespace SallyAPI::Network;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPing::CPing()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CPing::CPing() 
{ 
    bValid = FALSE; 
    WSADATA wsaData;            
    int nRet;          
	PingSuccess = 0;

    // Laden der ICMP.DLL
    hndlIcmp = LoadLibrary("ICMP.DLL"); 
    if (hndlIcmp == NULL) 
    { 
        //MessageBox(NULL, "Could not load ICMP.DLL", "Error:", MB_OK); 
        return; 
    } 
    //ICMP Funktionszeiger werden geholt
    pIcmpCreateFile  = (HANDLE (WINAPI *)()) 
        GetProcAddress((HMODULE)hndlIcmp,"IcmpCreateFile"); 
    pIcmpCloseHandle = (BOOL (WINAPI *)(HANDLE)) 
        GetProcAddress((HMODULE)hndlIcmp,"IcmpCloseHandle"); 
    pIcmpSendEcho = (DWORD (WINAPI *) 
        (HANDLE,DWORD,LPVOID,WORD,PIPINFO,LPVOID,DWORD,DWORD)) 
        GetProcAddress((HMODULE)hndlIcmp,"IcmpSendEcho"); 
    // Wurde die DLL korrekt geladen?
    if (pIcmpCreateFile == NULL        || 
        pIcmpCloseHandle == NULL    || 
        pIcmpSendEcho == NULL) 
    { 
        //MessageBox(NULL, "Error loading ICMP.DLL", "Error:", MB_OK); 
        FreeLibrary((HMODULE)hndlIcmp); 
        return; 
    } 

    // Winsock initialisieren
    nRet = WSAStartup(0x0101, &wsaData ); 
    if (nRet) 
    { 
        MessageBox(NULL, "WSAStartup() error:", "Error:", MB_OK); 
        WSACleanup(); 
        FreeLibrary((HMODULE)hndlIcmp); 
        return; 
    } 
    // Prüfen der WinSock Version 
    if (0x0101 != wsaData.wVersion) 
    { 
        MessageBox(NULL, "No WinSock version 1.1 support found", "Error:", MB_OK); 
        
        FreeLibrary((HMODULE)hndlIcmp); 
        return; 
    } 
    bValid = TRUE; 
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPing::~CPing()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CPing::~CPing() 
{ 
    WSACleanup(); 
    FreeLibrary((HMODULE)hndlIcmp); 
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CPing::GetPingSuccess()
///
/// \brief	Gets the ping success. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The ping success. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CPing::GetPingSuccess() 
{ 
	return PingSuccess;
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CPing::Ping(char* strHost)
///
/// \brief	Pings. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	strHost	If non-null, the string host. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CPing::Ping(char* strHost) 
{ 
    struct in_addr iaDest;        // Struktur der Inet Adresse
    LPHOSTENT pHost;            // Zeider auf den Host 
    DWORD *dwAddress;            // IP Addresse 
    IPINFO ipInfo;                // IP Informationen 
    ICMPECHO icmpEcho;            // ICMP Reply Buffer 
    HANDLE hndlFile;            // Handle für IcmpCreateFile() 

    if(!bValid) 
    { 
		PingSuccess = 0;
        return FALSE; 
    } 

	iaDest.s_addr = inet_addr(strHost); 
    if (iaDest.s_addr == INADDR_NONE) 
        pHost = gethostbyname(strHost); 
    else 
        pHost = gethostbyaddr((const char *)&iaDest, 
                        sizeof(struct in_addr), AF_INET); 
    if (pHost == NULL) 
    { 
		PingSuccess = 0;
        return FALSE; 
    } 

    // Kopieren der IP
    dwAddress = (DWORD *)(*pHost->h_addr_list); 

    // Handle für IcmpSendEcho()         
    hndlFile = pIcmpCreateFile(); 

    // Standardwerte 
    ipInfo.Ttl = 255; 
    ipInfo.Tos = 0; 
    ipInfo.IPFlags = 0; 
    ipInfo.OptSize = 0; 
    ipInfo.Options = NULL; 
    icmpEcho.Status = 0; 
    // Icmp Echoanfrage 
    pIcmpSendEcho( 
        hndlFile,        // Handle von IcmpCreateFile() 
        *dwAddress,        // Zieladresse 
        NULL,             
        0,                 
        &ipInfo,         
        &icmpEcho,        // Buffer 
        sizeof(struct tagICMPECHO), 
        1000);            // Time to wait in milliseconds 

    iaDest.s_addr = icmpEcho.Source; 
    if (icmpEcho.Status) 
    { 
		PingSuccess = 0;
        return FALSE; 
    } 

	

    // Schliessen des Handles
    pIcmpCloseHandle(hndlFile); 
	

	PingSuccess = 1;
    return TRUE; 
} 