////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\GameWindow.h
///
/// \brief	Declares the game Windows Form. 
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
#include "ApplicationWindow.h"
#include "Button.h"
#include "ImageBox.h"
#include "Processbar.h"
#include "ThreadStarter.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CGameWindow
		///
		/// \brief	Form for creating a game application.
		///
		/// \author	Christian Knobloch
		/// \date	08.09.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CGameWindow :
			public SallyAPI::GUI::CApplicationWindow
		{
		protected:
			SallyAPI::GUI::CThreadStarter*	m_ptStartLoading;

			SallyAPI::GUI::CForm*			m_pGameForm;
			SallyAPI::GUI::CForm*			m_pStartForm;
			SallyAPI::GUI::CButton*			m_pStartGame;
			SallyAPI::GUI::CImageBox*		m_pStartLogo;
			SallyAPI::GUI::CProcessbar*		m_pStartProcessbar;
			
			int			m_iLoadCount;

			void			OnCommandLoadGame();
			void			GameLoad();
			virtual void	GameLoadEx();

			void			OnCommandUnloadGame();
			void			GameUnload();
			virtual void	GameUnloadEx();
		public:
			CGameWindow(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
			virtual ~CGameWindow();

			virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual bool	IsGame();
		};
	}
}