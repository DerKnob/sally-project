////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ListViewButton.h
///
/// \brief	Declares the list view button class. 
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
#include "Button.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CListViewButton
		///
		/// \brief	List view button. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CListViewButton :
			public SallyAPI::GUI::CButton
		{
		private:
			bool	m_bFirst;
			bool	m_bLast;
			int		m_iNumber;
			bool	m_bSmallFont;

			bool	m_bTimeMouseClickReset;

			virtual bool ProcessMouseDoubleClick(int x, int y);
			virtual void RenderControl();
		public:
			CListViewButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId);
			virtual ~CListViewButton();

			virtual bool CheckProcessMouseUp(int x, int y);

			void	SetNumber(int number);
			int		GetNumber();

			void	SetFirst(bool first);
			void	SetLast(bool last);

			void	SetSmallFont(bool value);

			void	ResetFlicker();
		};
	}
}