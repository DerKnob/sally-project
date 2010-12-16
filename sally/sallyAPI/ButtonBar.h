////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ButtonBar.h
///
/// \brief	Declares the button bar class. 
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
#include "ButtonBarButton.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CButtonBar
		///
		/// \brief	Button bar. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CButtonBar
			: public SallyAPI::GUI::CForm
		{
		private:
			std::vector<SallyAPI::GUI::CButtonBarButton*>	m_Buttons;

			int m_iPositon;

			virtual void AddChild(SallyAPI::GUI::CControl* control);
		public:
			CButtonBar(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId = 0);
			virtual ~CButtonBar();

			void AddChild(SallyAPI::GUI::CButtonBarButton* button);
			SallyAPI::GUI::CButtonBarButton*	GetButton(int i);
		};
	}
}