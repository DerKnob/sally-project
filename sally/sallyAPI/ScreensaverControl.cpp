////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScreensaverControl.cpp
///
/// \brief	Implements the scrennsaver control class. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "ScreensaverControl.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScreensaverControl::CScreensaverControl(SallyAPI::GUI::CGUIBaseObject* parent,
/// int controlId = 0) :SallyAPI::GUI::CControl(parent, 0, 0, 80, 80, controlId),
/// m_iImage(GUI_NO_IMAGE),
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScreensaverControl::CScreensaverControl(SallyAPI::GUI::CGUIBaseObject* parent, int controlId)
	:SallyAPI::GUI::CControl(parent, 0, 0, 80, 80, controlId), m_iImage(GUI_NO_IMAGE)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScreensaverControl::~CScreensaverControl()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CScreensaverControl::~CScreensaverControl()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScreensaverControl::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreensaverControl::RenderControl()
{
	if (m_iImage == GUI_NO_IMAGE)
		return;

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	SallyAPI::GUI::CPicture* picture = theme->GetPicture(m_iImage);
	if (picture != NULL)
	{
		picture->SetAlphaBlending(m_iAlphaBlending);

		if ((picture->GetWidth() < m_iWidth) && (picture->GetWidth() < m_iHeight))
		{
			int width = (m_iWidth - picture->GetWidth()) / 2;
			int height = (m_iHeight - picture->GetHeight()) / 2;

			picture->Draw(m_iXAbsolut + width, m_iYAbsolut + height,
				picture->GetWidth(), picture->GetHeight());
		}
		else
		{
			picture->Draw(m_iXAbsolut, m_iYAbsolut, m_iWidth, m_iHeight);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScreensaverControl::ProcessMouseUp(int x, int y)
///
/// \brief	Process a mouse up. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScreensaverControl::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_SCREENSAVER_CONTROL_CLICKED, &message);
	}
	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScreensaverControl::ProcessMouseDoubleClick(int x, int y)
///
/// \brief	Process a mouse double click. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScreensaverControl::ProcessMouseDoubleClick(int x, int y)
{
	SallyAPI::GUI::CControl::ProcessMouseDoubleClick(x, y);

	m_pParent->SendMessageToParent(this, m_iControlId, GUI_SCREENSAVER_CONTROL_DOUBLECLICKED);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScreensaverControl::SetImageId(int image)
///
/// \brief	Sets an image identifier. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// \param	image	The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreensaverControl::SetImageId(int image)
{
	m_iImage = image;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CScreensaverControl::GetImageId()
///
/// \brief	Gets the image identifier. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// \return	The image identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CScreensaverControl::GetImageId()
{
	return m_iImage;
}