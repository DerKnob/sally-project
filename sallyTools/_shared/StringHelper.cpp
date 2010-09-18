////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\StringHelper.cpp
///
/// \brief	Implements the string helper class. 
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

#include "StringHelper.h"

using namespace SallyAPI::String;

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

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

std::string StringHelper::StringToUpper(const std::string& strToConvert)
{
	std::string result = strToConvert;
	for(unsigned int i=0; i<strToConvert.length(); ++i)
	{
		result[i] = toupper(strToConvert[i]);
	}
	return result;
}
 
std::string StringHelper::StringToLower(const std::string& strToConvert)
{
	std::string result = strToConvert;
	for(unsigned int i=0; i<strToConvert.length(); ++i)
	{
		result[i] = tolower(strToConvert[i]);
	}
	return result;
}

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

std::string StringHelper::ConvertToString(int i)
{
	char c[11];
	_itoa_s(i, c, 11, 10);

	return c;
}

int StringHelper::ConvertToInt(const std::string &sString)
{
	int i = atoi(sString.c_str());
	return i;
}

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

std::string StringHelper::ConvertDoubleToString(double i)
{
	std::ostringstream o;
	if (!(o << i))
		return "";
	return o.str();
}

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

std::string StringHelper::ConvertFloatToString(float i)
{
	std::ostringstream o;
	if (!(o << i))
		return "";
	return o.str();
} 


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