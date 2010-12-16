////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\InputBox.h
///
/// \brief	Declares the input box class. 
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
#include "DialogBox.h"
#include "Button.h"
#include "Edit.h"
#include "LabelBox.h"
#include "ParameterString.h"
#include "ParameterInputBox.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CInputBox
		///
		/// \brief	Input box. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CInputBox :
			public SallyAPI::GUI::CDialogBox
		{
		protected:
			SallyAPI::GUI::CButton*		m_pButtonOk;
			SallyAPI::GUI::CButton*		m_pButtonCancel;
			SallyAPI::GUI::CEdit*		m_pInput;
			SallyAPI::GUI::CLabelBox*	m_pLableDialogText;

			void OnCommandButtonOk();
		public:
			CInputBox(SallyAPI::GUI::CGUIBaseObject* parent);
			virtual ~CInputBox();

			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			void SetValues(SallyAPI::GUI::SendMessage::CParameterInputBox& inputBox);
		};
	}
}