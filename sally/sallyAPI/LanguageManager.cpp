////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\LanguageManager.cpp
///
/// \brief	Implements the language manager class. 
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

#include "languagemanager.h"

using namespace SallyAPI::Config;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLanguageManager::CLanguageManager(const std::string& lang,
/// const std::string& fileExtention)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	lang			The lang. 
/// \param	fileExtention	The file extention. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CLanguageManager::CLanguageManager(const std::string& lang, const std::string& fileExtention)
	:m_strLang(lang), m_strFileExtention(fileExtention)
{
	m_strBaseDir = SallyAPI::System::SystemHelper::GetModulePath();
	m_strBaseDir.append("languages\\");

	AddLangFileDir("sally-project");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLanguageManager::~CLanguageManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CLanguageManager::~CLanguageManager()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLanguageManager::AddLangFileDir(const std::string& langFileDir)
///
/// \brief	Adds a lang file directory to the language directory list.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	langFileDir	The lang file dir. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLanguageManager::AddLangFileDir(const std::string& langFileDir)
{
	// add only if not already in list
	if (std::find(m_vLangFileDirs.begin(), m_vLangFileDirs.end(), langFileDir) != m_vLangFileDirs.end())
		return;

	m_vLangFileDirs.push_back(langFileDir);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string> CLanguageManager::GetLangFileDirs()
///
/// \brief	Gets the lang file dirs. 
///
/// \author	Christian Knobloch
/// \date	24.06.2010
///
/// \return	The lang file dirs. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> CLanguageManager::GetLangFileDirs()
{
	return m_vLangFileDirs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CLanguageManager::GetString(const std::string& id)
///
/// \brief	Gets the localized string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
///
/// \return	The localized string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CLanguageManager::GetString(const std::string& id)
{
	if (id.length() == 0) {
		return id;
	}

	std::string searchString = SallyAPI::String::StringHelper::ReplaceString(id, "\n", "\\n");
	searchString = SallyAPI::String::StringHelper::ReplaceString(searchString, "\t", "\\t");

	// Search in the Cache
	if(m_mCache.find(searchString) != m_mCache.end())
	{
		return m_mCache[searchString];
	}
	std::string text;
	char		temp[MAX_LANGSTRING];

	// Iterator through all lang files.
	// The first one is the main lang files. Than the lang files of the plugins in the currently selected language
	std::vector<std::string>::iterator itr = m_vLangFileDirs.begin();
	while (itr != m_vLangFileDirs.end())
	{
		std::string dirINI = m_strBaseDir;

		dirINI.append(m_strLang);
		dirINI.append("\\");
		dirINI.append(*itr);
		dirINI.append(".");
		dirINI.append(m_strFileExtention);

		GetPrivateProfileString("lang", searchString.c_str(), "", temp, sizeof(temp), dirINI.c_str());

		// If we found it than break the loop and return the text
		if (strlen(temp) > 0)
		{
			text = SallyAPI::String::StringHelper::ReplaceString(temp, "\\n", "\n");
			text = SallyAPI::String::StringHelper::ReplaceString(text, "\\t", "\t");
			break;
		}
		++itr;
	}

	if (text.length() == 0)
	{
		// put the not found string to the logfile
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

		std::string debug = "language localisation not found: ";
		debug.append(id);
		logger->Debug(debug);

		// set the english test
		text = id;
	}
	// Put into Cache
	m_mCache[searchString] = text;
	return text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CLanguageManager::GetString(const std::string& id, const char* first,...)
///
/// \brief	Gets a string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id		The identifier. 
/// \param	first	The first. 
///
/// \return	The string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CLanguageManager::GetString(const std::string& id, const char* first,...) 
{
	std::string outputString = GetString(id);

	char* str;
	va_list vl;
	int pos = 0;

	str=(char*) first;

	va_start(vl,first);

	while ((str != NULL) && (std::string::npos != pos))
	{
		pos = outputString.find("%s", pos);

		// nothing found - we will run into the will and end
		if (std::string::npos == pos)
			continue;

		// don't replace something like this %%s => this means %s to the output
		if (pos != 0) 
		{
			if (outputString.at(pos - 1) == '%')
			{
				++pos;
				continue;
			}
		}

		// Have we found at last one?
		if (std::string::npos != pos)
		{
			outputString.replace(pos, 2, str); 

			// add the length of the replaced text to the pos
			pos += strlen(str);

		}
		str=va_arg(vl,char *);
	}


	pos = outputString.find("%%", pos);

	// now replace all %% with %
	pos = 0;
	do
	{
		pos = outputString.find("%%", pos);

		if (std::string::npos != pos) {
			outputString.replace(pos, 2, "%"); 

			++pos;
		}
	} while (std::string::npos != pos);

	va_end(vl);
	return outputString;
}