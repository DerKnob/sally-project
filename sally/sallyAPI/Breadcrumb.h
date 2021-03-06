////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Breadcrumb.h
///
/// \brief	Declares the breadcrumb class. 
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
		/// \class	CBreadcrumb
		///
		/// \brief	Breadcrumb. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CBreadcrumb :
			public SallyAPI::GUI::CControl
		{
		private:
			int				m_iImage;
			int				m_iImageSize;

			virtual void	RenderControl();
			virtual bool	ProcessMouseUp(int x, int y);
		public:
			CBreadcrumb(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId = 0);
			virtual ~CBreadcrumb();

			void	SetImageId(int image);
		};
	}
}
