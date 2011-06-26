////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FileBrowser.cpp
///
/// \brief	Implements the file browser class. 
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

#include "FileBrowser.h"

using namespace SallyAPI::GUI;

#define GUI_REFRESH_FOLDER		50000
#define GUI_GO_FOLDER_UP		50001
#define GUI_SORT_BY_NAME		50002
#define GUI_SORT_BY_DATE		50003
#define GUI_ACTION_BUTTON		50004
#define GUI_CHAR_SELECTOR		51000

std::string		SallyAPI::GUI::CFileBrowser::m_strMyDocument;
std::string		SallyAPI::GUI::CFileBrowser::m_strMyMusic;
std::string		SallyAPI::GUI::CFileBrowser::m_strMyVideos;
std::string		SallyAPI::GUI::CFileBrowser::m_strMyPictures;
std::string		SallyAPI::GUI::CFileBrowser::m_strMyDesktop;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFileBrowser::CFileBrowser(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height, controlId),
/// m_iFolderDepth(0), m_bShowRemovableDisk(true), m_bShowSubfolders(true), m_iActionCommand(0),
/// m_bShowHardDisks(false), m_cLastCharSelected(' '), m_bFolderOpend(false),
/// m_bShowUnkonwFiles(false), m_bShowSpecialFolders(false)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	18.05.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CFileBrowser::CFileBrowser(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId), m_iFolderDepth(0), m_bShowRemovableDisk(true),
	m_bShowSubfolders(true), m_iActionCommand(0), m_bShowHardDisks(false), m_cLastCharSelected(' '), m_bFolderOpend(false),
	m_bShowUnkonwFiles(false), m_bShowSpecialFolders(false), m_iActionImage(GUI_THEME_SALLY_ICON_ADD)
{
	std::map<int, int> columns;

	columns[0] = 30;
	columns[1] = 0;
	columns[2] = 120;

	int cols = ((height - ((CONTROL_HEIGHT + 10) * 2)) / LISTVIEW_ITEM_HEIGHT);
	int fileWalkerCorrection = 0;
	int xCorrection = 0;

	// char selector count
	if (cols >= 28)
	{
		m_eCharSelectorCount = CHAR_SELECTOR_COUNT_28;
		fileWalkerCorrection = 35;

		xCorrection = cols - 28;
		xCorrection = xCorrection * CONTROL_HEIGHT / 2;
	}
	else if (cols >= 15)
	{
		m_eCharSelectorCount = CHAR_SELECTOR_COUNT_15;
		fileWalkerCorrection = 45;

		xCorrection = cols - 15;
		xCorrection = xCorrection * CONTROL_HEIGHT / 2;
	}
	else
		m_eCharSelectorCount = CHAR_SELECTOR_COUNT_OFF;

	m_pListViewFileWalker = new SallyAPI::GUI::CListViewExt(this, fileWalkerCorrection, CONTROL_HEIGHT + 10,
		width - fileWalkerCorrection, height - ((CONTROL_HEIGHT + 10) * 2), 3, columns);
	m_pListViewFileWalker->SetLocalised(false);
	this->AddChild(m_pListViewFileWalker);

	// menu
	m_pMenu = new SallyAPI::GUI::CButtonBar(this, 0, 0, 60);
	this->AddChild(m_pMenu);

	m_pButtonGoUp = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 30, GUI_GO_FOLDER_UP);
	m_pButtonGoUp->SetImageId(GUI_THEME_SALLY_ICON_FOLDER_UP);
	m_pMenu->AddChild(m_pButtonGoUp);

	m_pButtonRefreshView = new SallyAPI::GUI::CButtonBarButton(m_pMenu, 30, GUI_REFRESH_FOLDER);
	m_pButtonRefreshView->SetImageId(GUI_THEME_SALLY_ICON_RELOAD);
	m_pMenu->AddChild(m_pButtonRefreshView);

	m_pButtonAction = new SallyAPI::GUI::CButton(this, m_iWidth - 280, m_iHeight - CONTROL_HEIGHT, 280, CONTROL_HEIGHT, GUI_ACTION_BUTTON, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonAction->Visible(false);
	this->AddChild(m_pButtonAction);

	// breadcrumb
	m_pBreadcrumb = new SallyAPI::GUI::CBreadcrumb(this, 60 + 10, 0, width - 60 - 10 - 60 - 10);
	m_pBreadcrumb->SetLocalised(false);
	m_pBreadcrumb->SetImageId(GUI_THEME_SALLY_ICON_DESKTOP);
	this->AddChild(m_pBreadcrumb);

	// sorting
	m_pMenuSort = new SallyAPI::GUI::CButtonBar(this, width - 60, 0, 60);
	this->AddChild(m_pMenuSort);

	m_pMenuSortName = new SallyAPI::GUI::CButtonBarButton(m_pMenuSort, 30, GUI_SORT_BY_NAME);
	m_pMenuSortName->SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_TEXT);
	m_pMenuSortName->SetCheckStatus(true);
	m_pMenuSort->AddChild(m_pMenuSortName);

	m_pMenuSortDate = new SallyAPI::GUI::CButtonBarButton(m_pMenuSort, 30, GUI_SORT_BY_DATE);
	m_pMenuSortDate->SetImageId(GUI_THEME_SALLY_ICON_DATE);
	m_pMenuSort->AddChild(m_pMenuSortDate);

	//
	if (m_eCharSelectorCount == CHAR_SELECTOR_COUNT_28)
	{
		m_pCharSelector[0] = new SallyAPI::GUI::CButton(this, 0, CONTROL_HEIGHT + 10 + xCorrection,
			CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_CHAR_SELECTOR);
		m_pCharSelector[0]->UseHoleWidth(true);
		m_pCharSelector[0]->SetAlign(DT_CENTER  | DT_VCENTER);
		m_pCharSelector[0]->SetLocalised(false);
		m_pCharSelector[0]->SetText("*");
		this->AddChild(m_pCharSelector[0]);

		m_pCharSelector[1] = new SallyAPI::GUI::CButton(this, 0, CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + xCorrection,
			CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_CHAR_SELECTOR);
		m_pCharSelector[1]->UseHoleWidth(true);
		m_pCharSelector[1]->SetAlign(DT_CENTER  | DT_VCENTER);
		m_pCharSelector[1]->SetLocalised(false);
		m_pCharSelector[1]->SetText("0-9");
		this->AddChild(m_pCharSelector[1]);

		for (int i = 2; i < 28; ++i)
		{
			char c[2];
			c[0] = 65 + i - 2;
			c[1] = '\0';

			m_pCharSelector[i] = new SallyAPI::GUI::CButton(this, 0, CONTROL_HEIGHT + 10 + (CONTROL_HEIGHT * i) + xCorrection,
				CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_CHAR_SELECTOR);
			m_pCharSelector[i]->UseHoleWidth(true);
			m_pCharSelector[i]->SetAlign(DT_CENTER  | DT_VCENTER);
			m_pCharSelector[i]->SetLocalised(false);
			m_pCharSelector[i]->SetText(c);
			this->AddChild(m_pCharSelector[i]);
		}
	}
	else if (m_eCharSelectorCount == CHAR_SELECTOR_COUNT_15)
	{
		m_pCharSelector[0] = new SallyAPI::GUI::CButton(this, 0, CONTROL_HEIGHT + 10 + xCorrection,
			CONTROL_HEIGHT + 10, CONTROL_HEIGHT, GUI_CHAR_SELECTOR);
		m_pCharSelector[0]->UseHoleWidth(true);
		m_pCharSelector[0]->SetAlign(DT_CENTER  | DT_VCENTER);
		m_pCharSelector[0]->SetLocalised(false);
		m_pCharSelector[0]->SetText("*");
		this->AddChild(m_pCharSelector[0]);

		m_pCharSelector[1] = new SallyAPI::GUI::CButton(this, 0, CONTROL_HEIGHT + 10 + CONTROL_HEIGHT + xCorrection,
			CONTROL_HEIGHT + 10, CONTROL_HEIGHT, GUI_CHAR_SELECTOR);
		m_pCharSelector[1]->UseHoleWidth(true);
		m_pCharSelector[1]->SetAlign(DT_CENTER  | DT_VCENTER);
		m_pCharSelector[1]->SetLocalised(false);
		m_pCharSelector[1]->SetText("0-9");
		this->AddChild(m_pCharSelector[1]);

		for (int i = 2; i < 15; ++i)
		{
			char c[4];
			c[0] = 65 + (i * 2) - 4;
			c[1] = ' ';
			c[2] = 65 + (i * 2) - 3;
			c[3] = '\0';

			m_pCharSelector[i] = new SallyAPI::GUI::CButton(this, 0, CONTROL_HEIGHT + 10 + (CONTROL_HEIGHT * i) + xCorrection,
				CONTROL_HEIGHT + 10, CONTROL_HEIGHT, GUI_CHAR_SELECTOR);
			m_pCharSelector[i]->UseHoleWidth(true);
			m_pCharSelector[i]->SetAlign(DT_CENTER  | DT_VCENTER);
			m_pCharSelector[i]->SetLocalised(false);
			m_pCharSelector[i]->SetText(c);
			this->AddChild(m_pCharSelector[i]);
		}
	}

	// init the windows folders
	if (m_strMyDocument.length() == 0)
	{
		// My Documents
		char szPath[MAX_PATH];
		LPITEMIDLIST lpStartFolder = NULL;
		SHGetSpecialFolderLocation(0, CSIDL_MYDOCUMENTS, &lpStartFolder);
		SHGetPathFromIDList(lpStartFolder, szPath);
		m_strMyDocument = szPath;

		// CleanUp
		CoTaskMemFree(lpStartFolder);
		lpStartFolder = NULL;

		// My Music
		SHGetSpecialFolderLocation(0, CSIDL_MYMUSIC, &lpStartFolder);
		SHGetPathFromIDList(lpStartFolder, szPath);
		m_strMyMusic = szPath;

		// CleanUp
		CoTaskMemFree(lpStartFolder);
		lpStartFolder = NULL;

		// My Video
		SHGetSpecialFolderLocation(0, CSIDL_MYVIDEO, &lpStartFolder);
		SHGetPathFromIDList(lpStartFolder, szPath);
		m_strMyVideos = szPath;

		// CleanUp
		CoTaskMemFree(lpStartFolder);
		lpStartFolder = NULL;

		// My Pictures
		SHGetSpecialFolderLocation(0, CSIDL_MYPICTURES, &lpStartFolder);
		SHGetPathFromIDList(lpStartFolder, szPath);
		m_strMyPictures = szPath;

		// CleanUp
		CoTaskMemFree(lpStartFolder);
		lpStartFolder = NULL;

		// Desktop
		SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &lpStartFolder);
		SHGetPathFromIDList(lpStartFolder, szPath);
		m_strMyDesktop = szPath;

		// CleanUp
		CoTaskMemFree(lpStartFolder);
		lpStartFolder = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFileBrowser::~CFileBrowser()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFileBrowser::~CFileBrowser()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFileBrowser::GetCurrentFolder()
