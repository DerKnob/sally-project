////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\VoiceInput.h
///
/// \brief	Declares the voice input class. 
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
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Define.h"
#include <sapi.h>
#include <list>
#include <stdio.h>
#include <string.h>
#include <atlbase.h>
#include <sphelper.h>

//Stuktur für den Tread
typedef struct PARAMS_VOICE_INPUT
{
	bool					running;
	bool					pleaseStop;
	std::list<std::string>*	commandList;
	CComPtr<ISpRecoContext>	cpRecoCtxt;
	CComPtr<ISpRecoGrammar>	cpGrammar;
	int						notifyCmd;
	HWND					notifyWindow;
}PARAMS_VOICE_INPUT, *PPARAMS_VOICE_INPUT;

#define VOICE_INPUT_RUNNING true
#define VOICE_INPUT_STOP	false

class CVoiceInput
{
private:
	CComPtr<ISpRecoContext>		m_cpRecoCtxt;
	CComPtr<ISpRecoGrammar>		m_cpGrammar;
	
	std::list<std::string>		m_CommandList;
	int							m_NotifyCmd;
	HWND						m_NotifyWindow;
	HANDLE						m_hThread;
	bool						m_Init;
	PARAMS_VOICE_INPUT			m_Params;
	std::string					m_Lang;
public:
	CVoiceInput(std::string lang, HWND notifyWindow, int notifyCmd);
	virtual ~CVoiceInput();

	bool		GetState() { m_Params.running; }
	void		Stop();
	bool		Init();
	bool		Start();
	std::string	GetCommand();
};
