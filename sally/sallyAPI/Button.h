////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Button.h
///
/// \brief	Declares the button class. 
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

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	ButtonType
		///
		/// \brief	Values that represent ButtonType. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum ButtonType {BUTTON_TYPE_NORMAL, BUTTON_TYPE_ONLY_IMAGE, BUTTON_TYPE_FOUR_TEXTS};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CButton
		///
		/// \brief	Button. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CButton :
			public SallyAPI::GUI::CControl
		{
		protected:
			int			m_iImage;
			ButtonType	m_eType;

			//! Not valide for BUTTON_TYPE_ONLY_IMAGE
			int			m_iImageSize;
			//! Not valide for BUTTON_TYPE_ONLY_IMAGE
			bool		m_bImageLeft;
			//! Not valide for BUTTON_TYPE_ONLY_IMAGE
			bool		m_bChecked;
			//! Not valide for BUTTON_TYPE_ONLY_IMAGE
			bool		m_bActive;
			//! Not valide for BUTTON_TYPE_ONLY_IMAGE
			bool		m_bDefaultButton;
			float		m_fDeltaStart;
			//! Only valide for BUTTON_TYPE_NORMAL
			bool		m_bUseHoleWidth;
			//! Only valide for BUTTON_TYPE_FOUR_TEXTS
			std::string	m_strFourTexts[4];
			//! Only valide for BUTTON_TYPE_FOUR_TEXTS
			int			m_iFourTextsActive;


			void			DrawFourTextItem(RECT r, const std::string& text, bool active, int itemNumber = -1, bool pressed = false);

			virtual void	RenderControl();

			virtual bool	ProcessMouseUp(int x, int y);
			virtual bool	ProcessMouseMove(int x, int y);
		public:
			CButton(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId = 0,
				ButtonType type = BUTTON_TYPE_NORMAL);
			virtual ~CButton();

			bool	GetCheckStatus();
			void	SetCheckStatus(bool value);

			void	SetActive(bool set);
			bool	GetActive();

			void	SetDefaultButton(bool value);
			bool	GetDefaultButton();

			void	SetImageLeft();
			void	SetImageRight();
			void	SetImageId(int image);

			bool	IsUseHoleWidth();
			void	UseHoleWidth(bool use);

			void		SetFourTexts(int i, const std::string& text);
			std::string	GetFourTexts(int i);

			void		SetFourTextActive(int i);
		};
	}
}