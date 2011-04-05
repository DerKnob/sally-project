////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Button.cpp
///
/// \brief	Implements the button class. 
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

#include "Button.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CButton::CButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// 	int height, int controlId, ButtonType type) :SallyAPI::GUI::CControl(parent, x, y, width,
/// 	height, controlId), m_eType(type), m_iImage(0), m_bImageLeft(true),
/// 	m_iImageSize(ICON_SIZE), m_bChecked(false), m_bActive(false), m_fDeltaStart(0),
/// 	m_bDefaultButton(false), m_bUseHoleWidth(false)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
/// \param	type			The type. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CButton::CButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, 
				 int controlId, ButtonType type)
	:SallyAPI::GUI::CControl(parent, x, y, width, height, controlId), m_eType(type), m_iImage(0),
	m_bImageLeft(true), m_iImageSize(ICON_SIZE), m_bChecked(false), m_bActive(false), m_fDeltaStart(0),
	m_bDefaultButton(false), m_bUseHoleWidth(false)
{
	if (height == 0)
		m_iHeight = CONTROL_HEIGHT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CButton::~CButton()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CButton::~CButton()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::SetDefaultButton(bool value)
///
/// \brief	Sets a default button. 
/// Set this flag to true to make it a default button. A default button is displayed eye-catching.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::SetDefaultButton(bool value)
{
	m_bDefaultButton = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CButton::GetDefaultButton()
///
/// \brief	Gets the default button. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CButton::GetDefaultButton()
{
	return m_bDefaultButton;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::RenderControl()
{
	int borderLeft = 4;
	int borderRight = 4;
	int borderTop = 4;
	int borderBottom = 4;
	bool pressed = false;

	switch (m_eType)
	{
	case BUTTON_TYPE_FOUR_TEXTS:
	case BUTTON_TYPE_NORMAL:
		if (!m_bEnabled)
		{
			DrawButtonBackground(GUI_THEME_BUTTON_DISABLED_LEFT_TOP, GUI_THEME_BUTTON_DISABLED_CENTER_TOP, GUI_THEME_BUTTON_DISABLED_RIGHT_TOP, 
				GUI_THEME_BUTTON_DISABLED_LEFT, GUI_THEME_BUTTON_DISABLED_CENTER, GUI_THEME_BUTTON_DISABLED_RIGHT, 
				GUI_THEME_BUTTON_DISABLED_LEFT_BOTTOM, GUI_THEME_BUTTON_DISABLED_CENTER_BOTTOM, GUI_THEME_BUTTON_DISABLED_RIGHT_BOTTOM);
		}
		else
		{
			if ((m_fTimeDelta < m_fTimeMouseClick + 0.1) || ((m_fTimeDelta > m_fTimeMouseClick + 0.2) && (m_fTimeDelta < m_fTimeMouseClick + 0.3)))
				pressed = true;
			else if ((m_fTimeDelta >= m_fTimeMouseClick + 0.1) && (m_fTimeDelta <= m_fTimeMouseClick + 0.2))
				pressed = false;
			else if ((m_bChecked) || (m_bActive) || (m_bPressed))
				pressed = true;

			if (pressed)
			{
				DrawButtonBackground(GUI_THEME_BUTTON_SELECTED_LEFT_TOP, GUI_THEME_BUTTON_SELECTED_CENTER_TOP, GUI_THEME_BUTTON_SELECTED_RIGHT_TOP, 
					GUI_THEME_BUTTON_SELECTED_LEFT, GUI_THEME_BUTTON_SELECTED_CENTER, GUI_THEME_BUTTON_SELECTED_RIGHT, 
					GUI_THEME_BUTTON_SELECTED_LEFT_BOTTOM, GUI_THEME_BUTTON_SELECTED_CENTER_BOTTOM, GUI_THEME_BUTTON_SELECTED_RIGHT_BOTTOM);
				pressed = true;
			}
			else if (m_bDefaultButton)
			{
				DrawButtonBackground(GUI_THEME_BUTTON_DEFAULT_LEFT_TOP, GUI_THEME_BUTTON_DEFAULT_CENTER_TOP, GUI_THEME_BUTTON_DEFAULT_RIGHT_TOP, 
					GUI_THEME_BUTTON_DEFAULT_LEFT, GUI_THEME_BUTTON_DEFAULT_CENTER, GUI_THEME_BUTTON_DEFAULT_RIGHT, 
					GUI_THEME_BUTTON_DEFAULT_LEFT_BOTTOM, GUI_THEME_BUTTON_DEFAULT_CENTER_BOTTOM, GUI_THEME_BUTTON_DEFAULT_RIGHT_BOTTOM);
			}
			else
			{
				DrawButtonBackground(GUI_THEME_BUTTON_NORMAL_LEFT_TOP, GUI_THEME_BUTTON_NORMAL_CENTER_TOP, GUI_THEME_BUTTON_NORMAL_RIGHT_TOP, 
					GUI_THEME_BUTTON_NORMAL_LEFT, GUI_THEME_BUTTON_NORMAL_CENTER, GUI_THEME_BUTTON_NORMAL_RIGHT, 
					GUI_THEME_BUTTON_NORMAL_LEFT_BOTTOM, GUI_THEME_BUTTON_NORMAL_CENTER_BOTTOM, GUI_THEME_BUTTON_NORMAL_RIGHT_BOTTOM);
			}
		}

		if (m_iImage)
		{
			if (m_bImageLeft)
			{
				borderLeft += 4 + m_iImageSize;
				int imageWidthLeft = 0;

				if (m_strText.length() > 0)
				{
					// Temp variables
					SallyAPI::GUI::CPicture* image = NULL;

					image = GetPicture(GUI_THEME_BUTTON_SELECTED_LEFT);
					if (image != NULL) {
						imageWidthLeft = image->GetWidth();
					}
				}
				else
				{
					// center if we have no text
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

				if (m_strText.length() > 0)
				{
					// Temp variables
					SallyAPI::GUI::CPicture* image = NULL;

					image = GetPicture(GUI_THEME_BUTTON_SELECTED_RIGHT);
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

		switch (m_eType)
		{
		case BUTTON_TYPE_NORMAL:
			if (m_bUseHoleWidth)
			{
				if ((m_bActive) || (pressed))
					DrawText(0, 0, borderLeft, borderRight, "button.active.font");
				else if (m_bDefaultButton)
					DrawText(0, 0, borderLeft, borderRight, "button.default.font");
				else if (!m_bEnabled)
					DrawText(0, 0, borderLeft, borderRight, "button.font");
				else
					DrawText(0, 0, borderLeft, borderRight, "button.font");
			}
			else
			{
				if ((m_bActive) || (pressed))
					DrawText(GUI_THEME_BUTTON_SELECTED_LEFT, GUI_THEME_BUTTON_SELECTED_RIGHT, borderLeft, borderRight, "button.active.font");
				else if (m_bDefaultButton)
					DrawText(GUI_THEME_BUTTON_DEFAULT_LEFT, GUI_THEME_BUTTON_DEFAULT_RIGHT, borderLeft, borderRight, "button.default.font");
				else if (!m_bEnabled)
					DrawText(GUI_THEME_BUTTON_DISABLED_LEFT, GUI_THEME_BUTTON_DISABLED_RIGHT, borderLeft, borderRight, "button.font");
				else
					DrawText(GUI_THEME_BUTTON_NORMAL_LEFT, GUI_THEME_BUTTON_NORMAL_RIGHT, borderLeft, borderRight, "button.font");
			}
			break;
		case BUTTON_TYPE_FOUR_TEXTS: // button with 4 text elements
			RECT r;

			// left top
			r.left = m_iXAbsolut + borderLeft;
			r.top = m_iYAbsolut + borderTop;
			r.right = m_iXAbsolut + (m_iWidth / 2) - borderLeft;
			r.bottom = m_iYAbsolut + (m_iHeight / 2);
			DrawFourTextItem(r, m_strFourTexts[0], false, 0, pressed);

			// left bottom
			r.left = m_iXAbsolut + borderLeft;
			r.top = m_iYAbsolut + (m_iHeight / 2) - borderBottom;
			r.right = m_iXAbsolut + (m_iWidth / 2) - borderLeft;
			r.bottom = m_iYAbsolut + m_iHeight - borderBottom;
			DrawFourTextItem(r, m_strFourTexts[1], false, 1, pressed);

			// right top
			r.left = m_iXAbsolut + (m_iWidth / 2) - borderRight;
			r.top = m_iYAbsolut + borderTop;
			r.right = m_iXAbsolut + m_iWidth - borderRight;
			r.bottom = m_iYAbsolut + (m_iHeight / 2);
			DrawFourTextItem(r, m_strFourTexts[2], false, 2, pressed);

			// right bottom
			r.left = m_iXAbsolut + (m_iWidth / 2) - borderRight;
			r.top = m_iYAbsolut + (m_iWidth / 2) - borderBottom;
			r.right = m_iXAbsolut + m_iWidth - borderRight;
			r.bottom = m_iYAbsolut + m_iHeight  - borderBottom;
			DrawFourTextItem(r, m_strFourTexts[3], false, 3, pressed);
			break;
		}
		break;
	case BUTTON_TYPE_ONLY_IMAGE:
		DrawImage(m_iImage, 0, 0, m_iWidth, m_iHeight);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::DrawFourTextItem(RECT r, const std::string& text, bool active,
/// int itemNumber, bool pressed)
///
/// \brief	Draw four text item. 
///
/// \author	Christian Knobloch
/// \date	15.02.2011
///
/// \param	r			The. 
/// \param	text		The text. 
/// \param	active		true to active. 
/// \param	itemNumber	The item number. 
/// \param	pressed		true to pressed. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::DrawFourTextItem(RECT r, const std::string& text, bool active, int itemNumber, bool pressed)
{
	if (m_eType == BUTTON_TYPE_FOUR_TEXTS)
	{
		std::string fontFace;

		if (pressed)
			fontFace = "button.4text.active.font";
		else if (m_bDefaultButton)
			fontFace = "button.4text.default.font";
		else if (!m_bEnabled)
			fontFace = "button.4text.font";
		else
			fontFace = "button.4text.font";

		if (itemNumber == m_iFourTextsActive)
			fontFace.append(".bold");

		DrawText(r, fontFace, text);
	}
	else
	{
		if (pressed)
			DrawText(r, "button.active.font", text);
		else if (m_bDefaultButton)
			DrawText(r, "button.default.font", text);
		else if (!m_bEnabled)
			DrawText(r, "button.font", text);
		else
			DrawText(r, "button.font", text);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::SetFourTexts(int i, const std::string& text)
///
/// \brief	Sets text if the button is from the type BUTTON_TYPE_FOUR_TEXTS. 
///
/// \author	Christian Knobloch
/// \date	21.09.2010
///
/// \param	i		The index from 0 to 3 
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::SetFourTexts(int i, const std::string& text)
{
	if (i < 0 || i > 3)
		return;

	m_strFourTexts[i] = text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CButton::GetFourTexts(int i)
///
/// \brief	Gets a four texts. 
///
/// \author	Christian Knobloch
/// \date	26.09.2010
///
/// \param	i	The index. 
///
/// \return	The four texts. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CButton::GetFourTexts(int i)
{
	if (i < 0 || i > 3)
		return "";

	return m_strFourTexts[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::SetFourTextActive(int i)
///
/// \brief	Sets a four text active. 
///
/// \author	Christian Knobloch
/// \date	26.09.2010
///
/// \param	i	The index. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::SetFourTextActive(int i)
{
	if (i < -1 || i > 3)
		return;

	m_iFourTextsActive = i;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CButton::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
/// The button sends a GUI_BUTTON_CLICKED message to his parent.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CButton::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_BUTTON_CLICKED);
	}
	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::SetImageLeft()
///
/// \brief	Sets draw the image on the left side of the button. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::SetImageLeft()
{
	m_bImageLeft = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::SetImageRight()
///
/// \brief	Sets draw the image on the right side of the button. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::SetImageRight()
{
	m_bImageLeft = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CButton::GetCheckStatus()
///
/// \brief	Gets the check status. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CButton::GetCheckStatus()
{
	return m_bChecked;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::SetCheckStatus(bool value)
///
/// \brief	Sets a check status. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::SetCheckStatus(bool value)
{
	m_bChecked = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::SetImageId(int image)
///
/// \brief	Sets an image identifier. 
/// If you wan't to remove the image set GUI_NO_IMAGE.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	image	The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::SetImageId(int image)
{
	m_iImage = image;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CButton::ProcessMouseMove(int x, int y)
///
/// \brief	Process a mouse move. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CButton::ProcessMouseMove(int x, int y)
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::SetActive(bool set)
///
/// \brief	Sets the button active flag. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	set	true to set. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::SetActive(bool set)
{
	m_bActive = set;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CButton::GetActive()
///
/// \brief	Gets the active flag. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CButton::GetActive()
{
	return m_bActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButton::UseHoleWidth(bool use)
///
/// \brief	Set use hole width for text draw.
/// Usually a button has a border on the left and on the right.
/// The thickness of the border depends on the theme used. If you set this value to true there will be no border.
/// This flag should be used with align center.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	use	true to use. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButton::UseHoleWidth(bool use)
{
	m_bUseHoleWidth = use;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CButton::IsUseHoleWidth()
///
/// \brief	Query if this object is use hole width. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if use hole width, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CButton::IsUseHoleWidth()
{
	return m_bUseHoleWidth;
}