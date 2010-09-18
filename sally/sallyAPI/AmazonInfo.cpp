////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\AmazonInfo.cpp
///
/// \brief	Implements the amazon information class. 
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

#include "AmazonInfo.h"

#pragma comment(lib, "xmlparser.lib")
#include <xmlParser.h>

using namespace SallyAPI::Media;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CAmazonInfo::CAmazonInfo()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	20.05.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CAmazonInfo::CAmazonInfo()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CAmazonInfo::~CAmazonInfo()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	20.05.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CAmazonInfo::~CAmazonInfo()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CAmazonInfo::GetCover(std::string artist, std::string album, std::string outputFile,
/// COVER_SIZE size)
///
/// \brief	Gets a cover. 
///
/// \author	Christian Knobloch
/// \date	20.05.2010
///
/// \param [in,out]	artist		The artist. 
/// \param [in,out]	album		The album. 
/// \param [in,out]	outputFile	The output file. 
/// \param	size				The size. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CAmazonInfo::GetCover(std::string artist, std::string album, std::string outputFile, COVER_SIZE size)
{
	std::string tempFile = outputFile;
	tempFile.append(".xml");

	bool result = GetCoverInternal(artist, album, outputFile, tempFile, size);

	if (!result)
	{
		CleanupStrings(artist, album);
	
		result = GetCoverInternal(artist, album, outputFile, tempFile, size);
	}

	DeleteFile(tempFile.c_str());
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CAmazonInfo::CleanupStrings(std::string& artist, std::string& album)
///
/// \brief	Cleanup the request strings. 
///
/// \author	Christian Knobloch
/// \date	08.07.2010
///
/// \param [in,out]	artist	The artist. 
/// \param [in,out]	album	The album. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CAmazonInfo::CleanupStrings(std::string& artist, std::string& album)
{
	artist = SallyAPI::String::StringHelper::RemoveStringBetween(artist, "(", ")");
	artist = SallyAPI::String::StringHelper::RemoveStringBetween(artist, "[", "]");
	artist = SallyAPI::String::StringHelper::ReplaceString(artist, "'", "");

	album = SallyAPI::String::StringHelper::RemoveStringBetween(album, "(", ")");
	album = SallyAPI::String::StringHelper::RemoveStringBetween(album, "[", "]");

	album = SallyAPI::String::StringHelper::ReplaceString(album, "'", "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CAmazonInfo::NormalizeString(const std::string& in)
///
/// \brief	Normalize the input string. 
///
/// \author	Christian Knobloch
/// \date	08.07.2010
///
/// \param	in	The in. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CAmazonInfo::NormalizeString(const std::string& in)
{
	std::string result = SallyAPI::String::StringHelper::StringToLower(in);

	result = SallyAPI::String::StringHelper::ReplaceString(result, " ", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "&amp;", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "&lt;", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "&gt;", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "&", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, ":", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, ";", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, ",", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "-", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "_", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "+", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "\\", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "\"", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "'", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "*", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "%", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "=", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "/", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "`", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "´", "");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "ö", "o");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "ä", "a");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "ü", "u");
	result = SallyAPI::String::StringHelper::ReplaceString(result, "ß", "ss");
	result = SallyAPI::String::StringHelper::RemoveStringBetween(result, "(", ")");
	result = SallyAPI::String::StringHelper::RemoveStringBetween(result, "[", "]");


	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CAmazonInfo::GetCoverInternal(std::string& artist, std::string& album,
/// std::string& outputFile, std::string& tempFile, COVER_SIZE size)
///
/// \brief	Gets a cover internal. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param [in,out]	artist		The artist. 
/// \param [in,out]	album		The album. 
/// \param [in,out]	outputFile	The output file. 
/// \param [in,out]	tempFile	The temp file. 
/// \param	size				The size. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CAmazonInfo::GetCoverInternal(std::string& artist, std::string& album, std::string& outputFile, std::string& tempFile, COVER_SIZE size)
{
	std::string xml = "";
	GetXML(artist, album, &xml);
	if (xml.length() == 0)
		return false;

	// create all variables
	std::string artistLow = NormalizeString(artist);
	std::string albumLow = NormalizeString(album);

	std::string completeURL = "";

	// delete the old file
	DeleteFile(tempFile.c_str());

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, xml);

	if (!SallyAPI::File::FileHelper::FileExists(tempFile))
		return false;

	XMLNode xMainNode = XMLNode::openFileHelper(tempFile.c_str());
	if (xMainNode.isEmpty())
		return false;

	XMLNode itemSearchResponse = xMainNode.getChildNode("ItemSearchResponse");
	if (itemSearchResponse.isEmpty())
		return false;

	XMLNode items = itemSearchResponse.getChildNode("Items");
	if (items.isEmpty())
		return false;

	// Item
	XMLNode item;
	int i = 0;
	do
	{
		item = items.getChildNode("Item", i);

		if (!item.isEmpty())
		{
			// ItemAttributes
			XMLNode itemAttributes = item.getChildNode("ItemAttributes");
			if (!itemAttributes.isEmpty())
			{
				// Artist
				XMLNode album = itemAttributes.getChildNode("Title"); // album
				XMLNode artist = itemAttributes.getChildNode("Artist");
				XMLNode creator = itemAttributes.getChildNode("Creator");

				if (!album.isEmpty() && (!artist.isEmpty() || !creator.isEmpty()))
				{
					const char* albumName = album.getText();

					const char* artistName = NULL;
					const char* creatorName = NULL;
					
					if (!artist.isEmpty())
						artistName = artist.getText();
					if (!creator.isEmpty())
						creatorName = creator.getText();

					if ((albumName != NULL) && ((artistName != NULL) || (creatorName != NULL)))
					{
						std::string albumNameLow = NormalizeString(albumName);

						std::string artistNameLow = "";
						std::string creatorNameLow = "";

						if (artistName != NULL)
							artistNameLow = NormalizeString(artistName);
						if (creatorName != NULL)
							creatorNameLow = NormalizeString(creatorName);

						if (((albumLow.find(albumNameLow) != std::string::npos) || (albumNameLow.find(albumLow) != std::string::npos)) &&
							(
								((artistLow.find(artistNameLow) != std::string::npos) || (artistNameLow.find(artistLow) != std::string::npos)) ||
								((artistLow.find(creatorNameLow) != std::string::npos) || (creatorNameLow.find(artistLow) != std::string::npos))
								)
							)
						{
							// Image
							XMLNode image = item.getChildNode("LargeImage");

							// fallback to the medium image
							if (image.isEmpty())
								image = item.getChildNode("MediumImage");

							if (image.isEmpty())
							{
								// ImageSets
								XMLNode imageSets = item.getChildNode("ImageSets");

								if (!imageSets.isEmpty())
								{
									// ImageSet
									XMLNode imageSet;
									int j = 0;
									do
									{
										imageSet = imageSets.getChildNode("ImageSet", j);

										if (!imageSet.isEmpty())
										{
											// Image
											image = imageSet.getChildNode("LargeImage");

											// fallback to the medium image
											if (image.isEmpty())
												image = imageSet.getChildNode("MediumImage");
										}
										j++;
									}
									while ((!imageSet.isEmpty()) && (image.isEmpty()));
								}
							}

							if (!image.isEmpty())
							{
								// URL
								XMLNode url = image.getChildNode("URL");

								if (!url.isEmpty())
								{
									const char* urlName = url.getText();
									if ((urlName != NULL) && (SallyAPI::String::StringHelper::StringEndsWith(urlName, ".jpg")))
									{
										completeURL = urlName;
									}
								}
							}
						}
					}
				}	
			}
		}
		++i;
	}
	while ((!item.isEmpty()) && (completeURL.length() == 0));


	// cleanup
	DeleteFile(tempFile.c_str());

	if (completeURL.length() == 0)
		return false;

	return DownloadImage(completeURL, outputFile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CAmazonInfo::GetXML(std::string& artist, std::string& album, std::string* response)
///
/// \brief	Gets an xml. 
///
/// \author	Christian Knobloch
/// \date	20.05.2010
///
/// \param [in,out]	artist		The artist. 
/// \param [in,out]	album		The album. 
/// \param [in,out]	response	If non-null, the response. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CAmazonInfo::GetXML(std::string& artist, std::string& album, std::string* response)
{
	std::string queryString;
	queryString.append(artist);
	queryString.append(" ");
	queryString.append(album);

	std::string request = COMMUNITY_URL;
	request.append("p_rest/");
	request.append("amazonRequest.php?querySearch=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(queryString));

	int byteRead = 0;

	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NetworkHelper::GetHTTPText(COMMUNITY_SERVER, COMMUNITY_PORT, request, &byteRead, response, proxy, proxyBypass);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CAmazonInfo::DownloadImage(std::string& imageURL, std::string& outputFile)
///
/// \brief	Downloads an image. 
///
/// \author	Christian Knobloch
/// \date	20.05.2010
///
/// \param [in,out]	imageURL	URL of the image. 
/// \param [in,out]	outputFile	The output file. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CAmazonInfo::DownloadImage(std::string& imageURL, std::string& outputFile)
{
	std::string url = SallyAPI::String::StringHelper::GetStringBetween(imageURL, "http://", "/");
	std::string request = imageURL.substr(url.length() + 7);

	char* image = NULL;
	int byteRead = 0;

	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NETWORK_RETURN networkReturn = SallyAPI::Network::NetworkHelper::GetHTTPContent(url, 80, request, &byteRead, &image, proxy, proxyBypass);

	if (networkReturn != SallyAPI::Network::SUCCESS)
		return false;

	HANDLE hFile = CreateFile(outputFile.c_str(), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);
	if ((hFile == INVALID_HANDLE_VALUE) && (hFile == NULL))
	{
		free(image);
		return false;
	}

	DWORD		DWord;

	WriteFile(hFile, image ,(DWORD) byteRead, &DWord, 0);
	CloseHandle(hFile);
	free(image);
	return true;
}