///
/// \brief	Gets the current folder. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \return	The current folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFileBrowser::GetCurrentFolder()
{
	return m_strCurrentFolderName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetStartFolders(std::vector<std::string>& startFolders)
///
/// \brief	Sets a start folders. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	startFolders	The start folders. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetStartFolders(std::vector<std::string>& startFolders)
{
	m_vStartFolders = startFolders;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetShowSpecialFolders(bool showSpecialFolders)
///
/// \brief	Sets a show special folders. Special folders are the users pictures path or the users
/// desktop.
///
/// \author	Christian Knobloch
/// \date	18.05.2011
///
/// \param	showSpecialFolders	true to show, false to hide the special folders. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetShowSpecialFolders(bool showSpecialFolders)
{
	m_bShowSpecialFolders = showSpecialFolders;
	if ((!m_bShowSubfolders) && (!m_bShowRemovableDisk) && (!m_bShowHardDisks) && (!m_bShowSpecialFolders))
	{
		m_pBreadcrumb->Visible(false);
		m_pMenu->Visible(false);
	}
	else
	{
		m_pBreadcrumb->Visible(true);
		m_pMenu->Visible(true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetShowRemovableDisk(bool showRemovableDisk)
///
/// \brief	Sets if removable disks should be shown. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param	showRemovableDisk	true to show, false to hide the removable disk. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetShowRemovableDisk(bool showRemovableDisk)
{
	m_bShowRemovableDisk = showRemovableDisk;
	if ((!m_bShowSubfolders) && (!m_bShowRemovableDisk) && (!m_bShowHardDisks) && (!m_bShowSpecialFolders))
	{
		m_pBreadcrumb->Visible(false);
		m_pMenu->Visible(false);
	}
	else
	{
		m_pBreadcrumb->Visible(true);
		m_pMenu->Visible(true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetShowUnkownFiles(bool showUnkonwFiles)
///
/// \brief	Sets a show unkown files. 
///
/// \author	Christian Knobloch
/// \date	29.03.2011
///
/// \param	showUnkonwFiles	true to show, false to hide the unkonw files. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetShowUnkownFiles(bool showUnkonwFiles)
{
	m_bShowUnkonwFiles = showUnkonwFiles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetShowHardDisks(bool showHardDisks)
///
/// \brief	Sets if hard disks should be shown. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param	showHardDisks	true to show, false to hide the hard disks. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetShowHardDisks(bool showHardDisks)
{
	m_bShowHardDisks = showHardDisks;
	if ((!m_bShowSubfolders) && (!m_bShowRemovableDisk) && (!m_bShowHardDisks) && (!m_bShowSpecialFolders))
	{
		m_pBreadcrumb->Visible(false);
		m_pMenu->Visible(false);
	}
	else
	{
		m_pBreadcrumb->Visible(true);
		m_pMenu->Visible(true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetMimetypeList(std::map<std::string, int>& mimetypeList)
///
/// \brief	Sets a mimetype list. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	mimetypeList	List of mimetypes. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetMimetypeList(std::map<std::string, int>& mimetypeList)
{
	std::vector<int> pictureList;

	std::map<std::string, int>::iterator iter = mimetypeList.begin();

	while (iter != mimetypeList.end())
	{
		int imageID = iter->second;
		pictureList.push_back(imageID);
		++iter;
	}

	m_mMimetypeList = mimetypeList;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetActionImageId(int icon)
///
/// \brief	Sets an action image id. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param	icon	The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetActionImageId(int icon)
{
	m_iActionImage = icon;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetShowSubfolders(bool showSubfolders)
///
/// \brief	Sets a show subfolders. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param	showSubfolders	true to show, false to hide the subfolders. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetShowSubfolders(bool showSubfolders)
{
	m_bShowSubfolders = showSubfolders;
	if ((!m_bShowSubfolders) && (!m_bShowRemovableDisk) && (!m_bShowHardDisks) && (!m_bShowSpecialFolders))
	{
		m_pBreadcrumb->Visible(false);
		m_pMenu->Visible(false);
	}
	else
	{
		m_pBreadcrumb->Visible(true);
		m_pMenu->Visible(true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CListViewExt* CFileBrowser::GetListView()
///
/// \brief	Gets the list view. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \return	null if it fails, else the list view. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CListViewExt* CFileBrowser::GetListView()
{
	return m_pListViewFileWalker;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::UpdateView()
///
/// \brief	Updates the view. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::UpdateView()
{
	if (m_iFolderDepth == 0)
		OnCommandReset();
	else
		OnCommandOpenFolder(m_strCurrentFolderName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::Reset()
///
/// \brief	Resets this object. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::Reset()
{
	OnCommandReset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_SYSTEM_DEVICECHANGE:
		if (m_iFolderDepth == 0)
			OnCommandReset();		
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BREADCRUMB_START_CLICKED:
		Reset();
		return;
	case GUI_LISTVIEW_ITEM_DOUBLECLICKED:
		if (!m_bFolderOpend)
			m_pParent->SendMessageToParent(this, reporterId, messageId, messageParameter);
		return;
	case GUI_LISTVIEW_ITEM_CLICKED:
		OnCommandListviewItemClicked(reporter, reporterId, messageId, messageParameter);
		break;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_CHAR_SELECTOR:
			OnCommandCharSelector(reporter);
			return;
		}
		break;
	}

	switch (reporterId)
	{
	case GUI_ACTION_BUTTON:
		m_pParent->SendMessageToParent(reporter, m_iActionCommand, messageId, messageParameter);
		return;
	case GUI_REFRESH_FOLDER:
		if (m_iFolderDepth > 0)
			OnCommandOpenFolder(m_strCurrentFolderName);
		else
			OnCommandReset();
		return;
	case GUI_GO_FOLDER_UP:
		if (m_iFolderDepth > 1)
		{
			m_strCurrentFolderName = SallyAPI::String::PathHelper::GetUpperDirectory(m_strCurrentFolderName);
			OnCommandOpenFolder(m_strCurrentFolderName);
			--m_iFolderDepth;
			m_pListViewFileWalker->SetStartItem(m_mListViewPages[m_iFolderDepth]);
		}
		else
			OnCommandReset();
		return;
	case GUI_SORT_BY_NAME:
	case GUI_SORT_BY_DATE:
		OnCommandChangeSorting(reporterId);
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::OnCommandListviewItemClicked(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command listview item clicked action. 
///
/// \author	Christian Knobloch
/// \date	26.06.2011
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::OnCommandListviewItemClicked(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterListItem* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterListItem*> (messageParameter);

	if (parameter == NULL)
		return;

	if (parameter->GetButton() == 0)
	{
		OnCommandActionClicked(reporter, reporterId, GUI_LISTVIEW_ITEM_ACTION_CLICKED, messageParameter);
		return;
	}

	if (OnCommandOpenFolder(messageParameter))
	{
		m_pButtonGoUp->Enable(true);
		m_pButtonAction->Enable(true);
		m_pMenuSortName->Enable(true);
		m_pMenuSortDate->Enable(true);
		++m_iFolderDepth;
		return;
	}
	else
	{
		OnCommandItemClicked(reporter, reporterId, messageId, messageParameter);
		return;
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::OnCommandChangeSorting(int reporterId)
///
/// \brief	Executes the command change sorting action. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
///
/// \param	reporterId	Identifier for the reporter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::OnCommandChangeSorting(int reporterId)
{
	if (reporterId == GUI_SORT_BY_NAME)
	{
		m_pMenuSortDate->SetCheckStatus(false);
		m_pMenuSortName->SetCheckStatus(true);

		int count = 28;
		if (m_eCharSelectorCount == CHAR_SELECTOR_COUNT_15)
			count = 15;
		else if (m_eCharSelectorCount == CHAR_SELECTOR_COUNT_OFF)
			count = 0;

		for (int i = 0; i < count; i++)
		{
			m_pCharSelector[i]->Visible(true);
		}
	}
	else
	{
		m_pMenuSortDate->SetCheckStatus(true);
		m_pMenuSortName->SetCheckStatus(false);

		int count = 28;
		if (m_eCharSelectorCount == CHAR_SELECTOR_COUNT_15)
			count = 15;
		else if (m_eCharSelectorCount == CHAR_SELECTOR_COUNT_OFF)
			count = 0;

		for (int i = 0; i < count; i++)
		{
			m_pCharSelector[i]->Visible(false);
		}
	}

	SortFiles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::OnCommandCharSelector(SallyAPI::GUI::CGUIBaseObject* reporter)
///
/// \brief	Executes the command char selector action. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	reporter	If non-null, the reporter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::OnCommandCharSelector(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	std::string temp = reporter->GetText();
	if (temp.length() <= 0)
		return;

	char searchForChar = temp[0];

	// go to the top
	if (searchForChar == '*')
	{
		m_cLastCharSelected = ' ';
		m_pListViewFileWalker->SetStartItem(0);
		return;
	}

	if ((searchForChar == m_cLastCharSelected) && (m_eCharSelectorCount == CHAR_SELECTOR_COUNT_15))
	{
		searchForChar = temp[2];
	}
	m_cLastCharSelected = searchForChar;

	int newPosition = 0;

	std::vector<SallyAPI::GUI::CListViewItem*>* listItems = m_pListViewFileWalker->GetListItems();

	std::vector<SallyAPI::GUI::CListViewItem*>::iterator iter = listItems->begin();
	while (iter != listItems->end())
	{
		SallyAPI::GUI::CListViewItem* item = *iter;

		temp = item->GetText(1);
		if (temp.length() > 0)
		{
			temp = SallyAPI::String::StringHelper::StringToUpper(temp);

			char c = temp[0];
			if (searchForChar == c)
			{
				// found!!!!
				m_pListViewFileWalker->SetStartItem(newPosition);
				return;
			}
			if (((c > searchForChar) && ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)))
				|| (((searchForChar == 48 && searchForChar == 57)) && ((c >= 32 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126))))
			{
				// ok, we already at the next char
				m_pListViewFileWalker->SetStartItem(newPosition);
				return;
			}
		}

		++newPosition;
		++iter;
	}
	// not found... go to the last one
	m_pListViewFileWalker->SetStartItem(newPosition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::OnCommandItemClicked(SallyAPI::GUI::CGUIBaseObject *reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command item clicked action. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::OnCommandItemClicked(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	m_pParent->SendMessageToParent(this, m_iControlId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::OnCommandActionClicked(SallyAPI::GUI::CGUIBaseObject *reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command action clicked action. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::OnCommandActionClicked(SallyAPI::GUI::CGUIBaseObject *reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	m_pParent->SendMessageToParent(this, m_iControlId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFileBrowser::OnCommandOpenFolder(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command open folder action. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFileBrowser::OnCommandOpenFolder(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterListItem* parameter = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterListItem*> (messageParameter);

	if (parameter == NULL)
		return true;

	SallyAPI::GUI::CListViewItem* listItem = m_pListViewFileWalker->GetItem(parameter->GetItem());

	// if it is a folder
	if (SallyAPI::File::FileHelper::IsDirectory(listItem->GetIdentifier()))
	{
		std::string folder = listItem->GetIdentifier();

		m_strCurrentFolderName = folder;

		m_mListViewPages[m_iFolderDepth] = m_pListViewFileWalker->GetStartItem();

		OnCommandOpenFolder(folder);

		m_bFolderOpend = true;
		return true;
	}

	m_bFolderOpend = false;

	// send to parent window if it is no folder
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::FilewalkerAddFolder(std::string& folder)
///
/// \brief	Filewalker add folder. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	folder	Pathname of the folder. 
/// \param [in,out]	folders	The folders. 
/// \param [in,out]	files	The files. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::FilewalkerAddFolder(std::string& folder)
{
	m_vFolders.clear();
	m_vFiles.clear();
	m_vFoldersFilesDate.clear();

	HANDLE				hFile;
	WIN32_FIND_DATA		FileInformation;

	std::string firstFile;

	folder = SallyAPI::String::PathHelper::CorrectPath(folder);

	firstFile.append(folder);
	firstFile.append("*");

	hFile = FindFirstFile(firstFile.c_str(), &FileInformation);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((strcmp(FileInformation.cFileName, ".") != NULL) &&
				(strcmp(FileInformation.cFileName, "..") != NULL))
			{
				std::string filename;
				filename.append(folder);
				filename.append(FileInformation.cFileName);

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					m_vFolders.push_back(filename);
				else
					m_vFiles.push_back(filename);

				// add the write date
				m_vFoldersFilesDate[filename] = SallyAPI::File::FileHelper::FormatFileTime(FileInformation.ftLastWriteTime);
			}
		} while(FindNextFile(hFile, &FileInformation) == TRUE);
	}
	FindClose(hFile);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::OnCommandOpenFolder(std::string& folder)
///
/// \brief	Executes the command open folder action. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param [in,out]	folder	Pathname of the folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::OnCommandOpenFolder(std::string& folder)
{
	m_cLastCharSelected = ' ';

	m_pBreadcrumb->SetText(folder);

	FilewalkerAddFolder(folder);

	SortFiles();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SortFiles()
///
/// \brief	Sort files. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SortFiles()
{
	// sort the list
	if (m_pMenuSortName->GetCheckStatus())
	{
		std::sort(m_vFolders.begin(), m_vFolders.end(), SallyAPI::String::StringHelper::StringCompareCaseInsensitivity);
		std::sort(m_vFiles.begin(), m_vFiles.end(), SallyAPI::String::StringHelper::StringCompareCaseInsensitivity);
	}
	else
	{
		std::sort(m_vFolders.begin(), m_vFolders.end(), SallyAPI::String::CStringCompareWithArray(&m_vFoldersFilesDate));
		std::sort(m_vFiles.begin(), m_vFiles.end(), SallyAPI::String::CStringCompareWithArray(&m_vFoldersFilesDate));
	}

	m_pListViewFileWalker->Clear();

	// add files
	if (m_bShowSubfolders)
	{
		std::vector<std::string>::iterator	foldersIterator = m_vFolders.begin();

		while (foldersIterator != m_vFolders.end())
		{
			std::string folderName = (*foldersIterator);

			SallyAPI::GUI::CListViewItem listItem(folderName, "", m_iActionImage);
			listItem.SetText(SallyAPI::String::PathHelper::GetFileFromPath(folderName), 1);
			listItem.SetImageId(GUI_THEME_SALLY_ICON_FOLDER, 1);

			listItem.SetText(m_vFoldersFilesDate[folderName], 2);
			listItem.SetSmallFont(true, 2);

			m_pListViewFileWalker->AddItem(listItem, false);

			++foldersIterator;
		}
	}

	// add to the list
	std::vector<std::string>::iterator	filesIterator = m_vFiles.begin();
	while (filesIterator != m_vFiles.end())
	{
		std::string fileName = (*filesIterator);

		std::map<std::string, int>::iterator iterMimetype = m_mMimetypeList.begin();

		while (iterMimetype != m_mMimetypeList.end())
		{
			std::string mimetype = iterMimetype->first;
			int imageId = iterMimetype->second;

			if (SallyAPI::String::StringHelper::StringEndsWith(fileName, mimetype))
			{
				SallyAPI::GUI::CListViewItem listItem(fileName, "", m_iActionImage);
				listItem.SetText(SallyAPI::String::PathHelper::GetFileFromPath(fileName), 1);
				listItem.SetImageId(imageId, 1);

				listItem.SetText(m_vFoldersFilesDate[fileName], 2);
				listItem.SetSmallFont(true, 2);

				m_pListViewFileWalker->AddItem(listItem, false);
				iterMimetype = m_mMimetypeList.end();
			}
			else
			{
				++iterMimetype;
			}
		}
		if ((iterMimetype == m_mMimetypeList.end()) && (m_bShowUnkonwFiles))
		{
			SallyAPI::GUI::CListViewItem listItem(fileName, "", m_iActionImage);
			listItem.SetText(SallyAPI::String::PathHelper::GetFileFromPath(fileName), 1);
			listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_TEXT, 1);

			listItem.SetText(SallyAPI::File::FileHelper::GetFormatedFileWriteDate(fileName), 2);
			listItem.SetSmallFont(true, 2);

			m_pListViewFileWalker->AddItem(listItem, false);
		}
		++filesIterator;
	}

	m_pListViewFileWalker->UpdateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetActionButtonText(const std::string& text)
///
/// \brief	Sets an action button text. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetActionButtonText(const std::string& text)
{
	m_pButtonAction->SetText(text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetActionButtonImage(int icon)
///
/// \brief	Sets an action button image. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param	icon	The icon. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetActionButtonImage(int icon)
{
	m_pButtonAction->SetImageId(icon);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetActionButtonCommand(int command)
///
/// \brief	Sets an action button command. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
///
/// \param	command	The command. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetActionButtonCommand(int command)
{
	if (command == 0)
	{
		m_pButtonAction->Visible(false);
	}
	else
	{
		m_pButtonAction->Visible(true);
		
	}
	m_iActionCommand = command;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::OnCommandReset()
///
/// \brief	Executes the command reset action. 
///
/// \author	Christian Knobloch
/// \date	09.06.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::OnCommandReset()
{
	m_strCurrentFolderName = "";
	m_iFolderDepth = 0;
	m_pListViewFileWalker->Clear();
	m_pButtonGoUp->Enable(false);
	m_pButtonAction->Enable(false);
	m_pMenuSortName->Enable(false);
	m_pMenuSortDate->Enable(false);

	if ((!m_bShowRemovableDisk) && (!m_bShowHardDisks) && (!m_bShowSpecialFolders) && (m_vStartFolders.size() == 1))
	{
		std::vector<std::string>::iterator iter = m_vStartFolders.begin();
		std::string folder = *iter;

		OnCommandOpenFolder(folder);
		m_pBreadcrumb->SetText("");
		return;
	}

	m_pBreadcrumb->SetText("");

	std::vector<std::string>::iterator iter = m_vStartFolders.begin();
	while (iter != m_vStartFolders.end())
	{
		std::string folder = *iter;

		if (folder.length() > 0)
		{
			SallyAPI::GUI::CListViewItem listItem(folder, "", m_iActionImage);

			if (m_strMyDocument.compare(folder) == 0)
			{
				listItem.SetImageId(GUI_THEME_SALLY_ICON_HOME, 1);
				listItem.SetText("My Documents", 1);
				listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			}
			else if (m_strMyVideos.compare(folder) == 0)
			{
				listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO, 1);
				listItem.SetText("My Videos", 1);
				listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			}
			else if (m_strMyMusic.compare(folder) == 0)
			{
				listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_MP3, 1);
				listItem.SetText("My Music", 1);
				listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			}
			else if (m_strMyPictures.compare(folder) == 0)
			{
				listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_IMAGE, 1);
				listItem.SetText("My Pictures", 1);
				listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			}
			else if (m_strMyDesktop.compare(folder) == 0)
			{
				listItem.SetImageId(GUI_THEME_SALLY_ICON_DESKTOP, 1);
				listItem.SetText("My Desktop", 1);
				listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			}
			else
			{
				listItem.SetImageId(GUI_THEME_SALLY_ICON_FOLDER, 1);
				listItem.SetText(folder, 1);
				listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 1);
			}
			
			m_pListViewFileWalker->AddItem(listItem, false);
		}

		++iter;
	}

	// should we show the special folders?
	if (m_bShowSpecialFolders)
	{
		{
			SallyAPI::GUI::CListViewItem listItem(m_strMyDesktop, "", m_iActionImage);
			listItem.SetImageId(GUI_THEME_SALLY_ICON_DESKTOP, 1);
			listItem.SetText("My Desktop", 1);
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			m_pListViewFileWalker->AddItem(listItem, false);
		}

		{
			SallyAPI::GUI::CListViewItem listItem(m_strMyDocument, "", m_iActionImage);
			listItem.SetImageId(GUI_THEME_SALLY_ICON_HOME, 1);
			listItem.SetText("My Documents", 1);
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			m_pListViewFileWalker->AddItem(listItem, false);
		}

		{
			SallyAPI::GUI::CListViewItem listItem(m_strMyMusic, "", m_iActionImage);
			listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_MP3, 1);
			listItem.SetText("My Music", 1);
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			m_pListViewFileWalker->AddItem(listItem, false);
		}

		{
			SallyAPI::GUI::CListViewItem listItem(m_strMyVideos, "", m_iActionImage);
			listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO, 1);
			listItem.SetText("My Videos", 1);
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			m_pListViewFileWalker->AddItem(listItem, false);
		}

		{
			SallyAPI::GUI::CListViewItem listItem(m_strMyPictures, "", m_iActionImage);
			listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_IMAGE, 1);
			listItem.SetText("My Pictures", 1);
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
			m_pListViewFileWalker->AddItem(listItem, false);
		}
	}

	// should we show hard drives or removable disks?
	if ((m_bShowHardDisks) || (m_bShowRemovableDisk))
	{
		std::map<std::string, SallyAPI::File::DRIVE_TYPE> driveList = SallyAPI::File::FileHelper::GetDriveList();
		std::map<std::string, SallyAPI::File::DRIVE_TYPE>::iterator iter = driveList.begin();

		while (iter != driveList.end())
		{
			SallyAPI::File::DRIVE_TYPE driveType = iter->second;
			std::string driveLetter = iter->first;
			std::string driveName;

			char volumeName[MAX_PATH + 1];
			int result = GetVolumeInformation(driveLetter.c_str(), volumeName, MAX_PATH + 1, NULL, NULL, NULL, NULL, NULL);
			if (result != 0)
			{
				driveName.append(volumeName);
				driveName.append(" (");
			}
			driveName.append(driveLetter);
			if (result != 0)
				driveName.append(")");


			switch (driveType)
			{
			case SallyAPI::File::DRIVE_TYPE_CDROM:
				if (m_bShowRemovableDisk)
				{
					SallyAPI::GUI::CListViewItem listItem(driveLetter, "", m_iActionImage);
					listItem.SetImageId(GUI_THEME_SALLY_ICON_CD, 1);
					listItem.SetText(driveLetter, 1);
					listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
					m_pListViewFileWalker->AddItem(listItem, false);
				}
				break;
			case SallyAPI::File::DRIVE_TYPE_DVDROM:
				if (m_bShowRemovableDisk)
				{
					SallyAPI::GUI::CListViewItem listItem(driveLetter, "", m_iActionImage);
					listItem.SetImageId(GUI_THEME_SALLY_ICON_DVD, 1);
					listItem.SetText(driveLetter, 1);
					listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
					m_pListViewFileWalker->AddItem(listItem, false);
				}
				break;
			case SallyAPI::File::DRIVE_TYPE_REMOVABLE:
				if (m_bShowRemovableDisk)
				{
					SallyAPI::GUI::CListViewItem listItem(driveLetter, "", m_iActionImage);
					listItem.SetImageId(GUI_THEME_SALLY_ICON_USB, 1);
					listItem.SetText(driveLetter, 1);
					listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
					m_pListViewFileWalker->AddItem(listItem, false);
				}
				break;
			default:
				if (m_bShowHardDisks)
				{
					SallyAPI::GUI::CListViewItem listItem(driveLetter, "", m_iActionImage);
					listItem.SetImageId(GUI_THEME_SALLY_ICON_HD, 1);
					listItem.SetText(driveLetter, 1);
					listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE, 1);
					m_pListViewFileWalker->AddItem(listItem, false);
				}
				break;
			}
			++iter;
		}
	}

	m_pListViewFileWalker->UpdateView();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFileBrowser::SetFolder(std::string& folder, int folderDepth)
///
/// \brief	Sets a folder. 
///
/// \author	Christian Knobloch
/// \date	18.05.2011
///
/// \param [in,out]	folder	Pathname of the folder. 
/// \param	folderDepth		Depth of the folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFileBrowser::SetFolder(std::string& folder, int folderDepth)
{
	if (folderDepth == -1)
	{
		std::vector<std::string> list = SallyAPI::String::StringHelper::TokenizeString(folder, "\\");
		folderDepth = list.size() - 1;
	}

	m_pButtonGoUp->Enable(true);
	m_pButtonAction->Enable(true);
	m_pMenuSortName->Enable(true);
	m_pMenuSortDate->Enable(true);
	m_iFolderDepth = folderDepth;

	m_strCurrentFolderName = folder;

	OnCommandOpenFolder(folder);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CFileBrowser::GetCurrentFolderDepth()
///
/// \brief	Gets the current folder depth. 
///
/// \author	Christian Knobloch
/// \date	18.05.2011
///
/// \return	The current folder depth. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CFileBrowser::GetCurrentFolderDepth()
{
	return m_iFolderDepth;
}