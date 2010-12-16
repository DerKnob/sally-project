////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\AppBase.cpp
///
/// \brief	Implements the application base class. 
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

#include "AppBase.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CAppBase::CAppBase(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
/// const std::string& explicidAppName) :SallyAPI::GUI::CForm(parent, 0, 0, WINDOW_WIDTH,
/// WINDOW_HEIGHT), m_iGraphicId(graphicId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	explicidAppName	Name of the explicid application. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CAppBase::CAppBase(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& explicidAppName)
	:SallyAPI::GUI::CForm(parent, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), m_iGraphicId(graphicId)
{
	std::string explicidAppNameTemp = explicidAppName;

	if (explicidAppNameTemp.length() > 0) {
		explicidAppNameTemp = SallyAPI::String::PathHelper::CorrectPath(explicidAppNameTemp);
		explicidAppNameTemp = explicidAppNameTemp.substr(0, explicidAppNameTemp.length() - 1);
		explicidAppNameTemp = SallyAPI::String::PathHelper::GetFileFromPath(explicidAppNameTemp);
	}

	m_strExplicitAppName.append(explicidAppNameTemp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CAppBase::~CAppBase()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CAppBase::~CAppBase()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CAppBase::GetPictureID(int iPicture)
///
/// \brief	Gets a picture identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iPicture	The picture. 
///
/// \return	The picture identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CAppBase::GetPictureID(int iPicture)
{
	return iPicture + m_iGraphicId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CAppBase::GetGraphicId()
///
/// \brief	Gets the graphic identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The graphic identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CAppBase::GetGraphicId()
{
	return m_iGraphicId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CAppBase::GetPropertyString(const std::string& ID,
/// const std::string& defaultReturn)
///
/// \brief	Gets a property string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	ID				The identifier. 
/// \param	defaultReturn	Default return. 
///
/// \return	The property string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CAppBase::GetPropertyString(const std::string& ID, const std::string& defaultReturn)
{
	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(m_strExplicitAppName);

	return option->GetPropertyString("config", ID, defaultReturn);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CAppBase::GetPropertyInt(const std::string& ID, int defaultReturn)
///
/// \brief	Gets a property int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	ID				The identifier. 
/// \param	defaultReturn	Default return. 
///
/// \return	The property int. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CAppBase::GetPropertyInt(const std::string& ID, int defaultReturn)
{
	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(m_strExplicitAppName);

	return option->GetPropertyInt("config", ID, defaultReturn);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CAppBase::GetPropertyBool(const std::string& ID, bool defaultReturn)
///
/// \brief	Gets a property bool. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	ID				The identifier. 
/// \param	defaultReturn	Default return. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CAppBase::GetPropertyBool(const std::string& ID, bool defaultReturn)
{
	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(m_strExplicitAppName);

	return option->GetPropertyBool("config", ID, defaultReturn);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SallyAPI::GUI::CAppBase::SetPropertyString(const std::string& ID,
/// const std::string& s)
///
/// \brief	Sets a property string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	ID	The identifier. 
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void SallyAPI::GUI::CAppBase::SetPropertyString(const std::string& ID, const std::string& s)
{
	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(m_strExplicitAppName);

	option->SetPropertyString("config", ID, s);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SallyAPI::GUI::CAppBase::SetPropertyInt(const std::string& ID, int i)
///
/// \brief	Sets a property int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	ID	The identifier. 
/// \param	i	The index. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void SallyAPI::GUI::CAppBase::SetPropertyInt(const std::string& ID, int i)
{
	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(m_strExplicitAppName);

	option->SetPropertyInt("config", ID, i);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SallyAPI::GUI::CAppBase::SetPropertyBool(const std::string& ID, bool i)
///
/// \brief	Sets a property bool. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	ID	The identifier. 
/// \param	i	true to. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void SallyAPI::GUI::CAppBase::SetPropertyBool(const std::string& ID, bool i)
{
	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(m_strExplicitAppName);

	option->SetPropertyBool("config", ID, i);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CAppBase::GetExplicitAppName()
///
/// \brief	Gets the explicit application name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The explicit application name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CAppBase::GetExplicitAppName()
{
	return m_strExplicitAppName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CAppBase::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CAppBase::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_DIALOG_CANCEL:
	case MS_DIALOG_NO:
	case MS_DIALOG_OK:
	case MS_DIALOG_YES:
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}
