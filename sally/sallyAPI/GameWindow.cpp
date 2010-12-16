////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\GameWindow.cpp
///
/// \brief	Implements the game Windows Form. 
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

#include "GameWindow.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGameWindow::CGameWindow(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
/// const std::string& pluginPath) :SallyAPI::GUI::CApplicationWindow(parent, graphicId,
/// pluginPath), m_strPluginPath(pluginPath)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	pluginPath		Full pathname of the plugin file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CGameWindow::CGameWindow(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CApplicationWindow(parent, graphicId, pluginPath), m_iLoadCount(0)
{
	m_pGameForm = new SallyAPI::GUI::CForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGameForm->Visible(false);
	this->AddChild(m_pGameForm);

	m_pStartForm = new SallyAPI::GUI::CForm(this, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->AddChild(m_pStartForm);

	m_pStartGame = new SallyAPI::GUI::CButton(m_pStartForm, (WINDOW_WIDTH - 128) / 2, (WINDOW_HEIGHT - 178) / 2 + 128 + 20, 128, CONTROL_HEIGHT, MS_SALLY_GAME_START);
	m_pStartGame->SetText("Start");
	m_pStartGame->SetAlign(DT_CENTER | DT_VCENTER);
	m_pStartForm->AddChild(m_pStartGame);

	m_pStartLogo = new SallyAPI::GUI::CImageBox(m_pStartForm, (WINDOW_WIDTH - 128) / 2, (WINDOW_HEIGHT - 178) / 2, 128, 128);
	m_pStartLogo->SetImageId(GetGraphicId());
	m_pStartForm->AddChild(m_pStartLogo);

	m_pStartProcessbar = new SallyAPI::GUI::CProcessbar(this, (WINDOW_WIDTH - 200) / 2, WINDOW_HEIGHT - 80, 200);
	m_pStartProcessbar->Visible(false);
	this->AddChild(m_pStartProcessbar);

	// loading thread
	m_ptStartLoading = new SallyAPI::GUI::CThreadStarter(this, 0, MS_SALLY_GAME_START_THREAD);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGameWindow::~CGameWindow()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CGameWindow::~CGameWindow()
{
	m_ptStartLoading->WaitForStop();

	SafeDelete(m_ptStartLoading);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::IsGame()
///
/// \brief	Query if this object is game. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
///
/// \return	true if game, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CGameWindow::IsGame()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGameWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	iMessageID					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (iMessageID)
	{
	case MS_SALLY_GAME_START_THREAD:
		GameLoad();
		return;
	case GUI_CONTROL_BLENDED:
		if (reporter->GetAlphaBlending() == 0)
		{
			if ((reporter == m_pStartForm) || (reporter == m_pGameForm))
			{
				SallyAPI::GUI::CControl* control = dynamic_cast<SallyAPI::GUI::CControl*> (reporter);
				control->Visible(false);

				if (reporter == m_pStartForm)
				{
					++m_iLoadCount;
					if (m_iLoadCount == 2)
					{
						m_pGameForm->Enable(true);
						m_pGameForm->Visible(true);
						m_pGameForm->BlendAnimated(255, 500);
					}
				}
				if (reporter == m_pGameForm)
				{
					m_pStartForm->Enable(true);
					m_pStartForm->Visible(true);
					m_pStartForm->BlendAnimated(255, 500);
				}
			}
		}
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case MS_SALLY_GAME_START:
			OnCommandLoadGame();
			return;
		case MS_SALLY_GAME_STOP:
			OnCommandUnloadGame();
			return;
		}
		break;
	}

	CApplicationWindow::SendMessageToParent(reporter, reporterId, iMessageID, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGameWindow::OnCommandLoadGame()
///
/// \brief	Executes the command load game action. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameWindow::OnCommandLoadGame()
{
	m_iLoadCount = 0;

	m_ptStartLoading->Start();

	m_pStartForm->Enable(false);
	m_pStartForm->BlendAnimated(0, 500);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGameWindow::GameLoad()
///
/// \brief	Game load. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameWindow::GameLoad()
{
	m_pStartProcessbar->SetMaxPosition(26);
	m_pStartProcessbar->SetPosition(0);
	m_pStartProcessbar->Visible(true);

	GameLoadEx();

	m_pStartProcessbar->Visible(false);

	++m_iLoadCount;
	if (m_iLoadCount == 2)
	{
		m_pGameForm->Enable(true);
		m_pGameForm->Visible(true);
		m_pGameForm->BlendAnimated(255, 500);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGameWindow::GameLoadEx()
///
/// \brief	Game load ex. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameWindow::GameLoadEx()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGameWindow::OnCommandUnloadGame()
///
/// \brief	Executes the command unload game action. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameWindow::OnCommandUnloadGame()
{
	m_pGameForm->Enable(false);
	m_pGameForm->BlendAnimated(0, 500);

	GameUnload();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGameWindow::GameUnload()
///
/// \brief	Game unload. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameWindow::GameUnload()
{
	GameUnloadEx();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CGameWindow::GameUnloadEx()
///
/// \brief	Game unload ex. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameWindow::GameUnloadEx()
{
}