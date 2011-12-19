////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DropDown.h
///
/// \brief	Declares the drop down class. 
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
#include "Control.h"
#include "DropDownItem.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CDropDown
		///
		/// \brief	Drop down. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CDropDown :
			public SallyAPI::GUI::CControl
		{
		private:
			bool		m_bParentLocalised;
			int			m_iImage;
			int			m_iImageSize;
			std::string	m_strIdentifier;
			float		m_fDeltaStart;
			std::vector<SallyAPI::GUI::CDropDownItem>	m_vItems;

			void			SetImageId(int image);
			virtual void	SetText(const std::string& text);

			virtual void	RenderControl();
			virtual bool	ProcessMouseUp(int x, int y);
		public:
			CDropDown(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId = 0);
			virtual ~CDropDown();

			virtual void	SetLocalised(bool localised);
			virtual bool	IsLocalised();

			void		AddItem(SallyAPI::GUI::CDropDownItem& item);

			bool		SelectItemById(int id);
			bool		SelectItemByIdentifier(const std::string& identifier);
			std::string	GetSelectedIdentifier();

			void		Clear();

			int			GetListSize();

			void		SortByName();

			std::vector<SallyAPI::GUI::CDropDownItem>& GetItemList();
		};
	}
}
