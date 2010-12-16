////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\NumberSelector.h
///
/// \brief	Declares the number selector class. 
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
#include "Form.h"
#include "Edit.h"
#include "Button.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CNumberSelector
		///
		/// \brief	Number selector. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CNumberSelector :
			public SallyAPI::GUI::CForm
		{
		private:
			SallyAPI::GUI::CEdit*	m_pEdit;
			SallyAPI::GUI::CButton*	m_pButtonUp;
			SallyAPI::GUI::CButton*	m_pButtonDown;

			float	m_fStartTime;
			int		m_iMaxValue;
			int		m_iMinValue;
			int		m_iSteps;
			bool	m_iStepProcessed;

			void OnCommandEditChanged();
			void OnCommandButtonUp(int step);
			void OnCommandButtonDown(int step);
		public:
			CNumberSelector(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId = 0);
			virtual ~CNumberSelector();

			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void Enable(bool enable);
			virtual void Timer(float timeDelta);

			void SetValue(int value);
			int GetValue();

			int GetMinValue();
			int GetMaxValue();
			int GetSteps();
			void SetMinValue(int value);
			void SetMaxValue(int value);
			void SetSteps(int value);
		};
	}
}