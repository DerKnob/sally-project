////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScrollList.h
///
/// \brief	Declares the scroll list class. 
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
#include "ScrollListItem.h"
#include "Button.h"
#include "LabelBox.h"
#include "Form.h"
#include "Box2DObject.h"
#include "ParameterScrollList.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CScrollList
		///
		/// \brief	List of c scrolls. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CScrollList :
			public SallyAPI::GUI::CForm
		{
		private:
			std::vector<SallyAPI::GUI::CScrollListItem>	m_vItems;
			int		m_iCols;
			int		m_iRows;
			int		m_iBorderTop;
			int		m_iIconSize;
			int		m_iIconSizeSpace;
			int		m_iStartPicture;

			std::map<int, std::vector<SallyAPI::GUI::CButton*>>		m_mImageBox;
			std::map<int, std::vector<SallyAPI::GUI::CLabelBox*>>		m_mImageName;

			void	UpdateAlpha();
			void	ResetImages();
			void	UpdateImages();

			void	OnCommandProcessClicked(int reporterId);

			virtual void OnCommandMouseMove(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			virtual void AddChild(SallyAPI::GUI::CControl* control);
		public:
			CScrollList(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int iconSize, int iconSizeSpace, int controlId = 0);
			virtual ~CScrollList();

			virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void	SetAlphaBlending(int alphaBlending);

			void AddItem(const SallyAPI::GUI::CScrollListItem& item);
			void Clear();

			void SetFont(const std::string& font);
		};
	}
}