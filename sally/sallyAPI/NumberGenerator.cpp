////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\NumberGenerator.cpp
///
/// \brief	Implements the number generator class. 
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

#include "NumberGenerator.h"

SallyAPI::System::CNumberGenerator*	SallyAPI::System::CNumberGenerator::m_pObject = NULL;

using namespace SallyAPI::System;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CNumberGenerator::CNumberGenerator()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CNumberGenerator::CNumberGenerator()
{
	srand(TimeSeed());

	// gernerate the first number
	rand();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CNumberGenerator::~CNumberGenerator()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CNumberGenerator::~CNumberGenerator()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	unsigned CNumberGenerator::TimeSeed()
///
/// \brief	Time seed. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned CNumberGenerator::TimeSeed()
{
	time_t now = time ( 0 );
	unsigned char* p = (unsigned char *)&now;
	unsigned seed = 0;
	size_t i;

	for ( i = 0; i < sizeof now; i++ )
		seed = seed * ( UCHAR_MAX + 2U ) + p[i];

	return seed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CNumberGenerator::GenerateNumber()
///
/// \brief	Generates a number. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The number. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CNumberGenerator::GenerateNumber()
{
	return (int) (rand() + time(0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberGenerator::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberGenerator::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float CNumberGenerator::GetFloatNumber(float min, float max)
///
/// \brief	Gets a float number. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	min	The minimum. 
/// \param	max	The maximum. 
///
/// \return	The float number. 
////////////////////////////////////////////////////////////////////////////////////////////////////

float CNumberGenerator::GetFloatNumber(float min, float max)
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::System::CNumberGenerator();
	}

	int number = m_pObject->GenerateNumber();

	float result = min + (max - min) * ((float) number / RAND_MAX);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CNumberGenerator::GetNumber(int min, int max)
///
/// \brief	Gets a number. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	min	The minimum. 
/// \param	max	The maximum. 
///
/// \return	The number. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CNumberGenerator::GetNumber(int min, int max)
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::System::CNumberGenerator();
	}

	int number = m_pObject->GenerateNumber();

	int range = (max - min);
	int result = number % (range + 1);

	return result + min;
}