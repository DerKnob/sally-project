////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterHoldClick.cpp
///
/// \brief	Implements the parameter hold click class. 
///
/// \author	Christian Knobloch
/// \date	26.05.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "ParameterHoldClick.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterHoldClick::CParameterHoldClick()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	26.05.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterHoldClick::CParameterHoldClick()
	:m_bHandled(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterHoldClick::~CParameterHoldClick()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	26.05.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterHoldClick::~CParameterHoldClick()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterHoldClick::SetHandled(bool handled)
///
/// \brief	Sets a handled. 
///
/// \author	Christian Knobloch
/// \date	26.05.2011
///
/// \param	handled	true to handled. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterHoldClick::SetHandled(bool handled)
{
	m_bHandled = handled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CParameterHoldClick::IsHandled()
///
/// \brief	Query if this object is handled. 
///
/// \author	Christian Knobloch
/// \date	26.05.2011
///
/// \return	true if handled, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CParameterHoldClick::IsHandled()
{
	return m_bHandled;
}