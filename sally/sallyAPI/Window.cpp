////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Window.cpp
///
/// \brief	Implements the window class. 
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

#include "Window.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWindow::CWindow() :SallyAPI::GUI::CGUIBaseObject(NULL)
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CWindow::CWindow()
	:SallyAPI::GUI::CGUIBaseObject(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWindow::~CWindow()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CWindow::~CWindow()
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter != m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		SafeDelete(control);
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::SaveConfig()
///
/// \brief	Is called before Sally is closed.
///			Here you save your config.
///
/// \author	Christian Knobloch
/// \date	25.05.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::SaveConfig()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId,
/// int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
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

void CWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	CWindowMessage* message = new CWindowMessage(messageId, reporterId, reporter);

	m_iMessages.push_back(message);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId,
/// int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	// Send Timer Event to all Childs
	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWindowMessage CWindow::GetMessage()
///
/// \brief	Gets the message. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CWindowMessage CWindow::GetMessage()
{
	CWindowMessage* message;
	CWindowMessage messageReturn;
	
	std::list<CWindowMessage*>::iterator iter = m_iMessages.begin();
	if (iter != m_iMessages.end())
	{
		message = *iter;
		messageReturn.SetMessageId(message->GetMessageId());
		messageReturn.SetReporterId(message->GetReporterId());
		messageReturn.SetReporter(message->GetReporter());

		m_iMessages.pop_front();
		SafeDelete(message);

		return messageReturn;
	}
	return messageReturn;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::AddChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::AddChild(SallyAPI::GUI::CControl* control)
{
	m_GUIControlList.push_back(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::RemoveChild(std::list<SallyAPI::GUI::CControl*>::iterator iter)
///
/// \brief	Removes the child described by iter. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	iter	If non-null, the iterator. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::RemoveChild(std::list<SallyAPI::GUI::CControl*>::iterator iter)
{
	m_GUIControlList.erase(iter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::RemoveChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Removes the child described by control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::RemoveChild(SallyAPI::GUI::CControl* control)
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* tempControl = *iter;
		if (tempControl == control)
		{
			SafeDelete(control);
			m_GUIControlList.erase(iter);
			return;
		}
		++iter;
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::Timer(float timeDelta)
{
	SallyAPI::GUI::CGUIBaseObject::Timer(timeDelta);

	// Send Timer Event to all Childs
	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->Timer(timeDelta);
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::Render()
///
/// \brief	Renders this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::Render()
{
	RenderControl();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::RenderControl()
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->Render();
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::Update()
///
/// \brief	Updates this object. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::Update()
{
	UpdateControl();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::UpdateControl()
///
/// \brief	Updates a control. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::UpdateControl()
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->Update();
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::ProcessKeyboard(char KeyboardState[256], char KeyboardStatePressed[256])
///
/// \brief	Process a keyboard. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	KeyboardState			State of the keyboard. 
/// \param	KeyboardStatePressed	The keyboard state pressed. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::ProcessKeyboard(char KeyboardState[256], char KeyboardStatePressed[256])
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter;
	iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->ProcessKeyboard(KeyboardState, KeyboardStatePressed);
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindow::GetAbsolutPosition(int* x, int* y)
///
/// \brief	Gets an absolut position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	x	If non-null, the x coordinate. 
/// \param [in,out]	y	If non-null, the y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::GetAbsolutPosition(int* x, int* y)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CWindow::CharInputPressed(char c)
///
/// \brief	Char input pressed. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	c	The character. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::CharInputPressed(char c)
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CWindow::KeyDown(int c)
///
/// \brief	Key down. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	c	The. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::KeyDown(int c)
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CWindow::ProcessMouseDoubleClick(int iX, int iY)
///
/// \brief	Process a mouse double click. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iX	The x coordinate. 
/// \param	iY	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::ProcessMouseDoubleClick(int iX, int iY)
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter;
	iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->CheckProcessMouseDoubleClick(iX, iY);
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CWindow::ProcessMouseUp(int iX, int iY)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iX	The x coordinate. 
/// \param	iY	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::ProcessMouseUp(int iX, int iY)
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter;
	iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->CheckProcessMouseUp(iX, iY);
		control->SendMessageToChilds(this, 0, GUI_MESSAGE_MOUSE_UP_INFO);
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CWindow::ProcessMouseDown(int iX, int iY)
///
/// \brief	Process a mouse down. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iX	The x coordinate. 
/// \param	iY	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::ProcessMouseDown(int iX, int iY)
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter;
	iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->CheckProcessMouseDown(iX, iY);
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CWindow::ProcessMouseMove(int iX, int iY)
///
/// \brief	Process a mouse move. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	iX	The x coordinate. 
/// \param	iY	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::ProcessMouseMove(int iX, int iY)
{
	std::list<SallyAPI::GUI::CControl*>::iterator iter;
	iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		SallyAPI::GUI::CControl* control = *iter;
		control->CheckProcessMouseMove(iX, iY);
		++iter;
	}
	return false;
}
