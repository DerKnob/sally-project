////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\PathHelper.cpp
///
/// \brief	Implements the path helper class. 
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

#include "PathHelper.h"

using namespace SallyAPI::String;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string PathHelper::CorrectPath(const std::string& sPath)
///
/// \brief	Correct path. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	sPath	Full pathname of the file. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string PathHelper::CorrectPath(const std::string& sPath)
{
	std::string result = sPath;

	if (result.at(result.length() - 1) != '\\')
		result.append("\\");

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string PathHelper::GetDirectoryFromPath(const std::string& sFile)
///
/// \brief	Gets a directory from a path string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	sFile	The file. 
///
/// \return	The directory from file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string PathHelper::GetDirectoryFromPath(const std::string& sFile)
{
	std::string path;
	std::string result;

	char* pdest;
	char modulepath[MAX_PATH];
	strcpy_s(modulepath, sFile.c_str());

	pdest = strrchr (modulepath,'\\');
	modulepath [pdest - modulepath+1] = '\0';

	path.append(modulepath);

	result = SallyAPI::String::PathHelper::CorrectPath(path);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string PathHelper::GetUpperDirectory(const std::string& sFile)
///
/// \brief	Gets a upper directory. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	sFile	The file. 
///
/// \return	The upper directory. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string PathHelper::GetUpperDirectory(const std::string& sFile)
{
	std::string path;
	char* pdest;
	char modulepath[MAX_PATH];
	strcpy_s(modulepath, sFile.c_str());

	if (modulepath[strlen(modulepath)-1] == '\\')
	{
		modulepath[strlen(modulepath)-1] = '\0';
	}

	pdest = strrchr(modulepath,'\\');
	modulepath [pdest - modulepath+1] = '\0';

	path.append(modulepath);

	path = SallyAPI::String::PathHelper::CorrectPath(path);
	return path;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string PathHelper::GetFileFromPath(const std::string& str,
/// const std::string sep)
///
/// \brief	Gets a file from path string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	str	The string. 
/// \param	sep	The sep. 
///
/// \return	The file from folder string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string PathHelper::GetFileFromPath(const std::string& str, const std::string sep)
{
	std::string::size_type index = str.find_last_of(sep);
	
	if (index == std::string::npos)
		return str;

	size_t length = str.length() - index;
	return str.substr(index + 1, length);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PathHelper::CreateRecursiveDirectory(const std::string& str)
///
/// \brief	Creates a recursive directory. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	str	The string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void PathHelper::CreateRecursiveDirectory(const std::string& str)
{
	
	if (str.size() > 3)
	{
		std::string tempDir = GetUpperDirectory(str);

		if (tempDir.size() > 3)
			CreateRecursiveDirectory(tempDir);
	}
	CreateDirectory(str.c_str(), NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string PathHelper::CorrectFileName(const std::string& str)
///
/// \brief	Correct file name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	str	The string. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string PathHelper::CorrectFileName(const std::string& str)
{
	std::string result = SallyAPI::String::StringHelper::ReplaceString(str, "/", "-");

	result = SallyAPI::String::StringHelper::ReplaceString(result, "\\", "-");
	
	result = SallyAPI::String::StringHelper::ReplaceString(result, ":", "-");
	
	result = SallyAPI::String::StringHelper::ReplaceString(result, "*", "-");
	
	result = SallyAPI::String::StringHelper::ReplaceString(result, "?", "-");
	
	result = SallyAPI::String::StringHelper::ReplaceString(result, "\"", "-");
	
	result = SallyAPI::String::StringHelper::ReplaceString(result, "<", "-");
	
	result = SallyAPI::String::StringHelper::ReplaceString(result, ">", "-");
	
	result = SallyAPI::String::StringHelper::ReplaceString(result, "|", "-");

	return result;
}