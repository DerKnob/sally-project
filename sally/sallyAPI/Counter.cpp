////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Counter.cpp
///
/// \brief	Implements the counter class. 
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

#include "Counter.h"

using namespace SallyAPI::Core;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCounter::CCounter()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCounter::CCounter()
	:m_lNextTime(0), m_lLastTime(0), m_lCurrentTime(0), m_dwTimeCount(16), m_bPerformanceFlag(false), m_fTimeElapsed(0)
{
	if (QueryPerformanceFrequency((LARGE_INTEGER *) &m_lPerfCnt))
	{
		
		// yes, set m_time_count and timer choice flag 
		m_bPerformanceFlag		= TRUE;
		m_dwTimeCount	= (DWORD) (m_lPerfCnt / 60);
		QueryPerformanceCounter((LARGE_INTEGER *) &m_lNextTime); 
		m_dTimeScale		= 1.0 / m_lPerfCnt;
	}
	else
	{
		m_lNextTime		= timeGetTime(); 
		m_dTimeScale	= 0.001;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCounter::~CCounter()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCounter::~CCounter()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCounter::GetCurrentTime()
///
/// \brief	Gets the current time. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCounter::GetCurrentTime()
{
	if (m_bPerformanceFlag)
		QueryPerformanceCounter((LARGE_INTEGER *) &m_lCurrentTime); 
	else 
		m_lCurrentTime = timeGetTime(); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CCounter::RenderNextFrame()
///
/// \brief	Renders a next frame. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCounter::RenderNextFrame()
{
	return (m_lCurrentTime > m_lNextTime);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCounter::CalculateElapsedTime()
///
/// \brief	Calculates the elapsed time. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCounter::CalculateElapsedTime()
{
	// calculate elapsed time
	m_fTimeElapsed = (float) ((m_lCurrentTime - m_lLastTime) * m_dTimeScale);
	if (m_fTimeElapsed > 5.0)
		m_fTimeElapsed = 5.0;

	// save frame time
	m_lLastTime = m_lCurrentTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCounter::CalculateNextFrame()
///
/// \brief	Calculates the next frame. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCounter::CalculateNextFrame()
{
	// set time for next frame 
	m_lNextTime = m_lCurrentTime + m_dwTimeCount; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float CCounter::GetElapsedTime()
///
/// \brief	Gets the elapsed time. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The elapsed time. 
////////////////////////////////////////////////////////////////////////////////////////////////////

float CCounter::GetElapsedTime()
{
	return m_fTimeElapsed;
}