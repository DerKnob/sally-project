////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScreensaverControlLabel.h
///
/// \brief	Declares the screensaver control label class. 
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

#pragma once
#include "Form.h"
#include "LabelBox.h"
#include "GUIHelper.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CScreensaverControlLabel
		///
		/// \brief	Screensaver control label. 
		///
		/// \author	Christian Knobloch
		/// \date	20.01.2012
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CScreensaverControlLabel
			: public SallyAPI::GUI::CForm
		{
		protected:
			SallyAPI::GUI::CLabelBox*	m_pLabelBox;

			virtual void	RenderControl();
		public:
			CScreensaverControlLabel(SallyAPI::GUI::CGUIBaseObject* parent, int controlId = 0);
			virtual ~CScreensaverControlLabel();

			void		SetText(const std::string& text);
			std::string	GetText();
		};
	}
}