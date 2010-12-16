////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Speech.cpp
///
/// \brief	Implements the speech class. 
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

#include "Speech.h"

using namespace SallyAPI::Speech;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSpeech::CSpeech()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSpeech::CSpeech()
{
	m_pVoice = NULL;
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_pVoice);
	
	if (hr != S_OK)
		m_pVoice = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSpeech::~CSpeech()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSpeech::~CSpeech()
{
	//SafeRelease(m_pVoice);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeech::Speak(const std::string& say)
///
/// \brief	Speaks. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	say	The say. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeech::Speak(const std::string& say)
{
	if (m_pVoice == NULL)
		return;

	WCHAR* theString;

	theString = (WCHAR*) malloc((say.length() + 1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, 0, (LPSTR) say.c_str(), -1, theString, MAX_PATH);
	m_pVoice->Speak(theString, SPF_DEFAULT, NULL);

	free(theString);
}