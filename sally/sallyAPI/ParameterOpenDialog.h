////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterOpenDialog.h
///
/// \brief	Declares the parameter open Dialog. 
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
#include "GUIBaseObject.h"

namespace SallyAPI
{
	namespace GUI
	{
		namespace SendMessage
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \class	CParameterOpenDialog
			///
			/// \brief	Dialog for setting the send message parameter open. 
			///
			/// \author	Christian Knobloch
			/// \date	19.04.2010
			////////////////////////////////////////////////////////////////////////////////////////////////////

			class DLL_API_SALLY CParameterOpenDialog
				: public SallyAPI::GUI::SendMessage::CParameterBase
			{
			private:
				int								m_iActionCommand;
				std::string						m_strActionText;
				int								m_iActionImage;
				int								m_iControlId;
				SallyAPI::GUI::CGUIBaseObject*	m_pNotificationWindow;
				std::string						m_strDialogText;
				std::vector<std::string>		m_vStartFolders;
				bool							m_bShowRemovableDisk;
				bool							m_bShowSpecialFolders;
				bool							m_bShowSubfolders;
				std::map<std::string, int>		m_mMimetypeList;
				std::string						m_strGoToFolder;
			public:
				CParameterOpenDialog(int controlId, SallyAPI::GUI::CGUIBaseObject* notificationWindow, const std::string& dialogText);
				virtual ~CParameterOpenDialog();

				SallyAPI::GUI::CGUIBaseObject*	GetNotificationWindow();
				std::string						GetDialogText();
				int								GetControlID();

				void	SetStartFolders(std::vector<std::string>& startFolders);
				void	SetShowRemovableDisk(bool showRemovableDisk);
				void	SetMimetypeList(std::map<std::string, int>& mimetypeList);
				void	SetShowSubfolders(bool showSubfolders);
				void	SetShowSpecialFolders(bool showSpecialFolders);
				void	SetAction(int command, const std::string& text, int image);
				void	SetGoToFolder(const std::string& goToFolder);

				std::vector<std::string>	GetStartFolders();
				bool						GetShowRemovableDisk();
				std::map<std::string, int>	GetMimetypeList();
				bool						GetShowSubfolders();
				bool						GetShowSpecialFolders();
				std::string					GetGoToFolder();

				std::string	GetActionText();
				int			GetActionImage();
				int			GetActionCommand();
			};
		}
	}
}