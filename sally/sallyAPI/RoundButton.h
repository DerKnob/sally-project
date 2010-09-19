////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\RoundButton.h
///
/// \brief	Declares the round button class. 
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
#include "Button.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	RoundButtonType
		///
		/// \brief	Values that represent RoundButtonType. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum RoundButtonType {ROUNDBUTTON_TYPE_SMALL, ROUNDBUTTON_TYPE_BIG};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CRoundButton
		///
		/// \brief	Round button. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CRoundButton :
			public SallyAPI::GUI::CButton
		{
		private:
			SallyAPI::GUI::RoundButtonType		m_eRoundButtonType;

			virtual void RenderControl();
		public:
			CRoundButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int controlId = 0, SallyAPI::GUI::RoundButtonType type = ROUNDBUTTON_TYPE_SMALL);
			virtual ~CRoundButton();
		};
	}
}