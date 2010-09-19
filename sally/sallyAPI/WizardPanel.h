////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\WizardPanel.h
///
/// \brief	Declares the wizard panel class. 
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
#include "Define.h"
#include "AppBase.h"
#include "GroupBox.h"
#include "ImageBox.h"
#include "Label.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CWizardPanel
		///
		/// \brief	Panel for editing the c wizard. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CWizardPanel :
			public SallyAPI::GUI::CAppBase
		{
		protected:
			SallyAPI::GUI::CLabel*		m_pLabelHeader;
			SallyAPI::GUI::CGroupBox*	m_pBackgroundGroup;
			SallyAPI::GUI::CImageBox*	m_pImageLogo;
		public:
			CWizardPanel(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
			virtual ~CWizardPanel();

			virtual void LoadConfig();
			virtual void SaveConfig();

			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

			void	SetParent(SallyAPI::GUI::CGUIBaseObject* parent);
			bool	IsFirstStart();
		};
	}
}