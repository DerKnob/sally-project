////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\GUIBaseObject.cpp
///
/// \brief	Implements the graphical user interface base object class. 
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

#include "GUIBaseObject.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGUIBaseObject::CGUIBaseObject(CGUIBaseObject* parent)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CGUIBaseObject::CGUIBaseObject(CGUIBaseObject* parent)
	:m_fTimeDelta(0), m_pParent(parent), m_iX(0), m_iY(0), m_iWidth(0), m_iHeight(0),
	m_iXAbsolut(0), m_iYAbsolut(0), m_iAlphaBlending(255), m_fAngleY(0.0), m_fAngleX(0.0), m_fAngleZ(0.0),
	m_bUpdateRequested(false)
{
	InitializeCriticalSection(&m_critSectLockRender);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGUIBaseObject::~CGUIBaseObject()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CGUIBaseObject::~CGUIBaseObject()
{
	DeleteCriticalSection(&m_critSectLockRender);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::EnterRenderLock()
///
/// \brief	Enter render lock. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::EnterRenderLock()
{
	EnterCriticalSection(&m_critSectLockRender);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::LeaveRenderLock()
///
/// \brief	Leave render lock. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::LeaveRenderLock()
{
	LeaveCriticalSection(&m_critSectLockRender);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::SetAlphaBlending(int alphaBlending)
///
/// \brief	Sets an alpha blending. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	alphaBlending	The alpha blending. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::SetAlphaBlending(int alphaBlending)
{
	m_iAlphaBlending = alphaBlending;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CGUIBaseObject::GetAlphaBlending()
///
/// \brief	Gets the alpha blending. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The alpha blending. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CGUIBaseObject::GetAlphaBlending()
{
	return m_iAlphaBlending;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::SetRotationAngleY(float angle)
///
/// \brief	Sets a rotation angle y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angle	The angle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::SetRotationAngleY(float angle)
{
	m_fAngleY = angle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float CGUIBaseObject::GetRotationAngleY()
///
/// \brief	Gets the rotation angle y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The rotation angle y coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

float CGUIBaseObject::GetRotationAngleY()
{
	return m_fAngleY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::SetRotationAngleX(float angle)
///
/// \brief	Sets a rotation angle x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angle	The angle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::SetRotationAngleX(float angle)
{
	m_fAngleX = angle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float CGUIBaseObject::GetRotationAngleX()
///
/// \brief	Gets the rotation angle x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The rotation angle x coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

float CGUIBaseObject::GetRotationAngleX()
{
	return m_fAngleX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::SetRotationAngleZ(float angle)
///
/// \brief	Sets a rotation angle z coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	angle	The angle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::SetRotationAngleZ(float angle)
{
	m_fAngleZ = angle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float CGUIBaseObject::GetRotationAngleZ()
///
/// \brief	Gets the rotation angle z coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The rotation angle z coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

float CGUIBaseObject::GetRotationAngleZ()
{
	return m_fAngleZ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::Timer(float timeDelta)
{
	m_fTimeDelta += timeDelta;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::SetText(const std::string& text)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::SetText(const std::string& text)
{
	m_strText = text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CGUIBaseObject::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CGUIBaseObject::GetText()
{
	return m_strText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::Render()
///
/// \brief	Renders this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::Render()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::Update()
///
/// \brief	Updates this object. 
///
/// \author	Christian Knobloch
/// \date	30.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::Update()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::GetAbsolutPosition(int* x, int* y)
///
/// \brief	Gets an absolut position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	x	If non-null, the x coordinate. 
/// \param [in,out]	y	If non-null, the y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::GetAbsolutPosition(int* x, int* y)
{
	if (m_pParent != NULL)
		m_pParent->GetAbsolutPosition(x, y);

	*x += m_iX;
	*y += m_iY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::DrawImage(int gui, int x, int y)
///
/// \brief	Draw image. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	gui	The graphical user interface. 
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::DrawImage(int gui, int x, int y)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	SallyAPI::GUI::CPicture* picture = theme->GetPicture(gui);
	if (picture != NULL)
	{
		picture->SetRotationAngleY(m_fAngleY);
		picture->SetRotationAngleX(m_fAngleX);
		picture->SetRotationAngleZ(m_fAngleZ);
		picture->SetAlphaBlending(m_iAlphaBlending);
		picture->Draw(m_iXAbsolut + x, m_iYAbsolut + y);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::DrawImagePart(int gui, int x, int y, int srcX, int srcY, int srcW,
/// int srcH)
///
/// \brief	Draw image part. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	gui		The graphical user interface. 
/// \param	x		The x coordinate. 
/// \param	y		The y coordinate. 
/// \param	srcX	Source x coordinate. 
/// \param	srcY	Source y coordinate. 
/// \param	srcW	Source w. 
/// \param	srcH	Source h. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::DrawImagePart(int gui, int x, int y, int srcX, int srcY, int srcW, int srcH)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	SallyAPI::GUI::CPicture* picture = theme->GetPicture(gui);
	if (picture != NULL)
	{
		picture->SetRotationAngleY(m_fAngleY);
		picture->SetRotationAngleX(m_fAngleX);
		picture->SetRotationAngleZ(m_fAngleZ);
		picture->SetAlphaBlending(m_iAlphaBlending);
		picture->Draw(m_iXAbsolut + x, m_iYAbsolut + y);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGUIBaseObject::DrawImage(int gui, int x, int y, int width, int height)
///
/// \brief	Draw image. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	gui		The graphical user interface. 
/// \param	x		The x coordinate. 
/// \param	y		The y coordinate. 
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGUIBaseObject::DrawImage(int gui, int x, int y, int width, int height)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	SallyAPI::GUI::CPicture* picture = theme->GetPicture(gui);
	if (picture != NULL)
	{
		picture->SetRotationAngleY(m_fAngleY);
		picture->SetRotationAngleX(m_fAngleX);
		picture->SetRotationAngleZ(m_fAngleZ);
		picture->SetAlphaBlending(m_iAlphaBlending);
		picture->Draw(m_iXAbsolut+ x, m_iYAbsolut + y, width, height);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CPicture* CGUIBaseObject::GetPicture(int gui)
///
/// \brief	Gets an image. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	gui	The graphical user interface. 
///
/// \return	null if it fails, else the image. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CPicture* CGUIBaseObject::GetPicture(int gui)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	SallyAPI::GUI::CPicture* picture = theme->GetPicture(gui);
	return picture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	RECT CGUIBaseObject::GetImageRect(int gui)
///
/// \brief	Gets an image rectangle. 
///
/// \author	Christian Knobloch
/// \date	03.05.2010
///
/// \param	gui	The graphical user interface. 
///
/// \return	The image rectangle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

RECT CGUIBaseObject::GetImageRect(int gui)
{
	SallyAPI::GUI::CPicture* image = GetPicture(gui);

	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;

	if (image != NULL)
	{
		rect.right = image->GetWidth();
		rect.bottom = image->GetHeight();
	}
	return rect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CGUIBaseObject::GetWidth()
///
/// \brief	Gets the width. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The width. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CGUIBaseObject::GetWidth()
{
	return m_iWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CGUIBaseObject::GetHeight()
///
/// \brief	Gets the height. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CGUIBaseObject::GetHeight()
{
	return m_iHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CGUIBaseObject::GetPositionX()
///
/// \brief	Gets the position x coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The position x coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CGUIBaseObject::GetPositionX()
{
	return m_iX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CGUIBaseObject::GetPositionY()
///
/// \brief	Gets the position y coordinate coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The position y coordinate coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CGUIBaseObject::GetPositionY()
{
	return m_iY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CGUIBaseObject* CGUIBaseObject::GetParent()
///
/// \brief	Gets the parent of this item. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CGUIBaseObject* CGUIBaseObject::GetParent()
{
	return m_pParent;
}