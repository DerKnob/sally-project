////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FileBrowser.h
///
/// \brief	Declares the file browser class. 
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

#include <algorithm>
#include <shlobj.h>
#include "Form.h"
#include "ListView.h"
#include "ButtonBar.h"
#include "Breadcrumb.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	CHAR_SELECTOR_COUNT
		///
		/// \brief	Values that represent CHAR_SELECTOR_COUNT. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum CHAR_SELECTOR_COUNT { CHAR_SELECTOR_COUNT_28, CHAR_SELECTOR_COUNT_15, CHAR_SELECTOR_COUNT_OFF};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CFileBrowser
		///
		/// \brief	File browser. 
		///
		/// \author	Christian Knobloch
		/// \date	13.09.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CFileBrowser :
			public SallyAPI::GUI::CForm
		{
		protected:
			static std::string		m_strMyDocument;
			static std::string		m_strMyMusic;
			static std::string		m_strMyVideos;
			static std::string		m_strMyPictures;
			static std::string		m_strMyDesktop;

			SallyAPI::GUI::CButton*				m_pButtonAction;
			SallyAPI::GUI::CBreadcrumb*			m_pBreadcrumb;
			SallyAPI::GUI::CButtonBarButton*	m_pButtonRefreshView;
			SallyAPI::GUI::CButtonBarButton*	m_pButtonGoUp;
			SallyAPI::GUI::CButtonBar*			m_pMenu;
			SallyAPI::GUI::CListView*			m_pListViewFileWalker;
			SallyAPI::GUI::CButton*				m_pCharSelector[28];
			int							m_iFolderDepth;
			std::string					m_strCurrentFolderName;
			std::vector<std::string>	m_vStartFolders;
			bool						m_bShowRemovableDisk;
			std::map<std::string, int>	m_mMimetypeList;
			bool						m_bShowSubfolders;
			bool						m_bShowHardDisks;
			bool						m_bShowSpecialFolders;
			bool						m_bShowUnkonwFiles;
			int							m_iActionCommand;
			std::map<int, int>			m_mListViewPages;
			CHAR_SELECTOR_COUNT			m_eCharSelectorCount;
			char						m_cLastCharSelected;
			bool						m_bFolderOpend;

			void	OnCommandItemClicked(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	OnCommandActionClicked(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			bool	OnCommandOpenFolder(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	OnCommandOpenFolder(std::string& folder);
			void	OnCommandReset();
			void	OnCommandCharSelector(SallyAPI::GUI::CGUIBaseObject* reporter);

			void	SetPictureList(std::vector<int>& pictureList);

			void	FilewalkerAddFolder(std::string& folder, std::vector<std::string>& folders, std::vector<std::string>& files);
		public:
			CFileBrowser(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId = 0);
			virtual ~CFileBrowser();

			void			SetStartFolders(std::vector<std::string>& startFolders);
			void			SetShowSpecialFolders(bool showSpecialFolders);
			void			SetShowRemovableDisk(bool showRemovableDisk);
			void			SetShowHardDisks(bool showHardDisks);
			void			SetShowUnkownFiles(bool showUnkonwFiles);
			void			SetMimetypeList(std::map<std::string, int>& mimetypeList);
			void			SetShowSubfolders(bool showSubfolders);
			void			SetActionImageId(int icon);
			void			SetActionButtonCommand(int command);
			void			SetActionButtonText(const std::string& text);
			void			SetActionButtonImage(int icon);
			void			SetFolder(std::string& folder, int folderDepth = -1);

			void			UpdateView();
			void			Reset();

			SallyAPI::GUI::CListView*	GetListView();
			
			virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void	SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

			std::string		GetCurrentFolder();
			int				GetCurrentFolderDepth();
		};
	}
}
