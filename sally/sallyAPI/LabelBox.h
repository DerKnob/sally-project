////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\LabelBox.h
///
/// \brief	Declares the label box class. 
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

#pragma once
#include "Define.h"
#include "Form.h"
#include "Scrollbar.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CLabelBox
		///
		/// \brief	Label box. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CLabelBox :
			public SallyAPI::GUI::CForm
		{
		private:
			SallyAPI::GUI::CScrollbar*	m_pScrollbar;
			std::string					m_strFontName;
			bool						m_bAutoResize;

			void						AutoResize();
			void						UpdateScrollbar();
			virtual void				RenderControl();
			virtual void				UpdateControl();
		public:
			CLabelBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height,
				bool showScrollbar = false, int controlId = 0);
			virtual ~CLabelBox();

			void SetFont(const std::string& fontName);
			void SetAutoResize(bool value);

			virtual void	SetText(const std::string& text);
			virtual void	Resize(int width, int height);

			virtual void	SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
		};
	}
}