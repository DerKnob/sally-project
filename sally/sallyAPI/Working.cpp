////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Working.cpp
///
/// \brief	Implements the working class. 
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

#include "Working.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWorking::CWorking(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y,
/// WorkingSize workingSize, bool rotation, int controlId) :SallyAPI::GUI::CImageBox(parent, x, y,
/// 22, 22, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	workingSize		Size of the working. 
/// \param	rotation		true to rotation. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CWorking::CWorking(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, WorkingSize workingSize, bool rotation, int controlId)
	:SallyAPI::GUI::CImageBox(parent, x, y, 22, 22, controlId)
{
	if (workingSize == WORKING_BIG)
	{
		this->Resize(128, 128);
		this->SetImageId(GUI_THEME_SALLY_WORKING_BIG);
	}
	else if (workingSize == WORKING_MEDIUM)
	{
		this->Resize(64, 64);
		this->SetImageId(GUI_THEME_SALLY_WORKING_BIG);
	}
	else
	{
		this->Resize(22, 22);
		this->SetImageId(GUI_THEME_SALLY_WORKING_SMALL);
	}
	if (rotation)
		this->RotateAnimatedY(1, true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWorking::~CWorking()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CWorking::~CWorking()
{
}
