////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Edit.h
///
/// \brief	Declares the edit class. 
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
		/// \enum	EditCharacterInputType
		///
		/// \brief	Values that represent EditCharacterInputType. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum EditCharacterInputType {EDIT_CHARACTER_INPUT_TYPE_NONE, EDIT_CHARACTER_INPUT_TYPE_LOWER, EDIT_CHARACTER_INPUT_TYPE_UPPER};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CEdit
		///
		/// \brief	Edit. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CEdit 
			: public SallyAPI::GUI::CControl
		{
		protected:
			int			m_iImage;
			int			m_iImageSize;
			bool		m_bImageLeft;
			bool		m_bNumberOnly;
			bool		m_bPassword;
			bool		m_bKeyboardInputEnabled;
			int			m_iMaxCharacterInputLength;
			EditCharacterInputType	m_eCharacterInputType;
			std::string	m_strInfoText;

			virtual void	RenderControl();
			virtual bool	ProcessMouseUp(int x, int y);
		public:
			CEdit(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId = 0);
			virtual ~CEdit();

			virtual void	SetText(const std::string& text);

			void		SetInfoText(const std::string& text);
			std::string	GetInfoText();

			void	SetImageLeft();
			void	SetImageRight();
			void	SetImageId(int image);
			void	SetNumberOnly(bool value);
			bool	GetNumberOnly();
			void	SetPassword(bool value);
			bool	GetPassword();
			void	SetKeyboardInputEnabled(bool value);
			bool	GetKeyboardInputEnabled();
			void	SetMaxCharacterInputLength(int value);
			int		GetMaxCharacterInputLength();
			void	SetCharacterInputType(EditCharacterInputType value);
			EditCharacterInputType	GetCharacterInputType();
		};
	}
}
