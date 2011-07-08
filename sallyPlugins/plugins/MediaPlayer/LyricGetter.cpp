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

void CLyricGetter::SetStaticValues(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CLabelBox* labelBox)
{
	m_pLabelBox = labelBox;
	m_pAppBase = appBase;
}

void CLyricGetter::RunEx()
{
	std::string uid = SallyAPI::System::SystemHelper::GenerateUniqueID();
	std::string tempFile = SallyAPI::System::SallyHelper::GetMediaDirectory(m_pAppBase);
	tempFile.append(uid);
	tempFile.append(".xml");

	std::string xml = "";
	GetXML(&xml);
	if (xml.length() == 0)
		return;

	DeleteFile(tempFile.c_str());

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, xml);

	if (!SallyAPI::File::FileHelper::FileExists(tempFile))
		return;

	XMLNode xMainNode = XMLNode::openFileHelper(tempFile.c_str());
	if (xMainNode.isEmpty())
		return;

	XMLNode itemGetLyricResult = xMainNode.getChildNode("GetLyricResult");
	if (itemGetLyricResult.isEmpty())
		return;

	XMLNode lyric = itemGetLyricResult.getChildNode("Lyric");
	if (lyric.isEmpty())
		return;

	const char* lyricsText = lyric.getText();

	if (lyricsText != NULL)
		m_pLabelBox->SetText(lyricsText);

	// cleanup
	DeleteFile(tempFile.c_str());
}

void CLyricGetter::GetXML(std::string* response)
{
	std::string request = "/apiv1.asmx/";
	request.append("SearchLyricDirect?artist=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(m_strArtist));
	request.append("&song=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(m_strTitle));

	int byteRead = 0;

	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NetworkHelper::GetHTTPText("api.chartlyrics.com", 80, request, &byteRead, response, proxy, proxyBypass, 0, 25);
	return;
}