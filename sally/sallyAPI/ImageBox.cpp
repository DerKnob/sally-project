////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ImageBox.cpp
///
/// \brief	Implements the image box class. 
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

#include "ImageBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CImageBox::CImageBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CControl(parent, x, y, width, height, controlId),
/// m_pPicture(NULL), m_iImage(GUI_NO_IMAGE), m_eDiyplayType(IMAGEBOX_DISPLAY_TYPE_STRETCH)
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
////////////////////////////////////////////////////////////////////////////////////////////////////

CImageBox::CImageBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CControl(parent, x, y, width, height, controlId), m_pPicture(NULL), m_iImage(GUI_NO_IMAGE),
	m_eDiyplayType(IMAGEBOX_DISPLAY_TYPE_STRETCH)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CImageBox::~CImageBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CImageBox::~CImageBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CImageBox::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CImageBox::RenderControl()
{
	if (m_iImage != GUI_NO_IMAGE)
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
		SallyAPI::Config::CTheme* theme = config->GetTheme();

		SallyAPI::GUI::CPicture* picture = theme->GetPicture(m_iImage);
		if (picture != NULL)
		{
			picture->SetRotationAngleY(m_fAngleY);
			picture->SetRotationAngleX(m_fAngleX);
			picture->SetRotationAngleZ(m_fAngleZ);
			picture->SetAlphaBlending(m_iAlphaBlending);

			if (m_eDiyplayType == IMAGEBOX_DISPLAY_TYPE_SCALE)
			{
				RECT imageRect;

				SallyAPI::GUI::GUIHelper::CalculateImageSize(picture->GetWidth(), picture->GetHeight(), m_iWidth, m_iHeight,
					(int&) imageRect.left, (int&) imageRect.top, (int&) imageRect.right, (int&) imageRect.bottom);

				imageRect.left += m_iXAbsolut;
				imageRect.top += m_iYAbsolut;

				picture->Draw(imageRect.left, imageRect.top, imageRect.right, imageRect.bottom);
			}
			else if (m_eDiyplayType == IMAGEBOX_DISPLAY_TYPE_STRETCH)
			{
				picture->Draw(m_iXAbsolut, m_iYAbsolut, m_iWidth, m_iHeight);
			}
			else if (m_eDiyplayType == IMAGEBOX_DISPLAY_TYPE_CENTER)
			{
				if ((picture->GetWidth() < m_iWidth) && (picture->GetWidth() < m_iHeight))
				{
					int width = (m_iWidth - picture->GetWidth()) / 2;
					int height = (m_iHeight - picture->GetHeight()) / 2;

					picture->Draw(m_iXAbsolut + width, m_iYAbsolut + height,
						m_iWidth + picture->GetWidth(), m_iHeight + picture->GetHeight());
				}
				else
				{
					picture->Draw(m_iXAbsolut, m_iYAbsolut, m_iWidth, m_iHeight);
				}
			}
		}
	}
	else if (m_pPicture != NULL)
	{
		if (m_pPicture->GetTexture() != NULL)
		{
			m_pPicture->SetRotationAngleY(m_fAngleY);
			m_pPicture->SetRotationAngleX(m_fAngleX);
			m_pPicture->SetRotationAngleZ(m_fAngleZ);
			m_pPicture->SetAlphaBlending(m_iAlphaBlending);

			if (m_eDiyplayType == IMAGEBOX_DISPLAY_TYPE_SCALE)
			{
				RECT imageRect;

				SallyAPI::GUI::GUIHelper::CalculateImageSize(m_pPicture->GetWidth(), m_pPicture->GetHeight(), m_iWidth, m_iHeight,
					(int&) imageRect.left, (int&) imageRect.top, (int&) imageRect.right, (int&) imageRect.bottom);

				imageRect.left += m_iXAbsolut;
				imageRect.top += m_iYAbsolut;

				m_pPicture->Draw(imageRect.left, imageRect.top, imageRect.right, imageRect.bottom);
			}
			else if (m_eDiyplayType == IMAGEBOX_DISPLAY_TYPE_STRETCH)
			{
				m_pPicture->Draw(m_iXAbsolut, m_iYAbsolut, m_iWidth, m_iHeight);
			}
			else if (m_eDiyplayType == IMAGEBOX_DISPLAY_TYPE_CENTER)
			{
				if ((m_pPicture->GetWidth() < m_iWidth) && (m_pPicture->GetWidth() < m_iHeight))
				{
					int width = (m_iWidth - m_pPicture->GetWidth()) / 2;
					int height = (m_iHeight - m_pPicture->GetHeight()) / 2;

					m_pPicture->Draw(m_iXAbsolut + width, m_iYAbsolut + height,
						m_iWidth + m_pPicture->GetWidth(), m_iHeight + m_pPicture->GetHeight());
				}
				else
				{
					m_pPicture->Draw(m_iXAbsolut, m_iYAbsolut, m_iWidth, m_iHeight);
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CImageBox::ProcessMouseUp(int x, int y)
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

bool CImageBox::ProcessMouseUp(int x, int y)
{
	if (m_bMouseDown)
	{
		SallyAPI::GUI::SendMessage::CParameterPoint message(x, y);
		m_pParent->SendMessageToParent(this, m_iControlId, GUI_IMAGEBOX_CLICKED, &message);
	}
	SallyAPI::GUI::CControl::ProcessMouseUp(x, y);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CImageBox::ProcessMouseMove(int x, int y)
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

bool CImageBox::ProcessMouseMove(int x, int y)
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CImageBox::SetImageId(int image)
///
/// \brief	Sets an image to this image box loaded by LoadPicture. Set to GUI_NO_IMAGE to erase the image.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	image	The image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CImageBox::SetImageId(int image)
{
	m_iImage = image;
	m_pPicture = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CImageBox::SetPicture(SallyAPI::GUI::CPicture* picture)
///
/// \brief	Sets a picture to this image box. Set to NULL to erase the image.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	picture	If non-null, the picture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CImageBox::SetPicture(SallyAPI::GUI::CPicture* picture)
{
	m_pPicture = picture;
	m_iImage = GUI_NO_IMAGE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CPicture* CImageBox::GetPicture()
///
/// \brief	Gets the picture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the picture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CPicture* CImageBox::GetPicture()
{
	return m_pPicture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CImageBox::GetImageId()
///
/// \brief	Gets the image identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The image identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CImageBox::GetImageId()
{
	return m_iImage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CImageBox::ProcessMouseDoubleClick(int x, int y)
///
/// \brief	Process a mouse double click. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CImageBox::ProcessMouseDoubleClick(int x, int y)
{
	SallyAPI::GUI::CControl::ProcessMouseDoubleClick(x, y);

	m_pParent->SendMessageToParent(this, m_iControlId, GUI_IMAGEBOX_DOUBLECLICKED);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CImageBox::SetDiyplayType(ImageBoxDisplayType value)
///
/// \brief	Sets a diyplay type. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CImageBox::SetDiyplayType(ImageBoxDisplayType value)
{
	m_eDiyplayType = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ImageBoxDisplayType CImageBox::GetDiyplayType()
///
/// \brief	Gets the diyplay type. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The diyplay type. 
////////////////////////////////////////////////////////////////////////////////////////////////////

ImageBoxDisplayType CImageBox::GetDiyplayType()
{
	return m_eDiyplayType;
}