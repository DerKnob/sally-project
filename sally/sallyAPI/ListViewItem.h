////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ListViewItem.h
///
/// \brief	Declares the list view item class. 
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
#include <string>
#include <map>

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	LISTVIEW_LOCALISATION
		///
		/// \brief	Values that represent LISTVIEW_LOCALISATION. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum LISTVIEW_LOCALISATION {LISTVIEW_LOCALISATION_FROM_PARENT, LISTVIEW_LOCALISATION_TRUE, LISTVIEW_LOCALISATION_FALSE};

		enum LISTVIEWITEM_TYPE {LISTVIEWITEM_TYPE_NORMAL, LISTVIEWITEM_TYPE_SORTER};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CListViewItem
		///
		/// \brief	List view item. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CListViewItem
		{
		private:
			std::string								m_strIdentifier;
			std::map<int, std::string>				m_mText;
			std::map<int, int>						m_mImageId;
			std::map<int, LISTVIEW_LOCALISATION>	m_mLocalisation;
			std::map<int, bool>						m_mFont;
			std::map<int, LISTVIEWITEM_TYPE>		m_mType;
		public:
			CListViewItem(const std::string& identifier, const std::string& firstLine = "",
				int imageIndex = GUI_NO_IMAGE, LISTVIEWITEM_TYPE type = LISTVIEWITEM_TYPE_NORMAL);
			virtual ~CListViewItem();

			void		SetText(const std::string& value, int number = 0);
			std::string GetText(int number = 0);
			void		SetIdentifier(const std::string& identifier);
			std::string GetIdentifier();
			void		SetImageId(int value, int number = 0);
			int			GetImageId(int number = 0);
			void					SetLocalised(LISTVIEW_LOCALISATION value, int number = 0);
			LISTVIEW_LOCALISATION	IsLocalised(int number = 0);
			void		SetSmallFont(bool value, int number = 0);
			bool		GetSmallFont(int number = 0);
			void					SetType(LISTVIEWITEM_TYPE value, int number = 0);
			LISTVIEWITEM_TYPE		GetType(int number = 0);
		};
	}
}