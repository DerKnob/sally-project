////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\StringHelper.cpp
///
/// \brief	Implements the string helper class. 
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

#include "StringHelper.h"

using namespace SallyAPI::String;

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::RemoveStringBetween(const std::string& searchInString,
/// const std::string& start, const std::string& end)
///
/// \brief	Removes a string between. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	searchInString	The search in string. 
/// \param	start			The start. 
/// \param	end				The end. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::RemoveStringBetween(const std::string& searchInString, const std::string& start,
					const std::string& end)
{
	std::string result = searchInString;

	int pos1 = result.find(start);
	if (pos1 != std::string::npos)
	{
		int pos2 = result.find_first_of(end, pos1);
		if (pos1 != std::string::npos)
		{
			result.replace(pos1, pos2, "");
		}
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::GetStringBetween(const std::string& searchInString,
/// const std::string& first, const std::string& last)
///
/// \brief	Gets a string between. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	searchInString	The search in string. 
/// \param	first			The first. 
/// \param	last			The last. 
///
/// \return	The string between. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::GetStringBetween(const std::string& searchInString, const std::string& first,
										   const std::string& last)
{
	size_t posStart;
	size_t posEnd;

	posStart = searchInString.find(first);
	if(posStart == std::string::npos) 
	{ 
		return "";;
	}
	posEnd = searchInString.find(last, posStart + first.length());	
	if(posEnd == std::string::npos) 
	{ 
		return "";;
	}
	return searchInString.substr(posStart + first.length(), posEnd - (first.length() + posStart));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool StringHelper::StringEndsWith(const std::string& str, const std::string& end)
///
/// \brief	String ends with. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	str	The string. 
/// \param	end	The end. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool StringHelper::StringEndsWith(const std::string& str, const std::string& end)
{
	std::string strTemp = StringToUpper(str);
	std::string endTemp = StringToUpper(end);

	std::string::size_type pos = strTemp.rfind(endTemp);
	if ((pos != std::string::npos) &&
		((strTemp.length() - (pos + endTemp.length())) == 0))
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool StringHelper::StringStartsWith(const std::string& str, const std::string& start)
///
/// \brief	String starts with. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	str		The string. 
/// \param	start	The start. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool StringHelper::StringStartsWith(const std::string& str, const std::string& start)
{
	std::string strTemp = StringToUpper(str);
	std::string endTemp = StringToUpper(start);

	std::string::size_type pos = strTemp.rfind(endTemp);
	if (pos == 0)
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::StringToUpper(const std::string& strToConvert)
///
/// \brief	String to upper. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	strToConvert	The string to convert. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::StringToUpper(const std::string& strToConvert)
{
	std::string result = strToConvert;
	for(unsigned int i=0; i<strToConvert.length(); ++i)
	{
		result[i] = toupper(strToConvert[i]);
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::StringToLower(const std::string& strToConvert)
///
/// \brief	String to lower. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	strToConvert	The string to convert. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::StringToLower(const std::string& strToConvert)
{
	std::string result = strToConvert;
	for(unsigned int i=0; i<strToConvert.length(); ++i)
	{
		result[i] = tolower(strToConvert[i]);
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string> StringHelper::TokenizeString(const std::string& sString,
/// const std::string& token)
///
/// \brief	Tokenize string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	sString	The string. 
/// \param	token	The token. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> StringHelper::TokenizeString(const std::string& sString, const std::string& token)
{
	std::vector<std::string> vResultVector;
	std::string::size_type pos = 0;
	std::string::size_type newPos = 0;
	std::string	temp;

	while ((newPos = sString.find(token, pos)) != std::string::npos)
	{
		temp = sString.substr(pos, newPos - pos);
		vResultVector.push_back(temp);

		pos = newPos + token.length();
	}
	temp = sString.substr(pos, sString.length() - pos);
	vResultVector.push_back(temp);

	return vResultVector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::ConvertToString(int i)
///
/// \brief	Convert to string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	i	The index. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::ConvertToString(int i)
{
	char c[33];
	_itoa_s(i, c, 11, 10);

	return c;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::ConvertToString(double i)
///
/// \brief	Convert to string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	i	The. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::ConvertToString(double i)
{
	char c[33];
	_snprintf_s(c, 33, "%f", i);

	return c;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::ConvertToString(float i)
///
/// \brief	Convert to string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	i	The. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::ConvertToString(float i)
{
	char c[33];
	sprintf_s(c, 32, "%f", i);

	return c;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::ConvertToString(DWORD i)
///
/// \brief	Convert to string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	i	The. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::ConvertToString(DWORD i)
{
	char c[33];
	_itoa_s(i, c, 11, 10);

	return c;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int StringHelper::ConvertToInt(const std::string &sString)
///
/// \brief	Convert to int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	sString	The string. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int StringHelper::ConvertToInt(const std::string &sString)
{
	int i = atoi(sString.c_str());
	return i;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	double StringHelper::ConvertToDouble(const std::string& sString)
///
/// \brief	Convert to double. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	sString	The string. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

double StringHelper::ConvertToDouble(const std::string& sString)
{
	std::string inputStream = sString;
	if(inputStream[inputStream.length() - 1] == '.'){
		inputStream = inputStream.append("0");
	}
	std::istringstream input(inputStream);
	double x;
	if (!(input >> x))
		return 0.0;
	return x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::ConvertDoubleToString(double i)
///
/// \brief	Convert double to string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	i	The. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::ConvertDoubleToString(double i)
{
	std::ostringstream o;
	if (!(o << i))
		return "";
	return o.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float StringHelper::ConvertToFloat(const std::string& sString)
///
/// \brief	Convert to float. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	sString	The string. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

float StringHelper::ConvertToFloat(const std::string& sString)
{
	std::string inputStream = sString;
	if(inputStream[inputStream.length() - 1] == '.'){
		inputStream = inputStream.append("0");
	}
	std::istringstream input(inputStream);
	float x;
	if (!(input >> x))
		return 0.0;
	return x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::ConvertFloatToString(float i)
///
/// \brief	Convert float to string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	i	The. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::ConvertFloatToString(float i)
{
	std::ostringstream o;
	if (!(o << i))
		return "";
	return o.str();
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::ReplaceString(const std::string& in,
/// const std::string& replaceThis, const std::string& replaceWith)
///
/// \brief	Replace string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	in			The in. 
/// \param	replaceThis	The replace this. 
/// \param	replaceWith	The replace with. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::ReplaceString(const std::string& in, const std::string& replaceThis, const std::string& replaceWith)
{
	std::string::size_type pos = 0;
	std::string result = in;

	do
	{
		pos = result.find(replaceThis, 0);
		if (pos != std::string::npos)
		{
			result.replace(pos, replaceThis.length(), replaceWith);
			pos += replaceThis.length();
		}
	}
	while (pos != std::string::npos);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::base64_encode(unsigned char const* bytes_to_encode,
/// unsigned int in_len)
///
/// \brief	Base 64 encode. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	bytes_to_encode	If non-null, the bytes to encode. 
/// \param	in_len					Length of the in. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';

	}

	return ret;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string StringHelper::base64_decode(std::string const& encoded_string)
///
/// \brief	Base 64 decode. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	encoded_string	The encoded string. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string StringHelper::base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool StringHelper::StringCompareCaseInsensitivity(const std::string& left,
/// const std::string& right)
///
/// \brief	String compare case insensitivity. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	left	The left. 
/// \param	right	The right. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool StringHelper::StringCompareCaseInsensitivity(const std::string& left, const std::string& right)
{
	for(std::string::const_iterator lit = left.begin(), rit = right.begin(); lit != left.end() && rit != right.end(); ++lit, ++rit )
	{
		if(tolower(*lit) < tolower(*rit) )
			return true;
		else if(tolower(*lit) > tolower(*rit) )
			return false;
	}
	if(left.size() < right.size())
		return true;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool StringHelper::StringContainsCaseInsensitivity(const std::string& left,
/// const std::string& right)
///
/// \brief	String contains case insensitivity. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \param	left	The left. 
/// \param	right	The right. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool StringHelper::StringContainsCaseInsensitivity(const std::string& left, const std::string& right)
{
	std::string leftLow = StringToLower(left);
	std::string rightLow = StringToLower(right);

	int found = leftLow.find(rightLow);

	if (found == std::string::npos)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool StringHelper::StripServerAddress(const std::string& completeURL, std::string& server,
/// int& port, std::string& uri)
///
/// \brief	Strip server address. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	completeURL		URL of the complete. 
/// \param [in,out]	server	The server. 
/// \param [in,out]	port	The port. 
/// \param [in,out]	uri		URI of the document. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool StringHelper::StripServerAddress(const std::string& completeURL, std::string& server, int& port,
									  std::string& uri)
{
	server = "";
	port = 0;
	uri = "";

	// split http:// from the rest
	std::vector<std::string> parts = TokenizeString(completeURL, "//");	

	if (parts.size() != 2)
		return false;

	std::string part1 = parts.at(0);
	std::string part2 = parts.at(1);

	std::string::size_type pos = 0;
	std::string	serverAndPort;

	if ((pos = part2.find("/", 0)) != std::string::npos)
	{
		serverAndPort = part2.substr(0, pos);
		uri = part2.substr(pos, part2.length() - 1);
	}
	else
	{
		serverAndPort = part2;
	}

	if ((pos = serverAndPort.find(":", 0)) != std::string::npos)
	{
		server = serverAndPort.substr(0, pos);
		port = SallyAPI::String::StringHelper::ConvertToInt(serverAndPort.substr(pos + 1, serverAndPort.length() - 1));
		return true;
	}

	server = serverAndPort;

	// no port found, try ot resolve the port with the protocol name
	if (part1.compare("http:") == 0)
	{
		port = 80;
		return true;
	}
	if (part1.compare("https:") == 0)
	{
		port = 443;
		return true;
	}
	if (part1.compare("ftp:") == 0)
	{
		port = 21;
		return true;
	}
	if (part1.compare("smtp:") == 0)
	{
		port = 25;
		return true;
	}
	if (part1.compare("pop3:") == 0)
	{
		port = 110;
		return true;
	}
	return false;
}