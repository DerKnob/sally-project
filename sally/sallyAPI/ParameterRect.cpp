////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterRect.cpp
///
/// \brief	Implements the parameter rectangle class. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
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

#include "ParameterRect.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterRect::CParameterRect()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterRect::CParameterRect()
{
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterRect::~CParameterRect()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterRect::~CParameterRect()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	RECT CParameterRect::GetRect()
///
/// \brief	Gets the rectangle. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
///
/// \return	The rectangle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

RECT CParameterRect::GetRect()
{
	return rect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CParameterRect::SetRect(RECT newRect)
///
/// \brief	Sets a rectangle. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
///
/// \param	newRect	The new rectangle. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CParameterRect::SetRect(RECT newRect)
{
	rect = newRect;
}
