////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\OptionManager.cpp
///
/// \brief	Implements the option manager class. 
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

#include "OptionManager.h"

SallyAPI::Core::COptionManager*						SallyAPI::Core::COptionManager::m_pObject = NULL;
std::map<std::string, SallyAPI::System::COption*>		SallyAPI::Core::COptionManager::m_OptionMap;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::COptionManager::COptionManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::COptionManager::COptionManager()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::COptionManager::~COptionManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::COptionManager::~COptionManager()
{
	std::map<std::string, SallyAPI::System::COption*>::iterator iter = m_OptionMap.begin();

	while (iter != m_OptionMap.end())
	{
		SallyAPI::System::COption* temp = (*iter).second;
		delete(temp);

		m_OptionMap.erase(iter);
		iter = m_OptionMap.begin();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::COptionManager* SallyAPI::Core::COptionManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::COptionManager* SallyAPI::Core::COptionManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Core::COptionManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SallyAPI::Core::COptionManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void SallyAPI::Core::COptionManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::System::COption* SallyAPI::Core::COptionManager::GetOption(const std::string& explicidAppName)
///
/// \brief	Gets an option. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
///
/// \return	null if it fails, else the option. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::System::COption* SallyAPI::Core::COptionManager::GetOption(const std::string& explicidAppName)
{
	if (m_OptionMap[explicidAppName] != NULL)
	{
		return m_OptionMap[explicidAppName];
	}
	std::string iniFile = SallyAPI::System::SallyHelper::GetMediaDirectory(explicidAppName);
	iniFile.append("option.ini");
	
	SallyAPI::System::COption* temp = new SallyAPI::System::COption(iniFile);

	m_OptionMap[explicidAppName] = temp;
	return temp;
}