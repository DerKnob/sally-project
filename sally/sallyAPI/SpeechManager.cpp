////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SpeechManager.cpp
///
/// \brief	Implements the speech manager class. 
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

#include "SpeechManager.h"

SallyAPI::Speech::CSpeechOutputManager*	SallyAPI::Speech::CSpeechOutputManager::m_pObject = NULL;
SallyAPI::Speech::CSpeech*				SallyAPI::Speech::CSpeechOutputManager::m_pSpeech = NULL;
SallyAPI::GUI::CWindow*					SallyAPI::Speech::CSpeechOutputManager::m_pMainWindow = NULL;
bool									SallyAPI::Speech::CSpeechOutputManager::m_bActive = false;

using namespace SallyAPI::Speech;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSpeechOutputManager::CSpeechOutputManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSpeechOutputManager::CSpeechOutputManager()
{
	m_pSpeech = new SallyAPI::Speech::CSpeech();
	m_pSpeechThread.SetValue(&m_pSpeech, m_pMainWindow);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSpeechOutputManager::~CSpeechOutputManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSpeechOutputManager::~CSpeechOutputManager()
{
	m_bActive = false;
	m_pSpeechThread.Stop();
	SafeDelete(m_pSpeech);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Speech::CSpeechOutputManager* CSpeechOutputManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Speech::CSpeechOutputManager* CSpeechOutputManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new CSpeechOutputManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechOutputManager::Activate()
///
/// \brief	Activates this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechOutputManager::Activate()
{
	m_bActive = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechOutputManager::Deactivate()
///
/// \brief	Deactivates this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechOutputManager::Deactivate()
{
	m_bActive = false;

	CSpeechOutputManager* speechOutputManager = GetInstance();
	speechOutputManager->Stop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechOutputManager::Stop()
///
/// \brief	Stops this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechOutputManager::Stop()
{
	m_pSpeechThread.WaitForStop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechOutputManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechOutputManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechOutputManager::SetValues(SallyAPI::GUI::CWindow* mainWindow)
///
/// \brief	Sets the values. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	mainWindow	If non-null, the main window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechOutputManager::SetValues(SallyAPI::GUI::CWindow* mainWindow)
{
	m_pMainWindow = mainWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechOutputManager::Speak(const std::string& say, bool isImportand)
///
/// \brief	Speaks. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	say			The say. 
/// \param	isImportand	true if is importand. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechOutputManager::Speak(const std::string& say, bool isImportand)
{
	Speak(say, isImportand, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechOutputManager::Speak(const std::string& say, bool isImportand,
/// const char* first,...)
///
/// \brief	Speaks. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	say			The say. 
/// \param	isImportand	true if is importand. 
/// \param	first		The first. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechOutputManager::Speak(const std::string& say, bool isImportand, const char* first,...)
{
	if (!m_bActive)
		return;

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* lang = config->GetSpeechOutputLocalization();

	std::string outputString = lang->GetString(say);

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

	m_pSpeechThread.Speak(outputString, isImportand);
	m_pSpeechThread.Start();
}
