////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterPoint.cpp
///
/// \brief	Implements the parameter point class. 
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

#include "ParameterPoint.h"

using namespace SallyAPI::GUI::SendMessage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterPoint::CParameterPoint(int x, int y)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	x	The x coordinate. 
/// \param	y	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterPoint::CParameterPoint(int x, int y)
	:m_iX(x), m_iY(y)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CParameterPoint::~CParameterPoint()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CParameterPoint::~CParameterPoint()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterPoint::GetX()
///
/// \brief	Get x coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The x coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int	CParameterPoint::GetX()
{
	return m_iX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CParameterPoint::GetY()
///
/// \brief	Get y coordinate. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int	CParameterPoint::GetY()
{
	return m_iY;
}