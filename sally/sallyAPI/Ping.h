////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Ping.h
///
/// \brief	Declares the ping class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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

#pragma  once

#pragma comment( lib, "Ws2_32.lib" )

#include "Define.h"
#include <stdio.h>
#include <string>

namespace SallyAPI
{
	namespace Network
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \struct	tagIPINFO
		///
		/// \brief	Tag ipinfo. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		typedef struct tagIPINFO 
		{ 
			u_char Ttl;                // Time To Live 
			u_char Tos;                // Type Of Service 
			u_char IPFlags;            // IP flags 
			u_char OptSize;            // Size of options data 
			u_char FAR *Options;    // Options data buffer 
			
		}IPINFO; 

		typedef IPINFO* PIPINFO; 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \struct	tagICMPECHO
		///
		/// \brief	Tag icmpecho. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		typedef struct tagICMPECHO 
		{ 
			u_long Source;            // Quelladresse
			u_long Status;            // IP status 
			u_long RTTime;             
			u_short DataSize;        // Größe der Antwort 
			u_short Reserved;        
			void FAR *pData;        // Buffer für die Antwort 
			IPINFO    ipInfo;             
		}ICMPECHO; 

		typedef ICMPECHO* PICMPECHO; 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CPing
		///
		/// \brief	Ping. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CPing 
		{ 
		public: 
			CPing(); 
			~CPing(); 
			BOOL Ping(char* strHost); 
			int	GetPingSuccess();
		private: 
		    
			HANDLE (WINAPI *pIcmpCreateFile)(); 
			BOOL (WINAPI *pIcmpCloseHandle)(HANDLE); 
			DWORD (WINAPI *pIcmpSendEcho) 
				(HANDLE,DWORD,LPVOID,WORD,PIPINFO,LPVOID,DWORD,DWORD); 
			HANDLE hndlIcmp;            // LoadLibrary() handle 
			BOOL bValid;
			int PingSuccess;
		}; 
	}
}

