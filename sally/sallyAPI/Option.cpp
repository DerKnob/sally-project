////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Option.cpp
///
/// \brief	Implements the option class. 
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

#include "Option.h"

std::string SallyAPI::System::COption::m_strDefaultFile;

using namespace SallyAPI::System;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	COption::COption(const std::string& iniFile)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iniFile	The initialise file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

COption::COption(const std::string& iniFile)
{
	if (iniFile.length() > 0)
	{
		m_strFile.append(iniFile);
	}
	else
	{
		m_strFile = SallyAPI::System::SystemHelper::GetModulePath();
		m_strFile.append("option.ini");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	COption::~COption()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

COption::~COption()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void COption::SetDefaultFile(const std::string& defaultFile)
///
/// \brief	Sets a default file. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	defaultFile	Default file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void COption::SetDefaultFile(const std::string& defaultFile)
{
	m_strDefaultFile = defaultFile;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int COption::GetPropertyInt(const std::string& component, const std::string& id,
/// int defaultValue)
///
/// \brief	Gets a property int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component		The component. 
/// \param	id				The identifier. 
/// \param	defaultValue	Default value. 
///
/// \return	The property int. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int COption::GetPropertyInt(const std::string& component, const std::string& id,
											int defaultValue)
{
	int result;

	char defaultValueString[11];
	_itoa_s(defaultValue, defaultValueString, 11, 10);

	std::string text = GetPropertyString(component, id, defaultValueString);
	result = atoi(text.c_str());

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool COption::GetPropertyBool(const std::string& component, const std::string& id,
/// bool defaultValue)
///
/// \brief	Gets a property bool. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component		The component. 
/// \param	id				The identifier. 
/// \param	defaultValue	Default value. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool COption::GetPropertyBool(const std::string& component, const std::string& id,
											 bool defaultValue)
{
	int result;

	char defaultValueString[11];
	_itoa_s(defaultValue, defaultValueString, 11, 10);

	std::string text = GetPropertyString(component, id, defaultValueString);
	result = atoi(text.c_str());

	if (result)
		return true;
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string COption::GetPropertyString(const std::string& component,
/// const std::string& id, const std::string& defaultValue)
///
/// \brief	Gets a property string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component		The component. 
/// \param	id				The identifier. 
/// \param	defaultValue	Default value. 
///
/// \return	The property string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string COption::GetPropertyString(const std::string& component, const std::string& id,
									   const std::string& defaultValue)
{
	std::string propertyName;
	propertyName.append(component);
	propertyName.append(".");
	propertyName.append(id);
	// Search in the Cache
	if(m_PropertyCache.find(propertyName) != m_PropertyCache.end())
	{
		return m_PropertyCache[propertyName];
	}
	std::string text;	
	char		temp[MAX_PATH];

	GetPrivateProfileString(component.c_str(), id.c_str(), defaultValue.c_str(), temp, sizeof(temp),
		m_strFile.c_str());
	text.append(temp);

	// Put into Cache
	m_PropertyCache[propertyName] = text;
	return text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int COption::GetPropertyIntStatic(const std::string& component, const std::string& id,
/// int defaultValue, const std::string& iniFile)
///
/// \brief	Gets a property int static. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component		The component. 
/// \param	id				The identifier. 
/// \param	defaultValue	Default value. 
/// \param	iniFile			The initialise file. 
///
/// \return	The property int static. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int COption::GetPropertyIntStatic(const std::string& component, const std::string& id,
													int defaultValue, const std::string& iniFile)
{
	int result;

	char defaultValueString[11];
	_itoa_s(defaultValue, defaultValueString, 11, 10);


	std::string text = GetPropertyStringStatic(component, id, defaultValueString, iniFile);
	result = atoi(text.c_str());

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string COption::GetPropertyStringStatic(const std::string& component,
/// const std::string& id, const std::string& defaultValue, const std::string& iniFile)
///
/// \brief	Gets a property string static. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component		The component. 
/// \param	id				The identifier. 
/// \param	defaultValue	Default value. 
/// \param	iniFile			The initialise file. 
///
/// \return	The property string static. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string COption::GetPropertyStringStatic(const std::string& component, const std::string& id,
											 const std::string& defaultValue, const std::string& iniFile)
{
	std::string text;	
	std::string	dirINI;
	char		temp[MAX_PATH];

	if (iniFile.length() > 0)
	{
		dirINI = iniFile;
	}
	else if (m_strDefaultFile.length() > 0)
	{
		dirINI = m_strDefaultFile;
	}
	else
	{
		dirINI = SallyAPI::System::SystemHelper::GetModulePath();
		dirINI.append("option.ini");
	}

	GetPrivateProfileString(component.c_str(), id.c_str(), defaultValue.c_str(), temp, sizeof(temp), dirINI.c_str());
	text.append(temp);

	return text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool COption::GetPropertyBoolStatic(const std::string& component, const std::string& id,
/// bool defaultValue, const std::string& iniFile)
///
/// \brief	Gets a property bool static. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component		The component. 
/// \param	id				The identifier. 
/// \param	defaultValue	Default value. 
/// \param	iniFile			The initialise file. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool COption::GetPropertyBoolStatic(const std::string& component, const std::string& id,
															 bool defaultValue, const std::string& iniFile)
{
	int result;

	char defaultValueString[11];
	_itoa_s(defaultValue, defaultValueString, 11, 10);


	std::string text = GetPropertyStringStatic(component, id, defaultValueString, iniFile);
	result = atoi(text.c_str());

	if (result)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void COption::SetPropertyInt(const std::string& component, const std::string& id,
/// int value)
///
/// \brief	Sets a property int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component	The component. 
/// \param	id			The identifier. 
/// \param	value		The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void COption::SetPropertyInt(const std::string& component, const std::string& id, int value)
{
	char	temp[11];

	_itoa_s(value, temp, 11, 10);

	SetPropertyString(component, id, temp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void COption::SetPropertyBool(const std::string& component, const std::string& id,
/// bool value)
///
/// \brief	Sets a property bool. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component	The component. 
/// \param	id			The identifier. 
/// \param	value		true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void COption::SetPropertyBool(const std::string& component, const std::string& id, bool value)
{
	char	temp[11];

	_itoa_s(value, temp, 11, 10);

	SetPropertyString(component, id, temp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void COption::SetPropertyString(const std::string& component, const std::string& id,
/// const std::string& value)
///
/// \brief	Sets a property string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component	The component. 
/// \param	id			The identifier. 
/// \param	value		The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void COption::SetPropertyString(const std::string& component, const std::string& id,
												  const std::string& value)
{
	std::string propertyName;
	propertyName.append(component);
	propertyName.append(".");
	propertyName.append(id);
	// Put in the Cache
	m_PropertyCache[propertyName] = value;

	WritePrivateProfileString(component.c_str(), id.c_str(), value.c_str(), m_strFile.c_str());
}

void SallyAPI::System::COption::SetPropertyIntStatic(const std::string& component, const std::string& id,
													 int value, const std::string& iniFile)
{
	char	temp[11];

	_itoa_s(value, temp, 11, 10);

	WritePrivateProfileString(component.c_str(), id.c_str(), temp, iniFile.c_str());
}

void SallyAPI::System::COption::SetPropertyBoolStatic(const std::string& component, const std::string& id,
													  bool value, const std::string& iniFile)
{
	char	temp[11];

	_itoa_s(value, temp, 11, 10);

	WritePrivateProfileString(component.c_str(), id.c_str(), temp, iniFile.c_str());
}

void SallyAPI::System::COption::SetPropertyStringStatic(const std::string& component, const std::string& id,
														const std::string& value, const std::string& iniFile)
{
	WritePrivateProfileString(component.c_str(), id.c_str(), value.c_str(), iniFile.c_str());
}