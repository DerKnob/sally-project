////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Edit.cpp
///
/// \brief	Implements the edit class. 
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

#include "Edit.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CEdit::CEdit(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
/// m_iImage(NULL), m_iImageSize(16), m_bImageLeft(true), m_bNumberOnly(false),
/// m_bPassword(false), m_bKeyboardInputEnabled(true), m_iMaxCharacterInputLength(-1),
/// m_eCharacterInputType(EDIT_CHARACTER_INPUT_TYPE_NONE), m_bEnableKeyboardBrainWordList(true),
/// m_bEnableKeyboardBrainHistory(true), m_bEnableKeyboardWordRequest(false)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CEdit::CEdit(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
	m_iImage(NULL), m_iImageSize(16), m_bImageLeft(true),	m_bNumberOnly(false), m_bPassword(false),
	m_bKeyboardInputEnabled(true), m_iMaxCharacterInputLength(-1),
	m_eCharacterInputType(EDIT_CHARACTER_INPUT_TYPE_NONE), m_bEnableKeyboardBrainWordList(true),
	m_bEnableKeyboardBrainHistory(true), m_bEnableKeyboardWordRequest(false)
{
	SetLocalised(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CEdit::~CEdit()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CEdit::~CEdit()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetText(const std::string& text)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetText(const std::string& text)
{
	if (m_eCharacterInputType == EDIT_CHARACTER_INPUT_TYPE_NONE)
	{
		CGUIBaseObject::SetText(text);
	}
	else if (m_eCharacterInputType == EDIT_CHARACTER_INPUT_TYPE_UPPER)
	{
		CGUIBaseObject::SetText(SallyAPI::String::StringHelper::StringToUpper(text));
	}
	else if (m_eCharacterInputType == EDIT_CHARACTER_INPUT_TYPE_LOWER)
	{
		CGUIBaseObject::SetText(SallyAPI::String::StringHelper::StringToLower(text));
	}
	

	m_pParent->SendMessageToParent(this, m_iControlId, GUI_EDIT_CHANGED);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetInfoText(const std::string& text)
///
/// \brief	Sets an information text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetInfoText(const std::string& text)
{
	m_strInfoText = text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CEdit::GetInfoText()
///
/// \brief	Gets the information text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The information text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string	CEdit::GetInfoText()
{
	return m_strInfoText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetImageLeft()
///
/// \brief	Sets the image left. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetImageLeft()
{
	m_bImageLeft = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetImageRight()
///
/// \brief	Sets the image right. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetImageRight()
{
	m_bImageLeft = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetImageId(int image)
///
/// \brief	Sets an image identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	image	The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetImageId(int image)
{
	m_iImage = image;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetNumberOnly(bool value)
///
/// \brief	Sets a number only. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetNumberOnly(bool value)
{
	m_bNumberOnly = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CEdit::GetNumberOnly()
///
/// \brief	Gets the number only. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CEdit::GetNumberOnly()
{
	return m_bNumberOnly;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetPassword(bool value)
///
/// \brief	Sets a password. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetPassword(bool value)
{
	m_bPassword = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CEdit::GetPassword()
///
/// \brief	Gets the password. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CEdit::GetPassword()
{
	return m_bPassword;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetKeyboardInputEnabled(bool value)
///
/// \brief	Sets a keyboard input enabled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetKeyboardInputEnabled(bool value)
{
	m_bKeyboardInputEnabled = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CEdit::GetKeyboardInputEnabled()
///
/// \brief	Gets the keyboard input enabled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CEdit::GetKeyboardInputEnabled()
{
	return m_bKeyboardInputEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetMaxCharacterInputLength(int value)
///
/// \brief	Sets a maximum character input length. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetMaxCharacterInputLength(int value)
{
	m_iMaxCharacterInputLength = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CEdit::GetMaxCharacterInputLength()
///
/// \brief	Gets the maximum character input length. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The maximum character input length. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CEdit::GetMaxCharacterInputLength()
{
	return m_iMaxCharacterInputLength;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetCharacterInputType(EditCharacterInputType value)
///
/// \brief	Sets a character input type. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetCharacterInputType(EditCharacterInputType value)
{
	m_eCharacterInputType = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EditCharacterInputType CEdit::GetCharacterInputType()
///
/// \brief	Gets the character input type. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The character input type. 
////////////////////////////////////////////////////////////////////////////////////////////////////

EditCharacterInputType CEdit::GetCharacterInputType()
{
	return m_eCharacterInputType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::RenderControl()
{
	int borderLeft = 4;
	int borderRight = 4;

	DrawButtonBackground(GUI_THEME_EDIT_LEFT, GUI_THEME_EDIT, GUI_THEME_EDIT_RIGHT);
	if (m_iImage)
	{
		if (m_bImageLeft)
		{
			borderLeft += 4 + m_iImageSize;
			int imageWidthLeft = 0;

			if ((m_strText.length() > 0) || (m_strInfoText.length() > 0))
			{
				// Temp variables
				SallyAPI::GUI::CPicture* image = NULL;

				image = GetPicture(GUI_THEME_EDIT_LEFT);
				if (image != NULL) {
					imageWidthLeft = image->GetWidth();
				}
			}
			else
			{
				imageWidthLeft = (m_iWidth - m_iImageSize) / 2;
			}
			int oldAlphaBlending = m_iAlphaBlending;
			if (!m_bEnabled)
				m_iAlphaBlending = m_iAlphaBlending / 2;
			DrawImage(m_iImage, imageWidthLeft, (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
			m_iAlphaBlending = oldAlphaBlending;
		}
		else
		{
			borderRight += 4 + m_iImageSize;
			int imageWidthRight = 0;

			// Temp variables
			SallyAPI::GUI::CPicture* image = NULL;

			if ((m_strText.length() > 0) || (m_strInfoText.length() > 0))
			{
				// Temp variables
				SallyAPI::GUI::CPicture* image = NULL;

				image = GetPicture(GUI_THEME_EDIT_RIGHT);
				if (image != NULL) {
					imageWidthRight = image->GetWidth();
				}
			}
			int oldAlphaBlending = m_iAlphaBlending;
			if (!m_bEnabled)
				m_iAlphaBlending = m_iAlphaBlending / 2;
			DrawImage(m_iImage, m_iWidth - (m_iImageSize + imageWidthRight), (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
			m_iAlphaBlending = oldAlphaBlending;
		}
	}
	if (m_strText.length() == 0)
	{
		// Draw Info Text
		bool localisedOld = m_bLocalised;
		m_bLocalised = true;
		DrawText(GUI_THEME_EDIT_LEFT, GUI_THEME_EDIT_RIGHT, borderLeft, borderRight, "edit.info.font", m_strInfoText);
		m_bLocalised = localisedOld;
	}
	else if (m_bPassword)
	{
		std::string temp;

		for (int i = 0; i < (int) m_strText.length(); ++i)
		{
			temp.append("*");
		}

		DrawText(GUI_THEME_EDIT_LEFT, GUI_THEME_EDIT_RIGHT, borderLeft, borderRight, "edit.info.font", temp);
	}
	else
	{
		DrawText(GUI_THEME_EDIT_LEFT, GUI_THEME_EDIT_RIGHT, borderLeft, borderRight, "edit.font");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CEdit::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CEdit::ProcessMouseUp(int x, int y)
{
	bool pressedTemp = m_bPressed;

	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);

	if (!m_bKeyboardInputEnabled)
		return true;

	if (pressedTemp)
		m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_KEYBOARD);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetEnableKeyboardBrainHistory(bool value)
///
/// \brief	Sets an enable keyboard brain history. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetEnableKeyboardBrainHistory(bool value)
{
	m_bEnableKeyboardBrainHistory = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CEdit::GetEnableKeyboardBrainHistory()
///
/// \brief	Gets the enable keyboard brain history. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CEdit::GetEnableKeyboardBrainHistory()
{
	return m_bEnableKeyboardBrainHistory;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetEnableKeyboardBrainWordList(bool value)
///
/// \brief	Sets an enable keyboard brain word list. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetEnableKeyboardBrainWordList(bool value)
{
	m_bEnableKeyboardBrainWordList = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CEdit::GetEnableKeyboardBrainWordList()
///
/// \brief	Gets the enable keyboard brain word list. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CEdit::GetEnableKeyboardBrainWordList()
{
	return m_bEnableKeyboardBrainWordList;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CEdit::SetEnableKeyboardWordRequest(bool value)
///
/// \brief	Sets an enable enable keyboard word request. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CEdit::SetEnableKeyboardWordRequest(bool value)
{
	m_bEnableKeyboardWordRequest = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CEdit::GetEnableKeyboardWordRequest()
///
/// \brief	Gets the enable enable keyboard word request. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CEdit::GetEnableKeyboardWordRequest()
{
	return m_bEnableKeyboardWordRequest;
}