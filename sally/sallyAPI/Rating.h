////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Rating.h
///
/// \brief	Declares the rating class. 
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
#include "Form.h"
#include "ImageBox.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CRating
		///
		/// \brief	Rating. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CRating :
			public SallyAPI::GUI::CForm
		{
		private:
			int							m_iRating;
			int							m_iRatingTemp;
			SallyAPI::GUI::CImageBox*	m_pImages[5];

			void			UpdateRating();

			virtual bool	ProcessMouseDown(int x, int y);
			virtual bool	ProcessMouseUp(int x, int y);
			virtual bool	ProcessMouseMove(int x, int y);
		public:
			CRating(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int controlId = 0);
			virtual ~CRating();
			
			virtual void	SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			int				GetRating();
			void			SetRating(int rating);
		};
	}
}
