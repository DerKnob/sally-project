////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\CritSection.cpp
///
/// \brief	Implements the crit section class. 
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

#include "CritSection.h"

using namespace SallyAPI::System;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCritSection::CCritSection()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCritSection::CCritSection()
{
	InitializeCriticalSection(&m_CritSec);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCritSection::~CCritSection()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCritSection::~CCritSection()
{
	DeleteCriticalSection(&m_CritSec);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCritSection::Lock()
///
/// \brief	Locks this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCritSection::Lock()
{
	EnterCriticalSection(&m_CritSec);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCritSection::Unlock()
///
/// \brief	Unlocks this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCritSection::Unlock()
{
	LeaveCriticalSection(&m_CritSec);
}