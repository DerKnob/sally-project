////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SallyHelper.cpp
///
/// \brief	Implements the sally helper class. 
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

#include "SallyHelper.h"

using namespace SallyAPI::System;

std::string SallyAPI::System::SallyHelper::m_strMediaWorkingDir;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyHelper::SallyHelper()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyHelper::SallyHelper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyHelper::~SallyHelper()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyHelper::~SallyHelper()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string SallyHelper::GetMediaDirectory(SallyAPI::GUI::CAppBase* appBase)
///
/// \brief	Gets a media directory. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	appBase	If non-null, the application base. 
///
/// \return	The media directory. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string SallyHelper::GetMediaDirectory(SallyAPI::GUI::CAppBase* appBase)
{	
	return GetMediaDirectory(appBase->GetExplicitAppName());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string SallyHelper::GetMediaDirectory(const std::string& explicidAppName)
///
/// \brief	Gets a media directory. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
///
/// \return	The media directory. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string SallyHelper::GetMediaDirectory(const std::string& explicidAppName)
{
	if (m_strMediaWorkingDir.length() == 0)
	{		
		m_strMediaWorkingDir = SallyAPI::Core::CGame::GetMediaFolder();

		m_strMediaWorkingDir.append("applications\\");
		CreateDirectory(m_strMediaWorkingDir.c_str(), 0);
	}
	std::string modulePath = m_strMediaWorkingDir;

	modulePath.append(explicidAppName);
	modulePath.append("\\");

	CreateDirectory(modulePath.c_str(), 0);

	return modulePath;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int SallyHelper::GetFlagImageId(const std::string& iso)
///
/// \brief	Gets a flag image identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iso	The iso. 
///
/// \return	The flag image identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int SallyHelper::GetFlagImageId(const std::string& iso)
{
	std::string isoLower = SallyAPI::String::StringHelper::StringToLower(iso);
	if (isoLower.compare("de") == 0)
		return GUI_THEME_SALLY_FLAG_DE;
	if (isoLower.compare("en") == 0)
		return GUI_THEME_SALLY_FLAG_EN;
	if (isoLower.compare("fr") == 0)
		return GUI_THEME_SALLY_FLAG_FR;
	if (isoLower.compare("es") == 0)
		return GUI_THEME_SALLY_FLAG_ES;
	if (isoLower.compare("it") == 0)
		return GUI_THEME_SALLY_FLAG_IT;
	if (isoLower.compare("at") == 0)
		return GUI_THEME_SALLY_FLAG_AT;
	if (isoLower.compare("ca") == 0)
		return GUI_THEME_SALLY_FLAG_CA;
	if (isoLower.compare("ch") == 0)
		return GUI_THEME_SALLY_FLAG_CH;

	// Not Found
	return GUI_NO_IMAGE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string SallyHelper::GetProxy()
///
/// \brief	Gets the proxy. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The proxy. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string SallyHelper::GetProxy()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if (option->GetPropertyBool("network", "useInternetExplorer", true) == false)
	{
		return option->GetPropertyString("network", "proxyServer", "");
	}
	else
	{
		unsigned long nSize = 4096;
		char szBuf[4096] =
		{
			0
		};
		INTERNET_PROXY_INFO* pInfo = (INTERNET_PROXY_INFO*)szBuf;
		if (!InternetQueryOption(NULL, INTERNET_OPTION_PROXY, pInfo, &nSize))
		{
			return "";
		}

		if (pInfo->dwAccessType != INTERNET_OPEN_TYPE_DIRECT)
		{
			std::string proxy = pInfo->lpszProxy;
			return proxy;
		}
	}
	// No proxy set
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string SallyHelper::GetProxyBypass()
///
/// \brief	Gets the proxy bypass. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The proxy bypass. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string SallyHelper::GetProxyBypass()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if (option->GetPropertyBool("network", "useInternetExplorer", true) == false)
	{
		return option->GetPropertyString("network", "proxyBypass", "");
	}
	else
	{
		unsigned long nSize = 4096;
		char szBuf[4096] =
		{
			0
		};
		INTERNET_PROXY_INFO* pInfo = (INTERNET_PROXY_INFO*)szBuf;
		if (!InternetQueryOption(NULL, INTERNET_OPTION_PROXY, pInfo, &nSize))
		{
			return "";
		}

		if (pInfo->dwAccessType != INTERNET_OPEN_TYPE_DIRECT)
		{
			std::string proxyBypass = pInfo->lpszProxyBypass;
			return proxyBypass;
		}
		// No proxyBypass set
	}
	return "";
}