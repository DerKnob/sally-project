////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DropDown.cpp
///
/// \brief	Implements the drop down class. 
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

#include "DropDown.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDropDown::CDropDown(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId),
/// m_iImage(NULL), m_fDeltaStart(0), m_iImageSize(ICON_SIZE), m_bParentLocalised(true)
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
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CDropDown::CDropDown(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, CONTROL_HEIGHT, controlId), m_iImage(NULL), m_fDeltaStart(0),
	m_iImageSize(ICON_SIZE), m_bParentLocalised(true)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDropDown::~CDropDown()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CDropDown::~CDropDown()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDown::SetImageId(int image)
///
/// \brief	Sets an image identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	image	The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDown::SetImageId(int image)
{
	m_iImage = image;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDown::SetText(const std::string& text)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDown::SetText(const std::string& text)
{
	SallyAPI::GUI::CControl::SetText(text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDown::AddItem(SallyAPI::GUI::CDropDownItem& item)
///
/// \brief	Adds an item. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	item	The item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDown::AddItem(SallyAPI::GUI::CDropDownItem& item)
{
	m_vItems.push_back(item);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CDropDown::SelectItemById(int id)
///
/// \brief	Select item by identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CDropDown::SelectItemById(int id)
{
	if (id > (int) m_vItems.size())
		return false;

	SallyAPI::GUI::CDropDownItem item = m_vItems.at(id);
	SelectItemByIdentifier(item.GetIdentifier());

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CDropDown::SelectItemByIdentifier(const std::string& identifier)
///
/// \brief	Select item by identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	identifier	The identifier. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CDropDown::SelectItemByIdentifier(const std::string& identifier)
{
	std::vector<SallyAPI::GUI::CDropDownItem>::iterator iter = m_vItems.begin();

	while (iter != m_vItems.end())
	{
		std::string ident = (*iter).GetIdentifier();
		if (identifier.compare(ident) == 0)
		{
			if ((*iter).IsLocalised() == SallyAPI::GUI::DROPDOWN_LOCALISATION_FROM_PARENT)
				m_bLocalised = m_bParentLocalised;
			else if ((*iter).IsLocalised() == SallyAPI::GUI::DROPDOWN_LOCALISATION_FALSE)
				m_bLocalised = false;
			else
				m_bLocalised = true;

			SetText((*iter).GetText());
			SetImageId((*iter).GetImageId());

			m_strIdentifier = (*iter).GetIdentifier();
			return true;
		}
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CDropDown::GetSelectedIdentifier()
///
/// \brief	Gets the selected identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The selected identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CDropDown::GetSelectedIdentifier()
{
	return m_strIdentifier;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDown::Clear()
///
/// \brief	Clears this object to its blank/initial state. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDown::Clear()
{
	m_vItems.clear();
	m_strIdentifier = "";
	m_iImage = NULL;
	SetText("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CDropDown::ProcessMouseUp(int x, int y)
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

bool CDropDown::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_DROPDOWN_CLICKED);
	}
	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<SallyAPI::GUI::CDropDownItem>& CDropDown::GetItemList()
///
/// \brief	Gets the item list. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The item list. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<SallyAPI::GUI::CDropDownItem>& CDropDown::GetItemList()
{
	return m_vItems;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CDropDown::GetListSize()
///
/// \brief	Gets the list size. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The list size. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CDropDown::GetListSize()
{
	return m_vItems.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDown::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDown::RenderControl()
{
	int borderLeft = 4;
	int borderRight = 4;
	
	if (!m_bEnabled)
		DrawButtonBackground(GUI_THEME_DROPDOWN_DISABLED_LEFT, GUI_THEME_DROPDOWN_DISABLED, GUI_THEME_DROPDOWN_DISABLED_RIGHT);
	else if (m_bPressed)
		DrawButtonBackground(GUI_THEME_DROPDOWN_SELECTED_LEFT, GUI_THEME_DROPDOWN_SELECTED, GUI_THEME_DROPDOWN_SELECTED_RIGHT);
	else
		DrawButtonBackground(GUI_THEME_DROPDOWN_NORMAL_LEFT, GUI_THEME_DROPDOWN_NORMAL, GUI_THEME_DROPDOWN_NORMAL_RIGHT);

	if (m_iImage)
	{
		borderLeft += 4 + m_iImageSize;
		int imageWidthLeft = 0;

		if (m_strText.length() > 0)
		{
			// Temp variables
			SallyAPI::GUI::CPicture* image = NULL;

			image = GetPicture(GUI_THEME_DROPDOWN_NORMAL_LEFT);
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
	if (m_bPressed)
		DrawText(GUI_THEME_DROPDOWN_NORMAL_LEFT, GUI_THEME_DROPDOWN_NORMAL_RIGHT, borderLeft, borderRight, "dropdown.active.font");
	else
		DrawText(GUI_THEME_DROPDOWN_NORMAL_LEFT, GUI_THEME_DROPDOWN_NORMAL_RIGHT, borderLeft, borderRight, "dropdown.font");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CDropDown::SetLocalised(bool localised)
///
/// \brief	Sets a localised. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	localised	true to localised. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CDropDown::SetLocalised(bool localised)
{
	m_bParentLocalised = localised;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CDropDown::IsLocalised()
///
/// \brief	Query if this object is localised. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if localised, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CDropDown::IsLocalised()
{
	return m_bParentLocalised;
}