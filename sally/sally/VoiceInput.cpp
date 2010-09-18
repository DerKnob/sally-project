////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\VoiceInput.cpp
///
/// \brief	Implements the voice input class. 
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

#include "VoiceInput.h"

inline HRESULT BlockForResult(ISpRecoContext * pRecoCtxt, ISpRecoResult ** ppResult, bool *pleaseStop)
{
	HRESULT hr = S_OK;
	CSpEvent event;

	while (SUCCEEDED(hr) &&
		SUCCEEDED(hr = event.GetFrom(pRecoCtxt)) &&
		hr == S_FALSE && *pleaseStop != true)
	{
		hr = pRecoCtxt->WaitForNotifyEvent(100);
	}
	if (*pleaseStop == true)
	{
		return NULL;
	}

	*ppResult = event.RecoResult();
	if (*ppResult)
	{
		(*ppResult)->AddRef();
	}

	return hr;
}

DWORD WINAPI RunVoiceInput (PVOID pvoid)
{
	USES_CONVERSION;

	volatile PPARAMS_VOICE_INPUT	pparams ;
	HRESULT hr;
	CComPtr<ISpRecoResult> cpResult;

	pparams = (PPARAMS_VOICE_INPUT) pvoid ;

	while ((SUCCEEDED(hr = BlockForResult(pparams->cpRecoCtxt, &cpResult, &(pparams->pleaseStop)))) && (!pparams->pleaseStop))
	{
		pparams->cpGrammar->SetDictationState( SPRS_INACTIVE );

		CSpDynamicString dstrText;

		if (SUCCEEDED(cpResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, 
			TRUE, &dstrText, NULL)))
		{
			pparams->commandList->push_back(W2A(dstrText));

			::SendMessage(pparams->notifyWindow, WM_COMMAND, pparams->notifyCmd, 0);

			cpResult.Release();
		}
		pparams->cpGrammar->SetDictationState( SPRS_ACTIVE );
	}

	pparams->running = false;
	ExitThread(0);
}

CVoiceInput::CVoiceInput(std::string lang, HWND notifyWindow, int notifyCmd)
{
	m_NotifyWindow = notifyWindow;
	m_NotifyCmd = notifyCmd;
	m_Init = false;
	m_Params.running = false;
	m_Params.pleaseStop = false;
	m_Params.commandList = &m_CommandList;
	m_cpRecoCtxt.CoCreateInstance(CLSID_SpSharedRecoContext);
}

CVoiceInput::~CVoiceInput()
{

}


bool CVoiceInput::Init()
{
	HRESULT hr;
	if (m_cpRecoCtxt &&
		SUCCEEDED(hr = m_cpRecoCtxt->SetNotifyWin32Event()) &&
		SUCCEEDED(hr = m_cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION))) &&
		SUCCEEDED(hr = m_cpRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL)) &&
		SUCCEEDED(hr = m_cpRecoCtxt->CreateGrammar(0, &m_cpGrammar)) &&
		SUCCEEDED(hr = m_cpGrammar->LoadDictation(NULL, SPLO_STATIC)) &&
		SUCCEEDED(hr = m_cpGrammar->SetDictationState(SPRS_ACTIVE)))
	{
		m_Params.cpRecoCtxt = m_cpRecoCtxt;
		m_Params.cpGrammar = m_cpGrammar;
		m_Params.notifyWindow = m_NotifyWindow;
		m_Params.notifyCmd = m_NotifyCmd;
		m_Init = true;
		return true;
	}
	return false;
}

bool CVoiceInput::Start()
{
	if ((m_Init) && (!m_Params.running))
	{
		m_Params.running = VOICE_INPUT_RUNNING;
		m_Params.pleaseStop = false;
		m_hThread = CreateThread (NULL, 0, RunVoiceInput, (PVOID) &m_Params, 0, 0);
		return true;
	}
	return false;
}

std::string CVoiceInput::GetCommand()
{
	if (m_CommandList.size() == 0)
	{
		return NULL;
	}
	
	std::string command = *m_CommandList.begin();
	m_CommandList.pop_front();
	return command;
}

void CVoiceInput::Stop()
{
	m_Params.pleaseStop = true;
	Sleep(1000);
}