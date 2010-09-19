////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\3DCircle.h
///
/// \brief	Declares the 3 d circle class. 
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
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include "Button.h"
#include "Label.h"
#include "Form.h"
#include "ImageBox.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	C3DCircle
		///
		/// \brief	3 d circle. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY C3DCircle :
			public SallyAPI::GUI::CForm
		{
		private:
			std::vector<SallyAPI::GUI::CButton*>	m_ApplicationButtons;
			std::vector<SallyAPI::GUI::CLabel*>		m_ApplicationLabels;
			int		m_iIconSize;

			float	m_fWinkelCorrection;
			float	m_iWinkelSize;
			float	m_iWinkelStart;
			bool	m_bMovedAtTopX;
			bool	m_bMovedAtTopY;
			bool	m_bMoveAnimation;
			int		m_iMoveAnimationSpeed;
			float	m_fDeltaStart;
			float	m_fDeltaLastDifferents;
			float	m_fDeltaRemainder;

			virtual bool ProcessMouseMove(int x, int y);

			void	UpdateRotation();
			void	MoveMenuX(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	MoveMenuY(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	MoveMenuFastXLeft(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			void	MoveMenuFastXRight(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter);
			virtual bool ProcessMouseDown(int x, int y);
			virtual void AddChild(SallyAPI::GUI::CControl* control);
		public:
			C3DCircle(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int iconSize = 128);
			virtual ~C3DCircle();

			void	AddItem(int image, const std::string& text, int controlId);

			virtual void Timer(float timeDelta);
			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
		};
	}
}