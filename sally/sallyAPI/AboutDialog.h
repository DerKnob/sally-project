////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\AboutDialog.h
///
/// \brief	Declares the about Dialog. 
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
#include "PopUpWindow.h"
#include "GroupBox.h"
#include "ImageBox.h"
#include "Label.h"
#include "EditBox.h"
#include "ApplicationWindow.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CAboutDialog
		///
		/// \brief	About Dialog Box. 
		///
		/// \author	Christian Knobloch
		/// \date	24.05.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CAboutDialog :
			public SallyAPI::GUI::CPopUpWindow
		{
		private:
			SallyAPI::GUI::CGroupBox*			m_pBackgroundGroup;
			SallyAPI::GUI::CImageBox*			m_pImage;
			SallyAPI::GUI::CLabel*				m_pTitle;
			SallyAPI::GUI::CLabel*				m_pVersion;
			SallyAPI::GUI::CLabel*				m_pAuthor;
			SallyAPI::GUI::CEditBox*			m_pAbout;
			SallyAPI::GUI::CApplicationWindow*	m_pApplication;
		public:
			CAboutDialog(SallyAPI::GUI::CApplicationWindow* parent, int graphicId, const std::string& explicidAppName, const std::string& aboutText);
			virtual ~CAboutDialog();
		};
	}
}
