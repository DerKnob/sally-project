////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\LyricGetter.cpp
///
/// \brief	Implements the lyric getter class. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LyricGetter.h"

#pragma comment(lib, "xmlparser.lib")
#include <xmlParser.h>

CLyricGetter::CLyricGetter()
{
}

CLyricGetter::~CLyricGetter()
{
}


void CLyricGetter::SetValues(const std::string& artist, const std::string& title)
{
	m_strArtist = artist;
	m_strTitle = title;
}

void CLyricGetter::SetStaticValues(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CGUIBaseObject* parent)
{
	m_pParent = parent;
	m_pAppBase = appBase;
}

void CLyricGetter::RunEx()
{
	std::string uid = SallyAPI::System::SystemHelper::GenerateUniqueID();
	std::string tempFile = SallyAPI::System::SallyHelper::GetMediaDirectory(m_pAppBase);
	tempFile.append(uid);
	tempFile.append(".xml");

	// cleanup
	DeleteFile(tempFile.c_str());

	if (!ProcessFile(tempFile))
	{
		SallyAPI::GUI::SendMessage::CParameterKeyValue parameter(this->GetId(), m_strErrorText);
		m_pParent->SendMessageToParent(m_pParent, 0, GUI_APP_LYRICS_LOADED, &parameter);
	}

	// cleanup
	DeleteFile(tempFile.c_str());
}

bool CLyricGetter::ProcessFile(const std::string& tempFile)
{
	std::string xml = "";
	SallyAPI::Network::NETWORK_RETURN errorCode = GetXML(&xml);

	if (errorCode != SallyAPI::Network::SUCCESS)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Debug("CLyricGetter::ProcessFile::GetXML not successful");
		logger->Debug(errorCode);
		logger->Debug(GetRequestURL());

		switch (errorCode)
		{
		case SallyAPI::Network::ERROR_PREPARE:
			m_strErrorText = "Network preparation failed";
		case SallyAPI::Network::ERROR_OPEN:
			m_strErrorText = "Network open failed";
		case SallyAPI::Network::ERROR_HTTP_TIMEOUT:
			m_strErrorText = "HTTP Timeout";
		case SallyAPI::Network::ERROR_NOTHING_READ:
			m_strErrorText = "Nothing read";
		default:
			break;
		}

		return false;
	}

	if (xml.length() == 0)
	{
		m_strErrorText = "Invalide Server response";
		return false;
	}

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, xml);

	if (!SallyAPI::File::FileHelper::FileExists(tempFile))
	{
		m_strErrorText = "Invalide Server response";
		return false;
	}

	XMLNode xMainNode = XMLNode::parseFile(tempFile.c_str());
	if (xMainNode.isEmpty())
	{
		m_strErrorText = "Invalide Server response";
		return false;
	}

	XMLNode itemGetLyricResult = xMainNode.getChildNode("GetLyricResult");
	if (itemGetLyricResult.isEmpty())
	{
		m_strErrorText = "No Lyric found";
		return false;
	}

	XMLNode lyric = itemGetLyricResult.getChildNode("Lyric");
	if (lyric.isEmpty())
	{
		m_strErrorText = "No Lyric found";
		return false;
	}

	const char* lyricsText = lyric.getText();

	if (lyricsText == NULL)
	{
		m_strErrorText = "No Lyric found";
		return false;
	}

	SallyAPI::GUI::SendMessage::CParameterKeyValue parameter(this->GetId(), lyricsText);
	m_pParent->SendMessageToParent(m_pParent, 0, GUI_APP_LYRICS_LOADED, &parameter);
	return true;
}

SallyAPI::Network::NETWORK_RETURN CLyricGetter::GetXML(std::string* response)
{
	std::string request = GetRequestURL();

	int byteRead = 0;

	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NETWORK_RETURN networkReturn = SallyAPI::Network::NetworkHelper::GetHTTPText(
		"api.chartlyrics.com", 80, request, &byteRead, response, proxy, proxyBypass, 0, 25);
	return networkReturn;
}

std::string CLyricGetter::GetRequestURL()
{
	std::string request = "/apiv1.asmx/";
	request.append("SearchLyricDirect?artist=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(m_strArtist));
	request.append("&song=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(m_strTitle));

	return request;
}