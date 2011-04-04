////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Font.cpp
///
/// \brief	Implements the font class. 
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

#include "Font.h"

#define getA(c) (((c)&0xff000000)>>24)
#define getR(c) (((c)&0x00ff0000)>>16)
#define getG(c) (((c)&0x0000ff00)>>8)
#define getB(c) ((c)&0x000000ff)

using namespace SallyAPI::Core;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFont::CFont(const std::string& fontFace, int height, bool bold, bool italic,
/// bool underlined)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fontFace	The font face. 
/// \param	height		The height. 
/// \param	bold		true to bold. 
/// \param	italic		true to italic. 
/// \param	underlined	true to underlined. 
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Config.h"

CFont::CFont(const std::string& fontFace, int height, bool bold, bool italic, bool underlined)
	:m_rgbFontColour(0), m_iAlign(DT_LEFT), m_iAlphaBlending(255)

{
	int iWeight			= FW_NORMAL;
	int iQuality		= DEFAULT_QUALITY;
	DWORD dwItalic		= 0;
	DWORD dwUnderlined	= 0;

	if(bold)
		iWeight = FW_BOLD;
	if(italic)
		dwItalic = 1;
	if(underlined)
		dwUnderlined = 1;

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if (option->GetPropertyBool("sally", "fontAntialasing", true))
		iQuality = PROOF_QUALITY;

	HRESULT hr = D3DXCreateFont(SallyAPI::Core::CGame::GetDevice(), (int) (height * 1.6), 0, iWeight, 2,
		dwItalic, DEFAULT_CHARSET, OUT_RASTER_PRECIS, iQuality, DEFAULT_PITCH|FF_DONTCARE, (LPSTR) fontFace.c_str(), &m_pFont);
	if (hr != S_OK)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error("CFont::CFont()");
		logger->Error(hr);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFont::~CFont()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFont::~CFont()
{
	SafeRelease(m_pFont);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	RECT CFont::CalcualteSize(const std::string& text, int align)
///
/// \brief	Calcualte size. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param	text	The text. 
/// \param	align	The align. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

RECT CFont::CalcualteSize(const std::string& text, int align)
{
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0;

	align |= DT_CALCRECT;

	m_pFont->DrawText(0, text.c_str(), -1, &rect, align, 0);
	
	return rect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	RECT CFont::CalcualteSize(const std::string& text, int align, RECT rectSize)
///
/// \brief	Calcualte size. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// \param	text		The text. 
/// \param	align		The align. 
/// \param	rectSize	Size of the rectangle. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

RECT CFont::CalcualteSize(const std::string& text, int align, RECT rectSize)
{
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = rectSize.right - rectSize.left;
	rect.bottom = 0;

	align |= DT_CALCRECT;

	m_pFont->DrawText(0, text.c_str(), -1, &rect, align, 0);
	
	return rect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFont::DrawText(const std::string& text, int x, int y, D3DCOLOR rgbFontColour,
/// int align)
///
/// \brief	Draw text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text			The text. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	rgbFontColour	The rgb font colour. 
/// \param	align			The align. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFont::DrawText(const std::string& text, int x, int y, D3DCOLOR rgbFontColour, int align)
{
	RECT rect;

	rect.left = x;
	rect.top = y;
	rect.right = 0;
	rect.bottom = 0;

	m_pFont->DrawText(0, text.c_str(), -1, &rect, DT_CALCRECT, 0);
	
	DrawText(text, &rect, rgbFontColour, align);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFont::DrawText(const std::string& text, RECT* rect, D3DCOLOR rgbFontColour,
/// int align)
///
/// \brief	Draw text. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text			The text. 
/// \param [in,out]	Rect	If non-null, the rectangle. 
/// \param	rgbFontColour	The rgb font colour. 
/// \param	align			The align. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFont::DrawText(const std::string& text, RECT* rect, D3DCOLOR rgbFontColour, int align)
{
	if (rgbFontColour == NULL)
		rgbFontColour = m_rgbFontColour;

	if (align == -1)
		m_pFont->DrawText(0, text.c_str(), -1, rect, m_iAlign, rgbFontColour);	//Draw the text
	else
		m_pFont->DrawText(0, text.c_str(), -1, rect, align, rgbFontColour);	//Draw the text
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFont::SetAlign(int align)
///
/// \brief	Sets an align. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	align	The align. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFont::SetAlign(int align)
{
	m_iAlign = align;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFont::SetColor(D3DCOLOR rgbFontColour)
///
/// \brief	Sets a color. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	rgbFontColour	The rgb font colour. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFont::SetColor(D3DCOLOR rgbFontColour)
{
	D3DCOLOR color = D3DCOLOR_ARGB(m_iAlphaBlending, getR(rgbFontColour), getG(rgbFontColour), getB(rgbFontColour));
	m_rgbFontColour = color;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	HRESULT CFont::OnLostDevice()
///
/// \brief	Executes the lost device action. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT CFont::OnLostDevice()
{
	return m_pFont->OnLostDevice();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	HRESULT CFont::OnResetDevice()
///
/// \brief	Executes the reset device action. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT CFont::OnResetDevice()
{
	return m_pFont->OnResetDevice();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFont::SetAlphaBlending(int alphaBlending)
///
/// \brief	Sets an alpha blending. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	alphaBlending	The alpha blending. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFont::SetAlphaBlending(int alphaBlending)
{
 	D3DCOLOR color = D3DCOLOR_ARGB(alphaBlending, getR(m_rgbFontColour), getG(m_rgbFontColour), getB(m_rgbFontColour));
 	m_rgbFontColour = color;
	m_iAlphaBlending = alphaBlending;
}