////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ImageBox.h
///
/// \brief	Declares the image box class. 
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
#include "GUIHelper.h"
#include "ParameterHoldClick.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	ImageBoxDisplayType
		///
		/// \brief	Values that represent ImageBoxDisplayType. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum ImageBoxDisplayType {IMAGEBOX_DISPLAY_TYPE_STRETCH, IMAGEBOX_DISPLAY_TYPE_SCALE, IMAGEBOX_DISPLAY_TYPE_CENTER};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CImageBox
		///
		/// \brief	Image box. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CImageBox :
			public SallyAPI::GUI::CControl
		{
		protected:
			int							m_iImage;
			SallyAPI::GUI::CPicture*	m_pPicture;
			ImageBoxDisplayType			m_eDiyplayType;

			virtual void	RenderControl();
			virtual bool	ProcessMouseDoubleClick(int x, int y);
			virtual bool	ProcessMouseUp(int x, int y);
			virtual bool	ProcessMouseMove(int x, int y);
		public:
			CImageBox(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId = 0);
			virtual ~CImageBox();

			virtual void Timer(float timeDelta);

			void						SetImageId(int image);
			int							GetImageId();

			void						SetPicture(SallyAPI::GUI::CPicture* picture);
			SallyAPI::GUI::CPicture*	GetPicture();

			void						SetDiyplayType(ImageBoxDisplayType value);
			ImageBoxDisplayType			GetDiyplayType();
		};
	}
}