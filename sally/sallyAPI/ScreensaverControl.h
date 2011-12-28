////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScreensaverControl.h
///
/// \brief	Declares the scrennsaver control class. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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
#include "Control.h"
#include "GUIHelper.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CScreensaverControl
		///
		/// \brief	Scrennsaver control. 
		///
		/// \author	Christian Knobloch
		/// \date	27.12.2011
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CScreensaverControl
			: public SallyAPI::GUI::CControl
		{
		protected:
			int			m_iImage;

			virtual void	RenderControl();
			virtual bool	ProcessMouseDoubleClick(int x, int y);
			virtual bool	ProcessMouseUp(int x, int y);
		public:
			CScreensaverControl(SallyAPI::GUI::CGUIBaseObject* parent, int controlId = 0);
			virtual ~CScreensaverControl();

			void						SetImageId(int image);
			int							GetImageId();

		};
	}
}