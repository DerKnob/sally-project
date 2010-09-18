////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Font.h
///
/// \brief	Declares the font class. 
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

#pragma once
#include "Define.h"

namespace SallyAPI
{
	namespace Core
	{
		class CFont;
	}
}

#include "BaseObject.h"
#include "Game.h"

namespace SallyAPI
{
	namespace Core
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CFont
		///
		/// \brief	Font. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CFont :
			public SallyAPI::Core::CBaseObject
		{
		private:
			int			m_iAlphaBlending;
			LPD3DXFONT	m_pFont;
			int			m_iAlign;
			D3DCOLOR	m_rgbFontColour;
		public:
			CFont(const std::string& fontFace, int height, bool bold, bool italic, bool underlined);
			virtual ~CFont();

			RECT		CalcualteSize(const std::string& text, int align);
			RECT		CalcualteSize(const std::string& text, int align, RECT rectSize);

			void		DrawText(const std::string& text, int x, int y, D3DCOLOR rgbFontColour = NULL, int align = -1);
			void		DrawText(const std::string& text, RECT* Rect, D3DCOLOR rgbFontColour = NULL, int align = -1);

			void		SetAlign(int algin);
			void		SetColor(D3DCOLOR rgbFontColour);

			HRESULT		OnLostDevice();
			HRESULT		OnResetDevice();

			void		SetAlphaBlending(int alphaBlending);
		};
	}
}