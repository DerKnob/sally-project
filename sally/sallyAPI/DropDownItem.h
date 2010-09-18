////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DropDownItem.h
///
/// \brief	Declares the drop down item class. 
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
#include <string>

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	DROPDOWN_LOCALISATION
		///
		/// \brief	Values that represent DROPDOWN_LOCALISATION. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum DROPDOWN_LOCALISATION {DROPDOWN_LOCALISATION_FROM_PARENT, DROPDOWN_LOCALISATION_TRUE, DROPDOWN_LOCALISATION_FALSE};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CDropDownItem
		///
		/// \brief	Drop down item. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CDropDownItem
		{
		private:
			std::string				m_strIdentifier;
			std::string				m_strText;
			int						m_iImage;
			DROPDOWN_LOCALISATION	m_eLocalisation;
		public:
			CDropDownItem(const std::string& identifier, const std::string& text, int image = 0);
			~CDropDownItem();

			void		SetText(const std::string& text);
			std::string GetText();
			void		SetIdentifier(const std::string& identifier);
			std::string GetIdentifier();
			void		SetImageId(int image);
			int			GetImageId();
			void		SetLocalised(DROPDOWN_LOCALISATION value);
			DROPDOWN_LOCALISATION	IsLocalised();
		};
	}
}